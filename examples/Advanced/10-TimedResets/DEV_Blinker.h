
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

// NOTE: This example is constructed only for the purpose of demonstrating how to
// use SpanTimedReset() to emulate a pushbutton in HomeSpan.  The length of the blinking
// routine is much longer than HomeSpan should spend on an update().  To see how this
// effects HomeKit, try changing the number of blinks to 50, or keep it at 3 and
// increase the delay times in update() so that the blink routine takes 10 seconds or more.
// When activated, HomeKit will think the device has become non-responsive.
//
// In practice, pushbuton emulation is used for very short routines, such as driving
// an IR LED or an RF transmitter to send a code to a remote device.

struct DEV_Blinker : Service::LightBulb {           // LED Blinker

  int ledPin;                                       // pin number defined for this LED
  int nBlinks;                                      // NEW! number of times to blink
  
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_Blinker(int ledPin, int nBlinks) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 

    // Here we create a new Timed Reset of 2000 milliseconds. Similar to SpanRange(), SpanTimedReset() automatically
    // attaches to the last Characteristic instantiated, which in this case the the "power" Characteristic::On above.
    // SpanTimedReset() will notify HomeKit that the Characteristic has been turned off by HomeSpan 2000 milliseconds
    // after HomeKit requests it be turned on.  This DOES NOT cause HomeKit to send an "off" request to HomeSpan (with
    // one exception --- see * below). Rather, HomeSpan is notifying HomeKit that HomeSpan itself has turned "off" the
    // Characteristic, and that HomeKit should reflect this new "off" status in the Tile shown for this device in the
    // HomeKit Controller.
    //
    // Note that in practice you'll want to set the reset time to 500ms or less to better emulate a pushbutton.
    // We've used a full 2 seconds in this example for illustrative purposes only.
    
    new SpanTimedReset(1000);                        // *** NEW!! instantiate SpanTimedReset with a delay of 2000 milliseconds
    
    this->ledPin=ledPin;                            
    this->nBlinks=nBlinks;                           // NEW! number of blinks
    pinMode(ledPin,OUTPUT);                         
    
    Serial.print("Configuring LED Blinker: Pin=");   // initialization message
    Serial.print(ledPin);
    Serial.print("  Blinks=");                       // NEW! add output message for number of blinks
    Serial.print(nBlinks);
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    // Instead of turning on or off the LED according to newValue, we blink it for
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
    
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

// * EXCEPTION:  There is an apparent bug in HomeKit such that if you have an Accessory with three or more
// Services, and the Accessory receives a notification message from the device, AND the HomeKit interface is
// open to show the detailed control for Service in the Accessory, then for some reason HomeKit tries to
// update the device with the same status it just received from the device, even though this is contrary to
// the purpose of notification requests.  This is why it's a good idea to check that newValue.BOOL==true.  It
// avoids triggering the device if for some reason HomeKit should send a reqeust to update newValue to false.
