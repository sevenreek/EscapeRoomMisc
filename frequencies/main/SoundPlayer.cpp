#include "SoundPlayer.h"
SoundController::SoundController(NeoSWSerial softSerial)
{
  this->myDFPlayer->begin(softSerial);
  
  //myDFPlayer.setTimeOut(500);
}
SoundController::SoundController(int RXpin, int TXpin, unsigned int baudrate)
{
  softSerial = new NeoSWSerial(RXpin,TXpin);
  softSerial->begin(baudrate);
  this->myDFPlayer = new DFPlayerMini_Fast();
  this->myDFPlayer->begin(*softSerial);
  //this->myDFPlayer.setTimeOut(500);
}
void SoundController::play(uint16_t number)
{
  this->myDFPlayer->play(number);
}
void SoundController::playLoop(uint16_t number)
{
  this->myDFPlayer->loop(number);
}
void SoundController::volume(uint8_t number)
{
  this->myDFPlayer->volume(number);
}
void SoundController::pause()
{
  this->myDFPlayer->pause();
}
void SoundController::resume()
{
  this->myDFPlayer->resume();
}
