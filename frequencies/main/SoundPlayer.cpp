#include "SoundPlayer.h"
SoundController::SoundController(NeoSWSerial softSerial)
{
this->myDFPlayer.begin(softSerial);
  
  //myDFPlayer.setTimeOut(500);
}
SoundController::SoundController(int RXpin, int TXpin, unsigned int baudrate)
{
  NeoSWSerial softSerial(RXpin,TXpin);
  softSerial.begin(baudrate);
  this->myDFPlayer.begin(softSerial);
  //this->myDFPlayer.setTimeOut(500);
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
