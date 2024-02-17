
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_GarageDoor : Service::GarageDoorOpener {     // A Garage Door Opener

  Characteristic::CurrentDoorState *current;            // reference to the Current Door State Characteristic (specific to Garage Door Openers)
  Characteristic::TargetDoorState *target;             // reference to the Target Door State Characteristic (specific to Garage Door Openers)  
  SpanCharacteristic *obstruction;        // reference to the Obstruction Detected Characteristic (specific to Garage Door Openers)

  DEV_GarageDoor() : Service::GarageDoorOpener(){       // constructor() method
        
    current=new Characteristic::CurrentDoorState(1);              // initial value of 1 means closed
    target=new Characteristic::TargetDoorState(1);                // initial value of 1 means closed
    obstruction=new Characteristic::ObstructionDetected(false);   // initial value of false means NO obstruction is detected
    
    Serial.print("Configuring Garage Door Opener");   // initialization message
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    // see HAP Documentation for details on what each value represents

    if(target->getNewVal()==0){                     // if the target-state value is set to 0, HomeKit is requesting the door to be in open position
      LOG1("Opening Garage Door\n");
      current->setVal(2);                           // set the current-state value to 2, which means "opening"
      obstruction->setVal(false);                   // clear any prior obstruction detection
    } else {
      LOG1("Closing Garage Door\n");                // else the target-state value is set to 1, and HomeKit is requesting the door to be in the closed position
      current->setVal(3);                           // set the current-state value to 3, which means "closing"         
      obstruction->setVal(false);                   // clear any prior obstruction detection
    }
    
    return(true);                               // return true
  
  } // update

  void loop(){                                     // loop() method

    if(current->getVal()==target->getVal())        // if current-state matches target-state there is nothing do -- exit loop()
      return;

    if(current->getVal()==3 && random(100000)==0){    // here we simulate a random obstruction, but only if the door is closing (not opening)
      current->setVal(4);                             // if our simulated obstruction is triggered, set the curent-state to 4, which means "stopped"
      obstruction->setVal(true);                      // and set obstruction-detected to true
      LOG1("Garage Door Obstruction Detected!\n");
    }

    if(current->getVal()==4)                       // if the current-state is stopped, there is nothing more to do - exit loop()     
      return;

    // This last bit of code only gets called if the door is in a state that represents actively opening or actively closing.
    // If there is an obstruction, the door is "stopped" and won't start again until the HomeKit Controller requests a new open or close action

    if(target->timeVal()>5000)                     // simulate a garage door that takes 5 seconds to operate by monitoring time since target-state was last modified
      current->setVal(target->getVal());           // set the current-state to the target-state
       
  } // loop
  
};

////////////////////////////////////

struct DEV_WindowShade : Service::WindowCovering {     // A motorized Window Shade with Hold Feature

  SpanCharacteristic *current;                     // reference to a "generic" Current Position Characteristic (used by a variety of different Service)
  SpanCharacteristic *target;                      // reference to a "generic" Target Position Characteristic (used by a variety of different Service)

  DEV_WindowShade() : Service::WindowCovering(){       // constructor() method
        
    current=new Characteristic::CurrentPosition(0);     // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)
    
    target=new Characteristic::TargetPosition(0);       // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)
    target->setRange(0,100,10);                         // set the allowable target-position range to 0-100 IN STEPS of 10
        
    Serial.print("Configuring Motorized Window Shade");   // initialization message
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    // The logic below is based on how HomeKit appears to operate in practice, which is NOT consistent with
    // HAP documentation.  In that document HomeKit seems to support fully opening or fully closing a window shade, with
    // an optional control to HOLD the window shade at a given in-between position while it is moving.

    // In practice, HomeKit does not appear to implement any form of a HOLD control button, even if you instantiate that
    // Characteristic.  Instead, HomeKit provides a full slider control, similar to the brightness control for a lightbulb,
    // that allows you to set the exact position of the window shade from 0-100%.  This obviates the need to any sort of HOLD button.
    // The resulting logic is also very simple:

    if(target->getNewVal()>current->getVal()){      // if the target-position requested is greater than the current-position, simply log a "raise" message  
      LOG1("Raising Shade\n");                      // ** there is nothing more to do - HomeKit keeps track of the current-position so knows raising is required
    } else 
    if(target->getNewVal()<current->getVal()){      // if the target-position requested is less than the current-position, simply log a "raise" message  
      LOG1("Lowering Shade\n");                     // ** there is nothing more to do - HomeKit keeps track of the current-position so knows lowering is required
    }
        
    return(true);                               // return true
  
  } // update

  void loop(){                                     // loop() method

    // Here we simulate a window shade that takes 5 seconds to move to its new target posiiton
    
    if(current->getVal()!=target->getVal() && target->timeVal()>5000){          // if 5 seconds have elapsed since the target-position was last modified...
      current->setVal(target->getVal());                                        // ...set the current position to equal the target position
    }

    // Note there is no reason to send continuous updates of the current position to the HomeKit.  HomeKit does NOT display the
    // current position.  Rather, it simply compares the value of the current position to the value of target positon as set by the
    // the user in the Home App.  If it finds current and target positions are the same, it knows the shade is stopped.  Otherwise
    // it will report the shade is raising or lowering depending on whether the specified target state is greater or less than
    // the current state.

    // According to HAP, the Characteristic Position State is also required.  However, this seems duplicative and is NOT needed
    // at all given the way HomeKit uses current position.
    
  } // loop
  
};
