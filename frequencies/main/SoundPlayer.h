#ifndef ARDUINO_H
#define ARDUINO_H
#include "Arduino.h"
#endif
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
class SoundController {
  private:
  DFRobotDFPlayerMini myDFPlayer;
  public:
  SoundController(SoftwareSerial softSerial);
  SoundController(int RXpin, int TXpin, int baudrate);
};
