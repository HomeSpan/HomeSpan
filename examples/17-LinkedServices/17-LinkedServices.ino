/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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

  // The HAP Valve Service requires both an Active Characteristic and an InUse Characteristic.  The Active Characteristic controls whether a Valve is open (active) or closed (inactive).
  // This Characteristic is normally controlled by the user through the Home App.  The InUse Characteristic specifies whether there is water (or gas, etc.) actually flowing
  // through the Valve.  This is because opening a Valve does not necessarily mean water will be flowing.  There may be another real-world "master" Valve that also needs to be open
  // before water can begin flowing. Or there may be another Service that must also be Active to enable water to flow through the Valve.  Hence, InUse can either be true or false
  // if the Valve is open, but it can only be false if the Valve is closed. The Home App cannot change the InUse Characteristic.  It is only read by the Home App as a status.

  // It is possible to create a multi-valve Accessory where each Valve is controlled independently from the Home App, and HomeSpan uses internal logic to determine, based
  // on the combination of Valves that are open or closed, which Valves have water flowing (InUse=true) and which do not (InUse=false).

  // The HAP Faucet Service is used to create a "central control switch" for all the Valves linked to it.  The Home App displays each Valve as a small icon on the control
  // page of the Faucet.  Clicking a Valve icon toggles it open/close, and changes the icon accordingly.  However, water is not supposed to flow unless the Shower control switch
  // itself is also turned on.  Thus, the logic you need to encode to implement a HAP Faucet is to set the InUse Characteristic of a Valve to true ONLY if the Valve is open
  // AND the Shower is switched on.  If the Shower is then switched off, the Valve remains open, but the InUse Characteristic needs to be reset to false.  Similarly, if the Shower
  // is switched back on, the InUse Characteristic of each Valve that is open needs to be set to true.  This mimics how an actual Shower with a central controlling switch
  // would operate.

  // In addition, the Home App displays one of 4 status messages as you operate the Shower and Valve controls:

  // OFF:      The Shower switch is OFF, AND the InUse Characteristic for EVERY Valve is set to FALSE (no water flowing anywhere);
  // STOPPING: The Shower switch is OFF, BUT at least one Valve still has its InUse Characteristic set to TRUE.  Presumably this means the Valve is in the process of turning off;
  // STARTING: The Shower switch is ON, BUT the InUse Characteristic for EVERY Valve is set to FALSE.  This indicates the Shower is waiting for water to start flowing;
  // RUNNING:  The Shower switch in ON, AND at least one of the Valves has its InUse Characteristic set to TRUE.  This indicates water is flowing.

  // Note that the Shower Service only monitors the InUse Characteristics of its Linked Valves. It does not monitor the Active Characteristics of the Linked Valves.  Also, turning
  // on and off the Shower Switch should NOT change the Active Characteristic of any Valve.  Below is the code that implements all of this HAP-required logic:
  
struct Shower : Service::Faucet {                 // this is our Shower structure, which we define as a child class of the HomeSpan Faucet structure

  SpanCharacteristic *active=new Characteristic::Active();      // our implementation only requires the Active Characteristic

  Shower(int nHeads){                   // this is the constructor for Shower.  It takes a single argument that specifies the number of spray heads (WaterValves)
    for(int i=0;i<nHeads;i++)           // for each spray head needed ---
      addLink(new WaterValve(this));    // --- instantiate a new WaterValve AND link it to the Shower.  Also, pass the Shower object's pointer to WaterValve constructor.  We'll see why below.
  }
  
  struct WaterValve : Service::Valve {                            // here we define our WaterValve structure as a child class of the HomeSpan Valve Service
    SpanCharacteristic *active=new Characteristic::Active(1);;    // the Active Characteristic is used to specify whether the Valve is Active (open) or Inactive (closed)
    SpanCharacteristic *inUse=new Characteristic::InUse();        // the InUser Characteristic is used to specify whether water is actually flowing through value
    Shower *shower;                                               // storage for the pointer to the "controlling" Shower Service
    
    WaterValve(Shower *s){                                                      // this is constructor for WaterValve.  It takes a single argument that points to the "controlling" Shower Service
      shower=s;                                                                 // store the pointer to the Shower Service
      new Characteristic::ValveType(Characteristic::ValveType::SHOWER_HEAD);    // specify the Valve Type as a Shower Head (note use of constant "Characteristic::ValveType::SHOWER_HEAD")
    }
    
    boolean update() override {                                   // HomeSpan calls this whenever the Home App requests a change in a Valve's Active Characteristic
      if(shower->active->getVal())                                // here's where we use the pointer to Shower: ONLY if the Shower object itself is active---
        inUse->setVal(active->getNewVal());                       // --- do we update the InUse Characteristic to reflect a change in the status of flowing water.
      return(true);                                               // Note that the Valve itself will still change from Active to Inactive (or vice versa) regardless of the status of the Shower
    }
    
    void loop() override {                                                    // Here we check if the Shower is turned on or off, and determine if that means we need to update the Valve
      if(shower->active->getVal() && active->getVal() && !inUse->getVal())    // If the Shower is Active, and the Valve is Active but NOT showing InUse...
        inUse->setVal(1);                                                     // ...show Valve as InUse
      else if(!shower->active->getVal() && inUse->getVal())                   // Otherwise, if the Shower is NOT Active but Valve IS showing InUse...
        inUse->setVal(0);                                                     // ...show Valve as NOT InUse
    }

  }; // WaterValve
};

//////////////////////////////////////

void setup() {

  Serial.begin(115200);
  
  homeSpan.begin(Category::ShowerSystems,"HomeSpan Shower");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify();                    
    
    new Shower(4);                                          // Create a Spa Shower with 4 spray heads

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////
