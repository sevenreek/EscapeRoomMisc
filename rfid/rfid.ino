
#include "SPI.h" 
#include "MFRC522.h" 
#define DUMPCARDID false // set to true to print found card id in serial monitor, set to false for normal operation
#define ONSOLVE_PIN 13 // pin to change state when correct card is detected.
#define ONSOLVE_STATE LOW // state to set on ONSOLVE_PIN when correct card is deteced
#define LED_RED 8 // will blink on incorrect card
#define LED_GREEN 7 // will turn on on correct card
#define LED_COMMON_GND true // set to true if both LEDs are connected to common ground. In such case HIGH will be put on respective pins.
#define CORRECT_CARDS_COUNT  1 // change when adding new cards
void onCorrectCard();
const uint32_t correctCardIDs[] = {0x89DE04B7}; // input in hexadecimal
const int pinRST = 9;
const int pinSDA = 10;
MFRC522 mfrc522(pinSDA, pinRST); 
void setup() {
  
  SPI.begin(); 
  mfrc522.PCD_Init(); 
  Serial.begin(9600); 
  pinMode(ONSOLVE_PIN,OUTPUT);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  digitalWrite(LED_RED, LED_COMMON_GND?LOW:HIGH);
  digitalWrite(LED_GREEN, LED_COMMON_GND?LOW:HIGH);
  digitalWrite(ONSOLVE_PIN, !ONSOLVE_STATE);
}
void loop() {
  if (mfrc522.PICC_IsNewCardPresent()) 
  { 
    if(mfrc522.PICC_ReadCardSerial())
    { 
      #if DUMPCARDID == true
      Serial.print("RFID TAG ID:");
      Serial.print(*((uint32_t*)mfrc522.uid.uidByte), HEX); // print id as hex values
      Serial.println(); // Print out of id is complete.
      #else
      uint32_t readCardID = *((uint32_t*)mfrc522.uid.uidByte);
      for(int i = 0; i<CORRECT_CARDS_COUNT; i++)
      {
        Serial.print(readCardID);Serial.print("==");Serial.print(correctCardIDs[i]);Serial.println();
        if(correctCardIDs[i] == readCardID)
          onCorrectCard();
        else
          onIncorrectCard();
      }
      #endif
    }
  }
}
void onCorrectCard()
{
  Serial.println("Correct!");
  digitalWrite(LED_GREEN, LED_COMMON_GND?HIGH:LOW);
  digitalWrite(ONSOLVE_PIN, ONSOLVE_STATE);
}
void onIncorrectCard()
{
  digitalWrite(LED_RED, LED_COMMON_GND?HIGH:LOW);  
  delay(100);
  digitalWrite(LED_RED, LED_COMMON_GND?!HIGH:!LOW);  
  delay(100);
  digitalWrite(LED_RED, LED_COMMON_GND?HIGH:LOW);  
  delay(100);
  digitalWrite(LED_RED, LED_COMMON_GND?!HIGH:!LOW);  
}
