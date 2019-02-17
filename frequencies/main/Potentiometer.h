#ifndef ARDUINO_H
#define ARDUINO_H
#include "Arduino.h"
#endif
class KnobController {
  private:
  int analogPin;
  int maxAnalogRelative; // maxAnalog - minAnalog
  int minAnalog;
  public:
  float getCurrentPosition();
  KnobController(int analogPin, int minAnalog, int maxAnalog);
};
