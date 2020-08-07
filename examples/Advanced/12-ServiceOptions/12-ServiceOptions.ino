
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
// Example 12: Service Options:                           //
//             * setting the Primary Service              //
//             * setting Service Names                    //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"       

void setup() {

  // Every Accessory we define has at least two Services, one of which will always be the required Accessory Information Service (and
  // which we've conveniently wrapped into a derived Service we called DEV_Identify).  The second is usually a Service that performs
  // some sort of actual operation, such as the LightBulb Service or the Fan Service.  It's this second operative Service that creates
  // the controls we see in the HomeKit iOS or MacOS application.  These appear as a tile with a lightbulb control or fan control.
  //
  // But we've also created Accessories with more than two operational Service, such as our definition of a ceiling fan that includeds
  // BOTH a Fan Service and a LightBulb Service.  The HomeKit application can display an Accessory with two or more operational Services
  // in one of two ways, as detemined by the user within the HomeKit settings panel.  One way is to display each Service as a separate
  // tile, so that our ceiling fan would show up as one standalone lightbulb tile for the LightBulb Service and one standalone fan tile
  // for the Fan Service.  The second way is for HomeKit to display our ceiling fan as a single tile that shows both the fan control and
  // the lightbulb control on a single screen once you click the ceiling fan tile.
  //
  // HomeSpan has no control over whether HomeKit displays our two ceiling fan Services as two separate tiles or a combined single tile.
  // However, HomeSpan does have control over the icon used to display the ceiling fan in single-tile mode.  Should it be a lightbulb or
  // a fan?  HomeKit determines which icon to show in a single tile based on what is considered the Primary Service of the Accessory.
  // This can be set directly in HomeSpan for any given Service by specifying the ServiceType for a Service.  ServiceType is an optional
  // argument to the new Service constructor.  As an optional argument, it therefore needs to placed last.  There are three possible
  // ServiceTypes:  Regular, Primary, and Hidden, as defined in the HomeSpan library Settings.h file.  ServiceType::Regular is used to
  // specifiy a service that has no special settings.  This is the default used if you don't specify any ServiceType, which means that in
  // all our previous examples, ServiceType::Regular has been implied.
  //
  // Specifying ServiceType::Primary for a Service tells HomeSpan to set the "primary" attribute to "true" for that Service in the attribute
  // database.  HomeKit will use this information to set the icon for the tile.  It wil also list that Service first once you click the tile
  // to open the Accessory's controls.  Specifying ServiceType::Hidden for a Service tells HomeSpan to set the "hidden" attribute to "true"
  // for that Service.  This signals HomeKit to completely hide the controls for this service in the HomeKit interface.  We've not used
  // this feature in any of our examples - it's generally only used when you need to create an internal Service to be used by other Services,
  // but not by the actual end-user.
  //
  // As noted, ServiceType is an optional argument for the new Service constructor, so ServiceType::Primary can simply be added as the last
  // argument whenever we create a new Service to indicate it's the Primary one for the Accessory.  But this won't work for derived Services
  // we created, such as DEV_LED or DEV_DimmableLED.  This is because when we created those derived classes we did not include an optional
  // ServiceType argument in the constructor.  Doing so is very easy - you just need to modify the constructors, as shown for DEV_LED and
  // DEV_DimmableLED in the the DEV_LED.h file.  We even modified the constructor for DEV_Identify in DEV_Identify.h so we can specify when
  // it should be considered the Primary Service.  There is a good use case for that as we'll see below.
  //
  // Once we've modified our constructors accordingly, we're ready to specify which Service are Primary in any given Accessory, as shown in the
  // specific examples below.
  
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  // Here we create two Accessories configured to control a Ceiling Fan containing a bi-directional, multi-speed fan AND a dimmable light.
  // In Ceiling Fan #1 we specify the Dimmable LED as the Primary Service.  In Ceiling Fan #2 we specify the Fan as the Primary Service.
  // If you set HomeKit to display each of these Accessories as single tiles, you'll immediately see the difference.  Ceiling Fan #1 shows as 
  // Lightbulb Tile, and the dimmable LED is the first listed when you open the controls.  Ceiling Fan #2 shows as a Fan Tile, and the 
  // fan is the first listed when you open its controls.  Nice and easy.

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #1","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17,ServiceType::Primary);    
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #2","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
    new Service::Fan(ServiceType::Primary);                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);

  // In addition to being able to specify a Service to be a Primary Service, HomeKit also allows you to give names to each individual Services.
  // ...
      
  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #3","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");
    new Service::Fan(ServiceType::Primary);                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #4","HomeSpan","123-ABC","20mA LED","0.9",0,ServiceType::Primary);
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");


} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
