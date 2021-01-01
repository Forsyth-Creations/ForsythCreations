/*
  Example sketch for the Wii Balance Board Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <Wii.h>
#include <usbhub.h>

//#define DEBUG //used for debugging

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "MotorControl.h"

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
//WII Wii(&Btd, PAIR); // This will start an inquiry and then pair with your Wii Balance Board - you only have to do this once
WII Wii(&Btd); // After that you can simply create the instance like so and then press the power button on the Wii Balance Board

int centerXUsingRight = 17;
int centerYUsingTop = 30;
double centerWeight = 0;
double centerThresholdRadius = 1.5;
double speedOfCart = 0;
double servoPos = 90;

void setup()
{
  setup1();
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial)
    ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ; //halt
  }
  Serial.print(F("\r\nWii Balance Board Bluetooth Library Started"));
}
void loop()
{
  Usb.Task();
  if (Wii.wiiBalanceBoardConnected)
  {
    centerWeight = Wii.getTotalWeight() / 4;
    //printCoordinates();
    //checkGitters();
    if (Wii.getButtonClick(A))
    {
      Wii.disconnect();
    }

    if (getAverageTop() > centerThresholdRadius + centerWeight || getAverageTop() < centerWeight - centerThresholdRadius)
    {
      activateHeadlights(true);
      speedOfCart = constrain(mapf(getAverageTop(), 0, centerWeight * 2, -450, 450), -255, 255);
      setSpeedTo(speedOfCart);
    }
    else
    {
      speedOfCart = 0;
      setSpeedTo(speedOfCart);
    }
    if (getAverageRight() > centerThresholdRadius + centerWeight || getAverageRight() < centerWeight - centerThresholdRadius)
    {
      servoPos = constrain(mapf(getAverageRight(), 0, centerWeight * 2, 0, 180), 40, 140);
      frontWheel.write(servoPos);
    }
    else
    {
      servoPos = 90;
      frontWheel.write(servoPos);
    }
    if (getAverageTop() <= centerWeight && getAverageBottom() <= centerWeight) //not standing on the board
    {
      stopBot();
    }
  }
  else
  {
    stopBot();
  }
  checkControlVals();
  //checkGitters();
}

double getAverageRight()
{
  return (Wii.getWeight((BalanceBoardEnum)1) + Wii.getWeight((BalanceBoardEnum)0)) / 2;
}
double getAverageLeft()
{
  return ((Wii.getWeight((BalanceBoardEnum)2) + Wii.getWeight((BalanceBoardEnum)3)) / 2);
}
double getAverageTop()
{
  return (Wii.getWeight((BalanceBoardEnum)2) + Wii.getWeight((BalanceBoardEnum)0)) / 2;
}
double getAverageBottom()
{
  return ((Wii.getWeight((BalanceBoardEnum)1) + Wii.getWeight((BalanceBoardEnum)3)) / 2);
}
void checkGitters()
{
  Serial.print("Top: ");
  Serial.print(getAverageTop());
  Serial.print(" Bottom: ");
  Serial.print(getAverageBottom());
  Serial.print(" Left: ");
  Serial.print(getAverageLeft());
  Serial.print(" Right: ");
  Serial.print(getAverageRight());
  Serial.print(" Center Weight: ");
  Serial.print(centerWeight);
  Serial.println();
}

void checkControlVals()
{
  Serial.print("Speed: ");
  Serial.print(speedOfCart);
  Serial.print(" Servo: ");
  Serial.print(servoPos);
  Serial.println(" ");
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void stopBot()
{
  activateHeadlights(false);
  speedOfCart = 0;
  setSpeedTo(speedOfCart);
  servoPos = 90;
  frontWheel.write(servoPos);
}
