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

#define MAX_LIGHTS 10

nvs_handle savedData;
int accNum[MAX_LIGHTS]={0};

//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  size_t len;  
  nvs_open("SAVED_DATA",NVS_READWRITE,&savedData);        // open SAVED DATA
  if(!nvs_get_blob(savedData,"ACC_NUM",NULL,&len))        // if ACCESSORY NUMBER data found
    nvs_get_blob(savedData,"ACC_NUM",&accNum,&len);       // retrieve data

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Bridges,"HomeSpan Bridge");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Model("HomeSpan Dynamic Bridge");

  for(int i=0;i<MAX_LIGHTS;i++)            // add previously-saved accessories
    if(accNum[i]>0)
      addLight(accNum[i]);

  new SpanUserCommand('a'," <num> add a new light accessory with id=num",addAccessory);
  new SpanUserCommand('d'," <num> delete a light accessory with id=num",deleteAccessory);
  new SpanUserCommand('u',"update configuration database",[](const char *buf){homeSpan.updateDatabase();});
  
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

  int i;
  for(i=0;i<MAX_LIGHTS && accNum[i]>0 && accNum[i]!=n;i++);

  if(i==MAX_LIGHTS){
    Serial.printf("Can't add any more lights - max is %d!\n",MAX_LIGHTS);
    return;
  }

  if(accNum[i]>0){    
    Serial.printf("Accessory Light-%d already implemented!\n",n);
    return;
  }

  accNum[i]=n;  
  nvs_set_blob(savedData,"ACC_NUM",&accNum,sizeof(accNum));        // update data
  nvs_commit(savedData); 

  addLight(n);  
}

///////////////////////////

void deleteAccessory(const char *buf){

  int n=atoi(buf+1);

  if(n<1){
    Serial.printf("Invalid Accessory number!\n");
    return;
  }

  SpanAccessory *acc=homeSpan.getAccessory(n+1);
  
  if(!acc){
    Serial.printf("No such Accessory: Light-%d\n",n);
    return;
  }

  Serial.printf("Deleting Accessory: Light-%d\n",n);
  delete acc;

  int i;                                              // delete entry in accNum
  for(i=0;accNum[i]!=n;i++);
  accNum[i]=0;

  nvs_set_blob(savedData,"ACC_NUM",&accNum,sizeof(accNum));        // update data
  nvs_commit(savedData);   
}
