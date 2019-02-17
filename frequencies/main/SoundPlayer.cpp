#include "SoundPlayer.h"
SoundController::SoundController(SoftwareSerial softSerial)
{
  if(!(this->myDFPlayer.begin(softSerial)))
  {
    Serial.println("Failed to initialze MP3 module!");
  }
  myDFPlayer.setTimeOut(500);
}
SoundController::SoundController(int RXpin, int TXpin, unsigned int baudrate)
{
  SoftwareSerial softSerial(RXpin,TXpin);
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
  this->myDFPlayer.loop(number);
}
void SoundController::volume(uint8_t number)
{
  this->myDFPlayer.volume(number);
}
