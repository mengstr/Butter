#include <SPI.h>
#include "ui.h"

#define BRIGHTNESS      15      // 0=Low --> 15=High brightness

const bool silent=false;


//
// Setup the pins on the arduino and initialize the MAX7219 chips
//
void setup() {
  Serial.begin(9600);

  pinMode(PIN_LEFT, INPUT_PULLUP);         // All buttons are inputs with internal pullup
  pinMode(PIN_MID, INPUT_PULLUP);
  pinMode(PIN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_A, INPUT_PULLUP);
  pinMode(PIN_B, INPUT_PULLUP);

  pinMode(PIN_MIDLED, OUTPUT);              // The two red LEDs in the middle are
  digitalWrite(PIN_MIDLED, HIGH);            // ... turned on during initialization
  
  // Initialize SPI
  SPI.begin();
  SPISettings(100000, MSBFIRST, SPI_MODE0);

  digitalWrite(SPI_CS, HIGH);               // Set SPI CS/LOAD inactive 

  // Intialize MAX7129's
  delay(100);
  SpiCommand(OP_DISPLAYTEST, 0);        // Disable test mode
  SpiCommand(OP_SCANLIMIT, 7);          // Use all scanlines
  SpiCommand(OP_DECODEMODE, 0);         // Display data as is, no chip decoding
  SpiCommand(OP_SHUTDOWN, 1);           // Make sure display is turned on
  SpiCommand(OP_INTENSITY, BRIGHTNESS); // Set the brighness to the desired value

  // Intialize MAX7129's again
  delay(100);
  SpiCommand(OP_DISPLAYTEST, 0);        // Disable test mode
  SpiCommand(OP_SCANLIMIT, 7);          // Use all scanlines
  SpiCommand(OP_DECODEMODE, 0);         // Display data as is, no chip decoding
  SpiCommand(OP_SHUTDOWN, 1);           // Make sure display is turned on
  SpiCommand(OP_INTENSITY, BRIGHTNESS); // Set the brighness to the desired value
  PrintString(0,(char *)"        ");  Refresh(); delay(500);
  PrintString(0,(char *)" BUTTER ");  Refresh(); delay(1800);
  PrintString(0,(char *)"        ");  Refresh(); delay(200); 
  PrintString(0,(char *)"   BY   ");  Refresh(); delay(1800);
  PrintString(0,(char *)"        ");  Refresh(); delay(200); 
  PrintString(0,(char *)"  MATS  ");  Refresh(); delay(1800);
  PrintString(0,(char *)"        ");  Refresh(); delay(200); 
  PrintString(0,(char *)"ENGSTROM");  Refresh(); delay(1800);
  PrintString(0,(char *)"        ");  Refresh(); delay(200); 

  digitalWrite(PIN_MIDLED, LOW);            //  Turn off the red LED when initialized
}

//
//
//
void loop() {
  uint8_t keys;

  keys=AttractMode();
  switch(keys) {
    case KEY_LEFT:  GamePingPong(); break;
    case KEY_MID:   GameHitIt(); break;
    case KEY_RIGHT: GameReaction(); break;
    case KEY_A:     GameCopy(); break;
    case KEY_B:     GameDestroy(); break;
  }
}

