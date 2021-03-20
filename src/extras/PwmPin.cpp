
#include "PwmPin.h"

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

void PwmPin::set(uint8_t level){
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

///////////////////

LedPin::LedPin(uint8_t pin, uint8_t level){
  if(numChannels>15){
    Serial.printf("\n*** ERROR:  Can't create LedPin(%d) - no open PWM channels ***\n\n",pin);
    return;
  }

  enabled=true;

  if(numChannels==0){         // first instantiation of an LedPin
    ledc_timer_config_t ledTimer;
    ledTimer.timer_num=LEDC_TIMER_0;
    ledTimer.duty_resolution=LEDC_TIMER_10_BIT;
    ledTimer.freq_hz=5000;
    
    ledTimer.speed_mode=LEDC_HIGH_SPEED_MODE;     // configure both the HIGH-Speed Timer 0 and Low-Speed Timer 0
    ledc_timer_config(&ledTimer);
    
    ledTimer.speed_mode=LEDC_LOW_SPEED_MODE;
    ledc_timer_config(&ledTimer);
  }

  ledChannel.gpio_num=pin;
  if(numChannels<8){
    ledChannel.speed_mode=LEDC_LOW_SPEED_MODE;
    ledChannel.channel=(ledc_channel_t)(7-numChannels);
  } else {
    ledChannel.speed_mode=LEDC_HIGH_SPEED_MODE;
    ledChannel.channel=(ledc_channel_t)(15-numChannels);
  }

  numChannels++;
    
  ledChannel.intr_type=LEDC_INTR_DISABLE;
  ledChannel.timer_sel=LEDC_TIMER_0;
  ledChannel.hpoint=0;
  ledc_channel_config(&ledChannel);
  set(level);  
}

void LedPin::set(uint8_t level){
  if(!enabled)
    return;
    
  ledChannel.duty=level*1023;
  ledChannel.duty/=100;
  ledChannel.duty&=0x03FF;
  ledc_channel_config(&ledChannel);
  
}

////////////////////////////

ServoPin::ServoPin(uint8_t channel, uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees){
  this->channel=channel & 0x07;
  this->pin=pin;
  this->minMicros=minMicros;
  this->maxMicros=maxMicros;
  this->minDegrees=minDegrees;
  microsPerDegree=(double)(maxMicros-minMicros)/(maxDegrees-minDegrees);

  ledc_timer_config_t ledTimer;
  ledTimer.timer_num=LEDC_TIMER_1;
  ledTimer.speed_mode=LEDC_HIGH_SPEED_MODE;
  ledTimer.duty_resolution=LEDC_TIMER_16_BIT;
  ledTimer.freq_hz=50;
  ledc_timer_config(&ledTimer);

  ledChannel.gpio_num=pin;
  ledChannel.speed_mode=LEDC_HIGH_SPEED_MODE;
  ledChannel.channel=(ledc_channel_t)(this->channel);
  ledChannel.intr_type=LEDC_INTR_DISABLE;
  ledChannel.timer_sel=LEDC_TIMER_1;
  ledChannel.hpoint=0;
  ledChannel.duty*=micros2duty;  
  set(initDegrees);
}

void ServoPin::set(double degrees){
  ledChannel.duty=(degrees-minDegrees)*microsPerDegree+minMicros;
  
  if(ledChannel.duty<minMicros)
    ledChannel.duty=minMicros;
  else if(ledChannel.duty>maxMicros)
    ledChannel.duty=maxMicros;

  ledChannel.duty*=micros2duty;  
  ledc_channel_config(&ledChannel);
}

const double ServoPin::micros2duty=65535.0/20000.0;
uint8_t LedPin::numChannels=0;
