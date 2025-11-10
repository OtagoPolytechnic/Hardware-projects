#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define LED properties
#define LED_PIN 5
#define NUM_LEDS 6  // cahnge to 25
#define BRIGHTNESS 70
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Button variables
#define ON_BTN_PIN 2
#define CLIMATE_BTN_PIN 12
#define MOIST_BTN_PIN 8

// Temp/Humid Sensor Variables
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Moisture sensor variables
#define MOIST_SENS_PIN A0
#define MOIST_THRESHOLD 530

int pins[] = {ON_BTN_PIN, CLIMATE_BTN_PIN, MOIST_BTN_PIN, MOIST_SENS_PIN}

// All possible machine states
enum State {
  OFF,
  ON,
  DISPLAY_CLIMATE,         // Climate of plant i.e. temp and humid
  DISPLAY_SOIL_INFO,       // Soil moisture and light percent
  DISPLAY_WATER_DISPENSE,  // "Dispensing water..."
  DISPLAY_PLANT            // Plant name, mood i.e. "Good!", "Alright", etc...
};

State machineState = ON;

// Compare climate differences for debouncing
float currentTemp = 0;
float currentHumid = 0;
static float lastTemp = 0;
static float lastHumid = 0;

// Measure overall status overtime
static int moisture = 25;
static int temperature = 25;
static int humidity = 25;
static int light = 25;
static int happiness = moisture + light + temperature + humidity;

void setup() {
  Serial.begin(9600);

  lcd.init();

  // Initialize buttons
  pinMode(ON_BTN_PIN, INPUT);
  pinMode(CLIMATE_BTN_PIN, INPUT);
  pinMode(MOIST_BTN_PIN, INPUT);

  // Init moisture sensor
  pinMode(MOIST_SENS_PIN, INPUT);

  // Init DHT11 sens
  dht.begin();

  // Init LED strip and set starting LED state
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  // Turn on screen and LED
  screenTransition();
  ledTransition();
}

void loop() {
  handleStateMachine();
  delay(100);
}

void handleStateMachine() {
  // When button pressed, change state and call transisiton methods
  if (debounceButton()) {
    if (machineState == OFF) {
      machineState = ON;
    } else if (machineState == ON) {
      machineState = OFF;
    } else if (machineState == ON) {
      machineState = OFF;
    } else if (machineState == ON) {
      machineState = OFF;
    }
    // When MS on/off, call transition check methods
    screenTransition();
    ledTransition();
  }

  tempTransition();
}

void moistTransition() {
  if (machineState == DISPLAY_SOIL_INFO)
}

void moistState() {
  int value = analogRead(AOUT_PIN);  // read the analog value from sensor
  // int outputValue = map(sensorValue, 0, 1023, 255, 0); // map the 10-bit data to 8-bit data --- may not need this?
  //send the lcd lines to screenTransition
  lcd.home();
  lcd.print("Moisture: ");
  lcd.print(value / 10);
  lcd.print("%");

  if (value >= MOIST_THRESHOLD && value <= 800) {
    moisture = 25;
  } else if (value >= 400 && value < MOIST_THRESHOLD) {
    moisture = 20;
  } else if (value >= 300 && value < 400) {
    moisture = 15;
  } else if (value >= 200 && value < 300) {
    moisture = 10;
  } else if (value >= 100 && value < 200) {
    moisture = 5;
  } else {
    moisture = 0;
  }

  delay(500);
}

void tempTransition() {
  // when temp button is pressed, display temp, redisplay every 5 seconds until another button is pressed
  if (machineState == DISPLAY_SOIL_INFO) {
    tempState();
  }
}

void tempState() {
  // Read current temperature and humidity from the sensor
  currentTemp = (float)dht.readTemperature();
  currentHumid = (float)dht.readHumidity();

  // Check if readings are valid
  if (isnan(currentTemp) || isnan(currentHumid)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.print("Failed");
    return false;
  }

  // If the latest temperature or humidity differs from the previous readings...
  if (currentTemp != lastTemp || lastHumid != currentHumid) {
    // Update the last temperature and humidity readings
    screenTransition();
  }

  // Update the last temperature and humidity readings
  lastHumid = currentHumid;
  lastTemp = currentTemp;
}

/**
 * Check temperature and humidity readings at regular intervals
 * and update global variables if there's a change.
 * 
 * @return bool Returns true if there's a change in temperature
 *             or humidity readings since the last check, false otherwise.
 */
bool printBuffer() {
  unsigned long currentCheckTime = millis();
  static unsigned long lastCheckTime = 0;

  // If 5 seconds have passed...
  if (currentCheckTime >= (lastCheckTime + 5000)) {
    // Change last time
    lastCheckTime = currentCheckTime;
    // Return true to indicate a change in readings
    return true;
  }

  // Return false if there's no change in readings
  return false;
}


// Function to handle LED transition based on machine state
void ledTransition() {
  // Read the state of the LED pin
  byte ledState = digitalRead(LED_PIN);

  if (machineState == ON) {
    // Turn on the LED strip by filling it with green color
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    FastLED.show();

  } else if (machineState == OFF) {
    // Turn off the LED strip by filling it with black color
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
  }
}

// Function to handle screen transition based on machine state
void screenTransition() {
  if (machineState == OFF) {
    // Clear, print off message, turn off screen
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Turning Off...");
    delay(1000);
    lcd.noBacklight();
    lcd.clear();
    lcd.noDisplay();

  } else if (machineState == ON) {
    // Clear, print turn on message, print home screen message
    lcd.backlight();
    lcd.display();
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Turning On...");
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Welcome to");
    lcd.setCursor(3, 1);
    lcd.print("Smart Pot!");
    delay(500);
  } else if (machineState == DISPLAY_SOIL_INFO && printBuffer()) {
    lcd.clear();
    lcd.setCursor(0, 0);  // set the cursor on the first row and column
    lcd.print("Humidity: ");
    lcd.print(currentHumid);  //print the humidity
    lcd.print("%");
    lcd.setCursor(0, 1);  //set the cursor on the second row and first column
    lcd.print("Temp: ");
    lcd.print(currentTemp);  //print the temperature
    lcd.print((char)0b11011111);
    lcd.print("C");
    Serial.println("Temp state changed");
    Serial.println(millis());
    delay(1000);
  }
}

bool debounceButton() {
  static int lastButtonState = 0;  // Initialize to 1 to detect falling edge
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;  // milliseconds

  int reading = digitalRead(ON_BTN_PIN);

  unsigned long elapsedTime = millis() - lastDebounceTime;

  if (reading != lastButtonState) {
    // Button state has changed, update debounce time
    lastDebounceTime = millis();
  }

  // If debounce delay has passed
  if (elapsedTime >= debounceDelay) {
    // Return true only if the button is pressed (reading is HIGH) AND If the current state is different from the last state
    if (reading == HIGH && reading != lastButtonState) {
      lastButtonState = reading;
      return true;
    }
  }

  lastButtonState = reading;
  return false;
}
