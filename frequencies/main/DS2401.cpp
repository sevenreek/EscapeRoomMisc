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
char DS2401::readData(byte * data)
{
  if(onewire->reset())
  {
    onewire->write(READ_COMMAND, 1);
    for(byte i = 0; i<8; i++)
    {
      data[i] = onewire->read();
      Serial.print(data[i],HEX);
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
