/*
Payload formatter for TTN

function Decoder(bytes, port) {
  let decoded = {};

  if (bytes.length !== 4) {
    decoded.error = "Invalid payload length";
    return decoded;
  }

  let tempRaw = (bytes[3] << 8) | bytes[2];
  let co2Raw = (bytes[1] << 8) | bytes[0];

  let temperature = tempRaw / 100.0;
  let CO2 = co2Raw;

  decoded.receivedString = `${CO2}:${temperature}\u0000\u0000`;
  return decoded;
}


*/
#if !defined(ESP32)
#pragma error ("This is not the example your device is looking for - ESP32 only")
#endif

#include <Preferences.h>
Preferences store;

#include "config.h" 
#include <Wire.h>
#include <SPI.h>
#include <RadioLib.h>


// RTC data to preserve across deep sleep
RTC_DATA_ATTR uint16_t bootCount = 0;
RTC_DATA_ATTR uint16_t bootCountSinceUnsuccessfulJoin = 0;
RTC_DATA_ATTR uint8_t LWsession[RADIOLIB_LORAWAN_SESSION_BUF_SIZE];

// Payload buffer (6 bytes for CO2, Temp)
unsigned char payload[4];

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER)
    Serial.println(F("Wake from sleep"));
  else {
    Serial.print(F("Wake not caused by deep sleep: "));
    Serial.println(wakeup_reason);
  }

  Serial.print(F("Boot count: "));
  Serial.println(++bootCount);
}

void gotoSleep(uint32_t seconds) {
  esp_sleep_enable_timer_wakeup(seconds * 1000UL * 1000UL);
  Serial.println(F("Sleeping...\n"));
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println(F("Sleep failed, restarting in 5 minutes..."));
  delay(5UL * 60UL * 1000UL);
  ESP.restart();
}

int16_t lwActivate() {
  int16_t state = RADIOLIB_ERR_UNKNOWN;
  node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  Serial.println(F("Recalling LoRaWAN nonces & session"));
  store.begin("radiolib");
  if (store.isKey("nonces")) {
    uint8_t buffer[RADIOLIB_LORAWAN_NONCES_BUF_SIZE];
    store.getBytes("nonces", buffer, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);
    state = node.setBufferNonces(buffer);
    state = node.setBufferSession(LWsession);
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("Restored session - activating"));
      state = node.activateOTAA();
      store.end();
      return state;
    }
  } else Serial.println(F("No nonces saved - starting fresh."));

  // Join loop
  while (state != RADIOLIB_LORAWAN_NEW_SESSION) {
    Serial.println(F("Joining LoRaWAN..."));
    state = node.activateOTAA();
    Serial.println(F("Saving nonces to flash"));
    uint8_t buffer[RADIOLIB_LORAWAN_NONCES_BUF_SIZE];
    uint8_t *persist = node.getBufferNonces();
    memcpy(buffer, persist, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);
    store.putBytes("nonces", buffer, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);

    if (state != RADIOLIB_LORAWAN_NEW_SESSION) {
      Serial.print(F("Join failed: "));
      Serial.println(state);
      gotoSleep(10);
    }
  }

  Serial.println(F("Joined LoRaWAN"));
  bootCountSinceUnsuccessfulJoin = 0;
  delay(1000);
  store.end();
  return state;
}

/* ----------------------------- SCD41 Functions ----------------------------- */

void scd41_sendCommand(uint16_t cmd) {
  Wire.beginTransmission(0x62);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission();
}

bool scd41_readMeasurement(uint16_t &co2, float &temperature, float &humidity) {
  Wire.beginTransmission(0x62);
  Wire.write(0xEC);
  Wire.write(0x05);
  Wire.endTransmission();

  delay(5); // small wait before reading

  Wire.requestFrom(0x62, (uint8_t)9);
  if (Wire.available() < 9) return false;

  uint16_t co2Raw = (Wire.read() << 8) | Wire.read(); Wire.read();
  uint16_t tempRaw = (Wire.read() << 8) | Wire.read(); Wire.read();
  uint16_t humRaw = (Wire.read() << 8) | Wire.read(); Wire.read();

  co2 = co2Raw;
  temperature = -45 + 175 * ((float)tempRaw / 65535.0);
  humidity = 100 * ((float)humRaw / 65535.0);
  return true;
}

/* --------------------------------------------------------------------------- */

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("\nSetup"));
  print_wakeup_reason();

  Wire.begin();
  delay(2000);

  Serial.println(F("Waking up SCD41..."));
  scd41_sendCommand(0x3646); // stop periodic measurement
  delay(10);
  scd41_sendCommand(0x21B1); // start periodic measurement
  delay(5000); // wait for first measurement to be ready (~5s)

  // Init Radio
  SPI.begin(5, 19, 27);
  delay(500);
  Serial.println(F("Initialize radio"));
  int16_t state = radio.begin();
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print(F("Radio init failed: "));
    Serial.println(state);
  }

  state = lwActivate();

  Serial.println(F("Reading SCD41 data..."));
  uint16_t co2;
  float temperature, humidity;

  bool ok = false;
  unsigned long start = millis();
  while (millis() - start < 10000) {
    if (scd41_readMeasurement(co2, temperature, humidity)) {
      ok = true;
      break;
    }
    delay(1000);
  }

  if (!ok) {
    Serial.println(F("Data not ready — check power or sensor state"));
    gotoSleep(30);
  }

  Serial.print(F("CO2: ")); Serial.print(co2);
  Serial.print(F(" ppm, Temp: ")); Serial.print(temperature, 2);
  Serial.print(F(" °C, Humidity: ")); Serial.print(humidity, 2);
  Serial.println(F(" %"));

  // Encode into payload (scaled for compact transmission)
  uint16_t payloadCO2 = co2;
  uint16_t payloadTemp = (uint16_t)(temperature * 100);

  payload[0] = lowByte(payloadCO2);
  payload[1] = highByte(payloadCO2);
  payload[2] = lowByte(payloadTemp);
  payload[3] = highByte(payloadTemp);


  Serial.print(F("Payload: "));
  for (int i = 0; i < 4; i++) {
    Serial.print(payload[i], HEX); Serial.print(" ");
  }
  Serial.println();

  Serial.println(F("Sending uplink..."));
  state = node.sendReceive(payload, sizeof(payload));
  if (state < RADIOLIB_ERR_NONE)
    Serial.print(F("Send error: ")), Serial.println(state);

  Serial.print(F("FCntUp: "));
  Serial.println(node.getFCntUp());

  uint8_t *persist = node.getBufferSession();
  memcpy(LWsession, persist, RADIOLIB_LORAWAN_SESSION_BUF_SIZE);

  gotoSleep(uplinkIntervalSeconds);
}

void loop() {}
