#include <Adafruit_NeoMatrix.h>
#include <Adafruit_GFX.h>

#define NEOPIXEL_PIN 6  // Pin where the NeoPixels are connected
#define NCOLUMNS 45     // Number of columns in the NeoMatrix
#define NHEIGHT 8       // Number of rows in the NeoMatrix
#define CHAR_WIDTH 6    // Width of each character in pixels

String textString;
String colorString;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(NCOLUMNS, NHEIGHT, NEOPIXEL_PIN,
                                               NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
                                               NEO_GRB + NEO_KHZ800);

byte red = 255;
byte green = 255;
byte blue = 255;

byte red_random = 0;  // Ensuring these are not removed
byte green_random = 0;
byte blue_random = 0;

// Variables for scrolling
static unsigned int index = 0;        // Pixel offset for starting the text
static unsigned long lastUpdate = 0;  // Time of the last scroll update
int scrollDelay = 50;                 // Milliseconds between scroll updates

// ============================== setup ============================================

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  Neopixel_Initial();  // Custom initialization for NeoPixels
}

// =============== Loop ======================

enum State { IDLE,
             DISPLAYING_TEXT };
State currentState = DISPLAYING_TEXT;

void loop() {

  if (currentState == DISPLAYING_TEXT) {
    Neomatrix_random_color();
    if (!Send_Text_NonBlocking("BIT Arcade Machine", red, green, blue)) {
      // When scrolling is done, reset to keep the text continuous
      matrix.setTextColor(matrix.Color(red_random, green_random, blue_random));
      resetTextDisplay();
    }
  }
}

void resetTextDisplay() {
  // Function to reset the text display parameters
  index = 0;              // Reset index for new text
  lastUpdate = millis();  // Reset the last update time to avoid immediate scrolling
}

// ============== Send Text Non-Blocking =================

bool Send_Text_NonBlocking(String inputstr, byte red, byte green, byte blue) {
  int totalWidth = inputstr.length() * CHAR_WIDTH + NCOLUMNS;  // Total pixels to scroll through
  
  if (millis() - lastUpdate > scrollDelay) {
    lastUpdate = millis();
    matrix.fillScreen(0);
    matrix.setCursor(-index + NCOLUMNS - 1, 0);  // Set the start position for the text
    matrix.print(inputstr);
    matrix.show();

    // Update index for scrolling
    index++;
    if (index >= totalWidth) {
      index = 0;     // Reset index after the full text has scrolled
      return false;  // Indicate completion of one scroll cycle
    }
  }
  return true;  // Continue scrolling
}

// ================== Convert HEX to RGB ========================

void hexToRGB(String hex, byte &r, byte &g, byte &b) {
  hex.replace("#", "");
  long value = strtol(hex.c_str(), NULL, 16);
  r = (value >> 16) & 0xFF;
  g = (value >> 8) & 0xFF;
  b = value & 0xFF;
}
