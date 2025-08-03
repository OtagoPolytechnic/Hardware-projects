#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include "MHZ19.h"

const int rx_pin = 13; //Serial rx pin no
const int tx_pin = 15; //Serial tx pin no

MHZ19 *mhz19_uart = new MHZ19(rx_pin,tx_pin);

// Webhook info
String KEY = "SomeSecurePassword";        // Webhook passcode (PHP)
String WH_URL = "http://carbon.otago.online:80/webhook.php";  //Webhook URL

void setup() {
  Serial.begin(115200);

  // WiFiManager
  WiFiManager wifiManager;

  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();

  // Use this for auto generated AP name based on MAC
  wifiManager.autoConnect();

  // If you get here you have connected to the WiFi
  Serial.println("Connected.");  

  // Sensor Serial
  mhz19_uart->begin(rx_pin, tx_pin);
  mhz19_uart->setAutoCalibration(true);

  // Important delay, bad readings if queried too soon
  delay(5000);
  measurement_t m = mhz19_uart->getMeasurement();
  
  // Read ppm
  int ppm = m.co2_ppm;
  
  // Read Temperature
  int temp = m.temperature;

  // Sensor Check (These values are effectivly null)
  if ((ppm == -1) || (ppm == 410))
  {
    Serial.println("Sensor still warming...\n\n");
    // 300e6 is 5 minutes of sleep in microseconds
    sleepy(300e6);
  }
  else
  {
    // Send data to posting method
    poster(ppm, temp);
      
    // 300e6 is 5 minutes of sleep in microseconds
    sleepy(300e6);
  }
}

void loop()
{
  // Nothing happens here, ESP resets after sleep
}

void poster(int ppm, int temp) 
{ 
   if(WiFi.status()== WL_CONNECTED)    // Check WiFi connection status
   {  
      // Declare object of class HTTPClient
      HTTPClient http;

      // Create POST data
      String POSTdata = "ppm=" + String(ppm) + "&temp=" + String(temp) + "&dev=" + WiFi.macAddress() + "&key=" + String(KEY);

      // Specify request destination
      http.begin(WH_URL);

      // Specify content-type header
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Send the request
      int httpCode = http.POST(POSTdata); 

      // Get the response payload
      String payload = http.getString();

      // Print HTTP return code
      Serial.println(httpCode);

      // Print request response payload
      Serial.println(payload);    

      // Close connection
      http.end();  
   }
   
   else
   {
      Serial.println("Problem with WiFi connection");
   }
}

void sleepy(int t)    // Accepts time to sleep in microseconds
{
  Serial.println("Deep sleep... Bye!\n\n");
  ESP.deepSleep(t); 
}