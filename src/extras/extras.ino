/* HomeSpan Pixel Example */

#include "Pixel.h"

struct Effect1 {

  Pixel *px;
  int H=0;
  uint32_t x[8];
  uint32_t alarmTime=0;
  uint32_t speed;

  Effect1(Pixel *px, uint32_t speed=20){
    this->px=px;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    for(int i=0;i<8;i++)
      x[i]=px->getColorHSV(H,i*3+79,i*2+5);

    px->setColors(x,8);
    H=(H+1)%360;
     
    alarmTime=millis()+speed;
  }
};

struct Effect2 {

  Pixel *px;
  int phase=0;
  int dir=1;
  int H=0;
  uint32_t x[8];
  uint32_t alarmTime=0;
  uint32_t speed;

  Effect2(Pixel *px, uint32_t speed=20){
    this->px=px;
    this->speed=speed;
  }

  void update(){
    if(millis()<alarmTime)
      return;
    
    for(int i=0;i<8;i++){
      if(i==phase)
        x[i]=px->getColorHSV(H,100,10);
      else if(i==7-phase)
        x[i]=px->getColorHSV(H+180,100,10);
      else
        x[i]=0;
    }

    px->setColors(x,8);
    phase=(phase+dir)%8;
    
    if(phase==0){
      dir=1;
      H=(H+10)%360;
    }
    else if(phase==7){
      dir=-1;
      H=(H+10)%360;
    }
     
    alarmTime=millis()+speed;
  }
};

#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define PIN 1
#elif defined(CONFIG_IDF_TARGET_ESP32S2)
  #define PIN 1
#else
  #define PIN 21
#endif

Pixel px(PIN);
Pixel px2(2);
Pixel px5(5);
Pixel px3(3);
Pixel px4(4);
Pixel px6(6);

Effect1 effect1(&px,5);
Effect2 effect2(&px,100);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");

  Serial.println(px.getPin());
  Serial.println(px2.getPin());
  Serial.println(px3.getPin());
  Serial.println(px4.getPin());
  Serial.println(px5.getPin());
  Serial.println(px6.getPin());
   
} // end of setup()

void loop(){
  effect2.update();
}
