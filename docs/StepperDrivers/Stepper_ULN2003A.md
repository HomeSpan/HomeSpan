# Stepper_ULN2003A

This is a derived class of **StepperControl** designed to operate stepper motors driven by a [Texas Instruments ULN2003A](https://www.ti.com/lit/ds/symlink/uln2003a.pdf) (or equivalent) chip.

The Texas Instruments ULN2003A chip containins an array of seven Darlington transistor pairs each capable of converting low-voltage/low-current digital signals into higher-voltage/higher-current outputs suitable for driving a stepper motor.[^1]

[^1]: Only four of the seven Darlington transistor pairs in the ULN2003A are needed to drive a typical unipolar stepper motor.

Various manufacturers have incorporated this chip into a dedicated stepper motor board designed to drive unipolar motors such as these [28BYJ-48 5-Volt](https://opencircuit.shop/product/28byj-48-5v-stepper-motor-4-phase-5-wire) and [28BYJ-48 12-Volt](https://opencircuit.shop/product/28byj-48-12v-stepper-motor-4-phase-5-wire) Stepper Motors.  Wiring for the [Opencircuit ULN2003 Stepper Motor Driver Board](https://opencircuit.shop/product/uln2003-stepper-motor-driver-module) that uses this chip is as follows:

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
