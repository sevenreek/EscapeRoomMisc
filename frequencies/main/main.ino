/**
 * This version is created with connections on the Arduino Pro Mini 3.3V 
 * 
 */
#include "Potentiometer.h"
#include "SoundPlayer.h"

#include "DS2401.h"
//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <math.h>
#include <string.h>
/* BASIC CONFIGURATION */
#define KNOB_COUNT (4)
#define CORRECT_FREQUENCY_COUNT (3)
/* MP3 FILES DEFINITIONS */
#define RADIO_NOISE (4) 
#define RADIO_MORSE (3)
#define RADIO_CHATTER0 (2)
#define RADIO_CHATTER1 (1)
/* PIN DEFINITIONS */
#define ARDUINO_TX_PIN (11) // Arduino pin used as the software transimtter. Connect to the mp3 module RX pin.
#define ARDUINO_RX_PIN (10) // Arduino pin used as the software reciever. Connect to the mp3 module TX pin. Not actually necessary, as no data is read from the mp3 module atm.
#define ARDUINO_VOLUME_PIN (A6) // used for volume control
#define FLOATING_ANALOG_PIN (A7) // used for random seed
#define ARDUINO_SDA_PIN (0x70) // A4 - SDA; A5 - SCL
#define ARDUINO_DS2401_PIN (7)
/* SOFTWARE SERIAL DEFINITIONS */
#define SSERIAL_BAUDRATE 9600
/* CONSTANTS */
const uint16_t knobPins[KNOB_COUNT] = {A0,A1,A2,A3}; // analog pins on the arduino that are connected to the middle terminal of the potentiometers. Connect the other two ends to VCC and GND
const uint16_t knobMaxes[KNOB_COUNT] = {1023,1023,1023,1023}; // Maximum value read by the analog pin from the extreme setting of a potentiometer
const uint16_t knobMins[KNOB_COUNT] = {0,0,0,0}; // Minimum value read by the analog pin from the other extreme setting of a potentiometer
const uint16_t MAX_FREQUENCY = 9; // Maximum frequency generated by a single knob. Changing this to a value greater than 9 will likely require changes in the comparison condtions and display
const uint16_t MIN_FREQUENCY = 0; // minimum frequency -||-
const byte CORRECT_DS2401[8] = {0x01,0x0B,0xB7,0xFE,0x1A,0x00,0x00,0x9F}; 
/* GLOBALS */
uint16_t correctFrequencies[CORRECT_FREQUENCY_COUNT] = {7982,3456,1024}; // Frequency index (first element's index is 0) has to correspond to the MP3 file definition of index+1(second frequency will play the sound defined as (2))
uint8_t currentPlayedSound = RADIO_NOISE;
uint16_t currentFrequency;
int lastReadVolume;
bool connectedBattery = false;

KnobController * knobReaders[KNOB_COUNT];
KnobController * volumeReader = NULL;
Adafruit_7segment matrix = Adafruit_7segment();
SoundController * soundPlayer = NULL;
DS2401 * ds2401 = NULL;
/* FUNCTION PROTOTYPES */
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF);

void setup() {
  Serial.begin(115200);
  
  soundPlayer = new SoundController(ARDUINO_RX_PIN, ARDUINO_TX_PIN, SSERIAL_BAUDRATE);
  ds2401 = new DS2401(ARDUINO_DS2401_PIN);
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    knobReaders[i] = new KnobController(knobPins[i],knobMins[i],knobMaxes[i]);
  }
  volumeReader = new KnobController(ARDUINO_VOLUME_PIN,0,1024);
  matrix.begin(ARDUINO_SDA_PIN);
  Serial.println("Starting board...");
   soundPlayer->playLoop(RADIO_NOISE); // starts with radio noise playing
}

void loop() {
  while(!connectedBattery)
  {
    byte data[8];
    uint8_t result = ds2401->readData(data);
    uint8_t comparisonResult = 1;
    if(result == RESULT_SUCCESS)
    {
      Serial.println("Found device. Checking serial...");
      comparisonResult = memcmp(data,CORRECT_DS2401,sizeof(CORRECT_DS2401));
    }
    else
    {
      Serial.println("Device not found.");
      return;
    }
    if(comparisonResult == 0)
    {
      Serial.println("Success. Device matching. Playing sound...");
      delay(200);
      connectedBattery = true;
      soundPlayer->playLoop(RADIO_NOISE); // starts with radio noise playing
      
    }
    else
      Serial.println("Serial incorrect.");
  }
  Serial.println("Continuing");
  delay(1000);
  int readVolume = volumeReader->getCurrentPosition()*30; 
  if(readVolume!=lastReadVolume)
  {
    soundPlayer->volume( readVolume);
    lastReadVolume = readVolume;
  }
  int i;
  currentFrequency = 0;
  for(i=0;i<KNOB_COUNT;i++)
  {
    uint8_t readDigit = convertPercentToFrequency( knobReaders[i]->getCurrentPosition(), MIN_FREQUENCY, MAX_FREQUENCY  );
    uint16_t valToAdd = readDigit;
    int j;
    for(j=KNOB_COUNT-i-1;j>0;j--)
      valToAdd*=10;
    matrix.writeDigitNum( i>1 ? i+1:i, readDigit); // position 2 in writeDigitNum is not the next digit but controls the dots
    currentFrequency += valToAdd;
    //Serial.print("RD: "); Serial.print(readDigit);  Serial.print(" FRQ: "); Serial.println(currentFrequency);
  }
  matrix.writeDisplay();
  bool isCorrectFrequency = false;
  for(i=0;i<CORRECT_FREQUENCY_COUNT;i++)
  {
    //Serial.print("Comparing "); Serial.print(correctFrequencies[i]);  Serial.print(" with "); Serial.println(currentFrequency);
    if(correctFrequencies[i]==currentFrequency)
    {
      isCorrectFrequency = true;
      if((i+1)!=currentPlayedSound)
      {
        soundPlayer->playLoop(i+1);
        currentPlayedSound = i+1;
      }
      //Serial.print("Matching frequency number ");Serial.println(i);
    }
  }
  if(!isCorrectFrequency && currentPlayedSound!=RADIO_NOISE)
  {
    soundPlayer->playLoop(RADIO_NOISE);
    currentPlayedSound = RADIO_NOISE;
  }

}
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF)
{
  return roundf(percentage*(maxF-minF)+minF);
}
