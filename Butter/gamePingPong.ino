#include "ui.h"


#define MAXSCORE      5
#define BALLSIZE      2
#define MAXFENCE      23
#define MAXSPEED      10
#define INITIALSPEED  30
#define INITIALFENCE  10

const uint8_t speedtable[32]={
  10,11,12,13,14,15,16,17,
  18,19,20,21,22,23,24,25,
  26,27,28,29,30,30,30,30,
  30,30,30,30,30,30,30,30
};


void GamePingPong() {
  uint8_t dir;
  int8_t pos;
  uint8_t keys;
  uint8_t fence;
  int8_t ballSpeed;
  uint32_t lastMove=0;
  uint8_t i,t;
  uint8_t score1, score2;
  uint8_t hits,h;
  
  score1=score2=0;
  dir=(rand()>>4)&1;

  // Main game loop, playing until one player reached MAXSCORE points.
  for (;;) {
    // Setup a random direction originating from middle
    // and wait for the ball to be released
    ballSpeed=INITIALSPEED;
    fence=0;
    pos=30;
    hits=0;
    lastMove=millis();
    do {
      Lightup(LIGHT_OFF);
      // Let the Title show for a while when game is just stared before switching
      // over to the scores
      if (millis()>lastMove+2000) {
        ShowScores(score1,score2);
      }
      DrawFence(INITIALFENCE+fence);
      // Flash the red middle leds      
      t=millis();
      t=t&64;
      if (t<5) digitalWrite(PIN_MIDLED, HIGH);
      if (t>=5) digitalWrite(PIN_MIDLED, LOW);
      Refresh();
    } while (millis()<lastMove+3000);
    digitalWrite(PIN_MIDLED, LOW);
  
    // 
    for (;;) {
      Lightup(LIGHT_OFF);
      DrawFence(INITIALFENCE+fence);
      DrawBall(pos);
      Refresh();

      keys=GetKeys(0);
      if (keys!=0 || 1==1) {
        // Going left and left button pressed?
        if (dir==0 && (keys&KEY_LEFT)) {
          // Fail ball if pressed with inside fence
          if (pos>31-(INITIALFENCE+fence)) {
            EarlyHit(&score2);
            break;
          }
          // Increase ball speed every X hits, and for even more hits
          // start increase the size of the fence
          h=hits/3;
          ballSpeed=speedtable[pos]-h;
          if (ballSpeed<MAXSPEED) ballSpeed=MAXSPEED;
          if (h>=8) fence=h-8;
          // Change direction
          dir=1;
          hits++;
          if (!silent) tone(3,1000-ballSpeed*20,100);
        }
 
        if (dir==1 && (keys&KEY_RIGHT)) {
          // Fail ball if pressed while inside fence
          if (pos<31+(INITIALFENCE+fence)) {
            EarlyHit(&score1);
            break;
          }
          // Increase ball speed every X hits, and for even more hits
          // start increase the size of the fence
          h=hits/3;
          ballSpeed=speedtable[64-BALLSIZE-pos]-h;
          if (ballSpeed<MAXSPEED) ballSpeed=MAXSPEED;
          if (h>=8) fence=h-8;
          // Change direction
          dir=0;
          hits++;
          if (!silent) tone(3,1000-ballSpeed*20,100);
        }
      }
  
      // Move the ball at desired speed, break out of current play
      // if it exited the play field
      if (millis()>lastMove+ballSpeed) {
        lastMove=millis();
        if (dir==1) {
          pos=pos+1;
          if (pos>64-BALLSIZE) {
            score1++;
            break;
          }
        } else {
          pos=pos-1;
          if (pos<0) {
            score2++;
            break;
          }
        }
      }
      
    }
    
    ShowScores(score1,score2);
    Refresh();

    // Check if game over
    if (score1==MAXSCORE || score2==MAXSCORE) break;
  }

  // Game over - show winner
  Lightup(LIGHT_OFF);
  noTone(3);

  // Highlight winner for 5 seconds by flashing it. 
  for (i=0; i<8*5; i++) {
    ShowScores(score1,score2);
    Lightup(LIGHT_OFF);
    Refresh();
    delay(100);
    if ((score1>score2)) {
      Lightup(LIGHT_LEFT);
      PrintString(0,(char *)"   ");
    }
    if (score2>score1) {
      Lightup(LIGHT_RIGHT);
      PrintString(5,(char *)"   ");
    }
    Refresh();
    delay(50);
  }


}


//
//
//
void EarlyHit(uint8_t *score) {
  if (!silent) tone(3,TONE_ERROR,500);
  PrintString(0,(char *)"EARLY   ");
  Refresh();
  delay(1500);
  (*score)++;
}

//
//
//
void DrawBall(uint8_t pos) {
  uint8_t i;
  for (i=pos; i<pos+BALLSIZE; i++) {
    bitbuf[i/8]^=1<<(i&0x07);
  }
}



//
// Draw the fence area
//
void DrawFence(uint8_t fence) {
  uint8_t i;

  if (fence>MAXFENCE) fence=MAXFENCE;
  for (i=32-fence; i<32+fence; i++) {
    bitbuf[i/8]|=1<<(i&0x07);
  }
  
}


//
//
//
void ShowScores(uint8_t score1, uint8_t score2) {
  Lightup(LIGHT_DISP_OFF);
  if (score1<10) PrintNum(7,score1,1); else PrintNum(6,score1,2);
  if (score2<10) PrintNum(0,score2,1); else PrintNum(0,score2,2);
}

