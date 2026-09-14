/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2026 Gregg E. Berman
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
//     Demonstrates how to use SpanCast() to implement    //
//     two remote temperature sensors on standalone       //
//     ESP32 or ESP8266 devices.                          //
//                                                        //
//     This sketch is for the main HomeSpan Device that   //
//     contains all the usual HomeSpan logic, plus two    //
//     instances of SpanCast to read and write messages   //
//     (from and to) the two remote temperature-sensing   //
//     devices.                                           //
//                                                        //
//     Sketches for these remote devices can be found in  //
//     the IndoorTempSensor and OutdoorTempSensor         //
//     examples.                                          //
//                                                        //   
////////////////////////////////////////////////////////////

#include "HomeSpan.h"             // include HomeSpan as usual

#include "SpanCast.h"             // MUST ALSO INCLUDE THE SPANCAST LIBRARY!

//////////////////////////////////////

struct RemoteTempSensor : Service::TemperatureSensor {

  SpanCharacteristic *temp;
  SpanCharacteristic *active;
  SpanCast *remoteTemp;
  char *deviceName;
  uint32_t timer=0;
  
  RemoteTempSensor(const char *name, uint8_t deviceID) : Service::TemperatureSensor(){

    asprintf(&deviceName,"%s",name);                         // save name for diagnostic reporting
    
    temp=new Characteristic::CurrentTemperature(-10.0);      // create Temperature Characteristic and set initial temperature to -10 Celsius
    temp->setRange(-50,100);                                 // remember to expand the allowed temperature range to include negative values

    active=new Characteristic::StatusActive(0);              // create Active Characteristic and set initial state to false

    // Configure a SpanCast connection to send an arbitrary message of up to 48 bytes to the remote device,
    // as well as receive a 4-byte message containing the remote device temperature (as a float).  Receipt
    // of the temperature FROM the remote device is of course the purpose of the sketch.  Sending of an
    // arbitrary message TO the remote device serves no practical purpose and is only included in this sketch
    // to show an example of how to both send and receive messages to and from a remote device.

    remoteTemp=new SpanCast(deviceID,48,sizeof(float));      // create a SpanCast with ID=deviceID (passed as a parameter below), send size=48, and receive size=sizeof(float)

  } // end constructor

  void loop(){

    float temperature;
       
    if(remoteTemp->get(&temperature)){      // try to read temperature data from the remote sensor
      temp->setVal(temperature);            // if a message is available, update the temperature characteristic
      active->setVal(1);                    // and set Active to true
       
      LOG1("Sensor %s update: Temperature=%0.1f °F\n",deviceName,temperature*9/5+32);
      
    } else if(remoteTemp->time()>60000 && active->getVal()){     // else if it has been a while since last update (60 seconds), and the sensor is Active
      active->setVal(0);                                         // set Active to false
      LOG1("Sensor %s update: NOT ACTIVE\n",deviceName);
    }

    if(millis()-timer>12000 && active->getVal()){                // every 12 seconds, send an arbitrary message (max 48 bytes) back to remote sensor (for illustrative purposes only)
      timer=millis();
      char msg[48];
      sprintf(msg,"Confirming your temp is %0.1f °F!",temp->getVal<float>()*9/5+32);
      LOG1("Sending to %s: '%s'\n",deviceName,msg);
      if(remoteTemp->send(msg))
        LOG1("Send Succeeded\n");
      else
        LOG1("Send Failed\n");
    }  
  } // loop
};

//////////////////////////////////////

void setup() {
  
  Serial.begin(115200);

  homeSpan.setLogLevel(1);

  // Configure SpanCast to use Device ID = 18.  Defaults will be used for all other SpanCast settings

  SpanCast::configure(18);

  homeSpan.begin(Category::Bridges,"Sensor Hub");

  new SpanAccessory();  
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Indoor Temp");
    new RemoteTempSensor("Indoor Temp",46);               // create remote sensor "Indoor Temp" with SpanCast Device ID = 46

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Outdoor Temp");
    new RemoteTempSensor("Outdoor Temp",2);               // create remote sensor "Indoor Temp" with SpanCast Device ID = 2
  
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
} // end of loop()

//////////////////////////////////////
