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

////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 21: Using the Identify Characteristic          //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

// This sketch is similar to Example 5, in which we implemented two simple Lightbulb Accessories, 
// except now we will also add functionality for the Identify Characteristic (we will also configure
// the device as a Bridge Accessory, instead of two standalone Accessories).

// Recall that the Identify Characteristic has been instantiated in every example sketch since it
// is a required Characteristic of the AccessoryInformation Service, and that Service is itself
// required to be present for every Accessory.  Thus, every Accessory (including the Bridge
// Accessory if used), has its own instant of the Identify Characteristic.

// Though not typically used during normal operation of an Accessory, the Identify Characteristic
// can be useful when first pairing your device to HomeKit.  You may have noticed when pairing your
// device using the Home App that there is the word "Identify" at the bottom of each of the screens
// that ask you what you want to name each Accessory, what room the Accessory should be assigned to, etc.

// Clicking "Identify" on any of those screens causes HomeKit to send an update request to the
// Identify Characteristic associated with the corresponding Accessory.  As with any Characteristic that
// is updated via the Home App, this will trigger a call to the update() method for the enclosing Service.

// The purpose of this is so that your device can run some sort of "identification routine" when requested,
// allowing you to visually confirm that you are indeed pairing the correct device.  For example, if you
// have three separate devices wired to three different lights or appliances, you want to make sure that when
// you start pairing each of them to the Home App you are connected to the device you intend.

// The identification routine can be anything you choose.  The only HAP requirement is that it should not take
// longer than 5 seconds to run.  In the sketch below we have created an identification routine that logs a
// message to the Serial Monitor and blinks the LED associated with the Accessory 3 times whenever its
// Identify Characteristic is updated.

#include "HomeSpan.h" 

//////////////////////////////////////

// Below is the same DEV_LED Lightbulb Service we've used in many of the previous examples

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
    LOG0("LED %d: Power %s\n",ledPin,power->getNewVal()?"ON":"OFF");
    return(true);
  }
};

//////////////////////////////////////

// NEW: Here we derive a new class, DEV_INFO, from the Accessory Information Service

// This structure takes a single argument (ledPin), creates a name from it, and assigns
// it to the Name Characteristic.

// It also instantiates the required Identify Characteristic, and implements an update() method
// that logs a message to the Serial Monitor and blinks the associated LED three times.

// Note that in the update() method we do not bother to check which Characteristic has been updated.
// This is because the only possibility is the Identify Characteristic.

// Also, we do not need to use getNewVal() to check the value.  The Home App always sends a value of 1,
// since it is just trying to trigger the identification routine (the value itself is meaningless).

struct DEV_INFO : Service::AccessoryInformation {

  int ledPin;
  
  DEV_INFO(int ledPin) : Service::AccessoryInformation(){

    new Characteristic::Identify();
    char c[64];
    sprintf(c,"LED-%d",ledPin);
    new Characteristic::Name(c);               
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);    
  }

  boolean update(){
    LOG0("Running Identification for LED %d\n",ledPin);
    for(int i=0;i<3;i++){
      digitalWrite(ledPin,HIGH);
      delay(500);
      digitalWrite(ledPin,LOW);
      delay(500);
    }
    return(true);
  }
};

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  homeSpan.setLogLevel(1);
  homeSpan.begin(Category::Lighting,"HomeSpan LEDS");

// Here we replace the usual construct:

//   new SpanAccessory();
//    new Service::AccessoryInformation();  
//      new Characteristic::Identify();

// with this:

  new SpanAccessory();  
    new DEV_INFO(13);         // instantiate a new DEV_INFO structure that will run our custom identification routine to blink an LED on pin 13 three times

  new SpanAccessory();
    new DEV_INFO(16);         // Note we instantiate a new DEV_INFO structure for each Accessory in this device
    new DEV_LED(16);          // Here we instantiate the usual DEV_LED structure that controls the LED during normal operation

  new SpanAccessory();        // Here we add a second LED Accessory
    new DEV_INFO(17);               
    new DEV_LED(17);    
}

//////////////////////////////////////

void loop(){ 
  homeSpan.poll();
}

//////////////////////////////////////

// NOTE:  Once a device has been paired, it is no longer possible to trigger the Identify Characteristic from the Home App.
// Apple assumes that the identification routine is no longer needed since you can always identify the device by simply operating it.
// However, the Eve for HomeKit app DOES provide an "ID" button in the interface for each Accessory that can be used to trigger
// the identification routine for that Accessory at any time after the device has been paired.
