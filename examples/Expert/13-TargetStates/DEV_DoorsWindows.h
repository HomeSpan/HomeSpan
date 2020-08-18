
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_GarageDoor : Service::GarageDoorOpener {     // A Garage Door Opener

  SpanCharacteristic *current;
  SpanCharacteristic *target;
  SpanCharacteristic *obstruction;

  DEV_GarageDoor(ServiceType sType=ServiceType::Regular) : Service::GarageDoorOpener(sType){       // constructor() method
        
    current=new Characteristic::CurrentDoorState(1);
    target=new Characteristic::TargetDoorState(1);
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
    
    return(StatusCode::OK);                         // return OK status code
  
  } // update

  void loop(){                                     // loop() method

    if(current->getVal()==target->getVal())
      return;

    if(current->getVal()==3 && random(100000)==0){
      current->setVal(4);
      obstruction->setVal(true);       
      LOG1("Garage Door Obstruction Detected!\n");
    }

    if(current->getVal()==4)
      return;

    if(target->timeVal()>5000)
      current->setVal(target->getVal());
       
  } // loop
  
};

////////////////////////////////////

struct DEV_WindowShade : Service::WindowCovering {     // A motorized Window Shade with Hold Feature

  SpanCharacteristic *current;
  SpanCharacteristic *target;

  DEV_WindowShade(ServiceType sType=ServiceType::Regular) : Service::WindowCovering(sType){       // constructor() method
        
    current=new Characteristic::CurrentPosition(0);
    new SpanRange(0,100,10);
    
    target=new Characteristic::TargetPosition(0);
    new SpanRange(0,100,10);
        
    Serial.print("Configuring Motorized Window Shade");   // initialization message
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    if(target->getNewVal()>current->getVal()){
      LOG1("Raising Shade\n");
    } else 
    if(target->getNewVal()<current->getVal()){
      LOG1("Lowering Shade\n");
    }
        
    return(StatusCode::OK);                         // return OK status code
  
  } // update

  void loop(){                                     // loop() method

    if(current->timeVal()>1000){
      if(target->getVal()>current->getVal()){
        current->setVal(current->getVal()+10);
      } else
      if(target->getVal()<current->getVal()){
        current->setVal(current->getVal()-10);        
      }
    }
     
  } // loop
  
};
