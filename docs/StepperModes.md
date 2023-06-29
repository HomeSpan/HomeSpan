# Stepper Motor Modes

A typical stepper motor has two sets of coils, *A* and *B*.  Applying a current to one or both coils creates a magnetic field that fixes the motor's position.  Any changes to the direction and/or magnitude of the current flowing through the coils causes the motor to "step" to a new fixed position.  By repeatedly changing the current flow according to a specific pattern the motor can be made to continuously step clockwise or counterclockwise.  The specific pattern chosen, known as the *step mode* in the **StepperControl** class, determines the size of the step and overall performance of the motor.  The speed at which changes to the current flow are made determines the speed at which the motor rotates.  The modes supported by **StepperControl** are described below.

### FULL STEP TWO PHASE

In this mode, a constant current is applied to both coils *A* and *B*. The motor is stepped by alternatively flipping the direction of the current flow in each coil as follows:

<img width="230" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/8bea7031-7325-4ded-8ebd-5554d8f1e13d"><br>

Note that though the pattern repeats after four steps, it is not symmetrical -  running the pattern from left to right causes the motor to rotate in one direction, whereas running the pattern from right to left will cause it to rotate in the opposite direction.
Many stepper motors are constructed to have 200 full steps, which means you need to repeat the above pattern 25 times to cause the motor to complete a single revolution.

Since in this mode each coil has only two possible states (i.e. the direction of the current flow), only one digital signal per coil is required to fully specify the stepping pattern.
However, fully flipping the direction of the current flow in a coil changes the magnetic fields very rapidly, which creates a rather clunky motion when stepping the motor.

### FULL STEP ONE PHASE

In this mode, 
			
<img width="291" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/cbf2fea5-072e-4fef-9231-504bb483b0c0"><br>
						
<img width="434" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/ec317c77-fbd9-4641-9d50-d822b477c9ec"><br>

<img width="505" alt="image" src="https://github.com/HomeSpan/HomeSpan/assets/68477936/75a6176b-b5b4-4b85-a394-a4d6e1f9bf3d"><br>

