
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
  } // end constructor

  boolean update(){                              // update() method

    digitalWrite(ledPin,power->getNewVal());      
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  LedPin *ledPin;                                   // reference to Led Pin
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int pin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    level->setRange(5,100,1);                       // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->ledPin=new LedPin(pin);                   // configures a PWM LED for output to the specified pin
    
  } // end constructor

  boolean update(){                              // update() method
    
    ledPin->set(power->getNewVal()*level->getNewVal());    
   
    return(true);                               // return true
  
  } // update
};
      
//////////////////////////////////
