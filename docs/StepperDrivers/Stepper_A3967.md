# Stepper_A3967

This is a derived class of **StepperControl** designed to operate stepper motors driven by an [Allegro A3967](https://cdn.sparkfun.com/datasheets/Robotics/A3967-Datasheet.pdf) (or equivalent) chip.

The Allegro A3967 is a specialized driver designed for stepper motors. It contains a built-in PWM generator and pre-programmed stepping modes.  Wiring for the [Sparkfun EasyDriver Stepper Motor Board](https://learn.sparkfun.com/tutorials/easy-driver-hook-up-guide?_ga=2.152816825.1841726212.1688220137-156607829.1686369274) that uses this chip is as follows:

#### **Power Connections**
  * *GND* - connect to GND on the ESP32, and to ground of external DC power supply
  * *M+* - connect to external DC power supply that will drive stepper motor.  An on-board regulator also uses this supply to provide VCC to the rest of the board. For use with an ESP32, you must short the 3.3V/5V jumper with a blob of solder to select 3.3V
#### **Motor Connections**
  *  *Motor A* - connect to the "A" coil of the stepper motor
  *  *Motor B* - connect to the "B" coil of the stepper motor
#### **Control Connections**
  * *MS1, MS2* - connect to two digital pins on the ESP32 - used to set the step type mode
  * *STEP, DIR* - connect to two digital pins on the ESP32 - used to step the motor and set the direction
  * *ENABLE* - connect to a digital pin on the ESP32 - used to enable/disable to motor driver
  * *SLEEP, RESET* - already pulled high on the EasyDriver board, so no connection neeed.  If using a different driver board, ensure these pins are pulled high, else connect to VCC
  * *PFD* - not used
 
The **Stepper_A3967** class includes the following constructor:
  * `Stepper_A3967(int MS1, int MS2, int STEP, int DIR, int ENABLE)`
    * controls the driver board using 5 digital pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
          
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
      * QUARTER_STEP
      * EIGHTH_STEP

❗Note: The A3967 chip does not support a short brake state.  Calls to the `brake()` method, as well as setting the *endAction* parameter in the `move()` and `moveTo()` methods to **StepperControl::BRAKE** have no effect on the motor driver.

---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
