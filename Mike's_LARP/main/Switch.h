#ifndef Switch_h
#define Switch_h
#include "Arduino.h"
#include "defines.h"

class Switch
{
  public:
    Switch();
    boolean setPin(int pin);
    void setSubdivisions(int values);
    int whichSubdivision();
  private:
    int _subdivisions = 0;
    int lowerRange = 900; //upper range of PWM
    int upperRange = 2000; //Lower Range of PWM
    int _pin = 0;
};

#endif
