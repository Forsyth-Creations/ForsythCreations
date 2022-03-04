#include "Light.h"

Light::Light()
  : pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800)
{
}

void Light::init()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
  pinMode(LEFT_HEADLIGHT, OUTPUT);
  pinMode(RIGHT_HEADLIGHT, OUTPUT);
  digitalWrite(LEFT_HEADLIGHT, HIGH);
  digitalWrite(RIGHT_HEADLIGHT, HIGH);
}

//----------------------------------------------------------------------------
void Light::headlightsOn()
{
  digitalWrite(LEFT_HEADLIGHT, LOW);
  digitalWrite(RIGHT_HEADLIGHT, LOW);
}

void Light::headlightsOff()
{
  digitalWrite(LEFT_HEADLIGHT, HIGH);
  digitalWrite(RIGHT_HEADLIGHT, HIGH);
}

void Light::headlightsRightOn()
{
  digitalWrite(LEFT_HEADLIGHT, !HIGH);
  digitalWrite(RIGHT_HEADLIGHT, HIGH);
}

void Light::headlightsLeftOn()
{
  digitalWrite(LEFT_HEADLIGHT, HIGH);
  digitalWrite(RIGHT_HEADLIGHT, !HIGH);
}

void Light::neoRightOn()
{
  ledRange(0, 31, activeColor1);
  ledRange(32, 63, activeColor2);
}

void Light::neoLeftOn()
{
  ledRange(0, 31, activeColor2);
  ledRange(32, 63, activeColor1);
}

void Light::batchesOfEight(uint32_t c1, uint32_t c2)
{
  swap = false;
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    if (i % BUNCHES == 0)
    {
      swap = !swap;
      Serial.println("in swap");
    }
    if (swap)
    {
      pixels.setPixelColor(i, c1);
    }
    else
    {
      pixels.setPixelColor(i, c2);
    }
  }
}

void Light::ledRange(int first, int second, uint32_t  color1)
{
  for (int i = first; i <= second; i++)
  {
    pixels.setPixelColor(i, color1);
  }
  //pixels.show();   // Send the updated pixel colors to the hardware.
}

void Light::setActiveColors(uint32_t  color1, uint32_t  color2)
{
  activeColor1 = color1;
  activeColor2 = color2;
}

uint32_t Light::getActiveColor1()
{
  return activeColor1;
}

uint32_t Light::getActiveColor2()
{
  return activeColor2;
}

void Light::setAllOff()
{
  for (int i = 0; i <= 63; i++)
  {
    pixels.setPixelColor(i, 0);
  }
}

void Light::showNeo()
{
    pixels.show();   // Send the updated pixel colors to the hardware.
}
