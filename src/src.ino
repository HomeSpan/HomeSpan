
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.

#include "HomeSpan.h"

CUSTOM_CHAR(CustomActive, E863F10A-079E-48FF-8F27-9C2605A29F52, PR+EV, UINT16, 0, 0, 4800, false);

void setup() {
 
  Serial.begin(115200);
 
  homeSpan.setLogLevel(2);
//  homeSpan.setStatusPin(13);
//  homeSpan.setControlPin(33);
  
  homeSpan.setHostNameSuffix("-lamp1");
  homeSpan.setPortNum(1201);
//  homeSpan.setMaxConnections(6);
//  homeSpan.setQRID("One1");
//  homeSpan.enableOTA();
  homeSpan.setSketchVersion("Test 1.3.1");
  homeSpan.setWifiCallback(wifiEstablished);

  new SpanUserCommand('d',"- My Description",userCom1);
  new SpanUserCommand('e',"- My second Description",userCom2);

//  homeSpan.enableAutoStartAP();
//  homeSpan.setApFunction(myWiFiAP);

  homeSpan.begin(Category::Lighting,"HomeSpan Lamp Server","homespan");

  new SpanAccessory();                                  // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments

    new Service::AccessoryInformation();                    // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("HomeSpan Test");                // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                            
      new Characteristic::Manufacturer("HomeSpan");             // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("HSL-123");              // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("HSL Test");                    // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision(HOMESPAN_VERSION);   // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)  
      new Characteristic::Identify();                           // Create the required Identify
  
    new Service::HAPProtocolInformation();                  // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");                     // Set the Version Characteristic to "1.1.0" as required by HAP

    new Service::LightBulb();
      new Characteristic::On(0);
      new Characteristic::CustomActive(1200);
      new Characteristic::Brightness(50);
      new Characteristic::Name("Light 1");
      new Characteristic::ColorTemperature();
      new Characteristic::Active();
    new Service::LightBulb();
      new Characteristic::On(0,true);
      (new Characteristic::Brightness(50,false))->setRange(10,100,5);
      new Characteristic::Name("Light 2");

  new SpanAccessory();                                  // Begin by creating a new Accessory using SpanAccessory(), which takes no arguments

    new Service::AccessoryInformation();                    // HAP requires every Accessory to implement an AccessoryInformation Service, which has 6 required Characteristics
      new Characteristic::Name("HomeSpan Test");                // Name of the Accessory, which shows up on the HomeKit "tiles", and should be unique across Accessories                                                            
      new Characteristic::Manufacturer("HomeSpan");             // Manufacturer of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::SerialNumber("HSL-123");              // Serial Number of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::Model("HSL Test");                    // Model of the Accessory (arbitrary text string, and can be the same for every Accessory)
      new Characteristic::FirmwareRevision(HOMESPAN_VERSION);   // Firmware of the Accessory (arbitrary text string, and can be the same for every Accessory)  
      new Characteristic::Identify();                           // Create the required Identify      

    new Service::HAPProtocolInformation();                  // Create the HAP Protcol Information Service  
      new Characteristic::Version("1.1.0");                     // Set the Version Characteristic to "1.1.0" as required by HAP

    new Service::LightBulb();
      new Characteristic::On(0,true);
      (new Characteristic::Brightness(50,true))->setRange(10,100,5);
      new Characteristic::Name("Light 3");
      new Characteristic::TargetPosition();
      new Characteristic::OzoneDensity();
      (new Characteristic::OzoneDensity())->addPerms(PW|AA)->removePerms(EV|PR);

} // end of setup()

//////////////////////////////////////

void loop(){

  homeSpan.poll();

} // end of loop()

//////////////////////////////////////

void myWiFiAP(){
  Serial.print("Calling My WIFI AP\n\n");
  homeSpan.setWifiCredentials("MY_NETWORK","MY_PASSWORD");
}

//////////////////////////////////////

void wifiEstablished(){
  Serial.print("IN CALLBACK FUNCTION\n\n");
}

//////////////////////////////////////

void userCom1(const char *v){
  Serial.printf("In User Command 1: '%s'\n\n",v);
}

//////////////////////////////////////

void userCom2(const char *v){
  Serial.printf("In User Command 2: '%s'\n\n",v);
}
