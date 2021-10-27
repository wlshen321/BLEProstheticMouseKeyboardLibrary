#include <BleConnectionStatus.h>
#include <BleMouse.h>

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

BleMouse testMouse;

int mouseX,mouseY;
long timer = 0;

void setup() {
  Serial.begin(9600);
  /*Init MPU6050*/
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");

  /*Init BLE Mouse*/
  testMouse.begin();
  Serial.println("TestMouse Init");
}

void loop() {
  mpu.update();

  /*If the bluetooth module is connected, move the mouse with the corresponding accel data*/
  if(millis() - timer > 100 && testMouse.isConnected())
  {
    /*Get Acceleration in units of Gs and turn them into values that can be easily observed when moving the mouse.*/
    mouseX = mpu.getAccX()*10;
    mouseY = mpu.getAccY()*-10;
    Serial.print(F("Mouse Movement  X: "));
    Serial.print(mouseX);
    Serial.print("\t");Serial.print("Y:");
    Serial.println(mouseY);
    testMouse.move(mouseX,mouseY,0);
    timer = millis();
  }
  /*If the bluetooth module isnt connected, let the user know*/
  else if(millis() - timer > 100)
  {
    Serial.println("Not Connected to Bluetooth");
    timer = millis();
  }
    

}
