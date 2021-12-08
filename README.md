# arm_ble ![GPLv3 License](https://img.shields.io/github/license/wlshen321/arm_ble)
# Overview
This project combines an "Air Mouse" with wireless keyboard. It uses the MPU6050 for orientation measurements to control mouse movement, and the ESP32 for general processing + Bluetooth.

Libraries used include the MPU6050 light library by [rfetick](https://www.github.com/rfetick), ESP32-BLE-Combo library by [ServAlex](https://www.github.com/ServAlex) forked from [blackketter](https://www.github.com/blackketter) and [T-vK](https://www.github.com/T-vK)'s BLE repository.

This is part of a larger project, where a prosthetic arm doubles as a remote mouse/keyboard based on (intended) hand gesture. More information coming soon.

# Libraries Used In the Creation of this Code
MPU library: https://github.com/rfetick/MPU6050_light (ver 1.2.1) **Note when using our custom PCB design, locate the MPU6050_light.h file which will be located where your Arduino Libraries are stored in the MPU6050_light folder. In the .h file change the value for MPU6050_ADDR from 0x68 to 0x69**

Install it by looking up "MPU6050" in the library manager. Follow the wiring instructions in this tutorial: https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/.
<br/> <br/>
BLE library: https://github.com/ServAlex/ESP32-BLE-Combo (ver 0.2.2)

Install it by following instructions in the github link

Using Arduino IDE and arduino.h for programming the ESP32 (https://github.com/espressif/arduino-esp32) (ver 1.0.6)

Install it by following instructions in the github link

# Installation Instructions
As mentioned above, first follow each repo's respecive instructions for their proper installations. Then, download the files within the "src" folder. Lastly, put the files witnin the "src" folder into a .zip file and use the following link to install the library: (https://www.arduino.cc/en/guide/libraries#toc4). When using this repo, use the src file from Tag2 for the most up to date code.

# Utilizing the Example Code/Schematic
Regarding hardware, the only thing that is needed is an ESP32-WROOM-32 Module.

In order to connect the ESP32-WROOM-32 Module to the computer, while running the example code, follow the instructions in the link: (https://support.microsoft.com/en-us/windows/pair-a-bluetooth-device-in-windows-2be7b51f-6ae9-b757-a3b9-95ee40c3e242#WindowsVersion=Windows_10)


