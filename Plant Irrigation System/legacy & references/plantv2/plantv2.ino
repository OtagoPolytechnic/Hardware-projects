#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <avr/wdt.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int SOL_PIN = 10;

// Define LED properties
#define LED_PIN 5
#define NUM_LEDS 8
#define BRIGHTNESS 70
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Button variables
int ON_BTN_PIN = 2;
int CLIMATE_BTN_PIN = 12;
int SOIL_BTN_PIN = 8;
int HOME_BTN_PIN = 4;
int WATER_BTN_PIN = 9;

int pins[] = { ON_BTN_PIN, CLIMATE_BTN_PIN, SOIL_BTN_PIN, HOME_BTN_PIN, WATER_BTN_PIN };

// Temp/Humid Sensor Variables
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float MIN_TEMP = 10.00;
float MAX_TEMP = 30.00;
float MIN_HUMID = 25.00;
float MAX_HUMID = 75.00;

// Moisture sensor variables
#define MOIST_SENS_PIN A0
int MIN_MOIST = 30;
int MAX_MOIST = 75;
bool tankEmpty = false;


// All possible machine states
enum State {
  OFF,
  ON,
  DISPLAY_CLIMATE,    // Climate of plant i.e. temp and humid
  DISPLAY_SOIL_INFO,  // Soil moisture and water threshold
  DISPENSE_WATER,     // "Dispensing water..."
  DISPLAY_HOME        // Plant name, mood i.e. "Good!", "Alright", etc...
};

// Automatically turn on when power first received
State machineState = ON;

// Compare climate differences for debouncing
static float currentTemp = 0;
static float currentHumid = 0;
static float lastTemp = 0;
static float lastHumid = 0;
static int currentMoist = 0;
static int lastMoist = 0;

// Plant mood based on climate and water levels
static String happiness = "";
int homeColumn = 0;

// Keep button reading consistent for each loop
static int currentBtn = 2;
static int lastBtn = 0;

void setup() {
  Serial.begin(9600);
  wdt_enable(WDTO_8S);  //Watchdog in case of reading errors

  lcd.init();

  pinMode(SOL_PIN, OUTPUT);

  // Init each button pin
  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], INPUT);
  }

  // Init moisture sensor
  pinMode(MOIST_SENS_PIN, INPUT);

  // Init DHT11 sens
  dht.begin();

  // Init LED strip and set starting LED state
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  turnOn();
}

void loop() {
  wdt_reset();
  handleMachineState();
}

void handleMachineState() {
  // If machine is on, always check soil/climate, and change LED
  if (machineState != OFF && machineState != ON) {
    // If 5 seconds have passed since last reading
    if (readingBuffer()) {
      if (currentMoist <= MIN_MOIST) {
        dispense();
      }
      readSoil();
      readClimate();
      stateAction();
      changeMood();
    }
  }

  // If a button is pressed, change state and do something
  if (debounceButton()) {
    changeState();
    stateAction();
  }
  delay(100);
}

// Change machine state depending on button press
void changeState() {
  // if machine state is not off, then allow user functionality
  if (machineState != OFF) {
    switch (currentBtn) {
      case 2:
        machineState = OFF;
        break;
      case 12:
        machineState = DISPLAY_CLIMATE;
        break;
      case 8:
        machineState = DISPLAY_SOIL_INFO;
        break;
      case 4:
        machineState = DISPLAY_HOME;
        break;
      case 9:
        if (machineState != DISPENSE_WATER) {
          machineState = DISPENSE_WATER;
        }
        break;
    }
  }
  //if machine state is off and on btn pressed, then turn on
  else if (machineState == OFF && currentBtn == 2) {
    machineState = ON;
  }
}

// Calls the corresponding actions depending on machine state
void stateAction() {
  switch (machineState) {
    case OFF:
      turnOff();
      break;
    case ON:
      turnOn();
      break;
    case DISPLAY_CLIMATE:
      displayClimate();
      break;
    case DISPLAY_SOIL_INFO:
      displaySoil();
      break;
    //display happiness, plant name
    case DISPLAY_HOME:
      home();
      break;
    case DISPENSE_WATER:
      if (dispense()) {
        machineState = DISPLAY_HOME;
        currentBtn = 4;
        home();
      }
      break;
  }
}

// Display basic home screen with project title and plant mood
void home() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Smart Plant");
  lcd.setCursor(1, 1);
  // lcd.print(happiness);
}

// Display message and dispense water. If no soil moisture change detected after 5 seconds then displays "failed to water, please check tank", else "dispense complete"
bool dispense() {
  Serial.println("Watering...");
  lcd.clear();
  lcd.home();
  lcd.print("Watering...");
  fill_solid(leds, NUM_LEDS, CRGB::Blue);

  // If low water levels detected in soil, attempt to water plant
  if (currentMoist <= 60) {
    while (currentMoist <= 60) {
      // digitalWrite(SOL_PIN, HIGH);  // Open solenoid

      // keep reading soil levels until moist enough
      readSoil();

      // if 5 seconds have passed and not watered enough, print failed and exit loop
      if (readingBuffer() && currentMoist <= 60) {
        // digitalWrite(SOL_PIN, LOW);
        lcd.clear();
        Serial.println("Failed to water");
        lcd.setCursor(1, 0);
        lcd.print("Cannot water");
        lcd.setCursor(2, 1);
        lcd.print("Check tank");
        delay(3000);
        return false;
      }
      wdt_reset();
    }

    // digitalWrite(SOL_PIN, LOW);
    lcd.clear();
    Serial.println("Complete");
    lcd.setCursor(3, 0);
    lcd.print("Plant");
    lcd.setCursor(1, 1);
    lcd.print("Watered!");
    delay(3000);
    return true;
  }
  // if someone pressed the button and not low, let them know it doesn't need watering
  else if (currentMoist > 75) {
    lcd.clear();
    Serial.println("Do not need watering");
    lcd.setCursor(0, 0);
    lcd.print("Already full");
    delay(3000);
    return true;
  }
  delay(1000);
}

// turn off screen w message, change LED to black (do not change led color state)
void turnOff() {
  Serial.println("Turning off");
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.println("Turning Off...");
  delay(1000);
  lcd.clear();
  lcd.noBacklight();
  lcd.noDisplay();
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void turnOn() {
  Serial.println("Turning on");
  lcd.backlight();
  lcd.display();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Turning On...");
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(1000);
  machineState = DISPLAY_HOME;
  currentBtn = 4;
  home();
}

void displaySoil() {
  Serial.println("Displaying soil!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(currentMoist);  //print the water content
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Threshold: ");
  lcd.print(MAX_MOIST);  //print the moisture max
  lcd.print("%");
}

void displayClimate() {
  Serial.println("Displaying climate");
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
}

void changeMood() {
  if (currentMoist > MAX_MOIST || currentTemp > MAX_TEMP || currentHumid > MAX_HUMID) {
    fill_solid(leds, NUM_LEDS, CRGB::Red);
    // happiness = "Overwhelmed!";
    homeColumn = 1;
  } else if (currentMoist < MIN_MOIST || currentTemp < MIN_TEMP || currentHumid < MIN_HUMID) {
    fill_solid(leds, NUM_LEDS, CRGB::Yellow);
    homeColumn = 2;
    if (currentMoist <= MIN_MOIST) {
      // happiness = "Please water";
    }
    // happiness = "Feeling dry";
  } else {
    fill_solid(leds, NUM_LEDS, CRGB::Green);
    // happiness = "Happy!";
    homeColumn = 4;
  }
  FastLED.show();
}

bool readSoil() {
  currentMoist = (analogRead(MOIST_SENS_PIN)) / 10;  // read the analog value from sensor and convert

  // Check if readings are valid
  if (isnan((double)currentMoist)) {
    Serial.println("Failed to read from Soil sensor!");
    lcd.print("Failed");
    return false;
  }

  // If the latest reading differs from the previous readings...
  if ((currentMoist - lastMoist) >= 5) {
    // Update the last moisture readings
    lastMoist = currentMoist;
    Serial.println("Current moist: ");
    Serial.println(currentMoist);
    return true;
  }

  return false;
}

bool readClimate() {
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
    lastHumid = currentHumid;
    lastTemp = currentTemp;
    Serial.print("Temp: ");
    Serial.println(currentTemp);
    Serial.print("Humid: ");
    Serial.println(currentHumid);
    return true;
  }

  return false;
}

// Checks if at least 5 seconds have passed since last call
bool readingBuffer() {
  unsigned long currentCheckTime = millis();
  static unsigned long lastCheckTime = 0;

  // If 5 seconds have passed...
  if ((currentCheckTime - lastCheckTime) >= 5000) {
    // Change last time
    lastCheckTime = currentCheckTime;
    // Return true to indicate a 5 second passing
    return true;
  }

  // Return false if 5 seconds have not passed
  return false;
}

// Delay button/state change by .5 seconds to count for button noise, return if any button has been pressed
bool debounceButton() {
  static int lastButtonState = 0;  // Initialize to 1 to detect falling edge
  static unsigned long lastDebounceTime = 0;
  const unsigned long debounceDelay = 50;  // milliseconds
  int reading = 0;

  for (int i = 0; i < 5; i++) {
    reading = digitalRead(pins[i]);
    if (reading == HIGH) {
      currentBtn = pins[i];
      break;
    }
  }

  unsigned long elapsedTime = millis() - lastDebounceTime;

  if (reading != lastButtonState) {
    // Button state has changed, update debounce time
    lastDebounceTime = millis();
  }

  // If debounce delay has passed
  if (elapsedTime >= debounceDelay) {
    // Return true only if the button is pressed (reading is HIGH) AND If the current state is different from the last state
    if (reading == HIGH && reading != lastButtonState) {  // is  && reading != lastButtonState needed?
      lastButtonState = reading;
      Serial.println("Button pressed!");
      return true;
    }
  }
  Serial.println("Waiting...");
  lastButtonState = reading;
  return false;
}
