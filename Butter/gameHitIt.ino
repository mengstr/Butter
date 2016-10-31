#include "ui.h"

//
//
//
void GameHitIt() {
  uint16_t cnt;
  unsigned long timeToEnd;
  uint8_t keydownfor=0;
  bool canCount=true;
  unsigned long lastmillis;
  uint16_t led;
  uint16_t lastled=0;
  
  Lightup(LIGHT_OFF);
  Refresh();
  
  // Count down from 9 to 0 while emitting short
  // beeps and flashing the red middle LEDs
  for (cnt=9; cnt>0; cnt--) {
    PrintNum(0,cnt,1);
    Refresh();
    digitalWrite(PIN_MIDLED, HIGH);
    if (!silent) tone(3,500);
    delay(50);
    digitalWrite(PIN_MIDLED, LOW);
    noTone(3);
    delay(950);
  }
  
  // Set all LEDs, clear display, and start the 30 seconds of gameplay
  Lightup(LIGHT_ON);
  Lightup(LIGHT_DISP_OFF);
  timeToEnd=millis()+30000;
  do { 
    // Make the LED bar go away one step approximately every 469 ms and also
    // make a descending sound as the bar is shrinking
    led=timeToEnd-millis();
    led/=469;
    bitbuf[led/8]&=~(1<<(led&0x07));
    if (lastled!=led) {
      lastled=led;
      if (!silent) tone(3,250+led*13,20);
    }
      
    // Display the hit count
    if (cnt<10) {
      PrintNum(3,cnt,1);
    } else if (cnt<100) {
      PrintNum(3,cnt,2);
    } else if (cnt<1000) {
      PrintNum(3,cnt,3);
    }
    Refresh();
    
    // Check for debounced button hits and increase hit count as nessecary
    if (lastmillis!=millis()) {
      lastmillis=millis();
      if (GetKeys(0)!=0) {
        keydownfor++;
        if (keydownfor>10) {
          keydownfor=10;
          if (canCount) {
            canCount=false;
            cnt++;
          }
        }
      } else {
        if (keydownfor==0) canCount=true;
        if (keydownfor>0) keydownfor--;
      }
    }

  } while (millis()<timeToEnd);
  
  delay(5000);
  
}

