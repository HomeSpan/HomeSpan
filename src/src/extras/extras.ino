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
 
//#include "PwmPin.h"
//
//LedPin led(13);
//
//void setup() {
// 
//  Serial.begin(115200);
//  delay(1000);
//
//  Serial.print("\n\nReady\n\n");
//  
//}
//
////////////////////////////////////////
//
//void loop(){
//
//  led.fade(100,1000);
//  while(led.fadeStatus()==LedPin::FADING);
//  led.fade(0,1000);
//  while(led.fadeStatus()==LedPin::FADING);
//
//}

/*
 * 
esp_err_t     err;
//volatile int iCycles;

#include <driver/rmt_tx.h>

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Serial.printf("\n\nREADY\n");

  rmt_channel_handle_t tx_chan = NULL;
  rmt_tx_channel_config_t tx_chan_config;
  tx_chan_config.clk_src = RMT_CLK_SRC_REF_TICK;   // select source clock
//  tx_chan_config.clk_src = RMT_CLK_SRC_APB;
  tx_chan_config.gpio_num = (gpio_num_t)13;                    // GPIO number
  tx_chan_config.mem_block_symbols = 64;          // memory block size, 64 * 4 = 256 Bytes
  tx_chan_config.resolution_hz = 1 * 1000 * 1000; // 1 tick = 100 us
  tx_chan_config.trans_queue_depth = 4;           // set the number of transactions that can pend in the background
  tx_chan_config.flags.invert_out = false;        // do not invert output signal
  tx_chan_config.flags.with_dma = false;          // do not need DMA backend
  tx_chan_config.intr_priority = 3;               // medium interrupt priority
  
  err=rmt_new_tx_channel(&tx_chan_config, &tx_chan);
  Serial.printf("%s\n",esp_err_to_name(err));

  err=rmt_enable(tx_chan);
  Serial.printf("%s\n",esp_err_to_name(err));

  #define NSYMBOLS 15
  rmt_symbol_word_t x[NSYMBOLS];
  for(int i=0;i<NSYMBOLS;i++){
    x[i].duration0=5000;
    x[i].level0=1;
    x[i].duration1=5000;
    x[i].level1=0;
  }
  x[NSYMBOLS-1].duration1=15000;
  
  rmt_transmit_config_t tx_config;
  tx_config.loop_count=0;
  tx_config.flags.eot_level=0;
  tx_config.flags.queue_nonblocking=0;
  
  rmt_copy_encoder_config_t copy_config;
  rmt_encoder_handle_t encoder;
  
  err=rmt_new_copy_encoder(&copy_config, &encoder);
  Serial.printf("%s\n",esp_err_to_name(err));

  uint32_t t0=millis();
  for(int i=0;i<5;i++){
    err=rmt_transmit(tx_chan, encoder, x, NSYMBOLS*4, &tx_config);
//    Serial.printf("%s\n",esp_err_to_name(err));
    rmt_tx_wait_all_done(tx_chan,-1);
  }
  uint32_t t1=millis();
  Serial.printf("Time = %lu msec\n",t1-t0);
  
 * 
 * 
 */




#include "RFControl.h"

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.print("\n\nHomeSpan RF Transmitter Example\n\n");

  RFControl rf(13);               // create an instance of RFControl with signal output to pin 13 of the ESP32

  Serial.printf("%d %d\n",SOC_RMT_MEM_WORDS_PER_CHANNEL,SOC_RMT_TX_CANDIDATES_PER_GROUP);

  rf.clear();                     // clear the pulse train memory buffer

  #define COUNT   641
  #define ONTIME  50
  #define OFFTIME 50

  Serial.printf("* %d\n",(COUNT*(ONTIME+OFFTIME)+ONTIME+10000)*4*10/1000);

  for(int i=0;i<COUNT;i++)
    rf.add(ONTIME,OFFTIME);              // create a pulse train with three 500-tick high/low pulses
  rf.add(ONTIME,10000);             // double duration of final low period

  Serial.print("Starting 4 cycles of three 50 ms on pulses...\n");

  long int x=millis();
  rf.start(4,10);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds
  long int y=millis();

  Serial.println(y-x);

  Serial.print("Done!\n");
  while(1);

  rf.clear();                     // clear the pulse train memory buffer

  rf.add(5000,5000);              // create a pulse train with three 5000-tick high/low pulses
  rf.add(5000,5000);
  rf.add(5000,10000);             // double duration of final low period

  Serial.print("Starting 4 cycles of three 500 ms on pulses...");
  
  rf.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds

  Serial.print("Done!\n");

  delay(2000);

  rf.clear();

  for(int i=1000;i<10000;i+=1000)
    rf.add(i,10000-i);
  rf.add(10000,10000);
  
  Serial.print("Starting 3 cycles of 100-1000 ms pulses...");
  
  rf.start(3,100);                // start transmission of 3 cycles of the pulse train with 1 tick=100 microseconds

  Serial.print("Done!\n");
  
  Serial.print("\nEnd Example");
  
} // end of setup()

void loop(){

} // end of loop()
