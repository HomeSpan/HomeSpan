/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
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

#define MAX_LIGHTS  60

struct RGB_Light : Service::LightBulb {

  Characteristic::On power{0,true};
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{0,true};

  int lightNumber;
  
  RGB_Light(int n) : Service::LightBulb(){

    lightNumber=n;

    WEBLOG("Configured RGB Light-%d",lightNumber);
    WEBLOG("Light-%d: Saved Power Setting = %s",lightNumber,power.getVal()?"ON":"OFF");
    WEBLOG("Light-%d: Saved Hue Setting = %d",lightNumber,H.getVal());
    WEBLOG("Light-%d: Saved Saturation Setting = %d",lightNumber,S.getVal());
    WEBLOG("Light-%d: Saved Brightness Setting = %d",lightNumber,V.getVal());    
        
  }

  boolean update(){

    if(power.updated())
      WEBLOG("Light-%d: Power=%s",lightNumber,power.getNewVal()?"ON":"OFF");
 
    if(H.updated())
      WEBLOG("Light-%d: Hue=%d",lightNumber,H.getNewVal());

    if(S.updated())
      WEBLOG("Light-%d: Saturation=%d",lightNumber,S.getNewVal());

    if(V.updated())
      WEBLOG("Light-%d: Brightness=%d",lightNumber,V.getNewVal());    
      
    return(true);
  
  }
};

void setup() {
 
  Serial.begin(115200);

  homeSpan.setLogLevel(1);
  homeSpan.enableWebLog(500);

  homeSpan.begin(Category::Lighting,"HomeSpan Max");

   new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();

  for(int i=1;i<=MAX_LIGHTS;i++){
    char c[30];
    sprintf(c,"Light-%d",i);
    
    new SpanAccessory();
      new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(c);
        
      new RGB_Light(i);
  }

}

//////////////////////////////////////

void loop(){
 
  homeSpan.poll();  
}
