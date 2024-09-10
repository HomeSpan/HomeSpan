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

#define PIXEL_PIN   26
#define LED_PIN     15

#define NCYCLES     4
#define COUNT       5
#define ONTIME      5000
#define OFFTIME     5000

void setup(){     
 
  Serial.begin(115200);           // start the Serial interface
  delay(1000);

  Serial.print("\n\nHomeSpan Pixel+RF Example\n\n");

  Pixel px(PIXEL_PIN,"G-B");
  px.setOnColor(Pixel::RGB(0,255,0))->setTemperatures(2000,6000)->setTiming(0.32, 0.88, 0.64, 0.56, 80.0);
  RFControl rf(LED_PIN);

  Pixel::Color q;
  
  q.CCT(4000,25,3000,6500);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q.CCT(4000,100,3000,6500);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q.CCT(3000,100,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q.CCT(4500,100,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q.CCT(6000,10,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q=px.CCT(6000,10,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q=px.CCT(6000,10);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q.CCT(7000,50,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  q=Pixel::CCT(8000,25,2000,7000);  
  Serial.printf("%d %d %d %d %d\n",q.col[0],q.col[1],q.col[2],q.col[3],q.col[4]);
  
  Pixel::Color c[8]={
    Pixel::RGB(255,0,0),
    Pixel::RGB(255,0,0),
    Pixel::RGB(255,0,0),
    px.RGB(0,255,0),
    px.RGB(0,255,0),
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

  Serial.printf("Starting cycles of RGB pattern...\n");

  for(int i=0;i<NCYCLES;i++){
      px.set(c,8);
      delay(1000);        
      px.set(d,8);
      delay(1000);
  }

  px.set(Pixel::RGB(0,0,0,255,0),100);
  delay(2000);
  px.set(Pixel::RGB(0,0,0,0,255),100);
  delay(2000);
  px.set(Pixel::RGB(0,0,0,0,0),100);
  while(1);
  
  rf.clear();                    // clear the pulse train memory buffer

  for(int i=0;i<COUNT;i++)
    rf.add(ONTIME,OFFTIME);
  rf.phase(OFFTIME,LOW);

  long int x,y;
  
  Serial.printf("Starting cycles of pulses without carrier...\n");
  x=millis();
  rf.start(NCYCLES,100);
  y=millis();
  Serial.printf("Total time: %ld ms\n",y-x);

  rf.enableCarrier(10,0.4);

  Serial.printf("Starting cycles of pulses with 10 Hz carrier...\n");
  x=millis();
  rf.start(NCYCLES,100);
  y=millis();
  Serial.printf("Total time: %ld ms\n",y-x);

  Serial.printf("Turning off RGB LEDs.\n");

  px.set(Pixel::RGB(0,0,0),8);

  Serial.print("Done!\n");  
}

void loop(){
}
