
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

    digitalWrite(ledPin,power->getNewVal());        // use a standard Arduino function to turn on/off ledPin based on the return of a call to power->getNewVal() (see below for more info)
   
    return(StatusCode::OK);                         // return OK status code.  There are other possibilties we will explore in later examples.
  
  } // update
};
      
//////////////////////////////////

// HOW update() WORKS:
// ------------------
//
// Whenever a HomeKit controller requests HomeSpan to update a Characteristic, HomeSpan calls the update() method for the SERVICE that contains the
// Characteristic.  It calls this only one time, even if multiple Characteristics updates are requested for that Service.  For example, if you
// direct HomeKit to turn on a light and set it to 50% brightness, it will send HomeSpan two requests: one to update the "On" Characteristic of the
// LightBulb Service from "false" to "true" nd another to update the "Brightness" Characteristic of that same Service to 50.  This is VERY inefficient
// and would require the user to process multiple updates to the same Service.
//
// Instead, HomeSpan combines both requests into a single call to update() for the Service itself, where you can process all of thre Characteristics
// that change at the same time.  In the example above, we only have a single Characteristic to deal with, so this does not mean much.  But in later
// examples we'll see how this works with multiple Characteristics.

// HOW TO ACCESS A CHARACTERISTIC'S NEW AND CURRENT VALUES
// -------------------------------------------------------
//
// HomeSpan stores the values for its Characteristics in a union structure that allows for different types, such as floats, booleans, etc.  The specific
// types are defined by HAP for each Characteristic.  Looking up whether a Characteristic is a uint8 or uint16 can be tiresome, so HomeSpan abstracts
// all these details.  Since C++ adheres to strict variable typing, this is done through the use of template methods.  Every Characteristic supports
// the following two methods:
//
//    getVal<type>()     - returns the CURRENT value of the Characterisic, after casting into "type"
//    getNewVal<type>()  - returns the NEW value (i.e. to be updated) of the Characteritic, after casting into "type"
//
// For example, MyChar->getVal<int>() returns the current value of SpanCharacterstic MyChar as an int, REGARDLESS of how the value is stored by HomeSpan.
// Similarly, MyChar->getVal<double>() returns a value as a double, even it is stored as as a boolean (in which case you'll either get 0.00 or 1.00).
// Of course you need to make sure you understand the range of expected values so that you don't try to access a value stored as 2-byte int using getVal<uint8_t>().
// But it's perfectly okay to use getVal<int>() to access the value of a Characteristic that HAP insists on storing as a float, even though its range is
// strictly between 0 and 100 in steps of 1.  Knowing the range and step size is all you need to know in determining you can access this as an <int> or even a <uint8_t>.
//
// Because most Characteristic values can properly be cast into int, getVal and getNewVal both default to <int> if the template parameter is not specified.
// As you can see above, we retrieved the new value HomeKit requested for the On Characteristic that we named "power" by simply calling power->getNewVal().
// Since no template parameter is specified, getNewVal() will return an int.  And since the On Characteristic is natively stored as a boolean, getNewVal()
// will either return a 0 or a 1, depending on whether HomeKit is requesting the Characteristic to be turned off or on. 
//
// You may also note that in the above example we needed to use getNewVal(), but did not use getVal() anywhere.  This is because we know exactly what
// to do if HomeKit requests an LED to be turned on or off.  The current status of the LED (on or off) does not matter.  In latter examples we will see
// instances where the current state of the device DOES matter, and we will need to access both current and new values.
//
// Finally, there is one additional method for Characteristics that is not used above but will be in later examples: updated().  This method returns a
// boolean indicating whether HomeKit has requested a Characteristic to be updated, which means that getNewVal() will contain the new value it wants to set
// for that Characteristic.  For a Service with only one Characteristic, as above, we don't need to ask if "power" was updated using power->updated() because
// the fact the the update() method for the Service is being called means that HomeKit is requesting an update, and the only thing to update is "power".
// But for Services with two or more Characteristics, update() can be called with a request to update only a subset of the Characteristics.  We will
// find good use for the updated() method in later, multi-Characteristic examples.

// WHAT THE RETURN CODE FOR update() MEANS
// ---------------------------------------
//
// HomeKit requires each Characteristic to return a status code when an attempt to update it's value is made.  HomeSpan automatically takes care of
// some of errors, such as a Characteristic not being found, or a request to update a Characteristic that is read only.  In these cases update() is never
// even called.  But if it is, HomeSpan will apply the return code you specify to each of the Characteristics that were to be updated in that Service.
// By returning StatusCode:OK you tell HomeSpan that the newValues requested are okay and you've made the required updates to the physical device.  Upon 
// receiving an OK status, HomeSpan updates the Characteristics themselves by copying the "newValue" data elements into the current "value" data elements.
// HomeSpan then sends a message back to HomeKit letting it know that the new values it requested have been sucessfully processed.  At no point does
// HomeKit as for, or allow, a value to be sent back from HomeSpan indicating the data in a Characteristic.  When requesting an update, HomeKit simply
// expects an okay or not okay.
//
// If for some reason the update() code cannot process an update request, it must return a HAP error code.  These are listing in Settings.h.  As noted above,
// some are created automatically by HomeSpan and update() is never called.  Others can only be determined from within an update() call and should be 
// used as the return value.  The StatusCodes that can be used in this fashion are: OK, Unable, Busy.  Any of the other StatusCode listed in Settings.h
// are automatically handled by HomeSpan and should not be used as a return code, though nothing bad happens if you do.  This is because the
// HomeKit application itself only seems to distinguish an "OK" status code from a an not-"OK" code.  HomeKit does not seem to process any error code
// differently from any other error code.  All error codes lead HomeKit to simply say "Device Not Reponding" in the Controller.  This suggests that the
// differences between "Unable" and "Busy" don't mean anything to HomeKit.  They are all interpreted as an error, so pick anyone you'd like in the event
// you can't update a characteristic to the requested value.  Note that when you do return an error code, HomeSpan will NOT copy the newValue data elements
// requested into the the current value data elements, thus keeping all Characteristics in the Service unchanged.
//
// Final note:  There are very few reasons you should need to return an error code since so much checking is done in advance by either HomeSpan or HomeKit
// itself.  For instance, HomeKit does not allow you to use the Controller, or even Siri, to change the brightness of LightBulb to a value outside the
// range of allowable values you specified.  This means that any update() requests you receive should only contain newValue data element that are in-range.
//
