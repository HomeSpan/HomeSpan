#include "HomeSpan.h"

const int OTHER = 0;
const int HOME_SCREEN = 1;
const int TUNER = 2;
const int HDMI = 3;
const int COMPOSITE_VIDEO = 4;
const int S_VIDEO = 5;
const int COMPONENT_VIDEO = 6;
const int DVI = 7;
const int AIRPLAY = 8;
const int USB = 9;
const int APPLICATION = 10;

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

    speaker = new Service::TelevisionSpeaker();
      mute = new Characteristic::Mute();
      speakerActive = new Characteristic::Active(1);
      volume = new Characteristic::Volume();
      new Characteristic::VolumeControlType(1);
      new Characteristic::VolumeSelector(1);

    hdmi1 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 1");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      new Characteristic::InputSourceType(HDMI);
      netflixIdentifier = new Characteristic::Identifier(0);
      hdmi1CurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    hdmi2 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 2");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      new Characteristic::InputSourceType(HDMI);
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

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Television,"HomeSpan Television");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("Test HKTV");
      new Characteristic::Manufacturer("HomeSpan");
      new Characteristic::SerialNumber("123-ABC");
      new Characteristic::Model("HomeSpan");
      new Characteristic::FirmwareRevision("0.1");
      new Characteristic::Identify();

  new Service::HAPProtocolInformation();
    new Characteristic::Version("1.1.0");  

  new HKTV();

    new Service::Speaker();
      new Characteristic::Mute();
//      new Characteristic::Active(1);
      new Characteristic::Volume();
//      new Characteristic::VolumeControlType(1);
//      new Characteristic::VolumeSelector(1);
        
}

void loop() {
  homeSpan.poll();
}
