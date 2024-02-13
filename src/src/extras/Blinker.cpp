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

#include "Blinker.h"

 ////////////////////////////////
//         Blinker            //
////////////////////////////////

Blinker::Blinker(Blinkable *led, uint16_t autoOffDuration){
  this->led=led;
  pauseDuration=autoOffDuration*1000;
}

//////////////////////////////////////

void Blinker::blinkTask(void *arg){

  Blinker *b=(Blinker *)arg;
  
  for(;;){
    for(int i=0;i<b->nBlinks;i++){
      b->led->on();
      delay(b->onTime);
      b->led->off();
      delay(b->offTime);
    }
    delay(b->delayTime);
  }
  
}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle){

  start(period, dutyCycle, 1, 0);
}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle, int nBlinks, int delayTime){

  if(!led)
    return;
    
  onTime=dutyCycle*period;
  offTime=period-onTime;
  this->delayTime=delayTime+offTime;
  this->nBlinks=nBlinks;

  stop();  
  xTaskCreate( blinkTask, "BlinkTask", 1024, (void *)this, 2, &blinkHandle );

  pauseTime=millis();
  isPaused=false;
  status=STATUS::BLINKING;
}

//////////////////////////////////////

void Blinker::stop(){

  if(!led)
    return;  
  
  if(blinkHandle!=NULL){
    vTaskDelete(blinkHandle);
    blinkHandle=NULL;
  }    

  isPaused=true;  
  status=STATUS::OFF;
}

//////////////////////////////////////

void Blinker::on(){

  if(!led)
    return;

  stop();
  led->on();
  
  pauseTime=millis();
  isPaused=false;
  status=STATUS::ON;
}

//////////////////////////////////////

void Blinker::off(){

  if(!led)
    return;

  stop();
  led->off();
  status=STATUS::OFF;
}

//////////////////////////////////////

void Blinker::check(){

  if(!led)
    return;

  if(pauseDuration==0 || isPaused || (millis()-pauseTime)<pauseDuration)
    return;

  ESP_LOGI(BLINKER_TAG,"Pausing LED");
  off();
}

//////////////////////////////////////

int Blinker::getPin(){
  
  if(!led)
    return(-1);
    
  return(led->getPin());
}
