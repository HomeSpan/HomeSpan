
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_GarageDoor : Service::GarageDoorOpener {     // A Garage Door Opener

  SpanCharacteristic *current;
  SpanCharacteristic *target;
  SpanCharacteristic *obstruction;

  unsigned long alarmTime;

  DEV_GarageDoor(ServiceType sType=ServiceType::Regular) : Service::GarageDoorOpener(sType){       // constructor() method
    
    new SpanEvent(1000);        // check for events on this Service every 1 second
    
    current=new Characteristic::CurrentDoorState(0);
    target=new Characteristic::TargetDoorState(0);
    obstruction=new Characteristic::ObstructionDetected(false);
    
    Serial.print("Configuring Garage Door Opener");   // initialization message
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    if(target->getNewVal()==0){
      LOG1("Opening Garage Door\n");
      current->setVal(2);     
      obstruction->setVal(false); 
    } else {
      LOG1("Closing Garage Door\n");
      current->setVal(3);            
      obstruction->setVal(false); 
    }
    
    alarmTime=millis()+10000;

    return(StatusCode::OK);                         // return OK status code
  
  } // update

  void event(){                                     // event() method

    if(current->getVal()==target->getVal())
      return;

    if(random(30)==0){
      current->setVal(4);
      obstruction->setVal(true);       
      LOG1("Garage Door Obstruction Detected!\n");
    }

    if(current->getVal()==4)
      return;

    if(millis()>alarmTime)
      current->setVal(target->getVal());
       
  } // event
  
};

////////////////////////////////////

struct DEV_WindowShade : Service::WindowCovering {     // A motorized Window Shade with Hold Feature

  SpanCharacteristic *current;
  SpanCharacteristic *target;
  SpanCharacteristic *state;
  SpanCharacteristic *hold;

  unsigned long alarmTime;
  int speed=5;

  DEV_WindowShade(ServiceType sType=ServiceType::Regular) : Service::WindowCovering(sType){       // constructor() method
    
    new SpanEvent(1000);        // check for events on this Service every 1 second
    
    current=new Characteristic::CurrentPosition(0);
    target=new Characteristic::TargetPosition(0);
    state=new Characteristic::PositionState(2);
    
    Serial.print("Configuring Motorized Window Shade");   // initialization message
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    if(target->getNewVal()>current->getVal()){
      LOG1("Raising Shade\n");
      state->setVal(1);     
      alarmTime=millis()+speed;
    } else 
    if(target->getNewVal()<current->getVal()){
      LOG1("Lowering Shade\n");
      state->setVal(0);            
      alarmTime=millis()+speed;
    }
    
    return(StatusCode::OK);                         // return OK status code
  
  } // update

  void event(){                                     // event() method

    if(current->getVal()==target->getVal())
      return;

    if(millis()<alarmTime)
      return;

    if(state->getVal()==1)
      current->setVal(current->getVal()+1);
    else
      current->setVal(current->getVal()-1);

    if(current->getVal()==target->getVal())
      state->setVal(2);   
    else         
      alarmTime=millis()+speed;
       
  } // event
  
};
