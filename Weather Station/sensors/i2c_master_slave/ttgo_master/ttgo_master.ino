/*
Payload formatter for TTN

function Decoder(bytes, port) {
  if (bytes.length !== 10) {
    return null; // Invalid payload length
  }

  let decoded = {};

    let tempRaw = (bytes[1] << 8) | bytes[0];
    let pressRaw = (bytes[3] << 8) | bytes[2];
    let windSpeedRaw = (bytes[5] << 8) | bytes[4];
    let rainRaw = (bytes[7] << 8) | bytes[6];
    let windDirRaw = (bytes[9] << 8) | bytes[8];

    let temperature = (tempRaw & 0x8000) ? -(0x10000 - tempRaw) : tempRaw;
    let pressure = (pressRaw & 0x8000) ? -(0x10000 - pressRaw) : pressRaw;
    let windSpeed = (windSpeedRaw & 0x8000) ? -(0x10000 - windSpeedRaw) : windSpeedRaw;
    let rain = (rainRaw & 0x8000) ? -(0x10000 - rainRaw) : rainRaw;
    let windDir = (windDirRaw & 0x8000) ? -(0x10000 - windDirRaw) : windDirRaw;
    
    decoded.temperature = temperature / 100.0; // Assuming the temp is in hundredths of degrees
    decoded.pressure = pressure; // Pressure in Pa or other unit
    decoded.windSpeed = windSpeed / 100.0; // Assuming the wind is in hundredths of m/s
    decoded.rain = rain; // rain in mm
    decoded.windDir = windDir; // Assuming the temp is in hundredths of degrees

  return decoded;
}
*/

#include <SPL06-007.h>
#include <Wire.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>
#include <arduino_lmic_hal_boards.h>

// LoRaWAN settings
static const u1_t PROGMEM APPEUI[8] = { FILL ME IN };
void os_getArtEui(u1_t* buf) {
  memcpy_P(buf, APPEUI, 8);
}

static const u1_t PROGMEM DEVEUI[8] = { FILL ME IN };
void os_getDevEui(u1_t* buf) {
  memcpy_P(buf, DEVEUI, 8);
}

static const u1_t PROGMEM APPKEY[16] = { FILL ME IN };
void os_getDevKey(u1_t* buf) {
  memcpy_P(buf, APPKEY, 16);
}

// Variables to track time
unsigned long previousMillis = 0;
const unsigned long resetInterval = 43200000;  // 12 hours in milliseconds / 24 hours : 86400000
unsigned long currentMilis = 0;

unsigned char payload[11];
unsigned char payloadR[11];
static osjob_t sendjob;
const unsigned TX_INTERVAL = 600;

// State variable to track which payload to send
int payloadState = 0;  // 0 = temperature/pressure, 1 = temperature/CO2

void setup() {
  delay(5000);              //Wait for everything to be powered up before initializing anything

  Wire.begin();
  Serial.begin(9600);
  Serial.println(F("Starting"));
  SPL_init(); // Setup initial SPL chip registers

  // LMIC init using the computed target
  const lmic_pinmap* pPinMap = Arduino_LMIC::GetPinmap_ThisBoard();

  // don't die mysteriously; die noisily.
  if (pPinMap == nullptr) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) {
      // flash lights, sleep.
      for (int i = 0; i < 5; ++i) {
        digitalWrite(LED_BUILTIN, 1);
        delay(100);
        digitalWrite(LED_BUILTIN, 0);
        delay(900);
      }
      Serial.println(F("board not known to library; add pinmap or update getconfig_thisboard.cpp"));
    }
  }

  os_init_ex(pPinMap);

  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  Serial.println(F("LMIC reset done"));

  LMIC_setLinkCheckMode(0);
  LMIC_setDrTxpow(DR_SF7, 14);
  LMIC_selectSubBand(1);

  delay(5000);                          // wait for the sensors to settle before starting anything

  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void printHex2(unsigned v) {
  v &= 0xff;
  if (v < 16)
    Serial.print('0');
  Serial.print(v, HEX);
}

void onEvent(ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      {
        u4_t netid = 0;
        devaddr_t devaddr = 0;
        u1_t nwkKey[16];
        u1_t artKey[16];
        LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
        Serial.print("netid: ");
        Serial.println(netid, DEC);
        Serial.print("devaddr: ");
        Serial.println(devaddr, HEX);
        Serial.print("AppSKey: ");
        for (size_t i = 0; i < sizeof(artKey); ++i) {
          if (i != 0)
            Serial.print("-");
          printHex2(artKey[i]);
        }
        Serial.println("");
        Serial.print("NwkSKey: ");
        for (size_t i = 0; i < sizeof(nwkKey); ++i) {
          if (i != 0)
            Serial.print("-");
          printHex2(nwkKey[i]);
        }
        Serial.println();
      }
      // Disable link check validation (automatically enabled
      // during join, but because slow data rates change max TX
      // size, we don't use it in this example.
      LMIC_setLinkCheckMode(0);
      break;
    /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
      }

      // Schedule next transmission
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
    case EV_TXSTART:
      Serial.println(F("EV_TXSTART"));
      break;
    case EV_TXCANCELED:
      Serial.println(F("EV_TXCANCELED"));
      break;
    case EV_RXSTART:
      /* do not print anything -- it wrecks timing */
      break;
    case EV_JOIN_TXCOMPLETE:
      Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
      break;

    default:
      Serial.print(F("Unknown event: "));
      Serial.println((unsigned)ev);
      break;
  }
}

void do_send(osjob_t* j) {
  
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Error handling for I2C request
    if (Wire.requestFrom(0x08, 7) == 7) {
      Wire.readBytes(payloadR, 7);
      Serial.print("Received Data: ");
      for (int i = 0; i < 7; i++) {
        Serial.print(payloadR[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      // Encode and send temperature and pressure
      float temperature = get_temp_c();
      float pressure = get_pressure();

      Serial.println(temperature);
      Serial.println(pressure);
      uint16_t payloadTemp = encodeFixedPoint100(temperature);
      uint16_t payloadPressure = encodeFixedPoint1(pressure);

      // Encode and send wind data and rain
      float WindSpeed = decodeTemperature(payloadR[0], payloadR[1]);
      float Rain = decodeCO2(payloadR[2], payloadR[3]);
      float WindDir = decodeCO2(payloadR[4], payloadR[5]);

      uint16_t payloadWindSpeed = encodeFixedPoint100(WindSpeed);
      uint16_t payloadRain = encodeFixedPoint1(Rain);
      uint16_t payloadWindDir = encodeFixedPoint1(WindDir);

      payload[0] = lowByte(payloadTemp);
      payload[1] = highByte(payloadTemp);
      payload[2] = lowByte(payloadPressure);
      payload[3] = highByte(payloadPressure);
      payload[4] = lowByte(payloadWindSpeed);
      payload[5] = highByte(payloadWindSpeed);
      payload[6] = lowByte(payloadRain);
      payload[7] = highByte(payloadRain);
      payload[8] = lowByte(payloadWindDir);
      payload[9] = highByte(payloadWindDir);

      Serial.print("Payload Data: ");
      for (int i = 0; i < 10; i++) {
        Serial.print(payload[i], HEX);
        Serial.print(" ");
      }
      Serial.println();

      LMIC_setTxData2(1, payload, 10, 0);  // Send on Fport 1
      Serial.println(F("Sending wind/rain"));
    } else {
      Serial.println("Failed to read from I2C device");
      LMIC_setTxData2(1, NULL, 0, 0);  // Send on Fport 1
      Serial.println(F("Sending nothing"));
    }
  }
}

void loop() {
  currentMilis = millis();

  // Check if 12 hours have passed
  if (currentMilis >= resetInterval) {
    Serial.println("Restarting ESP32 after 12 hours...");
    ESP.restart();  // Restart the ESP32
  }

  os_runloop_once();  // Continue LMIC task processing
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
