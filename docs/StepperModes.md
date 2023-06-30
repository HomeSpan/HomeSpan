# Stepper Motor Modes

A typical stepper motor has two sets of coils, *A* and *B*.  Applying a current to one or both coils creates a magnetic field that fixes the motor's position.  Any changes to the direction and/or magnitude of the current flowing through the coils causes the motor to "step" to a new fixed position.  By repeatedly changing the current flow according to a specific pattern the motor can be made to continuously step clockwise or counterclockwise.  The specific pattern chosen, known as the *step mode* in the **StepperControl** class, determines the size of the step and overall performance of the motor.  The speed at which changes to the current flow are made determines the speed at which the motor rotates.  The modes supported by **StepperControl** are described below.

## FULL STEP TWO PHASE (without PWM)

In this mode, a constant current is applied to both coils *A* and *B*. The motor is stepped by alternatively flipping the direction of the current flow in each coil as follows:

<img width="250" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/8bea7031-7325-4ded-8ebd-5554d8f1e13d"><br>

Note that though the pattern repeats after four steps, it is not symmetrical -  running the pattern from left to right causes the motor to rotate in one direction, whereas running the pattern from right to left will cause it to rotate in the opposite direction.  Many stepper motors are constructed to have 200 full steps, which means you need to repeat the above pattern 25 times to cause the motor to complete a single revolution.

Since in this mode each coil has only two possible states (i.e. the direction of the current flow), only one digital signal per coil is required to implement the stepping pattern.  However, fully flipping the direction of the current flow in a coil changes the magnetic fields very rapidly, which creates a rather clunky motion when stepping the motor.

## FULL STEP ONE PHASE (without PWM)

In this mode, a constant current is alternatively applied to either *A* **or** *B*, while also flipping its direction as follows:

<img width="250" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/cbf2fea5-072e-4fef-9231-504bb483b0c0"><br>

This mode uses only half the power as the FULL STEP TWO PHASE mode since current only flows through one coil at a time.  Also, though the step size is the same, the transtition from one step to another is not as harsh since the current to each coil transtions from one direction to zero before flipping to the other direction.  However, since in this mode each coil has three possible states for current flow (positive, negative, and off), two digital signals per coil are required to implement the stepping pattern.

## HALF STEP (without PWM)

Though the the step sizes for the two modes above are the same, the set of positions they represent as the motor rotates are out of sync by 1/2 step.  This means that by interleaving the four steps of the two modes together, we can create the following 8-step pattern where the step size is *half* that of a full step:
      
<img width="400" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/ec317c77-fbd9-4641-9d50-d822b477c9ec"><br>

The advantage of this mode is that the motor moves much more smoothly, and can be stopped in half-step increments for more precise control.  However, it takes twice as many steps for the motor to complete a full rotation (and thus moves twice as slow given the same delay between steps).

One other disadvantage of this mode is that in four of the steps both coils are powered, whereas in the other four only one coil is powered.  This results in uneven motor torque as the motor rotates, as well as different holding torques depending on which step the motor lies when stopped.  Depending on your specific application (raising a shade, closing a door, etc.) this may or may not be of concern.

<img width="505" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/75a6176b-b5b4-4b85-a394-a4d6e1f9bf3d"><br>
