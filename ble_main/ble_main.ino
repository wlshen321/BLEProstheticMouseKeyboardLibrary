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
void setup() 
{
  // Set up serial initialization
  Serial.begin(115200);
  Serial.println("Starting work!"); // Debug Code
  // Initialize Keyboard and Mouse Emulations
  Keyboard.begin();
  Mouse.begin();
  // Initialize and calibrate MPU6050
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
//  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
}

void loop() 
{
  // Ensure that Keyboard/Mouse Combination is connected before performing BLE commands
  if(Keyboard.isConnected()) 
  {
    // BLE Keyboard Testing onto Serial Monitor and BLE Keyboard
    Serial.println("Sending 'Hello world'");
    Keyboard.println("Hello World");
    delay(100);
    Serial.println("Sending Enter key...");
    Keyboard.write(KEY_RETURN);
    delay(100);
    // MPU6050/BLE Mouse Test Code onto Serial Monitor and BLE Mouse
    mpu.update();
    Serial.print("X : ");
    Serial.print(mpu.getAccX());
    Serial.print("\tY : ");
    Serial.print(mpu.getAccY());
    Mouse.move((mpu.getAccX()*10),(mpu.getAccY()*10),0);
    delay(100);
  }
  // Notifies user if the Keyboard/Mouse combination is not connected, without sending BLE commands
  else
  {
    Serial.println("BLE Keyboard/Mouse not connected. Waiting 2 seconds...");
    delay(2000);
  }
}
