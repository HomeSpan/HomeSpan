
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_TempSensor : Service::TemperatureSensor {     // A standalone Temperature sensor

  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  
  DEV_TempSensor(ServiceType sType=ServiceType::Regular) : Service::TemperatureSensor(sType){       // constructor() method

    // We begin by defining a new SpanEvent.  This instructs HomeSpan to call the Service's event() method (defined below) periodically.
    // The argument to SpanEvent() defines the periodicity, in milliseconds.  In this case we are instructing HomeSpan to check this Service for
    // updates every 5 seconds.  Checking takes time, and updates use network traffic, so choose your periodicity wisely.  In practice you could
    // probably set the periodicity for a temperature sensor to 60 seconds or more.  But for illustrative purposes we are specifying more frequent
    // updates so you can see how the this example works without needing to wait a full minute for each change.
    
    new SpanEvent(5000);        // check for events on this Service every 5 seconds

    // Next we instantiate the main Characteristic for a Temperature Sensor, namely the Current Temperature, and set its initial value
    // to 20 degrees.  For a real sensor, we would take a reading and initialize it to that value instead.  NOTE:  HomeKit uses
    // Celsius for all temperature settings.  HomeKit will DISPLAY temperatures in the HomeKit app according to the settings on your iPhone.
    // Though the HAP documentation includes a Characteristic that appears to allow the device to over-ride this setting by specifying a display
    // of Celsius or Fahrenheit for each Service, it does not appear to work as advertised.
    
    temp=new Characteristic::CurrentTemperature(20.0);        // instantiate the Current Temperature Characteristic
    
    Serial.print("Configuring Temperature Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  // Lastly, we create the event() method. This method take no arguments and returns no values.  It will be called every 5 seconds
  // as specified above in the instantiation of SpanEvent().  In order to simulate a temperature change from an actual sensor we 
  // will read the current value of the temp Characteristic using the getVal() function, with <float> as the template parameter;
  // add 0.5 degrees Celsius; and then store the result in a float variable named "temperature."  This will simulate an increment of
  // 0.5 degrees Celsius (a little less than 1 degree F) every 5 seconds.  We will cap the temperature to 35.0 degrees C, after which
  // it resets to 10.0 and starts over.

  // All of the action happens in the last line, in which we set the value of the temp Characteristic to the new value of temperature.
  // This tells HomeKit to send an Event Notification message to all available Controllers making them aware of the new temperature.
  // Note that setVal() is NOT a template function and does not require you to specify <float> as a template parameter.  This is because
  // setVal() can determine the type from the argument you specify.  If there is any chance of ambiguity, you can always specifically
  // cast the argument such: setVal((float)temperature).

  void event(){

    float temperature=temp->getVal<float>()+0.5;            // here we "simulate" a half-degree temperature change...
    if(temperature>35.0)                                    // ...but cap the maximum at 35 degrees before starting over at 10 degrees
      temperature=10.0;
      
    temp->setVal(temperature);                              // don't forgot to update the temperature Characteristic to the new value!
       
  } // event
  
};
      
//////////////////////////////////

struct DEV_AirQualitySensor : Service::AirQualitySensor {     // A standalone Air Quality sensor

  // An Air Quality Sensor is similar to a Temperature Sensor except that it supports a wide variety of measurements.
  // We will use three of them.  The first is required, the second two are optional.

  SpanCharacteristic *airQuality;                 // reference to the Air Quality Characteristic, which is an integer from 0 to 5
  SpanCharacteristic *o3Density;                  // reference to the Ozone Density Characteristic, which is a float from 0 to 1000
  SpanCharacteristic *no2Density;                 // reference to the Nitrogen Dioxide Characteristic, which is a float from 0 to 1000
  
  DEV_AirQualitySensor(ServiceType sType=ServiceType::Regular) : Service::AirQualitySensor(sType){       // constructor() method

    new SpanEvent(10000);                        // check for events on this Service every 10 seconds
    
    airQuality=new Characteristic::AirQuality(1);                         // instantiate the Air Quality Characteristic and set initial value to 1
    o3Density=new Characteristic::OzoneDensity(300.0);                    // instantiate the Ozone Density Characteristic and set initial value to 300.0
    no2Density=new Characteristic::NitrogenDioxideDensity(700.0);         // instantiate the Nitrogen Dioxide Density Characteristic and set initial value to 700.0
    
    Serial.print("Configuring Air Quality Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  void event(){

    airQuality->setVal((airQuality->getVal()+1)%6);           // simulate a change in Air Quality by incrementing the current value by one, and keeping in range 0-5
    o3Density->setVal((double)random(200,500));               // change the Ozone Density to some random value between 200 and 499.  Note use of (double) cast since random returns an integer

    // Note we are NOT updating the Nitrogen Dioxide Density Characteristic.  This should therefore remain steady at its initial value of 700.0
       
  } // event
};
      
//////////////////////////////////

// What you should see in your HomeKit Application
//
// If you load the above example, your HomeKit App should display two new tiles: one labeled "Temp Sensor" and the other labeled "Air Quality".
// The Temp Sensor should indicate a temperature in the range of 10C to 35C (50F to 95F), which automatically updates every 5 seconds. 
// The Air Quality, ranging from 0-5, should change states once every 10 seconds.  States are displayed in HomeKit as "Unknown", "Excellent", "Good", "Fair",
// "Inferior" and "Poor".
//
// HomeKit Tiles generally only display the value of one required Characteristic and maybe one optional Characteristic.  In the case of an Air Quality Sensor,
// only the state of the Air Quality is displayed.  To see all the oher Characteristics, such as Ozone Density and Nitrogen Dioxide Density, you need to click
// on the tile, AND open the settings screen (would be nicer if HomeKit displayed these values on the control screen instead of making you open the settings screen).
// On the setting screen you should see the values of all three of the Characteristics we instantiated: Air Quality, Nitrogen Dioxide Density, and Ozone Density.
// Both the Air Quality and Ozone Density should change every 10 seconds.  The Nitrogen Dioxide Density should remain steady at the initial value of 700.0.
//
// If you run HomeSpan at a VERBOSITY level of 2 (specified in the library's Settings.h file), you can see the under the hood HomeSpan is sending Event Notification
// messages to all registered controllers every 5 seconds for the Temp Sensor, and every 10 seconds for the Air Quality Sensor.  If you lok carefully you'll see that
// the Event Notification message for the Air Quality Sensor only include two values - one for the Air Quality state and one for the Ozone Density.  HomeSpan is NOT
// sending a value for the Nitrogen Dioxide Density Characteristic since it has not been changed with a setVal() function.  This is an important design feature and
// shows that the instantiation of a new SpanEvent only determines how often the event() method is checked by HomeSpan.  If the event() method ALWAYS updates a
// Characteristic, then an Event Notification will always be generated.  However, if event() does not update a Characteristic, then no message will be generated.
// This allows you to create a SpanEvent.
