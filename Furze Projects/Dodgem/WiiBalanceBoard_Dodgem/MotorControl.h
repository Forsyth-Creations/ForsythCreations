
#include <Servo.h>
Servo frontWheel;

#define speedPin 3
#define motorA1 4
#define motorA2 5
#define motorB1 6
#define motorB2 7
#define headlights A0

#define forward 0
#define backward 1
#define stop 2

boolean headlightsActive = false;

void setup1() {
  frontWheel.attach(2);
  pinMode(speedPin, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  analogWrite(speedPin, 0);
}

void setSpeedTo(double val)
{
  analogWrite(speedPin, abs(val));

  //Serial.println(val);

  if (val > 0) //One Direction (yuck)
  {
    digitalWrite(motorA1, HIGH);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, HIGH);
    digitalWrite(motorB2, LOW);
  }
  else if (val < 0) //another direciton
  {
    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, HIGH);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, HIGH);
  }
  else
  {
    //stop robot
    analogWrite(speedPin, 0);
  }
}



void activateHeadlights(boolean state)
{
  headlightsActive = state;
  //Serial.print("Headlight state is: ");
  //Serial.println(headlightsActive);

  if (headlightsActive)
  {
    analogWrite(headlights, 255);
  }
  else
  {
    digitalWrite(headlights, LOW);
  }
}

void toggleHeadLights()
{
  Serial.println("Toggling Lights");
  headlightsActive = !headlightsActive;
  activateHeadlights(headlightsActive);
}
