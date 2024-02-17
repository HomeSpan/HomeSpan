/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
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

#include "StepperControl.h"

StepperControl *bigMotor;
StepperControl *smallMotor;

#define BIG_MOTOR_POSITION    1600
#define SMALL_MOTOR_POSITION  2064

///////////////////

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.printf("\nHomeSpan Stepper Control\n\n");

  bigMotor=(new Stepper_TB6612(23,32,22,14,33,27))->setStepType(StepperControl::HALF_STEP)->setAccel(10,20);
  smallMotor=new Stepper_A3967(18,21,5,4,19);

//  smallMotor->setStepType(StepperControl::EIGHTH_STEP);

//  bigMotor->setStepType(StepperControl::HALF_STEP);
//  bigMotor->setAccel(10,20);
}

///////////////////

void loop(){

  if(smallMotor->position()==0)
    smallMotor->moveTo(SMALL_MOTOR_POSITION,2);
  else if(smallMotor->position()==SMALL_MOTOR_POSITION)
    smallMotor->moveTo(0,2);

  if(bigMotor->position()==0)
    bigMotor->moveTo(BIG_MOTOR_POSITION,4);
  else if(bigMotor->position()==BIG_MOTOR_POSITION)
    bigMotor->moveTo(0,4);    

  delay(1000);
  Serial.printf("Small Motor: %d     Big Motor %d\n",smallMotor->stepsRemaining(),bigMotor->stepsRemaining());
}
