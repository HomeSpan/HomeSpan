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

#include <Arduino.h>
#include "PwmPin.h"

[[maybe_unused]] static const char* STEPPER_TAG = "StepperControl";

//////////////////////////

class StepperControl {

  public:

  enum {
    FULL_STEP_ONE_PHASE=0,
    FULL_STEP_TWO_PHASE=1,
    HALF_STEP=2,
    QUARTER_STEP=4,
    EIGHTH_STEP=8
  };    
  
  enum endAction_t {
    NONE,
    DISABLE,
    BRAKE
  };

  enum action_t {
    MOVE,
    MOVETO,
    SET_POSITION
  };

  private:
  
  struct upLink_t {
    int nSteps;
    uint32_t msDelay;
    action_t action;
    endAction_t endAction;
  };

  struct downLink_t {
    int stepsRemaining=0;
    int position=0;
    boolean ack=false;
  };
  
  float accelSteps=1;
  float accelSize=0;
  downLink_t downLinkData;

  TaskHandle_t motorTaskHandle;
  QueueHandle_t upLinkQueue;
  QueueHandle_t downLinkQueue;

  void waitForAck();
  virtual void onStep(boolean direction)=0;
  virtual void onEnable(){};
  virtual void onDisable(){};
  virtual void onBrake(){};
  static void motorTask(void *args);

  public:

  StepperControl(uint32_t priority=1, uint32_t cpu=0);
  virtual StepperControl *setStepType(int mode){return(this);};
  StepperControl *setAccel(float accelSize, float accelSteps);
  void move(int nSteps, uint32_t msDelay, endAction_t endAction=NONE);
  void moveTo(int nPosition, uint32_t msDelay, endAction_t endAction=NONE);
  int position();
  void setPosition(int pos);
  int stepsRemaining();
  StepperControl *enable();
  StepperControl *disable();
  StepperControl *brake();
};

//////////////////////////

#include "Stepper_UNIPOLAR.h"
#include "Stepper_TB6612.h"     // https://www.adafruit.com/product/2448
#include "Stepper_A3967.h"      // https://www.sparkfun.com/products/12779


struct Stepper_ULN2003A : Stepper_UNIPOLAR {
  Stepper_ULN2003A(int IN1, int IN2, int IN3, int IN4, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : Stepper_UNIPOLAR(IN1,IN3,IN2,IN4,taskParams){}
};
