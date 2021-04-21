/*
  main.cpp - timer based Neopixel and relay lighting scheme
  Copyright (c) 2021 Henry Forysth.  All right reserved.
  This code is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
*/


#include "defines.h"
#include "Switch.h"
#include "Light.h"

Light botLight;
Switch knob;
Switch flip1;

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 60;           // interval at which to blink (milliseconds)

enum headlightStates
{
  BOTH_OFF, BOTH_ON, FLASHING
};

enum topLights
{
  ALL_OFF, ALL_ON, WHITE, YELLOW, FIRE, POLICE
};

void setup() {
  Serial.begin(9600);
  botLight.init();
  Serial.println("Starting Code");
  //delay(10000);

  knob.setPin(DIAL);
  flip1.setPin(SWITCH);

  delay(2000);

  knob.setSubdivisions(6);
  flip1.setSubdivisions(3);
}

void loop() {
  //botLight.ledRange(0, 63, 0);
  unsigned long currentMillis = millis();
  static headlightStates headLights;
  //static topLights neoLights = (topLights)knob.whichSubdivision();
  static topLights neoLights;

  //------------------- Set Top Lights --------------------------------------------------
  switch (knob.whichSubdivision())
  {
    case 0: neoLights = ALL_OFF; break;
    case 1: neoLights = ALL_ON; break;
    case 2: neoLights = WHITE; break;
    case 3: neoLights = YELLOW; break;
    case 4: neoLights = FIRE; break;
    case 5: neoLights = POLICE; break;
  }
  //Serial.println(neoLights);
  //knob.whichSubdivision();
  //------------------------ Set Head Lights --------------------------------------------
  switch (flip1.whichSubdivision())
  {
    case 0: headLights = BOTH_OFF; break;
    case 1: headLights = BOTH_ON; break;
    case 2: headLights = FLASHING; break;
  }
  static int counter = 0;
  //---------------------- Determine Active NeoPixel Colors ----------------------------------
  switch (neoLights)
  {
    case ALL_OFF:
      //botLight.setActiveColors(0, 0);
      botLight.ledRange(0, 63, 0);
      break;
    case ALL_ON:
      botLight.setActiveColors(WHITE_COLOR, WHITE_COLOR);
      botLight.ledRange(16, 31, WHITE_COLOR);
      botLight.ledRange(63 - 15, 63, RED_COLOR);
      botLight.ledRange(0 , 15, 0x00000000);
      botLight.ledRange(32, 47, 0x00000000);
      break;
    case WHITE:
      botLight.setActiveColors(WHITE_COLOR, WHITE_COLOR);
      break;
    case YELLOW:
      botLight.setActiveColors(YELLOW_COLOR, 0);
      break;
    case FIRE:
      botLight.setActiveColors(RED_COLOR, WHITE_COLOR);
      break;
    case POLICE:
      botLight.setActiveColors(RED_COLOR, BLUE_COLOR);
      break;
  }
  //------------------------------------------------------------------------------------------
  //  Serial.println("Counter: " + String(counter));
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    //---------------------- Heartbeat 1: When should be flashing -------------------------
    if ((counter % MODULUS == 1 || counter % MODULUS == 3 || counter % MODULUS == 6 || counter % MODULUS == 8))
    {
      if (counter % MODULUS  > 5 )
      {
        if (headLights == FLASHING)
          botLight.headlightsRightOn();
        if (neoLights != ALL_ON && neoLights != ALL_OFF)
          botLight.batchesOfEight(botLight.getActiveColor1(), botLight.getActiveColor2());
      }
      else
      {
        if (headLights == FLASHING)
          botLight.headlightsLeftOn();
        if (neoLights != ALL_ON && neoLights != ALL_OFF && neoLights != WHITE)
          botLight.batchesOfEight(botLight.getActiveColor2(), botLight.getActiveColor1());
      }
        botLight.showNeo();
    }
    else
    {
      botLight.headlightsOff();
      if (neoLights != ALL_ON && neoLights != ALL_OFF && neoLights != WHITE)
        botLight.ledRange(0, 63, 0);
    }
    //---------------------- Manipulate Headlight States  ------------------------------------------------
    if (headLights == BOTH_ON)
    {
      botLight.headlightsOn();
    }
    else if (headLights == BOTH_OFF)
    {
      botLight.headlightsOff();
    }

    counter++;
    if (counter == 10)
    {
      counter = 0;
    }
  }
  botLight.showNeo();
}
