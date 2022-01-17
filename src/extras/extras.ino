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

//Pixel px(21,8);
//Effect1 effect1(&px,5);
//Effect2 effect2(&px,100);

#ifdef CONFIG_IDF_TARGET_ESP32C3
  #define PIN 7
  #define RMT_MEM_SIZE 48
#else  
  #define PIN 13
  #define RMT_MEM_SIZE 64
#endif

volatile uint32_t data[10];

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Pixel Example\n");

  Pixel px0(10);
  Pixel px(1);
  Pixel px1(0);

  uint32_t colors[20];

  colors[0]=px.getColorRGB(40,0,0);
  colors[1]=px.getColorRGB(40,0,0);
  colors[2]=px.getColorRGB(40,0,0);
  colors[3]=px.getColorRGB(40,40,0);
  colors[4]=px.getColorRGB(40,40,0);
  colors[5]=px.getColorRGB(40,40,0);
  colors[6]=px.getColorRGB(0,40,0);
  colors[7]=px.getColorRGB(40,0,0);
  
  px.setColors(colors,8);
  px.setHSV(0,100,40,3);
  Serial.println("\n\nDone\n\n");
  while(1);

  RFControl rf(PIN,true,false);
  rmt_set_clk_div(rf.getChannel(),100);                  // set clock divider

  rmt_isr_register(eot,(void *)data,0,NULL);
  rmt_set_tx_intr_en(rf.getChannel(),true);
  rmt_set_tx_thr_intr_en(rf.getChannel(),true,32);  

  RMTMEM.chan[0].data32[0].val=5000<<16 | 5000 | 1<<15;
  RMTMEM.chan[0].data32[1].val=5000<<16 | 5000 | 1<<15;

  for(int i=2;i<RMT_MEM_SIZE;i++)
    RMTMEM.chan[0].data32[i].val=200<<16 | 200 | 1<<15;

  data[0]=5;

  rmt_tx_start(rf.getChannel(),true);

   while(1);
  
//  Serial.printf("PX on Pin=%d check: %s\n",px.getPin(),px?"OKAY":"BAD");
//
//  px.setRGB(0,0,0,8);
//  for(int i=1;i<5;i++){
//    px.setHSV(0,100,20,i);
//    delay(500);
//  }
//  
//  for(int i=5;i<8;i++){
//    px.setHSV(60,100,30,i);
//    delay(500);
//  }
//
//  px.setHSV(120,100,100,8);
//  delay(500);
   
} // end of setup()

void loop(){

//  effect2.update();
}

void eot(void *arg){
  Serial.printf("%08X\n",RMT.int_st.val);
  RMT.int_clr.ch0_tx_end=1;                           // must clear interrupt immediately, else havoc will break out
  RMT.int_clr.ch0_tx_thr_event=1;                           // must clear interrupt immediately, else havoc will break out
  Serial.printf("%08X\n",RMT.int_st.val);
  volatile uint32_t *x=(uint32_t *)arg;
  Serial.printf("%d\n",x[0]);
  if(--x[0]==0)
    RMTMEM.chan[0].data32[0].val=0;
  
//  x[1]--;
//  if(x[1])
//    REG_WRITE(RMT_CH0CONF1_REG,0x0000000D);           // use REF_TICK clock; reset xmit and receive memory address to start of channel; START TRANSMITTING!
}
