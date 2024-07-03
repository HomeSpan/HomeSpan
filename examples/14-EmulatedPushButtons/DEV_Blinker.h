
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

// NOTE: This example is constructed only for the purpose of demonstrating how to
// emulate a pushbutton in HomeSpan.  The length of the blinking routine is MUCH longer
// than HomeSpan should spend on an update().  To see how this effects HomeKit, try changing
// the number of blinks to 50, or keep it at 3 and increase the delay times in update() so
// that the blink routine takes 10 seconds or more. When activated, HomeKit will think the
// device has become non-responsive if it does not receive a return message from update() within
// a certain period of time.

// In practice, pushbuton emulation is used for very short routines, such as driving
// an IR LED or an RF transmitter to send a code to a remote device.

// New and changed lines in comparison with Example 9 are noted as "NEW!"

struct DEV_Blinker : Service::LightBulb {           // LED Blinker

  int ledPin;                                       // pin number defined for this LED
  int nBlinks;                                      // NEW! number of times to blink
  
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_Blinker(int ledPin, int nBlinks) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
        
    this->ledPin=ledPin;                            
    this->nBlinks=nBlinks;                           // NEW! number of blinks
    pinMode(ledPin,OUTPUT);                         
    
    Serial.print("Configuring LED Blinker: Pin=");   // initialization message
    Serial.print(ledPin);
    Serial.print("  Blinks=");                       // NEW! add output message for number of blinks
    Serial.print(nBlinks);
    Serial.print("\n");

  } // end constructor

  boolean update(){                              // update() method

    // NEW! Instead of turning on or off the LED according to newValue, we blink it for
    // the number of times specified, and leave it in the off position when finished.
    // This line is deleted...
    
    // digitalWrite(ledPin,power->getNewVal());      

    // and is replaced by...

    if(power->getNewVal()){                       // check to ensure HomeKit is requesting we "turn on" this device (else ignore)

      LOG1("Activating the LED Blinker on pin=");
      LOG1(ledPin);
      LOG1("\n");

      for(int i=0;i<nBlinks;i++){                     // loop over number of blinks specified
        digitalWrite(ledPin,HIGH);                    // turn pin on
        delay(100);                                   // wait 100 ms
        digitalWrite(ledPin,LOW);                     // turn pin off
        delay(250);                                   // wait 250 ms
      }
      
    } // if newVal=true

    // Note that the delays above of 100ms and 250ms are for illustrative purposes only
    // (and so you can see the LED blink). In practice, if you were controlling an IR LED
    // or an RF transmitter, the whole signal would likely transmit in 10ms total.
    
    return(true);                               // return true
  
  } // update

  // NEW! Here we implement a very simple loop() method that checks to see if the power Characteristic
  // is "on" for at least 3 seconds.  If so, it resets the value to "off" (false).

  void loop(){

    if(power->getVal() && power->timeVal()>3000){   // check that power is true, and that time since last modification is greater than 3 seconds 
      LOG1("Resetting Blinking LED Control\n");     // log message  
      power->setVal(false);                         // set power to false
    }      
    
  } // loop
  
};
      
//////////////////////////////////
