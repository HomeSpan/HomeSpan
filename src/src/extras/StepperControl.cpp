/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeStep
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

 #include "StepperControl.h"
 
//////////////////////////

StepperControl::StepperControl(uint32_t priority, uint32_t cpu){

  upLinkQueue = xQueueCreate(1,sizeof(upLink_t));  
  downLinkQueue = xQueueCreate(1,sizeof(downLink_t));  
  xTaskCreateUniversal(motorTask, "motorTaskHandle", 8096, this, priority, &motorTaskHandle, cpu);
}

//////////////////////////

void StepperControl::setAccel(float accelSize, float accelSteps){
  if(accelSize<0.0){
    ESP_LOGE(TAG,"accelSize cannot be less than 0.0");
    return;
  }
  
  if(accelSteps<1.0){
    ESP_LOGE(TAG,"accelSteps cannot be less than 1.0");
    return;
  }

  this->accelSize=accelSize;
  this->accelSteps=accelSteps;
}

//////////////////////////

void StepperControl::move(int nSteps, uint32_t msDelay, endAction_t endAction){
  if(msDelay==0){
    ESP_LOGE(TAG,"msDelay must be greater than zero");
    return;
  }
  
  upLink_t upLinkData = { .nSteps=nSteps, .msDelay=msDelay, .absoluteStep=false, endAction=endAction };
  xQueueReceive(downLinkQueue,&downLinkData,0);
  downLinkData.stepsRemaining=nSteps;
  xQueueOverwrite(upLinkQueue,&upLinkData);
}

//////////////////////////

void StepperControl::moveTo(int nPosition, uint32_t msDelay, endAction_t endAction){
  if(msDelay==0){
    ESP_LOGE(TAG,"msDelay must be greater than zero");
    return;
  }
  
  upLink_t upLinkData = { .nSteps=nPosition, .msDelay=msDelay, .absoluteStep=true, .endAction=endAction };
  xQueueReceive(downLinkQueue,&downLinkData,0);
  downLinkData.stepsRemaining=nPosition-downLinkData.position;
  xQueueOverwrite(upLinkQueue,&upLinkData);
}

//////////////////////////

int StepperControl::stepsRemaining(){
  xQueuePeek(downLinkQueue,&downLinkData,0);
  return(downLinkData.stepsRemaining);
}

//////////////////////////

int StepperControl::position(){
  xQueuePeek(downLinkQueue,&downLinkData,0);
  return(downLinkData.position);
}

//////////////////////////

void StepperControl::brake(){
  move(0,10,BRAKE);
  while(stepsRemaining());
}

//////////////////////////

void StepperControl::disable(){
  move(0,10,DISABLE);
  while(stepsRemaining());
}

//////////////////////////

void StepperControl::enable(){
  move(0,10);
  while(stepsRemaining());
  
}//////////////////////////

void StepperControl::motorTask(void *args){
  StepperControl *motor = (StepperControl *)args;
  upLink_t upLinkData = { .nSteps=0, .msDelay=10, .absoluteStep=false, .endAction=NONE };
  downLink_t downLinkData = { .stepsRemaining=0, .position=0 };  
  boolean running=false;

  for(;;){
    
    if(xQueueReceive(motor->upLinkQueue, &upLinkData, 0)){
      if(upLinkData.absoluteStep)
        upLinkData.nSteps-=downLinkData.position;
      downLinkData.stepsRemaining=upLinkData.nSteps;
      motor->onEnable();
      running=true;
    }

    uint32_t msDelay=upLinkData.msDelay;

    if(running==false){
      vTaskDelay(msDelay);
      continue;
    }
    
    if(downLinkData.stepsRemaining!=0)
      msDelay+=msDelay * motor->accelSize * (exp(-fabs(upLinkData.nSteps-downLinkData.stepsRemaining)/motor->accelSteps) + exp(-(fabs(downLinkData.stepsRemaining)-1.0)/motor->accelSteps));
      
    ESP_LOGD(TAG,"Position: %d   Steps Remaining: %d   Delay=%d ms",downLinkData.position,downLinkData.stepsRemaining,(int)(msDelay));

    int dStep=0;
        
    if(downLinkData.stepsRemaining>0){
      dStep=-1;
      motor->onStep(1);      
    } else if(downLinkData.stepsRemaining<0){
      dStep=1;
      motor->onStep(0);
    } else {
      if(upLinkData.endAction==DISABLE)
        motor->onDisable();
      else if(upLinkData.endAction==BRAKE)
        motor->onBrake();
      running=false;
    }

    xQueueOverwrite(motor->downLinkQueue,&downLinkData);
    downLinkData.stepsRemaining+=dStep;
    downLinkData.position-=dStep;
    vTaskDelay(msDelay);  
  }
}

//////////////////////////
