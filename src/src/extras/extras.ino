// This is a placeholder .ino file that allows you to easily edit the contents of this files using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "Blinker.h"
#include "Pixel.h"
#include <Wire.h>

Blinker p(new Pixel(2),10);
//Blinker p(NULL,10);

void setup() {     
 
  Serial.begin(115200);           // start the Serial interface
  Serial.flush();
  delay(1000);                    // wait for interface to flush

  Serial.println("\n\nHomeSpan Blinker Example\n");
   Serial.printf("Pins = %d\n",p.getPin());

  p.on();
  delay(2000);
  p.off();
  delay(2000);
  p.start(300,0.25,4,1000);
  delay(5000);
  Serial.printf("New Pattern\n");
  p.start(200,0.2,2,200);
}

void loop(){
  p.check();
}
