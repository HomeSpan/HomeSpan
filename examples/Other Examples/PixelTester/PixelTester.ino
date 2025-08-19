/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2024 Gregg E. Berman
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

/////////////////////// PIXEL TESTER //////////////////////////

// This sketch is designed to help identify the proper settings to use for a NeoPixel,
// NeoPixel Strip, or any device containing one or more single-wire addressable RGB-style LEDs

// DIRECTIONS:  Run sketch and and follow on-screen instructions

//////////////////////////////////////

#include "HomeSpan.h"

#define MAX_BRIGHTNESS  255     // lower this value (max=255) if pixels LEDs are too bright to look at when performing this test

int pin=-1;
int nPixels=0;

Pixel::Color colors[5]={
  Pixel::RGB(MAX_BRIGHTNESS,0,0,0,0),
  Pixel::RGB(0,MAX_BRIGHTNESS,0,0,0),
  Pixel::RGB(0,0,MAX_BRIGHTNESS,0,0),
  Pixel::RGB(0,0,0,MAX_BRIGHTNESS,0),
  Pixel::RGB(0,0,0,0,MAX_BRIGHTNESS)
};

Pixel *testPixel;

//////////////////////////////////////

char *getSerial(){
  static char buf[9];
  strcpy(buf,"");
  return(Utils::readSerial(buf,8));
}

//////////////////////////////////////

void setup() {
   
  Serial.begin(115200);
  delay(1000);
  
  Serial.printf("\n\n*************** PIXEL TESTER **********************\n\n");
  Serial.printf("This sketch helps you identity your Pixel Type\n\n");

  while(pin<0){
    Serial.printf("Enter PIN number to which NeoPixel is connected: ");
    sscanf(getSerial(),"%d",&pin);
    if(pin<0)
      Serial.printf("(invalid entry)\n");
    else
      Serial.printf("%d\n",pin);    
  }

  testPixel=new Pixel(pin,"01234");

  while(nPixels<=0){
    Serial.printf("Enter number of PIXELS in NeoPixel device: ");
    sscanf(getSerial(),"%d",&nPixels);
    if(nPixels<=0)
      Serial.printf("(invalid entry)\n");
    else
      Serial.printf("%d\n",nPixels);    
  }

  Serial.printf("\nFor each test below, specify COLORS shown using the following characters:\n\n");
  if(nPixels==1){
    Serial.printf("  'R' = Red\n");
    Serial.printf("  'G' = Green\n");
    Serial.printf("  'B' = Blue\n");
    Serial.printf("  'W' = White (or Warm-White)\n");
    Serial.printf("  'C' = Cool White\n");
    Serial.printf("  '-' = Pixel is NOT lit\n");
  }
  else{
    Serial.printf("  'R' = FIRST Pixel is Red\n");
    Serial.printf("  'G' = FIRST Pixel is Green\n");
    Serial.printf("  'B' = FIRST Pixel is Blue\n");
    Serial.printf("  'W' = FIRST Pixel is White (or Warm-White)\n");
    Serial.printf("  'C' = FIRST Pixel is Cool White\n");
    Serial.printf("  '-' = neither FIRST nor SECOND Pixel is lit\n");
    Serial.printf("  'X' = FIRST Pixel is not lit, but SECOND Pixel is lit (any color)\n");
  }
  Serial.printf("\nNote: entries are case-insensitive.\n\n");

  char pType[6]="";
  
  for(int i=0;i<5;i++){
    testPixel->set(colors[i]);
    while(strlen(pType)==i){
      Serial.printf("Test #%d - enter COLOR: ",i+1);
      if(nPixels==1)
        sscanf(getSerial(),"%1[RGBWCrgbwc-]",pType+i);
      else
        sscanf(getSerial(),"%1[RGBWCrgbwcxX-]",pType+i);
      if(strlen(pType)==i)
        Serial.printf("(invalid entry)\n");
      else{
        pType[i]=toupper(pType[i]);
        Serial.printf("'%s'\n",pType+i);
      }
    }
    if(pType[i]=='X')
      break;
  }

  while(strlen(pType)>3 && ((pType[strlen(pType)-1]=='-' && nPixels==1) || pType[strlen(pType)-1]=='X'))
      pType[strlen(pType)-1]='\0';
      
  Serial.printf("\nTest Concluded.  Best match for your Pixel Type is '%s'\n\n",pType);
  testPixel=new Pixel(pin,pType);
  testPixel->set(Pixel::RGB(0,0,0,0,0),nPixels);
  Serial.printf("Hit ENTER to verify with flashing test\n\n");
  getSerial();

}

//////////////////////////////////////

void loop(){
  
  char c[]="RGBWC";

  for(int i=0;i<5;i++){
    if(testPixel->hasColor(c[i])){
      Serial.printf("Color '%c'...",c[i]);
      
      for(int v=0;v<MAX_BRIGHTNESS;v++){
        testPixel->set(Pixel::RGB(i==0?v:0,i==1?v:0,i==2?v:0,i==3?v:0,i==4?v:0),nPixels);
        delay(4*255/MAX_BRIGHTNESS);
      }
        
      for(int v=MAX_BRIGHTNESS;v>=0;v--){
        testPixel->set(Pixel::RGB(i==0?v:0,i==1?v:0,i==2?v:0,i==3?v:0,i==4?v:0),nPixels);
        delay(4*255/MAX_BRIGHTNESS);
      }

      if(nPixels>1){
        delay(500);
        for(int n=1;n<=nPixels;n++){
          testPixel->set(Pixel::RGB(i==0?MAX_BRIGHTNESS:0,i==1?MAX_BRIGHTNESS:0,i==2?MAX_BRIGHTNESS:0,i==3?MAX_BRIGHTNESS:0,i==4?MAX_BRIGHTNESS:0),n);
          delay(500);
        }
        for(int n=1;n<=nPixels;n++){
          testPixel->set(Pixel::RGB(0,0,0,0,0),n);
          delay(500);
        }
      }       
    }
  }
  testPixel->set(Pixel::RGB(0,0,0,0,0),nPixels);
  Serial.printf("Done.\n");
  Serial.printf("Hit ENTER to repeat with flashing test, or type 'R' to restart program...\n");
  if(toupper(getSerial()[0])=='R')
    ESP.restart();
}

//////////////////////////////////////
