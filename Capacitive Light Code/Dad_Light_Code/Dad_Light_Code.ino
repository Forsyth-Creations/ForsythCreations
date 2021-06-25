#include <CapacitiveSensor.h>
CapacitiveSensor cs_4_8 = CapacitiveSensor(4, 8); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
unsigned long csSum;
boolean on = false;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        2 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 15 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

}

void loop() {
  checkLight();
}

void checkLight()
{
  if (handTouchedLight())
  {
    neoToggle();
    Serial.println("Hand Touched");
  }
}

//Check capacitance reading. If there is a large, positive slope
//then a hand touched the light and it should toggle. Else
//just wait for the hand, constantly updating the current capacitance reading
//every few seconds to check the slope
//update every 20 ms?

boolean handTouchedLight()
{
  boolean returnVal = false;
  int firstReading = cs_4_8.capacitiveSensor(80); 
  delay(20);//the time to update
  int secondReading = cs_4_8.capacitiveSensor(80); 
  int slope = (secondReading - firstReading)/20;
  Serial.println(slope);
  if (slope > 1) //some arbitrary value to set tolerance. 
  {
    return true;
  }
  return false;
}

long settingNumber = 0;
const long BRIGHTNESS = 85;

void neoToggle()
{
  settingNumber++;
  if (settingNumber >= 4) //number of settings you want plus 1
  {
    settingNumber = 0;
  }
  for (int i = 0; i < NUMPIXELS / 3; i++) {
    pixels.setPixelColor(i, pixels.Color(BRIGHTNESS * settingNumber, BRIGHTNESS * settingNumber, 0));
    pixels.setPixelColor(9 - i, pixels.Color(BRIGHTNESS * settingNumber, BRIGHTNESS * settingNumber, 0));
    pixels.setPixelColor(10 + i, pixels.Color(BRIGHTNESS * settingNumber, BRIGHTNESS * settingNumber, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
  }
}
