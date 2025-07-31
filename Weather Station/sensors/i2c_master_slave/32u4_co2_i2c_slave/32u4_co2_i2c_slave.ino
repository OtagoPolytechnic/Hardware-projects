/*
 * Modified by : Bee Kaan 
 * Last updated : 22/05/2020
 * Original Code from https://electronics.stackexchange.com/questions/262473/mh-z19-co2-sensor-giving-diferent-values-using-uart-and-pwm
 * Datasheet Link for reference : https://gitlab.op-bit.nz/BIT/Project/Internet-Of-Things/nodes/blob/CO2/MH-Z19B_CO2_Sensor.pdf
 * Project : C02 monitor
 * Description : This code is designed to calculate indoor level of c02 gas concentration with 5 seconds between each reading.  
 *                The Parts Per Million (PPM) for this project is obtained from the PWM output.  
 *                Therefore, the calculation is to generate data with 2000 ppm detection range based on information from datasheet.                  
 *                The project is created on Adafruit Feather 32u4 and MH-Z19B c02 sensor.  
 * More information on C02 level : https://www.kane.co.uk/knowledge-centre/what-are-safe-levels-of-co-and-co2-in-rooms
 *              
 */
#include <SPI.h>
#include "MHZ19.h"
#include <Wire.h>


#define BAUDRATE 9600

MHZ19 myMHZ19;

//global variables for temperature and Humidity
float temperature = 0;
float CO2 = 0;


/************************** Magic Begins Here ***********************************/


//identify response code
unsigned char response[9];

unsigned char payload[5];

void setup() {
  Serial.begin(9600);  //display on serial monitor
  while (!Serial)
    ;

  Serial1.begin(9600);

  myMHZ19.begin(Serial1);    // Initialize the sensor with hardware serial port 1
  Serial1.begin(9600);

  Wire.begin(0x08);
  Wire.onRequest(requestEvent);
  
  //delay(20000000); // Warmup delay before calibration
  
  myMHZ19.autoCalibration();  // Turn auto calibration ON (use false to turn it OF
  myMHZ19.setRange(5000);

  // Initialize LoRa
  Serial.print("Starting CO2...");

  
  Serial.println("OK");
}

void loop() {
  
  // Read the temperature from the DHT22
  float temperature = myMHZ19.getTemperature();  // Request Temperature (Celsius)
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Read the humidity from the DHT22
  float CO2 = myMHZ19.getCO2(true);  // Request CO2 (ppm)
  Serial.print("ppm ");
  Serial.println(CO2);

  uint16_t payloadTemp = encodeFixedPoint(temperature);
  uint16_t payloadCO2 = encodeFixedPointCO2(CO2);

  // Convert to bytes
  byte tempLow = lowByte(payloadTemp);
  byte tempHigh = highByte(payloadTemp);
  byte CO2Low = lowByte(payloadCO2);
  byte CO2High = highByte(payloadCO2);

  // Fill the payload
  payload[0] = tempLow;
  payload[1] = tempHigh;
  payload[2] = CO2Low;
  payload[3] = CO2High;

  Serial.print("Payload: ");
  for (int i = 0; i < sizeof(payload); i++) {
    Serial.print(payload[i], HEX);
    Serial.print(" ");
  }
  Serial.println();


  // blink LED to indicate packet sent
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);

  //sending interval 5 minutes
  //delay(300000);
  delay(30000); // 1 minute
}

void requestEvent() {
  Wire.write((char*)&payload, 5);
}

uint16_t encodeFixedPoint(float value) {
  // Convert float to fixed-point (scale by 100 for 2 decimal places)
  int16_t fixedPointValue = (int16_t)(value * 100);
  return (uint16_t)fixedPointValue;
}

uint16_t encodeFixedPointCO2(float value) {
  // Convert float to fixed-point (scale by 100 for 2 decimal places)
  int16_t fixedPointValue = (int16_t)(value);
  return (uint16_t)fixedPointValue;
}