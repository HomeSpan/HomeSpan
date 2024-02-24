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

// HomeSpan Fading-LED Example.  Demonstrates use of:
//
//    LedPin::fade() and LedPin::fadeStatus() methods
//
// In this sketch we control a single dimmable LED using the Home App as well as a SpanButton.
// You can control the brightness of the LED from the Home App, but the SpanButton only turns the
// LED either fully on (if it's off) or fully off (if it's already on).
//
// Rather than set the LED to a specific brightness, this sketch uses the ESP32's hardware-based fading
// functionality to fade the LED from one level to the next.  We set the timing for each fade to be 2000 ms,
// proportional to the difference between the current brightness and the desired brightness.  This means it
// will take a full 2 seconds to fade the LED from 0-100, but only 1 second to fade from half-brightness to
// off.

#include "HomeSpan.h" 

////////////////////////////////////

struct FadingLED : Service::LightBulb {
  
  LedPin *ledPin;                                   // reference to Led Pin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  FadingLED(int _ledPin, int _buttonPin) : Service::LightBulb(){

    power=new Characteristic::On();     
    level=new Characteristic::Brightness(0);
    ledPin=new LedPin(_ledPin);    
    new SpanButton(_buttonPin);
    
  }

  boolean update(){
    
    ledPin->fade(power->getNewVal()*level->getNewVal(),2000,LedPin::PROPORTIONAL);      // use fade() to set new level; timing=2 seconds, proportional scale
    while(ledPin->fadeStatus()==LedPin::FADING);                                        // wait until fading is completed

    return(true);
  
  }

  void button(int pin, int pressType) override {

    // Below we turn LED fully on or off depending on whether power is on
    // Unlike above, we will NOT wait for the fading to complete, but will return immediately
    
    if(ledPin->fade(100-(power->getVal())*100,2000,LedPin::PROPORTIONAL)!=0)            // use fade to either turn fully on or fully off; check return status to see if call was successful
      Serial.printf("Button Press Ignored\n");
  }

  void loop() override {

    // Below we set power and level once fading from a button press is completed
    
    if(ledPin->fadeStatus()==LedPin::COMPLETED){
      power->setVal(1-power->getVal());
      level->setVal(power->getVal()?100:0);
    }
  }

};
      
//////////////////////////////////

void setup() {
  
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"Fading LED");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
    
    new FadingLED(26,4);             // first argument is LED Pin, second argument is PushButton Pin
 
}

//////////////////////////////////////

void loop(){
    
  homeSpan.poll(); 
}
