
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
  // the controls we see in the HomeKit iOS or MacOS application.  These appear as tiles with a lightbulb control or fan control.

  // We've also created Accessories with more than two operational Service, such as our definition of a ceiling fan that includes
  // BOTH a Fan Service and a LightBulb Service.  The HomeKit application can display an Accessory with two or more operational Services
  // in one of two ways: one way is to display each Service as a separate tile, so that our ceiling fan would show up as one standalone
  // lightbulb tile for the LightBulb Service and one standalone fan tile for the Fan Service.  The second way is for HomeKit to display
  // our ceiling fan as a single tile that you click to open a new screen showing both the fan control and the lightbulb control side by side.

  // HomeSpan has no control over whether HomeKit displays multiple Services as separate tiles or as a single combined single tile.
  // This is determined by the user from within the HomeKit iOS or MacOS application (the default is to use the combined tile mode).
  // However, HomeSpan does have control over the which icon is used to display the ceiling fan in combined-tile mode.  Should it be a
  // lightbulb or a fan?
  
  // HomeKit determines which icon to show on the combined tile according to what is considered the Primary Service of the Accessory.
  // This can be set directly in HomeSpan for any given Service by setting its "ServiceType".  ServiceType is an optional argument to the
  // Service object constructor.  Note that as an optional argument, it needs to be placed last.
 
  // There are three possible ServiceTypes:  Regular, Primary, and Hidden, as defined in the HomeSpan library Settings.h file.
  // ServiceType::Regular is used to specifiy a service that has no special settings.  This is the default used if you don't specify any
  // ServiceType.  In all our previous examples, ServiceType::Regular has been implied.

  // Specifying ServiceType::Primary for a Service tells HomeSpan to set the "primary" attribute for that Service to "true" in the attribute
  // database.  HomeKit will use this information to deermine which icon to display for a combined tile.  HomeKit will also list the Primary Service
  // first when you click a combined tile to open its controls.
  
  // Specifying ServiceType::Hidden for a Service tells HomeSpan to set the "hidden" attribute for that Service to "true". This tells HomeKit
  // to completely hide the controls for this service in the HomeKit interface.  We've not used this feature in any of our examples.  It's generally
  // only needed to create an "internal" Service to be used by other Services, but not by the actual end-user.

  // As noted, ServiceType is an optional argument for the new Service constructor, so ServiceType::Primary can simply be added as the last
  // argument whenever we create a new Service to indicate it's the Primary one for the Accessory.  But this won't work for the derived Services
  // we created, such as DEV_LED or DEV_DimmableLED.  This is because when we created those derived classes we did not include an optional
  // ServiceType argument in the constructor.  Doing so is very easy - you just need to modify the constructors, as shown for DEV_LED and
  // DEV_DimmableLED in the the DEV_LED.h file.  We even modified the constructor for DEV_Identify in DEV_Identify.h so we can specify when
  // it should be considered the Primary Service.  There is a good use case for that as we'll see below.  Please see DEV_Identify.h and DEV_LED.h
  // for exactly how these new constructors are formulated.

  // Once we've modified our constructors accordingly, we're ready to specify which is the Primary Service for any given Accessory, as shown below.
  // First, we need to initialize HomeSpan and define our Bridge Accessory as in the previous examples:
  
  Serial.begin(115200);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();  
    new DEV_Identify("Bridge #1","HomeSpan","123-ABC","HS Bridge","0.9",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  // Next, we create two Accessories each configured to control a Ceiling Fan containing a bi-directional, multi-speed fan AND a dimmable light.
  // In Ceiling Fan #1 we specify the Dimmable LED as the Primary Service.  In Ceiling Fan #2 we specify the Fan as the Primary Service.
  // If you set HomeKit to display each of these Accessories as combined tiles, you'll immediately see the difference.  Ceiling Fan #1 shows as 
  // Lightbulb Tile with the dimmable LED listed first when you click open its controls.  Ceiling Fan #2 shows as a Fan Tile with the fan listed first
  // when you click open its controls.  Nice and easy.

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #1","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17,ServiceType::Primary);                                     // Here we specify DEV_DimmableLED as the Primary Service
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #2","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
    new Service::Fan(ServiceType::Primary);                                             // Here we specify the Fan as the Primary Service
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);

  //////////////////////////////////      

  // In addition to being able to specify which Service in an Accessory is the Primary Service, HomeKit also allows you to give names to each of
  // the individual Services.  This is done by instantiating a Name Characteristic for a Service.  We've already used this feature in creating
  // DEV_Identify --- the first argument is used to to name the Service (see DEV_Identify.h).  In fact, the Name Characteristic is required
  // for the AccessoryInformation Service, so we had to instantiate as part of DEV_Identify.

  // For all other Services the name Characteristic is optional.  If not instantiated, the name will be defaulted to whatever name we specified
  // in DEV_Identify, which means that if we have more than one operational Service in an Accessory, they will all be named the same.
  // This is not necessarily a problem since names don't always come into play in the HomeKit interface.  In the examples above, the only name
  // that gets displayed in combined-tile mode is "Ceiling Fan #1" or "Ceiling Fan #2", which makes sense.  When you click open the controls
  // for either Accessory you see a lightbulb control and a fan control.  They are not individually named, but the controls look different (one 
  // is a light control, the other a fan control) so there is no confusion.
    
  // If instead you set HomeKit to display the controls for these Accessories as separate tiles, you'll see that each of the light and fan controls
  // has their own name.  But since in the above examples we did not provide specific names for each of these Services, they will simply inherit the
  // name "Ceiling Fan #1" or "Ceiling Fan #2".  Again, there is no confusion since the light and fan controls each look different.

  // The situation becomes more interesting when you have an Accessory with 3 or more operational Services.  Sometimes HomeKit will display the names
  // of the Services on the control panel even in combined-tile mode.  Consider our ceiling fan example above, but with the added feature of a night-light,
  // which we will represent as a simple On/Off LED.  Let's instantiate the Name Characteristic for each Service, as shown below.
      
  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #3","HomeSpan","123-ABC","20mA LED","0.9",0);
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");                                           // Here we create a name for the Dimmable LED
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");                                          // Here we create a name for the On/Off LED
    new Service::Fan(ServiceType::Primary);                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");                                                  // Here we create a name for the Fan

  // If you let HomeKit display this as a single, combined tile, you'll notice two things.  The first is that the name of the tile is now "Fan" instead
  // of "Ceiling Fan #3".  Why is that?  It's because we set Fan to be the Primary Service AND gave it a name --- this is the name that shows up
  // on the combined tile.  If we did not give it a name, it would have inherited the name "Ceiling Fan #3", which would have been the name of the tile
  // as in the prior example.

  // The second thing you'll notice is that these names now appear next to each control if you click open the combined tile.  It says "Fan" next to the Fan
  // control, "Main Light" next to the Dimmable LED control, and "Night Light" next to the On/Off LED control.

  // If instead you tell HomeKit to display the controls for Ceiling Fan #3 as three separate tiles, you'll see that each tile contains the name specified
  // above for that Service.  In some circumstances that can be helpful, in others it can be confusing.  For instance, if you had two ceiling fans that
  // each had a main light and a night night, how would you know which "Main Light" is which?  One solution is to create names like "Main Light #3" and
  // "Main Light #4".  The other solution is to keep them combined in a single tile, but keep the name "Ceiling Fan #3" for the combined tile, instead of
  // having it over-ridden with the word "Fan"

  // This is easily done by specifying DEV_Identify as the Primary Service, instead of Fan, as follows:    

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #4","HomeSpan","123-ABC","20mA LED","0.9",0,ServiceType::Primary);      // specify DEV_Identify as the Primary Service
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");

  // HomeKit now shows the name "Ceiling Fan #3" for the combined tile AND it still shows the individual names for each control when you click open the tile.
  // The only downside to this configuration is that since the Fan is no longer specified as the Primary Service, the main icon on the combined tile now shows
  // as a lightbulb, instead of the fan.  HomeKit documentation is not clear on how the main icon is chosen under these circumstances, but I've found
  // that changing the order of Services as they are instantiated can impact the icon.  Here is the same example as above, but with the Fan 
  // instantiated as the first opertional Service, ahead of the Main Light and Night Night:

  new SpanAccessory();                                                          
    new DEV_Identify("Ceiling Fan #5","HomeSpan","123-ABC","20mA LED","0.9",0,ServiceType::Primary);      // specify DEV_Identify as the Primary Service
    new Service::Fan();                             
      new Characteristic::Active();             
      new Characteristic::RotationDirection();
      new Characteristic::RotationSpeed(0);
      new Characteristic::Name("Fan");
    new DEV_DimmableLED(0,17);    
      new Characteristic::Name("Main Light");
    new DEV_LED(16);    
      new Characteristic::Name("Night Light");

  // This seems to cause HomeKit to display the Fan icon on the combined tile, as well as to list the Fan Service control first when the tile is clicked
  // open, almost as if it were the Primary Service in all ways except for its name.

  // As you can see, there is no right or wrong way for how to name your Accessories and Services, including whether or not to even bother naming your
  // individual Services.  It's purely a matter of taste - experiment and see which combinations best serve your purposes.

  // IMPORTANT: HomeKit tries to cache as many items as possible, and although it should know when configurations change, it does not always respond
  // as expected.  If you are experimenting and find that name changes are NOT being reflected in the HomeKit interface, simply unpair HomeSpan and
  // re-pair.  This often causes a refresh.  If not, after unpairing you can additionally reset the HAP data in HomeSpan so that its HAP ID changes (see
  // HomeSpan documentation for how to easily do this).  This way when you re-pair, HomeKit should think this is a completely new device and start with
  // a clean slate.  *** In some limited circumstances, HomeKit may get so confused it refuses to operate the Accessories at all.  You may get a No Response
  // icon even though HomeSpan is operating correctly.  Though it is possible to misconfigure HomeSpan in ways that cause this, if you do get this
  // error please try the re-pairing methods above as it often fixes the problem. 
 
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()
