/* HomeSpan Pixel Example */

#include "Pixel.h"

struct Effect1 {

  Pixel *px;
  int H=0;
  uint32_t alarmTime=0;
  uint32_t speed;
  uint8_t nPixels;

  Effect1(Pixel *px, uint32_t speed, uint8_t nPixels){
    this->px=px;
    this->nPixels=nPixels;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    px->set(px->HSV(H,100,10),nPixels);
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
        x[i]=px->HSV(H,100,100);
      else if(i==nPixels-1-phase)
        x[i]=px->HSV(H+180,100,100);
      else
        x[i]=Pixel::HSV(0,0,0);
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

  Effect3(Dot *dot, uint32_t speed, uint8_t nPixels){
    this->dot=dot;
    this->nPixels=nPixels;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    dot->set(Dot::HSV(H,100,100),nPixels);
    H=(H+1)%360;
     
    alarmTime=millis()+speed;
  }
};

#if defined(CONFIG_IDF_TARGET_ESP32C3)

  #define PIXEL_PIN_1   8
  #define PIXEL_PIN_2   1 

#elif defined(CONFIG_IDF_TARGET_ESP32S2)

  #define PIXEL_PIN_1   18
  #define PIXEL_PIN_2   7

#elif defined(CONFIG_IDF_TARGET_ESP32)

  #define PIXEL_PIN_1   23
  #define PIXEL_PIN_2   21

  Dot dot(32,5);
  
#endif

Pixel px1(PIXEL_PIN_1);
Pixel px2(PIXEL_PIN_2,true);

Effect1 effect1(&px1,20,8);
Effect2 effect2(&px2,20,60);
Effect3 effect3(&dot,20,30);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");
   
} // end of setup()

void loop(){
  effect1.update();
  effect2.update();
  effect3.update();
}
