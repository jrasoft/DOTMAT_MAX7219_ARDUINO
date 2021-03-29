
void setpix( int x, int y, boolean dot) {
  int c;

  if (flipscreen) {
    if (x >= 24)  c = 3; else if (x >= 16) c = 2; else if (x >= 8) c = 1; else  c = 0;

    switch (x) {
      case  0: x = 7; break;    case  1: x = 6; break;  case  2: x = 5; break;  case  3: x = 4; break;
      case  4: x = 3; break;    case  5: x = 2; break;  case  6: x = 1; break;  case  7: x = 0; break;
      case  8: x = 7; break;    case  9: x = 6; break;  case 10: x = 5; break;  case 11: x = 4; break;
      case 12: x = 3; break;    case 13: x = 2; break;  case 14: x = 1; break;  case 15: x = 0; break;
      case 16: x = 7; break;    case 17: x = 6; break;  case 18: x = 5; break;  case 19: x = 4; break;
      case 20: x = 3; break;    case 21: x = 2; break;  case 22: x = 1; break;  case 23: x = 0; break;
      case 24: x = 7; break;    case 25: x = 6; break;  case 26: x = 5; break;  case 27: x = 4; break;
      case 28: x = 3; break;    case 29: x = 2; break;  case 30: x = 1; break;  case 31: x = 0; break;
    }

    switch (y) {
      case 0: y = 7; break;     case 1: y = 6; break;   case 2: y = 5; break;   case 3: y = 4; break;
      case 4: y = 3; break;     case 5: y = 2; break;   case 6: y = 1; break;   case 7: y = 0; break;
    }

  } else {
    if (x >= 24) {
      c = 0; x = x - 24;
    } else {
      if (x >= 16) {
        c = 1; x = x - 16;
      } else {
        if (x >= 8) {
          c = 2; x = x - 8;
        } else c = 3;
      }
    }
  }

  lc.setLed(c, y, x, dot);
}


int wSC(int x, int ch[9], int cl) { // Ligaturinfo = ch[8];

  bool LLL[8];
  bool LLR[8];
  bool b;

__SHIFT_LEFT:

  if (LLstat == 2) x--;

  for (int i = 0; i < 8 ; i++) {
    for (int j = 0; j < cl; j++) {
        b = pgm_read_word(& ch[i]) & (1 << j);  // Zugriff auf Flash
        if (reversedot) b = !b;                 // experimental negativ
        if (x + j > -1 && x + j < 32) M[i][x + j] = b;
        if (j == 0) LLL[i] = b;                 // Muster linker Rand
        if (j == cl - 1) LLR[i] = b;            // Muster rechter Rand
        /*
          rechten Rand später ! weitergeben
          In der Auswertung wird voriger Buchsten
          über globale Variable weiterverwendet
        */
    //  }
    }
  }

  if (LLstat == 0) {
    /*
       Analyse Abstand Pixel vom Vorhergehendem Zeichen
       Erlaubt  Nebeneinander 2x
                Diagonal 1x
    */

    int cc = 0;
    for (int i = 0; i < 8; i++) {
      bool b = LL[i];
      if (b == !reversedot) {
        if (b == LLL[i]) cc = cc + 1;                 // nebeneinander
        if (i > 0) if (b == LLL[i - 1]) cc = cc + 2;  // "diagonal"
        if (i < 6) if (b == LLL[i + 1]) cc = cc + 2;
      }
    }

    if (cc <= 2) {
      LLstat = 2;
      if (x > 0) goto __SHIFT_LEFT; // >0 wegen 1 Zeichen, SHL wird dann unterbunden
    }
  }

  if (x + cl  >= 0 && x + cl < 32) {
    for (int i = 0; i < 8; i++)  M[i][x + cl] = reversedot;
  }

  LLstat = pgm_read_word(& ch[8]);            // Status für nächste Runde
  if (LLstat == 0) if (nokerning) LLstat = 1; // Unterbindung der Kerningberechnung
  for (int i = 0; i < 8; i++) LL[i] = LLR[i]; // für die nächste Runde
  return  x + cl + 1;
}

void screen () {
  for (int x = 0; x < 32; x++) {
    for (int y = 0; y < 8; y++) {
      //setpix(x, y, M[y][x]);
      if (M[y][x] == true) {
        if (MR [y][x] == false) {
          setpix(x, y, true);
        }
      } else {
        if (MR [y][x] == true) {
          setpix(x, y, false);
        }
      }
      MR [y][x] = M [y][x];
    }
  }
}

  void testscr() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 32; x++) {
      MR [y][x] = true;
      setpix(x, y, true);
    }
  }
  }

void delscr() {
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 32; x++) {
      MR [y][x] = false;
      setpix(x, y, false);
    }
  }
}

void EEPROM_MAX7210Init() {
  intensity = EEPROM.read(0);
  nokerning = EEPROM.read(1);
  flipscreen = EEPROM.read(2);
  for (int address = 0; address < 4; address++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, intensity);
    lc.clearDisplay(address);
  }
}

bool setscreenHL (uint8_t intensity) {
  for (int c = 0; c < 4; c++) lc.setIntensity(c, intensity );
  EEPROM.write(0, intensity);
  return true;
}
