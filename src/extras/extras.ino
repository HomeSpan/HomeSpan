
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "PwmPin.h"
#include <soc/ledc_reg.h> 

void setup(){
 
  Serial.begin(115200);
  delay(1000);

  Serial.print("\n\nTest sketch for HomeSpan Extras Library\n\n");

  Serial.println("Starting...");

  LedPin led12(12,99,500);
  LedPin led5(5,20,20000);
  LedPin led6(6,20,5000);
//  LedPin led3(17,50,1);
//  LedPin led2(16,10,8000);
//  LedPin led4(23,100,2);
//  LedPin led5(22,0,3000);
//  LedPin led6(14,0,1);
//  LedPin led7(32,0,1850);
//  LedPin led8(15);
//  LedPin led9(33);
//  LedPin led10(27);
//  ServoPin led14(25);
//  LedPin led11(12,100,23);
//  LedPin led12(13,100);
//  LedPin led13(26);
//  LedPin led15(4,0);

Serial.printf("HELLO\n");

//  led16.set(20);
//  led0.set(5);
//  led14.set(100);

//  Serial.println(led0.getPin());
//  Serial.println(led14.getPin());
//  Serial.println(led15.getPin());
//  Serial.println(led16.getPin());

Serial.printf("RES0: %d\n",REG_GET_FIELD(LEDC_LSTIMER0_CONF_REG,LEDC_LSTIMER0_DUTY_RES));
Serial.printf("RES1: %d\n",REG_GET_FIELD(LEDC_LSTIMER1_CONF_REG,LEDC_LSTIMER1_DUTY_RES));
Serial.printf("RES2: %d\n",REG_GET_FIELD(LEDC_LSTIMER2_CONF_REG,LEDC_LSTIMER2_DUTY_RES));
Serial.printf("DIV0: %d\n",REG_GET_FIELD(LEDC_LSTIMER0_CONF_REG,LEDC_CLK_DIV_LSTIMER0)>>8);
Serial.printf("DIV1: %d\n",REG_GET_FIELD(LEDC_LSTIMER1_CONF_REG,LEDC_CLK_DIV_LSTIMER1)>>8);
Serial.printf("DIV2: %d\n",REG_GET_FIELD(LEDC_LSTIMER2_CONF_REG,LEDC_CLK_DIV_LSTIMER2)>>8);

//  uint32_t v=REG_READ(LEDC_HSTIMER0_CONF_REG);
//  Serial.printf("HS %d %d %d %d\n",(v>>25)&1,v&0x1f,(v>>13)&0x3FF,(v>>5)&0xFF);

//  v=REG_READ(LEDC_LSTIMER0_CONF_REG);
//  Serial.printf("LS %d %d %d %d %d\n",(v>>25)&1,v&0x1f,(v>>13)&0x3FF,(v>>5)&0xFF,REG_READ(LEDC_CONF_REG));

  

  while(1);
}

void loop(){
}
