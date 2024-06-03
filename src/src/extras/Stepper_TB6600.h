/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2024 Jeremy Laurenson
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

#pragma once

// Implementation of StepperControl for a TB6600 stepper driver
// https://www.dfrobot.com/product-1547.html

// This implementation uses direction and pulse pins and optinally enable pins only to save pinouts

//////////////////////////
 
struct Stepper_TB6600 : StepperControl {

  int stepPin;
  int dirPin;
  int enablePin;

//////////////////////////

  Stepper_TB6600(int stepPin, int dirPin, int enablePin, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : StepperControl(taskParams.first,taskParams.second){
    this->stepPin=stepPin;
    this->dirPin=dirPin;
    this->enablePin=enablePin;

    pinMode(stepPin,OUTPUT);
    pinMode(dirPin,OUTPUT);
    if(enablePin>0)pinMode(enablePin,OUTPUT);

    setStepType(EIGHTH_STEP);
  }

//////////////////////////

  void onStep(boolean direction) override {
    digitalWrite(dirPin,direction);
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(10); // Wait for controller to see this. Min is 2.2uS
    digitalWrite(stepPin,LOW);      
  }

//////////////////////////

  void onEnable() override {
    if(enablePin>0)digitalWrite(enablePin,0);
  }

//////////////////////////

  void onDisable() override {
    if(enablePin>0)digitalWrite(enablePin,1);
  }

//////////////////////////

  StepperControl *setStepType(int mode) override {
    switch(mode){
      case FULL_STEP_TWO_PHASE:
        break;
      default:
        ESP_LOGE(STEPPER_TAG,"Unknown StepType=%d",mode);
    }
    return(this);
  }
  
};

//////////////////////////
