
#include <Arduino.h>
#include <soc/rmt_reg.h>

#include "RFControl.h"

///////////////////

RFControl::RFControl(uint8_t pin){

#ifdef CONFIG_IDF_TARGET_ESP32C3
  if(nChannels==RMT_CHANNEL_MAX/2){
#else
  if(nChannels==RMT_CHANNEL_MAX){
#endif
    Serial.printf("\n*** ERROR:  Can't create RFControl(%d) - no open channels ***\n\n",pin);
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
  config->tx_config.loop_en=false;

  rmt_config(config);
  rmt_driver_install(config->channel,0,0);

  // Below we set the base clock to 1 MHz so tick-units are in microseconds (before CLK_DIV)

#ifdef CONFIG_IDF_TARGET_ESP32C3
  REG_SET_FIELD(RMT_SYS_CONF_REG,RMT_SCLK_DIV_NUM,80);        // ESP32-C3 does not have a 1 MHz REF Tick Clock, but allows the 80 MHz APB clock to be scaled by an additional RMT-specific divider
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
