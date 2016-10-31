#include "ui.h"

#define CORRECTBEEPTIME   50  // tone duration when pressing the right button

#define ERRORBEEPTIME     100 // Tone duration when pressing the wrong button
#define ERRORTONE         100 // Low error beep frequency
#define ERRORFLASHTIME    10  // Duration of the all-led-flash when pressing the wrong button
#define ERRORDEDUCTION    5   // Score to deduct when pressing the wrong button

uint8_t GetTarget() {
  static uint8_t targets[10];
  static uint8_t no=10;       // Start with out-of-targets
  uint8_t i,p1,p2,t;
  
  // Create new targets when out-of-targets
  if (no==10) {
    targets[0]=0;
    targets[1]=0;
    targets[2]=0;
    targets[3]=0;
    targets[4]=0;
    targets[5]=2;
    targets[6]=2;
    targets[7]=2;
    targets[8]=2;
    targets[9]=1;
    // Shuffle the target array for a bit
    for (i=0; i<100; i++) {
      p1=rand()%10;
      p2=rand()%10;
      t=targets[p1];
      targets[p1]=targets[p2];
      targets[p2]=t;
    }
    no=0;
  }

  // Return first unused target
  return targets[no++];  
}


//
//
//
void GameDestroy() {
  uint32_t timeout;
  bool fail;
  bool gotkey;
  int16_t score;
  uint8_t target;
  uint8_t keys;
  
  Lightup(LIGHT_OFF);
  Refresh();      
  delay(1500);
  Lightup(LIGHT_ALL_OFF);
  Refresh();      
  delay(1500);

  timeout=millis()+30000;
  score=0;
  InitDebouncedKeys(0);
  do {
    target=GetTarget();
    ShowTarget(target);

    gotkey=false;
    uint32_t keytime=millis();
    do {
      uint32_t points=(millis()-keytime)/100;
      if (points>10) points=10;
      points=11-points;
      Lightup(LIGHT_DISP_OFF);
      PrintNum(2,score+points,4);
      Refresh();
      keys=GetDebouncedKeys();
      switch(keys) {
        case KEY_LEFT:
          if (target==0) {
            gotkey=true;
            score+=points; 
            Indicate(LIGHT_LEFT, CORRECTBEEPTIME);
          } else {
            score-=ERRORDEDUCTION;
            if (!silent) tone(3, ERRORTONE, ERRORBEEPTIME);
            Lightup(LIGHT_ON);
            Refresh();
            delay(ERRORFLASHTIME);
            ShowTarget(target);
          }
          break;
        case KEY_MID: 
          if (target==1) {
            gotkey=true;
            score+=points; 
            Indicate(LIGHT_MID,CORRECTBEEPTIME);
          } else {
            score-=ERRORDEDUCTION;
            if (!silent) tone(3, ERRORTONE, ERRORBEEPTIME);
            Lightup(LIGHT_ON);
            Refresh();
            delay(ERRORFLASHTIME);
            ShowTarget(target);
          }
          break;
        case KEY_RIGHT: 
          if (target==2) {
            gotkey=true;
            score+=points; 
            Indicate(LIGHT_RIGHT, CORRECTBEEPTIME);
          } else {
            score-=ERRORDEDUCTION;
            if (!silent) tone(3, ERRORTONE, ERRORBEEPTIME);
            Lightup(LIGHT_ON);
            Refresh();
            delay(ERRORFLASHTIME);
            ShowTarget(target);
          }
          break;
      }
      if (score<0) score=0;
    } while (!gotkey);

    Lightup(LIGHT_OFF);
    Refresh();

    // Wait for release
    while(GetDebouncedKeys()>0) {};

  } while (millis()<timeout);

  


  Refresh();
  delay(4000);
}


//
//
//
void ShowTarget(uint8_t no) {
  Lightup(LIGHT_OFF);
  if (no==0) Lightup(LIGHT_LEFT_LONG);
  if (no==1) Lightup(LIGHT_MID_LONG);
  if (no==2) Lightup(LIGHT_RIGHT_LONG);
  Refresh();
}

