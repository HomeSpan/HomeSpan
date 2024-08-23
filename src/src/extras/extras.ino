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

long int x,y;

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.print("\n\nHomeSpan RF Transmitter Example\n\n");

  RFControl rf(13,false);               // create an instance of RFControl with signal output to pin 13 of the ESP32
  RFControl rf1(13);               // create an instance of RFControl with signal output to pin 13 of the ESP32

  Serial.printf("%d %d\n",SOC_RMT_MEM_WORDS_PER_CHANNEL,SOC_RMT_TX_CANDIDATES_PER_GROUP);

  rf.clear();                     // clear the pulse train memory buffer
  rf1.clear();

  #define COUNT   5
  #define ONTIME  5000
  #define OFFTIME 5000  

  for(int i=0;i<COUNT;i++)
    rf.add(ONTIME,OFFTIME);         // create a pulse train with three 500-tick high/low pulses
  rf.phase(OFFTIME,LOW);

  for(int i=0;i<COUNT;i++)
    rf1.add(ONTIME,OFFTIME);         // create a pulse train with three 500-tick high/low pulses
  rf1.phase(OFFTIME,LOW);  

//  rf.disableCarrier();

  Serial.print("Starting cycles of pulses...\n");
  
  x=millis();
  rf.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds
  y=millis();
  Serial.println(y-x);

  Serial.print("Starting cycles of pulses...\n");
  
  x=millis();
  rf1.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds
  y=millis();
  Serial.println(y-x);

  Serial.print("Starting cycles of pulses...\n");
  
  x=millis();
  rf1.start(4,10);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds
  y=millis();
  Serial.println(y-x);

  
  Serial.print("Done!\n");
  
} // end of setup()

void loop(){

} // end of loop()
