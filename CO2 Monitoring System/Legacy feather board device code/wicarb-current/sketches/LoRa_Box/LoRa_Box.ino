/************************** Configuration ***********************************/
#include "TinyLoRa.h"
#include <SPI.h>
#include "MHZ19.h"

const int rx_pin = 3; //Serial rx pin no
const int tx_pin = 4; //Serial tx pin no
MHZ19 *mhz19_uart = new MHZ19(rx_pin,tx_pin);

// Visit your thethingsnetwork.org device console if you need your session keys
uint8_t DevAddr[4] = { /* FILL FROM TTN */ };
uint8_t NwkSkey[16] = { /* FILL FROM TTN */ };
uint8_t AppSkey[16] = { /* FILL FROM TTN */ };

// Data Packet to Send to TTN
unsigned char loraData[8];

// How many times data transfer should occur, in seconds
const unsigned int sendInterval = 120;

// Pinout for radio
TinyLoRa lora = TinyLoRa(2, 10, 9);

void setup()
{
  delay(2000);
  Serial.begin(9600);

  mhz19_uart->begin(rx_pin, tx_pin);
  mhz19_uart->setAutoCalibration(false);
  
  // Initialize pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Initialize LoRa
  Serial.print("Starting LoRa...");
  // define multi-channel sending
  lora.setChannel(MULTI);
  // set datarate
  lora.setDatarate(SF8BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }
  Serial.println("OK");
  delay(5000);
  lora.frameCounter++; // TTN doesn't seem to like frame 0, so let's start at 1
}

void loop()
{
  measurement_t m = mhz19_uart->getMeasurement();

  Serial.print("CO2 PPM: ");
  Serial.println(m.co2_ppm);
  Serial.print("Temperature: ");
  Serial.println(m.temperature);

  if ((m.co2_ppm < 0) || (m.co2_ppm == 410))
  {
    Serial.println("Sensor Not Ready!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
  }
  else 
  {
    String temp = (String(m.co2_ppm)+":"+String(m.temperature));
    temp.toCharArray(loraData, 8);
    
    Serial.println("Sending LoRa Data...");
    lora.sendData(loraData, sizeof(loraData), lora.frameCounter);
    Serial.print("Frame Counter: ");Serial.println(lora.frameCounter);
    lora.frameCounter++;
  
    // blink LED to indicate packet sent
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    
    Serial.println("delaying...");
    for(int i = 0; i < sendInterval; i++)
    {
      delay(1000);
    }
  }
}
