
//////////////////////////////////
//   DEVICE-SPECIFIC SERVICES   //
//////////////////////////////////

struct DEV_Identify : Service::AccessoryInformation {

  int nBlinks;                    // number of times to blink built-in LED in identify routine
  SpanCharacteristic *identify;   // reference to the Identify Characteristic
  
  DEV_Identify(char *name, char *manu, char *sn, char *model, char *version, int nBlinks) : Service::AccessoryInformation(){
    
    new Characteristic::Name(name);                   // create all the required Characteristics with values set based on above arguments
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);    
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify=new Characteristic::Identify();          // store a reference to the Identify Characteristic for use below

    this->nBlinks=nBlinks;                            // store the number of times to blink the built-in LED

    pinMode(LED_BUILTIN,OUTPUT);                      // make sure built-in LED is set for output
  }

  boolean update(){
       
    for(int i=0;i<nBlinks;i++){
      digitalWrite(LED_BUILTIN,LOW);
      delay(250);
      digitalWrite(LED_BUILTIN,HIGH);
      delay(250);
    }

    return(true);                               // return true
    
  } // update
  
};
