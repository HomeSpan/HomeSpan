
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

struct DEV_TempSensor : Service::TemperatureSensor {     // A standalone temperature sensor

  SpanCharacteristic *temp;                         // reference to the Current Temperature Characteristic
  float step=0.5;
  
  DEV_TempSensor(ServiceType sType=ServiceType::Regular) : Service::TemperatureSensor(sType){       // constructor() method

    temp=new Characteristic::CurrentTemperature();                 
    temp->value.FLOAT=22.0;
    
    Serial.print("Configuring Temperature Sensor");   // initialization message
    Serial.print("\n");

  } // end constructor

  SpanCharacteristic *event(){

    temp->value.FLOAT+=step;
    if(temp->value.FLOAT>28.0)
      step=-step;
    else if(temp->value.FLOAT<18.0)
      step=-step;

   return(temp);
    
  } // event
};
      
//////////////////////////////////
