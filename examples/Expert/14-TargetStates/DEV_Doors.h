
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
    } else {
      LOG1("Closing Garage Door\n");
      current->setVal(3);            
    }
    
    alarmTime=millis()+10000;

    return(StatusCode::OK);                         // return OK status code
  
  } // update

  void event(){                                     // event() method

    if(current->getVal()==target->getVal())
      return;

    if(millis()>alarmTime)
      current->setVal(target->getVal());
       
  } // event
  
};
