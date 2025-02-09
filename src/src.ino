/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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
#include "FeatherPins.h"
#include "SpanRollback.h"

int watchDogSeconds=0;

void setup() {
 
  Serial.begin(115200);

  delay(1000);

//  homeSpan.setStatusPixel(18);
  homeSpan.setControlPin(0);
  
//  homeSpan.enableWatchdog();
  homeSpan.setLogLevel(2);
  homeSpan.enableOTA();
  homeSpan.setSketchVersion("1.9");
  homeSpan.enableWebLog();
  homeSpan.setCompileTime();
  homeSpan.setStatusCallback([](HS_STATUS status){Serial.printf("\n*** HOMESPAN STATUS: %s\n\n",homeSpan.statusString(status));});

  new SpanUserCommand('T'," - time delay",[](const char *buf){delay(20000);});
  new SpanUserCommand('B'," - rollback",[](const char *buf){esp_ota_mark_app_invalid_rollback_and_reboot();});
  new SpanUserCommand('v'," - validate sketch",[](const char *buf){homeSpan.markSketchOK();});
  
  new SpanUserCommand('w'," - watchdog",[](const char *buf){
    for(int i=0;i<CONFIG_FREERTOS_NUMBER_OF_CORES;i++){
    TaskHandle_t th;
    th=xTaskGetIdleTaskHandleForCore(i);
    char *name=pcTaskGetName(th);
    boolean wdt=(ESP_OK==esp_task_wdt_status(th));
    Serial.printf("%s: %s\n",name,wdt?"Enabled":"Disabled");
  }});
  
  new SpanUserCommand('e'," - enable HomeSpan watchdog",[](const char *buf){homeSpan.enableWatchdog(atoi(buf+1));});
  new SpanUserCommand('d'," - disable HomeSpan watchdog",[](const char *buf){homeSpan.disableWatchdog();});
             
  homeSpan.begin(Category::Lighting,"HomeSpan Test");

  new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();
    new Service::LightBulb();
      new Characteristic::On();

//  homeSpan.setPollingCallback([](){homeSpan.markSketchOK();});
//  sprintf(NULL,"HERE IS AN ERROR!");

homeSpan.autoPoll(8192,1,1);

//delay(20000);
//int i=0; while(1) i++;

}


//////////////////////////////////////

void loop(){
  
//  homeSpan.poll();
}
