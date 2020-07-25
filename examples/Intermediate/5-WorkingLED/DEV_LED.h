
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

    digitalWrite(ledPin,power->newValue.BOOL);      // use a standard Arduino function to turn on/off ledPin based on the boolean variable power->newValue.BOOL (see below for more info)
   
    return(StatusCode::OK);                         // return OK status code.  There are other possibilties we will explore in later examples.
  
  } // update
};
      
//////////////////////////////////

// How update() works:
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

// How to access Characteristic values:
//
// HomeSpan stores the values for its Characteristics in a union structure that allows for different types.  The current value of a Characteristic
// is stored in a union named "value" whereas upon an update request, the requested value is stored in a union named "newValue."  To access the data
// underlying either "value" or "newValue" you need to select the element of the union that matches the type.  This is arguably sloppy, but using
// C++ templates did not seem to make the process any less cumbersome.  The names of each element are based on those specified in HAP Table 6-5, and map
// to the Arduino data types as follows:
//
//    BOOL -> (boolean)
//    UINT8 -> (uint8_t)
//    UINT16 -> (uint16_t)
//    UINT32 -> (uint32_t)
//    UINT64 -> (uint64_t)
//    INT -> (int)
//    FLOAT -> (double)
//    STRING -> (const char *)
//
// In the above example we created pointer named "power" to point to our newly-created "On" Characteristic.  Hence, to access the current value of that
// Characteristic we use "power->value.BOOL"  To access to new value requested by HomeKit for this update, we use "power->newValue.BOOL" as shown above.
// In most cases, we can manage the update by just reading the newValue requested, regardless of the whatever the current value is, but access to the
// current value is available if neeed.

// How to determine the value type for any Characteristic:
//
// All HomeKit Characteristics that have been implemented in HomeSpan are defined in "Services.h" in the HomeSpan library.  The top part of "Services.h" defines
// all the implemented Services.  The bottom part defines the collection of Characteristics needed for those Services.  Within the definition of each
// Characteristic you'll see the HAP ID number, as well as the data type, such as (boolean), (uint16_t), etc.  Select the corresponding element name
// from the table above to access the underlying "value" or "newValue" data elements.

// What the return code means:
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
