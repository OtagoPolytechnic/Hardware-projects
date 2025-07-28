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
#include <TinyLoRa.h>
#include <SPI.h>
#include <SoftwareSerial.h>


// Network Session Key (MSB)
uint8_t NwkSkey[16] = { FILL ME IN };

// Application Session Key (MSB)
uint8_t AppSkey[16] = { FILL ME IN };


// Device Address (MSB)
uint8_t DevAddr[4] = { FILL ME IN };


/************************** Magic Begins Here ***********************************/

#define pwmPin 11
SoftwareSerial mySerial(0,1); // RX, TX   << reverse from sensor pins

//identify command 0x86 to read c02 concentration
byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

//identify response code
unsigned char response[9];

//variables for requirement items 
//unsigned data types are used when no negatives number expected
unsigned long th, tl,ppm = 0;
int ppm2= 0;
unsigned char payload[4];


// Pinout for Adafruit Feather 32u4 LoRa
TinyLoRa lora = TinyLoRa(7, 8, 4);

void setup() {
  Serial.begin(9600); //display on serial monitor
  mySerial.begin(9600); 

  
  // Initialize pin LED_BUILTIN as an output
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pwmPin, INPUT);  //using pin 11 as the pwm input
  Serial.print("\nUsing Pin: ");                            // Print Raw Pin Number
  Serial.println(pwmPin);

  // Initialize LoRa
  Serial.print("Starting LoRa...");
  
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

}

void loop() {
  
 // set up a new serial object
  mySerial.write(cmd,9);
  mySerial.readBytes(response, 9);
  
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  
  /*formula to find c02 concentration from datasheet MH-Z19B (c02 = high * 256 + low)*/
  ppm = (256*responseHigh)+responseLow;  
   
  //CO2 via PWM pin
  do {

    
    /*finding out the amount of time from low to high on pin11
    timeout at the end of cycle 1004 milli secs*/
    //formula to find high level output time during cycle
    th = pulseIn(pwmPin, HIGH, 1004000)/1000;  
    
    //formula to find low level output time during cycle
    tl = 1004 - th;
    
    //c02 level formula for 2000ppm range
    ppm2 = 2000 * (th-2)/(th+tl-4);

  } while (th == 0);

  //converted to string for char conversion for DB server readability
  String str;
  str = String(ppm2);
  str.toCharArray(payload,4);


  //identify destination port
  uint8_t framePort = 2;
 
  Serial.println("Sending LoRa Data...");

  //sending data to port 2 on destination app
  lora.sendData(payload, sizeof(payload), lora.frameCounter, framePort);
  
  //print on serial port
  Serial.print("Frame Counter: ");
  Serial.println(lora.frameCounter);
  Serial.println(framePort);
  
  lora.frameCounter++;


  // blink LED to indicate packet sent
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
            
  Serial.println("delaying...");
  Serial.print("ppm for 2000 detection range : ");
  Serial.println(ppm2);
  Serial.println("-----------");

  //sending interval 5 minutes
  delay(300000);
}