#include "HomeSpan.h"

struct HKTV : Service::Television {

  SpanCharacteristic *active;
  SpanCharacteristic *activeIdentifier;
  SpanCharacteristic *configuredName;
  SpanCharacteristic *remoteKey;
  SpanCharacteristic *sleepDiscoveryMode;

  SpanService *speaker;
  SpanCharacteristic *mute;
  SpanCharacteristic *speakerActive;
  SpanCharacteristic *volume;

  SpanService         *hdmi1;
  SpanCharacteristic  *hdmi1Identifier;
  SpanCharacteristic  *hdmi1CurrentVisibility;

  SpanService         *hdmi2;
  SpanCharacteristic  *hdmi2Identifier;
  SpanCharacteristic  *hdmi2CurrentVisibility;

  SpanService         *netflix;
  SpanCharacteristic  *netflixIdentifier;
  SpanCharacteristic  *netflixCurrentVisibility;

  HKTV() : Service::Television(){
    
    Serial.printf("Init TV\n");           // initialization message

    active            = new Characteristic::Active();
    activeIdentifier   = new Characteristic::ActiveIdentifier(0);
    new Characteristic::ConfiguredName("My TV");
    new Characteristic::PowerModeSelection();             // write-only Characteristic that enables "View TV Settings" on controls screen 
    new Characteristic::PictureMode(1);
    new Characteristic::ClosedCaptions();
    new Characteristic::Brightness(50);
    new Characteristic::CurrentMediaState(1);
    new Characteristic::TargetMediaState(1);
    new Characteristic::RemoteKey();
    new Characteristic::SleepDiscoveryMode(1);

    hdmi1 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 1");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      netflixIdentifier = new Characteristic::Identifier(0);
      hdmi1CurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    hdmi2 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 2");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      hdmi2Identifier = new Characteristic::Identifier(10);
      hdmi2CurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    netflix = new Service::InputSource();
      new Characteristic::ConfiguredName("NETFLIX");
//      new Characteristic::IsConfigured(0); // configured = 1, not configured = 0
      netflixIdentifier = new Characteristic::Identifier(4);
      netflixCurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden
//      new Characteristic::TargetVisibilityState(0);  // 0 shown, 1 hidden

    addLink(hdmi1);
    addLink(hdmi2);
    addLink(netflix);
    addLink(speaker);
  }

  boolean update(){
    if(active->updated()) {
        if(active->getVal() != active->getNewVal()) {
          Serial.printf("update(): active %d -> %d\n", active->getVal(), active->getNewVal());
        }
    }

    if(activeIdentifier->updated()) {
      Serial.printf("update(): activeIdentifier %d -> %d\n", activeIdentifier->getVal(), activeIdentifier->getNewVal());
    }

    return true;
  }

  void loop() {

  }
};

struct TV_Source : Service::InputSource{

  SpanCharacteristic *currentState = new Characteristic::CurrentVisibilityState(0,true);
  SpanCharacteristic *targetState = new Characteristic::TargetVisibilityState(0,true);
  SpanCharacteristic *configName =  new Characteristic::ConfiguredName("HDMI 12",true);

  TV_Source() : Service::InputSource(){
    new Characteristic::Identifier(12);
    new Characteristic::IsConfigured(1);
  }

  boolean update() override{

    char c[50];
    sprintf(c,"HERE I AM ");

    if(targetState->updated()){
      Serial.printf("Old Target State = %d    New Target State = %d\n",targetState->getVal(),targetState->getNewVal());
      currentState->setVal(targetState->getNewVal());
      Serial.printf("Name: %s\n",configName->getString());
      configName->setString(c);
      Serial.printf("Name: %s\n",configName->getString());
    }

    if(configName->updated()){
      Serial.printf("CURRENT NAME: %s   NEW NAME:  %s\n",configName->getString(),configName->getNewString());
    }

    return(true);
  }
  
};


void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Television,"HomeSpan Television");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("HomeSpan TV");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("HomeSpan");
      new Characteristic::FirmwareRevision("0.1");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");
       
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
    
  (new Service::Television())
    ->addLink(hdmi1)
    ->addLink(hdmi2)
    ->addLink(hdmi3)
    ->addLink(hdmi4)
    ->addLink(hdmi5)
    ->addLink(hdmi6)
    ->addLink(hdmi7)
    ->addLink(hdmi8)
    ->addLink(hdmi9)
    ;
    new Characteristic::Active(0);                  // TV On/Off (set to Off at start-up)
    new Characteristic::ConfiguredName("Test TV");  // Name of TV
    new Characteristic::ActiveIdentifier(3);        // Sets HDMI 3 on start-up
    new Characteristic::RemoteKey();                // Used to receive button presses from the Remote Control widget
    new Characteristic::PowerModeSelection();       // Adds "" option to Selection Screen
       
}

void loop() {
  homeSpan.poll();
}
