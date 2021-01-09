
//////////////////////////////////
//   DEVICE-SPECIFIC SERVICES   //
//////////////////////////////////

// Here we define the DEV_Identify Service as derived class of AccessoryInformation

struct DEV_Identify : Service::AccessoryInformation {

  int nBlinks;                    // number of times to blink built-in LED in identify routine
  SpanCharacteristic *identify;   // reference to the Identify Characteristic

  // Next we define the constructor using all the arguments needed to implement the required Characteristics
  // of AccessoryInformation, plus one extra argument at the end called "nBlinks" we will use to specify how many
  // times HomeSpan should blink the built-in LED when HomeKit calls this device's Identify routine during pairing.
  
  DEV_Identify(const char *name, const char *manu, const char *sn, const char *model, const char *version, int nBlinks) : Service::AccessoryInformation(){
    
    new Characteristic::Name(name);                   // create all the required Characteristics with values set based on above arguments
    new Characteristic::Manufacturer(manu);
    new Characteristic::SerialNumber(sn);    
    new Characteristic::Model(model);
    new Characteristic::FirmwareRevision(version);
    identify=new Characteristic::Identify();          // store a reference to the Identify Characteristic for use below

    this->nBlinks=nBlinks;                            // store the number of times to blink the LED

    pinMode(homeSpan.getStatusPin(),OUTPUT);          // make sure LED is set for output
  }

  // How HomeKit Identifies Devices:
  //
  // When HomeKit first pairs with a new device it "calls" that device's identify routine for every defined Accessory.
  // To do so, HomeKit requests the Identify Characteristic for each defined AccessoryInformation Service to be set to "true".
  // The Identify Characteristic is write-only, so no value is ever stored, even though HomeKit is requesting its value
  // be updated.  We can therefore use the same update() method as if the Identify Characteristic was the same as any 
  // other boolean Characteristic.

  // There are many ways to implement some form of identification.  For an LED, you could blink it one or more times.
  // For a LightBulb, you can flash it on and off.  For window shade, you could raise and lower it.
  // Most commerical devices don't do anything.  Because HomeSpan can be used to control many different types of
  // device, below we implement a very generic routine that simply blinks the Status LED the number of times specified above.
  // In principle, this code could call a user-defined routine that is different for each physcially-attached device (light, shade, fan, etc),
  // but in practice this is overkill.

  // Note that the blink routine below starts by turning off the Status LED and then leaves it on once it has blinked
  // the specified number of times.  This is because when HomeSpan starts up if confirms to user that it has connected
  // to the WiFi network by turning on the Status LED.  Thus we want to leave it on when blinking is completed.

  // Also note we use the homeSpan.getStatusPin() method to find the pin number associated with the Status LED

  boolean update(){
       
    for(int i=0;i<nBlinks;i++){
      digitalWrite(homeSpan.getStatusPin(),LOW);
      delay(250);
      digitalWrite(homeSpan.getStatusPin(),HIGH);
      delay(250);
    }

    return(true);                               // return true
    
  } // update
  
};
