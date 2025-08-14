/*
  RadioLib LoRaWAN DHT Temperature & Humidity Sensor

  This example joins a LoRaWAN network and sends temperature and humidity
  data from a DHT11/DHT22 sensor. Before you start, you will have to
  register your device at https://www.thethingsnetwork.org/

  Hardware connections:
  - DHT sensor VCC to 3.3V
  - DHT sensor GND to GND  
  - DHT sensor DATA to GPIO pin (defined as DHT_PIN below)
  - 4.7k resistor between DATA and VCC (pull-up resistor)

  Data payload format (5 bytes):
  - Byte 0: Temperature integer part (°C)
  - Byte 1: Temperature decimal part (*10)
  - Byte 2: Humidity integer part (%)
  - Byte 3: Humidity decimal part (*10)
  - Byte 4: Battery level (0-254) or sensor status
*/

#include "config.h"

// Include libraries
#include <RadioLib.h>
#include <Preferences.h>
#include <DHT.h>

// DHT sensor configuration
#define DHT_PIN 4        // GPIO pin connected to DHT sensor data line
#define DHT_TYPE DHT22   // Change to DHT11 if using DHT11 sensor

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Preferences for persistent storage
Preferences store;

void setup() {
  Serial.begin(921600);
  while (!Serial)
    ;           // Wait for serial to be initialised
  delay(5000);  // Give time to switch to the serial monitor
  Serial.println(F("\nDHT LoRaWAN Sensor Setup"));

  // Initialize DHT sensor
  Serial.println(F("Initializing DHT sensor..."));
  dht.begin();
  delay(2000); // Give DHT sensor time to stabilize

  // Test DHT sensor
  float testTemp = dht.readTemperature();
  float testHum = dht.readHumidity();
  
  if (isnan(testTemp) || isnan(testHum)) {
    Serial.println(F("ERROR: Failed to read from DHT sensor! Check wiring."));
    Serial.println(F("- VCC to 3.3V"));
    Serial.println(F("- GND to GND"));
    Serial.print(F("- DATA to GPIO "));
    Serial.println(DHT_PIN);
    Serial.println(F("- 4.7k resistor between DATA and VCC"));
    while(1) delay(1000); // Stop execution
  } else {
    Serial.println(F("DHT sensor OK!"));
    Serial.print(F("Initial readings - Temp: "));
    Serial.print(testTemp);
    Serial.print(F("°C, Humidity: "));
    Serial.print(testHum);
    Serial.println(F("%"));
  }

  int16_t state = 0;  // return value for calls to RadioLib

  Serial.println(F("Initialise the radio"));
  state = radio.begin();
  debug(state != RADIOLIB_ERR_NONE, F("Initialise radio failed"), state, true);

  // Override the default join rate
  uint8_t joinDR = 4;

  // Setup the OTAA session information
  node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

  Serial.println(F("Join ('login') the LoRaWAN Network"));
  // Setup the flash storage
  store.begin("radiolib");
  
  // If we have previously saved nonces, restore them and try to restore session as well
  if (store.isKey("nonces")) {
    uint8_t buffer[RADIOLIB_LORAWAN_NONCES_BUF_SIZE];
    store.getBytes("nonces", buffer, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);
    state = node.setBufferNonces(buffer);
    debug(state != RADIOLIB_ERR_NONE, F("Restoring nonces buffer failed"), state, false);

    // Recall session from RTC deep-sleep preserved variable
    state = node.setBufferSession(LWsession);

    debug((state != RADIOLIB_ERR_NONE), F("Restoring session buffer failed"), state, false);

    // If Nonces and Session restored successfully, activation is just a formality
    if (state == RADIOLIB_ERR_NONE) {
      Serial.println(F("Successfully restored session - now activating"));
      state = node.activateOTAA();
      debug((state != RADIOLIB_LORAWAN_SESSION_RESTORED), F("Failed to activate restored session"), state, true);

      store.end();
    }
  } else {
    Serial.println(F("No Nonces saved - starting fresh."));
  }

  // If we got here, there was no session to restore, so start trying to join
  uint32_t sleepForSeconds = 10 * 1000;
  state = RADIOLIB_ERR_NETWORK_NOT_JOINED;

  while (state != RADIOLIB_LORAWAN_NEW_SESSION) {
    Serial.println(F("Join ('login') to the LoRaWAN Network"));

    state = node.activateOTAA();

    // Save the join counters (nonces) to permanent store
    Serial.println(F("Saving nonces to flash"));
    uint8_t buffer[RADIOLIB_LORAWAN_NONCES_BUF_SIZE];
    uint8_t *persist = node.getBufferNonces();
    memcpy(buffer, persist, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);
    store.putBytes("nonces", buffer, RADIOLIB_LORAWAN_NONCES_BUF_SIZE);

    if (state != RADIOLIB_LORAWAN_NEW_SESSION) {
      Serial.print(F("Join failed: "));
      Serial.println(state);

      Serial.print(F("Retrying join in "));
      Serial.print(sleepForSeconds);
      Serial.println(F(" seconds"));

      delay(sleepForSeconds);
    }
  }

  // Print the DevAddr
  Serial.print("[LoRaWAN] DevAddr: ");
  Serial.println((unsigned long)node.getDevAddr(), HEX);

  // Configure LoRaWAN settings
  node.setADR(true);           // Enable ADR
  node.setDatarate(5);         // Set datarate
  node.setDutyCycle(true, 1250); // Fair use policy
  node.setDwellTime(true, 400);  // Dwell time limits

  Serial.println(F("DHT LoRaWAN Sensor Ready!\n"));
}

void loop() {
  int16_t state = RADIOLIB_ERR_NONE;

  // Set battery level (you can read actual battery voltage here)
  uint8_t battLevel = 200; // Assume good battery for now
  node.setDeviceStatus(battLevel);

  // Read DHT sensor
  Serial.println(F("Reading DHT sensor..."));
  float temperature = dht.readTemperature(); // Celsius
  float humidity = dht.readHumidity();       // Percentage

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println(F("ERROR: Failed to read DHT sensor!"));
    // Send error status instead of sensor data
    temperature = -999;  // Error indicator
    humidity = -999;     // Error indicator
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.print(F("°C, Humidity: "));
    Serial.print(humidity);
    Serial.println(F("%"));
  }

  // Prepare payload
  uint8_t uplinkPayload[5];
  
  if (temperature == -999) {
    // Sensor error - send error payload
    uplinkPayload[0] = 0xFF; // Error marker for temperature
    uplinkPayload[1] = 0xFF; 
    uplinkPayload[2] = 0xFF; // Error marker for humidity
    uplinkPayload[3] = 0xFF;
    uplinkPayload[4] = 0x00; // Error status
    Serial.println(F("Sending error payload"));
  } else {
    // Convert float values to fixed-point integers
    // Temperature: split into integer and decimal parts
    int16_t tempInt = (int16_t)temperature;
    uint8_t tempDec = (uint8_t)((temperature - tempInt) * 10);
    
    // Humidity: split into integer and decimal parts  
    uint8_t humInt = (uint8_t)humidity;
    uint8_t humDec = (uint8_t)((humidity - humInt) * 10);
    
    // Handle negative temperatures
    if (temperature < 0) {
      tempInt = abs(tempInt);
      uplinkPayload[0] = tempInt | 0x80; // Set MSB to indicate negative
    } else {
      uplinkPayload[0] = (uint8_t)tempInt;
    }
    
    uplinkPayload[1] = tempDec;
    uplinkPayload[2] = humInt;
    uplinkPayload[3] = humDec;
    uplinkPayload[4] = 0x01; // Good reading status
    
    Serial.print(F("Payload: Temp="));
    Serial.print(temperature);
    Serial.print(F("°C ("));
    Serial.print(uplinkPayload[0]);
    Serial.print(F("."));
    Serial.print(uplinkPayload[1]);
    Serial.print(F("), Hum="));
    Serial.print(humidity);
    Serial.print(F("% ("));
    Serial.print(uplinkPayload[2]);
    Serial.print(F("."));
    Serial.print(uplinkPayload[3]);
    Serial.println(F(")"));
  }

  uint8_t downlinkPayload[10];
  size_t downlinkSize;
  LoRaWANEvent_t uplinkDetails;
  LoRaWANEvent_t downlinkDetails;

  uint8_t fPort = 10; // Application port

  // Retrieve the last uplink frame counter
  uint32_t fCntUp = node.getFCntUp();

  // Send uplink
  Serial.println(F("Sending DHT data uplink"));
  if (fCntUp == 1) {
    Serial.println(F("Requesting LinkCheck and DeviceTime"));
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_LINK_CHECK);
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_DEVICE_TIME);
    state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload), fPort, downlinkPayload, &downlinkSize, true, &uplinkDetails, &downlinkDetails);
  } else {
    state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload), fPort, downlinkPayload, &downlinkSize, false, &uplinkDetails, &downlinkDetails);
  }
  
  debug(state < RADIOLIB_ERR_NONE, F("Error in sendReceive"), state, false);

  // Handle downlink
  if (state > 0) {
    Serial.println(F("Received a downlink"));
    if (downlinkSize > 0) {
      Serial.println(F("Downlink data: "));
      arrayDump(downlinkPayload, downlinkSize);
      
      // You can add downlink command processing here
      // For example, changing sensor reading interval
    } else {
      Serial.println(F("<MAC commands only>"));
    }

    // Print signal quality info
    Serial.print(F("[LoRaWAN] RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    Serial.print(F("[LoRaWAN] SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // Print additional downlink details
    Serial.println(F("[LoRaWAN] Event information:"));
    Serial.print(F("[LoRaWAN] Datarate:\t"));
    Serial.println(downlinkDetails.datarate);
    Serial.print(F("[LoRaWAN] Frequency:\t"));
    Serial.print(downlinkDetails.freq, 3);
    Serial.println(F(" MHz"));
    Serial.print(F("[LoRaWAN] Time-on-air: \t"));
    Serial.print(node.getLastToA());
    Serial.println(F(" ms"));

    // Handle MAC command responses
    uint8_t margin = 0;
    uint8_t gwCnt = 0;
    if (node.getMacLinkCheckAns(&margin, &gwCnt) == RADIOLIB_ERR_NONE) {
      Serial.print(F("[LoRaWAN] LinkCheck margin:\t"));
      Serial.println(margin);
      Serial.print(F("[LoRaWAN] LinkCheck count:\t"));
      Serial.println(gwCnt);
    }

    uint32_t networkTime = 0;
    uint8_t fracSecond = 0;
    if (node.getMacDeviceTimeAns(&networkTime, &fracSecond, true) == RADIOLIB_ERR_NONE) {
      Serial.print(F("[LoRaWAN] DeviceTime Unix:\t"));
      Serial.println(networkTime);
    }

  } else {
    Serial.println(F("[LoRaWAN] No downlink received"));
  }

  // Calculate delay until next transmission
  uint32_t minimumDelay = uplinkIntervalSeconds * 1000UL;
  uint32_t interval = node.timeUntilUplink();
  uint32_t delayMs = max(interval, minimumDelay);

  Serial.print(F("[LoRaWAN] Next reading in "));
  Serial.print(delayMs / 1000);
  Serial.println(F(" seconds\n"));

  delay(delayMs);
}