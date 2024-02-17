
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  // This version of the Dimmable LED Service includes a PushButton that can be used to turn on/off the LED.  Status of both the
  // power state and the brightness of the LED are stored in NVS for restoration if the device reboots.
  
  LedPin *LED;                                      // reference to an LedPin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
 
  DEV_DimmableLED(int ledPin, int buttonPin) : Service::LightBulb(){

    power=new Characteristic::On(0,true);           // NEW! Second argument is true, so the value of the On Characteristic (initially set to 0) will be saved in NVS
    level=new Characteristic::Brightness(5,true);   // NEW! Second argument is true, so the value of the Brightness Characteristic (initially set to 5) will be saved in NVS               
    level->setRange(5,100,1);                       // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    new SpanButton(buttonPin);                      // create a new SpanButton to control power using PushButton on pin number "buttonPin"

    this->LED=new LedPin(ledPin);                   // configures a PWM LED for output to pin number "ledPin"

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(LED->getPin());
    Serial.print("\n");

    LED->set(power->getVal()*level->getVal());      // NEW! IMPORTANT: Set the LED to its initial state at startup.  Note we use getVal() here, since it is set upon instantiation.
    
  } // end constructor

  boolean update(){                              // update() method

    LOG1("Updating Dimmable LED on pin=");
    LOG1(LED->getPin());
    LOG1(":  Current Power=");
    LOG1(power->getVal()?"true":"false");
    LOG1("  Current Brightness=");
    LOG1(level->getVal());
  
    if(power->updated()){
      LOG1("  New Power=");
      LOG1(power->getNewVal()?"true":"false");
    }

    if(level->updated()){
      LOG1("  New Brightness=");
      LOG1(level->getNewVal());
    } 

    LOG1("\n");
    
    LED->set(power->getNewVal()*level->getNewVal());         // update the physical LED to reflect the new values    
   
    return(true);                               // return true
  
  } // update

  void button(int pin, int pressType) override {

    if(pressType==SpanButton::SINGLE){                      // only respond to SINGLE presses
      power->setVal(1-power->getVal());                     // toggle the value of the power Characteristic
      LED->set(power->getVal()*level->getVal());            // update the physical LED to reflect the new values
    }   

  } // button

};
      
//////////////////////////////////
