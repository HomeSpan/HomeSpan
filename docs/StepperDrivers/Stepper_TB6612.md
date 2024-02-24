# Stepper_TB6612

This is a derived class of **StepperControl** designed to operate stepper motors driven by a [Toshiba TB6612](https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf) (or equivalent) chip, either with or without the use of ESP32 PWM pins.

The Toshiba TB6612 is a generic motor driver providing direct control of two full H-bridges.  Wiring for the [Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board](https://learn.adafruit.com/adafruit-tb6612-h-bridge-dc-stepper-motor-driver-breakout) that uses this chip is as follows:

#### **Power Connections**
  * *VCC* - connect to +3.3V on ESP32
  * *VM+* - connect to external DC power supply that will drive stepper motor (5-13V)
  * *GND* - connect to GND on the ESP32, and to ground of external DC power supply
#### **Motor Connections**
  *  *MOTORA* - connect to the "A" coil of the stepper motor
  *  *MOTORB* - connect to the "B" coil of the stepper motor
#### **Control Connections**
  * *AIN1, AIN2* - connect to two digital pins on the ESP32 - used to control direction and state of coil *A*
  * *BIN1, BIN2* - connect to two digital pins on the ESP32 - used to control direction and state of coil *B*
  * *PWMA, PWMB* - if using PWM, connect to two digital pins on the ESP32; if not, connect to +3.3V on ESP32 to pull high
  * *STBY* - connect to +3.3V on ESP32 to pull high
 
The **Stepper_TB6612** class includes two constructors:
  * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2)`
    * controls the driver board using only 4 digital pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
      
  * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2, int PWMA, int PWMB)`
    * controls the driver board using 4 digital pins and 2 PWM pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
      * QUARTER_STEP
      * EIGHTH_STEP
                
---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
