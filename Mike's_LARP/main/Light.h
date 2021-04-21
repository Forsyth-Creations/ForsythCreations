#ifndef Light_h
#define Light_h
#include "Arduino.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "defines.h"

class Light
{
  public:
    Light();
    void init();
    void batchesOfEight(uint32_t c1, uint32_t c2);
    void basicSequencer(uint32_t color1, uint32_t color2);
    void ledRange(int first, int second, uint32_t color1);
    void headlightsOn();
    void headlightsOff();
    void headlightsRightOn();
    void headlightsLeftOn();
    void setActiveColors(uint32_t  color1, uint32_t  color2);
    void neoRightOn();
    void neoLeftOn();
    uint32_t getActiveColor1();
    uint32_t getActiveColor2();
    void setAllOff();
    void showNeo();
  private:
    Adafruit_NeoPixel pixels;
    uint32_t activeColor1;
    uint32_t activeColor2;
    boolean swap;
};

#endif
