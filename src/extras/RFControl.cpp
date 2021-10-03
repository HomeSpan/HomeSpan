
#include <Arduino.h>
#include <soc/rmt_reg.h>
#include <soc/dport_reg.h>

#include "RFControl.h"

///////////////////

RFControl::RFControl(uint8_t pin){

//  config=new rmt_config_t;
  
  config.rmt_mode=RMT_MODE_TX;
  config.tx_config.carrier_en=false;
  config.channel=RMT_CHANNEL_0;
  config.clk_div = 1;
  config.mem_block_num=1;
  config.gpio_num=(gpio_num_t)pin;

  ESP_ERROR_CHECK(rmt_config(&config));
  ESP_ERROR_CHECK(rmt_driver_install(config.channel,0,0));
  rmt_set_source_clk(RMT_CHANNEL_0,RMT_BASECLK_REF);
}
 
///////////////////

void RFControl::start(uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from internal data structure, repeated for nCycles, where each tick in pulse is tickTime microseconds long
  start(data.data(), data.size(), nCycles, tickTime);  
}

///////////////////

void RFControl::start(uint32_t *data, int nData, uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from specified data pointer, repeated for nCycles, where each tick in pulse is tickTime microseconds long

  rmt_set_clk_div(RMT_CHANNEL_0,tickTime);                  // set clock divider

  for(int i=0;i<nCycles;i++)                                // loop over nCycles
    rmt_write_items(RMT_CHANNEL_0, (rmt_item32_t *) data, nData, true);      // start transmission and wait until completed before returning    
}

///////////////////

void RFControl::clear(){
  data.clear();
}

///////////////////

void RFControl::add(uint16_t onTime, uint16_t offTime){

  phase(onTime,HIGH);
  phase(offTime,LOW);  
}

///////////////////

void RFControl::phase(uint16_t nTicks, uint8_t phase){

  uint32_t ticks=nTicks&0x7FFF;

  if(lowWord)
    data.push_back(ticks | (phase?(1<<15):0));
  else
    data.back()|=ticks<<16 | (phase?(1<<31):0);

  lowWord=!lowWord;
}

///////////////////

uint8_t RFControl::nChannels=0;
