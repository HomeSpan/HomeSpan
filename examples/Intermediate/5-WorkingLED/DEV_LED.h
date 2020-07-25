
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

// HERE'S WHERE WE DEFINE OUR NEW LED SERVICE!

struct DEV_LED : Service::LightBulb {               // First we create a derived class from the HomeSpan LightBulb Service

  int ledPin;                                       // this variable stores the pin number defined for this LED
  SpanCharacteristic *power;                        // here we create a generic pointer to a SpanCharacteristic named "power" that we will use below

  // Next we define the constructor for DEV_LED.  Note that it takes one argument, ledPin,
  // which specifies the pin to which the LED is attached.
  
  DEV_LED(int ledPin) : Service::LightBulb(){

    power=new Characteristic::On();                 // this is where we create the On Characterstic we had previously defined in setup().  Save this in the pointer created above, for use below
    this->ledPin=ledPin;                            // don't forget to store ledPin...
    pinMode(ledPin,OUTPUT);                         // ...and set the mode for ledPin to be an OUTPUT (standard Arduino function)
    
  } // end constructor

  // Finally, we over-ride the default update() method with instructions that actually turn on/off the LED.  Note update() returns type "StatusCode"

  StatusCode update(){            

    digitalWrite(ledPin,power->newValue.BOOL);      // use a standard Arduino function to turn on/off ledPin based on the boolean variable power->newValue.BOOL
   
    return(StatusCode::OK);                         // return OK status code.  There are other possibilties we will explore in later examples.
  
  } // update
};
      
//////////////////////////////////
