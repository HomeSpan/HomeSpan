/* HomeSpan Pixel Example */

#include "Pixel.h"

struct Effect1 {

  Pixel *px;
  int H=0;
  uint32_t alarmTime=0;
  uint32_t speed;
  uint8_t nPixels;
  Pixel::Color c;

  Effect1(Pixel *px, uint32_t speed, uint8_t nPixels){
    this->px=px;
    this->nPixels=nPixels;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;

    px->set(c.HSV(H,100,10),nPixels);
    H=(H+1)%360;

    alarmTime=millis()+speed;
  }
};

struct Effect2 {

  Pixel *px;
  int phase=0;
  int dir=1;
  int H=0;
  Pixel::Color x[60];
  uint32_t alarmTime=0;
  uint32_t speed;
  uint8_t nPixels;

  Effect2(Pixel *px, uint32_t speed, uint8_t nPixels){
    this->px=px;
    this->nPixels=nPixels;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    for(int i=0;i<nPixels;i++){
      if(i==phase)
        x[i].HSV(H,100,100);
      else if(i==nPixels-1-phase)
        x[i].HSV(H+180,100,100);
      else
        x[i].RGB(0,0,0);
    }

    px->set(x,nPixels);
    phase=(phase+dir)%nPixels;
    
    if(phase==0){
      dir=1;
      H=(H+10)%360;
    }
    else if(phase==nPixels-1){
      dir=-1;
      H=(H+10)%360;
    }
     
    alarmTime=millis()+speed;
  }
};

struct Effect3 {

  Dot *dot;
  int H=0;
  uint32_t alarmTime=0;
  uint32_t speed;
  uint8_t nPixels;
  Dot::Color c;

  Effect3(Dot *dot, uint32_t speed, uint8_t nPixels){
    this->dot=dot;
    this->nPixels=nPixels;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    dot->set(c.HSV(H,100,100),nPixels);
    H=(H+1)%360;
     
    alarmTime=millis()+speed;
  }
};

#if defined(CONFIG_IDF_TARGET_ESP32C3)

  Pixel px1(8);          // NeoPixel RGB
  Pixel px2(9,true);     // NeoPixel RGBW
  Dot dot(2,3);          // DotStar

#elif defined(CONFIG_IDF_TARGET_ESP32)

//  Pixel px1(23,true);          // NeoPixel RGB
//  Dot dot(32,5);          // DotStar
  Pixel px2(21);     // NeoPixel RGBW
  Pixel neo(26);
  
#endif

//Effect1 effect1(&px1,20,60);
//Effect2 effect2(&px2,20,60);
//Effect2 effect2(&px1,20,60);
//Effect3 effect3(&dot,20,30);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");

  Pixel::Color c;
  int hue=0;

//  Pixel px2(21);     // NeoPixel RGBW

  while(1){
    neo.set(c.HSV(hue,100,10),8);
    hue=(hue+10)%360;
    delay(100);
  }
   
} // end of setup()

void loop(){
//  effect1.update();
//  effect2.update();
//  effect3.update();
}
