#include "Potentiometer.h"
#include "SoundPlayer.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <math.h>
#include <string.h>
/* DEBUG MODE CONFIGURATION */
#define POTENTIOMETER_TEST_MODE false
#define FRQ_TEST_MODE false
/* BASIC CONFIGURATION */
#define KNOB_COUNT (4)
#define CORRECT_FREQUENCY_COUNT (3)
/* MP3 FILES DEFINITIONS */
#define RADIO_NOISE (4) 
#define RADIO_MORSE (3)
#define RADIO_CHATTER0 (2)
#define RADIO_CHATTER1 (1)
/* PIN DEFINITIONS */
#define ARDUINO_TX_PIN (14) // Arduino pin used as the software transimtter. Connect to the mp3 module RX pin.
#define ARDUINO_RX_PIN (15) // Arduino pin used as the software reciever. Connect to the mp3 module TX pin. Not actually necessary, as no data is read from the mp3 module atm.
#define ARDUINO_VOLUME_PIN (A4) // used for volume control
#define FLOATING_ANALOG_PIN (A5) // used for random seed
/* SOFTWARE SERIAL DEFINITIONS */
#define SSERIAL_BAUDRATE 9600
/* CONSTANTS */
const uint16_t knobPins[KNOB_COUNT] = {A0,A1,A2,A3}; // analog pins on the arduino that are connected to the middle terminal of the potentiometers. Connect the other two ends to VCC and GND
const uint16_t knobMaxes[KNOB_COUNT] = {1023,1023,1023,1023}; // Maximum value read by the analog pin from the extreme setting of a potentiometer
const uint16_t knobMins[KNOB_COUNT] = {0,0,0,0}; // Minimum value read by the analog pin from the other extreme setting of a potentiometer
const uint16_t MAX_FREQUENCY = 9; // Maximum frequency generated by a single knob. Changing this to a value greater than 9 will likely require changes in the comparison condtions and display
const uint16_t MIN_FREQUENCY = 0; // minimum frequency -||-
/* GLOBALS */
uint16_t correctFrequencies[CORRECT_FREQUENCY_COUNT] = {7982,3456,1024}; // Frequency index (first element's index is 0) has to correspond to the MP3 file definition of index+1(second frequency will play the sound defined as (2))
uint8_t currentPlayedSound = RADIO_NOISE;
uint16_t currentFrequency;
int lastReadVolume;
KnobController * knobReaders[KNOB_COUNT];
KnobController * volumeReader;
Adafruit_7segment matrix = Adafruit_7segment();
SoundController * soundPlayer = NULL;
/* FUNCTION PROTOTYPES */
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF);

void setup() {
  Serial.begin(9600);
  soundPlayer = new SoundController(ARDUINO_RX_PIN, ARDUINO_TX_PIN, SSERIAL_BAUDRATE);
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    knobReaders[i] = new KnobController(knobPins[i],knobMins[i],knobMaxes[i]);
  }
  volumeReader = new KnobController(ARDUINO_VOLUME_PIN,0,1024);
  matrix.begin(0x70);
  int readVolume = volumeReader->getCurrentPosition()*30; 
  lastReadVolume = readVolume;
  soundPlayer->volume( readVolume);
  soundPlayer->playLoop(RADIO_NOISE); // starts with radio noise playing
}

void loop() {
  #if POTENTIOMETER_TEST_MODE == true
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    Serial.print(analogRead(knobPins[i]));
    Serial.print(" ");
  }
  Serial.print("\n\r");
  #elif FRQ_TEST_MODE == true
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    Serial.print( convertPercentToFrequency( convertPercentToFrequency(knobReaders[i]->getCurrentPosition()), MIN_FREQUENCY, MAX_FREQUENCY ) );
    Serial.print(" ");
  }
  Serial.print("\n\r");
  #else // end of debug configuration code
  int readVolume = volumeReader->getCurrentPosition()*30; 
  //Serial.println(readVolume);
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
  #endif

}
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF)
{
  return roundf(percentage*(maxF-minF)+minF);
}
