/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2021-2022 Gregg E. Berman
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
 
 // HomeSpan Television Service Example

// Covers all Characteristics of the Television Service that appear to
// be supported in the iOS 15 version of the Home App.  Note these Services
// are not documented by Apple and are not officially part HAP-R2.
//
// For Service::Television():
//
//    * Characteristic::Active()
//    * Characteristic::ConfiguredName()
//    * Characteristic::ActiveIdentifier()
//    * Characteristic::RemoteKey()
//    * Characteristic::PowerModeSelection()
//
// For Service::InputSource():
//
//    * Characteristic::ConfiguredName()
//    * Characteristic::ConfiguredNameStatic()        // a HomeSpan-specific variation of ConfiguredName()
//    * Characteristic::Identifier()
//    * Characteristic::IsConfigured()
//    * Characteristic::CurrentVisibilityState()
//    * Characteristic::TargetVisibilityState()

// NOTE: This example is only designed to demonstrate how Television Services and Characteristics
// appear in the Home App, and what they each control.  To keep things simple, actions for the
// Input Sources have NOT been implemented in the code below.  For example, the code below does not include
// any logic to update CurrentVisibilityState when the TargetVisibilityState checkboxes are clicked.

#include "HomeSpan.h"

struct HomeSpanTV : Service::Television {

    SpanCharacteristic *active = new Characteristic::Active(0);                     // TV On/Off (set to Off at start-up)
    SpanCharacteristic *activeID = new Characteristic::ActiveIdentifier(3);         // Sets HDMI 3 on start-up
    SpanCharacteristic *remoteKey = new Characteristic::RemoteKey();                // Used to receive button presses from the Remote Control widget
    SpanCharacteristic *settingsKey = new Characteristic::PowerModeSelection();     // Adds "View TV Setting" option to Selection Screen  

    HomeSpanTV(const char *name) : Service::Television() {
      new Characteristic::ConfiguredName(name);             // Name of TV
      Serial.printf("Configured TV: %s\n",name);
    }

    boolean update() override {

      if(active->updated()){
        Serial.printf("Set TV Power to: %s\n",active->getNewVal()?"ON":"OFF");
      }

      if(activeID->updated()){
        Serial.printf("Set Input Source to HDMI-%d\n",activeID->getNewVal());        
      }

      if(settingsKey->updated()){
        Serial.printf("Received request to \"View TV Settings\"\n");
      }
      
      if(remoteKey->updated()){
        Serial.printf("Remote Control key pressed: ");
        switch(remoteKey->getNewVal()){
          case 4:
            Serial.printf("UP ARROW\n");
            break;
          case 5:
            Serial.printf("DOWN ARROW\n");
            break;
          case 6:
            Serial.printf("LEFT ARROW\n");
            break;
          case 7:
            Serial.printf("RIGHT ARROW\n");
            break;
          case 8:
            Serial.printf("SELECT\n");
            break;
          case 9:
            Serial.printf("BACK\n");
            break;
          case 11:
            Serial.printf("PLAY/PAUSE\n");
            break;
          case 15:
            Serial.printf("INFO\n");
            break;
          default:
            Serial.print("UNKNOWN KEY\n");
        }
      }

      return(true);
    }
};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);
 
  homeSpan.begin(Category::Television,"HomeSpan Television");

  SPAN_ACCESSORY();

  // Below we define 10 different InputSource Services using different combinations
  // of Characteristics to demonstrate how they interact and appear to the user in the Home App
       
  SpanService *hdmi1 = new Service::InputSource();    // Source included in Selection List, but excluded from Settings Screen
    new Characteristic::ConfiguredName("HDMI 1");
    new Characteristic::Identifier(1);

  SpanService *hdmi2 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 2");
    new Characteristic::Identifier(2);
    new Characteristic::IsConfigured(0);              // Source excluded from both the Selection List and the Settings Screen

  SpanService *hdmi3 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 3");
    new Characteristic::Identifier(3);
    new Characteristic::IsConfigured(1);              // Source included in both the Selection List and the Settings Screen

  SpanService *hdmi4 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 4");
    new Characteristic::Identifier(4);
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(1);    // ...but excluded from the Selection List 

  SpanService *hdmi5 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 5");
    new Characteristic::Identifier(5);
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(0);    // ...and included in the Selection List 

  SpanService *hdmi6 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 6");
    new Characteristic::Identifier(6);
    new Characteristic::IsConfigured(0);              // Source excluded from both the Selection List and the Settings Screen
    new Characteristic::CurrentVisibilityState(0);    // If IsConfigured(0) is specified, CurrentVisibilityState() has no effect

  SpanService *hdmi7 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 7");
    new Characteristic::Identifier(7);
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(0);    // ...and included in the Selection List...
    new Characteristic::TargetVisibilityState(0);     // ...and a "checked" checkbox is provided on the Settings Screen that can be used to toggle CurrentVisibilityState()

  SpanService *hdmi8 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 8");
    new Characteristic::Identifier(8);
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(1);    // ...but excluded from the Selection List...
    new Characteristic::TargetVisibilityState(1);     // ...and an "un-checked" checkbox is provided on the Settings Screen that can be used to toggle CurrentVisibilityState()    
   
  SpanService *hdmi9 = new Service::InputSource();
    new Characteristic::ConfiguredName("HDMI 9");
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(0);    // ...but without an Identifier() set, the Source is excluded from the Selection List regardless of CurrentVisibilityState(0)
    new Characteristic::TargetVisibilityState(0);
    
  SpanService *hdmi10 = new Service::InputSource();
    (new Characteristic::ConfiguredName("HDMI 10"))->removePerms(PW);    // Source Name permissions changed and now cannot be edited in Settings Screen
    new Characteristic::Identifier(10);
    new Characteristic::IsConfigured(1);              // Source included in the Settings Screen...
    new Characteristic::CurrentVisibilityState(0);    // ...and included in the Selection List...
    new Characteristic::TargetVisibilityState(0);     // ...and a "checked" checkbox is provided on the Settings Screen that can be used to toggle CurrentVisibilityState()

  SpanService *speaker = new Service::TelevisionSpeaker();
    new Characteristic::VolumeSelector();
    new Characteristic::VolumeControlType(3);

  (new HomeSpanTV("Test TV"))                         // Define a Television Service.  Must link in InputSources!
    ->addLink(hdmi1)
    ->addLink(hdmi2)
    ->addLink(hdmi3)
    ->addLink(hdmi4)
    ->addLink(hdmi5)
    ->addLink(hdmi6)
    ->addLink(hdmi7)
    ->addLink(hdmi8)
    ->addLink(hdmi9)
    ->addLink(hdmi10)
    ->addLink(speaker)
    ;
      
}

///////////////////////////////

void loop() {
  homeSpan.poll();
}
