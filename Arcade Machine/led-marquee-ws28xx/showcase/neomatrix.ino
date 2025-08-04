// ===============================================
void Neopixel_Initial() {
  delay(2000);   // so no big surge of current at the beginning
  matrix.begin(); 
  matrix.setTextWrap(true); 
  matrix.setBrightness(255);  // from 0 to 255
  matrix.setTextColor(matrix.Color(155, 0, 0));
  matrix.fillScreen(0);
  matrix.setCursor(0, 0);
  //matrix.print(F("How"));
  matrix.show(); 
}

// ==============================================

void Neomatrix_text(String instr, byte R, byte G, byte B, int xcursor) {
  matrix.setTextColor(matrix.Color(R,G,B));
  matrix.setCursor(xcursor, 0);
  matrix.print(instr);
}

// ===============================================

void Neomatrix_scrolltext(String instr, byte R, byte G, byte B) {

  matrix.setTextColor(matrix.Color(R,G,B));

  //Serial.println(instr.length());
  for (int x=0 ; x < (instr.length()*CHAR_WIDTH+NCOLUMNS); x++) {
    matrix.fillScreen(0);
    matrix.setCursor(-x+NCOLUMNS-1, 0);

    matrix.print(instr);
    matrix.show();
    delay(50);
  }
}

// =============================================

void Neomatrix_random_color() {

  int randn = random(0,767);
  red_random = 0;
  green_random = 0;
  blue_random = 0;

  if (randn < 256) {
      red_random = 255 - randn;
      green_random = randn;
  } else if (randn < 511) {
      green_random = 511 - randn;
      blue_random = randn - 256;
  } else {
      red_random = 767 - randn;
      blue_random = randn - 512;
  }
}

// ==============================================

void Neomatrix_scrolltext_random_eachstep(String instr) {

  //Serial.println(instr.length());
  for (int x=0 ; x < (instr.length()*CHAR_WIDTH+NCOLUMNS); x++) {

    if ( (x % 2) == 0 ) {
        Neomatrix_random_color();
    }
    //Neomatrix_random_color();
    matrix.setTextColor(matrix.Color(red_random,green_random,blue_random));

    matrix.fillScreen(0);
    matrix.setCursor(-x+NCOLUMNS-1, 0);

    matrix.print(instr);

    matrix.show();
    delay(10);
  }
}