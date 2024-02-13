/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2024 Gregg E. Berman
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

//  Implementation of a basic 4-wire controller for a center-tapped Unipolar Stepper Motor
//  with two coils (Coil 1 and Coil 2) each having two driving inputs (A and B).
//
//  Requires 4 pins on the ESP32 connected to an appropriate driver circuit, such as a set
//  of Darlington transitors, that are in turn connected to each of the 4 Stepper Motor wires:
//
//  * Coil 1, Input A  (1A)
//  * Coil 1, Input B  (1B)
//  * Coil 2, Input A  (2A)
//  * Coil 2, Input B  (2B)
//
//  When any of the pins are HIGH, the driver circuit should cause current to flow in the corresponding
//  half of the coil. When the pin is set LOW, the driver circuit should stop the flow of current through
//  that half of the coil.  Supported modes and driving logic are as follows:
//
//  FULL_STEP_ONE_PHASE (4 steps, where current flows only through ONE of the phases of ONE of the coils at each step):
//
//             1A    2A    1B    2B
//  Step 1:    HI    --    --    --
//  Step 2:    --    HI    --    --
//  Step 3:    --    --    HI    --
//  Step 4:    --    --    --    HI
//
//  FULL_STEP_TWO_PHASE (4 steps, where current flows through ONE of the phases of EACH of the coils at each step):
//
//              1A    2A    1B    2B
//  Step 1:    HI    HI    --    --
//  Step 2:    --    HI    HI    --
//  Step 3:    --    --    HI    HI
//  Step 4:    HI    --    --    HI
//
//  HALF_STEP (8 steps that alternate between the 4 steps of the FULL_STEP modes above):
//
//             1A    2A    1B    2B
//  Step 1:    HI    --    --    --
//  Step 2:    HI    HI    --    --
//  Step 3:    --    HI    --    --
//  Step 4:    --    HI    HI    --
//  Step 5:    --    --    HI    --
//  Step 6:    --    --    HI    HI
//  Step 7:    --    --    --    HI
//  Step 8:    HI    --    --    HI

// NOTE ORDER OF CONSTRUCTOR PARAMETERS:  First the two pins that drive the A and B side of Coil 1,
// followed by the two pints that drive the A and B side of Coil 2.

// It does not matter which coil is defined as 1 or 2, nor which side is called A or B, as long as 
// the first two parameters are for one of the coils and the second two are for the other coil.

// Note: This driver supports enabling and disabling all current flow, but does NOT support a short brake.

//////////////////////////

struct Stepper_UNIPOLAR : StepperControl {

  int c1A, c1B, c2A, c2B;
  uint8_t phase, nPhases;
  double offset;

//////////////////////////

  Stepper_UNIPOLAR(int coil1A, int coil1B, int coil2A, int coil2B, std::pair<uint32_t, uint32_t> taskParams = {1,0}) : StepperControl(taskParams.first,taskParams.second) {

    c1A=coil1A;
    c1B=coil1B;
    c2A=coil2A;
    c2B=coil2B;

    pinMode(c1A,OUTPUT);
    pinMode(c1B,OUTPUT);
    pinMode(c2A,OUTPUT);
    pinMode(c2B,OUTPUT);
    
    setStepType(FULL_STEP_ONE_PHASE);
  }
  
//////////////////////////

  void onEnable() override {
    setPins();
  }

//////////////////////////

  void onDisable() override {
    digitalWrite(c1A,0);
    digitalWrite(c1B,0);
    digitalWrite(c2A,0);
    digitalWrite(c2B,0);
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
    digitalWrite(c1A,levelA>0.01);
    digitalWrite(c1B,levelA<-0.01);
    digitalWrite(c2A,levelB>0.01);
    digitalWrite(c2B,levelB<-0.01);
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
      default:
        ESP_LOGE(STEPPER_TAG,"Unknown StepType=%d",mode);
    }
    return(this);
  }
  
};
