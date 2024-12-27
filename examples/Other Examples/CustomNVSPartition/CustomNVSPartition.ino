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

// This example demonstrates the use of a custom Partition Scheme file: "partitions.csv"

// During compilation, if a file with this EXACT name is placed in the sketch folder,
// the esptool performing the compilation will use the partition scheme found
// in "partitions.csv" regardless of what partition scheme you selected in the Arduino IDE.

// Note if you change the partition scheme it is highly recommended that you fully erase the flash
// upon your next compile/upload by enabling the "Erase All Flash" option from the Arduino IDE menu.
// NOTE: remember to turn OFF this option after you've successully uploaded a sketch with the new
// partition scheme, else you will continue to erase everything saved in the NVS every time you upload
// a new sketch (which is likely NOT what you want to occur).

// The main reason for wanting to create your own partition scheme is to expand the NVS space.
// All of the pre-configured partition scheme you can select from the Arduino IDE provide
// for 504 records of NVS space. This is usuall sufficient for most HomeSpan projects, but if
// you have a LOT of Accessories (as per below) AND you are saving their states in NVS, you can
// use up all the NVS space.  If this occurs, HomeSpan will warn you of low NVS space upon boot-up.

// The custom partition scheme included in this sketch folder solves this problem by eliminating
// the SPIFFs partition (which is generally not used by HomeSpan) and using this portion of the flash
// to provide an NVS space with 3906 records --- more than enough for even the largest projects.

// For reference, in addition to HomeSpan's internal use of NVS (about 32 records), saving a
// numerical Characteristic consumes one additional NVS record, and saving a string Characteristic (of
// less than 32 characters) consumes two NVS records.  Also, the ESP32 WiFi stack consumes about 130
// additional NVS records once initialized.  As such, the sketch below requires:

//       32 records (internal HomeSpan use)
//    + 320 records (80 Accessories * 4 saved numerical Characterstics)
//    + 160 records (80 Accessories * 2 records per saved string Characterstic)
//    + 130 records (with WiFi initialized)
//    ----------------------------------------
//    = 642 NVS records needed (which exceeds the normal 504 limit, unless a custom partition scheme is used)

// Note that once HomeSpan is paired with HomeKit, additional NVS records will be consumed to store the
// pairing information for each verified HomeKit Controller.

// IMPORTANT!! When compiling under the Arduino IDE, the IDE itself first needs to make sure the size of your
// sketch is not larger than the size of the application partitions in your partition table.  However, the IDE
// has no access to your custom "partition.csv" file and instead computes the size of the application partitions
// based on the Partition Scheme you selected in the IDE menu, even though that scheme is not actually used if
// you have your own "partitions.csv" file, as in this example.  Before compiling, you MUST select a Partition Scheme
// in the IDE that has an application partition size matching the size of the application parititions in your
// custom "partitions.csv" file.

///////////////////////////////////////////////////////////////////////////////////////////

#include "HomeSpan.h"

#define MAX_LIGHTS  80                            // configure for 80 Light Accessories

struct RGB_Light : Service::LightBulb {

  Characteristic::On power{0,true};               // save these 4 numerical Characteristics  (4*80 = 320 NVS records)
  Characteristic::Hue H{0,true};
  Characteristic::Saturation S{0,true};
  Characteristic::Brightness V{0,true};

  int lightNumber;
  
  RGB_Light(int n) : Service::LightBulb(){

    lightNumber=n;
    LOG0("Configured RGB Light-%0d\n",lightNumber);           
  }

  boolean update(){

    if(power.updated())
      LOG0("Light-%d: Power=%s",lightNumber,power.getNewVal()?"ON":"OFF");
 
    if(H.updated())
      LOG0("Light-%d: Hue=%d",lightNumber,H.getNewVal());

    if(S.updated())
      LOG0("Light-%d: Saturation=%d",lightNumber,S.getNewVal());

    if(V.updated())
      LOG0("Light-%d: Brightness=%d",lightNumber,V.getNewVal());    
      
    return(false);
  
  }
};

void setup() {
 
  Serial.begin(115200);

  homeSpan.begin(Category::Lighting,"HomeSpan Max");

   new SpanAccessory();
    new Service::AccessoryInformation();  
      new Characteristic::Identify();

  for(int i=1;i<=MAX_LIGHTS;i++){
    char c[60];
    sprintf(c,"Light-%02d",i);
    
    new SpanAccessory();
      new Service::AccessoryInformation();
        new Characteristic::Identify();
        new Characteristic::Name(c,true);         // save this string Characteristic (2*80 = 160 NVS records)
        
      new RGB_Light(i);
  }

}

//////////////////////////////////////

void loop(){
 
  homeSpan.poll();  
}
