#include "DS2401.h"
DS2401::DS2401(int dataPin)
{
  this->onewire = new OneWire(dataPin);
}
uint8_t DS2401::readData(volatile byte * data)
{
  if(!data)
    return RESULT_NOPOINTER;
  if(onewire->reset())
  {
    onewire->write(READ_COMMAND,1);
    for(volatile int i = 0; i<8; i++)
    {
      data[i] = (volatile byte)onewire->read();
    }
    #if VERIFY_CRC == true
    if(data[7]== OneWire::crc8(data,7))
      return RESULT_SUCCESS;
    else
      return RESULT_FAIL_CRC;
    #else
    return RESULT_SUCCESS;
    #endif
  }
  return RESULT_FAILURE;
}
