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

// Sometimes you need to access Characteristics from outside of the Service structure
// in which they were created so you can read and/or modify them in other parts of
// a sketch, such as from within the main Arduino loop().

// This sketch is basically the same as Tutorial Example 5, in which we created
// two working LEDs attached to pins 16, and 17.  However, in this sketch we will 
// create global pointers to the LED Services that we can then use in the main loop() to 
// do something unique.

#include "HomeSpan.h" 

//////////////////////////////////////

// First we define our DEV_LED Service exactly the same as in Tutorial Example 5.
// This Service contains a single Characteristic named "power" of type Chacracteristic::On

struct DEV_LED : Service::LightBulb {

  int ledPin;
  SpanCharacteristic *power;
  
  DEV_LED(int ledPin) : Service::LightBulb(){

    power=new Characteristic::On();
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);    
  }

  boolean update(){            

    digitalWrite(ledPin,power->getNewVal());
    return(true);  
  }
};
   
//////////////////////////////////////

// Next we create two pointers to the DEV_LED Service.  These are created
// outside of any class or function so they are globally-scoped and can be
// accessed from anywhere else in this sketch.

// Note that there are just POINTERS to DEV_LED objects.  The objects themselves
// are not yet created.

DEV_LED *led16;     // pointer to a DEV_LED structure to be used below to reference a DEV_LED object assigned to pin 16
DEV_LED *led17;     // pointer to a DEV_LED structure to be used below to reference a DEV_LED object assigned to pin 17

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan LED");
  
  new SpanAccessory();  
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();                
    led16=new DEV_LED(16);                  // this is the key step - we SAVE the pointer returned by 'new DEV_LED(16)' in the global variable led16 created above


  new SpanAccessory();   
    new Service::AccessoryInformation();    
      new Characteristic::Identify();                       
    led17=new DEV_LED(17);                  // also save the pointer to the second LED object using the global variable led17 created above

}

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();

// Because the pointers led16 and led17 were created in the global scope, they will still exist even after setup() is completed.
// This means we can use them to access the Characteristics within each of those Services.

// Here we access the power Characteristic of both Services and check to see if they are BOTH on, and if so,
// we turn them both off and print a "power overload" message.

// Note how you can use all the same methods, such as getVal() and setVal(), just as you would do in the Service itself.
// Caution: always use getVal(), not getNewVal(), which is only formally defined from within the Service update() method.

  if(led16->power->getVal() && led17->power->getVal()){
    Serial.printf("Power overload!  Can't have both LED's on at the same time.  Turn off both LEDs...\n");
    led16->power->setVal(false);
    led17->power->setVal(false);
  }
  
}

//////////////////////////////////////
