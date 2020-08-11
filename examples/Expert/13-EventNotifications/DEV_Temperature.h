
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_TempSensor : Service::TemperatureSensor {     // A standalone temperature sensor

  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  
  DEV_TempSensor(ServiceType sType=ServiceType::Regular) : Service::TemperatureSensor(sType){       // constructor() method

    temp=new Characteristic::CurrentTemperature();                 
    
    Serial.print("Configuring Temperature Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  void event(){

    temp->setVal(22.0);
       
  } // event
};
      
//////////////////////////////////
