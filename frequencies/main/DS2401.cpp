#include "DS2401.h"
/*class DS2401 {
  private:
  OneWire * onewire;
  public:
  DS2401(int dataPin);
  bool readData(byte* data);
};
*/
DS2401::DS2401(int dataPin)
{
  this->onewire = new OneWire(dataPin);
}
uint8_t DS2401::readData(volatile byte* data)
{
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

bool DS2401::isCorrectDS2401Connected(const byte* correctSignature)
{
  byte data[8];
  if(this->readData(data) == RESULT_SUCCESS)
  {
    if(memcmp(data,correctSignature,8) == 0)
      return true;
    else
      return false; 
  }
  else
    return false; // if DS2401 is not found return false.
  
}
