/*
----------------------------------
-------------Includes-------------
----------------------------------
*/
#include <EECS473BLECombo.h>

/*
----------------------------------
--------------Macros--------------
----------------------------------
*/
// Printing with stream operator helper functions
template <class T> inline Print &operator<<(Print &obj, T arg) { obj.print(arg); return obj; }
template <> inline Print &operator<<(Print &obj, float arg) { obj.print(arg, 4); return obj; }

/*
----------------------------------
---ISR Test Function Prototypes---
----------------------------------
*/
void modeISR();
void clickRightISR();
void clickLeftISR();

/*
----------------------------------
-------------Structs--------------
----------------------------------
*/
enum class PCInputMode{mouse, keyboard};

/*
----------------------------------
-------------Globals--------------
----------------------------------
*/
const int MOUSE_DELAY = 100; // delay between subsequent mouse position reads in ms
const int KEYBOARD_DELAY = 1000; // delay between subsequent keys in ms; to be reduced later

// GPIO pins for LED and button for ESP32-WROOM-32D
const int ledGreenPin = 2;
const int ledBluePin = 4;
const int buttonModePin = 5;
const int buttonClickRightPin = 23; 
const int buttonClickLeftPin = 19;
// Globals that will control button interrupts and corresponding for-loop functionality
PCInputMode inputMode = PCInputMode::mouse;
volatile bool clickRightTrigger = false; 
volatile bool clickLeftTrigger = false; 
volatile bool inputModeTrigger = false;
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
  {
    Serial.println("Cant connect to MPU");
  }

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
  if (inputModeTrigger) {
    // Toggle input mode; for now, only 2 modes
    inputMode = (inputMode == PCInputMode::keyboard) ? PCInputMode::mouse : PCInputMode::keyboard;
    inputModeTrigger = false;
  }
  
  // Determine if keyboard/mouse combination is connected
  if(!Test.comboKeyboard.isConnected())
  {
    Serial.println("BLE Keyboard/Mouse not connected. Waiting 2 seconds...");
    delay(2000);
    return;
  }

  if (inputMode == PCInputMode::keyboard) 
  {
    // Light up green to indicate keyboard mode
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledBluePin, LOW);

    // BLE Keyboard Testing onto Serial Monitor and BLE Keyboard
    Serial.println("Sending 'Hello world'");
    Test.comboKeyboard.println("Hello World");
    Serial.println("Sending Enter key...");
    Test.comboKeyboard.write(KEY_RETURN);
    delay(KEYBOARD_DELAY);
  } 
  else if (inputMode == PCInputMode::mouse)
  {
    // Light up blue to indicate mouse mode
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledBluePin, HIGH);

    Test.mouseMove();
    if(clickRightTrigger)
    {
      Test.comboMouse.click(MOUSE_RIGHT);
      clickRightTrigger = false;
    }
    // NOTE: May click both at once
    if(clickLeftTrigger)
    {
      Test.comboMouse.click(MOUSE_LEFT);
      clickLeftTrigger = false;
    }
    delay(MOUSE_DELAY);
  }
  
}
/*
----------------------------------
-----ISR Test Function Initializations-----
----------------------------------
*/
void modeISR()
{
  // NOTE: Despite its simplicity, operation in main loop in order to negate bouncing
  inputModeTrigger = true;
}
void clickRightISR()
{
  clickRightTrigger = true;
}
void clickLeftISR()
{
  clickLeftTrigger = true; 
}