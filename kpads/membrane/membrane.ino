#include <Key.h>
#include <Keypad.h>
#define CODE_LENGTH 4
#define CLEAR_SIGN '\0'
#define CORRECT_CODE "1234"
#define LED_GREEN 9
#define LED_RED 10
#define BLINK_COUNT 4
#define BLINK_DELAY 100
#define ONSOLVE_PIN 11 // pin to change state when correct card is detected.
#define ONSOLVE_STATE LOW // state to set on ONSOLVE_PIN when correct card is deteced
#define DEBUG false
void clearCode(char * code);
bool isCodeCorrect(char* code);
void onPuzzleIncorrect();
void onPuzzleFinished();
const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'#','0','*'}
};
byte rowPins[rows] = {2,3,4,5}; //connect to the row pinouts of the keypad
byte colPins[cols] = {6,7,8}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

char * code;
unsigned char currentDigit = 0;
void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  code = (char*)malloc(CODE_LENGTH*sizeof(char));
  clearCode(code);
  #if DEBUG == true
  Serial.begin(9600);
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_GREEN,LOW);
  pinMode(ONSOLVE_PIN, OUTPUT);
  #endif
}

void loop() {
  char next = keypad.waitForKey();
  #if DEBUG == true
  Serial.print(next);
  #endif
  code[currentDigit++] = next;
  if(currentDigit>=CODE_LENGTH)
  {
    if(isCodeCorrect(code))
      onPuzzleFinished();
    else
      onPuzzleIncorrect();
    clearCode(code);
    currentDigit = 0;
  }
}
void clearCode(char * code)
{
  memset(code, CLEAR_SIGN, CODE_LENGTH);
}
bool isCodeCorrect(char * code)
{
  if(memcmp(code, CORRECT_CODE, CODE_LENGTH) == 0)
    return true;
  else 
    return false;
}
void onPuzzleIncorrect()
{
  for(int i = 0; i<BLINK_COUNT; i++)
  {
    digitalWrite(LED_RED,LOW);
    delay(BLINK_DELAY/2);
    digitalWrite(LED_RED,HIGH);
    delay(BLINK_DELAY/2);
  }
}

void onPuzzleFinished()
{
  #if DEBUG == true
  Serial.println("VICTORY!");
  #endif
  digitalWrite(LED_GREEN,HIGH);
  digitalWrite(LED_RED,LOW);
  digitalWrite(ONSOLVE_PIN, ONSOLVE_STATE);
}
