
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_WindowShade : Service::WindowCovering {     // A motorized Window Shade with Hold Feature

  SpanCharacteristic *current;                     // reference to a "generic" Current Position Characteristic (used by a variety of different Service)
  SpanCharacteristic *target;                      // reference to a "generic" Target Position Characteristic (used by a variety of different Service)
  SpanCharacteristic *hTiltCurrent;                // reference to horizontal tilt of window shade - current position
  SpanCharacteristic *hTiltTarget;                 // reference to horizontal tilt of window shade - target position

  ServoPin *hTiltServo;                            // reference to Servo Pin to control Horiontal Tilt

  DEV_WindowShade(uint8_t hTiltServoPin) : Service::WindowCovering(){       // constructor() method
        
    current=new Characteristic::CurrentPosition(0);     // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)    
    target=new Characteristic::TargetPosition(0);       // Window Shades have positions that range from 0 (fully lowered) to 100 (fully raised)
    target->setRange(0,100,10);                         // set the allowable target-position range to 0-100 IN STEPS of 10

    hTiltCurrent=new Characteristic::CurrentHorizontalTiltAngle();      // Tilt Angle is measured in degrees; HAP default is -90 to +90
    hTiltTarget=new Characteristic::TargetHorizontalTiltAngle();

    // Here we define our Servo using HomeSpan's ServoPin Class.
    // See the HomeSpan API Reference for full details and a list of all parameters.
    
    hTiltServo=new ServoPin(hTiltServoPin);
       
    Serial.print("Configuring Motorized Window Shade");   // initialization message
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    if(target->updated()){                            // check to see if shade target position was updated
      if(target->getNewVal()>current->getVal()){      // if the target-position requested is greater than the current-position, simply log a "raise" message  
        LOG1("Raising Shade\n");                      // ** there is nothing more to do - HomeKit keeps track of the current-position so knows raising is required
      } else 
      if(target->getNewVal()<current->getVal()){      // if the target-position requested is less than the current-position, simply log a "raise" message  
        LOG1("Lowering Shade\n");                     // ** there is nothing more to do - HomeKit keeps track of the current-position so knows lowering is required
      }
    }

    if(hTiltTarget->updated()){                         // check to see if shade tilt angle was updated
      hTiltCurrent->setVal(hTiltTarget->getNewVal());   // set current value of tilt to match target value
      hTiltServo->set(hTiltTarget->getNewVal());        // <--- update actual servo position with ServoPin->set(degrees) method
    }
        
    return(true);                               // return true
  
  } // update

  void loop(){                                     // loop() method

    // Here we simulate a window shade that takes 5 seconds to move to its new target position
    
    if(current->getVal()!=target->getVal() && target->timeVal()>5000){          // if 5 seconds have elapsed since the target-position was last modified...
      current->setVal(target->getVal());                                        // ...set the current position to equal the target position
    }
    
  } // loop
  
};
