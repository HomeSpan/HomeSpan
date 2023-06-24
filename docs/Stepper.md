# Stepper Motor Control

HomeSpan includes dedicated classes that provide for easy control of a stepper motor connected to an ESP32 via a stepper motor driver board.  These classes allow one or more stepper motors to operate smoothly and asynchronously in the background while HomeSpan continues to run in the foreground.  On devices with dual processors, stepper-motor control can be run either on the same or a different processor from HomeSpan.

The HomeSpan class that contains all the methods to control a stepper motor is called **StepperControl**.  However, this is a virtual class and cannot be instantiated directly.  Instead you instantiate stepper motor objects using driver-specific child-classes (derived from **StepperControl**) that contain all the logic to configure and operate a particular driver board.  Each child class supports one or more constructors allowing you to specify which output pins on your ESP32 device will be connected to the required pins on your driver board:

* **Stepper_TB6612**
  * This class is used to operate stepper motors driven by a [Toshiba TB6612](www.adafruit.com/product/2448) chip, either with or without the use of ESP32 PWM pins
  * To use, add the following to the top of your sketch: `#include "extras/Stepper_TB6612.h"`
  * Contructors:
    * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2)`  - controls the driver board using only 4 digital pins
    * `Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2, int PWMA, int PWMB)`  -  adds PWM control of the motor, allowing for additional stepping modes
   
* **Stepper_A3967**
  * This class is used to operate stepper motors driven by an [Allegro A3967](https://www.sparkfun.com/products/12779) chip
  * To use, add the following to the top of your sketch: `#include "extras/Stepper_A3967.h"`
  * Contructor: `Stepper_A3967(int M1, int M2, int STEP, int DIR, int ENABLE)`
 
Click on either of the driver-specific classes above for complete details on how to wire and configure a particular driver board.

## StepperControl Methods

The **StepperControl** class provides the following methods to operate and control a stepper motor object instantiated with one of the driver-specific classes described above:

* `void enable()`
  * enables current flow to the stepper motor coils, actively holding the motor in its position

* `void disable()`
  * disables current flow to the stepper motor coils and leaves in a state of high impedence, allowing the motor to turn freely

* `void brake()` 
  * disables current flow to the stepper motor coils and leaves in a state of low impedence, creating "friction" that prevents the motor from freely turning
  * applicable only for driver chips that support a "short brake" mode, otherwise has no effect

* `void move(int nSteps, uint32_t msDelay, endAction_t endAction=NONE)`
  * enables the stepper motor and moves it *nSteps* steps.  Note this is a **non-blocking** function and returns immediately after being called while the stepper turns for *nSteps* steps in the background
    
    * *nSteps* - the number of steps to move.  A positive number moves the motor in one direction; a negative number moved the motor in the opposite direction; a value of zero causes the motor to *stop* if it is already moving
    * *msDelay* - the delay, in milliseconds, to pause between steps.  Must be greater than zero.  The lower the number, the faster the motor moves, subject to limitations of the motor itself
    * *endAction* - an optional action to be performed *after* the motor has finished moving *nSteps* steps.  Choices include:
      *  **StepperControl::NONE** - no action is taken; the stepper motor is left in the enabled state (this is the default)
      *  **StepperControl::DISABLE** - current to the stepper motor is disabled
      *  **StepperControl::BRAKE** - the stepper motor is placed in a brake state
  * if this method is called while the stepper motor is moving, the number of steps to turn will be reset to the new *nSteps* value.  It is okay to change the sign of *nSteps* to reverse the motor while it is moving, though this may not be desireable depending on what your motor is connected to in the real world
  * calling this method with a value of *nSteps=0* causes the motor to stop, if it is moving.  If the motor is not moving, calling this method with *nSteps=0* simply enables the motor and the immediately performs the *endAction* (if specified).
  * example: `myMotor.move(200,5,StepperControl::BRAKE);` starts the motor turning for 200 steps with a delay of 5ms between steps.  When the motor has completed all 200 steps, it is placed in the brake state where inductive "friction" holds it in place 
    
* `int stepsRemaining()`
  * returns the number of steps remaining to turn
  * may be positive or negative depending on the direction the motor is turning
  * returns zero when the motor is not turning
  * example: `myMotor.move(200,5,StepperControl::BRAKE); while(myMotor.stepsRemaining()!=0);` starts the motor turning and then waits until is completes all 200 steps

---

[↩️](../README.md) Back to the Welcome page
