
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_TempSensor : Service::TemperatureSensor {     // A standalone Temperature sensor

  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  
  DEV_TempSensor(ServiceType sType=ServiceType::Regular) : Service::TemperatureSensor(sType){       // constructor() method

    temp=new Characteristic::CurrentTemperature();                 
    
    Serial.print("Configuring Temperature Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  void event(){

    temp->setVal((double)random(15,25));
       
  } // event
};
      
//////////////////////////////////

struct DEV_AirQualitySensor : Service::AirQualitySensor {     // A standalone Air Quality sensor

  SpanCharacteristic *airQuality;
  SpanCharacteristic *o3Density;
  SpanCharacteristic *no2Density;
  SpanCharacteristic *so2Density;
  SpanCharacteristic *pm25Density;
  SpanCharacteristic *pm10Density;
  SpanCharacteristic *vocDensity;
  
  DEV_AirQualitySensor(ServiceType sType=ServiceType::Regular) : Service::AirQualitySensor(sType){       // constructor() method

    airQuality=new Characteristic::AirQuality(1);                 
    o3Density=new Characteristic::OzoneDensity(40.7);                 
    no2Density=new Characteristic::NitrogenDioxideDensity(13.2);                 
    
    Serial.print("Configuring Air Quality Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  void event(){

    airQuality->setVal((int)random(1,6));
    o3Density->setVal((double)random(200,500));
    if(!random(2)){
      no2Density->setVal((double)random(600,800));
    }
       
  } // event
};
      
//////////////////////////////////
