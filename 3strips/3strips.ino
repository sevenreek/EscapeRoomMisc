#include "PinChangeInterrupt.h"
#include "NeoPixelStripController.cpp"
#define PIN_RELAY 9
#define PIN_RGB_CONTROL 8 /// pin tasmy wyswietlajacej wzor

#define PIN_RGB0_ON 10 // pin wlaczajacy tasme 1
#define PIN_RGB1_ON 11
#define PIN_RGB2_ON 12
#define PIN_RGB2  7 // PIN TASMY 3
#define PIN_RGB1  6 // PIN TASMY 2
#define PIN_RGB0  5 // PIN TASMY 1
#define INTERRUPT_PIN_RGB0 2 // Pin z przerwaniem dla przycisk u tasmny 1
#define INTERRUPT_PIN_RGB1 3 // Pin z przerwaniem dla przycisku tasmny 2
#define INTERRUPT_PIN_RGB2 4 // Pin z przerwaniem dla przycisku tasmny 3
#define NEOPIXEL_COUNT 12
#define LEDSTRIP_COUNT 3
#define UPDATE_DELAY_MS 200


NeoPixelStripController * stripControllers[3];
Adafruit_NeoPixel solutionStrip = Adafruit_NeoPixel(NEOPIXEL_COUNT,PIN_RGB_CONTROL,NEO_GRB+NEO_KHZ800);
const int SOLUTION[LEDSTRIP_COUNT][2] = {{0,3},{4,5},{6,11}}; // rozwiazanie w formacie {poczatek, koniec}
const int32_t COLORS[LEDSTRIP_COUNT] = {0xff0000,0x00ff00,0x0000ff}; // kolory tasm
void STRIP_IRQ_HANDLER_0();
void STRIP_IRQ_HANDLER_1();
void STRIP_IRQ_HANDLER_2();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(INTERRUPT_PIN_RGB0, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN_RGB1, INPUT_PULLUP);
  pinMode(INTERRUPT_PIN_RGB2, INPUT_PULLUP);
  pinMode(PIN_RGB0_ON, INPUT_PULLUP);
  pinMode(PIN_RGB1_ON, INPUT_PULLUP);
  pinMode(PIN_RGB2_ON, INPUT_PULLUP);
  stripControllers[0] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB0, NEO_GRB + NEO_KHZ800),COLORS[0],12, PIN_RGB0_ON); // Tasma 1
  stripControllers[1] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB1, NEO_GRB + NEO_KHZ800),COLORS[1],12, PIN_RGB1_ON); // Tasma 2
  stripControllers[2] = new NeoPixelStripController(new Adafruit_NeoPixel(NEOPIXEL_COUNT, PIN_RGB2, NEO_GRB + NEO_KHZ800),COLORS[2],12, PIN_RGB2_ON); // Tasma 3
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
  solutionStrip.begin();
  for(i = SOLUTION[0][0]; i <= SOLUTION[0][1]; i++)
    solutionStrip.setPixelColor(i,COLORS[0]);
  for(i = SOLUTION[1][0]; i <= SOLUTION[1][1]; i++)
    solutionStrip.setPixelColor(i,COLORS[1]);
  for(i = SOLUTION[2][0]; i <= SOLUTION[2][1]; i++)
    solutionStrip.setPixelColor(i,COLORS[2]);
  solutionStrip.show();
  //Serial.println(RUN_LEFT); Serial.println(RUN_RIGHT); Serial.println(STOPPED);
}


void loop()
{
  //Serial.print(stripControllers[0]->getState());Serial.print(stripControllers[1]->getState());Serial.print(stripControllers[2]->getState());Serial.print("\n");
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
  //Serial.println("Checking correctness...");
  bool isCorrect = true;
  int i;
  for(i = 0; i<LEDSTRIP_COUNT; i++)
  {
    NeoPixelStripController * strip = stripControllers[i];
    if(!(strip -> isMatchingSolution(SOLUTION[i][0], SOLUTION[i][1])) )
    {  
      //Serial.print(i); Serial.print(" is incorrect."); 
      isCorrect = false;
      break;
    }
  }
  return isCorrect;
}
void onSolutionCorrect()
{
  //Serial.print("VICTORY!\n");
  digitalWrite(PIN_RELAY,LOW);
}
void STRIP_IRQ_HANDLER_0()
{
  if(digitalRead(INTERRUPT_PIN_RGB0) != LOW)
    return;
  stripControllers[0]->handleIRQ();
}
void STRIP_IRQ_HANDLER_1()
{
  if(digitalRead(INTERRUPT_PIN_RGB1) != LOW)
    return;
  stripControllers[1]->handleIRQ();
}
void STRIP_IRQ_HANDLER_2()
{
  if(digitalRead(INTERRUPT_PIN_RGB2) != LOW)
    return;
  stripControllers[2]->handleIRQ();
}

 
