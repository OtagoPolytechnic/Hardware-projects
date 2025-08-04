#include <Keyboard.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 5
#define KEY_RETURN 0xB0  // The hex value for the return key is 0xB0.

MFRC522 mfrc522(SS_PIN, RST_PIN);
char Enter = KEY_RETURN;  // Return key is declared as Enter.
String readid;
const String cards[] = {
  "3A3D9D9E",
  "3AF3BE9E",
  "E222CF1D"
  };  // Change these values to the UID of your card.
int numberOfCards = (sizeof(cards) / sizeof(cards[0]));
bool screenLocked = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Setup complete. Scan your RFID tag.");
}

void temp(byte *buffer, byte bufferSize) {
  readid = ""; // Clear previous UID
  for (byte i = 0; i < bufferSize; i++) {
    readid = readid + String(buffer[i], HEX);
  }
  readid.toUpperCase(); // Convert to upper case
  Serial.print("Card UID: ");
  Serial.println(readid);  // Print the UID
}

void lockScreen() {
  Serial.println("Card recognized. Locking screen.");
  Keyboard.press(KEY_LEFT_GUI);  // Press the left Windows key.
  Keyboard.press('l');           // Press the "l" key.
  Keyboard.releaseAll();
  delay(200);         // Release all keys.
  screenLocked = true;
}

void unlockScreen() {
  Serial.println("Card recognized. Unlocking screen.");
  Keyboard.print("Cabinet24");       // Change this value to your Windows PIN/Password.
  Keyboard.press(Enter);         // Press the Enter key to submit the PIN/Password
  Keyboard.releaseAll();
  delay(200);
  screenLocked = false;
}

bool isCardValid(String cardID) {
  for(int i=0; i < numberOfCards; i++) {
    if(cardID == cards[i]) {
      return true;
    }
  }
  return false;
}

void loop() {
  // Continuously check for new RFID cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));  // Display card details in serial monitor.
  temp(mfrc522.uid.uidByte, mfrc522.uid.size);

  if (isCardValid(readid)) {
    if (!screenLocked) {
      lockScreen();
    } else {
      unlockScreen();
    }
  } else {
    Serial.println("Unrecognized card.");
  }
}