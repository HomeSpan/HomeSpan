# Stepper_UNIPOLAR

This class provides a generic driver for use with any center-tapped unipolar stepper motor.  Requires the use of a driver board that can convert the low-voltage/low-current digital signals from 4 pins on the ESP32 to higher-voltage/higher-current outputs suitable for direct connection to the two phases (*A* and *B*) of each coil (*1* and *2*) in the stepper motor.
 
The **Stepper_UNIPOLAR** class includes the following constructor:
  * `Stepper_UNIPOLAR(int coil1A, int coil1B, int coil2A, int coil2B)`
    * controls the driver board using 4 digital pins from the ESP32, where the parameters specify the pin numbers
    * the driver circuit should be connected and configured such that when any of ESP32 pins specified above are set to are HIGH, current flows through the corresponding coil/phase. Similarly, when a pin is set LOW, the driver circuit should stop the flow of current through the corresponding coil/phase.
    * supported modes are as follows:
      
      * FULL_STEP_ONE_PHASE
      * FULL_STEP_TWO_PHASE
      * HALF_STEP
        
❗Note: This class does not support a short brake state.  Calls to the `brake()` method, as well as setting the *endAction* parameter in the `move()` and `moveTo()` methods to **StepperControl::BRAKE** have no effect on the motor driver.<br><br>

> [!WARNING]
> **Note the order of the constructor parameters!**  The first two parameters specify the ESP32 pins that control the current flowing through phases *A* and *B* of ***Coil 1***;  the second two parameters are for phases *A* and *B* of ***Coil 2***.

It does *not* matter which coil is defined as *1* or *2*, nor which side is called *A* or *B*, as long as the first two parameters are for one of the coils and the second two are for the other coil.  You'll know if you mis-specified the order of the pins because the motor will vibrate back and forth instead of turning clockwise or counterclockwise.

## Technical Details

The patterns by which this class sets the specified pins HIGH and LOW depend on the *step mode* chosen as follows:

#### FULL_STEP_ONE_PHASE
#### FULL_STEP_TWO_PHASE
#### HALF_STEP

---

[↩️](../Stepper.md) Back to the Stepper Motor Control page
