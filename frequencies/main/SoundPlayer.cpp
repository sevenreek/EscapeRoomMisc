#include "SoundPlayer.h"
SoundController::SoundController(NeoSWSerial softSerial)
{
  if(!(this->myDFPlayer.begin(softSerial)))
  {
    Serial.println("Failed to initialze MP3 module!");
  }
  myDFPlayer.setTimeOut(500);
}
SoundController::SoundController(int RXpin, int TXpin, unsigned int baudrate)
{
  NeoSWSerial softSerial(RXpin,TXpin);
  softSerial.begin(baudrate);
  if(!(this->myDFPlayer.begin(softSerial)))
  {
    Serial.println("Failed to initialze MP3 module!");
  }
  this->myDFPlayer.setTimeOut(500);
}
void SoundController::play(uint16_t number)
{
  this->myDFPlayer.play(number);
}
void SoundController::playLoop(uint16_t number)
{
  Serial.println("playLoop");delay(100);
  this->myDFPlayer.loop(number);delay(100);Serial.println("playLoop return");delay(100);
}
void SoundController::volume(uint8_t number)
{
  this->myDFPlayer.volume(number);
}
