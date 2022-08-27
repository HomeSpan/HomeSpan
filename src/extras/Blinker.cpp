/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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

Blinker::Blinker(Blinkable *led, int timerNum, uint16_t autoOffDuration){

  this->led=led;

  pauseDuration=autoOffDuration*1000;

#if SOC_TIMER_GROUP_TIMERS_PER_GROUP>1                        // ESP32 and ESP32-S2 contains two timers per timer group
  group=((timerNum/2)%2==0)?TIMER_GROUP_0:TIMER_GROUP_1;
  idx=(timerNum%2==0)?TIMER_0:TIMER_1;                        // ESP32-C3 only contains one timer per timer group
#else
  group=(timerNum%2==0)?TIMER_GROUP_0:TIMER_GROUP_1;
  idx=TIMER_0;
#endif

  timer_config_t conf;
  conf.alarm_en=TIMER_ALARM_EN;
  conf.counter_en=TIMER_PAUSE;
  conf.intr_type=TIMER_INTR_LEVEL;
  conf.counter_dir=TIMER_COUNT_UP;
  conf.auto_reload=TIMER_AUTORELOAD_EN;
  conf.divider=getApbFrequency()/10000;                      // set divider to yield 10 kHz clock (0.1 ms pulses)

#ifdef SOC_TIMER_GROUP_SUPPORT_XTAL                          // set clock to APB (default is XTAL!) if clk_src is defined in conf structure
  conf.clk_src=TIMER_SRC_CLK_APB;
#endif

  timer_init(group,idx,&conf);
  timer_isr_register(group,idx,Blinker::isrTimer,(void *)this,0,NULL);
  timer_enable_intr(group,idx);

}

//////////////////////////////////////

void Blinker::isrTimer(void *arg){

  Blinker *b=(Blinker *)arg;

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)               // use new method that is generic to ESP32, S2, and C3
  timer_group_clr_intr_status_in_isr(b->group,b->idx);
#else                                                             // use older method that is only for ESP32
  if(b->group){
    if(b->idx)
      TIMERG1.int_clr_timers.t1=1;
    else
      TIMERG1.int_clr_timers.t0=1;
  } else {
    if(b->idx)
      TIMERG0.int_clr_timers.t1=1;
    else
      TIMERG0.int_clr_timers.t0=1;    
  }
#endif
  
  if(!(b->led->isOn())){
    b->led->on();
    timer_set_alarm_value(b->group,b->idx,b->onTime);
    b->count--;
  } else {
    b->led->off();    
    if(b->count){
      timer_set_alarm_value(b->group,b->idx,b->offTime);
    } else {
      timer_set_alarm_value(b->group,b->idx,b->delayTime);
      b->count=b->nBlinks;      
    }
  }
  
  timer_set_alarm(b->group,b->idx,TIMER_ALARM_EN);

}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle){

  start(period, dutyCycle, 1, 0);
}

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle, int nBlinks, int delayTime){

  pauseTime=millis();
  isPaused=false;
  
  period*=10;
  onTime=dutyCycle*period;
  offTime=period-onTime;
  this->delayTime=delayTime*10+offTime;
  this->nBlinks=nBlinks;
  count=nBlinks;
  timer_set_counter_value(group,idx,0);
  timer_set_alarm_value(group,idx,0);  
  timer_start(group,idx);
}

//////////////////////////////////////

void Blinker::stop(){
  
  timer_pause(group,idx);  
}

//////////////////////////////////////

void Blinker::on(){

  pauseTime=millis();
  isPaused=false;

  stop();
  led->on();
}

//////////////////////////////////////

void Blinker::off(){

  pauseTime=millis();
  isPaused=false;

  stop();
  led->off();
}

//////////////////////////////////////

void Blinker::check(){

  if(pauseDuration==0 || isPaused || (millis()-pauseTime)<pauseDuration)
    return;

  Serial.print("Pausing Status LED\n");
  isPaused=true;
  led->off();
}

//////////////////////////////////////

int Blinker::getPin(){
  return(led->getPin());
}
