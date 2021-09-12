
#include "PwmPin.h"

///////////////////

LedPin::LedPin(uint8_t pin, uint8_t level, uint16_t freq){

  if(freq==0)
    freq=DEFAULT_PWM_FREQ;

  for(int nMode=0;nMode<LEDC_SPEED_MODE_MAX;nMode++){
    for(int nChannel=0;nChannel<LEDC_CHANNEL_MAX;nChannel++){
      for(int nTimer=0;nTimer<LEDC_TIMER_MAX;nTimer++){
        if(!channelList[nChannel][nMode]){
          
          if(!timerList[nTimer][nMode]){                                // if this timer slot is free, use it
            timerList[nTimer][nMode]=new ledc_timer_config_t;           // create new timer instance
            timerList[nTimer][nMode]->speed_mode=(ledc_mode_t)nMode;
            timerList[nTimer][nMode]->timer_num=(ledc_timer_t)nTimer;
            timerList[nTimer][nMode]->freq_hz=freq;
          
            int res=20;                                                 // find the maximum possible resolution
            while(getApbFrequency()/(freq*pow(2,res))<1)
              res--;      
              
            timerList[nTimer][nMode]->duty_resolution=(ledc_timer_bit_t)res;
            ledc_timer_config(timerList[nTimer][nMode]);
          }
          
          if(timerList[nTimer][nMode]->freq_hz==freq){                      // if timer matches desired frequency (always true if newly-created above)
            channelList[nChannel][nMode]=new ledc_channel_config_t;         // create new channel instance
            channelList[nChannel][nMode]->speed_mode=(ledc_mode_t)nMode;
            channelList[nChannel][nMode]->channel=(ledc_channel_t)nChannel;
            channelList[nChannel][nMode]->timer_sel=(ledc_timer_t)nTimer;
            channelList[nChannel][nMode]->intr_type=LEDC_INTR_DISABLE;
            channelList[nChannel][nMode]->hpoint=0;
            channelList[nChannel][nMode]->gpio_num=pin;
            ledTimer=timerList[nTimer][nMode];
            ledChannel=channelList[nChannel][nMode];
            set(level);
            return;
          }
          
        }
      }
    }
  }

  Serial.printf("\n*** ERROR:  Can't create LedPin(%d) - no open PWM channels and/or Timers ***\n\n",pin);
    
}

///////////////////

void LedPin::set(uint8_t level){

  if(!ledChannel)
    return;

  Serial.printf("pin=%d, ch=%d, mode=%d, timer=%d, freq=%d, res=%d\n",
    ledChannel->gpio_num,
    ledChannel->channel,
    ledChannel->speed_mode,
    ledChannel->timer_sel,
    ledTimer->freq_hz,
    ledTimer->duty_resolution
    );

  if(level>100)
    level=100;
    
  ledChannel->duty=level*(pow(2,ledTimer->duty_resolution)-1);
  ledChannel->duty/=100;
  ledc_channel_config(ledChannel);
  
}

///////////////////

void LedPin::HSVtoRGB(float h, float s, float v, float *r, float *g, float *b ){

  // The algorithm below was provided on the web at https://www.cs.rit.edu/~ncs/color/t_convert.html
  // h = [0,360]
  // s = [0,1]
  // v = [0,1]

  int i;
  float f, p, q, t;
  
  if( s == 0 ){
    *r = *g = *b = v;
    return;
  }
  
  h /= 60;
  i = floor( h ) ;
  f = h - i;
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );
  switch( i % 6 ) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    case 5:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}

////////////////////////////

ServoPin::ServoPin(uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees){
  if(numChannels>7 || numChannels>(15-LedPin::numChannels)){
    Serial.printf("\n*** ERROR:  Can't create ServoPin(%d) - no open PWM channels ***\n\n",pin);
    servoChannel.gpio_num=0;
    return;
  }

  enabled=true;
  
  this->minMicros=minMicros;
  this->maxMicros=maxMicros;
  this->minDegrees=minDegrees;
  microsPerDegree=(double)(maxMicros-minMicros)/(maxDegrees-minDegrees);

  if(numChannels==0){                             // first instantiation of a ServoPin
    ledc_timer_config_t ledTimer;
    ledTimer.timer_num=LEDC_TIMER_1;
    ledTimer.speed_mode=LEDC_HIGH_SPEED_MODE;
    ledTimer.duty_resolution=LEDC_TIMER_16_BIT;
    ledTimer.freq_hz=50;
    ledc_timer_config(&ledTimer);
  }

  servoChannel.gpio_num=pin;
  servoChannel.speed_mode=LEDC_HIGH_SPEED_MODE;
  servoChannel.channel=(ledc_channel_t)numChannels++;
  servoChannel.intr_type=LEDC_INTR_DISABLE;
  servoChannel.timer_sel=LEDC_TIMER_1;
  servoChannel.hpoint=0;
  servoChannel.duty*=micros2duty;  
  set(initDegrees);
}

///////////////////

void ServoPin::set(double degrees){
  if(!enabled)
    return;
  
  servoChannel.duty=(degrees-minDegrees)*microsPerDegree+minMicros;
  
  if(servoChannel.duty<minMicros)
    servoChannel.duty=minMicros;
  else if(servoChannel.duty>maxMicros)
    servoChannel.duty=maxMicros;

  servoChannel.duty*=micros2duty;  
  ledc_channel_config(&servoChannel);
}

////////////////////////////

const double ServoPin::micros2duty=65535.0/20000.0;
uint8_t LedPin::numChannels=0;
uint8_t ServoPin::numChannels=0;
uint8_t LedC::nChannels=0;
vector<ledc_timer_config_t> LedC::timers;
ledc_channel_config_t *LedC::channelList[LEDC_CHANNEL_MAX][LEDC_SPEED_MODE_MAX]={};
ledc_timer_config_t *LedC::timerList[LEDC_TIMER_MAX][LEDC_SPEED_MODE_MAX]={};

//*******************************************************
// DEPRECATED - INCLUDED FOR BACKWARDS COMPATIBILITY ONLY
//*******************************************************

PwmPin::PwmPin(uint8_t channel, uint8_t pin){
  this->channel=channel & 0x0F;
  this->pin=pin;

  ledc_timer_config_t ledTimer;
  ledTimer.timer_num=LEDC_TIMER_0;
  ledTimer.speed_mode=(this->channel)<8?LEDC_HIGH_SPEED_MODE:LEDC_LOW_SPEED_MODE;
  ledTimer.duty_resolution=LEDC_TIMER_10_BIT;
  ledTimer.freq_hz=5000;
  ledc_timer_config(&ledTimer);

  ledChannel.gpio_num=pin;
  ledChannel.speed_mode=(this->channel)<8?LEDC_HIGH_SPEED_MODE:LEDC_LOW_SPEED_MODE;
  ledChannel.channel=(ledc_channel_t)(this->channel&0x07);
  ledChannel.intr_type=LEDC_INTR_DISABLE;
  ledChannel.timer_sel=LEDC_TIMER_0;
  ledChannel.duty=0;
  ledChannel.hpoint=0;
  ledc_channel_config(&ledChannel);
  
}

///////////////////

void PwmPin::set(uint8_t channel, uint8_t level){
  ledChannel.duty=level*1023;
  ledChannel.duty/=100;
  ledChannel.duty&=0x03FF;
  ledc_channel_config(&ledChannel);
  
}

///////////////////

void PwmPin::HSVtoRGB(float h, float s, float v, float *r, float *g, float *b ){

  // The algorithm below was provided on the web at https://www.cs.rit.edu/~ncs/color/t_convert.html
  // h = [0,360]
  // s = [0,1]
  // v = [0,1]

  int i;
  float f, p, q, t;
  
  if( s == 0 ){
    *r = *g = *b = v;
    return;
  }
  
  h /= 60;
  i = floor( h ) ;
  f = h - i;
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );
  switch( i % 6 ) {
    case 0:
      *r = v;
      *g = t;
      *b = p;
      break;
    case 1:
      *r = q;
      *g = v;
      *b = p;
      break;
    case 2:
      *r = p;
      *g = v;
      *b = t;
      break;
    case 3:
      *r = p;
      *g = q;
      *b = v;
      break;
    case 4:
      *r = t;
      *g = p;
      *b = v;
      break;
    case 5:
      *r = v;
      *g = p;
      *b = q;
      break;
  }
}
