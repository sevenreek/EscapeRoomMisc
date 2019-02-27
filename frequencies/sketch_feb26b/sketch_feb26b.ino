#include "DS2401.h"
#include "DFPlayerMini_Fast.h"
#include <SoftwareSerial.h>
#include <string.h>
/* MP3 FILES DEFINITIONS */
#define RADIO_NOISE (4) 
#define RADIO_MORSE (3)
#define RADIO_CHATTER0 (2)
#define RADIO_CHATTER1 (1)
/* PIN DEFINITIONS */
#define ARDUINO_TX_PIN (11) // Arduino pin used as the software transimtter. Connect to the mp3 module RX pin.
#define ARDUINO_RX_PIN (10) // Arduino pin used as the software reciever. Connect to the mp3 module TX pin.
#define ARDUINO_DS2401_PIN (7)

/* SOFTWARE SERIAL DEFINITIONS */
#define SSERIAL_BAUDRATE 9600
bool isCorrectDS2401Connected(const byte* correctSignature);
const byte CORRECT_DS2401[8] = {0x01,0x0B,0xB7,0xFE,0x1A,0x00,0x00,0x9F}; 
DFPlayerMini_Fast soundPlayer;
DS2401 * ds2401 = NULL;
SoftwareSerial softSerial(ARDUINO_RX_PIN,ARDUINO_TX_PIN);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Rebooting...");
  
  softSerial.begin(SSERIAL_BAUDRATE);
  soundPlayer.begin(softSerial);
  ds2401 = new DS2401(ARDUINO_DS2401_PIN);
  soundPlayer.loop(RADIO_NOISE);
  soundPlayer.pause();
}

void loop() {
  if(isCorrectDS2401Connected(CORRECT_DS2401))
  {
    Serial.println("OK");
    soundPlayer.resume(); 
  }
  else
  {
    Serial.println("Wrong");
    soundPlayer.pause();
  }

}
bool isCorrectDS2401Connected(const  byte* correctSignature)
{
  byte data[8];
  uint8_t result = ds2401->readData(data);
  uint8_t comparisonResult = 1;
  if(result == RESULT_SUCCESS)
  {
    comparisonResult = memcmp(data,correctSignature,8*sizeof(byte));
    if(comparisonResult == 0)
      return true;
    else
      return false; 
  }
  else
    return false; // if DS2401 is not found return false.
}
