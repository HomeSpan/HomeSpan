
#include "Utils.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Contains various generic utility functions and classes:
//
//  Utils::readSerial       - reads all characters from Serial port and saves only up to max specified
//  Utils::mask             - masks a string with asterisks (good for displaying passwords)
//
//  class PushButton        - tracks Long and Short presses of a pushbutton that connects a specified pin to ground
//  class Blinker           - creates customized blinking patterns on an LED connected to a specified pin
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char *Utils::readSerial(char *c, int max){
  int i=0;
  char buf;

  while(1){

    while(!Serial.available());       // wait until there is a new character
    
    buf=Serial.read();
    
    if(buf=='\n'){         // exit upon newline
      if(i>0)              // characters have been typed
        c[i]='\0';            // replace newline with string terminator
      return(c);           // return updated string
    }

    c[i]=buf;               // store new character
    
    if(i<max)               // do not store more than max characters (excluding string terminator)
      i++;
  
  } // while(1)
  
} // readSerial

//////////////////////////////////////

String Utils::mask(char *c, int n){
  String s="";
  int len=strlen(c);
  
  for(int i=0;i<len;i++){
    if(i<n || i>=len-n)
      s+=c[i];
    else
      s+='*';
  }
  
  return(s);  
} // mask

////////////////////////////////
//         PushButton         //
////////////////////////////////

PushButton::PushButton(){}

//////////////////////////////////////

PushButton::PushButton(uint8_t pin){
  init(pin);
}

//////////////////////////////////////

void PushButton::init(uint8_t pin){
  status=0;
  this->pin=pin;
  pinMode(pin, INPUT_PULLUP);
}

//////////////////////////////////////

boolean PushButton::triggered(uint16_t shortTime, uint16_t longTime){

  switch(status){
    
    case 0:
      if(!digitalRead(pin)){         // button is pressed
        status=1;
        shortAlarm=millis()+shortTime;
        longAlarm=millis()+longTime;
      }
    break;

    case 1:
    case 2:
      if(digitalRead(pin)){         // button is released          
        status=0;
        if(millis()>shortAlarm){
          isLongPress=false;
          return(true);
        }
      } else
      
      if(millis()>longAlarm){       // button is long-pressed
        longAlarm=millis()+longTime;
        status=3;
        isLongPress=true;
        return(true);
      }
    break;

    case 3:
      if(digitalRead(pin))          // button has been released after a long press
        status=0;
      else if(millis()>longAlarm){
        longAlarm=millis()+longTime;
        isLongPress=true;
        return(true);        
      }
    break;

  }

  return(false);
}

//////////////////////////////////////

boolean PushButton::primed(){
  
  if(millis()>shortAlarm && status==1){
    status=2;
    return(true);
  }
  
  return(false);
}

//////////////////////////////////////

boolean PushButton::longPress(){
  return(isLongPress);
}

//////////////////////////////////////

void PushButton::reset(){
  status=0;
}

////////////////////////////////
//         Blinker            //
////////////////////////////////

Blinker::Blinker(){
}

//////////////////////////////////////

Blinker::Blinker(int pin, int timerNum){
  init(pin, timerNum);
}

//////////////////////////////////////

void Blinker::init(int pin, int timerNum){
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

//////////////////////////////////////

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

//////////////////////////////////////

void Blinker::start(int period, float dutyCycle){

  start(period, dutyCycle, 1, period-dutyCycle*period);
}

//////////////////////////////////////

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

//////////////////////////////////////

void Blinker::stop(){
  timer_pause(group,idx);  
}

//////////////////////////////////////

void Blinker::on(){
  stop();
  digitalWrite(pin,1);
}

//////////////////////////////////////

void Blinker::off(){
  stop();
  digitalWrite(pin,0);
}
