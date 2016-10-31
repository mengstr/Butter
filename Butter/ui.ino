#include "ui.h"

// Variables to handling the debouncing of LEFT/MID/RIGHT buttons
static uint8_t keydownfor1,keydownfor2,keydownfor3;
static bool canCount1,canCount2,canCount3;
static bool debounced1,debounced2,debounced3;
static uint8_t cnt1,cnt2,cnt3;
static uint32_t lastmillis1,lastmillis2,lastmillis3;


// 8 bytes for the 7-segments plus 64 bits (8 bytes) for LEDs
byte bitbuf[16];                

// Reorders the LED due to a fsckup in the wiring ;-(
byte reorderleds[256] = {
  0x00, 0x40, 0x20, 0x60, 0x10, 0x50, 0x30, 0x70, 0x08, 0x48, 0x28, 0x68, 0x18, 0x58, 0x38, 0x78,
  0x04, 0x44, 0x24, 0x64, 0x14, 0x54, 0x34, 0x74, 0x0C, 0x4C, 0x2C, 0x6C, 0x1C, 0x5C, 0x3C, 0x7C,
  0x02, 0x42, 0x22, 0x62, 0x12, 0x52, 0x32, 0x72, 0x0A, 0x4A, 0x2A, 0x6A, 0x1A, 0x5A, 0x3A, 0x7A,
  0x06, 0x46, 0x26, 0x66, 0x16, 0x56, 0x36, 0x76, 0x0E, 0x4E, 0x2E, 0x6E, 0x1E, 0x5E, 0x3E, 0x7E,
  0x01, 0x41, 0x21, 0x61, 0x11, 0x51, 0x31, 0x71, 0x09, 0x49, 0x29, 0x69, 0x19, 0x59, 0x39, 0x79,
  0x05, 0x45, 0x25, 0x65, 0x15, 0x55, 0x35, 0x75, 0x0D, 0x4D, 0x2D, 0x6D, 0x1D, 0x5D, 0x3D, 0x7D,
  0x03, 0x43, 0x23, 0x63, 0x13, 0x53, 0x33, 0x73, 0x0B, 0x4B, 0x2B, 0x6B, 0x1B, 0x5B, 0x3B, 0x7B,
  0x07, 0x47, 0x27, 0x67, 0x17, 0x57, 0x37, 0x77, 0x0F, 0x4F, 0x2F, 0x6F, 0x1F, 0x5F, 0x3F, 0x7F,
  0x80, 0xC0, 0xA0, 0xE0, 0x90, 0xD0, 0xB0, 0xF0, 0x88, 0xC8, 0xA8, 0xE8, 0x98, 0xD8, 0xB8, 0xF8,
  0x84, 0xC4, 0xA4, 0xE4, 0x94, 0xD4, 0xB4, 0xF4, 0x8C, 0xCC, 0xAC, 0xEC, 0x9C, 0xDC, 0xBC, 0xFC,
  0x82, 0xC2, 0xA2, 0xE2, 0x92, 0xD2, 0xB2, 0xF2, 0x8A, 0xCA, 0xAA, 0xEA, 0x9A, 0xDA, 0xBA, 0xFA,
  0x86, 0xC6, 0xA6, 0xE6, 0x96, 0xD6, 0xB6, 0xF6, 0x8E, 0xCE, 0xAE, 0xEE, 0x9E, 0xDE, 0xBE, 0xFE,
  0x81, 0xC1, 0xA1, 0xE1, 0x91, 0xD1, 0xB1, 0xF1, 0x89, 0xC9, 0xA9, 0xE9, 0x99, 0xD9, 0xB9, 0xF9,
  0x85, 0xC5, 0xA5, 0xE5, 0x95, 0xD5, 0xB5, 0xF5, 0x8D, 0xCD, 0xAD, 0xED, 0x9D, 0xDD, 0xBD, 0xFD,
  0x83, 0xC3, 0xA3, 0xE3, 0x93, 0xD3, 0xB3, 0xF3, 0x8B, 0xCB, 0xAB, 0xEB, 0x9B, 0xDB, 0xBB, 0xFB,
  0x87, 0xC7, 0xA7, 0xE7, 0x97, 0xD7, 0xB7, 0xF7, 0x8F, 0xCF, 0xAF, 0xEF, 0x9F, 0xDF, 0xBF, 0xFF
};

// Bitmaps of the ASCII characters on the 7-segment displays - some chars looks really ugly   
const byte charTable [128] PROGMEM  = {
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, //
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, //
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, //
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, //
  B00000000, B10100000, B00100010, B00000000, B00000000, B00000000, B00000000, B00000010, //  !"#$%&'
  B01001110, B01111000, B00000000, B00000000, B00000000, B00000001, B10000000, B00100101, // ()*+,-./
  B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, // 01234567
  B01111111, B01111011, B00000000, B00000000, B01001110, B00001001, B01010011, B01100101, // 89:;<=>?
  B00000000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B01011110, // @ABCDEFG
  B00010111, B00000100, B00111100, B01010111, B00001110, B01110110, B00010101, B00011101, // HIJKLMNO
  B01100111, B01110011, B00000101, B01011011, B00001111, B00011100, B00111110, B00111110, // PQRSTUVW
  B00110111, B00111011, B01101101, B01001110, B00010011, B01111000, B01000000, B00001000, // XYZ[/]^_
  B00000010, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B01011110, // `abcdefg
  B00010111, B00000100, B00111100, B01010111, B00001110, B01110110, B00010101, B00011101, // hijklmno
  B01100111, B01110011, B00000101, B01011011, B00001111, B00011100, B00111110, B00111110, // pqrstuvw
  B00110111, B00111011, B01101101, B01001110, B00000110, B01111000, B01100010, B00000000 // xyz{|}~
};


//
// Dumps all 128 bits of the bitbuf into the MAX7219 drivers so both the LEDs and 
// 7-segment displays gets updated with current data
//
void Refresh() {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(i + 1);
    SPI.transfer(reorderleds[bitbuf[i]]);
    SPI.transfer(i + 1);
    SPI.transfer(bitbuf[i + 8]);
    digitalWrite(SPI_CS, HIGH);     //latch the data
  }
}

//
// Send the same command/data to both MAX7219's
//
void SpiCommand(uint8_t cmd, uint8_t data) {
    digitalWrite(SPI_CS, LOW);
    SPI.transfer(cmd);
    SPI.transfer(data);
    SPI.transfer(cmd);
    SPI.transfer(data);
    digitalWrite(SPI_CS, HIGH);     //latch the data
}


//
// Looks up the bitmap in the ASCII character table and puts the data
// into the right place in the bitbuf so it can be displayed at the
// next refresh
//
void SetChar(int digit, char ch, boolean dp) {
  byte index, v;

  index = ((byte)ch)&0x7F;
  v = pgm_read_byte_near(charTable + index);
  if (dp) v |= B10000000;
  bitbuf[8+digit] = v;
}



//
// Prints a string starting at position "p" onto the 7-segment display.
//
void PrintString(int p, char *s) {
  int i;
  for (i = 0; i < strlen(s); i++) {
    SetChar(7 - (p + i), s[i], false);
  }
}


//
// Prints a number at position p on the 7-segment displays. The number
// is formatted with "maxdigits" number leading zeros. 
//
void PrintNum(int p, long num, int maxdigits) {
  int v;
  int i;

  for (i = 0; i < maxdigits; i++) {
    v = num % 10;
    num /= 10;
    SetChar(p++, '0' + v, false);
  }
}


//
//
//
void Lightup(int8_t mode) {
  switch (mode) {
    case LIGHT_DISP_OFF:
      memset(bitbuf+8,0,8);
      break;
    case LIGHT_ON:
      memset(bitbuf,0xFF,8);
      break;
    case LIGHT_OFF:
      memset(bitbuf,0,8);
      break;  
    case LIGHT_ALL_OFF:
      memset(bitbuf,0,16);
      break;  
    case LIGHT_LEFT:
      bitbuf[0]=0x0F;
      break;
    case LIGHT_LEFT_LONG:
      bitbuf[0]=0xFF;
      break;
    case LIGHT_MID: 
      bitbuf[3]=0xC0;
      bitbuf[4]=0x03;
      break;
    case LIGHT_MID_LONG: 
      bitbuf[3]=0xF0;
      bitbuf[4]=0x0F;
      break;
    case LIGHT_RIGHT:
      bitbuf[7]=0xF0;
      break;
    case LIGHT_RIGHT_LONG:
      bitbuf[7]=0xFF;
      break;
    case LIGHT_A: 
      bitbuf[3]=0x0F;
      break;
    case LIGHT_B: 
      bitbuf[4]=0xF0;
      break;
  }
}


//
// Checks for any pressed keys while waiting "duration" millisconds.
// Returns 0 if no key was pressed before the time is up, else returns
// a bitmap marking the currently pressed keys.
//
uint8_t GetKeys(int duration) {
  unsigned long endtime=millis()+duration;
  uint8_t keys=0;

  do {
    keys=rand();
    keys=0;
    if (digitalRead(PIN_LEFT) == LOW) keys|=KEY_LEFT;
    if (digitalRead(PIN_RIGHT) == LOW) keys|=KEY_RIGHT;
    if (digitalRead(PIN_MID) == LOW) keys|=KEY_MID;
    if (digitalRead(PIN_A) == LOW) keys|=KEY_A;
    if (digitalRead(PIN_B) == LOW) keys|=KEY_B;
  } while (keys==0 && millis()<endtime);

  return keys;
}


//
//
//
void InitDebouncedKeys(uint8_t mode) {
  switch (mode) {
    case 0:
      lastmillis1=lastmillis2=lastmillis3=0;
      keydownfor1=keydownfor2=keydownfor3=0;
      canCount1=canCount2=canCount3=true;
      debounced1=debounced2=debounced3=false;
      cnt1=cnt2=cnt3=0;
      break;
     case 1:
      cnt1=0;
      break;
     case 2:
      cnt2=0;
      break;
     case 3:
      cnt3=0;
      break;
  }
        
}


//
//
//
uint8_t GetDebouncedKeys() {
  uint8_t keys=0;

  if (lastmillis1!=millis()) {
    lastmillis1=millis();
    if (GetKeys(0)&KEY_LEFT) {
      keydownfor1++;
      if (keydownfor1>10) {
        keydownfor1=10;
        if (canCount1) {
          canCount1=false;
          debounced1=true;
          cnt1++;
        }
      }
    } else {
      if (keydownfor1==0) {
        canCount1=true;
        debounced1=false;
      }
      if (keydownfor1>0) keydownfor1--;
    }
  }

  if (lastmillis2!=millis()) {
    lastmillis2=millis();
    if (GetKeys(0)&KEY_MID) {
      keydownfor2++;
      if (keydownfor2>10) {
        keydownfor2=10;
        if (canCount2) {
          canCount2=false;
          debounced2=true;
          cnt2++;
        }
      }
    } else {
      if (keydownfor2==0) {
        canCount2=true;
        debounced2=false;
      }
      if (keydownfor2>0) keydownfor2--;
    }
  }

  if (lastmillis3!=millis()) {
    lastmillis3=millis();
    if (GetKeys(0)&KEY_RIGHT) {
      keydownfor3++;
      if (keydownfor3>10) {
        keydownfor3=10;
        if (canCount3) {
          canCount3=false;
          debounced3=true;
          cnt3++;
        }
      }
    } else {
      if (keydownfor3==0) {
        canCount3=true;
        debounced3=false;
      }
      if (keydownfor3>0) keydownfor3--;
    }
  }

  if (debounced1) keys|=KEY_LEFT;
  if (debounced2) keys|=KEY_MID;
  if (debounced3) keys|=KEY_RIGHT;

  return keys;
}


//
//
//
uint8_t GetDebouncedCnt(uint8_t no) {
  if (no==1) return cnt1;
  if (no==2) return cnt2;
  if (no==3) return cnt3;
  return 0;
}


//
//
//
void Indicate(uint8_t what, uint16_t soundDuration) {
  Lightup(what);
  if (!silent) {
 
    if (what==LIGHT_LEFT || what==LIGHT_LEFT_LONG) {
      if (soundDuration==0) { 
        tone(3,TONE_LEFT); 
      } else {
        tone(3,TONE_LEFT,soundDuration);
      }
    }
    if (what==LIGHT_MID || what==LIGHT_MID_LONG) {
      if (soundDuration==0) { 
        tone(3,TONE_MID); 
      } else {
        tone(3,TONE_MID,soundDuration);
      }
    }
    if (what==LIGHT_RIGHT || what==LIGHT_RIGHT_LONG) {
      if (soundDuration==0) { 
        tone(3,TONE_RIGHT); 
      } else {
        tone(3,TONE_RIGHT,soundDuration);
      }
    }

  }
}

