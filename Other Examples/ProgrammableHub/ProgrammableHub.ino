/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2022 Gregg E. Berman
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
// Demonstrates how to implement a Web Server alongside   //
// of HomeSpan to create a Programmable Hub serving up to //
// 16 Configurable Lights.                                //
//                                                        //
////////////////////////////////////////////////////////////

#include "HomeSpan.h" 
#include "DEV_LED.h"     
#include "DEV_Identify.h"       

#include <WebServer.h>                    // include WebServer library
WebServer webServer(80);                  // create WebServer on port 80

#define NLIGHTS 16                        // maximum number of Lightbulb Accessories

uint8_t pinList[]={0,4,5,12,14,15,16,17,18,19,22,23,25,26,27,32,33};      // list of allowed pins
char lightNames[NLIGHTS][9];                                              // storage for default light names 

nvs_handle lightNVS;                                                      // handle for NVS storage

struct {                                                                  // structure to store pin numbers and dimmable flag
  uint8_t pin=0;
  uint8_t dimmable=0;
} lightData[NLIGHTS];

////////////////////////////////////////////////////////////

void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(1);

  homeSpan.setHostNameSuffix("");         // use null string for suffix (rather than the HomeSpan device ID)
  homeSpan.setPortNum(1201);              // change port number for HomeSpan so we can use port 80 for the Web Server
  homeSpan.enableOTA();                   // enable OTA updates
  homeSpan.setMaxConnections(5);          // reduce max connection to 5 (default is 8) since WebServer and a connecting client will need 2, and OTA needs 1
  homeSpan.setWifiCallback(setupWeb);     // need to start Web Server after WiFi is established 
  
  homeSpan.begin(Category::Bridges,"HomeSpan Light Hub","homespanhub");

  for(int i=0;i<NLIGHTS;i++)                              // create default names for each light
    sprintf(lightNames[i],"Light-%02d",i+1);

  size_t len;  
  nvs_open("LIGHTS",NVS_READWRITE,&lightNVS);             // open LIGHTS NVS
  if(!nvs_get_blob(lightNVS,"LIGHTDATA",NULL,&len))       // if data found
    nvs_get_blob(lightNVS,"LIGHTDATA",&lightData,&len);   // retrieve data

  // Create Bridge Accessory
  
  new SpanAccessory(1);  
    new DEV_Identify("HomeSpan Hub","HomeSpan","LS-123","Light Server","1.0",3);
    new Service::HAPProtocolInformation();
      new Characteristic::Version("1.1.0");

  // Dynamically create a new Accessory for each Light defined

  for(int i=0;i<NLIGHTS;i++){
    if(lightData[i].pin>0){
      new SpanAccessory(i+2);
        new DEV_Identify(lightNames[i],"HomeSpan",lightNames[i],lightData[i].dimmable?"Dimmable":"Not Dimmable","1.0",0);    
        new DEV_GenericLED(lightData[i].pin,lightData[i].dimmable);       
    }
  }
  
} // end of setup()

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  webServer.handleClient();               // need to process webServer once each loop
  
} // end of loop()

//////////////////////////////////////

void setupWeb(){
  Serial.print("Starting Light Server Hub...\n\n");
  webServer.begin();

  // Create web routines inline

  webServer.on("/addForm", []() {
    String content="<html><body><h3>Add New Light Accessory</h3>";

    content+="<form action='/addLight' method='get'>";
    content+="<label for='lightName'>Light Name:</label>";
    content+="<input type='text' id='lightName' name='lightName' maxlength='24' required><br><br>";
    content+="<input type='radio' id='0' name='isDimmable' value='Not Dimmable'>";

    content+="<br><input type='submit' value='Add'>";
    content+="<button type='button' onclick='window.history.back();'>Cancel</button>";

    content+="</form>";
    content+="</body></html>";
    
    webServer.send(200, "text/html", content);
    
  });  
  
  webServer.on("/", []() {
  
    String content = "<html><body><form action='/configure' method='POST'><b>HomeSpan Light Server Hub Configuration</b><br><br>";
    content += "Select pins and check box if dimmable:<br><br>";

    for(int i=0;i<NLIGHTS;i++){
      content += "<span style=\"color:";
      if(lightData[i].pin==0)
        content += "grey";
      else if(lightData[i].dimmable)
        content += "red";
      else
        content += "blue";
      content += ";\">";
        
      content += String(lightNames[i]) + ": ";
      
      content += "<select name='p" + String(i) + "'>";
      for(int j=0;j<sizeof(pinList);j++){
        content += "<option value='" + String(pinList[j]) + "'";
        if(lightData[i].pin==pinList[j])
          content += " selected";
        content += ">";
        if(j>0)
          content += "Pin " + String(pinList[j]);
        else
          content += "None";
        content += "</option>";
      }
      content += "</select> ";
      content += "<input type='checkbox' value='1'";
      if(lightData[i].dimmable)
        content += " checked";
      content += " name='t" + String(i) + "'></span><br>";      
    }
    
    content += "<br><input type='reset'><input type='submit' value='Update'></form><br>";
    
    webServer.send(200, "text/html", content);
    
  });  

  webServer.on("/configure", []() {

    for(int i=0;i<NLIGHTS;i++)      // clear dimmable status since checkboxes only provide data if box is checked
      lightData[i].dimmable=0;
    
    for(int i=0;i<webServer.args();i++){
      switch(webServer.argName(i).charAt(0)){
        case 'p':
          lightData[webServer.argName(i).substring(1).toInt()].pin=webServer.arg(i).toInt();
          break;
        case 't':
          lightData[webServer.argName(i).substring(1).toInt()].dimmable=webServer.arg(i).toInt();
          break;
      }
    }

    String content = "<html><body>Settings Saved!<br><br>";

    for(int i=0;i<NLIGHTS;i++)
      if(lightData[i].pin)
        content += lightNames[i] + String(": Pin=") + String(lightData[i].pin) + String(lightData[i].dimmable?" Dimmable":"") + "<br>";
      else
        lightData[i].dimmable=0;
    
    content += "<br><button onclick=\"document.location='/'\">Return</button> ";
    content += "<button onclick=\"document.location='/reboot'\">Reboot</button>";

    nvs_set_blob(lightNVS,"LIGHTDATA",&lightData,sizeof(lightData));        // update data
    nvs_commit(lightNVS);                                                   // commit to NVS

    webServer.send(200, "text/html", content);
  
  });

  webServer.on("/reboot", []() {
    
    String content = "<html><body>Rebooting!  Will return to configuration page in 10 seconds.<br><br>";
    content += "<meta http-equiv = \"refresh\" content = \"10; url = /\" />";
    webServer.send(200, "text/html", content);

    for(int j=0;j<sizeof(pinList);j++)            // this seems to be needed to ensure all pins are disconnected from led PWM on reboot
      gpio_reset_pin((gpio_num_t)pinList[j]);     // otherwise ESP32 seems to be retaining some info about pins connectivity?
      
    ESP.restart();
  });

} // setupWeb
