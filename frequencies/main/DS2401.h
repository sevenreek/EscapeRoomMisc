#ifndef ARDUINO_H
#define ARDUINO_H
#include "Arduino.h"
#endif
#define READ_COMMAND (0x33)
#define RESULT_FAILURE -1
#define RESULT_SUCCESS 0
#define RESULT_FAIL_CRC -2
#define VERIFY_CRC true
#include <OneWire.h>
class DS2401 {
  private:
  OneWire * onewire;
  public:
  DS2401(int dataPin);
  char readData( byte* data);
};
