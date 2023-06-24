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
  * example: `myMotor.move(200,5); while(myMotor.stepsRemaining()!=0); myMotor.move(-300,5);` starts the motor turning, waits until it completes all 200 steps, and then turns the motor in the opposite direction for 300 steps

* `int position()`
  * returns the absolute position of the stepper motor, which is defined as the cumulative sum of the all positive and negative steps turned since initial start-up
  * can be called when the stepper motor is moving or when it is stopped
  * example: `myMotor.move(-800,5); while(myMotor.stepsRemaining()); myMotor.move(200,5); while(myMotor.stepsRemaining()); Serial.print(myMotor.position())` would print a value of -600 after the motor finishes turning (first one direction for 800 steps, and then the other  for 200 steps)
 
* `void setPosition(int pos)`
  * resets the current position counter to *pos*
  * this method does *not* move the motor; it only resets the internal position counter as returned by `position()`
  * this method can only be called when the motor is **not** moving (if called when the motor is turning it is ignored and the internal position counter is **not** changed)
  * example: `myMotor.move(300,5); while(myMotor.stepsRemaining()); myMotor.setPosition(-200); myMotor.move(600,5); while(myMotor.stepsRemaining()); Serial.print(myMotor.position())` would print a value of +400 after the motor finishes turning
    
* `void moveTo(int nPosition, uint32_t msDelay, endAction_t endAction=NONE)`
  * enables the stepper motor and moves it to the position *nPosition*.  Note this is a **non-blocking** function and returns immediately after being called while the stepper motor turns until it reaches *nPosition*
    
    * *nPosition* - the position to which the stepper move should move, where position is defined as the cumulative number of positive and negative steps the motor has turned since initial start-up, as returned by `position()`
    * *msDelay* - the delay, in milliseconds, to pause between steps.  Must be greater than zero.  The lower the number, the faster the motor moves, subject to limitations of the motor itself
    * *endAction* - an optional action to be performed *after* the motor has reached *nPosition*.  Choices include:
      *  **StepperControl::NONE** - no action is taken; the stepper motor is left in the enabled state (this is the default)
      *  **StepperControl::DISABLE** - current to the stepper motor is disabled
      *  **StepperControl::BRAKE** - the stepper motor is placed in a brake state
  * it is okay to call this method while the stepper motor is already turning; the motor will either continue turning in the same direction, or reverse direction, until it reaches the *nPosition* specified
  * calls to `stepsRemaining()` after calling `moveTo()` work as expected - the value returned will be the number of steps remaining until the motor reaches the *nPosition* specified
  * note that `moveTo(nPosition)` is mathematically the same as `move(nPosition-position())`, but the `moveTo()` method is more accurate since it computes the position of the motor directly inside the task that is actually controlling the motor
 
* `void setAccel(float accelSize, float accelSteps)` 

* `void setStepType(int mode)` 

---

[↩️](../README.md) Back to the Welcome page
