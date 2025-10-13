#if !defined(ESP32)
#pragma error ("This is not the example your device is looking for - ESP32 only")
#endif

#include <Preferences.h>
Preferences store;

#include "config.h" 
#include <RadioLib.h>
#include <DHT.h>  // Include the Adafruit DHT library

// RTC data to preserve across deep sleep
RTC_DATA_ATTR uint16_t bootCount = 0;
RTC_DATA_ATTR uint16_t bootCountSinceUnsuccessfulJoin = 0;
RTC_DATA_ATTR uint8_t LWsession[RADIOLIB_LORAWAN_SESSION_BUF_SIZE];

// DHT Configuration
#define DHTPIN 25        // GPIO pin where DHT is connected
#define DHTTYPE DHT11   // DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println(F("Wake from sleep"));
  } else {
    Serial.print(F("Wake not caused by deep sleep: "));
    Serial.println(wakeup_reason);
  }

  Serial.print(F("Boot count: "));
  Serial.println(++bootCount);
}

void gotoSleep(uint32_t seconds) {
  esp_sleep_enable_timer_wakeup(seconds * 1000UL * 1000UL); // function uses uS
  Serial.println(F("Sleeping\n"));
  Serial.flush();

  esp_deep_sleep_start();

  Serial.println(F("\n\n### Sleep failed, delay of 5 minutes & then restart ###\n"));
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
    debug(state != RADIOLIB_ERR_NONE, F("Restoring nonces buffer failed"), state, false);

    state = node.setBufferSession(LWsession);
    debug((state != RADIOLIB_ERR_NONE) && (bootCount > 1), F("Restoring session buffer failed"), state, false);

    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("Successfully restored session - now activating"));
      state = node.activateOTAA();
      debug((state != RADIOLIB_LORAWAN_SESSION_RESTORED), F("Failed to activate restored session"), state, true);

      store.end();
      return state;
    }
  } else {
    Serial.println(F("No Nonces saved - starting fresh."));
  }

  state = RADIOLIB_ERR_NETWORK_NOT_JOINED;
  while (state != RADIOLIB_LORAWAN_NEW_SESSION) {
    Serial.println(F("Join ('login') to the LoRaWAN Network"));
    state = node.activateOTAA();

    Serial.println(F("Saving nonces to flash"));
    uint8_t buffer[RADIOLIB_LORAWAN_NONCES_BUF_SIZE];
    uint8_t *persist = node.getBufferNonces();
    memcpy(buffer, persist, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);
    store.putBytes("nonces", buffer, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);

    if (state != RADIOLIB_LORAWAN_NEW_SESSION) {
      Serial.print(F("Join failed: "));
      Serial.println(state);

      uint32_t sleepForSeconds = 10;
      Serial.print(F("Boots since unsuccessful join: "));
      Serial.println(bootCountSinceUnsuccessfulJoin);
      Serial.print(F("Retrying join in "));
      Serial.print(sleepForSeconds);
      Serial.println(F(" seconds"));

      gotoSleep(sleepForSeconds);
    }
  }

  Serial.println(F("Joined"));
  bootCountSinceUnsuccessfulJoin = 0;
  delay(1000);

  store.end();
  return state;
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  delay(2000);
  Serial.println(F("\nSetup"));
  print_wakeup_reason();

  dht.begin();  // Initialize the DHT sensor

  int16_t state = 0;
  delay(500);
  SPI.begin(5, 19, 27);
  delay(500);

  Serial.println(F("Initialize the radio"));
  state = radio.begin();
  debug(state != RADIOLIB_ERR_NONE, F("Initialize radio failed"), state, true);

  state = lwActivate();

  Serial.println(F("Sending uplink"));

  // Read temperature and humidity from the DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    gotoSleep(60);  // Retry after 60 seconds
  }

  // Build payload byte array
  uint8_t uplinkPayload[2];
  uplinkPayload[0] = (uint8_t)(temperature * 10); // Scaled temperature
  uplinkPayload[1] = (uint8_t)(humidity * 10);    // Scaled humidity

  state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload));
  debug((state < RADIOLIB_ERR_NONE) && (state != RADIOLIB_ERR_NONE), F("Error in sendReceive"), state, false);

  Serial.print(F("FCntUp: "));
  Serial.println(node.getFCntUp());

  uint8_t *persist = node.getBufferSession();
  memcpy(LWsession, persist, RADIOLIB_LORAWAN_SESSION_BUF_SIZE);

  gotoSleep(uplinkIntervalSeconds);
}

void loop() {}
