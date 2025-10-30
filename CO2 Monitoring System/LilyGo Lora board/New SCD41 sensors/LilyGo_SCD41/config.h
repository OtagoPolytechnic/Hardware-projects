#ifndef _CONFIG_H
#define _CONFIG_H

#include <RadioLib.h>
#include <SPI.h>


// How often to send an uplink - consider legal & FUP constraints - see notes
const uint32_t uplinkIntervalSeconds = 5UL * 60UL;    // minutes x seconds

// JoinEUI - previous versions of LoRaWAN called this AppEUI
// for development purposes you can use all zeros - see wiki for details
#define RADIOLIB_LORAWAN_JOIN_EUI  0x0000000000000000

// The Device EUI & two keys can be generated on the TTN console 
#ifndef RADIOLIB_LORAWAN_DEV_EUI   // Replace with your Device EUI
#define RADIOLIB_LORAWAN_DEV_EUI   0x70B3D57ED006EBB1
#endif
#ifndef RADIOLIB_LORAWAN_APP_KEY   // Replace with your App Key 
#define RADIOLIB_LORAWAN_APP_KEY    0x7D, 0xFC, 0x2E, 0x1E, 0x29, 0xC3, 0x6F, 0x39, 0x8D, 0x55, 0x6C, 0x01, 0x30, 0x4A, 0xD7, 0x3F
#endif
#ifndef RADIOLIB_LORAWAN_NWK_KEY   // Put your Nwk Key here
#define RADIOLIB_LORAWAN_NWK_KEY   0x12, 0x42, 0x8B, 0xF0, 0x8F, 0xA4, 0x17, 0x6E, 0x08, 0xA7, 0x24, 0x2D, 0x33, 0xE5, 0x50, 0x1A
#endif

// For the curious, the #ifndef blocks allow for automated testing &/or you can
// put your EUI & keys in to your platformio.ini - see wiki for more tips



// Regional choices: EU868, US915, AU915, AS923, IN865, KR920, CN780, CN500
const LoRaWANBand_t Region = AU915;
const uint8_t subBand = 2;  // For US915, change this to 2, otherwise leave on 0


// ============================================================================
// Below is to support the sketch - only make changes if the notes say so ...

// Auto select MCU <-> radio connections
// If you get an error message when compiling, it may be that the 
// pinmap could not be determined - see the notes for more info



// LilyGo 

SX1276 radio = new Module(18, 26, 23, 33);



// Copy over the EUI's & keys in to the something that will not compile if incorrectly formatted
uint64_t joinEUI =   RADIOLIB_LORAWAN_JOIN_EUI;
uint64_t devEUI  =   RADIOLIB_LORAWAN_DEV_EUI;
uint8_t appKey[] = { RADIOLIB_LORAWAN_APP_KEY };
uint8_t nwkKey[] = { RADIOLIB_LORAWAN_NWK_KEY };

// Create the LoRaWAN node
LoRaWANNode node(&radio, &Region, subBand);


// Helper function to display any issues
void debug(bool isFail, const __FlashStringHelper* message, int state, bool Freeze) {
  if (isFail) {
    Serial.print(message);
    Serial.print("(");
    Serial.print(state);
    Serial.println(")");
    while (Freeze);
  }
}

// Helper function to display a byte array
void arrayDump(uint8_t *buffer, uint16_t len) {
  for (uint16_t c = 0; c < len; c++) {
    Serial.printf("%02X", buffer[c]);
  }
  Serial.println();
}


#endif