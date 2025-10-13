/*
Payload formatter for TTN

function decodeUplink(input) {
  let bytes = input.bytes;

  if (bytes.length < 6) {
    return {
      errors: ["Invalid payload length"]
    };
  }

  // Decode fixed-point values
  let windSpeed = (bytes[1] << 8 | bytes[0]) / 100.0;  // scaled ×100
  let rain = (bytes[3] << 8 | bytes[2]);               // scaled ×1
  let windDir = (bytes[5] << 8 | bytes[4]);            // scaled ×1

  return {
    data: {
      windSpeed_kmh: windSpeed,
      rain_mm: rain,
      windDirection_deg: windDir
    }
  };
}

*/
#if !defined(ESP32)
#pragma error ("This is not the example your device is looking for - ESP32 only")
#endif

#include <Preferences.h>
Preferences store;

#include "config.h" 
#include <RadioLib.h>
#include <Wire.h>
#include <SPI.h>

// RTC data to preserve across deep sleep
RTC_DATA_ATTR uint16_t bootCount = 0;
RTC_DATA_ATTR uint16_t bootCountSinceUnsuccessfulJoin = 0;
RTC_DATA_ATTR uint8_t LWsession[RADIOLIB_LORAWAN_SESSION_BUF_SIZE];

unsigned char payload[6];
unsigned char payloadR[6];

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
  Wire.begin();
  delay(2000);
  Serial.println(F("\nSetup"));
  print_wakeup_reason();


  int16_t state = 0;
  delay(500);
  SPI.begin(5, 19, 27);
  delay(500);

  Serial.println(F("Initialize the radio"));
  state = radio.begin();
  debug(state != RADIOLIB_ERR_NONE, F("Initialize radio failed"), state, true);

  state = lwActivate();

  Serial.println(F("Sending uplink"));

  if (Wire.requestFrom(0x08, 6) == 6) {
      Wire.readBytes(payloadR, 6);
      Serial.print("Received Data: ");
      for (int i = 0; i < 6; i++) {
        Serial.print(payloadR[i], HEX);
        Serial.print(" ");
      }
      Serial.println();


      // Encode and send wind data and rain
      float WindSpeed = decodeTemperature(payloadR[0], payloadR[1]);
      float Rain = decodeTemperature(payloadR[2], payloadR[3]);
      float WindDir = decodeCO2(payloadR[4], payloadR[5]);

      uint16_t payloadWindSpeed = encodeFixedPoint100(WindSpeed);
      uint16_t payloadRain = encodeFixedPoint100(Rain);
      uint16_t payloadWindDir = encodeFixedPoint1(WindDir);

      payload[0] = lowByte(payloadWindSpeed);
      payload[1] = highByte(payloadWindSpeed);
      payload[2] = lowByte(payloadRain);
      payload[3] = highByte(payloadRain);
      payload[4] = lowByte(payloadWindDir);
      payload[5] = highByte(payloadWindDir);

      Serial.print("Payload Data: ");
      for (int i = 0; i < 6; i++) {
        Serial.print(payload[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      Serial.println(F("Sending wind/rain"));
    } else {
      Serial.println("Failed to read from I2C device");
      Serial.println(F("Sending nothing"));
    }

  state = node.sendReceive(payload, sizeof(payload));
  debug((state < RADIOLIB_ERR_NONE) && (state != RADIOLIB_ERR_NONE), F("Error in sendReceive"), state, false);

  Serial.print(F("FCntUp: "));
  Serial.println(node.getFCntUp());

  uint8_t *persist = node.getBufferSession();
  memcpy(LWsession, persist, RADIOLIB_LORAWAN_SESSION_BUF_SIZE);

  gotoSleep(uplinkIntervalSeconds);
}


uint16_t encodeFixedPoint100(float value) {
  // Convert float to fixed-point (scale by 100 for 2 decimal places)
  int16_t fixedPointValue = (int16_t)(value * 100);
  return (uint16_t)fixedPointValue;
}

uint16_t encodeFixedPoint1(float value) {
  // Convert float to fixed-point (scale by 1 for 0 decimal places)
  int16_t fixedPointValue = (int16_t)(value);
  return (uint16_t)fixedPointValue;
}

// Function to decode temperature from the first two bytes of payload
float decodeTemperature(uint8_t tempLow, uint8_t tempHigh) {
  // Combine the two bytes into a single 16-bit integer
  int16_t fixedPointTemp = (tempHigh << 8) | tempLow;
  // Convert back to float by dividing by 100 to account for the fixed-point scaling
  return (float)fixedPointTemp / 100.0;
}

// Function to decode CO2 from the next two bytes of payload
float decodeCO2(uint8_t CO2Low, uint8_t CO2High) {
  // Combine the two bytes into a single 16-bit integer
  int16_t fixedPointCO2 = (CO2High << 8) | CO2Low;
  // Convert back to float by dividing by 1 (no scaling applied for CO2)
  return (float)fixedPointCO2;
}


void loop() {}
