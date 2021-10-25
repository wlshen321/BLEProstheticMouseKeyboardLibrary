//calibrate_air_mouse
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Arduino.h>
#include "BleMouse.h"

BleMouse bleMouse;

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

void setup() {
  Serial.begin(115200);
  bleMouse.begin();
  Wire.begin();
  mpu.initialize();
  uint8_t deviceID = mpu.getDeviceID();
  Serial.println("Device ID: " + String(deviceID));
  if (!mpu.testConnection()) { Serial.println("Failed connection to mpu"); while (1); }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = (gx+300)/150;  // change 300 from 0 to 355
  vy = -(gz-100)/150; // same here about "-100"  from -355 to 0

  Serial.print("gx = ");
  Serial.print(gx);
  Serial.print(" | gz = ");
  Serial.print(gz);
  
  Serial.print("        | X = ");
  Serial.print(vx);
  Serial.print(" | Y = ");
  Serial.println(vy);

  if(bleMouse.isConnected()){
    bleMouse.move(vx, vy);
  }
  
  delay(20);
}
