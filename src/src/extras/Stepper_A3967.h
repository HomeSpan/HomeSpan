/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023-2024 Gregg E. Berman
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

// Implementation of StepperControl for a Sparkfun A3967 EasyDriver Stepper Motor Driver
// Breakout Board (https://www.sparkfun.com/products/12779)

// This implementation uses the driver board's MS1, MS2, STEP, DIR, and ENABLE pins.
// Separate PWM pins on the ESP32 are NOT needed since the driver board contains its own
// waveform generation.  Supported modes include FULL_STEP (double-phase only), HALF_STEP,
// QUARTER_STEP, and EIGHTH_STEP.

// The motor outputs can be enabled (current running through the coils) or
// disabled (no current / high impedence).  The EasyDriver board does NOT support
// the short brake mode.

//////////////////////////
 
struct Stepper_A3967 : StepperControl {

  int m1Pin;
  int m2Pin;
  int stepPin;
  int dirPin;
  int enablePin;

//////////////////////////

  Stepper_A3967(int m1Pin, int m2Pin, int stepPin, int dirPin, int enablePin, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : StepperControl(taskParams.first,taskParams.second){
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

  void onStep(boolean direction) override {
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

  StepperControl *setStepType(int mode) override {
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
        ESP_LOGE(STEPPER_TAG,"Unknown StepType=%d",mode);
    }
    return(this);
  }
  
};

//////////////////////////
