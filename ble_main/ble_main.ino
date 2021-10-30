//Libraries that combine BLE Keyboard and Mouse control
//#include <BleCombo.h> // This is commented out, because I want to create the class instantiations within the .ino file, rather than using the class instantiations within BleCombo.h
#include <BleComboKeyboard.h>
#include <BleComboMouse.h>
#include <BleConnectionStatus.h>
#include <KeyboardOutputCallbacks.h>
//Libraries that control the MSP6050
#include "Wire.h"
#include <MPU6050_light.h>


// Class instantiations for BLE Mouse/Keyboard control
BleComboKeyboard Keyboard;
BleComboMouse Mouse(&Keyboard);
// Class instantiation for MPU6050
MPU6050 mpu(Wire);
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
class AccelerometerMouseKeyboard
{
  public:
    byte init();
    void mouseMove();
    
};

/*
 * API Class Method Definitions 
 */ 
byte AccelerometerMouseKeyboard::init()
{
  // Initialize Keyboard and Mouse Emulations
  Keyboard.begin();
  Mouse.begin();
  // Initialize and calibrate MPU6050
  Wire.begin();
  byte status = mpu.begin();
  // Calculate offsets, depending on whether or not the MPU actually initialized.
  if(status !=0)
    return status;
  else
  {
    mpu.calcOffsets(true,true); // gyro and accel cal
    return status;
  }
}
void AccelerometerMouseKeyboard::mouseMove()
{
  mpu.update();
  Serial.print("X : ");
  Serial.print(mpu.getAccX());
  Serial.print("\tY : ");
  Serial.println(mpu.getAccY());
  Mouse.move((mpu.getAccX()*-10),(mpu.getAccY()*10),0);
}

AccelerometerMouseKeyboard Test;

void setup() 
{
  // Set up serial initialization
  Serial.begin(115200);
  Serial.println("Starting work!"); // Debug Code
//  // Initialize Keyboard and Mouse Emulations
//  Keyboard.begin();
//  Mouse.begin();
//  // Initialize and calibrate MPU6050
//  Wire.begin();
//  byte status = mpu.begin();
//  Serial.print(F("MPU6050 status: "));
//  Serial.println(status);
//  while(status!=0){ } // stop everything if could not connect to MPU6050
//  Serial.println(F("Calculating offsets, do not move MPU6050"));
//  delay(1000);
//  mpu.calcOffsets(true,true); // gyro and accel cal
  Test.init();
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
  if(Keyboard.isConnected()) 
  {
    if (modeState == HIGH) 
    {
      digitalWrite(ledGreenPin, HIGH);
      digitalWrite(ledBluePin, LOW);
      // BLE Keyboard Testing onto Serial Monitor and BLE Keyboard
      Serial.println("Sending 'Hello world'");
      Keyboard.println("Hello World");
      Serial.println("Sending Enter key...");
      Keyboard.write(KEY_RETURN);
      delay(1000);
    } 
    else 
    {
       digitalWrite(ledGreenPin, LOW);
       digitalWrite(ledBluePin, HIGH);
      // //Move Mouse
      // mpu.update();
      // Serial.print("X : ");
      // Serial.print(mpu.getAccX());
      // Serial.print("\tY : ");
      // Serial.println(mpu.getAccY());
      // Mouse.move((mpu.getAccX()*-10),(mpu.getAccY()*10),0);
      Test.mouseMove();
      if(clickTrigger == HIGH)
      {
        delay(100);
        Mouse.click(MOUSE_LEFT);
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
