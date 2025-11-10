const int WATER_BTN_PIN = 8;
const int RELAY_PIN = 9;

void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(WATER_BTN_PIN, INPUT_PULLUP);

  Serial.begin(9600);

  digitalWrite(RELAY_PIN, HIGH); //turn on
  delay(800); // fill tube - change this value depending on how long it takes to fill water tube
  digitalWrite(RELAY_PIN, LOW); //turn off
}

void loop() {
  int buttonState = digitalRead(WATER_BTN_PIN); // read new state
  
  if (buttonState == LOW) {
    Serial.println("The button is being pressed");
    digitalWrite(RELAY_PIN, HIGH); // turn on
    delay(1000);
  }
  else if (buttonState == HIGH) {
    Serial.println("The button is unpressed");
    digitalWrite(RELAY_PIN, LOW);  // turn off
  }
}
