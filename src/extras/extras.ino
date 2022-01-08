/* HomeSpan Pixel Example */

#include "Pixel.h"

struct Effect1 {

  Pixel *px;
  int H=0;
  color_t x[8];
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
  color_t x[8];
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

Pixel px(21);
Effect1 effect1(&px,5);
Effect2 effect2(&px,100);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");
  Serial.printf("PX on Pin=%d check: %s\n",px.getPin(),px?"OKAY":"BAD");

  px.setRGB(0,0,0,8);
  px.setRGB(255,0,0,1);
  delay(500);
  px.setRGB(0,255,0,2);
  delay(500);
  px.setRGB(0,255,0,4);
  delay(500);
  px.setRGB(0,255,255,6);
  delay(500);
  px.setRGB(0,0,255,8);
  delay(500);

//  while(1);
  
  
 
} // end of setup()

void loop(){

  effect2.update();
}
