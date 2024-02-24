# Stepper_ULN2003A

This is a derived class of **StepperControl** designed to operate stepper motors driven by a [Texas Instruments ULN2003A](https://www.ti.com/lit/ds/symlink/uln2003a.pdf) (or equivalent) chip.

The Texas Instruments ULN2003A chip containins an array of seven Darlington transistor pairs each capable of converting low-voltage/low-current digital signals into higher-voltage/higher-current outputs suitable for driving a stepper motor.[^1]

[^1]: Only four of the seven Darlington transistor pairs in the ULN2003A are needed to drive a typical unipolar stepper motor.

Various manufacturers have incorporated this chip into a dedicated stepper motor board designed to drive unipolar motors such the [28BYJ&#8209;48 5&#8209;Volt Stepper](https://opencircuit.shop/product/28byj-48-5v-stepper-motor-4-phase-5-wire) and [28BYJ&#8209;48 12&#8209;Volt Stepper](https://opencircuit.shop/product/28byj-48-12v-stepper-motor-4-phase-5-wire).  Wiring for the [Opencircuit ULN2003 Stepper Motor Driver Board](https://opencircuit.shop/product/uln2003-stepper-motor-driver-module) that uses this chip is as follows:

#### **Power Connections**[^2]
  * ➕ - connect to an external DC power supply that will drive stepper motor (5-12V)
  * ➖ - connect to GND on the ESP32, and to ground of external DC power supply
#### **Motor Connections**
  *  plug the motor directly into the board's 5-pin connector
#### **Control Connections**
  * *IN1, IN2, IN3, IN4* - connect to four digital pins on the ESP32 - used to step the motor in either direction
 
The **Stepper_ULN2003A** class includes the following constructors:
  * `Stepper_ULN2003A(int IN1, int IN2, int IN3, int IN4)`
    * controls the driver board using only 4 digital pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
        
❗Note: The ULN2003A chip does not support a short brake state.  Calls to the `brake()` method, as well as setting the *endAction* parameter in the `move()` and `moveTo()` methods to **StepperControl::BRAKE** have no effect on the motor driver.<br><br>

> [!TIP]
> If you set the motor to move very slowly (e.g. 500ms per step) you will be able to track how the current is turned on and off for each phase of the motor coils depending on the *step mode* selected by observing the four LEDs (labeled *A, B, C, D*) built into the driver board.  See also this [Last Minute Engineers Tutorial](https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial) for a detailed presentation of the ULN2003A and its use to drive a 28BYJ&#8209;48 5&#8209;Volt Stepper Motor.


[^2]: the ULN2003A is a passive chip - there is no VCC power connection between the driver board and the ESP32

---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
