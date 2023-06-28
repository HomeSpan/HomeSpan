# Stepper Motor Modes

A typical stepper motor has two sets of coils, *A* and *B*.  Applying a current to one or both coils creates a magnetic field that fixes the motor's position.  Any changes to the direction and/or magnitude of the current flowing through the coils causes the motor to "step" to a new fixed position.  By repeatedly changing the current flow according to a specific pattern the motor can be made to continuously step clockwise or counterclockwise.  The specific pattern chosen, known as the *step mode* in the **StepperControl** class, determines the size of the step and overall performance of the motor.  The speed at which changes to the current flow are made determines the speed at which the motor rotates.

**StepperControl** supports two different types of step modes: constant-current modes only set the direction of the current flow to either fully on or off, and PWM modes that use a PWM signal to also modulate the magnitude of the current flow.

### Constant-Current Stepper Modes

The most basic pattern you can use to drive a stepper motor is to sequentiually flip the direction of a constant current applied to the *A* and *B* coils by changing the voltage across their coil wires from positive to negative in the following manner:

<img width="290" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/8bea7031-7325-4ded-8ebd-5554d8f1e13d">

**StepperControl** refers to this pattern as the **FULL_STEP_TWO_PHASE** mode because each flip of the direction of the current in one of the coils (or phases) results in the motor moving a full step, and both phases are always powered.

Note that though the pattern repeats after four steps, it is not symmetrical -  running the pattern from left to right causes the motor to rotate in one direction, whereas running the pattern from right to left will cause it to rotate in the opposite direction.  Many stepper motors are constructed to have 200 distinct steps, which means you need to repeat te above pattern 25 times to cause the motor to complete a single revolution.

One advantage of the **FULL_STEP_TWO_PHASE** mode is that it is easy to implement since you only need a single.


				
<img width="291" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/cbf2fea5-072e-4fef-9231-504bb483b0c0"><br>
						
<img width="434" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/ec317c77-fbd9-4641-9d50-d822b477c9ec"><br>

<img width="505" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/75a6176b-b5b4-4b85-a394-a4d6e1f9bf3d"><br>
