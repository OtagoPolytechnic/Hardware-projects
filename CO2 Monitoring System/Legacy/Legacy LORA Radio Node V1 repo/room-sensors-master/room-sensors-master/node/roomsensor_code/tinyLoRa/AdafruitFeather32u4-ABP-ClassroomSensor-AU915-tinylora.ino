
// TinyLoRa DHT22 - ABP TTN Packet Sender (Multi-Channel)
// Tutorial Link: https://learn.adafruit.com/the-things-network-for-feather/using-a-feather-32u4
//
// Copyright 2015, 2016 Ideetron B.V.
//
// Modified by Brent Rubell for Adafruit Industries, 2018
// Modified by Henry Greenslade for Otago Polytechnic, 2019
/************************** Configuration ***********************************/
#include <SPI.h>
#include <Adafruit_SleepyDog.h>
#include "DHT.h"
#include "RH_RF95.h"
#include <TinyLoRa.h>

// Visit your thethingsnetwork.org device console to create an account, or if you need your session keys.
// Network Session Key (MSB)
uint8_t NwkSkey[16] = { FILL ME IN };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { FILL ME IN };

// Device Address (MSB)
uint8_t DevAddr[4] = { FILL ME IN };

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 600;       // 10 minute delay

/************************** Example Begins Here ***********************************/
// Data Packet to Send to TTN
unsigned char loraData[32];

// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(7, 8 ,4);

//Radiohead library for sleep only
RH_RF95 radio;

// Pin the DHT22 is connected to
#define DHTPIN 10
DHT dht(DHTPIN, DHT22);

// Battery voltage reading
#define VBATPIN A9

void setup()
{



  // 10 second window to flash new sketch
  Serial.begin(9600);
  delay(1000);
  Serial.println("Open to new sketch for 10 seconds!");
  delay(9000);

  // Initialize pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize LoRa
  Serial.print("Starting LoRa... ");

  // define multi-channel sending
  lora.setChannel(MULTI);

  // set datarate
  lora.setDatarate(SF7BW125);
  if(!lora.begin())
  {
    Serial.println("Failed");
    Serial.println("Check your radio");
    while(true);
  }
  Serial.println("OK");

  Serial.print("\nTX_INTERVAL: ");
  Serial.print(TX_INTERVAL);
  Serial.println(" sec");

  // Optional set transmit power. If not set default is +17 dBm.
  // Valid options are: -80, 1 to 17, 20 (dBm).
  // For safe operation in 20dBm: your antenna must be 3:1 VWSR or better
  // and respect the 1% duty cycle.
  lora.setPower(1);
}

void loop()
{
  // Initialize DHT
  dht.begin();

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(500);   // Stops freakout when DHT bad
    return;
  }

  // Battery voltage reading
  float v = analogRead(VBATPIN);
  v *= 2;    // we divided by 2, so multiply back
  v *= 3.3;  // Multiply by 3.3V, our reference voltage
  v /= 1024; // convert to voltage

  // DEBUG Values
  Serial.println("\nSensor values:");
  Serial.print("Battery Voltage: " );
  Serial.print(v);
  Serial.println(" V");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" C");

  // encode float as int
  //int16_t tempInt = round(t * 100);
  //int16_t humidInt = round(h * 100);
  //int16_t battInt = round(v * 100);

  // encode int as bytes
  //loraData[0] = highByte(tempInt);
  //loraData[1] = lowByte(tempInt);
  //loraData[2] = highByte(humidInt);
  //loraData[3] = lowByte(humidInt);
  //loraData[4] = highByte(battInt);
  //loraData[5] = lowByte(battInt);

  char temp[15];
  char hum[15];
  char bat[15];
  char packet[32] = "";

  //add temperature data to send
  dtostrf(t, 3, 2, temp);
  strcat(packet, "T: ");
  strcat(packet, temp);
  strcat(packet, "*C, ");
  strcpy((char *)loraData, packet);

  //add humidity data to send
  dtostrf(h, 3, 2, hum);
  strcat(packet, "H: ");
  strcat(packet, hum);
  strcat(packet, "%, ");
  strcpy((char *)loraData, packet);

  //add battery data to send
  dtostrf(v, 3, 2, bat);
  strcat(packet, "B: ");
  strcat(packet, bat);
  strcat(packet, "V");
  strcpy((char *)loraData, packet);

  Serial.println("\nSending LoRa Data...");
  Serial.println("Serial will die now. Don't expect more output. ");

  // Send data
  lora.sendData(loraData, sizeof(loraData)-1, lora.frameCounter);
  lora.frameCounter++;

  // Blink LED to indicate packet sent
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);

  // Sleep
  radio.sleep();
  for (int i = 0; i < (TX_INTERVAL/5); i++)
  {
    Watchdog.sleep(5000);   // Sleep for 5 seconds
  }
}
