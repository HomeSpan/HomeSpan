/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2021 Gregg E. Berman
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
 
#include <ESPmDNS.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <esp_ota_ops.h>

#include "HomeSpan.h"
#include "HAP.h"

using namespace Utils;

HAPClient **hap;                    // HAP Client structure containing HTTP client connections, parsing routines, and state variables (global-scoped variable)
Span homeSpan;                      // HAP Attributes database and all related control functions for this Accessory (global-scoped variable)

///////////////////////////////
//         Span              //
///////////////////////////////

void Span::begin(Category catID, const char *displayName, const char *hostNameBase, const char *modelName){
  
  this->displayName=displayName;
  this->hostNameBase=hostNameBase;
  this->modelName=modelName;
  sprintf(this->category,"%d",(int)catID);

  controlButton.init(controlPin);
  statusLED.init(statusPin);

  int maxLimit=CONFIG_LWIP_MAX_SOCKETS-2-otaEnabled;
  if(maxConnections>maxLimit)
    maxConnections=maxLimit;

  hap=(HAPClient **)calloc(maxConnections,sizeof(HAPClient *));
  for(int i=0;i<maxConnections;i++)
    hap[i]=new HAPClient;

  hapServer=new WiFiServer(tcpPortNum);

  delay(2000);
 
  Serial.print("\n************************************************************\n"
                 "Welcome to HomeSpan!\n"
                 "Apple HomeKit for the Espressif ESP-32 WROOM and Arduino IDE\n"
                 "************************************************************\n\n"
                 "** Please ensure serial monitor is set to transmit <newlines>\n\n");

  Serial.print("Message Logs:     Level ");
  Serial.print(logLevel);  
  Serial.print("\nStatus LED:       Pin ");
  Serial.print(statusPin);  
  Serial.print("\nDevice Control:   Pin ");
  Serial.print(controlPin);
  Serial.print("\nSketch Version:   ");
  Serial.print(getSketchVersion());  
  Serial.print("\nHomeSpan Version: ");
  Serial.print(HOMESPAN_VERSION);
  Serial.print("\nESP-IDF Version:  ");
  Serial.print(esp_get_idf_version());
  
  #ifdef ARDUINO_VARIANT
    Serial.print("\nESP32 Board:      ");
    Serial.print(ARDUINO_VARIANT);
  #endif
  
  Serial.print("\nSketch Compiled:  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);

  Serial.print("\n\nDevice Name:      ");
  Serial.print(displayName);  
  Serial.print("\n\n");
      
}  // begin

///////////////////////////////

void Span::poll() {

  if(!strlen(category)){
    Serial.print("\n** FATAL ERROR: Cannot run homeSpan.poll() without an initial call to homeSpan.begin()!\n** PROGRAM HALTED **\n\n");
    while(1);    
  }

  if(!isInitialized){
  
    if(!homeSpan.Accessories.empty()){
      
      if(!homeSpan.Accessories.back()->Services.empty())
        homeSpan.Accessories.back()->Services.back()->validate();    
        
      homeSpan.Accessories.back()->validate();    
    }

    processSerialCommand("i");        // print homeSpan configuration info
   
    if(nFatalErrors>0){
      Serial.print("\n*** PROGRAM HALTED DUE TO ");
      Serial.print(nFatalErrors);
      Serial.print(" FATAL ERROR");
      if(nFatalErrors>1)
        Serial.print("S");
      Serial.print(" IN CONFIGURATION! ***\n\n");
      while(1);
    }    

    Serial.print("\n");
        
    HAPClient::init();        // read NVS and load HAP settings  

    if(!strlen(network.wifiData.ssid)){
      Serial.print("*** WIFI CREDENTIALS DATA NOT FOUND.  YOU MAY CONFIGURE BY TYPING 'W <RETURN>'.\n\n");
      statusLED.start(LED_WIFI_NEEDED);
    } else {
      homeSpan.statusLED.start(LED_WIFI_CONNECTING);
    }
          
    controlButton.reset();        

    Serial.print(displayName);
    Serial.print(" is READY!\n\n");
    isInitialized=true;
    
  } // isInitialized

  if(strlen(network.wifiData.ssid)>0){
      checkConnect();
  }

  char cBuf[17]="?";
  
  if(Serial.available()){
    readSerial(cBuf,16);
    processSerialCommand(cBuf);
  }

  WiFiClient newClient;

  if(newClient=hapServer->available()){                        // found a new HTTP client
    int freeSlot=getFreeSlot();                                // get next free slot

    if(freeSlot==-1){                                          // no available free slots
      freeSlot=randombytes_uniform(maxConnections);
      LOG2("=======================================\n");
      LOG1("** Freeing Client #");
      LOG1(freeSlot);
      LOG1(" (");
      LOG1(millis()/1000);
      LOG1(" sec) ");
      LOG1(hap[freeSlot]->client.remoteIP());
      LOG1("\n");
      hap[freeSlot]->client.stop();                     // disconnect client from first slot and re-use
    }

    hap[freeSlot]->client=newClient;             // copy new client handle into free slot

    LOG2("=======================================\n");
    LOG1("** Client #");
    LOG1(freeSlot);
    LOG1(" Connected: (");
    LOG1(millis()/1000);
    LOG1(" sec) ");
    LOG1(hap[freeSlot]->client.remoteIP());
    LOG1(" on Socket ");
    LOG1(hap[freeSlot]->client.fd()-LWIP_SOCKET_OFFSET+1);
    LOG1("/");
    LOG1(CONFIG_LWIP_MAX_SOCKETS);
    LOG1("\n");
    LOG2("\n");

    hap[freeSlot]->cPair=NULL;                   // reset pointer to verified ID
    homeSpan.clearNotify(freeSlot);             // clear all notification requests for this connection
    HAPClient::pairStatus=pairState_M1;         // reset starting PAIR STATE (which may be needed if Accessory failed in middle of pair-setup)
  }

  for(int i=0;i<maxConnections;i++){                     // loop over all HAP Connection slots
    
    if(hap[i]->client && hap[i]->client.available()){       // if connection exists and data is available

      HAPClient::conNum=i;                                // set connection number
      hap[i]->processRequest();                           // process HAP request
      
      if(!hap[i]->client){                                 // client disconnected by server
        LOG1("** Disconnecting Client #");
        LOG1(i);
        LOG1("  (");
        LOG1(millis()/1000);
        LOG1(" sec)\n");
      }

      LOG2("\n");

    } // process HAP Client 
  } // for-loop over connection slots

  HAPClient::callServiceLoops();
  HAPClient::checkPushButtons();
  HAPClient::checkNotifications();  
  HAPClient::checkTimedWrites();

  if(otaEnabled)
    ArduinoOTA.handle();

  if(controlButton.primed()){
    statusLED.start(LED_ALERT);
  }
  
  if(controlButton.triggered(3000,10000)){
    statusLED.off();
    if(controlButton.type()==PushButton::LONG){
      controlButton.wait();
      processSerialCommand("F");        // FACTORY RESET
    } else {
      commandMode();                    // COMMAND MODE
    }
  }
    
} // poll

///////////////////////////////

int Span::getFreeSlot(){
  
  for(int i=0;i<maxConnections;i++){
    if(!hap[i]->client)
      return(i);
  }

  return(-1);          
}

//////////////////////////////////////

void Span::commandMode(){
  
  Serial.print("*** ENTERING COMMAND MODE ***\n\n");
  int mode=1;
  boolean done=false;
  statusLED.start(500,0.3,mode,1000);

  unsigned long alarmTime=millis()+comModeLife;

  while(!done){
    if(millis()>alarmTime){
      Serial.print("*** Command Mode: Timed Out (");
      Serial.print(comModeLife/1000);
      Serial.print(" seconds).\n\n");
      mode=1;
      done=true;
      statusLED.start(LED_ALERT);
      delay(2000);
    } else
    if(controlButton.triggered(10,3000)){
      if(controlButton.type()==PushButton::SINGLE){
        mode++;
        if(mode==6)
          mode=1;
        statusLED.start(500,0.3,mode,1000);        
      } else {
        done=true;
      }
    } // button press
  } // while

  statusLED.start(LED_ALERT);
  controlButton.wait();
  
  switch(mode){

    case 1:
      Serial.print("*** NO ACTION\n\n");
      if(strlen(network.wifiData.ssid)==0)
        statusLED.start(LED_WIFI_NEEDED);
      else
      if(!HAPClient::nAdminControllers())
        statusLED.start(LED_PAIRING_NEEDED);
      else
        statusLED.on();
    break;

    case 2:
      processSerialCommand("R");
    break;    

    case 3:
      processSerialCommand("A");
    break;
      
    case 4:
      processSerialCommand("U");
    break;    
    
    case 5:
      processSerialCommand("X");
    break;    
    
  } // switch
  
  Serial.print("*** EXITING COMMAND MODE ***\n\n");
}

//////////////////////////////////////

void Span::checkConnect(){

  if(connected){
    if(WiFi.status()==WL_CONNECTED)
      return;
      
    Serial.print("\n\n*** WiFi Connection Lost!\n");      // losing and re-establishing connection has not been tested
    connected=false;
    waitTime=60000;
    alarmConnect=0;
    homeSpan.statusLED.start(LED_WIFI_CONNECTING);
  }

  if(WiFi.status()!=WL_CONNECTED){
    if(millis()<alarmConnect)         // not yet time to try to try connecting
      return;

    if(waitTime==60000)
      waitTime=1000;
    else
      waitTime*=2;
      
    if(waitTime==32000){
      Serial.print("\n*** Can't connect to ");
      Serial.print(network.wifiData.ssid);
      Serial.print(".  You may type 'W <return>' to re-configure WiFi, or 'X <return>' to erase WiFi credentials.  Will try connecting again in 60 seconds.\n\n");
      waitTime=60000;
    } else {    
      Serial.print("Trying to connect to ");
      Serial.print(network.wifiData.ssid);
      Serial.print(".  Waiting ");
      Serial.print(waitTime/1000);
      Serial.print(" second(s) for response...\n");
      WiFi.begin(network.wifiData.ssid,network.wifiData.pwd);
    }

    alarmConnect=millis()+waitTime;

    return;
  }

  connected=true;

  Serial.print("Successfully connected to ");
  Serial.print(network.wifiData.ssid);
  Serial.print("! IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  char id[18];                              // create string version of Accessory ID for MDNS broadcast
  memcpy(id,HAPClient::accessory.ID,17);    // copy ID bytes
  id[17]='\0';                              // add terminating null

  // create broadcaset name from server base name plus accessory ID (without ':')

  int nChars;

  if(!hostNameSuffix)
    nChars=snprintf(NULL,0,"%s-%.2s%.2s%.2s%.2s%.2s%.2s",hostNameBase,id,id+3,id+6,id+9,id+12,id+15);
  else
    nChars=snprintf(NULL,0,"%s%s",hostNameBase,hostNameSuffix);
    
  char hostName[nChars+1];
  
  if(!hostNameSuffix)
    sprintf(hostName,"%s-%.2s%.2s%.2s%.2s%.2s%.2s",hostNameBase,id,id+3,id+6,id+9,id+12,id+15);
  else
    sprintf(hostName,"%s%s",hostNameBase,hostNameSuffix);

  Serial.print("\nStarting MDNS...\n\n");
  Serial.print("HostName:      ");
  Serial.print(hostName);
  Serial.print(".local:");
  Serial.print(tcpPortNum);
  Serial.print("\nDisplay Name:  ");
  Serial.print(displayName);
  Serial.print("\nModel Name:    ");
  Serial.print(modelName);
  Serial.print("\nSetup ID:      ");
  Serial.print(qrID);
  Serial.print("\n\n");

  MDNS.begin(hostName);                         // set server host name (.local implied)
  MDNS.setInstanceName(displayName);            // set server display name
  MDNS.addService("_hap","_tcp",tcpPortNum);    // advertise HAP service on specified port

  // add MDNS (Bonjour) TXT records for configurable as well as fixed values (HAP Table 6-7)

  char cNum[16];
  sprintf(cNum,"%d",hapConfig.configNumber);
  
  mdns_service_txt_item_set("_hap","_tcp","c#",cNum);            // Accessory Current Configuration Number (updated whenever config of HAP Accessory Attribute Database is updated)
  mdns_service_txt_item_set("_hap","_tcp","md",modelName);       // Accessory Model Name
  mdns_service_txt_item_set("_hap","_tcp","ci",category);        // Accessory Category (HAP Section 13.1)
  mdns_service_txt_item_set("_hap","_tcp","id",id);              // string version of Accessory ID in form XX:XX:XX:XX:XX:XX (HAP Section 5.4)

  mdns_service_txt_item_set("_hap","_tcp","ff","0");             // HAP Pairing Feature flags.  MUST be "0" to specify Pair Setup method (HAP Table 5-3) without MiFi Authentification
  mdns_service_txt_item_set("_hap","_tcp","pv","1.1");           // HAP version - MUST be set to "1.1" (HAP Section 6.6.3)
  mdns_service_txt_item_set("_hap","_tcp","s#","1");             // HAP current state - MUST be set to "1"

  if(!HAPClient::nAdminControllers())                            // Accessory is not yet paired
    mdns_service_txt_item_set("_hap","_tcp","sf","1");           // set Status Flag = 1 (Table 6-8)
  else
    mdns_service_txt_item_set("_hap","_tcp","sf","0");           // set Status Flag = 0

  mdns_service_txt_item_set("_hap","_tcp","hspn",HOMESPAN_VERSION);           // HomeSpan Version Number (info only - NOT used by HAP)
  mdns_service_txt_item_set("_hap","_tcp","sketch",sketchVersion);            // Sketch Version (info only - NOT used by HAP)
  mdns_service_txt_item_set("_hap","_tcp","ota",otaEnabled?"yes":"no");       // OTA Enabled (info only - NOT used by HAP)

  uint8_t hashInput[22];
  uint8_t hashOutput[64];
  char setupHash[9];
  size_t len;
  
  memcpy(hashInput,qrID,4);                                           // Create the Seup ID for use with optional QR Codes.  This is an undocumented feature of HAP R2!
  memcpy(hashInput+4,id,17);                                          // Step 1: Concatenate 4-character Setup ID and 17-character Accessory ID into hashInput
  mbedtls_sha512_ret(hashInput,21,hashOutput,0);                      // Step 2: Perform SHA-512 hash on combined 21-byte hashInput to create 64-byte hashOutput
  mbedtls_base64_encode((uint8_t *)setupHash,9,&len,hashOutput,4);    // Step 3: Encode the first 4 bytes of hashOutput in base64, which results in an 8-character, null-terminated, setupHash
  mdns_service_txt_item_set("_hap","_tcp","sh",setupHash);            // Step 4: broadcast the resulting Setup Hash

  if(otaEnabled){
    if(esp_ota_get_running_partition()!=esp_ota_get_next_update_partition(NULL)){
      ArduinoOTA.setHostname(hostName);
      ArduinoOTA.setPasswordHash(otaPwd);
    
      ArduinoOTA
        .onStart([]() {
          String type;
          if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
          else // U_SPIFFS
            type = "filesystem";
          Serial.println("\n*** OTA Starting:" + type);
          homeSpan.statusLED.start(LED_OTA_STARTED);
        })
        .onEnd([]() {
          Serial.println("\n*** OTA Completed.  Rebooting...");
          homeSpan.statusLED.off();
        })
        .onProgress([](unsigned int progress, unsigned int total) {
          Serial.printf("*** Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
          Serial.printf("*** OTA Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed\n");
          else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed\n");
          else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed\n");
          else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed\n");
          else if (error == OTA_END_ERROR) Serial.println("End Failed\n");
        });    
      
      ArduinoOTA.begin();
      Serial.print("Starting OTA: ");
      Serial.print(displayName);
      Serial.print(" at ");
      Serial.print(WiFi.localIP());
      Serial.print("\n");
    } else {
      Serial.print("\n*** Warning: Can't enable OTA - Partition table used to compile this sketch is not configured for OTA.\n\n");
    }
  }

  Serial.print("Starting Web (HTTP) Server supporting up to ");
  Serial.print(maxConnections);
  Serial.print(" simultaneous connections...\n");
  hapServer->begin();

  Serial.print("\n");

  if(!HAPClient::nAdminControllers()){
    Serial.print("DEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
    statusLED.start(LED_PAIRING_NEEDED);
  } else {
    statusLED.on();
  }

  if(wifiCallback)
    wifiCallback();
  
} // initWiFi

///////////////////////////////

void Span::setQRID(const char *id){
  
  char tBuf[5];
  sscanf(id,"%4[0-9A-Za-z]",tBuf);
  
  if(strlen(id)==4 && strlen(tBuf)==4){
    sprintf(qrID,"%s",id);
  }
    
} // setQRID

///////////////////////////////

void Span::processSerialCommand(const char *c){

  switch(c[0]){

    case 's': {    
      
      Serial.print("\n*** HomeSpan Status ***\n\n");

      Serial.print("IP Address:        ");
      Serial.print(WiFi.localIP());
      Serial.print("\n\n");
      Serial.print("Accessory ID:      ");
      HAPClient::charPrintRow(HAPClient::accessory.ID,17);
      Serial.print("                               LTPK: ");
      HAPClient::hexPrintRow(HAPClient::accessory.LTPK,32);
      Serial.print("\n");

      HAPClient::printControllers();
      Serial.print("\n");

      for(int i=0;i<maxConnections;i++){
        Serial.print("Connection #");
        Serial.print(i);
        Serial.print(" ");
        if(hap[i]->client){
      
          Serial.print(hap[i]->client.remoteIP());
          Serial.print(" on Socket ");
          Serial.print(hap[i]->client.fd()-LWIP_SOCKET_OFFSET+1);
          Serial.print("/");
          Serial.print(CONFIG_LWIP_MAX_SOCKETS);
          
          if(hap[i]->cPair){
            Serial.print("  ID=");
            HAPClient::charPrintRow(hap[i]->cPair->ID,36);
            Serial.print(hap[i]->cPair->admin?"   (admin)":" (regular)");
          } else {
            Serial.print("  (unverified)");
          }
      
        } else {
          Serial.print("(unconnected)");
        }

        Serial.print("\n");
      }

      Serial.print("\n*** End Status ***\n\n");
    } 
    break;

    case 'd': {      
      
      TempBuffer <char> qBuf(sprintfAttributes(NULL)+1);
      sprintfAttributes(qBuf.buf);  

      Serial.print("\n*** Attributes Database: size=");
      Serial.print(qBuf.len()-1);
      Serial.print("  configuration=");
      Serial.print(hapConfig.configNumber);
      Serial.print(" ***\n\n");
      prettyPrint(qBuf.buf);
      Serial.print("\n*** End Database ***\n\n");
    }
    break;

    case 'Q': {
      char tBuf[5];
      const char *s=c+1+strspn(c+1," ");
      sscanf(s," %4[0-9A-Za-z]",tBuf);
  
      if(strlen(s)==4 && strlen(tBuf)==4){
        sprintf(qrID,"%s",tBuf);
        Serial.print("\nChanging default Setup ID for QR Code to: '");
        Serial.print(qrID);
        Serial.print("'.  Will take effect after next restart.\n\n");
        nvs_set_str(HAPClient::hapNVS,"SETUPID",qrID);                           // update data
        nvs_commit(HAPClient::hapNVS);          
      } else {
        Serial.print("\n*** Invalid request to change Setup ID for QR Code to: '");
        Serial.print(s);
        Serial.print("'.  Setup ID must be exactly 4 alphanumeric characters (0-9, A-Z, and a-z).\n\n");  
      }        
    }
    break;
    
    case 'O': {

      char textPwd[34]="\0";
      
      Serial.print("\n>>> New OTA Password, or <return> to cancel request: ");
      readSerial(textPwd,33);
      
      if(strlen(textPwd)==0){
        Serial.print("(cancelled)\n\n");
        return;
      }

      if(strlen(textPwd)==33){
        Serial.print("\n*** Sorry, 32 character limit - request cancelled\n\n");
        return;
      }
      
      Serial.print(mask(textPwd,2));
      Serial.print("\n");      
      
      MD5Builder otaPwdHash;
      otaPwdHash.begin();
      otaPwdHash.add(textPwd);
      otaPwdHash.calculate();
      otaPwdHash.getChars(otaPwd);
      nvs_set_str(HAPClient::otaNVS,"OTADATA",otaPwd);                 // update data
      nvs_commit(HAPClient::otaNVS);          
      
      Serial.print("... Accepted! Password change will take effect after next restart.\n");
      if(!otaEnabled)
        Serial.print("... Note: OTA has not been enabled in this sketch.\n");
      Serial.print("\n");
    }
    break;

    case 'S': {
      
      char buf[128];
      char setupCode[10];

      struct {                                      // temporary structure to hold SRP verification code and salt stored in NVS
        uint8_t salt[16];
        uint8_t verifyCode[384];
      } verifyData;      

      sscanf(c+1," %9[0-9]",setupCode);
      
      if(strlen(setupCode)!=8){
        Serial.print("\n*** Invalid request to change Setup Code.  Code must be exactly 8 digits.\n");
      } else
      
      if(!network.allowedCode(setupCode)){
        Serial.print("\n*** Invalid request to change Setup Code.  Code too simple.\n");
      } else {
        
        sprintf(buf,"\n\nGenerating SRP verification data for new Setup Code: %.3s-%.2s-%.3s ... ",setupCode,setupCode+3,setupCode+5);
        Serial.print(buf);
        HAPClient::srp.createVerifyCode(setupCode,verifyData.verifyCode,verifyData.salt);                         // create verification code from default Setup Code and random salt
        nvs_set_blob(HAPClient::srpNVS,"VERIFYDATA",&verifyData,sizeof(verifyData));                              // update data
        nvs_commit(HAPClient::srpNVS);                                                                            // commit to NVS
        Serial.print("New Code Saved!\n");

        Serial.print("Setup Payload for Optional QR Code: ");
        Serial.print(qrCode.get(atoi(setupCode),qrID,atoi(category)));
        Serial.print("\n\n");        
      }            
    }
    break;

    case 'U': {

      HAPClient::removeControllers();                                                                           // clear all Controller data  
      nvs_set_blob(HAPClient::hapNVS,"CONTROLLERS",HAPClient::controllers,sizeof(HAPClient::controllers));      // update data
      nvs_commit(HAPClient::hapNVS);                                                                            // commit to NVS
      Serial.print("\n*** HomeSpan Pairing Data DELETED ***\n\n");
      
      for(int i=0;i<maxConnections;i++){     // loop over all connection slots
        if(hap[i]->client){                    // if slot is connected
          LOG1("*** Terminating Client #");
          LOG1(i);
          LOG1("\n");
          hap[i]->client.stop();
        }
      }
      
      Serial.print("\nDEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
      mdns_service_txt_item_set("_hap","_tcp","sf","1");                                                        // set Status Flag = 1 (Table 6-8)
      
      if(strlen(network.wifiData.ssid)==0)
        statusLED.start(LED_WIFI_NEEDED);
      else
        statusLED.start(LED_PAIRING_NEEDED);
    }
    break;

    case 'W': {

      network.serialConfigure();
      nvs_set_blob(HAPClient::wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
      nvs_commit(HAPClient::wifiNVS);                                                            // commit to NVS
      Serial.print("\n*** WiFi Credentials SAVED!  Re-starting ***\n\n");
      statusLED.off();
      delay(1000);
      ESP.restart();  
      }
    break;

    case 'A': {

      if(strlen(network.wifiData.ssid)>0){
        Serial.print("*** Stopping all current WiFi services...\n\n");
        hapServer->end();
        MDNS.end();
        WiFi.disconnect();
      }
      
      network.apConfigure();
      nvs_set_blob(HAPClient::wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
      nvs_commit(HAPClient::wifiNVS);                                                            // commit to NVS
      Serial.print("\n*** Credentials saved!\n\n");
      if(strlen(network.setupCode)){
        char s[10];
        sprintf(s,"S%s",network.setupCode);
        processSerialCommand(s);
      } else {
        Serial.print("*** Setup Code Unchanged\n");
      }
      
      Serial.print("\n*** Re-starting ***\n\n");
      statusLED.off();
      delay(1000);
      ESP.restart();                                                                             // re-start device   
    }
    break;
    
    case 'X': {

      statusLED.off();
      nvs_erase_all(HAPClient::wifiNVS);
      nvs_commit(HAPClient::wifiNVS);      
      Serial.print("\n*** WiFi Credentials ERASED!  Re-starting...\n\n");
      delay(1000);
      ESP.restart();                                                                             // re-start device   
    }
    break;

    case 'H': {
      
      statusLED.off();
      nvs_erase_all(HAPClient::hapNVS);
      nvs_commit(HAPClient::hapNVS);      
      Serial.print("\n*** HomeSpan Device ID and Pairing Data DELETED!  Restarting...\n\n");
      delay(1000);
      ESP.restart();
    }
    break;

    case 'R': {
      
      statusLED.off();
      Serial.print("\n*** Restarting...\n\n");
      delay(1000);
      ESP.restart();
    }
    break;

    case 'F': {
      
      statusLED.off();
      nvs_erase_all(HAPClient::hapNVS);
      nvs_commit(HAPClient::hapNVS);      
      nvs_erase_all(HAPClient::wifiNVS);
      nvs_commit(HAPClient::wifiNVS);      
      Serial.print("\n*** FACTORY RESET!  Restarting...\n\n");
      delay(1000);
      ESP.restart();
    }
    break;

    case 'E': {
      
      statusLED.off();
      nvs_flash_erase();
      Serial.print("\n*** ALL DATA ERASED!  Restarting...\n\n");
      delay(1000);
      ESP.restart();
    }
    break;

    case 'L': {

      int level=0;
      sscanf(c+1,"%d",&level);
      
      if(level<0)
        level=0;
      if(level>2)
        level=2;

      Serial.print("\n*** Log Level set to ");
      Serial.print(level);
      Serial.print("\n\n");
      delay(1000);
      setLogLevel(level);     
    }
    break;

    case 'i':{

      Serial.print("\n*** HomeSpan Info ***\n\n");

      Serial.print(configLog);
      Serial.print("\nConfigured as Bridge: ");
      Serial.print(homeSpan.isBridge?"YES":"NO");
      Serial.print("\n\n");

      char d[]="------------------------------";
      char cBuf[256];
      sprintf(cBuf,"%-30s  %s  %10s  %s  %s  %s  %s\n","Service","Type","AID","IID","Update","Loop","Button");
      Serial.print(cBuf);
      sprintf(cBuf,"%.30s  %.4s  %.10s  %.3s  %.6s  %.4s  %.6s\n",d,d,d,d,d,d,d);
      Serial.print(cBuf);
      for(int i=0;i<Accessories.size();i++){                             // identify all services with over-ridden loop() methods
        for(int j=0;j<Accessories[i]->Services.size();j++){
          SpanService *s=Accessories[i]->Services[j];
          sprintf(cBuf,"%-30s  %4s  %10u  %3d  %6s  %4s  %6s\n",s->hapName,s->type,Accessories[i]->aid,s->iid, 
                 (void(*)())(s->*(&SpanService::update))!=(void(*)())(&SpanService::update)?"YES":"NO",
                 (void(*)())(s->*(&SpanService::loop))!=(void(*)())(&SpanService::loop)?"YES":"NO",
                 (void(*)(int,boolean))(s->*(&SpanService::button))!=(void(*)(int,boolean))(&SpanService::button)?"YES":"NO"
                 );
          Serial.print(cBuf);
        }
      }
      Serial.print("\n*** End Info ***\n");
    }
    break;

    case '?': {    
      
      Serial.print("\n*** HomeSpan Commands ***\n\n");
      Serial.print("  s - print connection status\n");
      Serial.print("  i - print summary information about the HAP Database\n");
      Serial.print("  d - print the full HAP Accessory Attributes Database in JSON format\n");
      Serial.print("\n");      
      Serial.print("  W - configure WiFi Credentials and restart\n");      
      Serial.print("  X - delete WiFi Credentials and restart\n");      
      Serial.print("  S <code> - change the HomeKit Pairing Setup Code to <code>\n");
      Serial.print("  Q <id> - change the HomeKit Setup ID for QR Codes to <id>\n");
      Serial.print("  O - change the OTA password\n");
      Serial.print("  A - start the HomeSpan Setup Access Point\n");      
      Serial.print("\n");      
      Serial.print("  U - unpair device by deleting all Controller data\n");
      Serial.print("  H - delete HomeKit Device ID as well as all Controller data and restart\n");      
      Serial.print("\n");      
      Serial.print("  R - restart device\n");      
      Serial.print("  F - factory reset and restart\n");      
      Serial.print("  E - erase ALL stored data and restart\n");      
      Serial.print("\n");          
      Serial.print("  L <level> - change the Log Level setting to <level>\n");
      Serial.print("\n");      
      Serial.print("  ? - print this list of commands\n");
      Serial.print("\n");      
      Serial.print("\n*** End Commands ***\n\n");
    }
    break;

    default:
      Serial.print("** Unknown command: '");
      Serial.print(c);
      Serial.print("' - type '?' for list of commands.\n");
    break;
    
  } // switch
}

///////////////////////////////

int Span::sprintfAttributes(char *cBuf){

  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"accessories\":[");

  for(int i=0;i<Accessories.size();i++){
    nBytes+=Accessories[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL);    
    if(i+1<Accessories.size())
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
    }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]}");
  return(nBytes);
}

///////////////////////////////

void Span::prettyPrint(char *buf, int nsp){
  int s=strlen(buf);
  int indent=0;
  
  for(int i=0;i<s;i++){
    switch(buf[i]){
      
      case '{':
      case '[':
        Serial.print(buf[i]);
        Serial.print("\n");
        indent+=nsp;
        for(int j=0;j<indent;j++)
          Serial.print(" ");
        break;

      case '}':
      case ']':
        Serial.print("\n");
        indent-=nsp;
        for(int j=0;j<indent;j++)
          Serial.print(" ");
        Serial.print(buf[i]);
        break;

      case ',':
        Serial.print(buf[i]);
        Serial.print("\n");
        for(int j=0;j<indent;j++)
          Serial.print(" ");
        break;

      default:
        Serial.print(buf[i]);
           
    } // switch
  } // loop over all characters

  Serial.print("\n");
} // prettyPrint

///////////////////////////////

SpanCharacteristic *Span::find(uint32_t aid, int iid){

  int index=-1;
  for(int i=0;i<Accessories.size();i++){   // loop over all Accessories to find aid
    if(Accessories[i]->aid==aid){          // if match, save index into Accessories array
      index=i;
      break;
    }
  }

  if(index<0)                  // fail if no match on aid
    return(NULL);
    
  for(int i=0;i<Accessories[index]->Services.size();i++){                           // loop over all Services in this Accessory
    for(int j=0;j<Accessories[index]->Services[i]->Characteristics.size();j++){     // loop over all Characteristics in this Service
      
      if(iid == Accessories[index]->Services[i]->Characteristics[j]->iid)           // if matching iid
        return(Accessories[index]->Services[i]->Characteristics[j]);                // return pointer to Characteristic
    }
  }

  return(NULL);                // fail if no match on iid
}

///////////////////////////////

int Span::countCharacteristics(char *buf){

  int nObj=0;
  
  const char tag[]="\"aid\"";
  while((buf=strstr(buf,tag))){         // count number of characteristic objects in PUT JSON request
    nObj++;
    buf+=strlen(tag);
  }

  return(nObj);
}

///////////////////////////////

int Span::updateCharacteristics(char *buf, SpanBuf *pObj){

  int nObj=0;
  char *p1;
  int cFound=0;
  boolean twFail=false;
  
  while(char *t1=strtok_r(buf,"{",&p1)){           // parse 'buf' and extract objects into 'pObj' unless NULL
   buf=NULL;
    char *p2;
    int okay=0;
    
    while(char *t2=strtok_r(t1,"}[]:, \"\t\n\r",&p2)){

      if(!cFound){                                 // first token found
        if(strcmp(t2,"characteristics")){
          Serial.print("\n*** ERROR:  Problems parsing JSON - initial \"characteristics\" tag not found\n\n");
          return(0);
        }
        cFound=1;
        break;
      }
      
      t1=NULL;
      char *t3;
      if(!strcmp(t2,"aid") && (t3=strtok_r(t1,"}[]:, \"\t\n\r",&p2))){
        sscanf(t3,"%u",&pObj[nObj].aid);
        okay|=1;
      } else 
      if(!strcmp(t2,"iid") && (t3=strtok_r(t1,"}[]:, \"\t\n\r",&p2))){
        pObj[nObj].iid=atoi(t3);
        okay|=2;
      } else 
      if(!strcmp(t2,"value") && (t3=strtok_r(t1,"}[]:, \"\t\n\r",&p2))){
        pObj[nObj].val=t3;
        okay|=4;
      } else 
      if(!strcmp(t2,"ev") && (t3=strtok_r(t1,"}[]:, \"\t\n\r",&p2))){
        pObj[nObj].ev=t3;
        okay|=8;
      } else 
      if(!strcmp(t2,"pid") && (t3=strtok_r(t1,"}[]:, \"\t\n\r",&p2))){        
        uint64_t pid=strtoull(t3,NULL,0);        
        if(!TimedWrites.count(pid)){
          Serial.print("\n*** ERROR:  Timed Write PID not found\n\n");
          twFail=true;
        } else        
        if(millis()>TimedWrites[pid]){
          Serial.print("\n*** ERROR:  Timed Write Expired\n\n");
          twFail=true;
        }        
      } else {
        Serial.print("\n*** ERROR:  Problems parsing JSON characteristics object - unexpected property \"");
        Serial.print(t2);
        Serial.print("\"\n\n");
        return(0);
      }
    } // parse property tokens

    if(!t1){                                                                  // at least one token was found that was not initial "characteristics"
      if(okay==7 || okay==11  || okay==15){                                   // all required properties found                           
        nObj++;                                                               // increment number of characteristic objects found        
      } else {
        Serial.print("\n*** ERROR:  Problems parsing JSON characteristics object - missing required properties\n\n");
        return(0);
      }
    }
      
  } // parse objects

  snapTime=millis();                                           // timestamp for this series of updates, assigned to each characteristic in loadUpdate()

  for(int i=0;i<nObj;i++){                                     // PASS 1: loop over all objects, identify characteristics, and initialize update for those found

    if(twFail){                                                // this is a timed-write request that has either expired or for which there was no PID
      pObj[i].status=StatusCode::InvalidValue;                 // set error for all characteristics      
      
    } else {
      pObj[i].characteristic = find(pObj[i].aid,pObj[i].iid);  // find characteristic with matching aid/iid and store pointer          

      if(pObj[i].characteristic)                                                      // if found, initialize characterstic update with new val/ev
        pObj[i].status=pObj[i].characteristic->loadUpdate(pObj[i].val,pObj[i].ev);    // save status code, which is either an error, or TBD (in which case isUpdated for the characteristic has been set to true) 
      else
        pObj[i].status=StatusCode::UnknownResource;                                   // if not found, set HAP error            
    }
      
  } // first pass
      
  for(int i=0;i<nObj;i++){                                     // PASS 2: loop again over all objects       
    if(pObj[i].status==StatusCode::TBD){                       // if object status still TBD

      StatusCode status=pObj[i].characteristic->service->update()?StatusCode::OK:StatusCode::Unable;                  // update service and save statusCode as OK or Unable depending on whether return is true or false

      for(int j=i;j<nObj;j++){                                                      // loop over this object plus any remaining objects to update values and save status for any other characteristics in this service
        
        if(pObj[j].characteristic->service==pObj[i].characteristic->service){       // if service of this characteristic matches service that was updated
          pObj[j].status=status;                                                    // save statusCode for this object
          LOG1("Updating aid=");
          LOG1(pObj[j].characteristic->aid);
          LOG1(" iid=");  
          LOG1(pObj[j].characteristic->iid);
          if(status==StatusCode::OK){                          // if status is okay
            pObj[j].characteristic->value
              =pObj[j].characteristic->newValue;               // update characteristic value with new value
            LOG1(" (okay)\n");
          } else {                                             // if status not okay
            pObj[j].characteristic->newValue
              =pObj[j].characteristic->value;                  // replace characteristic new value with original value
            LOG1(" (failed)\n");
          }
          pObj[j].characteristic->isUpdated=false;             // reset isUpdated flag for characteristic
        }
      }

    } // object had TBD status
  } // loop over all objects
      
  return(1);
}

///////////////////////////////

void Span::clearNotify(int slotNum){
  
  for(int i=0;i<Accessories.size();i++){
    for(int j=0;j<Accessories[i]->Services.size();j++){
      for(int k=0;k<Accessories[i]->Services[j]->Characteristics.size();k++){
        Accessories[i]->Services[j]->Characteristics[k]->ev[slotNum]=false;
      }
    }
  }
}

///////////////////////////////

int Span::sprintfNotify(SpanBuf *pObj, int nObj, char *cBuf, int conNum){

  int nChars=0;
  boolean notifyFlag=false;
  
  nChars+=snprintf(cBuf,cBuf?64:0,"{\"characteristics\":[");

  for(int i=0;i<nObj;i++){                              // loop over all objects
    
    if(pObj[i].status==StatusCode::OK && pObj[i].val){           // characteristic was successfully updated with a new value (i.e. not just an EV request)
      
      if(pObj[i].characteristic->ev[conNum]){           // if notifications requested for this characteristic by specified connection number
      
        if(notifyFlag)                                                           // already printed at least one other characteristic
          nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",");               // add preceeding comma before printing next characteristic
        
        nChars+=pObj[i].characteristic->sprintfAttributes(cBuf?(cBuf+nChars):NULL,GET_AID+GET_NV);    // get JSON attributes for characteristic
        notifyFlag=true;
        
      } // notification requested
    } // characteristic updated
  } // loop over all objects

  nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"]}");

  return(notifyFlag?nChars:0);                          // if notifyFlag is not set, return 0, else return number of characters printed to cBuf
}

///////////////////////////////

int Span::sprintfAttributes(SpanBuf *pObj, int nObj, char *cBuf){

  int nChars=0;

  nChars+=snprintf(cBuf,cBuf?64:0,"{\"characteristics\":[");

  for(int i=0;i<nObj;i++){
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?128:0,"{\"aid\":%u,\"iid\":%d,\"status\":%d}",pObj[i].aid,pObj[i].iid,(int)pObj[i].status);
      if(i+1<nObj)
        nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",");
  }

  nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"]}");

  return(nChars);    
}

///////////////////////////////

int Span::sprintfAttributes(char **ids, int numIDs, int flags, char *cBuf){

  int nChars=0;
  uint32_t aid;
  int iid;
  
  SpanCharacteristic *Characteristics[numIDs];
  StatusCode status[numIDs];
  boolean sFlag=false;

  for(int i=0;i<numIDs;i++){              // PASS 1: loop over all ids requested to check status codes - only errors are if characteristic not found, or not readable
    sscanf(ids[i],"%u.%d",&aid,&iid);     // parse aid and iid
    Characteristics[i]=find(aid,iid);      // find matching chararacteristic
    
    if(Characteristics[i]){                                          // if found
      if(Characteristics[i]->perms&SpanCharacteristic::PR){          // if permissions allow reading
        status[i]=StatusCode::OK;                                    // always set status to OK (since no actual reading of device is needed)
      } else {
        Characteristics[i]=NULL;                                     
        status[i]=StatusCode::WriteOnly;
        sFlag=true;                                                  // set flag indicating there was an error
      }
    } else {
      status[i]=StatusCode::UnknownResource;
      sFlag=true;                                                    // set flag indicating there was an error
    }
  }

  nChars+=snprintf(cBuf,cBuf?64:0,"{\"characteristics\":[");  

  for(int i=0;i<numIDs;i++){              // PASS 2: loop over all ids requested and create JSON for each (with or without status code base on sFlag set above)
    
    if(Characteristics[i])                                                                         // if found
      nChars+=Characteristics[i]->sprintfAttributes(cBuf?(cBuf+nChars):NULL,flags);                // get JSON attributes for characteristic
    else{
      sscanf(ids[i],"%u.%d",&aid,&iid);     // parse aid and iid                        
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"{\"iid\":%d,\"aid\":%u}",iid,aid);      // else create JSON attributes based on requested aid/iid
    }
    
    if(sFlag){                                                                                    // status flag is needed - overlay at end
      nChars--;
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",\"status\":%d}",(int)status[i]);
    }
  
    if(i+1<numIDs)
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",");
    
  }

  nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"]}");

  return(nChars);    
}

///////////////////////////////
//      SpanAccessory        //
///////////////////////////////

SpanAccessory::SpanAccessory(uint32_t aid){

  if(!homeSpan.Accessories.empty()){

    if(homeSpan.Accessories.size()==HAPClient::MAX_ACCESSORIES){
      Serial.print("\n\n*** FATAL ERROR: Can't create more than ");
      Serial.print(HAPClient::MAX_ACCESSORIES);
      Serial.print(" Accessories.  Program Halting.\n\n");
      while(1);      
    }
    
    this->aid=homeSpan.Accessories.back()->aid+1;
    
    if(!homeSpan.Accessories.back()->Services.empty())
      homeSpan.Accessories.back()->Services.back()->validate();    
      
    homeSpan.Accessories.back()->validate();    
  } else {
    this->aid=1;
  }
  
  homeSpan.Accessories.push_back(this);

  if(aid>0){                 // override with user-specified aid
    this->aid=aid;
  }

  homeSpan.configLog+="+Accessory-" + String(this->aid);

  for(int i=0;i<homeSpan.Accessories.size()-1;i++){
    if(this->aid==homeSpan.Accessories[i]->aid){
      homeSpan.configLog+=" *** ERROR!  ID already in use for another Accessory. ***";
      homeSpan.nFatalErrors++;
      break;
    }
  }

  if(homeSpan.Accessories.size()==1 && this->aid!=1){
    homeSpan.configLog+=" *** ERROR!  ID of first Accessory must always be 1. ***";
    homeSpan.nFatalErrors++;    
  }

  homeSpan.configLog+="\n";

}

///////////////////////////////

void SpanAccessory::validate(){

  boolean foundInfo=false;
  boolean foundProtocol=false;
  
  for(int i=0;i<Services.size();i++){
    if(!strcmp(Services[i]->type,"3E"))
      foundInfo=true;
    else if(!strcmp(Services[i]->type,"A2"))
      foundProtocol=true;
    else if(aid==1)                             // this is an Accessory with aid=1, but it has more than just AccessoryInfo and HAPProtocolInformation.  So...
      homeSpan.isBridge=false;                  // ...this is not a bridge device
  }

  if(!foundInfo){
    homeSpan.configLog+="  !Service AccessoryInformation";
    homeSpan.configLog+=" *** ERROR!  Required Service for this Accessory not found. ***\n";
    homeSpan.nFatalErrors++;
  }    

  if(!foundProtocol && (aid==1 || !homeSpan.isBridge)){           // HAPProtocolInformation must always be present in Accessory if aid=1, and any other Accessory if the device is not a bridge)
    homeSpan.configLog+="  !Service HAPProtocolInformation";
    homeSpan.configLog+=" *** ERROR!  Required Service for this Accessory not found. ***\n";
    homeSpan.nFatalErrors++;
  }    
}

///////////////////////////////

int SpanAccessory::sprintfAttributes(char *cBuf){
  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"aid\":%u,\"services\":[",aid);

  for(int i=0;i<Services.size();i++){
    nBytes+=Services[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL);    
    if(i+1<Services.size())
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
    }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]}");

  return(nBytes);
}

///////////////////////////////
//       SpanService         //
///////////////////////////////

SpanService::SpanService(const char *type, const char *hapName){

  if(!homeSpan.Accessories.empty() && !homeSpan.Accessories.back()->Services.empty())      // this is not the first Service to be defined for this Accessory
    homeSpan.Accessories.back()->Services.back()->validate();    

  this->type=type;
  this->hapName=hapName;

  homeSpan.configLog+="-->Service " + String(hapName);
  
  if(homeSpan.Accessories.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new Service without a defined Accessory! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  homeSpan.Accessories.back()->Services.push_back(this);  
  iid=++(homeSpan.Accessories.back()->iidCount);  

  homeSpan.configLog+="-" + String(iid) + String(" (") + String(type) + String(") ");

  if(!strcmp(this->type,"3E") && iid!=1){
    homeSpan.configLog+=" *** ERROR!  The AccessoryInformation Service must be defined before any other Services in an Accessory. ***";
    homeSpan.nFatalErrors++;
  }

  homeSpan.configLog+="\n";

}

///////////////////////////////

SpanService *SpanService::setPrimary(){
  primary=true;
  return(this);
}

///////////////////////////////

SpanService *SpanService::setHidden(){
  hidden=true;
  return(this);
}

///////////////////////////////

int SpanService::sprintfAttributes(char *cBuf){
  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"iid\":%d,\"type\":\"%s\",",iid,type);
  
  if(hidden)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"hidden\":true,");
    
  if(primary)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"primary\":true,");
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"characteristics\":[");
  
  for(int i=0;i<Characteristics.size();i++){
    nBytes+=Characteristics[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL,GET_META|GET_PERMS|GET_TYPE|GET_DESC);    
    if(i+1<Characteristics.size())
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
  }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]}");

  return(nBytes);
}

///////////////////////////////

void SpanService::validate(){

  for(int i=0;i<req.size();i++){
    boolean valid=false;
    for(int j=0;!valid && j<Characteristics.size();j++)
      valid=!strcmp(req[i]->id,Characteristics[j]->type);
      
    if(!valid){
      homeSpan.configLog+="    !Characteristic " + String(req[i]->name);
      homeSpan.configLog+=" *** ERROR!  Required Characteristic for this Service not found. ***\n";
      homeSpan.nFatalErrors++;
    }
  }
}

///////////////////////////////
//    SpanCharacteristic     //
///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, const char *hapName){
  this->type=type;
  this->perms=perms;
  this->hapName=hapName;

  homeSpan.configLog+="---->Characteristic " + String(hapName);

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new Characteristic without a defined Service! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  iid=++(homeSpan.Accessories.back()->iidCount);
  service=homeSpan.Accessories.back()->Services.back();
  aid=homeSpan.Accessories.back()->aid;

  ev=(boolean *)calloc(homeSpan.maxConnections,sizeof(boolean));

  homeSpan.configLog+="-" + String(iid) + String(" (") + String(type) + String(") ");

  boolean valid=false;

  for(int i=0; !valid && i<homeSpan.Accessories.back()->Services.back()->req.size(); i++)
    valid=!strcmp(type,homeSpan.Accessories.back()->Services.back()->req[i]->id);

  for(int i=0; !valid && i<homeSpan.Accessories.back()->Services.back()->opt.size(); i++)
    valid=!strcmp(type,homeSpan.Accessories.back()->Services.back()->opt[i]->id);

  if(!valid){
    homeSpan.configLog+=" *** ERROR!  Service does not support this Characteristic. ***";
    homeSpan.nFatalErrors++;
  }

  boolean repeated=false;
  
  for(int i=0; !repeated && i<homeSpan.Accessories.back()->Services.back()->Characteristics.size(); i++)
    repeated=!strcmp(type,homeSpan.Accessories.back()->Services.back()->Characteristics[i]->type);
  
  if(valid && repeated){
    homeSpan.configLog+=" *** ERROR!  Characteristic already defined for this Service. ***";
    homeSpan.nFatalErrors++;
  }

  homeSpan.Accessories.back()->Services.back()->Characteristics.push_back(this);  

  homeSpan.configLog+="\n";

}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const  char *type, uint8_t perms, boolean value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=BOOL;
  this->value.BOOL=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, int32_t value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=INT;
  this->value.INT=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, uint8_t value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT8;
  this->value.UINT8=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, uint16_t value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT16;
  this->value.UINT16=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, uint32_t value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT32;
  this->value.UINT32=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, uint64_t value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT64;
  this->value.UINT64=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, double value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=FLOAT;
  this->value.FLOAT=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(const char *type, uint8_t perms, const char* value, const char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=STRING;
  this->value.STRING=value;
}

///////////////////////////////

int SpanCharacteristic::sprintfAttributes(char *cBuf, int flags){
  int nBytes=0;

  const char permCodes[][7]={"pr","pw","ev","aa","tw","hd","wr"};

  const char formatCodes[][8]={"bool","uint8","uint16","uint32","uint64","int","float","string"};

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"iid\":%d",iid);

  if(flags&GET_TYPE)  
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"type\":\"%s\"",type);

  if(perms&PR){
    
    if(perms&NV && !(flags&GET_NV)){   
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":null");
    } else {
      
      switch(format){
        case BOOL:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%s",value.BOOL?"true":"false");
        break;
    
        case INT:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%d",value.INT);
        break;
    
        case UINT8:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%u",value.UINT8);
        break;
          
        case UINT16:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%u",value.UINT16);
        break;
          
        case UINT32:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%u",value.UINT32);
        break;
          
        case UINT64:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%llu",value.UINT64);
        break;
          
        case FLOAT:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%lg",value.FLOAT);
        break;
          
        case STRING:
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":\"%s\"",value.STRING);
        break;
        
      } // switch
    } // print Characteristic value
  } // permissions=PR

  if(flags&GET_META){
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"%s\"",formatCodes[format]);
    
    if(range && (flags&GET_META))
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"minValue\":%d,\"maxValue\":%d,\"minStep\":%d",range->min,range->max,range->step);    
  }
    
  if(desc && (flags&GET_DESC)){
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"description\":\"%s\"",desc);    
  }

  if(flags&GET_PERMS){
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"perms\":[");
    for(int i=0;i<7;i++){
      if(perms&(1<<i)){
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"%s\"",permCodes[i]);
        if(perms>=(1<<(i+1)))
          nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
      }
    }
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]");
  }

  if(flags&GET_AID)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"aid\":%u",aid);
  
  if(flags&GET_EV)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"ev\":%s",ev[HAPClient::conNum]?"true":"false");

  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"}");

  return(nBytes);
}

///////////////////////////////

StatusCode SpanCharacteristic::loadUpdate(char *val, char *ev){

  if(ev){                // request for notification
    boolean evFlag;
    
    if(!strcmp(ev,"0") || !strcmp(ev,"false"))
      evFlag=false;
    else if(!strcmp(ev,"1") || !strcmp(ev,"true"))
      evFlag=true;
    else
      return(StatusCode::InvalidValue);
    
    if(evFlag && !(perms&EV))         // notification is not supported for characteristic
      return(StatusCode::NotifyNotAllowed);
      
    LOG1("Notification Request for aid=");
    LOG1(aid);
    LOG1(" iid=");
    LOG1(iid);
    LOG1(": ");
    LOG1(evFlag?"true":"false");
    LOG1("\n");
    this->ev[HAPClient::conNum]=evFlag;
  }

  if(!val)                // no request to update value
    return(StatusCode::OK);
  
  if(!(perms&PW))         // cannot write to read only characteristic
    return(StatusCode::ReadOnly);

  switch(format){
    
    case BOOL:
      if(!strcmp(val,"0") || !strcmp(val,"false"))
        newValue.BOOL=false;
      else if(!strcmp(val,"1") || !strcmp(val,"true"))
        newValue.BOOL=true;
      else
        return(StatusCode::InvalidValue);
      break;

    case INT:
      if(!sscanf(val,"%d",&newValue.INT))
        return(StatusCode::InvalidValue);
      break;

    case UINT8:
      if(!sscanf(val,"%hhu",&newValue.UINT8))
        return(StatusCode::InvalidValue);
      break;
            
    case UINT16:
      if(!sscanf(val,"%hu",&newValue.UINT16))
        return(StatusCode::InvalidValue);
      break;
      
    case UINT32:
      if(!sscanf(val,"%u",&newValue.UINT32))
        return(StatusCode::InvalidValue);
      break;
      
    case UINT64:
      if(!sscanf(val,"%llu",&newValue.UINT64))
        return(StatusCode::InvalidValue);
      break;

    case FLOAT:
      if(!sscanf(val,"%lg",&newValue.FLOAT))
        return(StatusCode::InvalidValue);
      break;

    default:
    break;

  } // switch

  isUpdated=true;
  updateTime=homeSpan.snapTime;
  return(StatusCode::TBD);
}

///////////////////////////////

void SpanCharacteristic::setVal(int val){

    switch(format){
      
      case BOOL:
        value.BOOL=(boolean)val;
        newValue.BOOL=(boolean)val;        
      break;

      case INT:
        value.INT=(int)val;
        newValue.INT=(int)val;
      break;

      case UINT8:
        value.UINT8=(uint8_t)val;
        newValue.INT=(int)val;
      break;

      case UINT16:
        value.UINT16=(uint16_t)val;
        newValue.UINT16=(uint16_t)val;
      break;

      case UINT32:
        value.UINT32=(uint32_t)val;
        newValue.UINT32=(uint32_t)val;
      break;

      case UINT64:
        value.UINT64=(uint64_t)val;
        newValue.UINT64=(uint64_t)val;
      break;

      default:
      break;
    }

    updateTime=homeSpan.snapTime;

    SpanBuf sb;                             // create SpanBuf object
    sb.characteristic=this;                 // set characteristic          
    sb.status=StatusCode::OK;               // set status
    char dummy[]="";
    sb.val=dummy;                           // set dummy "val" so that sprintfNotify knows to consider this "update"
    homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector
}

///////////////////////////////

void SpanCharacteristic::setVal(double val){
  
    value.FLOAT=(double)val;  
    newValue.FLOAT=(double)val;  
    updateTime=homeSpan.snapTime;

    SpanBuf sb;                             // create SpanBuf object
    sb.characteristic=this;                 // set characteristic          
    sb.status=StatusCode::OK;               // set status
    char dummy[]="";
    sb.val=dummy;                           // set dummy "val" so that sprintfNotify knows to consider this "update"
    homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector
}

///////////////////////////////

unsigned long SpanCharacteristic::timeVal(){
  
  return(homeSpan.snapTime-updateTime);
}

///////////////////////////////
//        SpanRange          //
///////////////////////////////

SpanRange::SpanRange(int min, int max, int step){
  this->min=min;
  this->max=max;
  this->step=step;

  homeSpan.configLog+="------>SpanRange: " + String(min) + "/" + String(max) + "/" + String(step);

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty() || homeSpan.Accessories.back()->Services.back()->Characteristics.empty() ){
    homeSpan.configLog+=" *** ERROR!  Can't create new Range without a defined Characteristic! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  homeSpan.configLog+="\n";    
  homeSpan.Accessories.back()->Services.back()->Characteristics.back()->range=this;  
}

///////////////////////////////
//        SpanButton         //
///////////////////////////////

SpanButton::SpanButton(int pin, uint16_t longTime, uint16_t singleTime, uint16_t doubleTime){

  homeSpan.configLog+="---->SpanButton: Pin=" + String(pin) + " Long/Single/Double=" + String(longTime) + "/" + String(singleTime) + "/" + String(doubleTime) + " ms";

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new PushButton without a defined Service! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  Serial.print("Configuring PushButton: Pin=");     // initialization message
  Serial.print(pin);
  Serial.print("\n");

  this->pin=pin;
  this->longTime=longTime;
  this->singleTime=singleTime;
  this->doubleTime=doubleTime;
  service=homeSpan.Accessories.back()->Services.back();

  if((void(*)(int,int))(service->*(&SpanService::button))==(void(*)(int,int))(&SpanService::button))
    homeSpan.configLog+=" *** WARNING:  No button() method defined for this PushButton! ***";

  pushButton=new PushButton(pin);         // create underlying PushButton
  
  homeSpan.configLog+="\n";  
  homeSpan.PushButtons.push_back(this);
}

///////////////////////////////
