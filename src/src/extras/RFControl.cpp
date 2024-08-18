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

  if(nChannels==0){
    rmt_ll_enable_bus_clock(0,true);                                  // enable RMT Peripheral clock
    rmt_ll_reset_register(0);                                         // reset RMT Peripheral registers
  }
  else if(nChannels==SOC_RMT_TX_CANDIDATES_PER_GROUP){
    ESP_LOGE(RFControl_TAG,"Can't create RFControl(%d) - no open channels",pin);
    return;
  }

  if(!GPIO_IS_VALID_OUTPUT_GPIO(pin)){
    ESP_LOGE(RFControl_TAG,"Can't create RFControl(%d) - invalid output pin",pin);
    return;    
  }

  channel=nChannels++;                                                  // save channel number and increment nChannels
  
  gpio_hal_iomux_func_sel(GPIO_PIN_MUX_REG[pin], PIN_FUNC_GPIO);        // set IOMUX for pin to GPIO, OUTPUT, and connect to RMT Transmitter Channel
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
  esp_rom_gpio_connect_out_signal(pin, rmt_periph_signals.groups[0].channels[channel].tx_sig, false, 0);
  
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

  rmt_isr_register(loadData,NULL,0,NULL);                                             // set custom interrupt handler

  rmt_set_tx_thr_intr_en(config->channel,false,SOC_RMT_MEM_WORDS_PER_CHANNEL/2);      // disable threshold interrupt
  txThrMask=RMT.int_ena.val;                                                          // save interrupt enable vector
  rmt_set_tx_thr_intr_en(config->channel,true,SOC_RMT_MEM_WORDS_PER_CHANNEL/2);       // enable threshold interrupt to trigger every memsize/2 pulses 
  txThrMask^=RMT.int_ena.val;                                                         // find bit that flipped and save as threshold mask for this channel 

  rmt_set_tx_intr_en(config->channel,false);           // disable end-of-transmission interrupt
  txEndMask=RMT.int_ena.val;                           // save interrupt enable vector
  rmt_set_tx_intr_en(config->channel,true);            // enable end-of-transmission interrupt
  txEndMask^=RMT.int_ena.val;                          // find bit that flipped and save as end-of-transmission mask for this channel 

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
    
  rmt_set_clk_div(config->channel,tickTime);    // set clock divider

  for(int i=0;i<nCycles;i++){                   // loop over nCycles
    status.nData=nData;
    status.iMem=0;
    status.started=true;
    status.rf=this;
    status.pulse=data;
    
    loadData(this);
    if(status.nData>=0)
      loadData(this);

    rmt_tx_start(config->channel,true);
    while(status.started);                      // wait for transmission to be complete
  }
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

void IRAM_ATTR RFControl::loadData(void *arg){

  if(RMT.int_st.val & status.rf->txEndMask){
    RMT.int_clr.val=status.rf->txEndMask;
    status.started=false;
    return;
  }
  
  RMT.int_clr.val=status.rf->txThrMask;       // if loadData() is called and it is NOT because of an END interrupt (above) then must either be a pre-load, or a threshold trigger

  int i=0;
  while(i<SOC_RMT_MEM_WORDS_PER_CHANNEL/2 && status.nData>0){
    RMTMEM.chan[status.rf->getChannel()].data32[status.iMem].val=*status.pulse;
    status.iMem=status.iMem+1;    
    status.pulse=status.pulse+1;
    status.nData=status.nData-1;
    status.iMem=status.iMem%SOC_RMT_MEM_WORDS_PER_CHANNEL;
    i++;
  }

  if(i<SOC_RMT_MEM_WORDS_PER_CHANNEL/2){
    RMTMEM.chan[status.rf->getChannel()].data32[status.iMem].val=0;  
    status.nData=status.nData-1;
  }
}

///////////////////

uint8_t RFControl::nChannels=0;
volatile RFControl::rf_status_t RFControl::status;
