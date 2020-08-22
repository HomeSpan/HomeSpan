
#include <Arduino.h>
#include "Blinker.h"

////////////////////////////////
//         Blinker            //
////////////////////////////////

Blinker::Blinker(int pin, int timerNum){
  this->pin=pin;
  pinMode(pin,OUTPUT);
  digitalWrite(pin,0);

  group=((timerNum/2)%2==0)?TIMER_GROUP_0:TIMER_GROUP_1;
  idx=(timerNum%2==0)?TIMER_0:TIMER_1;  

  timer_config_t conf;
  conf.alarm_en=TIMER_ALARM_EN;
  conf.counter_en=TIMER_PAUSE;
  conf.intr_type=TIMER_INTR_LEVEL;
  conf.counter_dir=TIMER_COUNT_UP;
  conf.auto_reload=TIMER_AUTORELOAD_EN;
  conf.divider=8000;                      // 80 MHz clock / 8,000 = 10 kHz clock (0.1 ms pulses)

  timer_init(group,idx,&conf);
  timer_isr_register(group,idx,Blinker::isrTimer,(void *)this,0,NULL);
  timer_enable_intr(group,idx);

}

void Blinker::isrTimer(void *arg){

  Blinker *b=(Blinker *)arg;
  
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

  if(!digitalRead(b->pin)){
    digitalWrite(b->pin,1);
    timer_set_alarm_value(b->group,b->idx,b->onTime);
    b->count--;
  } else {
    digitalWrite(b->pin,0);    
    if(b->count){
      timer_set_alarm_value(b->group,b->idx,b->offTime);
    } else {
      timer_set_alarm_value(b->group,b->idx,b->delayTime);
      b->count=b->nBlinks;      
    }
  }
  
  timer_set_alarm(b->group,b->idx,TIMER_ALARM_EN);
}

void Blinker::start(int period, float dutyCycle){

  start(period, dutyCycle, 1, period-dutyCycle*period);
}

void Blinker::start(int period, float dutyCycle, int nBlinks, int delayTime){

  period*=10;
  onTime=dutyCycle*period;
  offTime=period-onTime;
  this->delayTime=delayTime*10;
  this->nBlinks=nBlinks;
  count=nBlinks;
  timer_set_counter_value(group,idx,0);
  timer_set_alarm_value(group,idx,0);  
  timer_start(group,idx);
}

void Blinker::stop(){
  timer_pause(group,idx);  
}
