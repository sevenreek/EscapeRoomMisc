#include "PinChangeInterrupt.h"
#include "NeoPixelStripController.cpp"
#define PIN_RGB2  7 // PIN TASMY 1
#define PIN_RGB1  6 // PIN TASMY 1
#define PIN_RGB0  5 // PIN TASMY 1
#define INTERRUPT_PIN_RGB0 2 // Pin z przerwaniem dla przycisk u tasmny 1
#define INTERRUPT_PIN_RGB1 3 // Pin z przerwaniem dla przycisku tasmny 1
#define INTERRUPT_PIN_RGB2 4 // Pin z przerwaniem dla przycisku tasmny 1
#define NEOPIXEL_COUNT 12
#define LEDSTRIP_COUNT 3
#define UPDATE_DELAY_MS 500


NeoPixelStripController * stripControllers[3];

int SOLUTION[LEDSTRIP_COUNT][2] = {{0,3},{4,9},{9,11}};

void STRIP_IRQ_HANDLER_0();
void STRIP_IRQ_HANDLER_1();
void STRIP_IRQ_HANDLER_2();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(INTERRUPT_PIN_RGB0, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN_RGB1, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN_RGB2, INPUT_PULLUP);
  stripControllers[0] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB0, NEO_GRB + NEO_KHZ800),0xffff00,12); // Tasma 1
  stripControllers[1] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB1, NEO_GRB + NEO_KHZ800),0xffff00,12); // Tasma 1
  stripControllers[2] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB2, NEO_GRB + NEO_KHZ800),0xffff00,12); // Tasma 1
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(INTERRUPT_PIN_RGB0), STRIP_IRQ_HANDLER_0, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(INTERRUPT_PIN_RGB1), STRIP_IRQ_HANDLER_1, FALLING);
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(INTERRUPT_PIN_RGB2), STRIP_IRQ_HANDLER_2, FALLING);
  Serial.begin(9600);
  int i;
  for(i = 0; i<LEDSTRIP_COUNT; i++)
  {
    NeoPixelStripController * strip = stripControllers[i];
    strip->init();
  }
  Serial.println(RUN_LEFT); Serial.println(RUN_RIGHT); Serial.println(STOPPED);
}


void loop()
{
  Serial.print(stripControllers[0]->getState());Serial.print(stripControllers[1]->getState());Serial.print(stripControllers[2]->getState());Serial.print("\n");
  if((stripControllers[0]->getState() == STOPPED) && (stripControllers[1]->getState() == STOPPED) && (stripControllers[2]->getState() == STOPPED ))
    if(isPuzzleCorrect())
      onSolutionCorrect();
  delay(UPDATE_DELAY_MS);
  int i;
  for(i = 0; i<LEDSTRIP_COUNT; i++)
  {
    NeoPixelStripController* strip = stripControllers[i];
    strip->updateStrip();
    //Serial.print("Should update "); Serial.print(i); Serial.print("\n");
  }
}

bool isPuzzleCorrect()
{
  Serial.println("Checking correctness...");
  bool isCorrect = true;
  int i;
  for(i = 0; i<LEDSTRIP_COUNT; i++)
  {
    NeoPixelStripController * strip = stripControllers[i];
    if(!(strip -> isMatchingSolution(SOLUTION[i][0], SOLUTION[i][1])) )
    {  
      Serial.print(i); Serial.print(" is incorrect."); 
      isCorrect = false;
      break;
    }
  }
  return isCorrect;
}
void onSolutionCorrect()
{
  Serial.print("VICTORY!\n");
  Serial.print("VICTORY!\n");
  Serial.print("VICTORY!\n");
  Serial.print("VICTORY!\n");
  Serial.print("VICTORY!\n");
}
void STRIP_IRQ_HANDLER_0()
{
  stripControllers[0]->handleIRQ();
}
void STRIP_IRQ_HANDLER_1()
{
  stripControllers[1]->handleIRQ();
}
void STRIP_IRQ_HANDLER_2()
{
  stripControllers[2]->handleIRQ();
}

 
