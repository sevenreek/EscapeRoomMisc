#ifndef ARDUINO_H
#define ARDUINO_H
#include "Arduino.h"
#endif
#include <NeoSWSerial.h>
#include "DFPlayerMini_Fast.h"

class SoundController {
  private:
  DFPlayerMini_Fast myDFPlayer;
  public:
  SoundController(NeoSWSerial softSerial);
  SoundController(int RXpin, int TXpin, unsigned int baudrate);
  void play(uint16_t number);
  void playLoop(uint16_t number);
  void volume(uint8_t number);
};
