# Stepper Motor Modes

A typical stepper motor has two sets of coils, *A* and *B*.  Applying a current to one or both coils creates a magnetic field that fixes the motor's position.  Any changes to the direction and/or magnitude of the current flowing through the coils causes the motor to "step" to a new fixed position.  By repeatedly changing the current flow according to a specific pattern the motor can be made to continuously step clockwise or counterclockwise.  The specific pattern chosen, known as the *step mode* in the **StepperControl** class, determines the size of the step and overall performance of the motor.  The speed at which changes to the current flow are made determines the speed at which the motor rotates.

**StepperControl** supports two different types of step modes: those that simply set the direction of the current flow to either fully on or off, and those that also use PWM to modulate the magnitude of the current flow.

![fullStep2](images/fullStepTwoPhase.png)

![fullStep1](images/fullStepOnePhase.png)

![halfStep](images/halfStep.png)

![pwmSteps](images/StepperPWM.png)
