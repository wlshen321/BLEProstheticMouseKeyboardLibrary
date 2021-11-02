/*
----------------------------------
-----Includes-----
----------------------------------
*/
#include <EECS473BLECombo.h>
/*
----------------------------------
-----ISR Test Function Prototypes-----
----------------------------------
*/
void modeISR();
void clickRightISR();
void clickLeftISR();

/*
----------------------------------
-------------Globals------------
----------------------------------
*/
// Globals for LED and button pins
const int buttonModePin = 5;  
const int buttonClickRightPin = 23; 
const int buttonClickLeftPin = 19;
const int ledGreenPin =  2;  
const int ledBluePin = 4;
// Globals that will control button interrupts and corresponding for-loop functionality
byte modeState = LOW;
volatile byte modeTrigger = LOW;
volatile byte clickRightTrigger = LOW; 
volatile byte clickLeftTrigger = LOW; 
// Class instantiation
BLEClass Test;
/*
----------------------------------
-------------Main Code------------
----------------------------------
*/
void setup() 
{

  // Set up serial initialization
  Serial.begin(115200);
  Serial.println("Starting work!"); // Debug Code
  byte status = Test.init();
  while(status != 0)
    Serial.println("Cant connect to MPU");
  // initialize the LED pins as an output:
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  // initialize the pushbutton pins as an interrupt input:
  pinMode(buttonModePin, INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonModePin), modeISR, HIGH);
  pinMode(buttonClickRightPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonClickRightPin), clickRightISR, HIGH);
  pinMode(buttonClickLeftPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonClickLeftPin), clickLeftISR, HIGH);
  Serial.println("Done!\n");
}

void loop() 
{
  if(modeTrigger)
  {
    modeState = !modeState;
    modeTrigger = LOW;
  }
  if(Test.comboKeyboard.isConnected()) 
  {
    if (modeState == HIGH) 
    {
      digitalWrite(ledGreenPin, HIGH);
      digitalWrite(ledBluePin, LOW);
      // BLE Keyboard Testing onto Serial Monitor and BLE Keyboard
      Serial.println("Sending 'Hello world'");
      Test.comboKeyboard.println("Hello World");
      Serial.println("Sending Enter key...");
      Test.comboKeyboard.write(KEY_RETURN);
      delay(1000);
    } 
    else 
    {
       digitalWrite(ledGreenPin, LOW);
       digitalWrite(ledBluePin, HIGH);
      Test.mouseMove();
      if(clickRightTrigger == HIGH)
      {
        Test.comboMouse.click(MOUSE_RIGHT);
        clickRightTrigger = LOW;
      }
      else if(clickLeftTrigger == HIGH)
      {
        Test.comboMouse.click(MOUSE_LEFT);
        clickLeftTrigger = LOW;
      }
      delay(100);
    }
  }
  else
  {
    Serial.println("BLE Keyboard/Mouse not connected. Waiting 2 seconds...");
    delay(2000);
  }
}
/*
----------------------------------
-----ISR Test Function Initializations-----
----------------------------------
*/
void modeISR()
{
  modeTrigger = HIGH;
}
void clickRightISR()
{
  clickRightTrigger = HIGH;
}
void clickLeftISR()
{
  clickLeftTrigger = HIGH; 
}