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

//////////////////////////////////////////////////////////////////
//                                                              //
//       HomeSpan: A HomeKit implementation for the ESP32       //
//       ------------------------------------------------       //
//                                                              //
//      Demonstrates how to implement a Web Server alongside    //
//      of HomeSpan to create a Programmable Hub serving up to  //
//      12 Configurable Lights.  Allows for dynamic changes     //
//      to Accessories without needing to reboot                //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "HomeSpan.h"
#include <WebServer.h>                    // include WebServer library

WebServer webServer(80);                  // create WebServer on port 80
 
#define MAX_LIGHTS        12
#define MAX_NAME_LENGTH   32
#define HUB_NAME          "lighthub"

enum colorType_t : uint8_t {
  NO_COLOR,
  TEMPERATURE_ONLY,
  FULL_RGB
};

uint32_t aidStore=2;                      // keep track of unique AID numbers -  start with AID=2

struct lightData_t {
  char name[MAX_NAME_LENGTH+1]="";
  uint32_t aid=0;
  boolean isDimmable:1;
  colorType_t colorType:2;
} lightData[MAX_LIGHTS];

nvs_handle savedData;

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  size_t len;  
  nvs_open("SAVED_DATA",NVS_READWRITE,&savedData);       // open a new namespace called SAVED_DATA in the NVS
  
  if(!nvs_get_blob(savedData,"LIGHTDATA",NULL,&len))        // if LIGHTDATA data found
    nvs_get_blob(savedData,"LIGHTDATA",&lightData,&len);       // retrieve data

  nvs_get_u32(savedData,"AID",&aidStore);                   // get AID, if it exists

  homeSpan.setLogLevel(1);

  homeSpan.setHostNameSuffix("");             // use null string for suffix (rather than the HomeSpan device ID)
  homeSpan.setPortNum(1201);                  // change port number for HomeSpan so we can use port 80 for the Web Server
  homeSpan.setConnectionCallback(setupWeb);   // need to start Web Server after WiFi is established   

  homeSpan.begin(Category::Lighting,"HomeSpan Light Hub",HUB_NAME);

  new SpanAccessory(1);                                   // here we specified the AID=1 for clarity (it would default to 1 anyway if left blank)
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Model("HomeSpan Programmable Hub");
      new Characteristic::AccessoryFlags();

  for(int i=0;i<MAX_LIGHTS;i++){                         // create Light Accessories based on saved data
    if(lightData[i].aid)
      addLight(i);
  }
    
  new SpanUserCommand('a',"<name> - add non-dimmable light accessory using name=<name>",[](const char *c){addLight(c+1,false,NO_COLOR);});
  new SpanUserCommand('A',"<name> - add dimmable light accessory using name=<name>",[](const char *c){addLight(c+1,true,NO_COLOR);});
  new SpanUserCommand('t',"<name> - add non-dimmable light accessory with color-temperature control using name=<name>",[](const char *c){addLight(c+1,false,TEMPERATURE_ONLY);});
  new SpanUserCommand('T',"<name> - add dimmable light accessory with color-temperature control using name=<name>",[](const char *c){addLight(c+1,true,TEMPERATURE_ONLY);});
  new SpanUserCommand('r',"<name> - add non-dimmable light accessory with full RGB color control using name=<name>",[](const char *c){addLight(c+1,false,FULL_RGB);});
  new SpanUserCommand('R',"<name> - add dimmable light accessory with full RGB color control using name=<name>",[](const char *c){addLight(c+1,true,FULL_RGB);});

  new SpanUserCommand('l'," - list all light accessories",listAccessories);
  new SpanUserCommand('d',"<index> - delete a light accessory with index=<index>",[](const char *buf){deleteAccessory(atoi(buf+1));});
  new SpanUserCommand('D'," - delete ALL light accessories",deleteAllAccessories);  
  new SpanUserCommand('u',"- update accessories database",updateAccessories);
 
} // end of setup()

///////////////////////////

void loop(){
  homeSpan.poll();
  webServer.handleClient();           // handle incoming web server traffic
}

///////////////////////////

void addLight(int index){
  
  Serial.printf("Adding Light Accessory:  Name='%s'  Dimmable=%s  Color=%s\n",
    lightData[index].name,lightData[index].isDimmable?"YES":"NO",lightData[index].colorType==NO_COLOR?"NONE":(lightData[index].colorType==TEMPERATURE_ONLY?"TEMPERATURE_ONLY":"FULL_RGB"));

  new SpanAccessory(lightData[index].aid);
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name(lightData[index].name);
      char sNum[32];
      sprintf(sNum,"Light-%02d",index);
      new Characteristic::SerialNumber(sNum);
      
    new Service::LightBulb();
      new Characteristic::On(0,true);
      if(lightData[index].isDimmable)
        new Characteristic::Brightness(100,true);
      if(lightData[index].colorType==TEMPERATURE_ONLY)
        new Characteristic::ColorTemperature(200,true);        
      if(lightData[index].colorType==FULL_RGB){
        new Characteristic::Hue(0,true);
        new Characteristic::Saturation(0,true);
      }
  
}

///////////////////////////

int addLight(const char *name, boolean isDimmable, colorType_t colorType){

  int index=0;
  for(index=0;index<MAX_LIGHTS && lightData[index].aid;index++);
  
  if(index==MAX_LIGHTS){
    Serial.printf("Can't add Light Accessory - maximum number of %d are already defined.\n",MAX_LIGHTS);
    return(-1);
  }
 
  int n=strncpy_trim(lightData[index].name,name,sizeof(lightData[index].name));

  if(n==1){
    Serial.printf("Can't add Light Accessory without a name specified.\n");
    return(-1);
  }

  if(n>sizeof(lightData[index].name))
    Serial.printf("Warning - name trimmed to max length of %d characters.\n",MAX_NAME_LENGTH);
  
  lightData[index].isDimmable=isDimmable;
  lightData[index].colorType=colorType;
  lightData[index].aid=aidStore++;

  nvs_set_blob(savedData,"LIGHTDATA",&lightData,sizeof(lightData));      // update data in the NVS
  nvs_set_u32(savedData,"AID",aidStore);
  nvs_commit(savedData); 

  addLight(index);
  return(index);
}

///////////////////////////

size_t strncpy_trim(char *dest, const char *src, size_t dSize){

  while(*src==' ')                            // skip over any leading spaces
    src++;

  size_t sLen=strlen(src);                    // string length of src after skipping over leading spaces
  while(sLen>0 && src[sLen-1]==' ')           // shorten length to remove trailing spaces
    sLen--;

  size_t sSize=sLen+1;                        // add room for null terminator
    
  if(dest!=NULL)
    *stpncpy(dest,src,(dSize<sSize?dSize:sSize)-1)='\0';
    
  return(sSize);                              // return total size needed for entire trimmed string, including null terminator
}

///////////////////////////

void deleteAccessory(int index){

  if(index<0 || index>=MAX_LIGHTS){
    Serial.printf("Invalid Light Accessory index - must be between 0 and %d.\n",MAX_LIGHTS-1);
    return;
  }

  if(homeSpan.deleteAccessory(lightData[index].aid)){                            // if deleteAccessory() is true, a match has been found
    Serial.printf("Deleting Light Accessory:  Name='%s'\n",lightData[index].name);

    lightData[index].aid=0;
    nvs_set_blob(savedData,"LIGHTDATA",&lightData,sizeof(lightData));      // update data in the NVS
    nvs_commit(savedData);
    
  } else {   
    Serial.printf("Nothing to delete - there is no Light Accessory at index=%d.\n",index);
  }
}

///////////////////////////

void deleteAllAccessories(const char *buf){

  for(int i=0;i<MAX_LIGHTS;i++){
    homeSpan.deleteAccessory(lightData[i].aid);
    lightData[i].aid=0;
  }
  
  nvs_set_blob(savedData,"LIGHTDATA",&lightData,sizeof(lightData));      // update data in the NVS
  nvs_commit(savedData);

  Serial.printf("All Light Accessories deleted!\n");
}

///////////////////////////

void updateAccessories(const char *buf){
  
  if(homeSpan.updateDatabase())
    Serial.printf("Accessories Database updated.  New configuration number broadcasted...\n");
  else
    Serial.printf("Nothing to update - no changes were made!\n");
}

///////////////////////////

void listAccessories(const char *buf){

  Serial.printf("\nIndex  Dimmable  Color  Name\n");
  Serial.printf("-----  --------  -----  ");
  
  for(int i=0;i<MAX_NAME_LENGTH;i++)
    Serial.printf("-");
  Serial.printf("\n");
  for(int i=0;i<MAX_LIGHTS;i++){
    if(lightData[i].aid)
      Serial.printf("%5d  %8s  %5s  %-s\n",i,lightData[i].isDimmable?"YES":"NO",lightData[i].colorType==NO_COLOR?"NONE":(lightData[i].colorType==TEMPERATURE_ONLY?"TEMP":"RGB"),lightData[i].name);
  }
  Serial.printf("\n");
  
}

///////////////////////////

void setupWeb(int count){

  if(count>1)
    return;
  
  Serial.printf("Starting Light Server Hub at %s.local\n\n",HUB_NAME);
  webServer.begin();

  webServer.on("/", []() {
    
    String response = "<html><head><title>HomeSpan Programmable Light Hub</title>";
    response += "<style>table, th, td {border: 1px solid black; border-collapse: collapse;} th, td { padding: 5px; text-align: center; } </style></head>\n";
    response += "<body><h2>HomeSpan Lights</h2>";
    response += "<form action='/addLight' method='get'>";
    response += "<table><tr><th style='text-align:left;'>Accessory</th><th>Dim?</th><th>Color Control</th><th>Action</th></tr>";

    int openSlots=MAX_LIGHTS;
  
    for(int i=0;i<MAX_LIGHTS;i++){
      if(lightData[i].aid){
        response += "<tr><td style='text-align:left;'>" + String(lightData[i].name) + "</td>";
        response += "<td><input type='checkbox' disabled " + String(lightData[i].isDimmable?"checked>":">") + "</td>";
        response += "<td><input type='radio' disabled " + String(lightData[i].colorType==NO_COLOR?"checked>":">") + " NONE ";
        response += "<input type='radio' disabled " + String(lightData[i].colorType==TEMPERATURE_ONLY?"checked>":">") + " TEMP ONLY ";
        response += "<input type='radio' disabled " + String(lightData[i].colorType==FULL_RGB?"checked>":">") + " FULL COLOR </td>";
        response += "<td><button type='button' onclick=\"document.location='/deleteLight?index=" + String(i) + "'\">Delete Light</button></td>";
        response += "</tr>";
        openSlots--;
      }
    }

    response += "<tr><td style='text-align:left;'><input type='text' name='name' required placeholder='Type accessory name here...' size='"
             + String(MAX_NAME_LENGTH) + "' maxlength='" + String(MAX_NAME_LENGTH) + "'></td>";
    response += "<td><input type='checkbox' name='isDimmable'></td>";
    response += "<td><input type='radio' checked name='colorType' for='no_color' value='" + String(NO_COLOR) + "'><label for='no_color'> NONE </label>";
    response += "<input type='radio' name='colorType' for='temp_only' value='" + String(TEMPERATURE_ONLY) + "'><label for='temp_only'> TEMP ONLY </label>";
    response += "<input type='radio' name='colorType' for='full_rgb' value='" + String(FULL_RGB) + "'><label for='full_rgb'> FULL COLOR </label></td>";
    response += "<td><input type='submit' value='Add Light'" + String(openSlots?"":" disabled") + "></td>";
    response += "</tr>";      

    response += "</table>";
    response += "</form>";

    if(!openSlots)
      response += "<p>Can't add any more Light Accessories.  Max="+ String(MAX_LIGHTS) + "</p>";

    response += "<p>Press here to delete ALL Light Accessories: <button type='button' onclick=\"document.location='/deleteAll'\">Delete All Lights</button></p>";
    response += "<p>Press here to update the Home App when finished making changes: <button type='button' onclick=\"document.location='/update'\">Upddate HomeKit</button></p>";
    
    response += "</body></html>";
    webServer.send(200, "text/html", response);

  });

  webServer.on("/deleteLight", []() {

    int index=atoi(webServer.arg(0).c_str());

    String response = "<html><head><title>HomeSpan Programmable Light Hub</title><meta http-equiv='refresh' content = '3; url=/'/></head>";
    response += "<body>Deleting Light Accessory '" +  String(lightData[index].name) + "'...</body></html>";
    
    deleteAccessory(index);

    webServer.send(200, "text/html", response);

  });

  webServer.on("/deleteAll", []() {

    String response = "<html><head><title>HomeSpan Programmable Light Hub</title><meta http-equiv='refresh' content = '3; url=/'/></head>";
    response += "<body>Deleting All Light Accessories...</body></html>";    

    webServer.send(200, "text/html", response);
    deleteAllAccessories("");
    
  });  

  webServer.on("/update", []() {

    String response = "<html><head><title>HomeSpan Programmable Light Hub</title><meta http-equiv='refresh' content = '3; url=/'/></head><body>";
    
    if(homeSpan.updateDatabase())
      response += "Accessories Database updated.  New configuration number broadcasted...";
    else
      response += "Nothing to update - no changes were made...";

    response += "...</body></html>";      
    
    webServer.send(200, "text/html", response);

  });  

  webServer.on("/addLight", []() {

    colorType_t colorType=NO_COLOR;
    boolean isDimmable=false;
    int iName=-1;
     
    for(int i=0;i<webServer.args();i++){
      if(!webServer.argName(i).compareTo(String("colorType")))
        colorType=(colorType_t)webServer.arg(i).toInt();
      else if(!webServer.argName(i).compareTo(String("isDimmable")))
        isDimmable=true;
      else if(!webServer.argName(i).compareTo(String("name")))
        iName=i;
    }

    String response = "<html><head><title>HomeSpan Programmable Light Hub</title><meta http-equiv='refresh' content = '3; url=/'/></head><body>";

    if(iName!=-1){
      int index=addLight(webServer.arg(iName).c_str(),isDimmable,colorType);
      response += "Adding Light Accessory '" +  String(lightData[index].name) + "'";
    } else
      response += "Error - bad URL request";

    response += "...</body></html>";
    
    webServer.send(200, "text/html", response);

  });


}

///////////////////////////
