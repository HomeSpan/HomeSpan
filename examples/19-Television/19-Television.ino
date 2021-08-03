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
    Serial.print("Init HKTV\n");           // initialization message

    active            = new Characteristic::Active();
    activeIdentifier   = new Characteristic::ActiveIdentifier(1);
    configuredName     = new Characteristic::ConfiguredName();
    remoteKey          = new Characteristic::RemoteKey();
    sleepDiscoveryMode = new Characteristic::SleepDiscoveryMode(1);

    speaker = new Service::TelevisionSpeaker();
      mute = new Characteristic::Mute();
      speakerActive = new Characteristic::Active();
      volume = new Characteristic::Volume();
      new Characteristic::VolumeControlType(3);
      new Characteristic::VolumeSelector();

    hdmi1 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 1");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      new Characteristic::InputSourceType(HDMI);
      new Characteristic::Name("HDMI 1");
      netflixIdentifier = new Characteristic::Identifier(0);
      hdmi1CurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    hdmi2 = new Service::InputSource();
      new Characteristic::ConfiguredName("HDMI 2");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      new Characteristic::InputSourceType(HDMI);
      new Characteristic::Name("HDMI 2");
      hdmi2Identifier = new Characteristic::Identifier(1);
      hdmi2CurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    netflix = new Service::InputSource();
      new Characteristic::ConfiguredName("NETFLIX");
      new Characteristic::IsConfigured(1); // configured = 1, not configured = 0
      new Characteristic::InputSourceType(APPLICATION);
      new Characteristic::Name("Netflix");
      netflixIdentifier = new Characteristic::Identifier(2);
      netflixCurrentVisibility = new Characteristic::CurrentVisibilityState(0);  // 0 shown, 1 hidden

    addLink(hdmi1);
    addLink(hdmi2);
    addLink(netflix);
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
}

void loop() {
  homeSpan.poll();
}
