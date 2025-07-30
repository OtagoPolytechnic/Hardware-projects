#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>
#include <CCS811.h>

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

unsigned char payload[4]; // Adjusted for CO2 and TVOC
static osjob_t sendjob;

// Pin mapping for Arduino Duo with Dragino LoRa Shield
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

const unsigned TX_INTERVAL = 600;

CCS811 sensor;

void initializeSensor() {
    while(sensor.begin() != 0) {
        Serial.println("Failed to initialize CCS811, check connection.");
        delay(1000);
    }
    sensor.setMeasCycle(sensor.eCycle_250ms);
}

void setup() {
    Serial.begin(9600);
    initializeSensor();

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Start job 
    do_send(&sendjob);
}

void onEvent(ev_t ev) {
  Serial.println(ev);
    switch(ev) {
        case EV_JOINED:
            LMIC_setLinkCheckMode(0);
            break;
        case EV_TXCOMPLETE:
            os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
            break;
        default:
            break;
    }
}

void do_send(osjob_t* j) {
    if (LMIC.opmode & OP_TXRXPEND) {
        return;
    }

    // Read and process CO2 and TVOC data
    unsigned long startTime = millis();
    while (!sensor.checkDataReady()) {
        if (millis() - startTime > 10000) { // 10 seconds timeout
            Serial.println("Timeout waiting for CCS811 data.");
            return; // Exit if data is not ready
        }
        delay(100); // Short delay
    }

    uint16_t co2 = sensor.getCO2PPM();
    uint16_t tvoc = sensor.getTVOCPPB();

    // Prepare payload (2 bytes for CO2 + 2 bytes for TVOC)
    payload[0] = lowByte(co2);
    payload[1] = highByte(co2);
    payload[2] = lowByte(tvoc);
    payload[3] = highByte(tvoc);

    // Send payload over LoRaWAN
    LMIC_setTxData2(1, payload, sizeof(payload), 0);
}

void loop() {
    os_runloop_once();
}
