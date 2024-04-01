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

#include "HomeSpan.h"

CUSTOM_CHAR_TLV(DisplayOrder,136,PR+EV);
CUSTOM_CHAR_DATA(TestData,333,PR+EV);

struct HomeSpanTV : Service::Television {

    SpanCharacteristic *active = new Characteristic::Active(0);                     // TV On/Off (set to Off at start-up)
    SpanCharacteristic *activeID = new Characteristic::ActiveIdentifier(3);         // Sets HDMI 3 on start-up
    SpanCharacteristic *remoteKey = new Characteristic::RemoteKey();                // Used to receive button presses from the Remote Control widget
    SpanCharacteristic *settingsKey = new Characteristic::PowerModeSelection();     // Adds "View TV Setting" option to Selection Screen
    SpanCharacteristic *displayOrder = new Characteristic::DisplayOrder();
    SpanCharacteristic *testData = new Characteristic::TestData();
    SpanCharacteristic *tvname;

    HomeSpanTV(const char *name) : Service::Television() {
      tvname = new Characteristic::ConfiguredName(name);             // Name of TV
      Serial.printf("Configured TV: %s\n",name);

      TLV8 orderTLV;
      uint32_t order[]={5,10,6,2,1,9,11,3,18,12};

      for(int i=0;i<sizeof(order)/sizeof(uint32_t);i++){
        if(i>0)
          orderTLV.add(0);
        orderTLV.add(1,sizeof(uint32_t),(uint8_t*)(order+i));
      }

      orderTLV.print();
      displayOrder->setTLV(orderTLV);

      uint8_t blob[]={1,2,3,4,5,6,7,8,9,10,11,12};
//      testData->setData(blob,sizeof(blob));
      testData->setData(blob,1);

      new SpanUserCommand('P', "- change order of inputs", changeOrder, this);  
      new SpanUserCommand('C', "- change name of TV", setTVName, this); 
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

  static void setTVName(const char *buf, void *arg){
    HomeSpanTV *hsTV=(HomeSpanTV *)arg;
    hsTV->tvname->setString("New Name");
  }

  static void changeOrder(const char *buf, void *arg){
    HomeSpanTV *hsTV=(HomeSpanTV *)arg;

    TLV8 orderTLV;
    
    hsTV->displayOrder->getTLV(orderTLV);
    orderTLV.print();
    orderTLV.wipe();
    
    uint32_t order[]={12,10,6,2,1,9,11,3,18,5};

    for(int i=0;i<sizeof(order)/sizeof(uint32_t);i++){
      if(i>0)
        orderTLV.add(0);
      orderTLV.add(1,sizeof(uint32_t),(uint8_t*)(order+i));
    }

    Serial.printf("AFTER:\n");
    orderTLV.print();
    size_t n=orderTLV.pack_size();
    Serial.printf("Size=%d\n",n);
    uint8_t c[n];
    orderTLV.pack(c);
    hsTV->displayOrder->setData(c,n);    
  }
      
};

///////////////////////////////

void setup() {
  
  Serial.begin(115200);

  homeSpan.setLogLevel(2);
 
  homeSpan.begin(Category::Television,"HomeSpan Television");

  SPAN_ACCESSORY();
       
  SpanService *hdmi1 = new Service::InputSource();    // Source included in Selection List, but excluded from Settings Screen
    new Characteristic::ConfiguredName("Alpha");
    new Characteristic::Identifier(5);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi2 = new Service::InputSource();
    new Characteristic::ConfiguredName("Gamma");
    new Characteristic::Identifier(10);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi3 = new Service::InputSource();
    new Characteristic::ConfiguredName("Beta");
    new Characteristic::Identifier(6);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi4 = new Service::InputSource();
    new Characteristic::ConfiguredName("Zebra");
    new Characteristic::Identifier(2);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi5 = new Service::InputSource();
    new Characteristic::ConfiguredName("Delta");
    new Characteristic::Identifier(1);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi6 = new Service::InputSource();
    new Characteristic::ConfiguredName("Trident");
    new Characteristic::Identifier(9);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi7 = new Service::InputSource();
    new Characteristic::ConfiguredName("Netflix");
    new Characteristic::Identifier(11);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

  SpanService *hdmi8 = new Service::InputSource();
    new Characteristic::ConfiguredName("Alpha2");
    new Characteristic::Identifier(3);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);
   
  SpanService *hdmi9 = new Service::InputSource();
    new Characteristic::ConfiguredName("Moon");
    new Characteristic::Identifier(18);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);
    
  SpanService *hdmi10 = new Service::InputSource();
    new Characteristic::ConfiguredName("Gamba");
    new Characteristic::Identifier(12);
    new Characteristic::IsConfigured(1);
    new Characteristic::CurrentVisibilityState(0);
    new Characteristic::TargetVisibilityState(0);

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


//////////////////////////////////////

void loop(){
 
  homeSpan.poll();  
}

//////////////////////////////////////
