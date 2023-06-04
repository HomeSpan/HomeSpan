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

#include "StepperControl.h"

//////////////////////////

struct Stepper_TB6612 : StepperControl {

  int pins[4];
  uint8_t phase;
  uint8_t nPhases;
  uint32_t runCode;

//////////////////////////

  Stepper_TB6612(int a1Pin, int a2Pin, int b1Pin, int b2Pin) : StepperControl(){    
    pins[3]=a1Pin;
    pins[2]=a2Pin;
    pins[1]=b1Pin;
    pins[0]=b2Pin;

    for(int i=0;i<4;i++)
      pinMode(pins[i],OUTPUT);

    setStepType(FULL_STEP_TWO_PHASE);
  }

//////////////////////////

  void onEnable() override {
    setPinCode((runCode>>(phase*4)) & 0xF);
  }

//////////////////////////

  void onDisable() override {
    setPinCode(0);
  }

//////////////////////////

  void onBrake() override {
    setPinCode(0xF);
  }

//////////////////////////

  void onStep(boolean direction){
    if(direction)
      phase=(phase+1)%nPhases;
    else
      phase=(phase+nPhases-1)%nPhases;
      
    setPinCode((runCode>>(phase*4)) & 0xF);    
  }

//////////////////////////

  void setPinCode(uint8_t pinCode){
    for(int i=0;i<4;i++)
      digitalWrite(pins[i],(pinCode>>i)&1);    
  }

//////////////////////////

  void setStepType(int mode) override {
    switch(mode){
      case FULL_STEP_ONE_PHASE:
        phase=0;
        nPhases=4;
        runCode=0x2418;
        break;
      case FULL_STEP_TWO_PHASE:
        phase=0;
        nPhases=4;
        runCode=0x659A;
        break;
      case HALF_STEP:
        phase=0;
        nPhases=8;
        runCode=0x2645198A;
        break;
      default:
        ESP_LOGE(TAG,"Unknown StepType=%d.  Stepper Unchanged",mode);
    }
  }
  
};

//////////////////////////

 struct Stepper_A3967 : StepperControl {

  int m1Pin;
  int m2Pin;
  int stepPin;
  int dirPin;
  int enablePin;

//////////////////////////

  Stepper_A3967(int m1Pin, int m2Pin, int stepPin, int dirPin, int enablePin) : StepperControl(){
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

  void onStep(boolean direction){
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

  void setStepType(int mode) override {
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
        ESP_LOGE(TAG,"Unknown StepType=%d. Stepper Unchanged",mode);
    }
  }
  
};

//////////////////////////

StepperControl *motor[2];

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.printf("\nReady.\n\n");

  pinMode(33,OUTPUT);
  pinMode(27,OUTPUT);
  digitalWrite(33,HIGH);
  digitalWrite(27,HIGH);

  motor[0]=new Stepper_A3967(16,17,21,19,18);
  motor[1]=new Stepper_TB6612(23,32,22,14);

  motor[0]->setStepType(Stepper_A3967::QUARTER_STEP);
  motor[1]->setStepType(Stepper_TB6612::HALF_STEP);

  motor[0]->setAccel(10,20);
  motor[1]->setAccel(50,100);
  
}

void loop() {
  motor[0]->move(1000,5);
  motor[1]->move(2000,2);

  do {
    for(int i=0;i<2;i++)
      Serial.printf("Motor %d - %5d %5d     ",i,motor[i]->stepsRemaining(),motor[i]->position());
    Serial.printf("\n");
    delay(500);
  } while(motor[0]->stepsRemaining() || motor[1]->stepsRemaining());

  motor[0]->moveTo(0,5);
  motor[1]->moveTo(0,2); 

  do {
    for(int i=0;i<2;i++)
      Serial.printf("Motor %d - %5d %5d     ",i,motor[i]->stepsRemaining(),motor[i]->position());
    Serial.printf("\n");
    delay(500);
  } while(motor[0]->stepsRemaining() || motor[1]->stepsRemaining());

}

//////////////////////////
