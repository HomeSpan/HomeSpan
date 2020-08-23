
//////////////////////////////////
//   DEVICE-SPECIFIC SERVICES   //
//////////////////////////////////

struct DEV_Identify : Service::AccessoryInformation {

  int nBlinks;                    // number of times to blink built-in LED in identify routine
  SpanCharacteristic *identify;
  
  DEV_Identify(int nBlinks, char *name, char *model, char *manu, char *sn, char *firm, ServiceType mod=ServiceType::Regular) : Service::AccessoryInformation(mod){
    
    new Characteristic::FirmwareRevision(firm);
    new Characteristic::Manufacturer(manu);
    new Characteristic::Model(model);
    new Characteristic::Name(name);
    new Characteristic::SerialNumber(sn);    
    identify=new Characteristic::Identify();

    this->nBlinks=nBlinks;

    pinMode(LED_BUILTIN,OUTPUT);
  }


  StatusCode update(){
  
    if(identify->newValue.BOOL){
      
      for(int i=0;i<nBlinks;i++){
        digitalWrite(LED_BUILTIN,LOW);
        delay(250);
        digitalWrite(LED_BUILTIN,HIGH);
        delay(250);
      }
    }

    return(StatusCode::OK);
    
  } // update
  
};
