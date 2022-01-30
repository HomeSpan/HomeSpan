/* HomeSpan Pixel Example */

#include "Pixel.h"

struct Effect1 {

  Pixel *px;
  int H=0;
  uint32_t alarmTime=0;
  uint32_t speed;

  Effect1(Pixel *px, uint32_t speed=20){
    this->px=px;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    px->setHSV(H,100,100,0,60);
    H=(H+1)%360;
     
    alarmTime=millis()+speed;
  }
};

struct Effect2 {

  Pixel *px;
  int phase=0;
  int dir=1;
  int H=0;
  uint32_t x[60];
  uint32_t alarmTime=0;
  uint32_t speed;

  Effect2(Pixel *px, uint32_t speed=20){
    this->px=px;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    for(int i=0;i<60;i++){
      if(i==phase)
        x[i]=px->getColorHSV(H,100,100);
      else if(i==59-phase)
        x[i]=px->getColorHSV(H+180,100,100);
      else
        x[i]=0;
    }

    px->setColors(x,60);
    phase=(phase+dir)%60;
    
    if(phase==0){
      dir=1;
      H=(H+10)%360;
    }
    else if(phase==59){
      dir=-1;
      H=(H+10)%360;
    }
     
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
  
#endif

Pixel px1(PIXEL_PIN_1,Pixel::RGBW);
Pixel px2(PIXEL_PIN_2,Pixel::RGBW);

Effect1 effect1(&px1,20);
Effect2 effect2(&px2,20);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");
   
} // end of setup()

void loop(){
  effect1.update();
  effect2.update();
}
