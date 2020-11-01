
////////////////////////////////////////////////////
//   DEVICE-SPECIFIC PROGRAMMABLE SWITCH SERVICES //
////////////////////////////////////////////////////

struct DEV_ProgButton : Service::StatelessProgrammableSwitch {       // Stateless Programmable Switch
 
  SpanCharacteristic *switchEvent;                  // reference to the ProgrammableSwitchEvent Characteristic
  
  DEV_ProgButton(int buttonPin, int index) : Service::StatelessProgrammableSwitch(){

    switchEvent=new Characteristic::ProgrammableSwitchEvent();  // ProgrammableSwitchEvent Characteristic
    new Characteristic::ServiceLabelIndex(index);               // set service label index
                
    new SpanButton(buttonPin);                                  // create new SpanButton

    Serial.print("Configuring Programmable Pushbutton: Pin=");  // initialization message
    Serial.print(buttonPin);
    Serial.print("  Index=");
    Serial.print(index);
    Serial.print("\n");
    
  } // end constructor

  void button(int pin, int pressType) override {

    LOG1("Found button press on pin: ");            // always a good idea to log messages
    LOG1(pin);
    LOG1("  type: ");
    LOG1(pressType==SpanButton::LONG?"LONG":(pressType==SpanButton::SINGLE)?"SINGLE":"DOUBLE");
    LOG1("\n");

    switchEvent->setVal(pressType);                // set the value of the switchEvent Characteristic

  }

};
      
//////////////////////////////////
