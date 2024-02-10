# Stepper_UNIPOLAR

This class provides a generic driver for use with any center-tapped unipolar stepper motor.  Requires the use of a driver board that can convert the low-voltage/low-current digital signals from 4 pins on the ESP32 to higher-voltage/higher-current outputs suitable for direct connection to the two phases (*A* and *B*) of each coil (*1* and *2*) in the stepper motor.
 
The **Stepper_UNIPOLAR** class includes the following constructors:
  * `Stepper_UNIPOLAR(int coil1A, int coil1B, int coil2A, int coil2B)`
    * controls the driver board using only 4 digital pins from the ESP32, where the parameters specify the pin numbers.  Supports the following step type modes:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
        
❗Note: This class does not support a short brake state.  Calls to the `brake()` method, as well as setting the *endAction* parameter in the `move()` and `moveTo()` methods to **StepperControl::BRAKE** have no effect on the motor driver.<br><br>

---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
