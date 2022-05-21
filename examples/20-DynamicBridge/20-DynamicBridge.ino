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
 
#include "HomeSpan.h"

#include <array>

nvs_handle savedData;
std::array<int,10> lights;

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  size_t len;  
  nvs_open("SAVED_DATA",NVS_READWRITE,&savedData);       // open SAVED DATA
  if(!nvs_get_blob(savedData,"LIGHTS",NULL,&len))        // if LIGHTS data found
    nvs_get_blob(savedData,"LIGHTS",&lights,&len);       // retrieve data

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Lighting,"HomeSpan Lights");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Model("HomeSpan Dynamic Bridge");

  for(auto it=lights.begin(); it!=lights.end() && *it!=0; it++)
    addLight(*it);

  new SpanUserCommand('a',"<num> - add a new light accessory with id=<num>",addAccessory);
  new SpanUserCommand('d',"<num> - delete a light accessory with id=<num>",deleteAccessory);
  new SpanUserCommand('D'," - delete ALL light accessories",deleteAllAccessories);  
  new SpanUserCommand('u',"- update accessories database",updateAccessories);
  
}

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();
  
}

///////////////////////////

void addLight(int n){

  char name[32];
  sprintf(name,"Light-%d",n);
  char sNum[32];
  sprintf(sNum,"%0.10d",n);

  Serial.printf("Adding Accessory: %s\n",name);
  
  new SpanAccessory(n+1);                       // add 1, since first Accessory is reserved for the bridge
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name(name);
      new Characteristic::SerialNumber(sNum);
    new Service::LightBulb();
      new Characteristic::On(0,true);  
}

///////////////////////////

void addAccessory(const char *buf){
 
  int n=atoi(buf+1);

  if(n<1){
    Serial.printf("Invalid Accessory number!\n");
    return;
  }

  if(std::find(lights.begin(),lights.end(),n)!=lights.end()){
    Serial.printf("Accessory Light-%d already implemented!\n",n);
    return;
  }
  
  auto it=std::find(lights.begin(),lights.end(),0);
  
  if(it==lights.end()){
    Serial.printf("Can't add any more lights - max is %d!\n",lights.size());
    return;
  }

  *it=n;                                                        // save light number
  nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));      // update data
  nvs_commit(savedData); 
  addLight(n);                                                  // add light accessory
}

///////////////////////////

void deleteAccessory(const char *buf){

  int n=atoi(buf+1);

  if(n<1){
    Serial.printf("Invalid Accessory number!\n");
    return;
  }

  if(homeSpan.deleteAccessory(n+1)!=0){
    Serial.printf("No such Accessory: Light-%d\n",n);
    return;
  }

  Serial.printf("Deleting Accessory: Light-%d\n",n);

  auto it=std::remove(lights.begin(),lights.end(),n);
  *it=0;                                                        // overwrite end with a 0
  nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));      // update data
  nvs_commit(savedData); 
}

///////////////////////////

void deleteAllAccessories(const char *buf){

  if(lights[0]==0){
    Serial.printf("There are no Light Accessories to delete!\n");
    return;
  }

  for(auto it=lights.begin(); it!=lights.end() && *it!=0; it++)
    homeSpan.deleteAccessory(*it+1);

  std::fill(lights.begin(),lights.end(),0);
  nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));      // update data
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
