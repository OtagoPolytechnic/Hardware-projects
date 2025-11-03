
/*******************************************************************************
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 * Copyright (c) 2018 Thomas Laurenson
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *
 * This example sends a valid LoRaWAN packet with payload "Hello,
 * world!", using frequency and encryption settings matching those of
 * the The Things Network.
 *
 * This uses ABP (Activation-by-personalisation), where a DevAddr and
 * Session keys are preconfigured (unlike OTAA, where a DevEUI and
 * application key is configured, while the DevAddr and session keys are
 * assigned/generated in the over-the-air-activation procedure).
 *
 * Note: LoRaWAN per sub-band duty-cycle limitation is enforced (1% in
 * g1, 0.1% in g2), but not the TTN fair usage policy (which is probably
 * violated by this sketch when left running for longer)!
 *
 * To use this sketch, first register your application and device with
 * the things network, to set or generate a DevAddr, NwkSKey and
 * AppSKey. Each device should have their own unique values for these
 * fields.
 *
 * Do not forget to define the radio type correctly in config.h.
 *
 *******************************************************************************/

#include <lmic.h>
#include <hal/hal.h>
//#include <SPI.h>
//#include <elapsedMillis.h>
#include "Adafruit_SleepyDog.h"
#include "RFM69.h"

// include the DHT22 Sensor Library
#include "DHT.h"

// DHT digital pin and sensor type
#define DHTPIN 10
#define DHTTYPE DHT22
#define VBATPIN A9

// LoRaWAN NwkSKey, network session key
static const PROGMEM u1_t NWKSKEY[16] = { FILLMEIN };

// LoRaWAN AppSKey, application session key
static const u1_t PROGMEM APPSKEY[16] = { FILLMEIN };

// LoRaWAN end-device address (DevAddr)
// See http://thethingsnetwork.org/wiki/AddressSpace
// The library converts the address to network byte order as needed.
static const u4_t DEVADDR = FILLMEIN; // <-- Change this address for every node!

// payload to send to TTN gateway
//static uint8_t payload[5];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 300;

// Pin mapping
// TL Modifications:
// Specifically for Arduino Uno + Dragino LoRa Shield US 915MHz
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {7, 6, LMIC_UNUSED_PIN},
    .rxtx_rx_active = 0,
    .rssi_cal = 8,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
    .spi_freq = 8000000,
};

// init. DHT
DHT dht(DHTPIN, DHTTYPE);

void onEvent (ev_t ev) {

  RFM69 radio;
    //Serial.print(os_getTime());
    //Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            //Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            //Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            //Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            //Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            //Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            //Serial.println(F("EV_JOINED"));
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
            //Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            //Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            //Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              //Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              //Serial.println(F("Received "));
              //Serial.println(LMIC.dataLen);
              //Serial.println(F(" bytes of payload"));
            }
          digitalWrite(13, HIGH);
          delay(1000);    
          digitalWrite(13, LOW);  
          delay(1000); 
          radio.sleep();
          Watchdog.sleep(TX_INTERVAL*1000);

            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            //Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            //Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            //Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            //Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            //Serial.println(F("EV_LINK_ALIVE"));
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
            //Serial.println(F("EV_TXSTART"));
            break;
        default:
            //Serial.print(F("Unknown event: "));
            //Serial.println((unsigned) ev);
            break;
    }
}

void do_send(osjob_t* j){
     // Start timing how long since starting to send data
      //elapsedMillis sinceWake = 0;
      uint8_t payload[32];
      char temp[15];
      char hum[15];
      char bat[15];
      char packet[32] = "";
        
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        //Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // read the temperature from the DHT22
        float temperature = dht.readTemperature();
        //Serial.print("Temperature: "); Serial.print(temperature);
        //Serial.println(" *C");

        // read the humidity from the DHT22
        float humidity = dht.readHumidity();
        //Serial.print("Humidity: ");
        //Serial.println(humidity);
        
        // Get the battery voltage
        float measuredvbat = analogRead(VBATPIN);
        measuredvbat *= 2;    // we divided by 2, so multiply back
        measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
        measuredvbat /= 1024; // convert to voltage
        //Serial.print("VBat: " ); Serial.println(measuredvbat);

        //add temperature data to send
        dtostrf(temperature, 3, 2, temp);
        //strcat(packet, "T: ");
        strcat(packet, temp);
        //strcat(packet, "*C, ");
        strcpy((char *)payload, packet);

        //add humidity data to send
        dtostrf(humidity, 3, 2, hum);
        //strcat(packet, "H: ");
        strcat(packet, hum);
        //strcat(packet, "%, ");
        strcpy((char *)payload, packet);

//        //add battery data to send
//        dtostrf(measuredvbat, 3, 2, bat);
//        //strcat(packet, "B: ");
//        strcat(packet, bat);
//        //strcat(packet, "V");
//        strcpy((char *)payload, packet);
//     

        // prepare upstream data transmission at the next possible time.
        // transmit on port 1 (the first parameter); you can use any value from 1 to 223 (others are reserved).
        // don't request an ack (the last parameter, if not zero, requests an ack from the network).
        // Remember, acks consume a lot of network resources; don't ask for an ack unless you really need it.
        LMIC_setTxData2(1, payload, sizeof(payload)-1, 0);
        
        //Serial.println(F("Packet queued"));
        //Serial.print(F("Sending packet on frequency: "));
        //Serial.println(LMIC.freq);

         // Output sleep time
           
//        int sleepSeconds = TX_INTERVAL;
//        sleepSeconds -= sinceWake/1000;
//        Serial.print("Sleeping for ");
//        Serial.print(sleepSeconds);
//        Serial.println(" seconds");
//        delay(500); // time for Serial send buffer to clear
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup() {
 
    delay(5000);
    //Comment this line out if using battery
    //while (! Serial);
    Serial.begin(115200);
    //Serial.println(F("Starting..."));

    #ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    //delay(1000);
    #endif

    dht.begin();
  
    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    // Set static session parameters. Instead of dynamically establishing a session
    // by joining the network, precomputed session parameters are be provided.
    #ifdef PROGMEM
    // On AVR, these values are stored in flash and only copied to RAM
    // once. Copy them to a temporary buffer here, LMIC_setSession will
    // copy them into a buffer of its own again.
    uint8_t appskey[sizeof(APPSKEY)];
    uint8_t nwkskey[sizeof(NWKSKEY)];
    memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
    memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
    LMIC_setSession (0x13, DEVADDR, nwkskey, appskey);
    #else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession (0x13, DEVADDR, NWKSKEY, APPSKEY);
    #endif

    #if defined(CFG_eu868)
    // Set up the channels used by the Things Network, which corresponds
    // to the defaults of most gateways. Without this, only three base
    // channels from the LoRaWAN specification are used, which certainly
    // works, so it is good for debugging, but can overload those
    // frequencies, so be sure to configure the full frequency range of
    // your network here (unless your network autoconfigures them).
    // Setting up channels should happen after LMIC_setSession, as that
    // configures the minimal channel set.
    LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
    LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
    LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
    // TTN defines an additional channel at 869.525Mhz using SF9 for class B
    // devices' ping slots. LMIC does not have an easy way to define set this
    // frequency and support for class B is spotty and untested, so this
    // frequency is not configured here.
    #elif defined(CFG_us915)
    // NA-US channels 0-71 are configured automatically
    // but only one group of 8 should (a subband) should be active
    // TTN recommends the second sub band, 1 in a zero based count.
    // https://github.com/TheThingsNetwork/gateway-conf/blob/master/US-global_conf.json
    LMIC_selectSubBand(1);
    // TL Modifications:
    // Specify to operate on AU915 sub-band 2
    #elif defined(CFG_au921)
    //Serial.println(F("Loading AU915/AU921 Configuration..."));
    // Set to AU915 sub-band 2
    LMIC_selectSubBand(1); 
    #endif

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF7,14);
    
    // Start job
    do_send(&sendjob);
}

void loop() {
  
 os_runloop_once();
}