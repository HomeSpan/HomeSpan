# Stepper Motor Control

HomeSpan includes dedicated classes that provide for easy control of a stepper motor connected to an ESP32 via a stepper motor driver board.  These classes allow one or more stepper motors to operate smoothly and asynchronously in the background while HomeSpan continues to run in the foreground.  On devices with dual processors, stepper-motor control can be run either on the same or a different processor from HomeSpan.

The HomeSpan class that contains all the methods to control a stepper motor is called **StepperControl**.  However, this is an abstract class and cannot be instantiated directly.  Instead you instantiate stepper motor objects using driver-specific child-classes (derived from **StepperControl**) that contain all the logic to configure and operate a particular driver board.  Each child class supports one or more constructors allowing you to specify which output pins on your ESP32 device will be connected to the required pins on your driver board.

The following drivers are currently included in HomeSpan:

* **[Stepper_TB6612](StepperDrivers/Stepper_TB6612.md)**
  * This class is used to operate stepper motors driven by a [Toshiba TB6612](https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf) (or equivalent) chip
  * Can be used either with or without ESP32 PWM pins
  * See, for example, the [Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board](https://www.adafruit.com/product/2448)
   
* **[Stepper_A3967](StepperDrivers/Stepper_A3967.md)**
  * This class is used to operate stepper motors driven by an [Allegro A3967](https://cdn.sparkfun.com/datasheets/Robotics/A3967-Datasheet.pdf) (or equivalent) chip
  * See, for example, the [Sparkfun EasyDriver Stepper Motor Board](https://www.sparkfun.com/products/12779)

* **[Stepper_ULN2003A](StepperDrivers/Stepper_ULN2003A.md)**
  * This class is used to operate stepper motors driven by a [Texas Instruments ULN2003A](https://www.ti.com/lit/ds/symlink/uln2003a.pdf) (or equivalent) chip
  * See, for example, the [Opencircuit ULN2003 Stepper Motor Driver Board](https://opencircuit.shop/product/uln2003-stepper-motor-driver-module)

* **[Stepper_UNIPOLAR](StepperDrivers/Stepper_UNIPOLAR.md)**
  * This class provides a generic driver for use with any center-tapped unipolar stepper motor
  * Use requires a driver board that can convert the low-voltage/low-current digital signals from 4 pins on the ESP32 to higher-voltage/higher-current signals suitable for operating the stepper motor
  
Click on any of the driver-specific classes above for complete details on how to wire and configure a particular driver board.

## StepperControl Methods

The **StepperControl** class provides the following methods to operate and control a stepper motor object instantiated with one of the driver-specific classes described above:

* `StepperControl *enable()`
  * enables current flow to the stepper motor coils, actively holding the motor in its position
  * returns pointer to itself so methods can be daisy-chained
    * example: `myMotor=(new Stepper_TB6612(23,32,22,14,33,27))->enable();`
      
* `StepperControl *disable()`
  * disables current flow to the stepper motor coils and leaves them in a state of high impedence, allowing the motor to turn freely
  * returns pointer to itself so methods can be daisy-chained
    * example: `myMotor=(new Stepper_TB6612(23,32,22,14,33,27))->disable();`

* `StepperControl *brake()` 
  * disables current flow to the stepper motor coils but leaves them in a state of low impedence, preventing the motor from freely turning
  * applicable only for driver chips that support a "short brake" mode, otherwise has no effect
  * returns pointer to itself so methods can be daisy-chained
    * example: `myMotor=(new Stepper_TB6612(23,32,22,14,33,27))->brake();`

* `void move(int nSteps, uint32_t msDelay, endAction_t endAction=NONE)`
  * enables the stepper motor and turns it *nSteps* steps.  Note this is a **non-blocking** function and returns immediately after being called while the motor turns for *nSteps* steps in the background
    
    * *nSteps* - the number of steps to turn.  A positive number turns the motor in one direction; a negative number turns the motor in the opposite direction; a value of zero causes the motor to *stop* if it is already turning
    * *msDelay* - the delay, in milliseconds, to pause between steps.  Must be greater than zero.  The lower the number, the faster the motor turns, subject to limitations of the motor itself
    * *endAction* - an optional action to be performed *after* the motor has finished moving *nSteps* steps.  Choices include:
    
      *  **StepperControl::NONE** - no action is taken; the stepper motor is left in the enabled state (this is the default)
      *  **StepperControl::DISABLE** - current to the stepper motor is disabled
      *  **StepperControl::BRAKE** - the stepper motor is placed in a brake state
  * if this method is called while the stepper motor is already turning, the number of steps to turn will be reset to the new *nSteps* value.  It is okay to change the sign of *nSteps* to reverse the direction of motor while it is turning, though this may not be desireable depending on what your motor is connected to in the real world
  * calling this method with a value of *nSteps=0* causes the motor to stop, if it is already turning.  If the motor is not turning, calling this method with *nSteps=0* simply enables the motor and the immediately performs the *endAction* (if specified).
  * example: `myMotor.move(200,5,StepperControl::BRAKE);` starts the motor turning for 200 steps with a delay of 5ms between steps.  When the motor has completed all 200 steps, it is placed in the brake state. 
    
* `int stepsRemaining()`
  * returns the number of steps remaining to turn
  * may be positive or negative depending on the direction the motor is turning
  * returns zero if the motor is not turning
  * example: `myMotor.move(200,5); while(myMotor.stepsRemaining()!=0); myMotor.move(-300,5);` starts the motor turning, waits until it completes all 200 steps, and then turns the motor in the opposite direction for 300 steps

* `int position()`
  * returns the absolute position of the stepper motor, which is defined as the cumulative sum of the all positive and negative steps turned since initial start-up
  * can be called when the stepper motor is turning or when it is stopped
  * example: `myMotor.move(-800,5); while(myMotor.stepsRemaining()); myMotor.move(200,5); while(myMotor.stepsRemaining()); Serial.print(myMotor.position())` would print a value of -600 after the motor finishes turning (first one direction for 800 steps, and then the other for 200 steps)
 
* `void setPosition(int pos)`
  * resets the current position counter to *pos*
  * this method does *not* turn the motor; it only resets the internal position counter as returned by `position()`
  * this method is only effective when the motor is **not** turning (if called when the motor is turning the internal position counter remainms unchanged)
  * example: `myMotor.move(300,5); while(myMotor.stepsRemaining()); myMotor.setPosition(-200); myMotor.move(600,5); while(myMotor.stepsRemaining()); Serial.print(myMotor.position())` would print a value of +400 after the motor finishes turning
    
* `void moveTo(int nPosition, uint32_t msDelay, endAction_t endAction=NONE)`
  * enables the stepper motor and turns it to the position *nPosition*.  Note this is a **non-blocking** function and returns immediately after being called while the stepper motor turns until it reaches *nPosition*
    
    * *nPosition* - the position to which the stepper move should turn, where position is defined as the cumulative number of positive and negative steps the motor has turned since initial start-up, as returned by `position()`
    * *msDelay* - the delay, in milliseconds, to pause between steps.  Must be greater than zero.  The lower the number, the faster the motor turns, subject to limitations of the motor itself
    * *endAction* - an optional action to be performed *after* the motor has reached *nPosition*.  Choices include:
      
      *  **StepperControl::NONE** - no action is taken; the stepper motor is left in the enabled state (this is the default)
      *  **StepperControl::DISABLE** - current to the stepper motor is disabled
      *  **StepperControl::BRAKE** - the stepper motor is placed in a brake state
  * it is okay to call this method while the stepper motor is already turning; the motor will either continue turning in the same direction, or reverse direction, until it reaches the *nPosition* specified
  * calls to `stepsRemaining()` after calling `moveTo()` work as expected - the value returned will be the number of steps remaining until the motor reaches the *nPosition* specified
  * note that `moveTo(nPosition)` is mathematically the same as `move(nPosition-position())`, but the `moveTo()` method is more accurate since it computes the position of the motor directly inside the task that is actually controlling the motor
 
* `StepperControl *setAccel(float accelSize, float accelSteps)`
  * adds an additional set of delays between steps so that the motor gradually accelerates when it starts and decelerates when it stops
  
    * *accelSize* - the maximum size of the additional delay, expressed as a factor to be multiplied by the *msDelay* parameter used in `move()` and `moveTo()`.  Must be a value greater or equal to 0.  The larger the value, the greater the magnitude of the acceleration and deceleration. A value of zero yields no acceleration/deceleration
      
    * *accelSteps* - the number of steps over which the *accelSize* factor exponentially decays, at which point  he motor begins turning at the full speed specified by the *msDelay* parameter.  Must be a value greater or equal to 1.  The larger the value, the longer the acceleration and deceleration period
      
  * the total delay between steps (when *stepsRemaining* is not zero) is given by the following formula:
  $$totalDelay = msDelay \times (1 + accelSize \times (e^{\frac{-\mid nSteps-stepsRemaining \mid}{accelSteps}} + e^{\frac{-(\mid stepsRemaining \mid - 1)}{accelSteps}}))$$
    
  * example: `myMotor.setAccel(10,20); myMotor.move(200,5);`
    * yields a 55ms delay after the first step, a 52ms delay after the second step, a 50ms delay after the third step, and so forth, until at step 82 the additional delay has fully decayed such that the delay between steps remains fixed at the 5ms *msDelay* parameter specified.  Then, starting at step 118 (with 82 steps remaining) the delay increases to 6ms; at step 134 it further increases to 7ms, and so forth, until the delay reaches its maxmimum of 55ms once again at step 199 just before the motor stops turning at step 200

  * returns pointer to itself so methods can be daisy-chained
    * example: `myMotor=(new Stepper_TB6612(23,32,22,14,33,27))->setAccel(10,20);`
          
* `StepperControl *setStepType(int mode)`
  * sets the step type of the motor to one of the following *mode* enumerations:
      
    * **StepperControl::FULL_STEP_ONE_PHASE** (0)      
    * **StepperControl::FULL_STEP_TWO_PHASE** (1)
    * **StepperControl::HALF_STEP** (2)
    * **StepperControl::QUARTER_STEP** (4)
    * **StepperControl::EIGHTH_STEP** (8)      
  * *mode* can be specified using either the name of the enumeration or its integer equivalent
  * returns pointer to itself so methods can be daisy-chained
    * example: `myMotor=(new Stepper_TB6612(23,32,22,14,33,27))->setStepType(StepperControl::HALF_STEP);`
  * smaller step types provide for smoother operation of the motor, but require more steps to turn a complete revolution
    * not all *modes* are supported by all driver chips
    * the quarter- and eighth-step modes require microstepping PWM functionality
  * it is possible, though not recommended, to change the step type *mode* while the motor is turning
  * see [Stepper Motor Modes](StepperModes.md) for a brief primer on how stepper motors are typically driven

### CPU and Task Priority

All **StepperControl** constructors support an *optional* final parameter consisting of a *brace-enclosed* pair of numbers (not shown above).  The first number in the braces specifies the *priority* of the background task used to control the stepper motor.  The second number in the braces specifies the CPU (0 or 1) that **StepperControl** will use to run the background control task (this number is ignored for single-processor chips).  The default (and recommended) value of this optional final parameter is {1,0}.  Example:

* `new Stepper_TB6612(23,32,22,14,{0,1})` instantiates control of a stepper motor driven by a TB6612 chip, where ESP32 pins 23, 32, 22, and 14 are connected to the AIN1, AIN2, BIN1, and BIN2 pins on the TB6612, respectively; the priority of the background task is set to 0; and the task will be run on cpu 1 (applicable only for a dual-processor chip) 

## Example Sketches

Below is a simple sketch demonstrating the above methods:

```C++
// StepperControl Example using TB6612-based Driver Board with HALF STEP PWM MODE

#include "HomeSpan.h"         // HomeSpan includes all the StepperControl classes

StepperControl *motor;        // create a global pointer to StepperControl so it can be accessed in both setup() and loop()

///////////////////

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.printf("\nHomeSpan Stepper Control\n\n");

  motor=new Stepper_TB6612(23,32,22,14,33,27);      // instantiate the motor object with optional PWM pin specified (33 and 27)

  motor->setStepType(StepperControl::HALF_STEP);    // set the mode to HALF STEP, which means 400 steps are needed for a complete revolution of a 200-step motor
  motor->setAccel(10,20);                           // add acceleration parameters: extra delay is 10x, decaying over 20 steps

  Serial.printf("Moving motor 400 steps and waiting until motor stops...\n");
  
  motor->move(-400,5);              // move the motor -400 steps (1 revolution), with 5ms between steps.
  while(motor->stepsRemaining());   // wait until there no remaining steps

  Serial.printf("Moving motor to absolute position of +1200 (i.e reverse direction for 1600 steps, or 4 revolutions) without waiting...\n");
  
  motor->moveTo(1200,2,StepperControl::BRAKE);    // move the motor to an absolute position of 1200 steps with 2ms between steps; enter brake state when done

  // Motor will continue moving in background even once setup() exits and loop() below starts
}

///////////////////

void loop(){
  
  Serial.printf("Motor has %d remaining steps\n",motor->stepsRemaining());
  
  delay(1000);      // motor is unaffected by delay()
  
  if(motor->position()==1200){
    Serial.printf("Motor has reached final position and is now stopped.\n");
    while(1);
  }
}
```

### Motorized Window Shade Example

A fully worked example showing how to use the *StepperControl* class within a complete HomeSpan sketch to control a Motorize Window Shade using both the TB6612 and A3967 driver boards can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → MotorizedWindowShade*](../examples/Other%20Examples/MotorizedWindowShade).

## Creating your own **StepperControl** Driver

If none of the above motor driver classes works for your specific chip or driver board, it is relatively straightfoward to create a new driver to use in your sketch.  This is because all the logic to operate a stepper motor in the background is already embedded in the abstract **StepperControl** class.  To create your own driver, start by creating a child class derived from **StepperControl**.  Next, add a constructor that defines the pins and performs any initializations if needed.  Finally, define the following methods that **StepperControl** calls to operate the motor:

* `void onStep(boolean direction)` - contains the logic to advance the motor by a single step based on the *direction* parameter
* `void onEnable()` - contains the logic that enables the motor driver
* `void onDisable()` - contains the logic that disables the motor driver
* `void onBrake()` - contains the logic that puts the motor into a short brake state
* `StepperControl *setStepType(int mode)` - contains the logic to set the step type mode based on the *mode* parameter

Only the first method, `onStep()`, is required to be defined.  You can leave any of the other methods undefined if they are not applicable for your specific driver board.  You can of course create additional methods to reflect any other features your driver board may support.

As an example, below is the complete code for the **Stepper_A3967** class:

```C++
#include "HomeSpan.h"

//////////////////////////
 
struct Stepper_A3967 : StepperControl {

  int m1Pin;
  int m2Pin;
  int stepPin;
  int dirPin;
  int enablePin;

//////////////////////////

  Stepper_A3967(int m1Pin, int m2Pin, int stepPin, int dirPin, int enablePin, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : StepperControl(taskParams.first,taskParams.second){
    this->m1Pin=m1Pin;
    this->m2Pin=m2Pin;
    this->stepPin=stepPin;
    this->dirPin=dirPin;
    this->enablePin=enablePin;

    pinMode(m1Pin,OUTPUT);
    pinMode(m2Pin,OUTPUT);
    pinMode(stepPin,OUTPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(enablePin,OUTPUT);

    setStepType(FULL_STEP_TWO_PHASE);
  }

//////////////////////////

  void onStep(boolean direction) override {
    digitalWrite(dirPin,direction);
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepPin,LOW);      
  }

//////////////////////////

  void onEnable() override {
    digitalWrite(enablePin,0);
  }

//////////////////////////

  void onDisable() override {
    digitalWrite(enablePin,1);
  }

//////////////////////////

  StepperControl *setStepType(int mode) override {
    switch(mode){
      case FULL_STEP_TWO_PHASE:
        digitalWrite(m1Pin,LOW);
        digitalWrite(m2Pin,LOW);
        break;
      case HALF_STEP:
        digitalWrite(m1Pin,HIGH);
        digitalWrite(m2Pin,LOW);
        break;
      case QUARTER_STEP:
        digitalWrite(m1Pin,LOW);
        digitalWrite(m2Pin,HIGH);
        break;
      case EIGHTH_STEP:
        digitalWrite(m1Pin,HIGH);
        digitalWrite(m2Pin,HIGH);
        break;
      default:
        ESP_LOGE(STEPPER_TAG,"Unknown StepType=%d",mode);
    }
    return(this);
  }
  
};
```

---

[↩️](../README.md) Back to the Welcome page
