# Butter
Five games made out of a row of 64 LEDs and 
eight 7-segment displays running on a Arduino Nano.


## PingPong
This a version of the two player 1-D pong game where the ball is bounced back and forth between the players by hitting their respective buttons.

In the middle of the playfield there is a fence that the ball must clear before it can be shot back to the other player. If the ball is hit while inside the fence zone the player loses the round giving a point to the other player.

The ball is returned at a higher speed if the button is pressed as close to the end of the playfield as possible. The speed of the ball are also increased step by step every third hit. After a number of hits the fence area is also growing in size making the round harder and harder until one of the players is making a mistake.

The first player getting 5 points wins the game.

#### Player controls
The **Left Pink** button starts the game.  
Player 1 uses the **Left Pink** button.  
Player 2 uses the **Right Pink** button.

## CopyTone
This is the classic *Simon* game where the player have to memorize and repeat an ever-growing sequence of tones being played. 

When the game starts it plays a single tone, lighting up the LEDs close to the a button, and then waits for the player to press that button. It then moves on to the next round by adding another tone to the sequence, plays them both, and waits for the player to repeat the sequence.

If the player makes a mistake or is too slow (about 5 seconds between the button presses) the game is lost and the final score is displayed.

#### Player controls
The **Black** button starts the game.  
The player uses the **Left Pink**, **Green** and **Right Pink** buttons.

## Hit It
The object of this game is to hit a button as many times as possible during 30 seconds.

#### Player controls
The **Green** button starts the game.  
The player uses the **Left Pink**, **Green** or **Right Pink** buttons.

## Destroy
This is a version of the old Whack-a-Mole game where the player has as quickly as possible hit a mole that pops up randomly on the playfield.

Here the user have to press the button as soon as the LEDs close to it lights up. The faster the button is hit the more points are awarded. 5 points are deducted for hitting the wrong button. 

After 30 seconds the total score is displayed.

#### Player controls
The **Red** button starts the game.  
The player uses the **Left Pink**, **Green** or **Right Pink** buttons.

## Reaction
This is a one- or two-player version of the decades old reaction time tester.

When the game starts it beeps and flashes for a random time before starting the count-up while displaying a steady light and tone.
The player(s) then have to press their respective buttons as fast as possible. The reaction time is displayed in milliseconds (1/1000th of a second) on the display and the fastest time is indicated by flashing it.

#### Player controls
The **Right Pink** button starts the game.  
The player(s) uses the **Left Pink** and/or the **Right Pink** buttons.
