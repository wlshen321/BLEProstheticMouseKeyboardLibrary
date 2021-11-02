#ifndef EECS473_BLECOMBO_h
#define EECS473_BLECOMBO_h
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


/*
----------------------------------
-------------Class Definition-----
----------------------------------
*/
class BLEClass
{
  public:
    int c_accelx;
    int c_accely;
    BleComboKeyboard comboKeyboard;
    MPU6050 comboMPU;
    BleComboMouse comboMouse; // if I put &comboKeyboard in here, it wont compile. How do I fix this?
    BLEClass()
    : comboKeyboard{}, comboMouse(&comboKeyboard), comboMPU(Wire)
    {
    }
    /*
    ----------------------------------
    -----Method Prototypes-----
    ----------------------------------
    */
   byte init();

   void mouseMove();

   void hello();
};
#endif // EECS473_BLECOMBO_h