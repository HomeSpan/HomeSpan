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

#include "HomeSpan.h" 
#include "SpanCast.h"

//////////////////////////////////////

struct DEV_AirQualitySensor : Service::AirQualitySensor {

  Characteristic::AirQuality airQuality;
  Characteristic::OzoneDensity o3Density;
  Characteristic::NitrogenDioxideDensity no2Density;
  Characteristic::SulphurDioxideDensity so2Density;
  Characteristic::PM25Density pm25Density;
  Characteristic::PM10Density pm10Density;
  Characteristic::VOCDensity vocDensity;
  Characteristic::StatusActive active{Characteristic::StatusActive::NOT_FUNCTIONING};

  SpanCast *remoteSensor;

  struct SensorData {
    uint8_t quality;
    float o3;
    float no2;
    float so2;
    float pm25;
    float pm10;
    float voc;
  } sensorData;
  
  DEV_AirQualitySensor() : Service::AirQualitySensor(){

    SpanCast::configure(1);
    remoteSensor=new SpanCast(2,0,sizeof(sensorData));

    LOG0("Configured Air Quality Sensor.\n");
  }

  void loop() override {

    if(remoteSensor->get(&sensorData)){
      LOG0("Received new Air Quality Sensor data: quality=%d o3=%g no2=%g so2=%g pm25=%g pm10=%g voc=%g\n",
          sensorData.quality,sensorData.o3,sensorData.no2,sensorData.so2,sensorData.pm25,sensorData.pm10,sensorData.voc);

      airQuality.setVal(sensorData.quality);
      o3Density.setVal(sensorData.o3);
      no2Density.setVal(sensorData.no2);
      so2Density.setVal(sensorData.so2);
      pm25Density.setVal(sensorData.pm25);
      pm10Density.setVal(sensorData.pm10);
      vocDensity.setVal(sensorData.voc);
      active.setVal(Characteristic::StatusActive::FUNCTIONING);
    }

    if(active.getVal() && !remoteSensor->isActive()){
      LOG0("No longer receiving data from Air Quality Sensor!\n");
      active.setVal(Characteristic::StatusActive::NOT_FUNCTIONING);
      airQuality.setVal(Characteristic::AirQuality::UNKNOWN);
    }

  }
};

//////////////////////////////////////

void setup() {
 
  Serial.begin(115200);

  homeSpan.begin(Category::Sensors,"Air Quality Sensor");
  
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
      new Characteristic::Name("Backyard Air Quality");
    new DEV_AirQualitySensor();                                                          // Create an Air Quality Sensor (see DEV_Sensors.h for definition)

} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
 
}

//////////////////////////////////////
