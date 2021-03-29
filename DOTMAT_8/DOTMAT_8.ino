/*
   V 8.0
   Version für Simple Internet Radio nur Ausgabe
   57600 baud!
   erweiterter Zeichensatz ohne geteilter Matrix mit Auto-Kerning
   Definition in "radio-com.ods"
   Status Kerning Flip REversedot und Helligkeit wird in EEPROM gespreichert

   A Textausgabe
   B Befehle in []
   [test] test screen
   [BX] [0]..[15] Helligkeit
   [R] [r] Reverse
   [K] [k] Kerning
   [L] [C] Linksbündig, Centriert (Default)
   [RESET] Arduino Reset
   [PWRON][PWROFF] Relais Ein Aus
   [AUXON][AUXOFF] AUX Ein Aus
   nach »lenOfWait« Millisekunden schaltet Display auf 1 Pkt, wenn keine neuen Datan ankommen
*/
/******************* VERSION ********************************/
const char LogoA[] PROGMEM = "^L^E^D^ ^V^8^.^1" ;
const char LogoB[] PROGMEM = "^R^a^d^i^o^ ^V^7" ;

#include <EEPROM.h>
#include <LedControl.h>
const int RELAIS = 7;
const int AUX = 13;
const int Max7219_pinCLK = 5;
const int Max7219_pinCS = 4;
const int Max7219_pinDIN = 3;
LedControl lc = LedControl(Max7219_pinDIN, Max7219_pinCLK, Max7219_pinCS, 4);
uint8_t intensity;                // Helligkeit                   | EEPROM 0

//IO Seriell
const byte numChars = 1023;       // maximale Stringlänge
char stream[numChars] = "";       // Zeichenkette zur Anzeige
char oldstream[numChars] = "";    // Vergleichskette
String inputString = "";          // String Object Serielle Schnittstelle
bool stringComplete = false;      // whether the string is complete
bool resetStream = false;         //neue Zeichenkette gefunden

// Pixelmanagement
bool M [8][32] = {0};             // Anzeige LEDs
bool MR [8][32] = {0};            // alte MAtrix (zum Vergleich = unnötiges pixel setzen)
bool LL [8];                      // Linker Rand für nachfolgende Ligaturberechnung
int LLstat = 0;                   // 0=Lig 1 zwangs Abstand 2= zwangsverbindung aus Flash
bool alLeft = false;              // kurze Strings Linksbündig
bool nokerning = false;           // Autokerning aus default ein  | EEPROM 1
bool flipscreen = false;          // Kopfstand                    | EEPROM 2
bool reversedot = false;          // "schwarze Pixel"             | EEPROM 3
bool small;                       // permanet small ASCII 32-126, |

// Zeitmessungen
unsigned long timestamp = 0;      // automatisches Ausschalten, wenn kein Update erfolgt
const unsigned long lenOfWait = 300000; //ms 5 min

void(* resetFunc) (void) = 0;     // declare reset function @ address 0

#include "char_dots.h"            // Zeichendefinition aus Tabelle
#include "pixels.h"
#include "char_ascii_def.h"       // switch case aus Tabelle
#include "dot_text.h"

void setup() {
  Serial.begin(57600);
  Serial.println("OK");
  pinMode(RELAIS, OUTPUT);
  digitalWrite(RELAIS, HIGH);
  digitalWrite(AUX, HIGH);
  EEPROM_MAX7210Init();
  //startlogo(4);
  resetStream = true;
}

void loop() {
  if (stringComplete) {
    inputString.toCharArray(stream, numChars);
    inputString = "";
    stringComplete = false;
    resetStream = true;
    delay(100);
  }

  if (resetStream == true) {
    resetStream = false;
    timestamp = millis();
    if (textanalyzer(stream)) strcpy(stream, oldstream);
    strcpy(oldstream, stream);
  }

  if (millis() > (timestamp + lenOfWait)) {
    strcpy(stream, "/."); //1 Punkt bis was neues kommt
    resetStream = true;
  }
  textloop(stream);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
      //Serial.println(inputString);
    }
  }
}
