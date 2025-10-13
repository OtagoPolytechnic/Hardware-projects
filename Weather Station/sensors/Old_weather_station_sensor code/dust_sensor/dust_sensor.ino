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

// Pin mapping for Arduino Duo with Dragino LoRa Shield
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

// 10 minute delay
const unsigned TX_INTERVAL = 600;

int measurePin = A5;
int ledPower = 12;

unsigned int samplingTime = 280;
unsigned int deltaTime = 40;
unsigned int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup() {
  pinMode(ledPower,OUTPUT);
  Serial.begin(9600);

  // LMIC init
  os_init();

  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  // Start job 
  do_send(&sendjob);
}

void onEvent (ev_t ev) {
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
  else {
    // Read and process dust sensor data
    digitalWrite(ledPower, LOW);
    delayMicroseconds(samplingTime);

    voMeasured = analogRead(measurePin);

    delayMicroseconds(deltaTime);
    digitalWrite(ledPower, HIGH);
    delayMicroseconds(sleepTime);
    
    calcVoltage = voMeasured*(5.0/1024);
    dustDensity = 0.17*calcVoltage-0.1;

    if ( dustDensity < 0)
    {
      dustDensity = 0.00;
    }

    // Prepare payload
    uint16_t payloadDust = (uint16_t)(dustDensity * 100); 
    payload[0] = lowByte(payloadDust); 
    payload[1] = highByte(payloadDust); 

    // Send payload over LoRaWAN
    LMIC_setTxData2(1, payload, sizeof(payload), 0);
  }
}

void loop() {
  os_runloop_once();
}
