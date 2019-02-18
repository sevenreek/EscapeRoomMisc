#include "Potentiometer.h"
float KnobController::getCurrentPosition()
{
    int currentPosition;
    currentPosition = analogRead(analogPin);
    return (float)(currentPosition-minAnalog)/(maxAnalogRelative);
}
int KnobController::getCurrentRawPosition()
{
    return analogRead(analogPin);
}
KnobController::KnobController(int analogPin, int minAnalog, int maxAnalog)
{
  this->minAnalog = minAnalog;
  this->maxAnalogRelative = maxAnalog-minAnalog;
  this->analogPin = analogPin;
} 
