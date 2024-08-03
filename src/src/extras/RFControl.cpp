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
 
#include "RFControl.h"

///////////////////

RFControl::RFControl(uint8_t pin, boolean refClock, boolean installDriver){

#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32S3)
  if(nChannels==RMT_CHANNEL_MAX/2){
#else
  if(nChannels==RMT_CHANNEL_MAX){
#endif
    ESP_LOGE(RFControl_TAG,"Can't create RFControl(%d) - no open channels",pin);
    return;
  }

  config=new rmt_config_t;
  
  config->rmt_mode=RMT_MODE_TX;
  config->tx_config.carrier_en=false;
  config->channel=(rmt_channel_t)nChannels;
  config->flags=0;
  config->clk_div = 1;
  config->mem_block_num=1;
  config->gpio_num=(gpio_num_t)pin;
  config->tx_config.idle_output_en=false;
  config->tx_config.idle_level=RMT_IDLE_LEVEL_LOW;
  config->tx_config.loop_en=false;

  rmt_config(config);

  if(installDriver)
    rmt_driver_install(config->channel,0,0);

  // If specified, set the base clock to 1 MHz so tick-units are in microseconds (before any CLK_DIV is applied), otherwise default will be 80 MHz APB clock

  this->refClock=refClock;
  
  if(refClock)
#ifdef RMT_SYS_CONF_REG
  REG_SET_FIELD(RMT_SYS_CONF_REG,RMT_SCLK_DIV_NUM,79);        // ESP32-C3 and ESP32-S3 do not have a 1 MHz REF Tick Clock, but allows the 80 MHz APB clock to be scaled by an additional RMT-specific divider
#else  
  rmt_set_source_clk(config->channel,RMT_BASECLK_REF);        // use 1 MHz REF Tick Clock for ESP32 and ESP32-S2
#endif

  nChannels++;

}
 
///////////////////

void RFControl::start(uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from internal data structure, repeated for nCycles, where each tick in pulse is tickTime microseconds long
  start(data.data(), data.size(), nCycles, tickTime);  
}

///////////////////

void RFControl::start(uint32_t *data, int nData, uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from specified data pointer, repeated for nCycles, where each tick in pulse is tickTime microseconds long

  if(!config || nData==0)
    return;
    
  rmt_set_clk_div(config->channel,tickTime);                  // set clock divider

  for(int i=0;i<nCycles;i++)                                // loop over nCycles
    rmt_write_items(config->channel, (rmt_item32_t *) data, nData, true);      // start transmission and wait until completed before returning    
}

///////////////////

void RFControl::clear(){
  data.clear();
  lowWord=true;
}

///////////////////

void RFControl::add(uint32_t onTime, uint32_t offTime){

  phase(onTime,HIGH);
  phase(offTime,LOW);  
}

///////////////////

void RFControl::phase(uint32_t nTicks, uint8_t phase){

  while(nTicks>0){                                      // create as many repeated phases as needed to accomodate duration of nTicks
    uint32_t ticks=nTicks>0x7FFF?0x7FFF:nTicks;
    nTicks-=ticks;

    if(lowWord)
      data.push_back(ticks | (phase?(1<<15):0));
    else
      data.back()|=ticks<<16 | (phase?(1<<31):0);
  
    lowWord=!lowWord;
  }
}

///////////////////

void RFControl::enableCarrier(uint32_t freq, float duty){

  if(duty<0)
    duty=0;
  if(duty>1)
    duty=1;

  if(freq>0){
    float period=1.0e6/freq*(refClock?1:80);
    uint32_t highTime=period*duty+0.5;
    uint32_t lowTime=period*(1.0-duty)+0.5;

    if(highTime>0xFFFF || lowTime>0xFFFF){
      ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Frequency is too low!",freq,config->gpio_num,duty);
      return;      
    }

    if(highTime==0){
      ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Duty is too low or frequency is too high!",freq,config->gpio_num,duty);
      return;
    }
    
    if(lowTime==0){
      ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Duty is too high or frequency is too high!",freq,config->gpio_num,duty);
      return;
    }

    rmt_set_tx_carrier(config->channel,true,highTime,lowTime,RMT_CARRIER_LEVEL_HIGH);       
  } else {
    rmt_set_tx_carrier(config->channel,false,0,0,RMT_CARRIER_LEVEL_HIGH);           
  }
}

///////////////////

uint8_t RFControl::nChannels=0;
