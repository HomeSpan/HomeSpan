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
//      24 Configurable Lights.  Allows for dynamic changes     //
//      to Accessories without needing to reboot                //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "HomeSpan.h"
 
#define MAX_LIGHTS        24
#define MAX_NAME_LENGTH   32

enum colorType_t : uint8_t{
  NO_COLOR,
  TEMPERATURE_ONLY,
  FULL_RGB
};

struct lightData_t {
  char name[MAX_NAME_LENGTH+1]="";
  union {
    struct {
      boolean isConfigured:1;
      boolean isDimmable:1;
      colorType_t colorType:2;
    };
    uint8_t val=0;
  };
} lightData[MAX_LIGHTS];

nvs_handle savedData;

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  size_t len;  
  nvs_open("SAVED_DATA",NVS_READWRITE,&savedData);       // open a new namespace called SAVED_DATA in the NVS
  
  if(!nvs_get_blob(savedData,"LIGHTDATA",NULL,&len))        // if LIGHTDATA data found
    nvs_get_blob(savedData,"LIGHTDATA",&lightData,&len);       // retrieve data

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Lighting,"HomeSpan Light Hub");

  new SpanAccessory(1);                                   // here we specified the AID=1 for clarity (it would default to 1 anyway if left blank)
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Model("HomeSpan Programmable Hub");

  for(int i=0;i<MAX_LIGHTS;i++){                         // create Light Accessories based on saved data
    if(lightData[i].isConfigured)
      addLight(i,lightData[i].name,lightData[i].isDimmable,lightData[i].colorType);
  }
    
  new SpanUserCommand('a',"<name> - add non-dimmable light accessory using name=<name>",[](const char *c){addLight(-1,c+1,false,NO_COLOR);});
  new SpanUserCommand('A',"<name> - add dimmable light accessory using name=<name>",[](const char *c){addLight(-1,c+1,true,NO_COLOR);});
  new SpanUserCommand('t',"<name> - add non-dimmable light accessory with color-temperature control using name=<name>",[](const char *c){addLight(-1,c+1,false,TEMPERATURE_ONLY);});
  new SpanUserCommand('T',"<name> - add dimmable light accessory with color-temperature control using name=<name>",[](const char *c){addLight(-1,c+1,true,TEMPERATURE_ONLY);});
  new SpanUserCommand('r',"<name> - add non-dimmable light accessory with full RGB color control using name=<name>",[](const char *c){addLight(-1,c+1,false,FULL_RGB);});
  new SpanUserCommand('R',"<name> - add dimmable light accessory with full RGB color control using name=<name>",[](const char *c){addLight(-1,c+1,true,FULL_RGB);});

  new SpanUserCommand('l'," - list all light accessories",listAccessories);
  new SpanUserCommand('d',"<index> - delete a light accessory with index=<index>",deleteAccessory);
  new SpanUserCommand('D'," - delete ALL light accessories",deleteAllAccessories);  
  new SpanUserCommand('u',"- update accessories database",updateAccessories);

  homeSpan.autoPoll();
  
} // end of setup()

///////////////////////////

void addLight(int index, const char *name, boolean isDimmable, colorType_t colorType){

  if(index<0){
    for(index=0;index<MAX_LIGHTS && lightData[index].isConfigured;index++);
    if(index==MAX_LIGHTS){
      Serial.printf("Can't add Light Accessory - maximum number of %d are already defined.\n",MAX_LIGHTS);
      return;
    }
   
    int n=strncpy_trim(lightData[index].name,name,sizeof(lightData[index].name));

    if(n==1){
      Serial.printf("Can't add Light Accessory without a name specified.\n");
      return;
    }

    if(n>sizeof(lightData[index].name))
      Serial.printf("Warning - name trimmed to max length of %d characters.\n",MAX_NAME_LENGTH);
    
    lightData[index].isDimmable=isDimmable;
    lightData[index].colorType=colorType;
    lightData[index].isConfigured=true;
    name=lightData[index].name;

    nvs_set_blob(savedData,"LIGHTDATA",&lightData,sizeof(lightData));      // update data in the NVS
    nvs_commit(savedData);     
  }

  Serial.printf("Adding Light Accessory:  Name='%s'  Dimmable=%s  Color=%s\n",name,isDimmable?"YES":"NO",colorType==NO_COLOR?"NONE":(colorType==TEMPERATURE_ONLY?"TEMPERATURE_ONLY":"FULL_RGB"));

  new SpanAccessory(index+2);                       // IMPORTANT: add 2, since first Accessory with AID=1 is already used by the Bridge Accessory
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name(name);
    new Service::LightBulb();
      new Characteristic::On(0,true);
      if(isDimmable)
        new Characteristic::Brightness(100,true);
      if(colorType==TEMPERATURE_ONLY)
        new Characteristic::ColorTemperature(200,true);        
      if(colorType==FULL_RGB){
        new Characteristic::Hue(0,true);
        new Characteristic::Saturation(0,true);
      }
    
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

void deleteAccessory(const char *buf){

  int n=atoi(buf+1);

  if(n<1 || n>MAX_LIGHTS){
    Serial.printf("Invalid Light Accessory index - must be between 1 and %d.\n",MAX_LIGHTS);
    return;
  }

  if(homeSpan.deleteAccessory(n+1)){                            // if deleteAccessory() is true, a match has been found
    Serial.printf("Deleting Light Accessory:  Name='%s'\n",lightData[n-1].name);

    lightData[n-1].isConfigured=false;
    nvs_set_blob(savedData,"LIGHTDATA",&lightData,sizeof(lightData));      // update data in the NVS
    nvs_commit(savedData);
    
  } else {   
    Serial.printf("Nothing to delete - there is no Light Accessory at index=%d.\n",n);
  }
}

///////////////////////////

void deleteAllAccessories(const char *buf){

  for(int i=0;i<MAX_LIGHTS;i++){
    lightData[i].isConfigured=false;
    homeSpan.deleteAccessory(i+2);
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
    if(lightData[i].isConfigured)
      Serial.printf("%5d  %8s  %5s  %-s\n",i+1,lightData[i].isDimmable?"YES":"NO",lightData[i].colorType==NO_COLOR?"NONE":(lightData[i].colorType==TEMPERATURE_ONLY?"TEMP":"RGB"),lightData[i].name);
  }
  
}

///////////////////////////
