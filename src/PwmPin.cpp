
#include "PwmPin.h"
#include <Arduino.h>

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
  
}

void PwmPin::set(uint8_t channel, uint8_t level){
  ledChannel.duty=level*1023;
  ledChannel.duty/=100;
  ledChannel.duty&=0x03FF;
  ledc_channel_config(&ledChannel);
  
}
