
// This is a placeholder .ino file that allows you to easily edit the contents of this library using the Arduino IDE,
// as well as compile and test from this point.  This file is ignored when the library is included in other sketches.
#include "HomeSpan.h"

struct RemoteTempSensor : Service::TemperatureSensor {

  SpanCharacteristic *temp;
  SpanCharacteristic *fault;
  SpanPoint *remoteTemp;
  const char *name;
  float temperature;
  
  RemoteTempSensor(const char *name, const char*macAddress, boolean is8266=false) : Service::TemperatureSensor(){

    this->name=name;
    
    temp=new Characteristic::CurrentTemperature(-10.0);      // set initial temperature
    temp->setRange(-50,100);                                 // expand temperature range to allow negative values

    fault=new Characteristic::StatusFault(1);                // set initial state = fault

    remoteTemp=new SpanPoint(macAddress,0,sizeof(float),1,is8266);    // create a SpanPoint with send size=0 and receive size=sizeof(float)

  } // end constructor

  void loop(){
       
    if(remoteTemp->get(&temperature)){      // if there is data from the remote sensor
      temp->setVal(temperature);            // update temperature
      fault->setVal(0);                     // clear fault
       
      LOG1("Sensor %s update: Temperature=%0.2f\n",name,temperature*9/5+32);
      
    } else if(remoteTemp->time()>60000 && !fault->getVal()){    // else if it has been a while since last update (60 seconds), and there is no current fault
      fault->setVal(1);                                         // set fault state
      LOG1("Sensor %s update: FAULT\n",name);
    }
    
  } // loop
  
};

//////////////////////////////////////

void setup() {
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Bridges,"Sensor Hub");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Indoor Temp");
    new RemoteTempSensor("Device 1","AC:67:B2:77:42:20");        // pass MAC Address of Remote Device

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Outdoor Temp");
    new RemoteTempSensor("Device 2","BC:FF:4D:40:8E:71",true);        // pass MAC Address of Remote Device with 8266 flag set (will use AP MAC Address)

  
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////
