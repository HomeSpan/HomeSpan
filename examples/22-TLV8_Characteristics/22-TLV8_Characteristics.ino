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

////////////////////////////////////////////////////////////////
//                                                            //
//      HomeSpan: A HomeKit implementation for the ESP32      //
//      ------------------------------------------------      //
//                                                            //
// Example 22: Demonstrates the use of the TLV8 Library       //
//             by implementing DisplayOrder, an optional      //
//             TLV8 Characteristic used with the TV Service   //
//             to set the order in which TV Inputs are        //
//             displayed for selection in the Home App        //
//                                                            //
////////////////////////////////////////////////////////////////

#include "HomeSpan.h"

// NOTE: Please see the "Other Examples -> Television" sketch for complete details on how to implement a Television Service.  The focus
// of this sketch is solely to demonstrate how to use the TLV8 Library to create TLV8 data for use with the DisplayOrder Characteristic.

// First we define a simple Television Input Source Service

struct TVInput : Service::InputSource {

  SpanCharacteristic *inputID;
  SpanCharacteristic *inputName;
  
  TVInput(uint32_t id, const char *name) : Service::InputSource() {

     inputID = new Characteristic::Identifier(id);
     inputName = new Characteristic::ConfiguredName(name);
     new Characteristic::IsConfigured(Characteristic::IsConfigured::CONFIGURED);
     new Characteristic::CurrentVisibilityState(Characteristic::CurrentVisibilityState::VISIBLE);
  }
};

// Next we define a simple Television Service

struct HomeSpanTV : Service::Television {

  SpanCharacteristic *active = new Characteristic::Active(0);
  SpanCharacteristic *activeID = new Characteristic::ActiveIdentifier(10);
  
  SpanCharacteristic *displayOrder;       //  Create a pointer to use for the new TLV8 DisplayOrder Characteristic, which will be instantiated below once we build the TLV8 record

  HomeSpanTV() : Service::Television() {

    // Before we instantiate displayOrder, we need to build a TLV8 object with the information required
    // by the DisplayOrder Characteristic.  The (undocumented by Apple!) TLV8 specifications for the
    // DisplayOrder Characteristic are as follows:

    // TAG     NAME             FORMAT     DESCRIPTION
    // ----    -------------    ------     --------------------------------------------
    // 0x01    inputSourceID    uint32     ID of the Input Source to be displayed first
    // 0x00    separator        none       Empty element to separate the inputSourceIDs
    // 0x01    inputSourceID    uint32     ID of the Input Source to be displayed second
    // 0x00    separator        none       Empty element to separate the inputSourceIDs
    // 0x01    inputSourceID    uint32     ID of the Input Source to be displayed third
    // 0x00    separator        none       Empty element to separate the inputSourceIDs
    // etc...

    // To start, instantiate a new TLV8 object
           
    TLV8 orderTLV;              // creates an empty TLV8 object

    // Next, fill it with TAGS and VALUES based on the above specification.  The easiest, though
    // not necessarily most elegant, way to do this is by simply adding each TAG/VALUE as follows:

    orderTLV.add(1,10);        // TAG=1, VALUE=ID of first Input Source to be displayed
    orderTLV.add(0);           // TAG=0  (no value)
    orderTLV.add(1,20);        // TAG=1, VALUE=ID of the second Input Source to be displayed
    orderTLV.add(0);           // TAG=0  (no value)
    orderTLV.add(1,50);        // TAG=1, VALUE=ID of the third Input Source to be displayed
    orderTLV.add(0);           // TAG=0  (no value)
    orderTLV.add(1,30);        // TAG=1, VALUE=ID of the fourth Input Source to be displayed
    orderTLV.add(0);           // TAG=0  (no value)
    orderTLV.add(1,40);        // TAG=1, VALUE=ID of the fifth Input Source to be displayed

    // Based on the above structure, we expect the Home App to display our input sources based on their IDs
    // in the following order: 10, 20, 50, 30, 40.  These IDs must of course match the IDs you choose
    // for your input sources when you create them at the end of this sketch in setup()

    // Now we can instantiate displayOrder using the TLV8 object created above as its initial value

    displayOrder = new Characteristic::DisplayOrder(orderTLV);           // set the "value" of DisplayOrder to be the orderTLV object we just created

    // That's it - you've created your first TLV8 Characteristic!      
  }

  // Below we define the usual update() loop.  There is nothing "TLV-specific" about this part of the code
  
  boolean update() override {

    if(active->updated()){
      LOG0("Set TV Power to: %s\n",active->getNewVal()?"ON":"OFF");
    }

    if(activeID->updated()){
      LOG0("Set Input Source to ID=%d\n",activeID->getNewVal());        
    }      

  return(true);
  }
};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);

  homeSpan.setLogLevel(2);
 
  homeSpan.begin(Category::Television,"HomeSpan Television");

  SPAN_ACCESSORY();      

  (new HomeSpanTV())                             // Define a Television Service and link in the InputSources!
    ->addLink(new TVInput(10,"Xfinity"))
    ->addLink(new TVInput(20,"BlueRay Disc"))
    ->addLink(new TVInput(30,"Amazon Prime"))
    ->addLink(new TVInput(40,"Netflix"))
    ->addLink(new TVInput(50,"Hulu"))
    ;
      
}

//////////////////////////////////////

void loop(){ 
  homeSpan.poll();
}

//////////////////////////////////////
