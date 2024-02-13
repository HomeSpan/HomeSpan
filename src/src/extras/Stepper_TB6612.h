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

// Implementation of StepperControl for an Adafruit TB6612 1.2A DC/Stepper Motor Driver
// Breakout Board (https://www.adafruit.com/product/2448)

// This implementation supports two constructors reflecting implementations both with and
// without the use of PWM pins.  The first operates the driver board using only its 
// four digital control pins: AIN1, AIN2, BIN1, BIN2.  In this configuration the
// PWMA, PWMB, and STBY pins on the driver board should be directly connected to Vcc.

// The second configuration uses the four digital control pins (AIN1, AIN2, BIN1, and BIN2)
// as well as the PWMA and PWMB pins.  In this configuration only the STBY pin on the
// driver board should be directly connected to Vcc.

// The first configuration supports both single-phase and double-phase FULL_STEP modes,
// as well as a HALF_STEP mode. The second configuration also includes QUARTER_STEP
// and EIGHTH_STEP modes, made possible by the use of the PWM pins to micro-step the motor.

// In either configuration the motor outputs can be enabled (current running through the coils)
// disabled (no current / high impedence) or set to a short brake.

//////////////////////////

struct Stepper_TB6612 : StepperControl {

  int ain1, ain2, bin1, bin2;
  uint8_t phase, nPhases;
  double offset;
  LedPin *pwmA=NULL, *pwmB;

//////////////////////////

  Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : StepperControl(taskParams.first,taskParams.second){

    ain1=AIN1;
    ain2=AIN2;
    bin1=BIN1;
    bin2=BIN2;

    pinMode(ain1,OUTPUT);
    pinMode(ain2,OUTPUT);
    pinMode(bin1,OUTPUT);
    pinMode(bin2,OUTPUT);
    
    setStepType(FULL_STEP_TWO_PHASE);
  }

//////////////////////////

  Stepper_TB6612(int AIN1, int AIN2, int BIN1, int BIN2, int PWMA, int PWMB, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : Stepper_TB6612(AIN1,AIN2,BIN1,BIN2,taskParams){

    pwmA=new LedPin(PWMA,0,50000);
    pwmB=new LedPin(PWMB,0,50000);
  }
  
//////////////////////////

  void onEnable() override {
    setPins();
  }

//////////////////////////

  void onDisable() override {
    digitalWrite(ain1,0);
    digitalWrite(ain2,0);
    digitalWrite(bin1,0);
    digitalWrite(bin2,0);
  }

//////////////////////////

  void onBrake() override {
    digitalWrite(ain1,1);
    digitalWrite(ain2,1);
    digitalWrite(bin1,1);
    digitalWrite(bin2,1);
  }

//////////////////////////

  void onStep(boolean direction) override {
    if(direction)
      phase=(phase+1)%nPhases;
    else
      phase=(phase+nPhases-1)%nPhases;
      
    setPins();    
  }

//////////////////////////

  void setPins(){
    float levelA=cos(phase*TWO_PI/nPhases+offset)*100.0;
    float levelB=sin(phase*TWO_PI/nPhases+offset)*100.0;
    digitalWrite(ain1,levelA>0.01);
    digitalWrite(ain2,levelA<-0.01);
    digitalWrite(bin1,levelB>0.01);
    digitalWrite(bin2,levelB<-0.01);
    if(pwmA){
      pwmA->set(fabs(levelA));
      pwmB->set(fabs(levelB));
    }
  }

//////////////////////////

  StepperControl *setStepType(int mode) override {

    switch(mode){
      case FULL_STEP_ONE_PHASE:
        phase=0;
        nPhases=4;
        offset=0;
        break;
      case FULL_STEP_TWO_PHASE:
        phase=0;
        nPhases=4;
        offset=TWO_PI/8.0;
        break;
      case HALF_STEP:
        phase=0;
        nPhases=8;
        offset=0;
        break;
      case QUARTER_STEP:
        if(!pwmA){
          ESP_LOGE(STEPPER_TAG,"QUARTER_STEP requires PWM pins");
          return(this);
        }
        phase=0;
        nPhases=16;
        offset=0;
        break;
      case EIGHTH_STEP:
        if(!pwmA){
          ESP_LOGE(STEPPER_TAG,"EIGHTH_STEP requires PWM pins");
          return(this);
        }
        phase=0;
        nPhases=32;
        offset=0;
        break;
      default:
        ESP_LOGE(STEPPER_TAG,"Unknown StepType=%d",mode);
    }
    return(this);
  }
  
};

//////////////////////////
