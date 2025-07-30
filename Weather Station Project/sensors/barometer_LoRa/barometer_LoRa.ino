#include <SPL06-007.h>
#include <Wire.h>

#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>

// LoRaWAN settings
static const u1_t PROGMEM APPEUI[8] = { /*Fill me in*/ };
void os_getArtEui(u1_t* buf) {
  memcpy_P(buf, APPEUI, 8);
}

static const u1_t PROGMEM DEVEUI[8] = { /*Fill me in*/ };
void os_getDevEui(u1_t* buf) {
  memcpy_P(buf, DEVEUI, 8);
}

static const u1_t PROGMEM APPKEY[16] = { /*Fill me in*/ };
void os_getDevKey(u1_t* buf) {
  memcpy_P(buf, APPKEY, 16);
}

unsigned char payload[5];
static osjob_t sendjob;

const unsigned TX_INTERVAL = 30;

// Pin mapping for TTGO ESP32
const lmic_pinmap lmic_pins = {
  .nss = 18,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 14,
  .dio = { 26, 33, 32 }  // Pins for the Heltec ESP32 Lora board/ TTGO Lora32 with 3D metal antenna
};

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println(F("Starting"));

  // LMIC init
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  SPL_init();
  // Start job (sending automatically starts OTAA too)
  do_send(&sendjob);
}

void onEvent(ev_t ev) {
  switch (ev) {
    case EV_JOINED:
      LMIC_setLinkCheckMode(0);
      break;
    case EV_TXCOMPLETE:
      if (LMIC.txrxFlags & TXRX_ACK)
      os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
      break;
    default:
      break;
  }
}
void do_send(osjob_t* j) {
  // Check if there is not a current TX/RX job running
  if (LMIC.opmode & OP_TXRXPEND) {
    //Serial.println(F("OP_TXRXPEND, not sending"));
  } else {
    // Read sensor data
    float temperature = get_temp_c();
    float pressure = get_pressure();

    //Serial.print("Temp: ");
    //Serial.println(temperature);
    //Serial.print("Press: ");
    //Serial.println(pressure);
    
    uint16_t payloadTemp = encodeFixedPoint100(temperature);
    uint16_t payloadPressure = encodeFixedPoint1(pressure);

    // Convert to bytes
    byte tempLow = lowByte(payloadTemp);
    byte tempHigh = highByte(payloadTemp);
    byte PressureLow = lowByte(payloadPressure);
    byte PressureHigh = highByte(payloadPressure);

     // Fill the payload
    payload[0] = tempLow;
    payload[1] = tempHigh;
    payload[2] = PressureLow;
    payload[3] = PressureHigh;

    // Prepare upstream data transmission at the next possible time
    LMIC_setTxData2(1, payload, sizeof(payload) - 1, 0);
  }
  // Next TX is scheduled after TX_COMPLETE event.
}

void loop() {
  os_runloop_once();
}

uint16_t encodeFixedPoint100(float value) {
  // Convert float to fixed-point (scale by 100 for 2 decimal places)
  int16_t fixedPointValue = (int16_t)(value * 100);
  return (uint16_t)fixedPointValue;
}

uint16_t encodeFixedPoint1(float value) {
  // Convert float to fixed-point (scale by 100 for 2 decimal places)
  int16_t fixedPointValue = (int16_t)(value);
  return (uint16_t)fixedPointValue;
}
