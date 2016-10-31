#include "ui.h"


//
//
//
const static uint8_t snakepath[] = {
  0*8+0, 1*8+0, 2*8+0, 3*8+0, 4*8+0, 5*8+0, 6*8+0, 7*8+0, 
  7*8+5, 
  7*8+6, 6*8+6, 5*8+6, 4*8+6, 3*8+6, 2*8+6, 1*8+6, 0*8+6, 
  0*8+2, 
  0*8+3, 1*8+3, 2*8+3, 3*8+3, 4*8+3, 5*8+3, 6*8+3, 7*8+3, 
  7*8+4, 
  7*8+6, 6*8+6, 5*8+6, 4*8+6, 3*8+6, 2*8+6, 1*8+6, 0*8+6, 
  0*8+1
};


//
//
//
void AttractSnake() {
  static unsigned long lastTime=0;
  static uint8_t h=0;
  static uint8_t t=sizeof(snakepath)-8;
  uint8_t i=1;
  
  if (lastTime>millis()-60) return;
  lastTime=millis();
  i=snakepath[h++];
  if (h==sizeof(snakepath)) h=0;
  bitbuf[8+i/8] |= (64 >> (i & 0x07));
  i=snakepath[t++];
  if (t==sizeof(snakepath)) t=0;
  bitbuf[8+i/8] &= ~(64 >> (i & 0x07));
}


//
//
//
bool AttractGlitter(int duration) {
  unsigned long endtime=millis()+duration;
  uint8_t i;

  while (millis()<endtime) {
    i = (rand() >> 8) & 63;
    bitbuf[i/8] |= (1<<(i&0x07));
    bitbuf[8+(i/8)] |= (1<<(i&0x07));
    Refresh();
    bitbuf[i/8] &= ~(1<<(i&0x07));
    bitbuf[8+(i/8)] &= ~(1<<(i&0x07));
    if (GetKeys(30)>0) return true;
  }
  return false;  
}


//
//
//
bool AttractSweep(int duration) {
  unsigned long endtime=millis()+duration;
  int8_t i;

  while (millis()<endtime) {
    for (i=0; i<64; i++) {
      AttractSnake();
      bitbuf[i/8] |= (1 << (i & 0x07));
      Refresh();
      if (GetKeys(5)>0) return true;
    }
    for (i=0; i<64; i++) {
      AttractSnake();
      bitbuf[i/8] &= ~(1 << (i & 0x07));
      Refresh();
      if (GetKeys(5)>0) return true;
    }
    for (i=63; i>=0; i--) {
      AttractSnake();
      bitbuf[i/8] |= (1 << (i & 0x07));
      Refresh();
      if (GetKeys(5)>0) return true;
    }
    for (i=63; i>=0; i--) {
      AttractSnake();
      bitbuf[i/8] &= ~(1 << (i & 0x07));
      Refresh();
      if (GetKeys(5)>0) return true;
    }
  }
  return false;  
}


//
//
//
void ShowGameName(uint8_t no) {
  if (no==KEY_LEFT) {
    Lightup(LIGHT_LEFT);
    PrintString(0,(char *)"PINGPONG");
  }
  if (no==KEY_RIGHT) {
    Lightup(LIGHT_RIGHT);
    PrintString(0,(char *)"REACTION");
  }
  if (no==KEY_MID) {
    bitbuf[3]=0xC0;
    bitbuf[4]=0x03;
    PrintString(0,(char *)"HIT IT");
  }
  if (no==KEY_A) {
    Lightup(LIGHT_A);
    PrintString(0,(char *)"COPYTONE");
  }
  if (no==KEY_B) {
    Lightup(LIGHT_B);
    PrintString(0,(char *)"DESTROY");
  }
  
}

//
//
//
uint8_t AttractGames() {
  uint8_t list[]={KEY_LEFT, KEY_A, KEY_MID, KEY_B, KEY_RIGHT}; 
  uint8_t i,j;

  for (i=0; i<5; i++) {
    for (j=0; j<25; j++) {
      Lightup(LIGHT_ALL_OFF);
      ShowGameName(list[i]);
      Refresh();
      if (GetKeys(50)>0) return true;
      Lightup(LIGHT_OFF);
      Refresh();
      if (GetKeys(50)>0) return true;
    }
  } 
  Lightup(LIGHT_ALL_OFF);
  Refresh();
  return false;
}


//
//
//
uint8_t AttractMode() {
  uint8_t keys;

  Lightup(LIGHT_ALL_OFF);
  
  for (;;) {
    if (AttractSweep(5000)) break;
    if (AttractGames()) break;
    if (AttractGlitter(5000)) break;
    if (AttractGames()) break;
  }
 
  Lightup(LIGHT_ALL_OFF);
  delay(100);
  keys=GetKeys(0);
  ShowGameName(keys);  
  Refresh();
  delay(1000);
  return keys;
}

