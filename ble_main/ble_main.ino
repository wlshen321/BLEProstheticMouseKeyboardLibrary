//Libraries that combine BLE Keyboard and Mouse control
//#include <BleCombo.h> // This is commented out, because I want to create the class instantiations within the .ino file, rather than using the class instantiations within BleCombo.h
#include <BleComboKeyboard.h>
#include <BleComboMouse.h>
#include <BleConnectionStatus.h>
#include <KeyboardOutputCallbacks.h>
//Libraries that control the MSP6050
#include "Wire.h"
#include <MPU6050_light.h>

void modeISR();
void clickISR();

// // // Class instantiations for BLE Mouse/Keyboard control
//  BleComboKeyboard Keyboard;
//  BleComboMouse Mouse(&Keyboard);
// // Class instantiation for MPU6050
// MPU6050 mpu(Wire);
// Globals for LED and button pins
const int buttonModePin = 5;  
const int buttonClickPin = 23; 
const int ledGreenPin =  2;  
const int ledBluePin = 4;
// Globals that will control button interrupts and corresponding for-loop functionality
byte modeState = LOW;
volatile byte modeTrigger = LOW;
volatile byte clickTrigger = LOW; 

/*
 *  API Class Definition 
 */
class BLEClass
{
  public:
    
    byte init();
    void mouseMove();
    void hello();
    int c_accelx;
    int c_accely;
    BleComboKeyboard comboKeyboard;
    MPU6050 comboMPU;
    BleComboMouse comboMouse; // if I put &comboKeyboard in here, it wont compile. How do I fix this?
    BLEClass()
    : comboKeyboard{}, comboMouse(&comboKeyboard), comboMPU(Wire)
    {
//      comboKeyboard = new BleComboKeyboard;
//      comboMPU = new MPU6050(Wire);
//      comboMouse = new BleComboMouse(comboKeyboard);
    }

};
//
//BLEClass::BLEClass() 
//{
//  comboKeyboard = new BleComboKeyboard;
//  comboMPU = new MPU6050(Wire);
//  comboMouse = new BleComboMouse(comboKeyboard);
//}

/*
 * API Class Method Definitions 
 */ 
byte BLEClass::init()
{
  // Initialize Keyboard and Mouse Emulations
  Serial.println("Beginning keyboard");
  comboKeyboard.begin();
  Serial.println("Beginning mouse");
  comboMouse.begin();
  // Initialize and calibrate MPU6050
  Serial.println("Beginning wire");
  Wire.begin();
  Serial.println("Beginning mpu");
  byte status = comboMPU.begin();
  Serial.println("Began mpu");
  // Calculate offsets, depending on whether or not the MPU actually initialized.
  if(status !=0)
    return status;
  else
  {
    comboMPU.calcOffsets(true,true); // gyro and accel cal
    return status;
  }
}

void BLEClass::mouseMove()
{
  comboMPU.update();

#ifdef BLE_DEBUG
  Serial.print("X : ");
  Serial.print(comboMPU.getAccX());
  Serial.print("\tY : ");
  Serial.println(comboMPU.getAccY());
#endif

  comboMouse.move((comboMPU.getAccX()*-10),(comboMPU.getAccY()*10),0);
}

void BLEClass::hello()
{
  comboKeyboard.println("Hello World");
}

BLEClass Test;

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
  pinMode(buttonClickPin,INPUT);
  attachInterrupt(digitalPinToInterrupt(buttonClickPin), clickISR, HIGH);
  Serial.println("Done!\n");
}

void loop() 
{
  if(modeTrigger)
  {
    delay(100);
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
      Test.hello();
      Serial.println("Sending Enter key...");
      Test.comboKeyboard.write(KEY_RETURN);
      delay(1000);
    } 
    else 
    {
       digitalWrite(ledGreenPin, LOW);
       digitalWrite(ledBluePin, HIGH);
      Test.mouseMove();
      if(clickTrigger == HIGH)
      {
        delay(100);
        Test.comboMouse.click(MOUSE_LEFT);
        clickTrigger = LOW;
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
// Button Interrupts
void modeISR()
{
  modeTrigger = HIGH;
}
void clickISR()
{
  clickTrigger = HIGH;
}