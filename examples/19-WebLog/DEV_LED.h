
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_LED : Service::LightBulb {

  int ledPin;
  SpanCharacteristic *power;
  
  DEV_LED(int ledPin) : Service::LightBulb(){

    power=new Characteristic::On();
    this->ledPin=ledPin;
    pinMode(ledPin,OUTPUT);    
    WEBLOG("Configuring LED on Pin %d",ledPin);                         // NEW!  This creates a Web Log message announcing the configuration of the device
    
  } // end constructor

  boolean update(){            

    digitalWrite(ledPin,power->getNewVal());    
    WEBLOG("LED on Pin %d: %s",ledPin,power->getNewVal()?"ON":"OFF");   // NEW!  This creates a Web Log message whenever an LED is turned ON or OFF
    return(true);
  
  } // update
};
      
//////////////////////////////////

// SOME MORE ABOUT WEB LOGGING
// ---------------------------
//
// * The WEBLOG() macro operates by calling Serial.printf(), so the first argument always needs to be a text string containing printf-like format instructions.
//   The rest of the arguments, if any, are the variables to print.  For example, you cannot simply write WEBLOG(ledPin).  This will cause errors at compile time,
//   though you can write LOG1(ledPin) or LOG2(ledPin) to output log messages just to the Serial Monitor.
//
// * You do NOT need to include a "\n" at the end of your format string since all Web Log messages are formatted into an HTML table when presented, and HTML ignores "\n".
//
// * Every Web Log message is recorded with TWO timestamps.  The first timestamp is relative to when the device first booted, and is presented as DAYS:HH:MM:SS.  This timestamp
//   is always present.  The second timestamp is an absolute clock time, in standard Unix form, such as "Mon Aug 10 13:52:48 2020".  This timestamp will only be present
//   if the clock time of the device was set, else it will be shown as "Unknown".  Note that in the example above, the first Web Log message ("Configuring...") will
//   have a clock timestamp of "Unknown" even though we enabled Web Logging with a Time Server.  This is because the Time Server cannot be configured until WiFi has
//   been established, and the first Web Log message above is created during initial configuratin of the device, BEFORE a WiFi connection is made.  This is perfectly fine to do.
//
// * Every Web Log message also includes the IP Address of the Client that made the request, unless the Web Log message was generated independently of any Client request,
//   such as in the first message above.  In these cases the IP Address will be displayed as 0.0.0.0.
//
// * Web Log messages are printed to the Serial Monitor whenever the HomeSpan Log Level is set to 1 or greater.  Hence there is no reason to duplicate the same message
//   using WEBLOG() and LOG1() at the same time.
