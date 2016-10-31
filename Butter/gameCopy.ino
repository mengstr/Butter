#include "ui.h"


//
//
//
void GameCopy() {
  uint8_t sequence[32];
  uint8_t currentLength;
  uint8_t r;
  uint16_t i;
  uint8_t keys;
  uint8_t failed;
  uint8_t gothit;
  uint8_t elapsed;
  
  // Initialize the tone sequence randomly
  for (i=0; i<sizeof(sequence); i++) {
    r=rand()%100;
    switch (r) {
      case  0 ... 33: sequence[i]=0; break;
      case 34 ... 66: sequence[i]=1; break;
      case 67 ... 99: sequence[i]=2; break;
    }
  }

  Lightup(LIGHT_OFF);
  Refresh();      
  delay(1500);
  
  // Main loop of the copy/simon game
  for (currentLength=1; currentLength<sizeof(sequence); currentLength++) {
    // Show the round number for a while, then a short delay
    PrintString(0,(char *)"Round   ");
    PrintNum(0,currentLength,2);
    Refresh();      
    delay(1500);
    Lightup(LIGHT_ALL_OFF);
    Refresh();      
    delay(500);

    // Play sequence
    PlayCurrentSequence(sequence, currentLength);
    InitDebouncedKeys(0);

    // Let the player repeat the entire sequence back
    for (i=0; i<currentLength; i++) {
      gothit=false;
      failed=false;
      uint32_t timeout=millis()+5000;
      memset(bitbuf+8,0x80,8);
      do {
        keys=GetDebouncedKeys();
        Lightup(LIGHT_OFF);
        if (keys&KEY_LEFT) {
          gothit=true;
          if (sequence[i]!=0) failed=true;
          Indicate(LIGHT_LEFT,0);
        }
        if (keys&KEY_MID) {
          gothit=true;
          if (sequence[i]!=1) failed=true;
          Indicate(LIGHT_MID,0);
        }
        if (keys&KEY_RIGHT) {
          gothit=true;
          if (sequence[i]!=2) failed=true;
          Indicate(LIGHT_RIGHT,0);
        }

        // Remove DP's from the display as timeout approaches
        elapsed=(timeout-millis())/555;
        if (elapsed==0) bitbuf[15]=0;
        if (elapsed==1) bitbuf[14]=0;
        if (elapsed==2) bitbuf[13]=0;
        if (elapsed==3) bitbuf[12]=0;
        if (elapsed==4) bitbuf[11]=0;
        if (elapsed==5) bitbuf[10]=0;
        if (elapsed==6) bitbuf[9]=0;
        if (elapsed==7) bitbuf[8]=0;
        Refresh();
        
        // If user failed or is timeout then play sad sound and
        // display score for a while.
        if (failed || millis()>timeout) {
          PrintString(0,(char *)"  FAIL  ");
          Refresh();
          for (i=400; i>50; i--) {
            if (!silent) tone(3,i);
            delay(10);
          }
          noTone(3);
          Lightup(LIGHT_ALL_OFF);
          PrintString(0,(char *)"SCORE");
          if (currentLength-1<10) {
            PrintNum(1,currentLength-1,1);
          } else {
            PrintNum(0,currentLength-1,2);
          }
          Refresh();
          delay(4000);
          return;
        }

      } while (!(gothit==true && keys==0));
      noTone(3);
  
    }

    // User entered the sequence correct. Wait for a bit and make it longer
    Lightup(LIGHT_ALL_OFF);
    Refresh();      
    delay(1500);
  }
  
}


//
//
//
void PlayCurrentSequence(uint8_t *sequence ,uint8_t len) {
  uint8_t i;

  for (i=0; i<len; i++) {
    if (*sequence==0) Indicate(LIGHT_LEFT, 0);
    if (*sequence==1) Indicate(LIGHT_MID, 0);
    if (*sequence==2) Indicate(LIGHT_RIGHT, 0);
    sequence++;

    Refresh();
    delay(500);
    noTone(3);
    Lightup(LIGHT_OFF);
    Refresh();
    delay(250);
  }
}

