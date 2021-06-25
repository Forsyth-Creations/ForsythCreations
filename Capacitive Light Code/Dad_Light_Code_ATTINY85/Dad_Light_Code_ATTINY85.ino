#include <CapacitiveSensor.h>
CapacitiveSensor cs_4_8 = CapacitiveSensor(0, 1); //10M Resistor between pins 0 and 1, you may also connect an antenna on pin 1
unsigned long csSum;
boolean on = false;


#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 15 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //Serial.begin(9600);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();
  pixels.show();

}

void loop() {
  if (handTouchedLight())
  {
    on = !on;
    digitalWrite(3, on);
    neoToggle();
    delay(200);
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
  long firstReading = cs_4_8.capacitiveSensor(80); 
  delay(150);//the time to update
  long secondReading = cs_4_8.capacitiveSensor(80); 
  long slope = ((secondReading - firstReading)/20); //the 100 acts as an arbitrary scalar to provide greater sensititvity
  //Serial.println(slope);
  if (slope > 10) //some arbitrary value to set tolerance. 
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
