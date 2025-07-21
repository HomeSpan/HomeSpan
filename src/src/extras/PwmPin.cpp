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
 
#include "PwmPin.h"

///////////////////

LedC::LedC(uint8_t pin, uint16_t freq, boolean invert){

  if(freq==0)
    freq=DEFAULT_PWM_FREQ;

  for(int nMode=0;nMode<LEDC_SPEED_MODE_MAX;nMode++){
    for(int nChannel=0;nChannel<LEDC_CHANNEL_MAX;nChannel++){
      for(int nTimer=0;nTimer<LEDC_TIMER_MAX;nTimer++){
        if(!channelList[nChannel][nMode]){
          
          if(!timerList[nTimer][nMode]){                                // if this timer slot is free, use it
            timerList[nTimer][nMode]=new ledc_timer_config_t();         // create new timer instance
            timerList[nTimer][nMode]->speed_mode=(ledc_mode_t)nMode;
            timerList[nTimer][nMode]->timer_num=(ledc_timer_t)nTimer;
            timerList[nTimer][nMode]->freq_hz=freq;
#if defined(SOC_LEDC_SUPPORT_APB_CLOCK)
            timerList[nTimer][nMode]->clk_cfg=LEDC_USE_APB_CLK;
#elif defined(SOC_LEDC_SUPPORT_PLL_DIV_CLOCK)
            timerList[nTimer][nMode]->clk_cfg=LEDC_USE_PLL_DIV_CLK;
#endif

#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(5, 1, 5)
            timerList[nTimer][nMode]->deconfigure=false;
#endif
            
            
            int res=LEDC_TIMER_BIT_MAX-1;                               // find the maximum possible resolution
            while(80.0e6/(freq*pow(2,res))<1)
              res--;     
              
            timerList[nTimer][nMode]->duty_resolution=(ledc_timer_bit_t)res;
            if(ledc_timer_config(timerList[nTimer][nMode])!=0){
              ESP_LOGE(PWM_TAG,"Frequency=%d Hz is out of allowed range ---",freq);
              delete timerList[nTimer][nMode];
              timerList[nTimer][nMode]=NULL;
              return;              
            }
          }
          
          if(timerList[nTimer][nMode]->freq_hz==freq){                      // if timer matches desired frequency (always true if newly-created above)
            channelList[nChannel][nMode]=new ledc_channel_config_t();       // create new channel instance
            channelList[nChannel][nMode]->speed_mode=(ledc_mode_t)nMode;
            channelList[nChannel][nMode]->channel=(ledc_channel_t)nChannel;
            channelList[nChannel][nMode]->timer_sel=(ledc_timer_t)nTimer;
            channelList[nChannel][nMode]->intr_type=LEDC_INTR_DISABLE;
            channelList[nChannel][nMode]->flags.output_invert=invert;
            channelList[nChannel][nMode]->hpoint=0;
            channelList[nChannel][nMode]->gpio_num=pin;
            timer=timerList[nTimer][nMode];
            channel=channelList[nChannel][nMode];
            return;
          }       
        }
      }
    }
  }
}

///////////////////

LedPin::LedPin(uint8_t pin, float level, uint16_t freq, boolean invert) : LedC(pin, freq, invert){
  
  if(!channel){
    ESP_LOGE(PWM_TAG,"Can't create LedPin(%d) - no open PWM channels and/or Timers",pin);
    return;
  }
  else
    ESP_LOGI(PWM_TAG,"LedPin=%d: mode=%d, channel=%d, timer=%d, freq=%d Hz, resolution=%d bits %s",
      channel->gpio_num,
      channel->speed_mode,
      channel->channel,
      channel->timer_sel,
      timer->freq_hz,
      timer->duty_resolution,
      channel->flags.output_invert?"(inverted)":""
      );

  if(!fadeInitialized){
    ledc_fade_func_install(0);
    fadeInitialized=true;
  }
  ledc_cbs_t fadeCallbackList = {.fade_cb = fadeCallback};                          // for some reason, ledc_cb_register requires the function to be wrapped in a structure
  ledc_cb_register(channel->speed_mode,channel->channel,&fadeCallbackList,this);

  set(level);   
}

///////////////////

void LedPin::set(float level){

  if(!channel)
    return;

  if(level>100)
    level=100;

  float d=level*(pow(2,(int)timer->duty_resolution)-1)/100.0;  
    
  channel->duty=d;
  ledc_channel_config(channel); 
}

///////////////////

int LedPin::fade(float level, uint32_t fadeTime, int fadeType){

  if(!channel)
    return(1);

  if(fadeState==FADING)       // fading already in progress
    return(1);                // return error

  if(level>100)
    level=100;

  float d=level*(pow(2,(int)timer->duty_resolution)-1)/100.0;

  if(fadeType==PROPORTIONAL)
    fadeTime*=fabs((float)ledc_get_duty(channel->speed_mode,channel->channel)-d)/(float)(pow(2,(int)timer->duty_resolution)-1);

  fadeState=FADING;
  ledc_set_fade_time_and_start(channel->speed_mode,channel->channel,d,fadeTime,LEDC_FADE_NO_WAIT);
  return(0);
}

///////////////////

int LedPin::fadeStatus(){
  if(fadeState==COMPLETED){
    fadeState=NOT_FADING;
    return(COMPLETED);
  }

  return(fadeState);
}

///////////////////

bool IRAM_ATTR LedPin::fadeCallback(const ledc_cb_param_t *param, void *arg){
  ((LedPin *)arg)->fadeState=COMPLETED;
  return(false);
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

ServoPin::ServoPin(uint8_t pin, double initDegrees, uint16_t minMicros, uint16_t maxMicros, double minDegrees, double maxDegrees) : LedC(pin, 50){
  
  if(!channel)
    ESP_LOGE(PWM_TAG,"Can't create ServoPin(%d) - no open PWM channels and/or Timers",pin);
  else
    ESP_LOGI(PWM_TAG,"ServoPin=%d: mode=%d channel=%d, timer=%d, freq=%d Hz, resolution=%d bits",
      channel->gpio_num,
      channel->speed_mode,
      channel->channel,
      channel->timer_sel,
      timer->freq_hz,
      timer->duty_resolution
      );
            
  this->minMicros=minMicros;
  this->maxMicros=maxMicros;
  this->minDegrees=minDegrees;
  microsPerDegree=(double)(maxMicros-minMicros)/(maxDegrees-minDegrees);

  set(initDegrees);
      
}

///////////////////

void ServoPin::set(double degrees){

  if(!channel)
    return;

  if(!isnan(degrees)){
    double usec=(degrees-minDegrees)*microsPerDegree+minMicros;
    
    if(usec<minMicros)
      usec=minMicros;
    else if(usec>maxMicros)
      usec=maxMicros;
  
    usec*=timer->freq_hz/1e6*(pow(2,(int)timer->duty_resolution)-1);
  
    channel->duty=usec;
  } else {
    channel->duty=0;
  }
  
  ledc_channel_config(channel);
}

////////////////////////////

ledc_channel_config_t *LedC::channelList[LEDC_CHANNEL_MAX][LEDC_SPEED_MODE_MAX]={};
ledc_timer_config_t *LedC::timerList[LEDC_TIMER_MAX][LEDC_SPEED_MODE_MAX]={};
boolean LedPin::fadeInitialized=false;
