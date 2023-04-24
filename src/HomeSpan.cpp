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
 
#include <ESPmDNS.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <WiFi.h>
#include <driver/ledc.h>
#include <mbedtls/version.h>
#include <mbedtls/sha256.h>
#include <esp_task_wdt.h>
#include <esp_sntp.h>
#include <esp_ota_ops.h>
#include <esp_wifi.h>

#include "HomeSpan.h"
#include "HAP.h"

const __attribute__((section(".rodata_custom_desc"))) SpanPartition spanPartition = {HOMESPAN_MAGIC_COOKIE,0};

using namespace Utils;

HAPClient **hap;                    // HAP Client structure containing HTTP client connections, parsing routines, and state variables (global-scoped variable)
Span homeSpan;                      // HAP Attributes database and all related control functions for this Accessory (global-scoped variable)
HapCharacteristics hapChars;        // Instantiation of all HAP Characteristics (used to create SpanCharacteristics)

///////////////////////////////
//         Span              //
///////////////////////////////

void Span::begin(Category catID, const char *displayName, const char *hostNameBase, const char *modelName){
  
  this->displayName=displayName;
  this->hostNameBase=hostNameBase;
  this->modelName=modelName;
  sprintf(this->category,"%d",(int)catID);

  SpanPoint::setAsHub();

  statusLED=new Blinker(statusDevice,autoOffLED);             // create Status LED, even is statusDevice is NULL

  esp_task_wdt_delete(xTaskGetIdleTaskHandleForCPU(0));       // required to avoid watchdog timeout messages from ESP32-C3

  if(requestedMaxCon<maxConnections)                          // if specific request for max connections is less than computed max connections
    maxConnections=requestedMaxCon;                           // over-ride max connections with requested value
    
  hap=(HAPClient **)calloc(maxConnections,sizeof(HAPClient *));
  for(int i=0;i<maxConnections;i++)
    hap[i]=new HAPClient;

  hapServer=new WiFiServer(tcpPortNum);

  nvs_flash_init();                             // initialize non-volatile-storage partition in flash  
  nvs_open("CHAR",NVS_READWRITE,&charNVS);      // open Characteristic data namespace in NVS
  nvs_open("WIFI",NVS_READWRITE,&wifiNVS);      // open WIFI data namespace in NVS
  nvs_open("OTA",NVS_READWRITE,&otaNVS);        // open OTA data namespace in NVS

  size_t len;

  if(strlen(network.wifiData.ssid)){                                                // if setWifiCredentials was already called
    nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
    nvs_commit(wifiNVS);                                                            // commit to NVS
  } else
  
  if(!nvs_get_blob(wifiNVS,"WIFIDATA",NULL,&len))                                   // else if found WiFi data in NVS
    nvs_get_blob(wifiNVS,"WIFIDATA",&homeSpan.network.wifiData,&len);               // retrieve data  

  delay(2000);

  Serial.print("\n************************************************************\n"
                 "Welcome to HomeSpan!\n"
                 "Apple HomeKit for the Espressif ESP-32 WROOM and Arduino IDE\n"
                 "************************************************************\n\n"
                 "** Please ensure serial monitor is set to transmit <newlines>\n\n");

  Serial.print("Message Logs:     Level ");
  Serial.print(logLevel);  
  Serial.print("\nStatus LED:       Pin ");
  if(getStatusPin()>=0){
    Serial.print(getStatusPin());
    if(autoOffLED>0)
      Serial.printf("  (Auto Off=%d sec)",autoOffLED);
  }
  else
    Serial.print("-  *** WARNING: Status LED Pin is UNDEFINED");
  Serial.print("\nDevice Control:   Pin ");
  if(getControlPin()>=0)
    Serial.print(getControlPin());
  else
    Serial.print("-  *** WARNING: Device Control Pin is UNDEFINED");
  Serial.print("\nSketch Version:   ");
  Serial.print(getSketchVersion());  
  Serial.print("\nHomeSpan Version: ");
  Serial.print(HOMESPAN_VERSION);
  Serial.print("\nArduino-ESP Ver.: ");
  Serial.print(ARDUINO_ESP_VERSION);
  Serial.printf("\nESP-IDF Version:  %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
  Serial.printf("\nESP32 Chip:       %s Rev %d %s-core %dMB Flash", ESP.getChipModel(),ESP.getChipRevision(),
                ESP.getChipCores()==1?"single":"dual",ESP.getFlashChipSize()/1024/1024);
  
  #ifdef ARDUINO_VARIANT
    Serial.print("\nESP32 Board:      ");
    Serial.print(ARDUINO_VARIANT);
  #endif
  
  Serial.printf("\nPWM Resources:    %d channels, %d timers, max %d-bit duty resolution",
                LEDC_SPEED_MODE_MAX*LEDC_CHANNEL_MAX,LEDC_SPEED_MODE_MAX*LEDC_TIMER_MAX,LEDC_TIMER_BIT_MAX-1);

  Serial.printf("\nSodium Version:   %s  Lib %d.%d",sodium_version_string(),sodium_library_version_major(),sodium_library_version_minor());
  char mbtlsv[64];
  mbedtls_version_get_string_full(mbtlsv);
  Serial.printf("\nMbedTLS Version:  %s",mbtlsv);

  Serial.print("\nSketch Compiled:  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);

  Serial.printf("\nPartition:        %s",esp_ota_get_running_partition()->label);
  Serial.printf("\nMAC Address:      %s",WiFi.macAddress().c_str());
  
  Serial.print("\n\nDevice Name:      ");
  Serial.print(displayName);  
  Serial.print("\n\n");

  uint8_t otaRequired=0;
  nvs_get_u8(otaNVS,"OTA_REQUIRED",&otaRequired);
  nvs_set_u8(otaNVS,"OTA_REQUIRED",0);
  nvs_commit(otaNVS);
  if(otaRequired && !spanOTA.enabled){
    Serial.printf("\n\n*** OTA SAFE MODE ALERT:  OTA REQUIRED BUT NOT ENABLED.  ROLLING BACK TO PREVIOUS APPLICATION ***\n\n");
    delay(100);
    esp_ota_mark_app_invalid_rollback_and_reboot();
  }
  
}  // begin

///////////////////////////////

void Span::poll() {

  if(pollTaskHandle){
    Serial.print("\n** FATAL ERROR: Do not call homeSpan.poll() directly if homeSpan.start() is used!\n** PROGRAM HALTED **\n\n");
    vTaskDelete(pollTaskHandle);
    while(1);    
  }
  
  pollTask();
}

///////////////////////////////

void Span::pollTask() {

  if(!strlen(category)){
    Serial.print("\n** FATAL ERROR: Cannot start homeSpan polling without an initial call to homeSpan.begin()!\n** PROGRAM HALTED **\n\n");
    while(1);    
  }

  if(!isInitialized){
  
    processSerialCommand("i");        // print homeSpan configuration info
           
    HAPClient::init();                // read NVS and load HAP settings  

    if(!strlen(network.wifiData.ssid)){
      Serial.print("*** WIFI CREDENTIALS DATA NOT FOUND.  ");
      if(autoStartAPEnabled){
        Serial.print("AUTO-START OF ACCESS POINT ENABLED...\n\n");
        processSerialCommand("A");
      } else {
        Serial.print("YOU MAY CONFIGURE BY TYPING 'W <RETURN>'.\n\n");
        STATUS_UPDATE(start(LED_WIFI_NEEDED),HS_WIFI_NEEDED)
      }
    } else {
      STATUS_UPDATE(start(LED_WIFI_CONNECTING),HS_WIFI_CONNECTING)
    }
          
    if(controlButton)
      controlButton->reset();        

    Serial.print(displayName);
    Serial.print(" is READY!\n\n");
    isInitialized=true;
    
  } // isInitialized

  if(strlen(network.wifiData.ssid)>0){
      checkConnect();
  }

  char cBuf[65]="?";
  
  if(Serial.available()){
    readSerial(cBuf,64);
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

      HAPClient::conNum=i;                                          // set connection number
      homeSpan.lastClientIP=hap[i]->client.remoteIP().toString();   // store IP Address for web logging
      hap[i]->processRequest();                                     // process HAP request
      homeSpan.lastClientIP="0.0.0.0";                              // reset stored IP address to show "0.0.0.0" if homeSpan.getClientIP() is used in any other context
      
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

  snapTime=millis();                                     // snap the current time for use in ALL loop routines
  
  for(auto it=Loops.begin();it!=Loops.end();it++)                 // call loop() for all Services with over-ridden loop() methods
    (*it)->loop();                           

  for(auto it=PushButtons.begin();it!=PushButtons.end();it++)     // check for SpanButton presses
    (*it)->check();
    
  HAPClient::checkNotifications();  
  HAPClient::checkTimedWrites();

  if(spanOTA.enabled)
    ArduinoOTA.handle();

  if(controlButton && controlButton->primed())
    STATUS_UPDATE(start(LED_ALERT),HS_ENTERING_CONFIG_MODE)
  
  if(controlButton && controlButton->triggered(3000,10000)){
    if(controlButton->type()==PushButton::LONG){
      STATUS_UPDATE(off(),HS_FACTORY_RESET)
      controlButton->wait();
      processSerialCommand("F");        // FACTORY RESET
    } else {
      commandMode();                    // COMMAND MODE
    }
  }

  statusLED->check();

  vTaskDelay(5);
    
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

  if(!statusDevice && !statusCallback){
    Serial.print("*** ERROR: CAN'T ENTER COMMAND MODE WITHOUT A DEFINED STATUS LED OR EVENT HANDLER CALLBACK***\n\n");
    return;
  }
  
  Serial.print("*** COMMAND MODE ***\n\n");
  int mode=1;
  boolean done=false;
  STATUS_UPDATE(start(500,0.3,mode,1000),static_cast<HS_STATUS>(HS_ENTERING_CONFIG_MODE+mode))
  unsigned long alarmTime=millis()+comModeLife;

  while(!done){
    if(millis()>alarmTime){
      Serial.print("*** Command Mode: Timed Out (");
      Serial.print(comModeLife/1000);
      Serial.print(" seconds).\n\n");
      mode=1;
      done=true;
    } else
    if(controlButton->triggered(10,3000)){
      if(controlButton->type()==PushButton::SINGLE){
        mode++;
        if(mode==6)
          mode=1;
        STATUS_UPDATE(start(500,0.3,mode,1000),static_cast<HS_STATUS>(HS_ENTERING_CONFIG_MODE+mode))
      } else {
        done=true;
      }
    } // button press
  } // while

  STATUS_UPDATE(start(LED_ALERT),static_cast<HS_STATUS>(HS_ENTERING_CONFIG_MODE+mode+5))
  controlButton->wait();
  
  switch(mode){

    case 1:
      resetStatus();
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

  if(connected%2){
    if(WiFi.status()==WL_CONNECTED)
      return;
      
    addWebLog(true,"*** WiFi Connection Lost!");
    connected++;
    waitTime=60000;
    alarmConnect=0;
    STATUS_UPDATE(start(LED_WIFI_CONNECTING),HS_WIFI_CONNECTING)
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
      addWebLog(true,"Trying to connect to %s.  Waiting %d sec...",network.wifiData.ssid,waitTime/1000);
      WiFi.begin(network.wifiData.ssid,network.wifiData.pwd);
    }

    alarmConnect=millis()+waitTime;

    return;
  }

  resetStatus();  
  connected++;

  addWebLog(true,"WiFi Connected!  IP Address = %s",WiFi.localIP().toString().c_str());

  if(connected>1)                           // Do not initialize everything below if this is only a reconnect
    return;
    
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

  char d[strlen(hostName)+1];  
  sscanf(hostName,"%[A-Za-z0-9-]",d);
  
  if(strlen(hostName)>255|| hostName[0]=='-' || hostName[strlen(hostName)-1]=='-' || strlen(hostName)!=strlen(d)){
    Serial.printf("\n*** Error:  Can't start MDNS due to invalid hostname '%s'.\n",hostName);
    Serial.print("*** Hostname must consist of 255 or less alphanumeric characters or a hyphen, except that the hyphen cannot be the first or last character.\n");
    Serial.print("*** PROGRAM HALTED!\n\n");
    while(1);
  }
    
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

  mdns_service_txt_item_set("_hap","_tcp","hspn",HOMESPAN_VERSION);             // HomeSpan Version Number (info only - NOT used by HAP)
  mdns_service_txt_item_set("_hap","_tcp","ard-esp32",ARDUINO_ESP_VERSION);     // Arduino-ESP32 Version Number (info only - NOT used by HAP)
  mdns_service_txt_item_set("_hap","_tcp","board",ARDUINO_VARIANT);             // Board Name (info only - NOT used by HAP)
  mdns_service_txt_item_set("_hap","_tcp","sketch",sketchVersion);              // Sketch Version (info only - NOT used by HAP)

  uint8_t hashInput[22];
  uint8_t hashOutput[64];
  char setupHash[9];
  size_t len;
  
  memcpy(hashInput,qrID,4);                                           // Create the Seup ID for use with optional QR Codes.  This is an undocumented feature of HAP R2!
  memcpy(hashInput+4,id,17);                                          // Step 1: Concatenate 4-character Setup ID and 17-character Accessory ID into hashInput
  mbedtls_sha512_ret(hashInput,21,hashOutput,0);                      // Step 2: Perform SHA-512 hash on combined 21-byte hashInput to create 64-byte hashOutput
  mbedtls_base64_encode((uint8_t *)setupHash,9,&len,hashOutput,4);    // Step 3: Encode the first 4 bytes of hashOutput in base64, which results in an 8-character, null-terminated, setupHash
  mdns_service_txt_item_set("_hap","_tcp","sh",setupHash);            // Step 4: broadcast the resulting Setup Hash

  if(spanOTA.enabled){
    ArduinoOTA.setHostname(hostName);

    if(spanOTA.auth)
      ArduinoOTA.setPasswordHash(spanOTA.otaPwd);

    ArduinoOTA.onStart(spanOTA.start).onEnd(spanOTA.end).onProgress(spanOTA.progress).onError(spanOTA.error);  
    
    ArduinoOTA.begin();
    Serial.print("Starting OTA Server: ");
    Serial.print(displayName);
    Serial.print(" at ");
    Serial.print(WiFi.localIP());
    Serial.print("\nAuthorization Password: ");
    Serial.print(spanOTA.auth?"Enabled\n\n":"DISABLED!\n\n");
  }
  
  mdns_service_txt_item_set("_hap","_tcp","ota",spanOTA.enabled?"yes":"no");                     // OTA status (info only - NOT used by HAP)

  if(webLog.isEnabled){
    mdns_service_txt_item_set("_hap","_tcp","logURL",webLog.statusURL.c_str()+4);           // Web Log status (info only - NOT used by HAP)
    
    Serial.printf("Web Logging enabled at http://%s.local:%d%swith max number of entries=%d\n\n",hostName,tcpPortNum,webLog.statusURL.c_str()+4,webLog.maxEntries);
    webLog.initTime();
  }
  
  Serial.printf("Starting HAP Server on port %d supporting %d simultaneous HomeKit Controller Connections...\n",tcpPortNum,maxConnections);

  hapServer->begin();

  Serial.print("\n");

  if(!HAPClient::nAdminControllers())
    Serial.print("DEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
  
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
      spanOTA.setPassword(textPwd);
      nvs_set_str(otaNVS,"OTADATA",spanOTA.otaPwd);                 // update data
      nvs_commit(otaNVS);          
      
      Serial.print("... Accepted! Password change will take effect after next restart.\n");
      if(!spanOTA.enabled)
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
        Serial.print("\n*** Invalid request to change Setup Code.  Code must be exactly 8 digits.\n\n");
      } else
      
      if(!network.allowedCode(setupCode)){
        Serial.print("\n*** Invalid request to change Setup Code.  Code too simple.\n\n");
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

      if(homeSpan.pairCallback)
        homeSpan.pairCallback(false);

      resetStatus();      
    }
    break;

    case 'W': {

      if(strlen(network.wifiData.ssid)>0){
        Serial.print("*** Stopping all current WiFi services...\n\n");
        hapServer->end();
        MDNS.end();
        WiFi.disconnect();
      }

      network.serialConfigure();
      nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
      nvs_commit(wifiNVS);                                                            // commit to NVS
      Serial.print("\n*** WiFi Credentials SAVED!  Restarting ***\n\n");
      reboot();  
      }
    break;

    case 'A': {

      if(strlen(network.wifiData.ssid)>0){
        Serial.print("*** Stopping all current WiFi services...\n\n");
        hapServer->end();
        MDNS.end();
        WiFi.disconnect();
      }

      if(apFunction){
        apFunction();
        return;
      }
      
      network.apConfigure();
      nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
      nvs_commit(wifiNVS);                                                            // commit to NVS
      Serial.print("\n*** Credentials saved!\n");
      if(strlen(network.setupCode)){
        char s[10];
        sprintf(s,"S%s",network.setupCode);
        processSerialCommand(s);
      } else {
        Serial.print("*** Setup Code Unchanged\n");
      }
      
      Serial.print("\n*** Restarting...\n\n");
      STATUS_UPDATE(start(LED_ALERT),HS_AP_TERMINATED)
      reboot();
    }
    break;
    
    case 'X': {
     
      nvs_erase_all(wifiNVS);
      nvs_commit(wifiNVS);
      WiFi.begin("none");     
      Serial.print("\n*** WiFi Credentials ERASED!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'V': {
      
      nvs_erase_all(charNVS);
      nvs_commit(charNVS);      
      Serial.print("\n*** Values for all saved Characteristics erased!\n\n");
    }
    break;

    case 'H': {
      
      nvs_erase_all(HAPClient::hapNVS);
      nvs_commit(HAPClient::hapNVS);      
      Serial.print("\n*** HomeSpan Device ID and Pairing Data DELETED!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'R': {

      reboot();      
    }
    break;

    case 'F': {
      
      nvs_erase_all(HAPClient::hapNVS);
      nvs_commit(HAPClient::hapNVS);      
      nvs_erase_all(wifiNVS);
      nvs_commit(wifiNVS);   
      nvs_erase_all(charNVS);
      nvs_commit(charNVS);
      nvs_erase_all(otaNVS);
      nvs_commit(otaNVS);
      WiFi.begin("none");  
      Serial.print("\n*** FACTORY RESET!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'E': {
      
      nvs_flash_erase();
      Serial.print("\n*** ALL DATA ERASED!  Restarting...\n\n");
      reboot();
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

    case 'm': {
      Serial.printf("Free Memory: %d bytes\n",heap_caps_get_free_size(MALLOC_CAP_DEFAULT));  
    }
    break;       

    case 'i':{

      Serial.print("\n*** HomeSpan Info ***\n\n");

      int nErrors=0;
      int nWarnings=0;
      
      unordered_set<uint32_t> aidValues;
      char pNames[][7]={"PR","PW","EV","AA","TW","HD","WR"};

      for(auto acc=Accessories.begin(); acc!=Accessories.end(); acc++){
        Serial.printf("\u27a4 Accessory:  AID=%d\n",(*acc)->aid);
        boolean foundInfo=false;

        if(acc==Accessories.begin() && (*acc)->aid!=1)
          Serial.printf("   *** ERROR #%d!  AID of first Accessory must always be 1 ***\n",++nErrors);

        if(aidValues.find((*acc)->aid)!=aidValues.end())
          Serial.printf("   *** ERROR #%d!  AID already in use for another Accessory ***\n",++nErrors);
        
        aidValues.insert((*acc)->aid);

        for(auto svc=(*acc)->Services.begin(); svc!=(*acc)->Services.end(); svc++){
          Serial.printf("   \u279f Service %s:  IID=%d, %sUUID=\"%s\"",(*svc)->hapName,(*svc)->iid,(*svc)->isCustom?"Custom-":"",(*svc)->type);
          Serial.printf("\n");

          if(!strcmp((*svc)->type,"3E")){
            foundInfo=true;
            if((*svc)->iid!=1)
              Serial.printf("     *** ERROR #%d!  The Accessory Information Service must be defined before any other Services in an Accessory ***\n",++nErrors);
          }
          else if((*acc)->aid==1)            // this is an Accessory with aid=1, but it has more than just AccessoryInfo.  So...
            isBridge=false;                  // ...this is not a bridge device          

          unordered_set<HapChar *> hapChar;
        
          for(auto chr=(*svc)->Characteristics.begin(); chr!=(*svc)->Characteristics.end(); chr++){
            Serial.printf("      \u21e8 Characteristic %s(%s):  IID=%d, %sUUID=\"%s\", %sPerms=",
              (*chr)->hapName,(*chr)->uvPrint((*chr)->value).c_str(),(*chr)->iid,(*chr)->isCustom?"Custom-":"",(*chr)->type,(*chr)->perms!=(*chr)->hapChar->perms?"Custom-":"");

            int foundPerms=0;
            for(uint8_t i=0;i<7;i++){
              if((*chr)->perms & (1<<i))
                Serial.printf("%s%s",(foundPerms++)?"+":"",pNames[i]);
            }           
            
            if((*chr)->format!=FORMAT::STRING && (*chr)->format!=FORMAT::BOOL && (*chr)->format!=FORMAT::DATA){
              if((*chr)->validValues)
                Serial.printf(", Valid Values=%s",(*chr)->validValues);
              else if((*chr)->uvGet<double>((*chr)->stepValue)>0)
                Serial.printf(", %sRange=[%s,%s,%s]",(*chr)->customRange?"Custom-":"",(*chr)->uvPrint((*chr)->minValue).c_str(),(*chr)->uvPrint((*chr)->maxValue).c_str(),(*chr)->uvPrint((*chr)->stepValue).c_str());
              else
                Serial.printf(", %sRange=[%s,%s]",(*chr)->customRange?"Custom-":"",(*chr)->uvPrint((*chr)->minValue).c_str(),(*chr)->uvPrint((*chr)->maxValue).c_str());
            }
            
            if((*chr)->nvsKey)
              Serial.printf(" (nvs)");
            Serial.printf("\n");        
            
            if(!(*chr)->isCustom && !(*svc)->isCustom  && (*svc)->req.find((*chr)->hapChar)==(*svc)->req.end() && (*svc)->opt.find((*chr)->hapChar)==(*svc)->opt.end())
              Serial.printf("          *** WARNING #%d!  Service does not support this Characteristic ***\n",++nWarnings);
            else
            if(invalidUUID((*chr)->type,(*chr)->isCustom))
              Serial.printf("          *** ERROR #%d!  Format of UUID is invalid ***\n",++nErrors);
            else       
            if(hapChar.find((*chr)->hapChar)!=hapChar.end())
              Serial.printf("          *** ERROR #%d!  Characteristic already defined for this Service ***\n",++nErrors);

            if((*chr)->setRangeError)
              Serial.printf("          *** WARNING #%d!  Attempt to set Custom Range for this Characteristic ignored ***\n",++nWarnings);

            if((*chr)->setValidValuesError)
              Serial.printf("          *** WARNING #%d!  Attempt to set Custom Valid Values for this Characteristic ignored ***\n",++nWarnings);

            if((*chr)->format!=STRING && ((*chr)->uvGet<double>((*chr)->value) < (*chr)->uvGet<double>((*chr)->minValue) || (*chr)->uvGet<double>((*chr)->value) > (*chr)->uvGet<double>((*chr)->maxValue)))
              Serial.printf("          *** WARNING #%d!  Value of %g is out of range [%g,%g] ***\n",++nWarnings,(*chr)->uvGet<double>((*chr)->value),(*chr)->uvGet<double>((*chr)->minValue),(*chr)->uvGet<double>((*chr)->maxValue));

            hapChar.insert((*chr)->hapChar);
          
          } // Characteristics

          for(auto req=(*svc)->req.begin(); req!=(*svc)->req.end(); req++){
            if(hapChar.find(*req)==hapChar.end())
              Serial.printf("          *** WARNING #%d!  Required '%s' Characteristic for this Service not found ***\n",++nWarnings,(*req)->hapName);
          }

          for(auto button=PushButtons.begin(); button!=PushButtons.end(); button++){
            if((*button)->service==(*svc)){
              
              if((*button)->buttonType==SpanButton::BUTTON)
                Serial.printf("      \u25bc SpanButton: Pin=%d, Single=%ums, Double=%ums, Long=%ums, Type=",(*button)->pin,(*button)->singleTime,(*button)->doubleTime,(*button)->longTime);
              else
                Serial.printf("      \u25bc SpanToggle: Pin=%d, Toggle=%ums, Type=",(*button)->pin,(*button)->longTime);
                
              if((*button)->triggerType==PushButton::TRIGGER_ON_LOW)
                Serial.printf("TRIGGER_ON_LOW\n");
              else if((*button)->triggerType==PushButton::TRIGGER_ON_HIGH)
                Serial.printf("TRIGGER_ON_HIGH\n");

#if SOC_TOUCH_SENSOR_NUM > 0
              else if((*button)->triggerType==PushButton::TRIGGER_ON_TOUCH)
                Serial.printf("TRIGGER_ON_TOUCH\n");
#endif
              else
                Serial.printf("USER-DEFINED\n");
              
              if((void(*)(int,int))((*svc)->*(&SpanService::button))==(void(*)(int,int))(&SpanService::button))
                Serial.printf("          *** WARNING #%d!  No button() method defined in this Service ***\n",++nWarnings);
            }
          }
          
        } // Services
        
        if(!foundInfo)
          Serial.printf("   *** ERROR #%d!  Required 'AccessoryInformation' Service not found ***\n",++nErrors);
          
      } // Accessories
      
      Serial.printf("\nConfigured as Bridge: %s\n",isBridge?"YES":"NO");
      if(hapConfig.configNumber>0)
        Serial.printf("Configuration Number: %d\n",hapConfig.configNumber);
      Serial.printf("\nDatabase Validation:  Warnings=%d, Errors=%d\n\n",nWarnings,nErrors);    

      char d[]="------------------------------";
      Serial.printf("%-30s  %8s  %10s  %s  %s  %s  %s  %s\n","Service","UUID","AID","IID","Update","Loop","Button","Linked Services");
      Serial.printf("%.30s  %.8s  %.10s  %.3s  %.6s  %.4s  %.6s  %.15s\n",d,d,d,d,d,d,d,d);
      for(int i=0;i<Accessories.size();i++){                             // identify all services with over-ridden loop() methods
        for(int j=0;j<Accessories[i]->Services.size();j++){
          SpanService *s=Accessories[i]->Services[j];
          Serial.printf("%-30s  %8.8s  %10u  %3d  %6s  %4s  %6s  ",s->hapName,s->type,Accessories[i]->aid,s->iid, 
                 (void(*)())(s->*(&SpanService::update))!=(void(*)())(&SpanService::update)?"YES":"NO",
                 (void(*)())(s->*(&SpanService::loop))!=(void(*)())(&SpanService::loop)?"YES":"NO",
                 (void(*)(int,boolean))(s->*(&SpanService::button))!=(void(*)(int,boolean))(&SpanService::button)?"YES":"NO"
                 );
          if(s->linkedServices.empty())
            Serial.print("-");
          for(int k=0;k<s->linkedServices.size();k++){
            Serial.print(s->linkedServices[k]->iid);
            if(k<s->linkedServices.size()-1)
              Serial.print(",");
          }
          Serial.print("\n");
        }
      }

      if(SpanPoint::SpanPoints.size()>0){
        uint8_t channel;
        wifi_second_chan_t channel2; 
        esp_wifi_get_channel(&channel,&channel2);
        Serial.printf("\nFound %d SpanPoint Links:\n\n",SpanPoint::SpanPoints.size());
        Serial.printf("%-17s  %18s  %7s  %7s  %7s\n","Local MAC Address","Remote MAC Address","Send","Receive","Depth"); 
        Serial.printf("%.17s  %.18s  %.7s  %.7s  %.7s\n",d,d,d,d,d);
        for(auto it=SpanPoint::SpanPoints.begin();it!=SpanPoint::SpanPoints.end();it++)
          Serial.printf("%-18s  %02X:%02X:%02X:%02X:%02X:%02X  %7d  %7d  %7d\n",(*it)->peerInfo.ifidx==WIFI_IF_AP?WiFi.softAPmacAddress().c_str():WiFi.macAddress().c_str(),
                 (*it)->peerInfo.peer_addr[0],(*it)->peerInfo.peer_addr[1],(*it)->peerInfo.peer_addr[2],(*it)->peerInfo.peer_addr[3],(*it)->peerInfo.peer_addr[4],(*it)->peerInfo.peer_addr[5],
                 (*it)->sendSize,(*it)->receiveSize,uxQueueSpacesAvailable((*it)->receiveQueue));           
        Serial.printf("\nSpanPoint using WiFi Channel %d%s\n",channel,WiFi.status()!=WL_CONNECTED?" (subject to change once WiFi connection established)":"");
      }
      
      Serial.print("\n*** End Info ***\n\n");
    }
    break;

    case 'P': {
      
      Serial.printf("\n*** Pairing Data used for Cloning another Device\n\n");
      size_t olen;
      TempBuffer<char> tBuf(256);
      mbedtls_base64_encode((uint8_t *)tBuf.buf,256,&olen,(uint8_t *)&HAPClient::accessory,sizeof(struct Accessory));
      Serial.printf("Accessory data:  %s\n",tBuf.buf);
      for(int i=0;i<HAPClient::MAX_CONTROLLERS;i++){
        if(HAPClient::controllers[i].allocated){
          mbedtls_base64_encode((uint8_t *)tBuf.buf,256,&olen,(uint8_t *)(HAPClient::controllers+i),sizeof(struct Controller));
          Serial.printf("Controller data: %s\n",tBuf.buf);
        }
      }
      Serial.printf("\n*** End Pairing Data\n\n");
    }
    break;

    case 'C': {

      Serial.printf("\n*** Clone Pairing Data from another Device\n\n");
      TempBuffer<char> tBuf(200);
      size_t olen;

      tBuf.buf[0]='\0';
      Serial.print(">>> Accessory data:  ");
      readSerial(tBuf.buf,199);
      if(strlen(tBuf.buf)==0){
        Serial.printf("(cancelled)\n\n");
        return;
      }
      mbedtls_base64_decode((uint8_t *)&HAPClient::accessory,sizeof(struct Accessory),&olen,(uint8_t *)tBuf.buf,strlen(tBuf.buf));
      if(olen!=sizeof(struct Accessory)){
        Serial.printf("\n*** Error in size of Accessory data - cloning cancelled.  Restarting...\n\n");
        reboot();
      } else {
        HAPClient::charPrintRow(HAPClient::accessory.ID,17);
        Serial.printf("\n");
      }
      
      for(int i=0;i<HAPClient::MAX_CONTROLLERS;i++){
        tBuf.buf[0]='\0';
        Serial.print(">>> Controller data: ");
        readSerial(tBuf.buf,199);
        if(strlen(tBuf.buf)==0){
          Serial.printf("(done)\n");
          while(i<HAPClient::MAX_CONTROLLERS)              // clear data from remaining controller slots
            HAPClient::controllers[i++].allocated=false;
        } else {
          mbedtls_base64_decode((uint8_t *)(HAPClient::controllers+i),sizeof(struct Controller),&olen,(uint8_t *)tBuf.buf,strlen(tBuf.buf));
          if(olen!=sizeof(struct Controller)){
            Serial.printf("\n*** Error in size of Controller data - cloning cancelled.  Restarting...\n\n");
            reboot();
          } else {
            HAPClient::charPrintRow(HAPClient::controllers[i].ID,36);
            Serial.printf("\n");
          }
        }
      }

      char qSave[2];
      while(1){
        qSave[0]='-';
        Serial.printf("Save Cloned Pairing Data (y/n): ");
        readSerial(qSave,1);
        if(qSave[0]=='y'){
          Serial.printf("(yes)\nData saved!  Rebooting...");
          nvs_set_blob(HAPClient::hapNVS,"ACCESSORY",&HAPClient::accessory,sizeof(HAPClient::accessory));           // update data
          nvs_set_blob(HAPClient::hapNVS,"CONTROLLERS",HAPClient::controllers,sizeof(HAPClient::controllers));      
          nvs_commit(HAPClient::hapNVS);                                                      // commit to NVS
          reboot();
        } else
        if(qSave[0]=='n'){
          Serial.printf("(no)\nProcess Cancelled!  Rebooting...");
          reboot();
        }
        Serial.printf("\n");
      }
    }
    break;   

    case '?': {    
      
      Serial.print("\n*** HomeSpan Commands ***\n\n");
      Serial.print("  s - print connection status\n");
      Serial.print("  i - print summary information about the HAP Database\n");
      Serial.print("  d - print the full HAP Accessory Attributes Database in JSON format\n");
      Serial.print("  m - print free heap memory\n");
      Serial.print("\n");      
      Serial.print("  W - configure WiFi Credentials and restart\n");      
      Serial.print("  X - delete WiFi Credentials and restart\n");      
      Serial.print("  S <code> - change the HomeKit Pairing Setup Code to <code>\n");
      Serial.print("  Q <id> - change the HomeKit Setup ID for QR Codes to <id>\n");
      Serial.print("  O - change the OTA password\n");
      Serial.print("  A - start the HomeSpan Setup Access Point\n");
      Serial.print("\n");      
      Serial.print("  V - delete value settings for all saved Characteristics\n");
      Serial.print("  U - unpair device by deleting all Controller data\n");
      Serial.print("  H - delete HomeKit Device ID as well as all Controller data and restart\n");      
      Serial.print("\n");      
      Serial.print("  P - output Pairing Data that can be saved offline to clone a new device\n");      
      Serial.print("  C - clone Pairing Data previously saved offline from another device\n");      
      Serial.print("\n");      
      Serial.print("  R - restart device\n");      
      Serial.print("  F - factory reset and restart\n");      
      Serial.print("  E - erase ALL stored data and restart\n");      
      Serial.print("\n");          
      Serial.print("  L <level> - change the Log Level setting to <level>\n");
      Serial.print("\n");

      for(auto uCom=homeSpan.UserCommands.begin(); uCom!=homeSpan.UserCommands.end(); uCom++)      // loop over all UserCommands using an iterator
        Serial.printf("  @%c %s\n",uCom->first,uCom->second->s);

      if(!homeSpan.UserCommands.empty())
        Serial.print("\n");
        
      Serial.print("  ? - print this list of commands\n\n");     
      Serial.print("*** End Commands ***\n\n");
    }
    break;

    case '@':{

      auto uCom=UserCommands.find(c[1]);

      if(uCom!=UserCommands.end()){
        if(uCom->second->userFunction1)        
          uCom->second->userFunction1(c+1);
        else
          uCom->second->userFunction2(c+1,uCom->second->userArg);
      } else {
        Serial.print("*** Undefined user command: '");
        Serial.print(c);
        Serial.print("'.  Type '?' for list of commands.\n");        
      }
    }
    break;

    default:
      Serial.print("*** Unknown command: '");
      Serial.print(c);
      Serial.print("'.  Type '?' for list of commands.\n");
    break;
    
  } // switch
}

///////////////////////////////

void Span::resetStatus(){
  if(strlen(network.wifiData.ssid)==0)
    STATUS_UPDATE(start(LED_WIFI_NEEDED),HS_WIFI_NEEDED)
  else if(WiFi.status()!=WL_CONNECTED)
    STATUS_UPDATE(start(LED_WIFI_CONNECTING),HS_WIFI_CONNECTING)
  else if(!HAPClient::nAdminControllers())
    STATUS_UPDATE(start(LED_PAIRING_NEEDED),HS_PAIRING_NEEDED)
  else
    STATUS_UPDATE(on(),HS_PAIRED)
}

///////////////////////////////

void Span::reboot(){
  STATUS_UPDATE(off(),HS_REBOOTING)
  delay(1000);
  ESP.restart();  
}

///////////////////////////////

const char* Span::statusString(HS_STATUS s){
  switch(s){
    case HS_WIFI_NEEDED: return("WiFi Credentials Needed");
    case HS_WIFI_CONNECTING: return("WiFi Connecting");
    case HS_PAIRING_NEEDED: return("Device not yet Paired");
    case HS_PAIRED: return("Device Paired");
    case HS_ENTERING_CONFIG_MODE: return("Entering Command Mode");
    case HS_CONFIG_MODE_EXIT: return("1. Exit Command Mode"); 
    case HS_CONFIG_MODE_REBOOT: return("2. Reboot Device");
    case HS_CONFIG_MODE_LAUNCH_AP: return("3. Launch Access Point");
    case HS_CONFIG_MODE_UNPAIR: return("4. Unpair Device");
    case HS_CONFIG_MODE_ERASE_WIFI: return("5. Erase WiFi Credentials");
    case HS_CONFIG_MODE_EXIT_SELECTED: return("Exiting Command Mode...");
    case HS_CONFIG_MODE_REBOOT_SELECTED: return("Rebooting Device...");
    case HS_CONFIG_MODE_LAUNCH_AP_SELECTED: return("Launching Access Point...");
    case HS_CONFIG_MODE_UNPAIR_SELECTED: return("Unpairing Device...");
    case HS_CONFIG_MODE_ERASE_WIFI_SELECTED: return("Erasing WiFi Credentials...");
    case HS_REBOOTING: return("REBOOTING!");
    case HS_FACTORY_RESET: return("Performing Factory Reset...");
    case HS_AP_STARTED: return("Access Point Started");
    case HS_AP_CONNECTED: return("Access Point Connected");
    case HS_AP_TERMINATED: return("Access Point Terminated");
    case HS_OTA_STARTED: return("OTA Update Started");
    default: return("Unknown");
  }
}

///////////////////////////////

void Span::setWifiCredentials(const char *ssid, const char *pwd){
  sprintf(network.wifiData.ssid,"%.*s",MAX_SSID,ssid);
  sprintf(network.wifiData.pwd,"%.*s",MAX_PWD,pwd);
  if(wifiNVS){                                                                      // is begin() already called and wifiNVS is open
    nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
    nvs_commit(wifiNVS);                                                            // commit to NVS
  }
}

///////////////////////////////

int Span::sprintfAttributes(char *cBuf, int flags){

  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"accessories\":[");

  for(int i=0;i<Accessories.size();i++){
    nBytes+=Accessories[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL,flags);    
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


///////////////////////////

boolean Span::deleteAccessory(uint32_t n){
  
  auto it=homeSpan.Accessories.begin();
  for(;it!=homeSpan.Accessories.end() && (*it)->aid!=n; it++);
  
  if(it==homeSpan.Accessories.end())
    return(false);

  delete *it;
  return(true);
}

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
      if(!strcmp(t2,"value") && (t3=strtok_r(t1,"}[]:,\"",&p2))){
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
          if(status==StatusCode::OK){                                                     // if status is okay
            pObj[j].characteristic->uvSet(pObj[j].characteristic->value,pObj[j].characteristic->newValue);               // update characteristic value with new value
            if(pObj[j].characteristic->nvsKey){                                                                                               // if storage key found
              if(pObj[j].characteristic->format!=FORMAT::STRING && pObj[j].characteristic->format!=FORMAT::DATA)
                nvs_set_blob(charNVS,pObj[j].characteristic->nvsKey,&(pObj[j].characteristic->value),sizeof(pObj[j].characteristic->value));  // store data
              else
                nvs_set_str(charNVS,pObj[j].characteristic->nvsKey,pObj[j].characteristic->value.STRING);                                     // store data
              nvs_commit(charNVS);
            }
            LOG1(" (okay)\n");
          } else {                                                                        // if status not okay
            pObj[j].characteristic->uvSet(pObj[j].characteristic->newValue,pObj[j].characteristic->value);                // replace characteristic new value with original value
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
        
        nChars+=pObj[i].characteristic->sprintfAttributes(cBuf?(cBuf+nChars):NULL,GET_VALUE|GET_AID|GET_NV);    // get JSON attributes for characteristic
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
    Characteristics[i]=find(aid,iid);     // find matching chararacteristic
    
    if(Characteristics[i]){                                          // if found
      if(Characteristics[i]->perms&PERMS::PR){                       // if permissions allow reading
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

boolean Span::updateDatabase(boolean updateMDNS){

  uint8_t tHash[48];
  TempBuffer <char> tBuf(sprintfAttributes(NULL,GET_META|GET_PERMS|GET_TYPE|GET_DESC)+1);
  sprintfAttributes(tBuf.buf,GET_META|GET_PERMS|GET_TYPE|GET_DESC);  
  mbedtls_sha512_ret((uint8_t *)tBuf.buf,tBuf.len(),tHash,1);     // create SHA-384 hash of JSON (can be any hash - just looking for a unique key)

  boolean changed=false;

  if(memcmp(tHash,hapConfig.hashCode,48)){           // if hash code of current HAP database does not match stored hash code
    memcpy(hapConfig.hashCode,tHash,48);             // update stored hash code
    hapConfig.configNumber++;                        // increment configuration number
    if(hapConfig.configNumber==65536)                // reached max value
      hapConfig.configNumber=1;                      // reset to 1
                   
    nvs_set_blob(HAPClient::hapNVS,"HAPHASH",&hapConfig,sizeof(hapConfig));     // update data
    nvs_commit(HAPClient::hapNVS);                                              // commit to NVS
    changed=true;

    if(updateMDNS){
      char cNum[16];
      sprintf(cNum,"%d",hapConfig.configNumber);
      mdns_service_txt_item_set("_hap","_tcp","c#",cNum);      
    }
  }

  Loops.clear();

  for(auto acc=Accessories.begin(); acc!=Accessories.end(); acc++){                        // identify all services with over-ridden loop() methods
    for(auto svc=(*acc)->Services.begin(); svc!=(*acc)->Services.end(); svc++){
      if((void(*)())((*svc)->*(&SpanService::loop)) != (void(*)())(&SpanService::loop))    // save pointers to services in Loops vector
        homeSpan.Loops.push_back((*svc));
    }
  }    

  return(changed);
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
                
  } else {
    this->aid=1;
  }
  
  homeSpan.Accessories.push_back(this);

  if(aid>0){                 // override with user-specified aid
    this->aid=aid;
  }

}

///////////////////////////////

SpanAccessory::~SpanAccessory(){

  while(Services.rbegin()!=Services.rend())           // delete all Services in this Accessory
    delete *Services.rbegin();                       
  
  auto acc=homeSpan.Accessories.begin();              // find Accessory in homeSpan vector and erase entry
  while((*acc)!=this)
    acc++;
  homeSpan.Accessories.erase(acc);
  LOG1("Deleted Accessory AID=%d\n",aid);
}

///////////////////////////////

int SpanAccessory::sprintfAttributes(char *cBuf, int flags){
  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"aid\":%u,\"services\":[",aid);

  for(int i=0;i<Services.size();i++){
    nBytes+=Services[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL,flags);    
    if(i+1<Services.size())
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
    }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]}");

  return(nBytes);
}

///////////////////////////////
//       SpanService         //
///////////////////////////////

SpanService::SpanService(const char *type, const char *hapName, boolean isCustom){

  if(homeSpan.Accessories.empty()){
    Serial.printf("\nFATAL ERROR!  Can't create new Service '%s' without a defined Accessory ***\n",hapName);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  this->type=type;
  this->hapName=hapName;
  this->isCustom=isCustom;

  homeSpan.Accessories.back()->Services.push_back(this);  
  accessory=homeSpan.Accessories.back();
  iid=++(homeSpan.Accessories.back()->iidCount);
}

///////////////////////////////

SpanService::~SpanService(){
  
  while(Characteristics.rbegin()!=Characteristics.rend())           // delete all Characteristics in this Service
    delete *Characteristics.rbegin();
      
  auto svc=accessory->Services.begin();              // find Service in containing Accessory vector and erase entry
  while((*svc)!=this)
    svc++;
  accessory->Services.erase(svc);

  for(svc=homeSpan.Loops.begin(); svc!=homeSpan.Loops.end() && (*svc)!=this; svc++);    // search for entry in Loop vector...
  if(svc!=homeSpan.Loops.end()){                                                        // ...if it exists, erase it
    homeSpan.Loops.erase(svc);
    LOG1("Deleted Loop Entry\n");
  }

  auto pb=homeSpan.PushButtons.begin();         // loop through PushButton vector and delete ALL PushButtons associated with this Service
  while(pb!=homeSpan.PushButtons.end()){
    if((*pb)->service==this){
      pb=homeSpan.PushButtons.erase(pb);
      LOG1("Deleted PushButton on Pin=%d\n",(*pb)->pin);
    }
    else {
      pb++;
    }
  }
  
  LOG1("Deleted Service AID=%d IID=%d\n",accessory->aid,iid); 
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

SpanService *SpanService::addLink(SpanService *svc){
  linkedServices.push_back(svc);
  return(this);
}

///////////////////////////////

int SpanService::sprintfAttributes(char *cBuf, int flags){
  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"iid\":%d,\"type\":\"%s\",",iid,type);
  
  if(hidden)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"hidden\":true,");
    
  if(primary)
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"primary\":true,");

  if(!linkedServices.empty()){
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"linked\":[");
    for(int i=0;i<linkedServices.size();i++){
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"%d",linkedServices[i]->iid);
      if(i+1<linkedServices.size())
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
    }
     nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"],");
  }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"\"characteristics\":[");
  
  for(int i=0;i<Characteristics.size();i++){
    nBytes+=Characteristics[i]->sprintfAttributes(cBuf?(cBuf+nBytes):NULL,flags);    
    if(i+1<Characteristics.size())
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",");
  }
    
  nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,"]}");

  return(nBytes);
}

///////////////////////////////
//    SpanCharacteristic     //
///////////////////////////////

SpanCharacteristic::SpanCharacteristic(HapChar *hapChar, boolean isCustom){

  type=hapChar->type;
  perms=hapChar->perms;
  hapName=hapChar->hapName;
  format=hapChar->format;
  staticRange=hapChar->staticRange;
  this->isCustom=isCustom;
  this->hapChar=hapChar;

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    Serial.printf("\nFATAL ERROR!  Can't create new Characteristic '%s' without a defined Service ***\n",hapName);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  homeSpan.Accessories.back()->Services.back()->Characteristics.push_back(this);  
  iid=++(homeSpan.Accessories.back()->iidCount);
  service=homeSpan.Accessories.back()->Services.back();
  aid=homeSpan.Accessories.back()->aid;

  ev=(boolean *)calloc(homeSpan.maxConnections,sizeof(boolean));
}

///////////////////////////////

SpanCharacteristic::~SpanCharacteristic(){

  auto chr=service->Characteristics.begin();              // find Characteristic in containing Service vector and erase entry
  while((*chr)!=this)
    chr++;
  service->Characteristics.erase(chr);

  free(ev);
  free(desc);
  free(unit);
  free(validValues);
  free(nvsKey);

  if(format==FORMAT::STRING || format==FORMAT::DATA){
    free(value.STRING);
    free(newValue.STRING);
  }
  
  LOG1("Deleted Characteristic AID=%d IID=%d\n",aid,iid);  
}

///////////////////////////////

int SpanCharacteristic::sprintfAttributes(char *cBuf, int flags){
  int nBytes=0;

  const char permCodes[][7]={"pr","pw","ev","aa","tw","hd","wr"};

  const char formatCodes[][9]={"bool","uint8","uint16","uint32","uint64","int","float","string","data"};

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"iid\":%d",iid);

  if(flags&GET_TYPE)  
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"type\":\"%s\"",type);

  if((perms&PR) && (flags&GET_VALUE)){    
    if(perms&NV && !(flags&GET_NV))
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":null");
    else
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%s",uvPrint(value).c_str());      
  }

  if(flags&GET_META){
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"%s\"",formatCodes[format]);
    
    if(customRange && (flags&GET_META)){
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"minValue\":%s,\"maxValue\":%s",uvPrint(minValue).c_str(),uvPrint(maxValue).c_str());
        
      if(uvGet<float>(stepValue)>0)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"minStep\":%s",uvPrint(stepValue).c_str());
    }

    if(unit){
      if(strlen(unit)>0)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"unit\":\"%s\"",unit);
     else
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"unit\":null");
    }

    if(validValues){
      nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?128:0,",\"valid-values\":%s",validValues);      
    }
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
      if(!strcmp(val,"false"))
        newValue.INT=0;
      else if(!strcmp(val,"true"))
        newValue.INT=1;
      else if(!sscanf(val,"%d",&newValue.INT))
        return(StatusCode::InvalidValue);
      break;

    case UINT8:
      if(!strcmp(val,"false"))
        newValue.UINT8=0;
      else if(!strcmp(val,"true"))
        newValue.UINT8=1;
      else if(!sscanf(val,"%hhu",&newValue.UINT8))
        return(StatusCode::InvalidValue);
      break;
            
    case UINT16:
      if(!strcmp(val,"false"))
        newValue.UINT16=0;
      else if(!strcmp(val,"true"))
        newValue.UINT16=1;
      else if(!sscanf(val,"%hu",&newValue.UINT16))
        return(StatusCode::InvalidValue);
      break;
      
    case UINT32:
      if(!strcmp(val,"false"))
        newValue.UINT32=0;
      else if(!strcmp(val,"true"))
        newValue.UINT32=1;
      else if(!sscanf(val,"%u",&newValue.UINT32))
        return(StatusCode::InvalidValue);
      break;
      
    case UINT64:
      if(!strcmp(val,"false"))
        newValue.UINT64=0;
      else if(!strcmp(val,"true"))
        newValue.UINT64=1;
      else if(!sscanf(val,"%llu",&newValue.UINT64))
        return(StatusCode::InvalidValue);
      break;

    case FLOAT:
      if(!sscanf(val,"%lg",&newValue.FLOAT))
        return(StatusCode::InvalidValue);
      break;

    case STRING:
      uvSet(newValue,(const char *)val);
      break;

    default:
    break;

  } // switch

  isUpdated=true;
  updateTime=homeSpan.snapTime;
  return(StatusCode::TBD);
}

///////////////////////////////

unsigned long SpanCharacteristic::timeVal(){
  
  return(homeSpan.snapTime-updateTime);
}

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::setValidValues(int n, ...){
 
  String s="[";
  va_list vl;
  va_start(vl,n);
  for(int i=0;i<n;i++){
    switch(format){
      case FORMAT::UINT8:
        s+=(uint8_t)va_arg(vl,uint32_t);
        break;
      case FORMAT::UINT16:
        s+=(uint16_t)va_arg(vl,uint32_t);
        break;
      case FORMAT::UINT32:
        s+=(uint32_t)va_arg(vl,uint32_t);
        break;
      case FORMAT::INT:
        s+=(int)va_arg(vl,uint32_t);
        break;
      default:
        setValidValuesError=true;
        return(this);      
    }
    if(i!=n-1)
      s+=",";
  }
  va_end(vl);
  s+="]";

  validValues=(char *)realloc(validValues, strlen(s.c_str()) + 1);
  strcpy(validValues,s.c_str());

  return(this);
}

///////////////////////////////
//        SpanRange          //
///////////////////////////////

SpanRange::SpanRange(int min, int max, int step){

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty() || homeSpan.Accessories.back()->Services.back()->Characteristics.empty() ){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanRange(%d,%d,%d) without a defined Characteristic ***\n",min,max,step);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  } else {
    homeSpan.Accessories.back()->Services.back()->Characteristics.back()->setRange(min,max,step);
  }
}

///////////////////////////////
//        SpanButton         //
///////////////////////////////

SpanButton::SpanButton(int pin, uint16_t longTime, uint16_t singleTime, uint16_t doubleTime, triggerType_t triggerType) : PushButton(pin, triggerType){

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    if(buttonType==BUTTON)
      Serial.printf("\nFATAL ERROR!  Can't create new SpanButton(%d,%u,%u,%u) without a defined Service ***\n",pin,longTime,singleTime,doubleTime);
    else
      Serial.printf("\nFATAL ERROR!  Can't create new SpanToggle(%d,%u) without a defined Service ***\n",pin,longTime);
      
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  this->longTime=longTime;
  this->singleTime=singleTime;
  this->doubleTime=doubleTime;
  service=homeSpan.Accessories.back()->Services.back();

  homeSpan.PushButtons.push_back(this);
}

///////////////////////////////

void SpanButton::check(){

  if( (buttonType==BUTTON && triggered(singleTime,longTime,doubleTime)) ||
      (buttonType==TOGGLE && toggled(longTime)) )                             // if the underlying PushButton is triggered/toggled
    service->button(pin,type());                                              // call the Service's button() routine with pin and type as parameters    
}

///////////////////////////////
//     SpanUserCommand       //
///////////////////////////////

SpanUserCommand::SpanUserCommand(char c, const char *s, void (*f)(const char *)){
  this->s=s;
  userFunction1=f;
   
  homeSpan.UserCommands[c]=this;
}

///////////////////////////////

SpanUserCommand::SpanUserCommand(char c, const char *s, void (*f)(const char *, void *), void *arg){
  this->s=s;
  userFunction2=f;
  userArg=arg;
   
  homeSpan.UserCommands[c]=this;
}

///////////////////////////////
//        SpanWebLog         //
///////////////////////////////

void SpanWebLog::init(uint16_t maxEntries, const char *serv, const char *tz, const char *url){
  isEnabled=true;
  this->maxEntries=maxEntries;
  timeServer=serv;
  timeZone=tz;
  statusURL="GET /" + String(url) + " ";
  log = (log_t *)calloc(maxEntries,sizeof(log_t));
  if(timeServer)
    homeSpan.reserveSocketConnections(1);
}

///////////////////////////////

void SpanWebLog::initTime(){
  if(!timeServer)
    return;

  Serial.printf("Acquiring Time from %s (%s).  Waiting %d second(s) for response... ",timeServer,timeZone,waitTime/1000);
  configTzTime(timeZone,timeServer);
  struct tm timeinfo;
  if(getLocalTime(&timeinfo,waitTime)){
    strftime(bootTime,sizeof(bootTime),"%c",&timeinfo);
    Serial.printf("%s\n\n",bootTime);
    timeInit=true;
  } else {
    Serial.printf("Can't access Time Server - time-keeping not initialized!\n\n");
  }
}

///////////////////////////////

void SpanWebLog::vLog(boolean sysMsg, const char *fmt, va_list ap){

  char *buf;
  vasprintf(&buf,fmt,ap);

  if(sysMsg)
    Serial.printf("%s\n",buf);
  else if(homeSpan.logLevel>0)
    Serial.printf("WEBLOG: %s\n",buf);
  
  if(maxEntries>0){
    int index=nEntries%maxEntries;
  
    log[index].upTime=esp_timer_get_time();
    if(timeInit)
      getLocalTime(&log[index].clockTime,10);
    else
      log[index].clockTime.tm_year=0;
  
    log[index].message=(char *)realloc(log[index].message, strlen(buf) + 1);
    strcpy(log[index].message, buf);
    
    log[index].clientIP=homeSpan.lastClientIP;  
    nEntries++;
  }

  free(buf);
}

///////////////////////////////
//         SpanOTA           //
///////////////////////////////

int SpanOTA::init(boolean _auth, boolean _safeLoad, const char *pwd){
  if(esp_ota_get_running_partition()==esp_ota_get_next_update_partition(NULL)){
    Serial.print("\n*** WARNING: Can't start OTA Server - Partition table used to compile this sketch is not configured for OTA.\n\n");
    return(-1);     
  }
  
  enabled=true;
  safeLoad=_safeLoad;
  auth=_auth;
  homeSpan.reserveSocketConnections(1);
  if(pwd==NULL)
    return(0);
  return(setPassword(pwd));
}

///////////////////////////////

int SpanOTA::setPassword(const char *pwd){
  if(strlen(pwd)<1 || strlen(pwd)>32){
    Serial.printf("\n*** WARNING: Cannot change OTA password to '%s'. Password length must be between 1 and 32 characters.\n\n",pwd);
    return(-1);
  }

  MD5Builder otaPwdHash;
  otaPwdHash.begin();
  otaPwdHash.add(pwd);
  otaPwdHash.calculate();
  otaPwdHash.getChars(homeSpan.spanOTA.otaPwd);  
  return(0);
}

///////////////////////////////

void SpanOTA::start(){
  Serial.printf("\n*** Current Partition: %s\n*** New Partition: %s\n*** OTA Starting..",
    esp_ota_get_running_partition()->label,esp_ota_get_next_update_partition(NULL)->label);
  otaPercent=0;
  STATUS_UPDATE(start(LED_OTA_STARTED),HS_OTA_STARTED)
}

///////////////////////////////

void SpanOTA::end(){
  nvs_set_u8(homeSpan.otaNVS,"OTA_REQUIRED",safeLoad);
  nvs_commit(homeSpan.otaNVS);
  Serial.printf(" DONE!  Rebooting...\n");
  homeSpan.reboot();
}

///////////////////////////////

void SpanOTA::progress(uint32_t progress, uint32_t total){
  int percent=progress*100/total;
  if(percent/10 != otaPercent/10){
    otaPercent=percent;
    Serial.printf("%d%%..",progress*100/total);
  }

  if(safeLoad && progress==total){
    SpanPartition newSpanPartition;   
    esp_partition_read(esp_ota_get_next_update_partition(NULL), sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t), &newSpanPartition, sizeof(newSpanPartition));
    Serial.printf("Checking for HomeSpan Magic Cookie: %s..",newSpanPartition.magicCookie);
    if(strcmp(newSpanPartition.magicCookie,spanPartition.magicCookie))
      Update.abort();
  }
}

///////////////////////////////

void SpanOTA::error(ota_error_t err){
  Serial.printf("*** OTA Error[%u]: ", err);
  if (err == OTA_AUTH_ERROR) Serial.println("Auth Failed\n");
    else if (err == OTA_BEGIN_ERROR) Serial.println("Begin Failed\n");
    else if (err == OTA_CONNECT_ERROR) Serial.println("Connect Failed\n");
    else if (err == OTA_RECEIVE_ERROR) Serial.println("Receive Failed\n");
    else if (err == OTA_END_ERROR) Serial.println("End Failed\n");
}

///////////////////////////////

int SpanOTA::otaPercent;
boolean SpanOTA::safeLoad;
boolean SpanOTA::enabled=false;
boolean SpanOTA::auth;

///////////////////////////////
//        SpanPoint          //
///////////////////////////////

SpanPoint::SpanPoint(const char *macAddress, int sendSize, int receiveSize, int queueDepth, boolean useAPaddress){

  if(sscanf(macAddress,"%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",peerInfo.peer_addr,peerInfo.peer_addr+1,peerInfo.peer_addr+2,peerInfo.peer_addr+3,peerInfo.peer_addr+4,peerInfo.peer_addr+5)!=6){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\") - Invalid MAC Address ***\n",macAddress);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(sendSize<0 || sendSize>200 || receiveSize<0 || receiveSize>200 || queueDepth<1 || (sendSize==0 && receiveSize==0)){
    Serial.printf("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\",%d,%d,%d) - one or more invalid parameters ***\n",macAddress,sendSize,receiveSize,queueDepth);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  this->sendSize=sendSize;
  this->receiveSize=receiveSize;
  
  if(receiveSize>0)
    WiFi.mode(WIFI_AP_STA);
  else if(WiFi.getMode()==WIFI_OFF)
    WiFi.mode(WIFI_STA);    

  init();                             // initialize SpanPoint
  peerInfo.channel=0;                 // 0 = matches current WiFi channel
  
  peerInfo.ifidx=useAPaddress?WIFI_IF_AP:WIFI_IF_STA;         // specify interface as either STA or AP
  
  peerInfo.encrypt=true;              // turn on encryption for this peer
  memcpy(peerInfo.lmk, lmk, 16);      // set local key
  esp_now_add_peer(&peerInfo);        // add peer to ESP-NOW

  if(receiveSize>0)
    receiveQueue = xQueueCreate(queueDepth,receiveSize);  

  SpanPoints.push_back(this);             
}

///////////////////////////////

void SpanPoint::init(const char *password){

  if(initialized)
    return;

  if(WiFi.getMode()==WIFI_OFF)
    WiFi.mode(WIFI_STA);  
  
  wifi_config_t conf;                       // make sure AP is hidden (if WIFI_AP_STA is used), since it is just a "dummy" AP to keep WiFi alive for ESP-NOW
  esp_wifi_get_config(WIFI_IF_AP,&conf);
  conf.ap.ssid_hidden=1;
  esp_wifi_set_config(WIFI_IF_AP,&conf);
    
  uint8_t hash[32];
  mbedtls_sha256_ret((const unsigned char *)password,strlen(password),hash,0);      // produce 256-bit bit hash from password

  esp_now_init();                           // initialize ESP-NOW
  memcpy(lmk, hash, 16);                    // store first 16 bytes of hash for later use as local key
  esp_now_set_pmk(hash+16);                 // set hash for primary key using last 16 bytes of hash
  esp_now_register_recv_cb(dataReceived);   // set callback for receiving data
  esp_now_register_send_cb(dataSent);       // set callback for sending data
  
  statusQueue = xQueueCreate(1,sizeof(esp_now_send_status_t));    // create statusQueue even if not needed
  setChannelMask(channelMask);                                    // default channel mask at start-up uses channels 1-13  

  uint8_t channel;
  if(!isHub){                                                   // this is not a hub
    nvs_flash_init();                                           // initialize NVS
    nvs_open("POINT",NVS_READWRITE,&pointNVS);                  // open SpanPoint data namespace in NVS
    if(!nvs_get_u8(pointNVS,"CHANNEL",&channel)){               // if channel found in NVS...
      if(channelMask & (1<<channel))                            // ... and if channel is allowed by channel mask
        esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);   // set the WiFi channel
    }
  }
  
  initialized=true;
}

///////////////////////////////

void SpanPoint::setChannelMask(uint16_t mask){
  channelMask = mask & 0x3FFE;

  if(isHub)
    return;

  uint8_t channel=0;

  for(int i=1;i<=13 && channel==0;i++)          // find first "allowed" channel based on mask
    channel=(channelMask & (1<<i))?i:0;

  if(channel==0){
    Serial.printf("\nFATAL ERROR!  SpanPoint::setChannelMask(0x%04X) - mask must allow for at least one channel ***\n",mask);
    Serial.printf("\n=== PROGRAM HALTED ===");
    while(1);
  }

  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
}

///////////////////////////////

uint8_t SpanPoint::nextChannel(){

  uint8_t channel;
  wifi_second_chan_t channel2; 
  esp_wifi_get_channel(&channel,&channel2);     // get current channel

  if(isHub || channelMask==(1<<channel))        // do not change channel if device is either a hub, or channel mask does not allow for any other channels
    return(channel);

  do {
    channel=(channel<13)?channel+1:1;       // advance to next channel
  } while(!(channelMask & (1<<channel)));   // until we find next valid one

  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);         // set the WiFi channel
  
  nvs_set_u8(pointNVS,"CHANNEL",channel);
  nvs_commit(pointNVS);  
     
  return(channel);  
}

///////////////////////////////

boolean SpanPoint::get(void *dataBuf){

  if(receiveSize==0)
    return(false);

  return(xQueueReceive(receiveQueue, dataBuf, 0));
}

///////////////////////////////

boolean SpanPoint::send(const void *data){

  if(sendSize==0)
    return(false);
  
  uint8_t channel;
  wifi_second_chan_t channel2; 
  esp_wifi_get_channel(&channel,&channel2);     // get current channel
  uint8_t startingChannel=channel;              // set starting channel to current channel

  esp_now_send_status_t status = ESP_NOW_SEND_FAIL;

  do {
    for(int i=1;i<=3;i++){
      
      LOG1("SpanPoint: Sending %d bytes to MAC Address %02X:%02X:%02X:%02X:%02X:%02X using channel %hhu...\n",
        sendSize,peerInfo.peer_addr[0],peerInfo.peer_addr[1],peerInfo.peer_addr[2],peerInfo.peer_addr[3],peerInfo.peer_addr[4],peerInfo.peer_addr[5],channel);
        
      esp_now_send(peerInfo.peer_addr, (uint8_t *) data, sendSize);
      xQueueReceive(statusQueue, &status, pdMS_TO_TICKS(2000));
      if(status==ESP_NOW_SEND_SUCCESS)
        return(true);
      delay(10);
    }    
    channel=nextChannel();
  } while(channel!=startingChannel);

  return(false);
} 

///////////////////////////////

void SpanPoint::dataReceived(const uint8_t *mac, const uint8_t *incomingData, int len){
  
  auto it=SpanPoints.begin();
  for(;it!=SpanPoints.end() && memcmp((*it)->peerInfo.peer_addr,mac,6)!=0; it++);
  
  if(it==SpanPoints.end())
    return;

  if((*it)->receiveSize==0)
    return;

  if(len!=(*it)->receiveSize){
    Serial.printf("SpanPoint Warning! %d bytes received from %02X:%02X:%02X:%02X:%02X:%02X does not match %d-byte queue size\n",len,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],(*it)->receiveSize);
    return;
  }

  (*it)->receiveTime=millis();                             // set time of receive
  xQueueSend((*it)->receiveQueue, incomingData, 0);        // send to queue - do not wait if queue is full and instead fail immediately since we need to return from this function ASAP
}

///////////////////////////////

uint8_t SpanPoint::lmk[16];
boolean SpanPoint::initialized=false;
boolean SpanPoint::isHub=false;
vector<SpanPoint *> SpanPoint::SpanPoints;
uint16_t SpanPoint::channelMask=0x3FFE;
QueueHandle_t SpanPoint::statusQueue;
nvs_handle SpanPoint::pointNVS;


///////////////////////////////
//          MISC             //
///////////////////////////////

void __attribute__((weak)) loop(){
}

///////////////////////////////



 
