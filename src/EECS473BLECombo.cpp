/*
----------------------------------
-----Includes-----
----------------------------------
*/
//Libraries that combine BLE Keyboard and Mouse control
//#include <BleCombo.h> // This is commented out, because I want to create the class instantiations within the .ino file, rather than using the class instantiations within BleCombo.h
#include <BleComboKeyboard.h>
#include <BleComboMouse.h>
#include <BleConnectionStatus.h>
#include <KeyboardOutputCallbacks.h>
//Libraries that control the MSP6050
#include "Wire.h"
#include <MPU6050_light.h>

#include "EECS473BLECombo.h"
/*
----------------------------------
---Defines-------
----------------------------------
*/
#define BLE_DEBUG 0
/*
----------------------------------
---Method Initializations-------
----------------------------------
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

#if BLE_DEBUG
  Serial.print("X : ");
  Serial.print(comboMPU.getAccX());
  Serial.print("\tY : ");
  Serial.println(comboMPU.getAccY());
#endif

  comboMouse.move((comboMPU.getAccY()*15),(comboMPU.getAccZ()*-15),0);
}

void BLEClass::hello()
{
  comboKeyboard.println("Hello World");
}
