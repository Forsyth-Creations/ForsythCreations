#include "Switch.h"

Switch::Switch()
{
  
}

boolean Switch::setPin(int pin)
{
   _pin = pin;
  pinMode(pin, INPUT);
  Serial.println("Creating PWM Object on pin: " + String(_pin));
  return true;
}

void Switch::setSubdivisions(int values)
{
  _subdivisions = values;
}

int Switch::whichSubdivision()
{
  int PWM = pulseIn(_pin, HIGH, 25000);
  if (PWM != 0)
  {
  int subrange = (upperRange - lowerRange) / _subdivisions;
  //Serial.println("PWM IS: " + String(PWM) + " AND PIN IS " + String(_pin) + " AND SUBRANGE IS " + String(subrange) + " Of the subranges " + String(_subdivisions));
  for (int i = 0; i < _subdivisions; i++)
  {
    if ((PWM > (lowerRange + (subrange * i))) && (PWM < (lowerRange + (subrange * (i+1)))))
    {
      return i;
    }
  }
  }
  return -1;
}
