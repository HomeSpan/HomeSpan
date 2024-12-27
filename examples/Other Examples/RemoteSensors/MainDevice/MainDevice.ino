/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
////////////////////////////////////////////////////////////
//                                                        //
//    HomeSpan: A HomeKit implementation for the ESP32    //
//    ------------------------------------------------    //
//                                                        //
//     Demonstrates how to use SpanPoint() to implement   //
//     two remote temperature sensors on separate ESP32   //
//     devices.                                           //
//                                                        //
//     This sketch is for the MAIN DEVICE that contains   //
//     all the usual HomeSpan logic, plus two instances   //
//     of SpanPoint to read temperatures from two other   //
//     remote devices.                                    //
//                                                        //
////////////////////////////////////////////////////////////

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
    new RemoteTempSensor("Device 1","BC:FF:4D:40:8E:71",true);        // pass MAC Address of Remote Device with flag noting it is an ESP8266

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Outdoor Temp");
    new RemoteTempSensor("Device 2","84:CC:A8:11:B4:84");             // pass MAC Address of Remote Device

  
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////
