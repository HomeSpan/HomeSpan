/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2021 Gregg E. Berman
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
// Example 17: Linked Services                            //
//             * implementing a multi-head Spa Shower     //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 

  // HAP normally treats multiple Services created within the same Accessory as independent of one another.  However, certain HAP Services are designed to represent a central point
  // of control over other, more typical Services.  For example, you can create an Accessory with one or more Valve Services, each operating independently.  But HAP also includes a
  // Faucet Service that can be used to "control" one or more Valve Services.  This is done by LINKING the Faucet Service to one or more Valve Services.
  //
  // Only a few types of HAP Services allow/require links to be made to other Services, and only a few types of Services can be selected as a link.
  //
  // Linked Services can be created in HomeSpan using the addLink() method.  For example, if spaShower is a pointer to a Faucet Service, and showerHead and handSprayer are both
  // pointers to Valve Services, you can link the faucet to the valves as follows:
  //
  //   spaShower->addLink(showerHead);
  //   spaShower->addLink(handSprayer);
  //
  // The addLink method returns a pointer to the object that called it, which provides you with the option of combining both methods above into a single line as follows:
  //
  //   spaShower->addLink(showerHead)->addLink(handSprayer);
  //

  // Note that HAP does *not* provide any of the actual logic that's needed for the "controlling" Service to operate the "linked" Services.  This must still be programmed by the user.
  // More so, the logic needs to conform with the behavior HAP expects for the Service as outlined in the HAP documention for the controlling Service.  The only thing HAP really does with
  // Linked Services, besides making you do extra work, is to provide a customized Tile that shows you the controlling Service and the Services to which it is linked.

  // Also as noted above, only a few Services support the Linked Services protcol.  If you use the addLink() method with Services that do not support linkages, HAP will simply ignore
  // the linkage request.  But the reverse is not true.  If you implement a Service that requires other Linked Services (such as a Faucet) you MUST create those linkages for the 
  // Service to operate properly.
  
  // Example 17 below demonstrates Linked Services by implementing a multi-head Spa Shower using one HAP Faucet Service and muliple HAP Valve Services.  As usual, we will create
  // our own "child" Services from HAP's Faucet and Valve Services so we can add the logic required to implement our device.  However, instead of placing all that logic in separate
  // *.h files, we include them directly in the main sketch file (below) to illustrate an alternative way of organizing your sketch code.

  // This Example further illustrates yet another coding style option: instead of instantiating all the Services needed in the setup() portion of the sketch, we only instantiate
  // the Shower Service, and have the Shower Service itself instantiate all the Valve Services.  In fact, our entire definition of the Value Service is fully encapsulated
  // in the definition of the Shower Service.
  
  // This hopefully provides a good example of the flexibility of HomeSpan.  Because all HomeSpan components are defined using standard C++ structures (as opposed to external files
  // based on some pre-defined format), you can choose whatever coding style you'd like.  The style below was chosen since it seemed to fit well for illustating how Linked Services work.
  // But note that it is only the addLink() method that creates the actual linkages.  The fact that the WaterValve Service is defined within the Shower Service is purely a style choice
  // and does not itself create the linkages.  We could have used a standalone structure for the WaterValve definitions and the results would be the same.

//////////////////////////////////////
  
struct Shower:Service::Faucet{                 // this is our Shower structure, which we define as a child class of the HomeSpan Faucet structure

  SpanCharacteristic *active=new Characteristic::Active();      // our implementation only requires the Active Characteristic

  Shower(int nHeads){                  // this is the constructor for Shower.  It takes a single argument that specifies the number of spray heads (WaterValves)
    for(int i=0;i<nHeads;i++)             // for each spray head needed ---
      addLink(new WaterValve(this));      // --- instantiate a new WaterValue AND link it to the Shower.  Also, pass the Shower object's pointer to WaterValve constructor.  We'll see why below.
  }
  
  struct WaterValve:Service::Valve{                               // here we define our WaterValve structure as a child class of the HomeSpan Valve Service
    SpanCharacteristic *active=new Characteristic::Active();;     // the Active Characteristic is used to specify whether the Valve is Active (open) or Inactive (closed)
    SpanCharacteristic *inUse=new Characteristic::InUse();        // the InUser Characteristic is used to specify whether water is actually flowing through value
    Shower *shower;                                               // storage the pointer to the Shower Service
    
    WaterValve(Shower *s){                                        // this is constructor for WaterValve.  It takes a single argument that points to the Shower Service
      shower=s;                                                   // store the pointer to the Shower
      new Characteristic::ValveType(2);                           // specify the Value Type (2=Shower Head)
    }
    
    boolean update() override {                                   // HomeSpan calls this whenever the Home App requests a change in Valve's Active Characteristic
      if(shower->active->getVal())                                // Here's where we use the pointer to Shower.  ONLY if the Shower object itself is active---
        inUse->setVal(active->getNewVal());                       // --- do we update the InUse Characteristic to reflect a change in the status of flowing water.
      return(true);                                               // Note that the Valve itself will still change from Active to Inactive (or vice versa) regardless of the status of the Shower
    }
    
    void loop() override {                                                    // Here we check if the Shower is turned on or off, and determine if that means we need to update the Valve
      if(shower->active->getVal() && active->getVal() && !inUse->getVal())    // If the Shower is Active, and the Valve is Active, but InUse is NOT Active
        inUse->setVal(1);                                                     // set the InUse Characteristic to Active
      else if(!shower->active->getVal() && inUse->getVal())
        inUse->setVal(0);
    }

  }; // WaterValve
};

//////////////////////////////////////

void setup() {

  Serial.begin(115200);
  
  homeSpan.begin(Category::ShowerSystems,"HomeSpan Shower");

  new SpanAccessory();                                  

    new Service::AccessoryInformation();                    // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("Spa Shower");                   
      new Characteristic::Manufacturer("HomeSpan");             
      new Characteristic::SerialNumber("HSL-123");              
      new Characteristic::Model("HSL Test");                    
      new Characteristic::FirmwareRevision(HOMESPAN_VERSION);   
      new Characteristic::Identify();                           
  
    new Service::HAPProtocolInformation();                  // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");                     
  
    new Shower(4);                                          // Create a Spa Shower with 4 spray heads

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////
