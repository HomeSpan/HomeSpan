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

#include "Pixel.h"
#include "RFControl.h"

void setup(){     
 
  Serial.begin(115200);           // start the Serial interface
  delay(1000);

  Serial.print("\n\nHomeSpan Pixel+RF Example\n\n");

  Pixel px(23);
  
  Pixel::Color c[8]={
    Pixel::RGB(255,0,0),
    Pixel::RGB(255,0,0),
    Pixel::RGB(255,0,0),
    Pixel::RGB(0,255,0),
    Pixel::RGB(0,255,0),
    Pixel::RGB(0,0,255),
    Pixel::RGB(0,0,255),
    Pixel::RGB(0,0,255)
  };
  
  Pixel::Color d[8]={
    Pixel::HSV(0,100,10),
    Pixel::HSV(0,100,10),
    Pixel::HSV(0,100,10),
    Pixel::HSV(120,100,10),
    Pixel::HSV(120,100,10),
    Pixel::HSV(240,100,10),
    Pixel::HSV(240,100,10),
    Pixel::HSV(240,100,10)
  };

for(int i=0;i<4;i++){
    px.set(c,8);
    delay(1000);        
    px.set(d,8);
    delay(1000);
}

  long int x,y;

  RFControl rfi(21,false);
  RFControl rf(13);              // use REF TICK

  rf.clear();                     // clear the pulse train memory buffer
  rfi.clear();

  #define COUNT   5
  #define ONTIME  5000
  #define OFFTIME 5000  

  for(int i=0;i<COUNT;i++)
    rfi.add(ONTIME*80,OFFTIME*80);
  rfi.phase(OFFTIME*80,LOW);
  
  for(int i=0;i<COUNT;i++)
    rf.add(ONTIME,OFFTIME);
  rf.phase(OFFTIME,LOW);

//  rf.enableCarrier(10,0.4);

  Serial.print("Starting cycles of pulses...\n");
  x=millis();
  rfi.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100/80 microseconds
  y=millis();
  Serial.println(y-x);

  Serial.print("Starting cycles of pulses...\n");
  x=millis();
  rf.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100/80 microseconds
  y=millis();
  Serial.println(y-x);

  c[0]=Pixel::RGB(0,0,0);
  c[1]=c[0];
  c[2]=c[1];
  c[3]=Pixel::RGB(0,0,0);
  c[4]=c[3];
  c[5]=Pixel::RGB(0,0,0);
  c[6]=c[5];
  c[7]=c[6];

  px.set(c,8);

  Serial.print("Done!\n");  
}

void loop(){
  
}


/////////

//#include "RFControl.h"
//
//long int x,y;
//
//void setup(){     
// 
//  Serial.begin(115200);
//  delay(1000);
//
//  Serial.print("\n\nHomeSpan RF Transmitter Example\n\n");
//
//  RFControl rf(13,false);         // do not use REF TICK
//  RFControl rf1(21);              // use REF TICK
//
//  Serial.printf("%d %d\n",SOC_RMT_MEM_WORDS_PER_CHANNEL,SOC_RMT_TX_CANDIDATES_PER_GROUP);
//
//  rf.clear();                     // clear the pulse train memory buffer
//  rf1.clear();
//
//  #define COUNT   5
//  #define ONTIME  5000
//  #define OFFTIME 5000  
//
//  for(int i=0;i<COUNT;i++)
//    rf.add(ONTIME*80,OFFTIME*80);
//  rf.phase(OFFTIME*80,LOW);
//
//  for(int i=0;i<COUNT;i++)
//    rf1.add(ONTIME,OFFTIME);
//  rf1.phase(OFFTIME,LOW);  
//
//  rf1.enableCarrier(10,0.4);
//
//  Serial.print("Starting cycles of pulses...\n");
//  x=millis();
//  rf.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100/80 microseconds
//  y=millis();
//  Serial.println(y-x);
//
//  Serial.print("Starting cycles of pulses...\n");
//  x=millis();
//  rf1.start(4,100);                // start transmission of 4 cycles of the pulse train with 1 tick=100 microseconds
//  y=millis();
//  Serial.println(y-x);
//
//  Serial.print("Done!\n");
//  
//}
//
//void loop(){
//
//}

///////////////////
