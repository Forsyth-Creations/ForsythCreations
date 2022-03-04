//----------------- Color Define Statements --------------
#define BLUE_COLOR      0xff0000ff
#define RED_COLOR       0xffff0000
//#define YELLOW_COLOR  0xffffcf00
#define YELLOW_COLOR    0xffbdb900
#define OFF_COLOR       0xff000000
#define WHITE_COLOR     0xffFFFF88

//---------------- Pin Define Statements -----------------
#define PIN             6 // Neopixel Pin
#define BUNCHES         8
#define LEFT_HEADLIGHT  2
#define RIGHT_HEADLIGHT 3
#define SWITCH          4 
#define DIAL            5

//------------- Delay Times for Various Methods ----------
#define OFF        1300;
#define DAY_TIME   1700;
#define NIGHT_TIME 2000;
#define DELAYVAL   100 // Time (in milliseconds) to pause between pixels
#define DELAYVAL2  75 // Time (in milliseconds) to pause between pixels
#define DELAYVAL3  75 // Time (in milliseconds) to pause between pixels

//------------ Define Number of Neopixels ----------------
#define AMOUNT_PER_BUNCH 8
#define NUMPIXELS        AMOUNT_PER_BUNCH*BUNCHES // Number of Neopixels total

//---------- Defines the Heartbeat -----------------------
#define MODULUS    10
