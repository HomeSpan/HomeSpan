
////////////////////////////////////////////////////
//   DEVICE-SPECIFIC PROGRAMMABLE SWITCH SERVICES //
////////////////////////////////////////////////////

struct DEV_ProgButton : Service::StatelessProgrammableSwitch {       // Stateless Programmable Switch
 
  int buttonPin;                                    // pin with programmable pushbutton
  SpanCharacteristic *switchEvent;                  // reference to the ProgrammableSwitchEvent Characteristic
  
  DEV_ProgButton(int buttonPin) : Service::StatelessProgrammableSwitch(){

    switchEvent=new Characteristic::ProgrammableSwitchEvent(); // ProgrammableSwitchEvent Characteristic
                
    new SpanButton(buttonPin);                      // create new SpanButton
    this->buttonPin=buttonPin;                      // save button pin number

    Serial.print("Configuring Programmable Pushbuton: Pin="); // initialization message
    Serial.print(buttonPin);
    Serial.print("\n");
    
  } // end constructor

  void button(int pin, boolean isLong) override {

    LOG1("Found button press on pin: ");            // always a good idea to log messages
    LOG1(pin);
    LOG1("  type: ");
    LOG1(isLong?"LONG":"SHORT");
    LOG1("\n");

    switchEvent->setVal(isLong?2:0);                // set the value of the switchEvent Characteristic to 2 for long press or 0 for short press

  }

};
      
//////////////////////////////////
