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

RFControl::RFControl(uint8_t pin, boolean refClock){

  this->refClock=refClock;
  this->pin=pin;

  rmt_tx_channel_config_t tx_chan_config;
  memset((void *)&tx_chan_config, 0, sizeof(rmt_tx_channel_config_t));
  tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT;                       // use default as a placeholder - will be reset to required clock before each transmission
  tx_chan_config.gpio_num = (gpio_num_t)pin;                          // GPIO number
  tx_chan_config.mem_block_symbols = SOC_RMT_MEM_WORDS_PER_CHANNEL;   // set number of symbols to match those in a single channel block
  tx_chan_config.resolution_hz = 1 * 1000 * 1000;                     // use 1MHz as a placeholder - will be reset to required clock before each transmission
  tx_chan_config.intr_priority = 3;                                   // medium interrupt priority
  tx_chan_config.trans_queue_depth = 4;                               // set the number of transactions that can pend in the background
  tx_chan_config.flags.invert_out = false;                            // do not invert output signal
  tx_chan_config.flags.with_dma = false;                              // use RMT channel memory, not DMA (most chips do not support use of DMA anyway)
  tx_chan_config.flags.io_loop_back = false;                          // do not use loop-back mode
  tx_chan_config.flags.io_od_mode = false;                            // do not use open-drain output

  if(!GPIO_IS_VALID_OUTPUT_GPIO(pin)){
    ESP_LOGE(RFControl_TAG,"Can't create RFControl(%d) - invalid output pin",pin);
    return;    
  }
  
  if(rmt_new_tx_channel(&tx_chan_config, &tx_chan)!=ESP_OK){
    ESP_LOGE(RFControl_TAG,"Can't create RFControl(%d) - no open channels",pin);
    return;
  }

  rmt_enable(tx_chan);                            // enable channel
  channel=((int *)tx_chan)[0];                    // get channel number

  memset((void *)&tx_config, 0, sizeof(rmt_transmit_config_t));    
  rmt_copy_encoder_config_t copy_config;                                  // required, though nothing to populate
  memset((void *)&copy_config, 0, sizeof(rmt_copy_encoder_config_t));
  rmt_new_copy_encoder(&copy_config, &encoder);                           // create copy encoder
}
 
///////////////////

void RFControl::start(uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from internal data structure, repeated for nCycles
  start(data.data(), data.size(), nCycles, tickTime);  
}

///////////////////

void RFControl::start(uint32_t *data, size_t nData, uint8_t nCycles, uint8_t tickTime){     // starts transmission of pulses from specified data pointer, repeated for nCycles

  if(channel<0 || nData==0)
    return;

  rmt_ll_tx_set_channel_clock_div(&RMT, channel, tickTime);      // set clock divider

  if(refClock)
#if defined(SOC_RMT_SUPPORT_REF_TICK)
    rmt_ll_set_group_clock_src(&RMT, channel, RMT_CLK_SRC_REF_TICK, 0, 0, 0);        // use REF_TICK, which is 1 MHz
#else
    rmt_ll_set_group_clock_src(&RMT, channel, RMT_CLK_SRC_DEFAULT, 80, 0, 0);        // use DEFAULT CLOCK, which is always 80 MHz, and scale by 80 to get 1 MHz
#endif
  else
    rmt_ll_set_group_clock_src(&RMT, channel, RMT_CLK_SRC_DEFAULT, 1, 0, 0);         // use DEFAULT CLOCK, which is always 80 MHz, without any scaling

  for(int i=0;i<nCycles;i++){
    rmt_transmit(tx_chan, encoder, data, nData*4, &tx_config);
    rmt_tx_wait_all_done(tx_chan,-1);
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

  if(channel<0)
    return;
    
  if(freq==0){
    rmt_ll_tx_enable_carrier_modulation(&RMT, channel, 0);    // disable carrier wave
    return;
  }

  if(duty<0)
    duty=0;
  if(duty>1)
    duty=1;
  
  float period=(refClock?1.0e6:80.0e6)/freq;
  uint32_t highTime=period*duty+0.5;
  uint32_t lowTime=period*(1.0-duty)+0.5;

  if(highTime>0xFFFF || lowTime>0xFFFF){
    ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Frequency is too low!",freq,pin,duty);
    return;      
  }

  if(highTime==0){
    ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Duty is too low or frequency is too high!",freq,pin,duty);
    return;
  }
  
  if(lowTime==0){
    ESP_LOGE(RFControl_TAG,"Can't enable carrier frequency=%d Hz for RF Control pin=%d, duty=%0.2f. Duty is too high or frequency is too high!",freq,pin,duty);
    return;
  }

#if SOC_RMT_SUPPORT_TX_CARRIER_DATA_ONLY
  rmt_ll_tx_enable_carrier_always_on(&RMT, channel, 0);                       // for chips that support carrier is always on, disable it
#endif

  rmt_ll_tx_set_carrier_level(&RMT, channel, 1);                              // turn on carrier wave when signal is HIGH
  rmt_ll_tx_set_carrier_high_low_ticks(&RMT, channel, highTime, lowTime);     // set high/low ticks for carrier wave frequency and duty cycle
  rmt_ll_tx_enable_carrier_modulation(&RMT, channel, 1);                      // enable carrier wave

}
