/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
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

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan Light");
  
  new SpanAccessory();   
    new Service::AccessoryInformation(); 
      new Characteristic::Identify();  
    new Service::LightBulb();
      new Characteristic::On();

//  new SpanUserCommand('k',"- list controllers",list_controllers);
  homeSpan.setControllerCallback(list_controllers);
}   


//////////////////////////////////////

void loop(){
 
  homeSpan.poll();  
}

//////////////////////////////////////


void list_controllers(){
  Serial.printf("\nControllers\n");
  for(auto it=homeSpan.controllerListBegin(); it!=homeSpan.controllerListEnd(); ++it){
    Serial.printf("Admin=%d  ID=",it->isAdmin());
    for(int i=0;i<36;i++)
      Serial.printf("%02X",it->getID()[i]);
    Serial.printf("  LTPK=");
    for(int i=0;i<32;i++)
      Serial.printf("%02X",it->getLTPK()[i]);
    Serial.printf("\n");
  }
}
