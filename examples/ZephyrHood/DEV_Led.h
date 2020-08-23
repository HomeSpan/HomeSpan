
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include <analogWrite.h>

struct DEV_DimmableLED : Service::LightBulb {

  // Controls a Dimmable LED connect to pin 'ledPin'

  int ledPin;
  SpanCharacteristic *power;
  SpanCharacteristic *brightness;

  DEV_DimmableLED(int ledPin, ServiceType mod=ServiceType::Regular) : Service::LightBulb(mod){

    power=new Characteristic::On();
    brightness=new Characteristic::Brightness(50);
    
    brightness->range = new SpanRange(20,100,1);

    this->ledPin=ledPin;

    Serial.print("Configuring Dimmable LED on Pin: ");
    Serial.println(ledPin);
    pinMode(ledPin,OUTPUT);
  }

  StatusCode update(){

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin);
    LOG1(":  Power=");
    LOG1(power->value.BOOL?"true":"false");
    LOG1("  Brightness=");
    LOG1(brightness->value.INT);
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->newValue.BOOL?"true":"false");
    }

    if(brightness->isUpdated){
      LOG1("  New Brightness=");
      LOG1(brightness->newValue.INT);
    } 

    LOG1("\n");

    analogWrite(ledPin,power->newValue.BOOL*brightness->newValue.INT,100);    
   
    return(StatusCode::OK);
  
  } // update
};
      
//////////////////////////////////
