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
// Example 20: Demonstrates various advance HomeSpan functions  //
//             by implementing a Bridge in which one or more    //
//             Lightbulb Accessories can be added and deleted   //  
//             *dynamically* without needing to restart the     //
//             device                                           //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "HomeSpan.h"

  // In Example 20 we will implement a bridge device supporting up to 10 Lightbulb Accessories.  However, rather than pre-specifying the number of Lights, we
  // will allow Light Accessories to be added and deleted dynamically by the user via the CLI.  Changes are reflected in the Home App without the need to restart
  // the device!  Note this example uses a variety of advanced HomeSpan functions, as well as some detailed features of both the ESP32-IDF and C++ that have not been used
  // in any of the previous examples.

  // We will use a C++ array with 10 elements containing integers representing the Light "ID" of each Lightbulb Accessory implemented.  An ID of zero means there is no
  // Light defined in that element.
  
#include <array>                // include the C++ standard library array container

std::array<int,10> lights;      // declare "lights" to be an array of 10 integers

using std::fill;                // place the std library function fill, remove, and find, into the global namespace so we can use them below without prefacing with "std::"
using std::remove;
using std::find;

  // We will use non-volatile storage (NVS) to store the lights array so that the device can restore the current configuration upon rebooting
  
nvs_handle savedData;           // declare savdData as a handle to be used with the NVS (see the ESP32-IDF for details on how to use NVS storage)


//////////////////////////////////////

void setup() {

  Serial.begin(115200);

  fill(lights.begin(),lights.end(),0);                   // initialize lights array with zeros in each of the 10 elements (no Light Accessories defined)
  
  size_t len;  
  nvs_open("SAVED_DATA",NVS_READWRITE,&savedData);       // open a new namespace called SAVED_DATA in the NVS
  if(!nvs_get_blob(savedData,"LIGHTS",NULL,&len))        // if LIGHTS data found
    nvs_get_blob(savedData,"LIGHTS",&lights,&len);       // retrieve data

  homeSpan.setLogLevel(1);

  homeSpan.begin(Category::Lighting,"HomeSpan Lights");

  // We begin by creating the Bridge Accessory

  new SpanAccessory(1);                                   // here we specified the AID=1 for clarity (it would default to 1 anyway if left blank)
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Model("HomeSpan Dynamic Bridge");     // defining the Model is optional

  // Now we create Light Accessories based on what is recorded in the lights array
  // We'll use C++ iterators to loop over all elements until we reach the end of the array, or find an element with a value of zero

  for(auto it=lights.begin(); it!=lights.end() && *it!=0; it++)       // loop over all elements (stopping when we get to the end, or hit an element with a value of zero)
    addLight(*it);                                                    // call addLight (defined further below) with an argument equal to the integer stored in that element

  // Next we create four user-defined CLI commands so we can add and delete Light Accessories from the CLI.
  // The functions for each command are defined further below.

  new SpanUserCommand('a',"<num> - add a new light accessory with id=<num>",addAccessory);
  new SpanUserCommand('d',"<num> - delete a light accessory with id=<num>",deleteAccessory);
  new SpanUserCommand('D'," - delete ALL light accessories",deleteAllAccessories);  
  new SpanUserCommand('u',"- update accessories database",updateAccessories);

  // Finally we call autoPoll to start polling the background.  Note this is purely optional and only used here to illustrate how to
  // use autoPoll - you could instead have called the usual homeSpan.poll() function by including it inside the Arduino loop() function

  homeSpan.autoPoll();
  
} // end of setup()

// Usually the Arduino loop() function would be defined somewhere here.  But since we used autoPoll in the setup() function,
// we don't have to define the loop() function at all in this sketch!  Why don't we get an error?  Because HomeSpan includes
// a default loop() function, which prevents the compiler from complaining about loop() being undefined.

///////////////////////////

// This function creates a new Light Accessory with n as the "ID".
// It is called initially in setup() above to create Light Accessories based
// on what was stored in the lights array.  It is also called in response to 
// typing 'a' into the CLI (see below), which dynamically adds a new Light Accessory
// while the device is running.

void addLight(int n){

  char name[32];
  sprintf(name,"Light-%d",n);                    // create the name of the device using the specified "ID"
  char sNum[32];
  sprintf(sNum,"%0.10d",n);                      // create serial number from the ID - this is helpful in case we rename the Light to something else using the Home App

  Serial.printf("Adding Accessory: %s\n",name);
  
  new SpanAccessory(n+1);                       // IMPORTANT: add 1, since first Accessory with AID=1 is already used by the Bridge Accessory
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name(name);
      new Characteristic::SerialNumber(sNum);
    new Service::LightBulb();
      new Characteristic::On(0,true);  
}

///////////////////////////

// This function is called in response to typing '@a <num>' into the CLI.
// It adds a new Light Accessory with ID=num, by calling addLight(num) above.

void addAccessory(const char *buf){
 
  int n=atoi(buf+1);                                // read the value of <num> specified

  if(n<1){                                          // ensure <num> is greater than 0
    Serial.printf("Invalid Accessory number!\n");
    return;
  }

  if(find(lights.begin(),lights.end(),n)!=lights.end()){              // search for this ID in the existing lights array - if found, report an error and return
    Serial.printf("Accessory Light-%d already implemented!\n",n);
    return;
  }
  
  auto it=find(lights.begin(),lights.end(),0);                        // find the next "free" element in the light array (the first element with a value of zero)
  
  if(it==lights.end()){                                                       // if there were no elements with a zero, the array is full and no new Lights can be added
    Serial.printf("Can't add any more lights - max is %d!\n",lights.size());
    return;
  }

  *it=n;                                                        // save light number
  nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));      // update data in the NVS
  nvs_commit(savedData); 
  addLight(n);                                                  // add light accessory by calling the function above!
}

///////////////////////////

// This function deletes an existing Light Accessory and is called
// in response to typing '@d <num>' into the CLI.

void deleteAccessory(const char *buf){

  int n=atoi(buf+1);                                  // same as above, we read the specified <num> and check that it is valid (i.e. greater than 0)

  if(n<1){
    Serial.printf("Invalid Accessory number!\n");
    return;
  }

  // Below we use the homeSpan method deleteAccessory(aid) to completely delete the Accessory with AID=n+1.
  // We add 1 because the AID of the first Light Accessory is 2, since the Bridge Accessory has an AID of 1.
  // The deleteAccessory() method returns true if an Accessory with matching AID is found, otherwise it returns false.
  // When deleting an Accessory, HomeSpan will print a delete message for every Service, Characteristic, loop() method,
  // button() method, and SpanButton, associated with that Accessory.  These are Level-1 Log messages, so you'll need
  // to have the Log Level in the sketch set to 1 or 2 to receive the output.

  if(homeSpan.deleteAccessory(n+1)){                            // if deleteAccessory() is true, a match has been found
    Serial.printf("Deleting Accessory: Light-%d\n",n);
  
    fill(remove(lights.begin(),lights.end(),n),lights.end(),0);     // remove entry from lights array and fill any undefined elements with zero
    nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));        // update data in the NVS
    nvs_commit(savedData);
    
  } else {   
    Serial.printf("No such Accessory: Light-%d\n",n);
  }
}

///////////////////////////

void deleteAllAccessories(const char *buf){

// This function is called in response to typing '@D' into the CLI.
// It deletes all Light Accessories

  if(lights[0]==0){                                                   // first check that there is at least one Light Accessory by checking for a non-zero ID in lights[0]
    Serial.printf("There are no Light Accessories to delete!\n");
    return;
  }

  for(auto it=lights.begin(); it!=lights.end() && *it!=0; it++)       // use an iterator to loop over all non-zero elements in the lights array...
    homeSpan.deleteAccessory(*it+1);                                  // ... and delete the matching Light Accessory (don't forgot to add 1 to the Light ID to form the AID)

  fill(lights.begin(),lights.end(),0);                          // zero out all the elements in the lights array, since all Light Accessories have been deleted
  nvs_set_blob(savedData,"LIGHTS",&lights,sizeof(lights));      // update data in the NVS
  nvs_commit(savedData);

  Serial.printf("All Light Accessories deleted!\n");
}

///////////////////////////

// Lastly we have the all-important updateAccessories function.
// This is called in response to typing '@u' into the CLI.
// Though the above functions can be used to add and delete Light Accessories
// dyammically, Controllers such as the Home App that are already connected to
// the device don't yet know additional Light Accessories have been added to (or
// deleted from) the overall Accessories datase.  To let them know, HomeSpan needs
// to increment the HAP Configuration Number and re-broadcast it via MDNS so all
// connected Controllers are aware that they need to request a refresh from the device.

// When you type '@u' into the CLI, you should see a lot of activity between the device
// and any connected Controllers as they request a refresh.  Be patient - it can take up to a
// minute for changes to be properly reflected in the Home App on your iPhone or Mac.

void updateAccessories(const char *buf){

  // note the updateDatabase() method returns true if the database has indeed changed (e.g. one or more new Light Accessories were added), or false if nothing has changed
  
  if(homeSpan.updateDatabase())
    Serial.printf("Accessories Database updated.  New configuration number broadcasted...\n");
  else
    Serial.printf("Nothing to update - no changes were made!\n");
}

///////////////////////////
