/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
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

#include "HomeSpan.h"

struct LED_Service : Service::LightBulb {

  int ledPin;
  SpanCharacteristic *power;
  
  LED_Service(int ledPin) : Service::LightBulb(){
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

struct invertedLED : Blinkable {        // create a child class derived from Blinkable

  int pin;                              // variable to store the pin number
  
  invertedLED(int pin) : pin{pin} {     // constructor that initializes the pin parameter
    pinMode(pin,OUTPUT);                // set the pin to OUTPUT
    digitalWrite(pin,HIGH);             // set pin HIGH (which is off for an inverted LED)
  }

  void on() override { digitalWrite(pin,LOW); }        // required function on() - sets pin LOW
  void off() override { digitalWrite(pin,HIGH); }      // required function off() - sets pin HIGH
  int getPin() override { return(pin); }               // required function getPin() - returns pin number
};


//////////////////////////////////////

void setup() {
  
  Serial.begin(115200);

//  homeSpan.setLogLevel(-1);
//  homeSpan.setSerialInputDisable(true);
  homeSpan.enableOTA();

  homeSpan.setStatusDevice(new invertedLED(13));    // set Status LED to be a new Blinkable device attached to pin 13
  homeSpan.setStatusAutoOff(30);

  homeSpan.begin(Category::Lighting,"HomeSpan LED");
  
  new SpanAccessory();   
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();
    new LED_Service(13);  
}

//////////////////////////////////////

void loop(){ 
  homeSpan.poll();
}

//////////////////////////////////////
