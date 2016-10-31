// Opcodes for the MAX7219
#define OP_NOOP         0
#define OP_DIGIT0       1
#define OP_DIGIT1       2
#define OP_DIGIT2       3
#define OP_DIGIT3       4
#define OP_DIGIT4       5
#define OP_DIGIT5       6
#define OP_DIGIT6       7
#define OP_DIGIT7       8
#define OP_DECODEMODE   9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15

#define PIN_MIDLED      4      // The two red LEDs in the middle of the board

#define PIN_LEFT       14      // The left action button
#define PIN_RIGHT      15      // The right action button
#define PIN_MID        16      // The middle action button
#define PIN_B          17      // The right selection button
#define PIN_A          18      // The left selection button

#define SPI_MOSI       11      // Pin for MAX7219 DATA
#define SPI_CLK        13      // Pin for MAX7219 data CLOCK
#define SPI_CS         10      // Pin for MAX7219 LOAD signal


#define KEY_LEFT        1       // Bitmap values for the buttons as returned by GetKey()
#define KEY_RIGHT       2
#define KEY_MID         4
#define KEY_A           8
#define KEY_B          16

#define LIGHT_OFF         0
#define LIGHT_LEFT        1
#define LIGHT_LEFT_LONG   2
#define LIGHT_MID         3
#define LIGHT_MID_LONG    4
#define LIGHT_RIGHT       5
#define LIGHT_RIGHT_LONG  6
#define LIGHT_A           7
#define LIGHT_B           8
#define LIGHT_ON          9
#define LIGHT_ALL_OFF     10
#define LIGHT_DISP_OFF    11

#define TONE_LEFT         300
#define TONE_MID          450
#define TONE_RIGHT        600
#define TONE_ERROR        100

extern byte bitbuf[16];
extern byte reorderleds[256];
extern const byte charTable[128] PROGMEM;



