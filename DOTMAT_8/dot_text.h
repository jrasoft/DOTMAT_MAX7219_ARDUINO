int writestring(int x, char *ch) {

  // clear
  for (int y = 0; y < 8; y++) for (int i = 0; i < 32; i++) M[y][i] = reversedot;

  for (int i = 0; i < strlen(ch); i++) {
    if (ch[i] == '/') {
      i++;
      if (ch[i] != '/') x = wC(x, ch[i], 1); else x = wC(x, '/', 0); // Maskierung "/" darstellen
    } else {
      if (ch[i] == '^') {
        i++;
        if (ch[i] != '^') x = wC(x, ch[i], 2); else x = wC(x, '^', 0);// Caret darstellen
      } else {
        if (small) x = wC(x, ch[i], 2); else x = wC(x, ch[i], 0);
      }
    }
  }
  return x;
}

void textloop(char *txt) {

  // LLstat = 1;
  int sl = writestring(0, txt);
  if (sl > 32) {
    int starter;
    if  (alLeft) starter = 0; else starter = 31;  //Durchlaufen wenn nicht alLeft
    for (int i = starter; i > -1 * sl; i--) {
      if (!Serial.available()) {
        writestring(i, txt);
        screen();
      }
    }
    //for (int i = 0; i < 10; i++) if (!Serial.available()) delay(200);

  } else {
    if (alLeft) {
      writestring(0, txt);
    } else {
      writestring( (int)((33 - sl) / 2), txt);
    }
    screen();
  }
}

const char demoA[] PROGMEM = "BIG Letters + 123 + /A/O/U/s";
const char demoB[] PROGMEM = "^s^m^a^l^l ^l^e^t^t^e^r^s ^A^b^c^d^e^f^g ^+ ^1^2^3";
const char demoC[] PROGMEM = "^C^y^r^i^l^l A/B.../V/}.../S/r/R";
const char demoD[] PROGMEM = "^S^i^g^n^s /m/n /N /k/| /1/2/#/3/4 /k/| /|/z/!/@/i/F/l/Y/|" ;
const char demoE[] PROGMEM = "^K^e^r^n^i^n^g^/^F^l^i^p ^.^.^." ;
const char demoF[] PROGMEM = "/Offi" ;
const char *const string_table[] PROGMEM = {demoA, demoB, demoC, demoD, demoE, demoF};

bool demo() {
  strcpy(stream, "DEMO");
  textloop(stream);
  delay(333);

  for (int i = 0; i < 5; i++) {
    strcpy_P(stream, (char *)pgm_read_word(&(string_table[i])));
    textloop(stream); 
  }

  strcpy_P(stream, (char *)pgm_read_word(&(string_table[5])));
  nokerning = !nokerning; textloop(stream); delay(1000);
  nokerning = !nokerning; textloop(stream); delay(1000);
  flipscreen = !flipscreen;	delscr(); textloop(stream); delay(1000);
  flipscreen = !flipscreen; delscr(); textloop(stream); delay(1000);
  return true;
}
 
const char LogoC[] PROGMEM = "^c^o^n^n^e^c^t" ;
const char LogoD[] PROGMEM = "wait^ ^.^.^." ;
const char *const Logo[] PROGMEM = {LogoA, LogoB, LogoC, LogoD};
// "Hello World!";
// "^R^a^d^i^o^ ^V^7";

bool startlogo(int mx) {
  int l = 1500;
  for (int i = 0; i < mx; i++) {
      testscr();
    strcpy_P(stream, (char *)pgm_read_word(&(Logo[i])));
    textloop(stream); delay(l);
  }
  return true;
}
  
bool testscreen() {

  strcpy(stream, "Test"); textloop(stream); delay(333);
  reversedot = !reversedot; strcpy(stream, "Test"); textloop(stream);

  delay(333);
  for (int i = 0; i < 4; i++) lc.setIntensity(i, 0);

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 32; i++) {
      if (((i - 24) >= 0) && ((i - 24) < 8)) lc.setIntensity(0, i - 24); else lc.setIntensity(0, 0);
      if (((i - 16) >= 0) && ((i - 16) < 8)) lc.setIntensity(1, i - 16); else lc.setIntensity(1, 0);
      if (((i - 8) >= 0) && ((i - 8) < 8)) lc.setIntensity(2, i - 8); else lc.setIntensity(2, 0);
      if (((i ) >= 0) && ((i ) < 8)) lc.setIntensity(3, i ); else lc.setIntensity(3, 0);
    }
    for (int i = 31; i > 0; i--) {
      if (((i - 24) >= 0) && ((i - 24) < 8)) lc.setIntensity(0, i - 24); else lc.setIntensity(0, 0);
      if (((i - 16) >= 0) && ((i - 16) < 8)) lc.setIntensity(1, i - 16); else lc.setIntensity(1, 0);
      if (((i - 8) >= 0) && ((i - 8) < 8)) lc.setIntensity(2, i - 8 ); else lc.setIntensity(2, 0);
      if (((i ) >= 0) && ((i ) < 8)) lc.setIntensity(3, i ); else lc.setIntensity(3, 0);
    }
  }

  reversedot = !reversedot; strcpy(stream, "OK"); textloop(stream); delay(666);
  for (int i = 0; i < 4; i++) lc.setIntensity(i, intensity);

  return true;
}

bool textanalyzer(char *txt) {

  //Serial.print(txt[0]);
  //Serial.println(txt[1]);
  //TODO Analyse Umlaute
  
  bool rw = false;

  if (strcmp(txt, "[0]\n" ) == 0) rw = setscreenHL(0);
  if (strcmp(txt, "[1]\n" ) == 0) rw = setscreenHL(1);
  if (strcmp(txt, "[2]\n" ) == 0) rw = setscreenHL(2);
  if (strcmp(txt, "[3]\n" ) == 0) rw = setscreenHL(3);
  if (strcmp(txt, "[4]\n" ) == 0) rw = setscreenHL(4);
  if (strcmp(txt, "[5]\n" ) == 0) rw = setscreenHL(5);
  if (strcmp(txt, "[6]\n" ) == 0) rw = setscreenHL(6);
  if (strcmp(txt, "[7]\n" ) == 0) rw = setscreenHL(7);
  if (strcmp(txt, "[8]\n" ) == 0) rw = setscreenHL(8);
  if (strcmp(txt, "[9]\n" ) == 0) rw = setscreenHL(9);
  if (strcmp(txt, "[10]\n" ) == 0) rw = setscreenHL(10);
  if (strcmp(txt, "[11]\n" ) == 0) rw = setscreenHL(11);
  if (strcmp(txt, "[12\n]" ) == 0) rw = setscreenHL(12);
  if (strcmp(txt, "[13]\n" ) == 0) rw = setscreenHL(13);
  if (strcmp(txt, "[14]\n" ) == 0) rw = setscreenHL(14);
  if (strcmp(txt, "[15]\n" ) == 0) rw = setscreenHL(15);

  if (strcmp(txt, "[BX]\n" ) == 0) {
    intensity++; if (intensity > 15) intensity = 0; rw = setscreenHL(intensity);
  }

  if (strcmp(txt, "[F]\n" ) == 0) {
    rw = true;
    flipscreen = true;
    EEPROM.write(2, flipscreen);
    delscr();
  }
  if (strcmp(txt, "[f]\n" ) == 0) {
    rw = true;
    flipscreen = false;
    EEPROM.write(2, flipscreen);
    delscr();
  }
  if (strcmp(txt, "[K]\n" ) == 0) {
    rw = true;
    nokerning = false;
    EEPROM.write(1, nokerning);
  }
  if (strcmp(txt, "[k]\n" ) == 0) {
    rw = true;
    nokerning = true;
    EEPROM.write(1, nokerning);
  }
  if (strcmp(txt, "[r]\n" ) == 0) {
    rw = true;
    reversedot = false;
  }
  if (strcmp(txt, "[R]\n" ) == 0) {
    rw = true;
    reversedot = true;
  }
  if (strcmp(txt, "[l]\n" ) == 0) {
    rw = true;
    alLeft = false;
  }
  if (strcmp(txt, "[L]\n" ) == 0) {
    rw = true;
    alLeft = true;
  }
  if (strcmp(txt, "[S]\n" ) == 0) {
    rw = true;
    small = true;
  }
  if (strcmp(txt, "[s]\n" ) == 0) {
    rw = true;
    small = false;
  }

  if (strcmp(txt, "[RESET]\n" ) == 0) {
    rw = true;
    resetFunc();
  }
  if (strcmp(txt, "[ver]\n" ) == 0) rw = startlogo(2);
  if (strcmp(txt, "[test]\n" ) == 0) rw = testscreen();
  if (strcmp(txt, "[demo]\n" ) == 0) rw = demo();
  if (strcmp(txt, "[PWRON]\n" ) == 0) {
    rw = true;
    digitalWrite(RELAIS, HIGH);
  }
  if (strcmp(txt, "[PWROFF]\n") == 0) {
    rw = true;
    digitalWrite(RELAIS, LOW);
  }
  if (strcmp(txt, "[AUXON]\n" ) == 0) {
    rw = true;
    digitalWrite(AUX, LOW);
  }
  if (strcmp(txt, "[AUXOFF]\n") == 0) {
    rw = true;
    digitalWrite(AUX, HIGH);
  }

  return rw;
}
