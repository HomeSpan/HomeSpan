/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023 Gregg E. Berman
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
 
 #include "extras/StepperControl.h"

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
