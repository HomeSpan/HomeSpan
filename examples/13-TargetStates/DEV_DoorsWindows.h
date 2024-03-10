
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_GarageDoor : Service::GarageDoorOpener {    // A Garage Door Opener

  Characteristic::CurrentDoorState *current;           // reference to the Current Door State Characteristic (specific to Garage Door Openers)
  Characteristic::TargetDoorState *target;             // reference to the Target Door State Characteristic (specific to Garage Door Openers)  
  SpanCharacteristic *obstruction;                     // reference to the Obstruction Detected Characteristic (specific to Garage Door Openers)

  DEV_GarageDoor() : Service::GarageDoorOpener(){      // constructor() method

    // Below we use enumerated constants rather than integers to set the values of the Characteristics.
    // Using enumerated constants means not having to remember the integer code for each state.  You'll find
    // a complete list of all available enumerated constants on HomeSpan's Services and Characteristics page.
    // Note the use of enumerated constants is optional - you can always use the integer code representing
    // each state instead.
        
    current=new Characteristic::CurrentDoorState(Characteristic::CurrentDoorState::CLOSED);      // here we use the fully-qualified name of the constant "CLOSED"
    target=new Characteristic::TargetDoorState(target->CLOSED);                                  // here we use the name of the object instead of the fully-qualified name (much less typing)

    // Below we must use the fully-qualified name of the enumerated constant and cannot use "obstruction->NOT_DETECTED".
    // Why?  Because above we declared "obstruction" to be a pointer to a generic SpanCharacteristic instead of a pointer to 
    // the more specific Characteristic::ObstructionDetected.  Either is fine, and it's just a matter of programming preference
    // (as you can see we use both conventions in this sketch).  But the downside of using SpanCharacteristic to declare a
    // Characteristic that contains enumerated constants is that the object itself does not know about these constants. This is
    // because all enumerated constants are uniquely defined within their respective specific Characteristic classes, and not in the
    // generic SpanCharacteristic class from which all specific Characterstics are derived.
    
    obstruction=new Characteristic::ObstructionDetected(Characteristic::ObstructionDetected::NOT_DETECTED);   // this works
//  obstruction=new Characteristic::ObstructionDetected(obstruction->NOT_DETECTED);                           // this would produce a compiler error (try it and see)
    
    Serial.print("Configuring Garage Door Opener");   // initialization message
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    // see HAP Documentation for details on what each value represents

    if(target->getNewVal()==target->OPEN){          // HomeKit is requesting the door to be in OPEN position
      LOG1("Opening Garage Door\n");
      current->setVal(current->OPENING);            // set the current-state value to OPENING
      obstruction->setVal(false);                   // clear any prior obstruction detection - note we do not bother using an enumerated constant here
    } else {
      LOG1("Closing Garage Door\n");                // else HomeKit must be requesting the door to be in the CLOSED position
      current->setVal(current->CLOSING);            // set the current-state value to CLOSING
      obstruction->setVal(false);                   // clear any prior obstruction detection
    }
    
    return(true);                               // return true
  
  } // update

  void loop(){                                     // loop() method

    if(current->getVal()==target->getVal())        // if current-state matches target-state there is nothing do -- exit loop()
      return;

    if(current->getVal()==current->CLOSING && random(100000)==0){    // here we simulate a random obstruction, but only if the door is closing (not opening)
      current->setVal(current->STOPPED);                             // if our simulated obstruction is triggered, set the curent-state to STOPPED
      obstruction->setVal(true);                                     // and set obstruction-detected to true
      LOG1("Garage Door Obstruction Detected!\n");
    }

    if(current->getVal()==current->STOPPED)                          // if the current-state is stopped, there is nothing more to do - exit loop()     
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
   
  } // loop
  
};
