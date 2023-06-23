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

#include "Stepper_TB6612.h"
#include "Stepper_A3967.h"

StepperControl *motor;

////////////////////////////////////

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.printf("\nHomeSpan Steppers\n\n");


//  motor=new Stepper_TB6612(23,32,22,14,33,27,{1,1});
  motor=new Stepper_TB6612(23,32,22,14);

  motor->setStepType(StepperControl::FULL_STEP_ONE_PHASE);
  motor->setAccel(10,5);

  motor->enable();
  while(1);
  
  for(int i=0;i<100;i++){
    motor->disable();
    delay(10);
    motor->enable();
    delay(10);
  }
  Serial.printf("Done\n");
  while(1);
  
  delay(1000);
  for(int i=0;i<10;i++){
    motor->move(1,100);
    while(motor->stepsRemaining());
    motor->move(-0,5);
    while(motor->stepsRemaining());
  }
  while(1);
    
  delay(100);
  motor->move(-400,5,StepperControl::BRAKE);
  while(1);
  motor->setPosition(800);
  motor->moveTo(0,5,StepperControl::BRAKE);
}

//////////////////////////////////////

void loop(){
}

//////////////////////////////////////


//////////////////////////
