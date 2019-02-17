#include "Potentiometer.h"
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <math.h>
#include <string.h>
#define POTENTIOMETER_TEST_MODE false
#define FRQ_TEST_MODE false
#define KNOB_COUNT (4)
#define CORRECT_FREQUENCY_COUNT (3)
#define FLOATING_ANALOG_PIN A4 // used for random seed
const uint16_t knobPins[KNOB_COUNT] = {A0,A1,A2,A3};
const uint16_t knobMaxes[KNOB_COUNT] = {1023,1023,1023,1023};
const uint16_t knobMins[KNOB_COUNT] = {0,0,0,0};
const uint16_t MAX_FREQUENCY = 9;
const uint16_t MIN_FREQUENCY = 0;

uint16_t correctFrequencies[CORRECT_FREQUENCY_COUNT][KNOB_COUNT] = {{0,0,0,0}, {0,0,0,1}, {1,2,3,4}};
uint16_t currentFrequencies[KNOB_COUNT];
KnobController * knobReaders[KNOB_COUNT];
Adafruit_7segment matrix = Adafruit_7segment();
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF);
void setup() {
  Serial.begin(9600);
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    knobReaders[i] = new KnobController(knobPins[i],knobMins[i],knobMaxes[i]);
  }
  matrix.begin(0x70);
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
  #else
  int i;
  for(i=0;i<KNOB_COUNT;i++)
  {
    currentFrequencies[i] = convertPercentToFrequency( knobReaders[i]->getCurrentPosition(), MIN_FREQUENCY, MAX_FREQUENCY  );
  }
  matrix.writeDigitNum(0, currentFrequencies[0]);
  matrix.writeDigitNum(1, currentFrequencies[1]);
  matrix.writeDigitNum(3, currentFrequencies[2]);
  matrix.writeDigitNum(4, currentFrequencies[3]);
  matrix.writeDisplay();
  for(i=0;i<CORRECT_FREQUENCY_COUNT;i++)
    if(memcmp(correctFrequencies[i],currentFrequencies,4*sizeof(uint16_t))==0)
    {
      Serial.print("Matching frequency number ");Serial.println(i);
    }
  #endif

}
uint16_t convertPercentToFrequency(float percentage, uint16_t minF, uint16_t maxF)
{
  return roundf(percentage*(maxF-minF)+minF);
}
