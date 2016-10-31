#include "ui.h"


//
//
//
void GameReaction() {
  uint8_t keys;
  uint16_t m;  
  unsigned long timeToStart;
  uint16_t player1Time;
  uint16_t player2Time;
  uint8_t stop1;
  uint8_t stop2;
  uint8_t flash;
  uint8_t i;

  Lightup(LIGHT_OFF);
  Refresh();
  
  // Wait a random interval between 3 and 8 seconds while emitting short
  // beeps and flashing the red middle LEDs
  timeToStart=millis()+3000+rand()%5000;
  while (millis()<timeToStart) {
    m=millis()%300;
    if (m>0 && m<5) {
      digitalWrite(PIN_MIDLED, HIGH);
      if (!silent) tone(3,500);
    }
    if (m>20 && m<25) {
      digitalWrite(PIN_MIDLED, LOW);
      noTone(3);
    }
  }

  // Check for cheating
    keys=GetKeys(0);
    if (keys!=0) {
      for (i=0; i<5; i++) {
        PrintString(0,(char *)"CHEAT   ");
        Refresh();
        for (m=400; m<2000; m+=75) {
          if (!silent) tone(3,m);
          delay(20);  
        }
        PrintString(0,(char *)"        ");
        Refresh();
        for (m=2000; m>400; m-=75) {
          if (!silent) tone(3,m);
          delay(20);  
        }
      }
    noTone(3);
    return;
    }

  // Tell players that the countdown has started
  if (!silent) tone(3,300);
  Lightup(LIGHT_ON);
  Lightup(LIGHT_DISP_OFF);

  // Countdown until reached 999ms or both players pressed their buttons
  stop1=false;
  stop2=false;
  do { 
    keys=GetKeys(0);
    if (!stop1) player1Time=millis()-timeToStart;
    if (!stop2) player2Time=millis()-timeToStart;
    if (keys&KEY_LEFT) stop1=true;
    if (keys&KEY_RIGHT) stop2=true;
    if (player1Time>=999) {
      player1Time=999;
      stop1=true;
    }
    if (player2Time>=999) {
      player2Time=999;
      stop2=true;
    }
    PrintNum(5,player1Time,3);
    PrintNum(0,player2Time,3);
    Refresh();
  } while (!stop1 || !stop2);

  // Turn off LEDs and silence tone
  Lightup(LIGHT_OFF);
  noTone(3);

  // Highlight winner for 5 seconds by flashing it. Times of 999 ms
  // is shown as ---
  for (i=0; i<8*5; i++) {
    if (player1Time==999) {
      PrintString(0,(char *)"---");
    } else {
      PrintNum(5,player1Time,3);
    }
    if (player2Time==999) {
      PrintString(5,(char *)"---");
    } else {
      PrintNum(0,player2Time,3);
    }
    Lightup(LIGHT_OFF);
    Refresh();
    delay(100);
    flash=0;
    if (player1Time<999 && player1Time<=player2Time) flash|=1;
    if (player2Time<999 && player2Time<=player1Time) flash|=2;
    if (flash&1) {
      Lightup(LIGHT_LEFT);
      PrintString(0,(char *)"   ");
    }
    if (flash&2) {
      Lightup(LIGHT_RIGHT);
      PrintString(5,(char *)"   ");
    }
    Refresh();
    delay(50);
  }

}

