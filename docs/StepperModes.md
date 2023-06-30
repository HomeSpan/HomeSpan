# Stepper Motor Modes

A typical stepper motor has two sets of coils, *A* and *B*.  Applying a current to one or both coils creates a magnetic field that fixes the motor's position.  Any changes to the direction and/or magnitude of the current flowing through the coils causes the motor to "step" to a new fixed position.  By repeatedly changing the current flow according to a specific pattern the motor can be made to continuously step clockwise or counterclockwise.  The specific pattern chosen, known as the *step mode* in the **StepperControl** class, determines the size of the step and overall performance of the motor.  The speed at which changes to the current flow are made determines the speed at which the motor rotates.  The modes supported by **StepperControl** are described below.

## FULL STEP TWO PHASE MODE (without PWM)

In this mode, a constant current is applied to both coils *A* and *B*. The motor is stepped by alternatively flipping the direction of the current flow in each coil as follows:

<img width="250" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/8bea7031-7325-4ded-8ebd-5554d8f1e13d"><br>

Note that though the pattern repeats after four steps, it is not symmetrical -  running the pattern from left to right causes the motor to rotate in one direction, whereas running the pattern from right to left will cause it to rotate in the opposite direction.  Many stepper motors are constructed to have 200 full steps, which means you need to repeat the above pattern 25 times to cause the motor to complete a single revolution.

Since in this mode each coil has only two possible states (i.e. the direction of the current flow), only one digital signal per coil is required to implement the stepping pattern.  However, fully flipping the direction of the current flow in a coil changes the magnetic fields very rapidly, which creates a rather clunky motion when stepping the motor.

## FULL STEP ONE PHASE MODE (without PWM)

In this mode, a constant current is alternatively applied to either *A* **or** *B*, while also flipping its direction as follows:

<img width="250" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/cbf2fea5-072e-4fef-9231-504bb483b0c0"><br>

This mode uses only half the power as the FULL STEP TWO PHASE mode since current only flows through one coil at a time.  Also, though the step size is the same, the transtition from one step to another is not as harsh since the current to each coil transtions from one direction to zero before flipping to the other direction.  However, since in this mode each coil has three possible states for current flow (positive, negative, and off), two digital signals per coil are required to implement the stepping pattern.

## HALF STEP MODE (without PWM)

Though the the step sizes for the two modes above are the same, the set of positions they represent as the motor rotates are out of sync by 1/2 step.  This means that by interleaving the four steps of the two modes together, we can create the following 8-step pattern where the step size is *half* that of a full step:
      
<img width="400" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/ec317c77-fbd9-4641-9d50-d822b477c9ec"><br>

The advantage of this mode is that the motor moves much more smoothly, and can be stopped in half-step increments for more precise control.  However, it takes twice as many steps for the motor to complete a full rotation (and thus moves twice as slow given the same delay between steps).

One other disadvantage of this mode is that in four of the steps both coils are powered, whereas in the other four only one coil is powered.  This results in uneven motor torque as the motor rotates, as well as different holding torques depending on which step the motor lies when stopped.  Depending on your specific application (raising a shade, closing a door, etc.) this may or may not be of concern.

## QUARTER STEP and EIGHTH STEP MODES (including PWM versions of the above modes)

In the above modes, current to each coil is either "fully on" in one direction, fully on in the other direction, or completely off.  As a result, there is a trade-off between step granularity and smoothness of rotation versus power and torque consistency.  The solution to this problem is to drive the current in each coil with a PWM signal (or an equivalent limiting mechanism) that allows the *magnitude* of the current flow to be controlled in addition to its direction.

Typically this is done by setting the magnitude of the current in each coil based on sinusoidal patterns offset by 90° as follows:

<img width="505" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/75a6176b-b5b4-4b85-a394-a4d6e1f9bf3d"><br>

Here, the blue dots perfectly replicate the steps of the FULL STEP ONE PHASE mode (e.g. positive/off/negative/off for coil *A*), the red dots replicate the steps of the FULL STEP TWO PHASE MODE, and the combined set of both the red blue and red dots taken together replicate the steps of the HALF STEP mode.   Each non-PWM mode above can therefore be replicated using a single set of PWM-based sinusoidal curves, but with one important difference: the PWM method yields the same power for every step in every mode, avoiding the problems of inconsistent torque described above for the non-PWM modes.  This is because power is typically proportional to the *square* of the magnitude of the current running through a coil; coil *A* follows a cosine curve; coil *B* follows a sine curve; and and $cos^2+sin^2=1$, which is a constant.

It is generally preferable to use the PWM method when running a motor in any of the three modes above, rather than running with just a constant current applied to each coil, provided that PWM signals are available.  Fortunately, many stepper motor driver boards contain a built-in PWM module.  And if they don't, you can have **StepperControl**  generate the required PWM signals using the ESP32's built-in PWM peripheral.

In addition to providing better motor performance, the PWM method also provides for even more granular stepping modes by adding "microsteps" to the step pattern.  For example, adding 8 additional steps between the half-steps, as indicated on the curves by the 8 black diamonds placed between the blue and red dots, yields a QUARTER STEP mode.  This mode is twice as "smooth" as the HALF STEP mode, but requires twice as many steps for the motor to complete a full rotation.  Halving the steps even further (not shown on the diagram) yields the EIGHTH STEP mode where each cycle contains 32 individual stepping points.  **StepperControl** supports both the QUARTER STEP and EIGHTH STEP modes, though you can easily extend this further by adding modes with 64 or 128 steps per cycle.  For motors designed to operate a window shade, such granularity is generally not needed, and too fine a granularity can cause misteps depending on the specific characteristics of the motor.

---

[↩️](Stepper.md) Back to the Stepper Motor Control page






