/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
// This is a placeholder .ino file that allows you to easily edit the contents of this files using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "Stepper_TB6612.h"   // include the driver for a TB6612 chip

StepperControl *motor;        // create a global pointer to StepperControl so it can be accessed in both setup() and loop()

///////////////////

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.printf("\nHomeSpan Stepper Control\n\n");

  motor=new Stepper_TB6612(23,32,22,14,33,27);      // instantiate the motor object (with PWM pins 33 and 27 specified)

  motor->setStepType(StepperControl::HALF_STEP);    // set the mode to half-step, which means 400 steps are needed for a complete revolution for a 200-step motor
  motor->setAccel(10,20);                           // add acceleration parameters of 10x spread of 20 steps

  Serial.printf("Moving motor 400 steps and waiting until motor stops...\n");
  
  motor->move(-400,5);              // move the motor -400 steps (1 revolution), with 2ms between steps.
  while(motor->stepsRemaining());   // wait until there no remaining steps

  Serial.printf("Moving motor to absolute position of +1200 (reversing a total of 1600 steps, or 4 revolutions) without waiting...\n");
  
  motor->moveTo(1200,2,StepperControl::BRAKE);    // move the motor to an absolute position of 1200 steps with 3ms between steps; enter brake state when done

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
