# Stepper_A3967

TO BE EDITED


This is a derived class of **StepperControl** designed to operate stepper motors driven by a [Toshiba TB6612](https://www.adafruit.com/product/2448) chip, either with or without the use of ESP32 PWM pins.  To use, add `#include "extras/Stepper_TB6612.h"` to the top of your sketch.

The Toshiba TB6612 is a generic motor driver providing direct control of two full H-bridges.  Wiring is as follows (using Toshiba's names for each pin):

#### **Power Connections**
  * *VCC* - connect to +3.3V on ESP32
  * *VM* - connect to external DC power supply that will drive stepper motor (5-13V)
  * *GND* (and *PGND1/PGND2*) - connect to GND on the ESP32, and to ground of external DC power supply
#### **Motor Connections**
  *  *AO1, AO2* - connect to the "A" coil of the stepper motor
  *  *BO1, BO2* - connect to the "B" coil of the stepper motor
#### **Control Connections**
  * *AIN1, AIN2* - connect to two digital pins on the ESP32 - will control direction and state of coil *A*
  * *BIN1, BIN2* - connect to two digital pins on the ESP32 - will control direction and state of coil *B*
  * *PWMA, PWMB* - if using PWM, connect to two digital pins on the ESP32; if not, connect to +3.3V on ESP32 to pull high
  * *STBY* - connect to +3.3V on ESP32 to pull high
 
The **Stepper_A3967** class includes two constructors:
  * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2 [,{uint32_t priority, uint32_t cpu}] )`
    * controls the driver board using only 4 digital pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
      
  * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2, int PWMA, int PWMB, [,{uint32_t priority, uint32_t cpu}])`
    * controls the driver board using 4 digital pins and 2 PWM pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
      * QUARTER_STEP
      * EIGHTH_STEP
        
Both constructors support an *optional* final parameter consisting of a *brace-enclosed* pair of numbers.  The first number in the braces specifies the *priority* of the background task used to control the stepper motor.  The second number in the braces specifies the CPU (0 or 1) that **Stepper_A3967** will use to run the background control task (this number is ignored for single-processor chips).  The default (and recommended) value of this optional final parameter is {1,0}.  
        
---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
