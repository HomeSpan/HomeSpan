/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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

#include "version.h"
 
#include <ESPmDNS.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <WiFi.h>
#include <driver/ledc.h>
#include <mbedtls/version.h>
#include <mbedtls/sha256.h>
#include <esp_task_wdt.h>
#include <esp_sntp.h>
#include <esp_wifi.h>
#include <esp_app_format.h>
#include <esp_flash.h>

#if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(3, 3, 2)
  #include <SHA2Builder.h>
  typedef SHA256Builder hsOtaHash_t;
#else
  #include <MD5Builder.h>
  typedef MD5Builder hsOtaHash_t;
#endif

#include "HomeSpan.h"
#include "HAP.h"
#include <mutex>
#include <algorithm>

const __attribute__((section(".rodata_custom_desc"))) SpanPartition spanPartition = {HOMESPAN_MAGIC_COOKIE,0};

using namespace Utils;

HapOut hapOut;                      // Specialized output stream that can both print to serial monitor and encrypt/transmit to HAP Clients with minimal memory usage (global-scoped variable)
Span homeSpan;                      // HAP Attributes database and all related control functions for this Accessory (global-scoped variable)
HapCharacteristics hapChars;        // Instantiation of all HAP Characteristics used to create SpanCharacteristics (global-scoped variable)

///////////////////////////////
//        init()             //
///////////////////////////////

// init() is a global "weak" function pre-defined in the Arduino-ESP32 library.
// It gets called at the end of initArduino(), which in turn is called just before the loopTask that then calls setup() and loop().
// Defining init() here allows HomeSpan to perform late-stage initializations immediately before setup() is called

extern "C" void init(){                        
  static boolean initialized=false;         // ensure this function is only called once
  if(initialized)
    return;
  initialized=true;
  homeSpan.init();                          // call the init() method in homeSpan
}

///////////////////////////////
//         Span              //
///////////////////////////////

Span::Span(){

  nvs_flash_init();                                       // initialize non-volatile-storage partition in flash 

  nvs_open("CHAR",NVS_READWRITE,&charNVS);                // open Characteristic data namespace in NVS
  nvs_open("WIFI",NVS_READWRITE,&wifiNVS);                // open WIFI data namespace in NVS
  nvs_open("OTA",NVS_READWRITE,&otaNVS);                  // open OTA data namespace in NVS

  nvs_open("SRP",NVS_READWRITE,&srpNVS);                  // open SRP data namespace in NVS 
  nvs_open("HAP",NVS_READWRITE,&hapNVS);                  // open HAP data namespace in NVS
  
  nvs_get_u8(wifiNVS,"REBOOTS",&rebootCount);
  rebootCount++;
  nvs_set_u8(wifiNVS,"REBOOTS",rebootCount);
  nvs_commit(wifiNVS);
}

///////////////////////////////

void Span::init(){

  log_i("Initializing Span");

  WiFi.setAutoReconnect(false);                           // allow HomeSpan to handle disconnect/reconnect logic
  WiFi.persistent(false);                                 // do not permanently store WiFi configuration data
  WiFi.setScanMethod(WIFI_ALL_CHANNEL_SCAN);              // scan ALL channels - do NOT stop at first SSID match, else you could connect to weaker BSSID
  WiFi.setSortMethod(WIFI_CONNECT_AP_BY_SIGNAL);          // sort scan data by RSSI and connect to strongest BSSID with matching SSID
  
  networkEventQueue=xQueueCreate(10,sizeof(arduino_event_t));             // queue to transmit network events
  
  Network.onEvent([](arduino_event_t *event){xQueueSend(homeSpan.networkEventQueue, event, (TickType_t) 0);});
  Network.onEvent([](arduino_event_id_t event){homeSpan.useEthernet();},arduino_event_id_t::ARDUINO_EVENT_ETH_START);   
}

///////////////////////////////

void Span::begin(Category catID, const char *_displayName, const char *_hostNameBase, const char *_modelName){

  loopTaskHandle=xTaskGetCurrentTaskHandle();                 // a roundabout way of getting the current task handle
  
  asprintf(&displayName,"%s",_displayName);
  asprintf(&hostNameBase,"%s",_hostNameBase);
  asprintf(&modelName,"%s",_modelName);
  sprintf(this->category,"%d",(int)catID);

  SpanPoint::setAsHub();

  statusLED=new Blinker(statusDevice,autoOffLED);             // create Status LED, even is statusDevice is NULL

  hapServer=new NetworkServer(tcpPortNum);                    // create HAP Server (can be WiFi or Ethernet)
 
  size_t len;

  if(strlen(network.wifiData.ssid)){                                                // if setWifiCredentials was already called
    nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
    nvs_commit(wifiNVS);                                                            // commit to NVS
  } else
  
  if(!nvs_get_blob(wifiNVS,"WIFIDATA",NULL,&len))                                   // else if found WiFi data in NVS
    nvs_get_blob(wifiNVS,"WIFIDATA",&homeSpan.network.wifiData,&len);               // retrieve data  

  delay(2000);

  LOG0("\n************************************************************\n"
                 "Welcome to HomeSpan!\n"
                 "Apple HomeKit for the Espressif ESP-32/S2/S3/C3/C5/C6 chips\n"
                 "************************************************************\n\n"
                 "** Please ensure serial monitor is set to transmit <newlines>\n\n");

  processSerialCommand("c");        // print homeSpan configuration

  uint8_t otaRequired=0;
  nvs_get_u8(otaNVS,"OTA_REQUIRED",&otaRequired);
  nvs_set_u8(otaNVS,"OTA_REQUIRED",0);
  nvs_commit(otaNVS);
  if(otaRequired && !spanOTA.enabled){
    LOG0("\n\n*** OTA SAFE MODE ALERT:  OTA REQUIRED BUT NOT ENABLED.  ROLLING BACK TO PREVIOUS APPLICATION ***\n\n");
    delay(100);
    esp_ota_mark_app_invalid_rollback_and_reboot();
  }
  
}  // begin

///////////////////////////////

void Span::poll() {

  if(pollTaskHandle){
    LOG0("\n** FATAL ERROR: Do not call homeSpan.poll() directly if homeSpan.autoPoll() is used!\n** PROGRAM HALTED **\n\n");
    vTaskDelete(pollTaskHandle);
    while(1);    
  }
  
  pollTask();
}

///////////////////////////////

void Span::pollTask() {

  std::unique_lock pollLock(homeSpan.pollMutex);

  if(!strlen(category)){
    LOG0("\n** FATAL ERROR: Cannot start homeSpan polling without an initial call to homeSpan.begin()!\n** PROGRAM HALTED **\n\n");
    while(1);    
  }

  if(!isInitialized){
  
    processSerialCommand("i");        // print homeSpan configuration info
           
    HAPClient::init();                // read NVS and load HAP settings  

    if(pollTaskHandle)
      LOG0("*** AutoPolling Task started on Core-%d with priority=%d\n\n",xTaskGetCoreID(pollTaskHandle),uxTaskPriorityGet(pollTaskHandle));

    if(heap_caps_get_free_size(MALLOC_CAP_DEFAULT|MALLOC_CAP_INTERNAL)<DEFAULT_LOW_MEM_THRESHOLD)
      LOG0("\n*** WARNING!  Internal Free Heap of %d bytes is less than Low-Memory Threshold of %d bytes.  Device *may* run out of Internal memory.\n\n",
          heap_caps_get_free_size(MALLOC_CAP_DEFAULT|MALLOC_CAP_INTERNAL),DEFAULT_LOW_MEM_THRESHOLD);

    if(!ethernetEnabled && !strlen(network.wifiData.ssid)){
      LOG0("*** WIFI CREDENTIALS DATA NOT FOUND.  ");
      if(autoStartAPEnabled){
        LOG0("AUTO-START OF ACCESS POINT ENABLED...\n\n");
        processSerialCommand("A");
      } else {
        LOG0("YOU MAY CONFIGURE BY TYPING 'W <RETURN>'.\n\n");
      }      
    }
                
    if(controlButton)
      controlButton->reset();

    resetStatus();     
  
    LOG0("%s is READY!\n\n",displayName);
    isInitialized=true;    
    
  } // isInitialized

  if(!ethernetEnabled && strlen(network.wifiData.ssid) && !(connected%2) && millis()>alarmConnect){
    if(verboseWifiReconnect)
      addWebLog(true,"Trying to connect to %s.  Waiting %ld sec...",network.wifiData.ssid,wifiTimeCounter/1000);
    
    alarmConnect=millis()+(wifiTimeCounter++);
    wifiBegin(network.wifiData.ssid,network.wifiData.pwd);
  }

  if(rescanStatus==RESCAN_PENDING && millis()>rescanAlarm){
    rescanStatus=RESCAN_RUNNING;
    LOG2("Rescanning %s for potentially better BSSID...\n",network.wifiData.ssid);
    WiFi.scanDelete();
    STATUS_UPDATE(start(LED_WIFI_SCANNING),HS_WIFI_SCANNING)
    WiFi.scanNetworks(true, false, false, 300, 0, network.wifiData.ssid, nullptr);     // start scan in background
  }

  arduino_event_t event;
  if(xQueueReceive(networkEventQueue, &event, (TickType_t)0))
    networkCallback(event);

  char cBuf[65]="-";
  
  if(!serialInputDisabled && Serial.available()){
    readSerial(cBuf,64);
    processSerialCommand(cBuf);
  }

  if(hapServer->hasClient()){  
 
    auto it=hapList.emplace(hapList.begin());                                // create new HAPClient connection
    it->client=hapServer->accept();
    it->clientNumber=it->client.fd()-LWIP_SOCKET_OFFSET;
            
    HAPClient::pairStatus=pairState_M1;                                      // reset starting PAIR STATE (which may be needed if Accessory failed in middle of pair-setup)    

    LOG2("=======================================\n");
    LOG1("** Client #%d Connected (%lu sec): %s\n",it->clientNumber,millis()/1000,it->client.remoteIP().toString().c_str());
    LOG2("\n");
  }

  currentClient=hapList.begin();
  while(currentClient!=hapList.end()){

    if(currentClient->client.connected()){                                   // if the client is connected
      if(currentClient->client.available()){                                 // if client has data available
        homeSpan.lastClientIP=currentClient->client.remoteIP().toString();   // store IP Address for web logging
        currentClient->processRequest();                                     // PROCESS HAP REQUEST
        homeSpan.lastClientIP="0.0.0.0";                                     // reset stored IP address to show "0.0.0.0" if homeSpan.getClientIP() is used in any other context 
      }
      currentClient++;
    } else {
      LOG1("** Client #%d DISCONNECTED (%lu sec)\n",currentClient->clientNumber,millis()/1000);
      clearNotify(&*currentClient);                                          // clear all notification requests for this connection
      currentClient=hapList.erase(currentClient);                            // remove HAPClient connection
    }
  }
      
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

  if(rebootCallback && snapTime>rebootCallbackTime){
    rebootCallback(rebootCount-1);
    rebootCount=0;
    rebootCallback=NULL;
    nvs_set_u8(wifiNVS,"REBOOTS",rebootCount);
    nvs_commit(wifiNVS);    
  }

  if(!initialPollingCompleted && pollingCallback){
    initialPollingCompleted=true;
    pollingCallback();
  }

  pollLock.unlock();
  resetWatchdog();      // reset watchdog timer  
} // poll

//////////////////////////////////////

void Span::commandMode(){

  if(!statusDevice && !statusCallback){
    LOG0("*** ERROR: CAN'T ENTER COMMAND MODE WITHOUT A DEFINED STATUS LED OR EVENT HANDLER CALLBACK***\n\n");
    return;
  }
  
  LOG0("*** COMMAND MODE ***\n\n");
  int mode=1;
  boolean done=false;
  STATUS_UPDATE(start(500,0.3,mode,1000),static_cast<HS_STATUS>(HS_ENTERING_CONFIG_MODE+mode))
  unsigned long alarmTime=millis()+comModeLife;

  while(!done){
    if(millis()>alarmTime){
      LOG0("*** Command Mode: Timed Out (%ld seconds)",comModeLife/1000);
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
    
    resetWatchdog();
  } // while

  STATUS_UPDATE(start(LED_ALERT),static_cast<HS_STATUS>(HS_ENTERING_CONFIG_MODE+mode+5))
  controlButton->wait();
  
  switch(mode){

    case 1:
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
  
  LOG0("*** EXITING COMMAND MODE ***\n\n");
  resetStatus();
}

//////////////////////////////////////

Span& Span::setConnectionTimes(uint32_t minTime, uint32_t maxTime, uint8_t nSteps){
  
  if(minTime==0 || maxTime==0 || nSteps==0)
    LOG0("\n*** WARNING!  Call to setConnectionTimes(%ld,%ld,%d) ignored: illegal parameters\n",minTime,maxTime,nSteps);
  else
    wifiTimeCounter.config(minTime*1000,maxTime*1000,nSteps);
  return(*this);
}

//////////////////////////////////////

void Span::networkCallback(const arduino_event_t &event){
  
  switch (event.event_id) {

    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      LOG2("Acquiring WiFi IP Addresses...\n");
    break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      if(connected%2){
        connected++;
        addWebLog(true,"*** WiFi Connection Lost!");
        wifiTimeCounter.reset();
        alarmConnect=millis();
        if(rescanInitialTime>0){
          rescanAlarm=millis()+rescanInitialTime;
          rescanStatus=RESCAN_PENDING;
        }
        resetStatus();
      }
    break;

    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
    case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
      if(event.event_id==ARDUINO_EVENT_WIFI_STA_GOT_IP6){
        IPAddress ip6=IPAddress(IPv6, (const uint8_t *)event.event_info.got_ip6.ip6_info.ip.addr, event.event_info.got_ip6.ip6_info.ip.zone);
        addWebLog(true,"Received IPv6 Address: %s",ip6.toString(true).c_str());
      } else {
        addWebLog(true,"Received IPv4 Address: %s",WiFi.localIP().toString().c_str());
      }
      if(!(connected%2)){
        if(bssidNames.count(WiFi.BSSIDstr().c_str()))
          addWebLog(true,"WiFi Connected! (RSI=%d  BSSID=%s  \"%s\")",WiFi.RSSI(),WiFi.BSSIDstr().c_str(),bssidNames[WiFi.BSSIDstr().c_str()].c_str());
        else
          addWebLog(true,"WiFi Connected! (RSI=%d  BSSID=%s)",WiFi.RSSI(),WiFi.BSSIDstr().c_str());      
        connected++;
        if(connected==1)
          configureNetwork();
        if(connectionCallback)
          connectionCallback((connected+1)/2);
        if(rescanInitialTime>0){
          rescanAlarm=millis()+rescanInitialTime;
          rescanStatus=RESCAN_PENDING;
        }
        resetStatus();
      }
    break;

    case ARDUINO_EVENT_WIFI_SCAN_DONE:
      if(rescanStatus==RESCAN_RUNNING){
        if(WiFi.scanComplete()>0 && WiFi.BSSIDstr(0)!=WiFi.BSSIDstr() && WiFi.RSSI(0)>=WiFi.RSSI()+rescanThreshold){
          addWebLog(true,"*** Switching to Access Point with stronger RSSI...");
          WiFi.disconnect();
        } else {
          LOG2("Rescan completed.  No stronger signals found.\n");
          if(rescanPeriodicTime>0){
            rescanAlarm=millis()+rescanPeriodicTime;
            rescanStatus=RESCAN_PENDING;
          } else {
            rescanStatus=RESCAN_IDLE;            
          }
        }
      }
      resetStatus();     
    break;

    case ARDUINO_EVENT_ETH_CONNECTED:
      LOG2("Acquiring Ethernet IP Addresses...\n");
    break;

    case ARDUINO_EVENT_ETH_GOT_IP:
    case ARDUINO_EVENT_ETH_GOT_IP6:
      if(event.event_id==ARDUINO_EVENT_ETH_GOT_IP6){
        IPAddress ip6=IPAddress(IPv6, (const uint8_t *)event.event_info.got_ip6.ip6_info.ip.addr, event.event_info.got_ip6.ip6_info.ip.zone);
        addWebLog(true,"Received IPv6 Address: %s",ip6.toString(true).c_str());
      } else {
        addWebLog(true,"Received IPv4 Address: %s",ETH.localIP().toString().c_str());
      }
      if(!(connected%2)){
        addWebLog(true,"Ethernet Connected!");      
        connected++;
        if(connected==1)
          configureNetwork();
        if(connectionCallback)
          connectionCallback((connected+1)/2);
        resetStatus();     
      }
    break;

    case ARDUINO_EVENT_ETH_DISCONNECTED:
      if(connected%2){
        connected++;
        addWebLog(true,"*** Ethernet Connection Lost!");
        resetStatus();     
      }
    break;
                
    default:
      // LOG2("Event Callback: %s\n",Network.eventName(event.event_id));
    break;
  }
}

//////////////////////////////////////

IPAddress Span::getUniqueLocalIPv6(NetworkInterface &nif){

  esp_ip6_addr_t if_ip6[CONFIG_LWIP_IPV6_NUM_ADDRESSES];
  int v6addrs = esp_netif_get_all_ip6(nif.netif(), if_ip6);
  for(int i=0;i<v6addrs;i++){
    if(esp_netif_ip6_get_addr_type(&if_ip6[i])==ESP_IP6_ADDR_IS_UNIQUE_LOCAL)
    return(IPAddress(IPv6, (const uint8_t *)if_ip6[v6addrs-1].addr, if_ip6[v6addrs-1].zone));
  }
  return(IPAddress(IPv6));
}

//////////////////////////////////////


void Span::configureNetwork(){
   
  char id[18];                              // create string version of Accessory ID for MDNS broadcast
  memcpy(id,HAPClient::accessory.ID,17);    // copy ID bytes
  id[17]='\0';                              // add terminating null

  // create broadcast name from server base name plus accessory ID (without ':')
  
  if(!hostNameSuffix)
    asprintf(&hostName,"%s-%.2s%.2s%.2s%.2s%.2s%.2s",hostNameBase,id,id+3,id+6,id+9,id+12,id+15);
  else
    asprintf(&hostName,"%s%s",hostNameBase,hostNameSuffix);

  char *d;
  sscanf(hostName,"%m[A-Za-z0-9-]",&d);
  
  if(strlen(hostName)>255 || hostName[0]=='-' || hostName[strlen(hostName)-1]=='-' || strlen(hostName)!=strlen(d)){
    LOG0("\n*** Error:  Can't start MDNS due to invalid hostname '%s'.\n",hostName);
    LOG0("*** Hostname must consist of 255 or less alphanumeric characters or a hyphen, except that the hyphen cannot be the first or last character.\n");
    LOG0("*** PROGRAM HALTED!\n\n");
    while(1);
  }

  free(d);
    
  LOG0("\nStarting MDNS...\n\n");
  LOG0("HostName:      %s.local:%d\n",hostName,tcpPortNum);
  LOG0("Display Name:  %s\n",displayName);
  LOG0("Model Name:    %s\n",modelName);
  LOG0("Setup ID:      %s\n\n",qrID);
  
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
  
  memcpy(hashInput,qrID,4);                                           // Create the Setup ID for use with optional QR Codes.  This is an undocumented feature of HAP R2!
  memcpy(hashInput+4,id,17);                                          // Step 1: Concatenate 4-character Setup ID and 17-character Accessory ID into hashInput
  mbedtls_sha512(hashInput,21,hashOutput,0);                          // Step 2: Perform SHA-512 hash on combined 21-byte hashInput to create 64-byte hashOutput
  mbedtls_base64_encode((uint8_t *)setupHash,9,&len,hashOutput,4);    // Step 3: Encode the first 4 bytes of hashOutput in base64, which results in an 8-character, null-terminated, setupHash
  mdns_service_txt_item_set("_hap","_tcp","sh",setupHash);            // Step 4: broadcast the resulting Setup Hash

  if(spanOTA.enabled){
    ArduinoOTA.setHostname(hostName);
    if(spanOTA.auth)
      ArduinoOTA.setPasswordHash(spanOTA.otaPwd);

    ArduinoOTA.onStart(spanOTA.start).onEnd(spanOTA.end).onProgress(spanOTA.progress).onError(spanOTA.error);  
    ArduinoOTA.begin();

    LOG0("Starting OTA Server:    %s\n",displayName);
    LOG0("Authorization Password: ");
    if(spanOTA.auth)
      LOG0("Enabled with %s Hash = %s\n",strlen(spanOTA.otaPwd)==32 ? "MD5" : "SHA256", spanOTA.otaPwd);
    else
      LOG0("DISABLED!\n");    
    LOG0("Auto Rollback:          %s",verifyRollbackLater()?"Enabled\n\n":"Disabled\n\n");
  }
  
  mdns_service_txt_item_set("_hap","_tcp","ota",spanOTA.enabled?"yes":"no");                // OTA status (info only - NOT used by HAP)

  if(webLog.isEnabled){
    mdns_service_txt_item_set("_hap","_tcp","logURL",webLog.statusURL);                     // Web Log status (info only - NOT used by HAP)
    
    LOG0("Web Logging enabled at http://%s.local:%d%s with max number of entries=%d\n\n",hostName,tcpPortNum,webLog.statusURL,webLog.maxEntries);
  }

  if(webLog.timeServer)
    xTaskCreateUniversal(webLog.initTime, "timeSeverTaskHandle", 8096, &webLog, 1, NULL, 0);  
  
  LOG0("Starting HAP Server on port %d...\n\n",tcpPortNum);

  hapServer->begin();

  LOG0("\n");

  if(!HAPClient::nAdminControllers())
    LOG0("DEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
  
  if(wifiCallback)
    wifiCallback();
  
} // initWiFi

///////////////////////////////

Span& Span::setQRID(const char *id){
  
  char tBuf[5];
  sscanf(id,"%4[0-9A-Za-z]",tBuf);
  
  if(strlen(id)==4 && strlen(tBuf)==4){
    sprintf(qrID,"%s",id);
  }

  return(*this);
} // setQRID

///////////////////////////////

void Span::processSerialCommand(const char *c){

  switch(c[0]){

    case '-': {
      LOG0("Please type '?' for a list of commands\n\n");
    }
    break;

    case 'c': {
      LOG0("Message Logs:     Level %d",logLevel);
      LOG0("\nStatus LED:       Pin ");
      if(getStatusPin()>=0){
        LOG0(getStatusPin());
        if(autoOffLED>0)
          LOG0("  (Auto Off=%d sec)",autoOffLED);
      }
      else
        LOG0("-  *** WARNING: Status LED Pin is UNDEFINED");
      LOG0("\nDevice Control:   Pin ");
      if(getControlPin()>=0){
        LOG0(getControlPin());
      }
      else{
        LOG0("-  *** WARNING: Device Control Pin is UNDEFINED");
      }
      LOG0("\nSketch Version:   %s",getSketchVersion());  
      LOG0("\nHomeSpan Version: %s",HOMESPAN_VERSION);
      LOG0("\nArduino-ESP Ver.: %s",ARDUINO_ESP_VERSION);
      LOG0("\nESP-IDF Version:  %d.%d.%d",ESP_IDF_VERSION_MAJOR,ESP_IDF_VERSION_MINOR,ESP_IDF_VERSION_PATCH);
      LOG0("\nESP32 Chip:       %s Rev %d %s-core %luMB Flash", ESP.getChipModel(),ESP.getChipRevision(),
                    ESP.getChipCores()==1?"single":"dual",ESP.getFlashChipSize()/1024/1024);
      LOG0("\nESP32 Board:      %s",ARDUINO_BOARD);      
      LOG0("\nPWM Resources:    %d channels, %d timers, max %d-bit duty resolution",
                    (int)LEDC_SPEED_MODE_MAX*(int)LEDC_CHANNEL_MAX,(int)LEDC_SPEED_MODE_MAX*(int)LEDC_TIMER_MAX,LEDC_TIMER_BIT_MAX-1);
      LOG0("\nRMT Resources:    %d transmission channels of %d symbols each",SOC_RMT_TX_CANDIDATES_PER_GROUP,SOC_RMT_MEM_WORDS_PER_CHANNEL);
      
      #ifdef SOC_TOUCH_SENSOR_NUM
        LOG0("\nTouch Sensors:    %d pins",SOC_TOUCH_SENSOR_NUM);
      #else
        LOG0("\nTouch Sensors:    none");
      #endif

      LOG0("\nSodium Version:   %s  Lib %d.%d",sodium_version_string(),sodium_library_version_major(),sodium_library_version_minor());
      char mbtlsv[64];
      mbedtls_version_get_string_full(mbtlsv);
      LOG0("\nMbedTLS Version:  %s",mbtlsv);

      LOG0("\nSketch Compiled:  %s",compileTime?compileTime:"N/A");
      LOG0("\nPartition:        %s",esp_ota_get_running_partition()->label);
      if(hsWDT.getSeconds())
        LOG0("\nHS Watchdog:      %d seconds",hsWDT.getSeconds());
      else
        LOG0("\nHS Watchdog:      DISABLED");

      for(int i=0;i<CONFIG_FREERTOS_NUMBER_OF_CORES;i++)
        LOG0("\nIDLE-%d Watchdog:  %s",i,(ESP_OK==esp_task_wdt_status(xTaskGetIdleTaskHandleForCore(i)))?"ENABLED":"DISABLED");

      LOG0("\nReset Reason:     %s",Utils::resetReason());
      LOG0("\nMAC Address:      %s",Network.macAddress().c_str());
      LOG0("\nInterface:        %s",ethernetEnabled?"ETHERNET":"WIFI");
      LOG0("\nDevice Name:      %s\n\n",displayName);
    }
    break;
    
    case 'p': {
      LOG0("\n  Partition           Address       Size   OTA State");
      LOG0("\n  ----------------   --------   --------   --------- \n");
      auto it=esp_partition_find(ESP_PARTITION_TYPE_ANY,ESP_PARTITION_SUBTYPE_ANY,NULL);
      uint32_t totalSize=0;
      uint32_t flashSize;
      while(it){
        const esp_partition_t *part=esp_partition_get(it);
        if(totalSize==0){
          totalSize=part->address;      // add in offset of first partition
          esp_flash_get_physical_size(part->flash_chip,&flashSize);
        }
        totalSize+=part->size;
        LOG0("%1.1s %-16.16s   0x%06lX   %8lu   ",esp_ota_get_running_partition()==part?"*":" ",part->label,part->address,part->size);
        esp_ota_img_states_t state;
        if(ESP_OK==esp_ota_get_state_partition(part,&state)){
          switch(state){
            case ESP_OTA_IMG_VALID:           LOG0("    VALID"); break;
            case ESP_OTA_IMG_UNDEFINED:       LOG0("UNDEFINED"); break;
            case ESP_OTA_IMG_INVALID:         LOG0("  INVALID"); break;
            case ESP_OTA_IMG_ABORTED:         LOG0("  ABORTED"); break;
            case ESP_OTA_IMG_PENDING_VERIFY:  LOG0("  PENDING"); break;
            default:                          LOG0("      ???");
          }
        }
        LOG0("\n");
        it=esp_partition_next(it);
      }
      esp_partition_iterator_release(it);
      LOG0("\nTotal Size Allocated (including any initial offset): %lu of %lu available\n\n",totalSize,flashSize);
    }
    break;

    case 'D': {
      WiFi.disconnect();
    }
    break;
    
    case 'Z': {
      LOG0("Scanning WiFi Networks...\n");
      WiFi.scanDelete();
      STATUS_UPDATE(start(LED_WIFI_SCANNING),HS_WIFI_SCANNING)
      int n=WiFi.scanNetworks();
      if(n==0){
        LOG0("No networks found!\n");
      } else {
        char d[]="----------------------------------------";
        LOG0("\n%-32.32s  %17.17s  %4.4s  %12.12s\n","SSID","BSSID","RSSI","ENCRYPTION");
        LOG0("%-32.32s  %17.17s  %4.4s  %12.12s\n",d,d,d,d);
        for(int i=0;i<n;i++){
          LOG0("%-32.32s  %17.17s  %4ld  ",WiFi.SSID(i).c_str(),WiFi.BSSIDstr(i).c_str(),WiFi.RSSI(i));
          switch(WiFi.encryptionType(i)){
            case WIFI_AUTH_OPEN:            LOG0("%12.12s","OPEN"); break;
            case WIFI_AUTH_WEP:             LOG0("%12.12s","WEP"); break;
            case WIFI_AUTH_WPA_PSK:         LOG0("%12.12s","WPA"); break;
            case WIFI_AUTH_WPA2_PSK:        LOG0("%12.12s","WPA2"); break;
            case WIFI_AUTH_WPA_WPA2_PSK:    LOG0("%12.12s","WPA+WPA2"); break;
            case WIFI_AUTH_WPA2_ENTERPRISE: LOG0("%12.12s","WPA2-EAP"); break;
            case WIFI_AUTH_WPA3_PSK:        LOG0("%12.12s","WPA3"); break;
            case WIFI_AUTH_WPA2_WPA3_PSK:   LOG0("%12.12s","WPA2+WPA3"); break;
            case WIFI_AUTH_WAPI_PSK:        LOG0("%12.12s","WAPI"); break;
            default:                        LOG0("%12.12s","UNKNOWN");
          }
          if(bssidNames.count(WiFi.BSSIDstr(i).c_str()))
            LOG0("   \"%s\"",bssidNames[WiFi.BSSIDstr(i).c_str()].c_str());
          LOG0("\n");
        }
        LOG0("\n");
      }
    }
    break;

    case 's': {    
      
      LOG0("\n*** HomeSpan Status ***\n\n");

      if(!ethernetEnabled){
        LOG0("IP Addresses:      IPv4 = %s  IPv6 = %s  (Band: %s  RSSI=%d  BSSID=%s",WiFi.localIP().toString().c_str(),getUniqueLocalIPv6(WiFi).toString().c_str(),WiFi.getBand()==1?"2.4 GHz":"5.0 GHz",WiFi.RSSI(),WiFi.BSSIDstr().c_str());
        if(bssidNames.count(WiFi.BSSIDstr().c_str()))
          LOG0("  \"%s\"",bssidNames[WiFi.BSSIDstr().c_str()].c_str());
        LOG0(")\n");
      } else {
        LOG0("IP Addresses:      IPv4 = %s  IPv6 = %s  (Ethernet)\n",ETH.localIP().toString().c_str(),getUniqueLocalIPv6(ETH).toString().c_str());        
      }
      
      if(webLog.isEnabled && hostName!=NULL)   
        LOG0("Web Logging:       http://%s.local:%d%s\n",hostName,tcpPortNum,webLog.statusURL);
      LOG0("\nAccessory ID:      ");
      HAPClient::charPrintRow(HAPClient::accessory.ID,17);
      LOG0("                               LTPK: ");
      HAPClient::hexPrintRow(HAPClient::accessory.LTPK,32);
      LOG0("\n");

      HAPClient::printControllers();
      LOG0("\n");

      for(auto it=hapList.begin(); it!=hapList.end(); ++it){
        LOG0("Client #%d: %s",(*it).clientNumber,(*it).client.remoteIP().toString().c_str());
        if((*it).cPair){
          LOG0("  ID=");
          HAPClient::charPrintRow((*it).cPair->getID(),36);
          LOG0((*it).cPair->isAdmin()?"   (admin)\n":" (regular)\n");
        } else {
          LOG0("  (unverified)\n");
        }
      }          

      if(hapList.empty())
        LOG0("No Client Connections!\n");
        
      LOG0("\n*** End Status ***\n\n");
    } 
    break;

    case 'd': {            

      LOG0("\n*** Attributes Database ***\n\n");
      hapOut.prettyPrint();
      printfAttributes();
      size_t nBytes=hapOut.getSize();
      hapOut.flush();
      LOG0("\n\n*** End Database: size=%d  configuration=%d ***\n\n",nBytes,hapConfig.configNumber);      
    }
    break;

    case 'Q': {
      
      char tBuf[5];
      const char *s=c+1+strspn(c+1," ");
      sscanf(s," %4[0-9A-Za-z]",tBuf);
  
      if(strlen(s)==4 && strlen(tBuf)==4){
        sprintf(qrID,"%s",tBuf);
        LOG0("\nChanging default Setup ID for QR Code to: '%s'.  Will take effect after next restart.\n\n",qrID);
        nvs_set_str(hapNVS,"SETUPID",qrID);
        nvs_commit(hapNVS);          
      } else {
        LOG0("\n*** Invalid request to change Setup ID for QR Code to: '%s'.  Setup ID must be exactly 4 alphanumeric characters (0-9, A-Z, and a-z).\n\n",s);
      }        
    }
    break;
    
    case 'O': {

      char textPwd[68]="\0";
      
      LOG0("\n>>> New OTA Password, or <return> to cancel request: ");
      readSerial(textPwd,67);
      
      if(strlen(textPwd)==0){
        LOG0("(cancelled)\n\n");
        return;
      }

      if(spanOTA.setPassword(textPwd)==-1)
        return;

      if(strlen(textPwd)<=32)
        LOG0("%s\n",textPwd);
      else
        LOG0("(accepted as valid hash)\n");
      LOG0(">>> Hash stored in NVS as: %s\n",spanOTA.otaPwd);

      nvs_set_str(otaNVS,"OTADATA",spanOTA.otaPwd);                 // update data
      nvs_commit(otaNVS);          
      
      if(!spanOTA.enabled)
        LOG0("... Note: OTA has not been enabled in this sketch.\n");
      LOG0("\n");
    }
    break;

    case 'S': {

      setPairingCode(c+1,false);          
    }
    break;

    case 'U': {

      HAPClient::controllerList.clear();                                        // clear all Controller data  
      HAPClient::saveControllers();
      LOG0("\n*** HomeSpan Pairing Data DELETED ***\n\n");
      HAPClient::tearDown(NULL);                                                // tear down all verified connections
            
      LOG0("\nDEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
      mdns_service_txt_item_set("_hap","_tcp","sf","1");                        // set Status Flag = 1 (Table 6-8)

      if(homeSpan.pairCallback)
        homeSpan.pairCallback(false);

      resetStatus();      
    }
    break;

    case 'W': {

      if(serialInputDisabled || logLevel<0)       // do not proceed if serial input/output is not fully enabled
        return;

      if(strlen(network.wifiData.ssid)>0){
        LOG0("*** Stopping all current WiFi services...\n\n");
        hapServer->end();
        MDNS.end();
        WiFi.disconnect();
        delay(1000);
      }

      network.serialConfigure();
      nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
      nvs_commit(wifiNVS);                                                            // commit to NVS
      LOG0("\n*** WiFi Credentials SAVED!  Restarting ***\n\n");
      reboot();  
      }
    break;

    case 'A': {

      if(strlen(network.wifiData.ssid)>0){
        LOG0("*** Stopping all current WiFi services...\n\n");
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
      LOG0("\n*** Credentials saved!\n");
      if(strlen(network.setupCode))
        setPairingCode(network.setupCode,false);
      else
        LOG0("*** Setup Code Unchanged\n");
            
      LOG0("\n*** Restarting...\n\n");
      STATUS_UPDATE(start(LED_ALERT),HS_AP_TERMINATED)
      reboot();
    }
    break;
    
    case 'X': {
     
      nvs_erase_all(wifiNVS);
      nvs_commit(wifiNVS);
      WiFi.begin("none");     
      LOG0("\n*** WiFi Credentials ERASED!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'V': {
      
      nvs_erase_all(charNVS);
      nvs_commit(charNVS);      
      LOG0("\n*** Values for all saved Characteristics erased!\n\n");
    }
    break;

    case 'H': {
      
      nvs_erase_all(hapNVS);
      nvs_commit(hapNVS);      
      LOG0("\n*** HomeSpan Device ID and Pairing Data DELETED!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'R': {

      reboot();      
    }
    break;

    case 'F': {
      
      nvs_erase_all(hapNVS);
      nvs_commit(hapNVS);      
      nvs_erase_all(wifiNVS);
      nvs_commit(wifiNVS);   
      nvs_erase_all(charNVS);
      nvs_commit(charNVS);
      nvs_erase_all(otaNVS);
      nvs_commit(otaNVS);
      WiFi.begin("none");  
      LOG0("\n*** FACTORY RESET!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'E': {
      
      nvs_flash_erase();
      LOG0("\n*** ALL DATA ERASED!  Restarting...\n\n");
      reboot();
    }
    break;

    case 'L': {

      int level=0;
      sscanf(c+1,"%d",&level);
      
      if(level<-1)
        level=-1;
      if(level>2)
        level=2;

      setLogLevel(level);     
      LOG0("\n*** Log Level set to %d\n\n",level);
    }
    break;

    case 'm': {
      multi_heap_info_t heapAll;
      multi_heap_info_t heapInternal;
      multi_heap_info_t heapPSRAM;
    
      heap_caps_get_info(&heapAll,MALLOC_CAP_DEFAULT);
      heap_caps_get_info(&heapInternal,MALLOC_CAP_DEFAULT|MALLOC_CAP_INTERNAL);
      heap_caps_get_info(&heapPSRAM,MALLOC_CAP_DEFAULT|MALLOC_CAP_SPIRAM);
    
      Serial.printf("\n            Allocated      Free   Largest       Low\n");
      Serial.printf("            --------- --------- --------- ---------\n");
      Serial.printf("Total Heap: %9d %9d %9d %9d\n",heapAll.total_allocated_bytes,heapAll.total_free_bytes,heapAll.largest_free_block,heapAll.minimum_free_bytes);
      Serial.printf("  Internal: %9d %9d %9d %9d\n",heapInternal.total_allocated_bytes,heapInternal.total_free_bytes,heapInternal.largest_free_block,heapInternal.minimum_free_bytes);
      Serial.printf("     PSRAM: %9d %9d %9d %9d\n\n",heapPSRAM.total_allocated_bytes,heapPSRAM.total_free_bytes,heapPSRAM.largest_free_block,heapPSRAM.minimum_free_bytes);
      
      if(getAutoPollTask())
        LOG0("Lowest stack level: %d bytes (%s)\n",uxTaskGetStackHighWaterMark(getAutoPollTask()),pcTaskGetName(getAutoPollTask()));
      LOG0("Lowest stack level: %d bytes (%s)\n",uxTaskGetStackHighWaterMark(loopTaskHandle),pcTaskGetName(loopTaskHandle));
      nvs_stats_t nvs_stats;
      nvs_get_stats(NULL, &nvs_stats);
      LOG0("NVS Flash Partition: %d of %d records used\n\n",nvs_stats.used_entries,nvs_stats.total_entries-126);      
    }
    break;       

    case 'i':{

      LOG0("\n*** HomeSpan Info ***\n\n");

      int nErrors=0;
      int nWarnings=0;
      
      vector<uint32_t, Mallocator<uint32_t>> aidValues;
      char pNames[][7]={"PR","PW","EV","AA","TW","HD","WR"};

      for(auto acc=Accessories.begin(); acc!=Accessories.end(); acc++){
        LOG0("\u27a4 Accessory:  AID=%lu\n",(*acc)->aid);
        boolean foundInfo=false;

        if(acc==Accessories.begin() && (*acc)->aid!=1)
          LOG0("   *** ERROR #%d!  AID of first Accessory must always be 1 ***\n",++nErrors);

        if(std::find(aidValues.begin(),aidValues.end(),(*acc)->aid)!=aidValues.end())
          LOG0("   *** ERROR #%d!  AID already in use for another Accessory ***\n",++nErrors);
        
        aidValues.push_back((*acc)->aid);
        vector<uint32_t, Mallocator<uint32_t>> iidValues;

        for(auto svc=(*acc)->Services.begin(); svc!=(*acc)->Services.end(); svc++){
          LOG0("   \u279f Service %s:  IID=%lu, %sUUID=\"%s\"\n",(*svc)->hapName,(*svc)->iid,(*svc)->isCustom?"Custom-":"",(*svc)->type);

          if(invalidUUID((*svc)->type))
            LOG0("     *** ERROR #%d!  Format of UUID is invalid ***\n",++nErrors);         

          if(!strcmp((*svc)->type,"3E")){
            foundInfo=true;
            if((*svc)->iid!=1)
              LOG0("     *** ERROR #%d!  The Accessory Information Service must be defined with IID=1 (i.e. before any other Services in an Accessory) ***\n",++nErrors);
          }
          else if((*acc)->aid==1)            // this is an Accessory with aid=1, but it has more than just AccessoryInfo.  So...
            isBridge=false;                  // ...this is not a bridge device

          if(std::find(iidValues.begin(),iidValues.end(),(*svc)->iid)!=iidValues.end())
            LOG0("     *** ERROR #%d!  IID already in use for another Service or Characteristic within this Accessory ***\n",++nErrors);

          iidValues.push_back((*svc)->iid);

          for(auto chr=(*svc)->Characteristics.begin(); chr!=(*svc)->Characteristics.end(); chr++){
            LOG0("      \u21e8 Characteristic %s(%.33s%s):  IID=%lu, %sUUID=\"%s\", %sPerms=",
              (*chr)->hapName,(*chr)->uvPrint((*chr)->value).c_str(),strlen((*chr)->uvPrint((*chr)->value).c_str())>33?"...\"":"",(*chr)->iid,(*chr)->isCustom?"Custom-":"",(*chr)->type,(*chr)->perms!=(*chr)->hapChar->perms?"Custom-":"");

            int foundPerms=0;
            for(uint8_t i=0;i<7;i++){
              if((*chr)->perms & (1<<i))
                LOG0("%s%s",(foundPerms++)?"+":"",pNames[i]);
            }           
            
            if((*chr)->format<FORMAT::STRING && (*chr)->format!=FORMAT::BOOL){
              if((*chr)->validValues)
                LOG0(", Valid Values=%s",(*chr)->validValues);
              else if((*chr)->uvGet<double>((*chr)->stepValue)>0)
                LOG0(", %sRange=[%s,%s,%s]",(*chr)->customRange?"Custom-":"",(*chr)->uvPrint((*chr)->minValue).c_str(),(*chr)->uvPrint((*chr)->maxValue).c_str(),(*chr)->uvPrint((*chr)->stepValue).c_str());
              else
                LOG0(", %sRange=[%s,%s]",(*chr)->customRange?"Custom-":"",(*chr)->uvPrint((*chr)->minValue).c_str(),(*chr)->uvPrint((*chr)->maxValue).c_str());
            }

            if(((*chr)->perms)&EV){
              LOG0(", EV=(");
              boolean addComma=false;
              for(auto const &hc : (*chr)->evList){
                LOG0("%s%d",addComma?",":"",hc->clientNumber);
                addComma=true;
              }
              LOG0(")");              
            }
            
            if((*chr)->nvsKey)
              LOG0(" (nvs)");
              
            LOG0("\n");        
            
            if(!(*chr)->isCustom && !(*svc)->isCustom  && std::find((*svc)->req.begin(),(*svc)->req.end(),(*chr)->hapChar)==(*svc)->req.end() && std::find((*svc)->opt.begin(),(*svc)->opt.end(),(*chr)->hapChar)==(*svc)->opt.end())
              LOG0("          *** WARNING #%d!  Service does not support this Characteristic ***\n",++nWarnings);
            else
            if(invalidUUID((*chr)->type))
              LOG0("          *** ERROR #%d!  Format of UUID is invalid ***\n",++nErrors);
            else       
              if(std::find_if((*svc)->Characteristics.begin(),chr,[chr](SpanCharacteristic *c)->boolean{return(c->hapChar==(*chr)->hapChar);})!=chr)
              LOG0("          *** ERROR #%d!  Characteristic already defined for this Service ***\n",++nErrors);

            if((*chr)->setRangeError)
              LOG0("          *** WARNING #%d!  Attempt to set Custom Range for this Characteristic ignored ***\n",++nWarnings);

            if((*chr)->setValidValuesError)
              LOG0("          *** WARNING #%d!  Attempt to set Custom Valid Values for this Characteristic ignored ***\n",++nWarnings);

            if((*chr)->format<STRING && (!(((*chr)->uvGet<double>((*chr)->value) >= (*chr)->uvGet<double>((*chr)->minValue)) && ((*chr)->uvGet<double>((*chr)->value) <= (*chr)->uvGet<double>((*chr)->maxValue)))))
              LOG0("          *** WARNING #%d!  Value of %g is out of range [%g,%g] ***\n",++nWarnings,(*chr)->uvGet<double>((*chr)->value),(*chr)->uvGet<double>((*chr)->minValue),(*chr)->uvGet<double>((*chr)->maxValue));

            if(std::find(iidValues.begin(),iidValues.end(),(*chr)->iid)!=iidValues.end())
              LOG0("   *** ERROR #%d!  IID already in use for another Service or Characteristic within this Accessory ***\n",++nErrors);

            iidValues.push_back((*chr)->iid);
          
          } // Characteristics

          for(auto req=(*svc)->req.begin(); req!=(*svc)->req.end(); req++){
            if(std::find_if((*svc)->Characteristics.begin(),(*svc)->Characteristics.end(),[req](SpanCharacteristic *c)->boolean{return(c->hapChar==*req);})==(*svc)->Characteristics.end())
              LOG0("          *** WARNING #%d!  Required '%s' Characteristic for this Service not found ***\n",++nWarnings,(*req)->hapName);
          }

          for(auto button=PushButtons.begin(); button!=PushButtons.end(); button++){
            if((*button)->service==(*svc)){
              
              if((*button)->buttonType==SpanButton::HS_BUTTON)
                LOG0("      \u25bc SpanButton: Pin=%d, Single=%ums, Double=%ums, Long=%ums, Type=",(*button)->pin,(*button)->singleTime,(*button)->doubleTime,(*button)->longTime);
              else
                LOG0("      \u25bc SpanToggle: Pin=%d, Toggle=%ums, Type=",(*button)->pin,(*button)->longTime);
                
              if((*button)->triggerType==PushButton::TRIGGER_ON_LOW)
                LOG0("TRIGGER_ON_LOW\n");
              else if((*button)->triggerType==PushButton::TRIGGER_ON_HIGH)
                LOG0("TRIGGER_ON_HIGH\n");

#if SOC_TOUCH_SENSOR_NUM > 0
              else if((*button)->triggerType==PushButton::TRIGGER_ON_TOUCH)
                LOG0("TRIGGER_ON_TOUCH\n");
#endif
              else
                LOG0("USER-DEFINED\n");
              
              if((void(*)(int,int))((*svc)->*(&SpanService::button))==(void(*)(int,int))(&SpanService::button))
                LOG0("          *** WARNING #%d!  No button() method defined in this Service ***\n",++nWarnings);
            }
          }
          
        } // Services
        
        if(!foundInfo)
          LOG0("   *** ERROR #%d!  Required 'AccessoryInformation' Service not found ***\n",++nErrors);
          
      } // Accessories   

      char d[]="------------------------------";
      LOG0("\n%-30s  %8s  %10s  %s  %s  %s  %s  %s\n","Service","UUID","AID","IID","Update","Loop","Button","Linked Services");
      LOG0("%.30s  %.8s  %.10s  %.3s  %.6s  %.4s  %.6s  %.15s\n",d,d,d,d,d,d,d,d);
      for(int i=0;i<Accessories.size();i++){                             // identify all services with over-ridden loop() methods
        for(int j=0;j<Accessories[i]->Services.size();j++){
          SpanService *s=Accessories[i]->Services[j];
          LOG0("%-30s  %8.8s  %10lu  %3lu  %6s  %4s  %6s  ",s->hapName,s->type,Accessories[i]->aid,s->iid, 
                 (void(*)())(s->*(&SpanService::update))!=(void(*)())(&SpanService::update)?"YES":"NO",
                 (void(*)())(s->*(&SpanService::loop))!=(void(*)())(&SpanService::loop)?"YES":"NO",
                 (void(*)(int,boolean))(s->*(&SpanService::button))!=(void(*)(int,boolean))(&SpanService::button)?"YES":"NO"
                 );
          if(s->linkedServices.empty())
            LOG0("-");
          for(int k=0;k<s->linkedServices.size();k++){
            LOG0("%lu",s->linkedServices[k]->iid);
            if(k<s->linkedServices.size()-1)
              LOG0(",");
          }
          LOG0("\n");
        }
      }

      if(SpanPoint::SpanPoints.size()>0){
        uint8_t channel;
        wifi_second_chan_t channel2; 
        esp_wifi_get_channel(&channel,&channel2);
        LOG0("\nFound %d %s SpanPoint Links:\n\n",SpanPoint::SpanPoints.size(),SpanPoint::useEncryption?"encrypted":"unencrypted");
        LOG0("%-17s  %18s  %7s  %7s  %7s\n","Local MAC Address","Remote MAC Address","Send","Receive","Depth"); 
        LOG0("%.17s  %.18s  %.7s  %.7s  %.7s\n",d,d,d,d,d);
        for(auto it=SpanPoint::SpanPoints.begin();it!=SpanPoint::SpanPoints.end();it++)
          LOG0("%-18s  %02X:%02X:%02X:%02X:%02X:%02X  %7d  %7d  %7d  %s\n",(*it)->peerInfo.ifidx==WIFI_IF_AP?WiFi.softAPmacAddress().c_str():WiFi.macAddress().c_str(),
                 (*it)->peerInfo.peer_addr[0],(*it)->peerInfo.peer_addr[1],(*it)->peerInfo.peer_addr[2],(*it)->peerInfo.peer_addr[3],(*it)->peerInfo.peer_addr[4],(*it)->peerInfo.peer_addr[5],
                 (*it)->sendSize,(*it)->receiveSize,(*it)->receiveSize?uxQueueSpacesAvailable((*it)->receiveQueue):0,esp_now_is_peer_exist((*it)->peerInfo.peer_addr)?"":"(max connections exceeded!)");           
        LOG0("\nSpanPoint using WiFi Channel %d%s\n",channel,WiFi.status()!=WL_CONNECTED?" (subject to change once WiFi connection established)":"");
      }

      LOG0("\nConfigured as Bridge: %s\n",isBridge?"YES":"NO");
      if(!isBridge && Accessories.size()>3)
        LOG0("*** WARNING #%d!  HomeKit requires the device be configured as a Bridge when more than 3 Accessories are defined ***\n",++nWarnings);
      
      if(hapConfig.configNumber>0)
        LOG0("Configuration Number: %d\n",hapConfig.configNumber);
      LOG0("\nDatabase Validation:  Warnings=%d, Errors=%d\n",nWarnings,nErrors);      
      LOG0("\n*** End Info ***\n\n");
    }
    
    break;

    case 'P': {
      
      LOG0("\n*** Pairing Data used for Cloning another Device\n\n");
      size_t olen;
      TempBuffer<char> tBuf(256);
      mbedtls_base64_encode((uint8_t *)tBuf.get(),256,&olen,(uint8_t *)&HAPClient::accessory,sizeof(struct Accessory));
      LOG0("Accessory data:  %s\n",tBuf.get());
      for(const auto &cont : HAPClient::controllerList){
        mbedtls_base64_encode((uint8_t *)tBuf.get(),256,&olen,(uint8_t *)(&cont),sizeof(struct Controller));
        LOG0("Controller data: %s\n",tBuf.get());        
      }
      LOG0("\n*** End Pairing Data\n\n");
    }
    break;

    case 'C': {

      LOG0("\n*** Clone Pairing Data from another Device\n\n");
      TempBuffer<char> tBuf(200);
      size_t olen;

      tBuf[0]='\0';
      LOG0(">>> Accessory data:  ");
      readSerial(tBuf,199);
      if(strlen(tBuf)==0){
        LOG0("(cancelled)\n\n");
        return;
      }
      mbedtls_base64_decode((uint8_t *)&HAPClient::accessory,sizeof(struct Accessory),&olen,(uint8_t *)tBuf.get(),strlen(tBuf));
      if(olen!=sizeof(struct Accessory)){
        LOG0("\n*** Error in size of Accessory data - cloning cancelled.  Restarting...\n\n");
        reboot();
      } else {
        HAPClient::charPrintRow(HAPClient::accessory.ID,17);
        LOG0("\n");
      }

      HAPClient::controllerList.clear();
      Controller tCont;
      
      while(HAPClient::controllerList.size()<16){
        tBuf[0]='\0';
        LOG0(">>> Controller data: ");
        readSerial(tBuf,199);
        if(strlen(tBuf)==0){
          LOG0("(done)\n");
          break;
        } else {
          mbedtls_base64_decode((uint8_t *)(&tCont),sizeof(struct Controller),&olen,(uint8_t *)tBuf.get(),strlen(tBuf));
          if(olen!=sizeof(struct Controller)){
            LOG0("\n*** Error in size of Controller data - cloning cancelled.  Restarting...\n\n");
            reboot();
          } else {
            HAPClient::controllerList.push_back(tCont);
            HAPClient::charPrintRow(tCont.getID(),36);
            LOG0("\n");
          }
        }
      }

      char qSave[2];
      while(1){
        qSave[0]='-';
        LOG0("Save Cloned Pairing Data (y/n): ");
        readSerial(qSave,1);
        if(qSave[0]=='y'){
          LOG0("(yes)\nData saved!  Rebooting...");
          nvs_set_blob(hapNVS,"ACCESSORY",&HAPClient::accessory,sizeof(HAPClient::accessory));           // update data (commit is included in saveControllers below)
          HAPClient::saveControllers();
          reboot();
        } else
        if(qSave[0]=='n'){
          LOG0("(no)\nProcess Cancelled!  Rebooting...");
          reboot();
        }
        LOG0("\n");
      }
    }
    break;   

    case '?': {    
      
      LOG0("\n*** HomeSpan Commands ***\n\n");
      LOG0("  c - print configuration info\n");
      LOG0("  s - print connection status\n");
      LOG0("  i - print summary information about the HAP Database\n");
      LOG0("  d - print the full HAP Accessory Attributes Database in JSON format\n");
      LOG0("  m - print free heap memory\n");
      LOG0("  p - print flash partition table\n");
      LOG0("\n");      
      LOG0("  W - configure WiFi Credentials and restart\n");      
      LOG0("  X - delete WiFi Credentials and restart\n");      
      LOG0("  S <code> - change the HomeKit Pairing Setup Code to <code>\n");
      LOG0("  Q <id> - change the HomeKit Setup ID for QR Codes to <id>\n");
      LOG0("  O - change the OTA password\n");
      LOG0("  A - start the HomeSpan Setup Access Point\n");
      LOG0("\n");      
      LOG0("  V - delete value settings for all saved Characteristics\n");
      LOG0("  U - unpair device by deleting all Controller data\n");
      LOG0("  H - delete HomeKit Device ID as well as all Controller data and restart\n");      
      LOG0("\n");      
      LOG0("  P - output Pairing Data that can be saved offline to clone a new device\n");      
      LOG0("  C - clone Pairing Data previously saved offline from another device\n");      
      LOG0("\n");      
      LOG0("  D - disconnect/reconnect to WiFi\n");
      LOG0("  Z - scan for available WiFi networks\n");
      LOG0("\n");      
      LOG0("  R - restart device\n");      
      LOG0("  F - factory reset and restart\n");      
      LOG0("  E - erase ALL stored data and restart\n");      
      LOG0("\n");          
      LOG0("  L <level> - change the Log Level setting to <level>\n");
      LOG0("\n");

      for(auto uCom=homeSpan.UserCommands.begin(); uCom!=homeSpan.UserCommands.end(); uCom++)      // loop over all UserCommands using an iterator
        LOG0("  @%c %s\n",uCom->first,uCom->second->s);

      if(!homeSpan.UserCommands.empty())
        LOG0("\n");
        
      LOG0("  ? - print this list of commands\n\n");     
      LOG0("*** End Commands ***\n\n");
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
        LOG0("*** Undefined user command: '%s'.  Type '?' for list of commands.\n\n",c);
      }
    }
    break;

    default:
      LOG0("*** Unknown command: '%s'.  Type '?' for list of commands.\n\n",c);
    break;
    
  } // switch
}

///////////////////////////////

void Span::getWebLog(void (*f)(const char *, void *), void *user_data){
  HAPClient::getStatusURL(NULL,f,user_data);
}

///////////////////////////////

void Span::resetStatus(){
  if(!ethernetEnabled && strlen(network.wifiData.ssid)==0)
    STATUS_UPDATE(start(LED_WIFI_NEEDED),HS_WIFI_NEEDED)
  else if(!(connected%2))
    STATUS_UPDATE(start(LED_WIFI_CONNECTING),ethernetEnabled?HS_ETH_CONNECTING:HS_WIFI_CONNECTING)
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
    case HS_ETH_CONNECTING: return("Ethernet Connecting");
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
    case HS_WIFI_SCANNING: return("WiFi Scanning Started");
    default: return("Unknown");
  }
}

///////////////////////////////

Span& Span::setPairingCode(const char *s, boolean progCall){
   
  char setupCode[10];

  sscanf(s," %9[0-9]",setupCode);

  if(strlen(setupCode)!=8){
    LOG0("\n*** Invalid request to change Setup Code to '%s'.  Code must be exactly 8 digits.\n\n",s);
    if(progCall){
      LOG0("=== PROGRAM HALTED ===");
      while(1);
    }
    return(*this);
  }   

  if(!network.allowedCode(setupCode)){
    LOG0("\n*** Invalid request to change Setup Code to '%s'.  Code too simple.\n\n",s);
    if(progCall){
      LOG0("=== PROGRAM HALTED ===");
      while(1);
    }
    return(*this);
  }

  TempBuffer<Verification> verifyData;       // temporary storage for verification data
  SRP6A *srp=new SRP6A;                      // create temporary instance of SRP

  if(!progCall)
    LOG0("\nGenerating new SRP verification data for Setup Code: %.3s-%.2s-%.3s ... ",setupCode,setupCode+3,setupCode+5);

  srp->createVerifyCode(setupCode,verifyData);                                       // create random salt and compute verification code from specified Setup Code
  nvs_set_blob(srpNVS,"VERIFYDATA",verifyData,verifyData.len());                     // update data
  nvs_commit(srpNVS);                                                                // commit to NVS
  
  if(!progCall)
    LOG0("New Code Saved!\nSetup Payload for Optional QR Code: %s\n\n",qrCode.get(atoi(setupCode),qrID,atoi(category)));

  delete srp;
  return(*this);
}

///////////////////////////////

Span& Span::setWifiCredentials(const char *ssid, const char *pwd){
  sprintf(network.wifiData.ssid,"%.*s",MAX_SSID,ssid);
  sprintf(network.wifiData.pwd,"%.*s",MAX_PWD,pwd);
  if(wifiNVS){                                                                      // is begin() already called and wifiNVS is open
    nvs_set_blob(wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
    nvs_commit(wifiNVS);                                                            // commit to NVS
  }

  return(*this);
}

///////////////////////////////

void Span::printfAttributes(int flags){

  hapOut << "{\"accessories\":[";

  for(int i=0;i<Accessories.size();i++){
    Accessories[i]->printfAttributes(flags);    
    if(i+1<Accessories.size())
      hapOut << "," ;
  }
    
  hapOut << "]}";
}

///////////////////////////////

boolean Span::deleteAccessory(uint32_t n){
  
  auto it=homeSpan.Accessories.begin();
  for(;it!=homeSpan.Accessories.end() && (*it)->aid!=n; it++);
  
  if(it==homeSpan.Accessories.end())
    return(false);

  delete *it;
  return(true);
}

///////////////////////////////

SpanCharacteristic *Span::find(uint32_t aid, uint32_t iid){

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

char *Span::escapeJSON(char *jObj){
 
  boolean quoting=false;
  char *p;
  
  for(int i=0,k=0;;i++){
    
    if(jObj[i]=='\0'){
      jObj[k]='\0';
      break;
    }

    if(!quoting){
      if(strchr(" \t\n\r",jObj[i]))
        continue;
    if(jObj[i]=='"')
      quoting=true;
    } else {
      if(jObj[i]=='"' && i>0 && jObj[i-1]!='\\')
        quoting=false;
      else if((p=strchr(delims,jObj[i])))
        jObj[i]=DELIM+p-delims;
    }
          
    jObj[k++]=jObj[i];
  }    

  return(jObj);
}

///////////////////////////////

char *Span::unEscapeJSON(char *jObj){

  for(int i=0;;i++){
    
    if(jObj[i]=='\0')
      break;

    if(jObj[i]>=DELIM && jObj[i]<=END_DELIM)
      jObj[i]=delims[jObj[i]-DELIM];
  }
    
  return(jObj);
}

///////////////////////////////

char *Span::strstr_r(const char *haystack, const char *needle){
  char *s=strstr(haystack,needle);
  if(s)
    s+=strlen(needle);
  return(s);  
}

///////////////////////////////

boolean Span::updateCharacteristics(char *buf, SpanBufVec &pVec){

  boolean twFail=false;
  char *jObj=escapeJSON(buf);
  size_t end=0;

  char *pidObj=strstr_r(jObj,"\"pid\":");
  if(pidObj){
    uint64_t pid=strtoull(pidObj,NULL,0);
    if(!TimedWrites.count(pid)){
      LOG0("\n*** ERROR:  Timed Write PID not found\n\n");
      twFail=true;
    } else        
    if(millis()>TimedWrites[pid]){
      LOG0("\n*** ERROR:  Timed Write Expired\n\n");
      twFail=true;
    }        
  }

  jObj=strstr_r(jObj,"\"characteristics\":");

  if(!jObj || !sscanf(jObj,"[%[^]]]%n",jObj,&end) || !end){
    LOG0("\n*** ERROR: Cannot extract properly-formatted \"characteristics\" array from JSON text\n\n");
    return(false);
  }

  for(;;){                // loop over objects in characteristics array
    int okay=0;
    end=0;
    if(!sscanf(jObj,"{%[^}]}%n",jObj,&end) || end==0){
      LOG0("\n*** ERROR: Cannot extract properly-formatted object from \"characteristics\" array\n\n");
      return(false);
    }

    SpanBuf sBuf;
    
    for(;;){              // loop over all name-value pairs in the object 
      end=0;
      char *name=jObj;
      if(sscanf(name,"\"%[^\"]\"%n",name,&end)!=1  || end==0){
        LOG0("\n*** ERROR: Cannot extract name from \"characteristics\" object\n\n");
        return(false);
      }
      char *value=name+end;
      if(sscanf(value,":%[^,]%n",value,&end)!=1){
        LOG0("\n*** ERROR: Cannot extract value from \"characteristics\" object\n\n");
        return(false);        
      }

      if(*value=='\"'){
        value++;
        end--;
        if(value[end-2]=='\"'){
          value[end-2]='\0';
          unEscapeJSON(value);
        }          
      }
      
      if(!strcmp(name,"aid")){
        sscanf(value,"%lu",&sBuf.aid);
        okay|=1;
      } else 
      if(!strcmp(name,"iid")){
        sscanf(value,"%lu",&sBuf.iid);
        okay|=2;
      } else 
      if(!strcmp(name,"value")){
        sBuf.val=value;
        okay|=4;
      } else 
      if(!strcmp(name,"ev")){
        sBuf.ev=value;
        okay|=8;
      } else 
      if(!strcmp(name,"r")){
        sBuf.wr=(!strcmp(value,"1") || !strcmp(value,"true"));
      } else {
        LOG0("\n*** ERROR:  Problems parsing JSON characteristics object - unexpected property \"%s\"\n\n",name);
        return(false);
      }
      
      jObj=value+end;
      if(*jObj++=='\0')
        break;
    }

    if(okay==7 || okay==11  || okay==15){                             // all required properties found
      if(!sBuf.val)                                                   // if value is NOT being updated
        sBuf.wr=false;                                                // ignore any request for write-response
      pVec.push_back(sBuf);                                           // add sBuf to pVec vector
    } else {
      LOG0("\n*** ERROR:  Problems parsing JSON characteristics object - missing required properties\n\n");
      return(false);
    }
    
    if(*++jObj=='\0')
      break;
    else if(*jObj++!=','){
      LOG0("\n*** ERROR: Unexpected characters trailing last object in \"characteristics\" array\n\n");
      return(false);
    }
  }

  snapTime=millis();                                           // timestamp for this series of updates, assigned to each characteristic in loadUpdate()

  for(auto it=pVec.begin();it!=pVec.end();it++){               // PASS 1: loop over all objects, identify characteristics, and initialize update for those found

    if(twFail){                                                // this is a timed-write request that has either expired or for which there was no PID
      (*it).status=StatusCode::InvalidValue;                   // set error for all characteristics      
      
    } else {
      (*it).characteristic = find((*it).aid,(*it).iid);        // find characteristic with matching aid/iid and store pointer          

      if((*it).characteristic)                                                           // if found, initialize characteristic update with new val/ev
        (*it).status=(*it).characteristic->loadUpdate((*it).val,(*it).ev,(*it).wr);      // save status code, which is either an error, or TBD (in which case updateFlag for the characteristic has been set to either 1 or 2) 
      else
        (*it).status=StatusCode::UnknownResource;                                        // if not found, set HAP error            
    }
      
  } // first pass
      
  for(auto it=pVec.begin();it!=pVec.end();it++){               // PASS 2: loop again over all objects       
    if((*it).status==StatusCode::TBD){                         // if object status still TBD

      StatusCode status=(*it).characteristic->service->update()?StatusCode::OK:StatusCode::Unable;          // update service and save statusCode as OK or Unable depending on whether return is true or false

      for(auto jt=it;jt!=pVec.end();jt++){                                                                  // loop over this object plus any remaining objects to update values and save status for any other characteristics in this service
        
        if((*jt).characteristic->service==(*it).characteristic->service){                                   // if service of this characteristic matches service that was updated
          (*jt).status=status;                                                                              // save statusCode for this object
          LOG1("Updating aid=%lu iid=%lu",(*jt).characteristic->aid,(*jt).characteristic->iid);
          if(status==StatusCode::OK){                                                                       // if status is okay
            (*jt).characteristic->uvSet((*jt).characteristic->value,(*jt).characteristic->newValue);        // update characteristic value with new value
            if((*jt).characteristic->nvsKey){                                                               // if storage key found
              if((*jt).characteristic->format<FORMAT::STRING)
                nvs_set_u64(charNVS,(*jt).characteristic->nvsKey,(*jt).characteristic->value.UINT64);       // store data as uint64_t regardless of actual type (it will be read correctly when access through uvGet())         
              else
                nvs_set_str(charNVS,(*jt).characteristic->nvsKey,(*jt).characteristic->value.STRING);       // store data
              nvs_commit(charNVS);
            }
            LOG1(" (okay)\n");
          } else {                                                                                          // if status not okay
            (*jt).characteristic->uvSet((*jt).characteristic->newValue,(*jt).characteristic->value);        // replace characteristic new value with original value
            LOG1(" (failed)\n");
          }
          (*jt).characteristic->updateFlag=0;                                                               // reset updateFlag for characteristic
        }
      }

    } // object had TBD status
  } // loop over all objects
      
  return(true);
}

///////////////////////////////

void Span::clearNotify(HAPClient *hc){

  for(auto const &acc : Accessories)
    for(auto const &svc : acc->Services)
      for(auto const &chr : svc->Characteristics)
        chr->evList.remove(hc);  
} 

///////////////////////////////

void Span::printfNotify(SpanBufVec &pVec, HAPClient *hc){

  boolean notifyFlag=false;
  
  for(auto it=pVec.begin();it!=pVec.end();it++){                 // loop over all objects
    
    if((*it).status==StatusCode::OK && (*it).val){               // characteristic was successfully updated with a new value (i.e. not just an EV request)
      if((*it).characteristic->evList.has(hc)){                  // if connection hc is subscribed to EV notifications for this characteristic
      
        if(!notifyFlag)                                          // this is first notification for any characteristic
          hapOut << "{\"characteristics\":[";                    // print start of JSON array
        else                                                     // else already printed at least one other characteristic
          hapOut << ",";                                         // add preceeding comma before printing this characteristic
        
        (*it).characteristic->printfAttributes(GET_VALUE|GET_AID|GET_NV);    // print JSON attributes for this characteristic
        notifyFlag=true;        
      }
    }
  }

  if(notifyFlag)
    hapOut << "]}";
}

///////////////////////////////

void Span::printfAttributes(SpanBufVec &pVec){

  hapOut << "{\"characteristics\":[";

  for(auto it=pVec.begin();it!=pVec.end();it++){
    if(it!=pVec.begin())
      hapOut << ",";
    hapOut << "{\"aid\":" << (*it).aid << ",\"iid\":" << (*it).iid << ",\"status\":" << (int)(*it).status;
    if((*it).status==StatusCode::OK && (*it).wr && (*it).characteristic)
      hapOut << ",\"value\":" << (*it).characteristic->uvPrint((*it).characteristic->value).c_str();
    hapOut << "}";
  }

  hapOut << "]}";
}

///////////////////////////////

boolean Span::printfAttributes(char **ids, int numIDs, int flags){

  uint32_t aid;
  uint32_t iid;
  
  SpanCharacteristic *Characteristics[numIDs];
  StatusCode status[numIDs];

  for(int i=0;i<numIDs;i++){              // PASS 1: loop over all ids requested to check status codes - only errors are if characteristic not found, or not readable
    sscanf(ids[i],"%lu.%lu",&aid,&iid);   // parse aid and iid
    Characteristics[i]=find(aid,iid);     // find matching chararacteristic
    
    if(Characteristics[i]){                                         // if found
      if(Characteristics[i]->perms&PERMS::PR){                      // if permissions allow reading
        status[i]=StatusCode::OK;                                   // always set status to OK (since no actual reading of device is needed)
      } else {
        Characteristics[i]=NULL;                                    // set to NULL to trigger not-found in Pass 2 below                                     
        status[i]=StatusCode::WriteOnly;
        flags|=GET_STATUS;                                          // update flags to require status attribute for all characteristics
      }
    } else {
      status[i]=StatusCode::UnknownResource;
      flags|=GET_STATUS;                                            // update flags to require status attribute for all characteristics
    }
  }

  hapOut << "{\"characteristics\":[";

  for(int i=0;i<numIDs;i++){              // PASS 2: loop over all ids requested and create JSON for each (either all with, or all without, a status attribute based on final flags setting)
    
    if(Characteristics[i])                                          // if found
      Characteristics[i]->printfAttributes(flags);                  // get JSON attributes for characteristic (may or may not include status=0 attribute)
    else{                                                           // else create JSON status attribute based on requested aid/iid
      sscanf(ids[i],"%lu.%lu",&aid,&iid);                             
      hapOut << "{\"iid\":" << iid << ",\"aid\":" << aid << ",\"status\":" << (int)status[i] << "}";     
    }
      
    if(i+1<numIDs)
      hapOut << ",";    
  }

  hapOut << "]}";

  return(flags&GET_STATUS);    
}

///////////////////////////////

Span& Span::resetIID(uint32_t newIID){

  if(Accessories.empty()){
    LOG0("\nFATAL ERROR!  Can't reset the Accessory IID count without a defined Accessory ***\n");
    LOG0("\n=== PROGRAM HALTED ===");
    while(1);
  }
  
  if(newIID<1){
    LOG0("\nFATAL ERROR!  Request to reset the Accessory IID count to 0 not allowed (IID must be 1 or greater) ***\n");
    LOG0("\n=== PROGRAM HALTED ===");
    while(1);    
  }
  
  Accessories.back()->iidCount=newIID-1;
  return(*this);
}

///////////////////////////////

boolean Span::updateDatabase(boolean updateMDNS){

  printfAttributes(GET_META|GET_PERMS|GET_TYPE|GET_DESC);   // stream attributes database, which automtically produces a SHA-384 hash
  hapOut.flush();  

  boolean changed=false;

  if(forceConfigIncrement || memcmp(hapOut.getHash(),hapConfig.hashCode,48)){       // if hash code of current HAP database does not match stored hash code, or force-increment is requested
    memcpy(hapConfig.hashCode,hapOut.getHash(),48);                                 // update stored hash code
    hapConfig.configNumber++;                                                       // increment configuration number
    if(hapConfig.configNumber==65536)                                               // reached max value
      hapConfig.configNumber=1;                                                     // reset to 1
                   
    nvs_set_blob(hapNVS,"HAPHASH",&hapConfig,sizeof(hapConfig));     // update data
    nvs_commit(hapNVS);                                              // commit to NVS
    changed=true;

    if(updateMDNS){
      char cNum[16];
      sprintf(cNum,"%d",hapConfig.configNumber);
      mdns_service_txt_item_set("_hap","_tcp","c#",cNum);      
    }
  }

  nvs_stats_t nvs_stats;
  nvs_get_stats(NULL, &nvs_stats);
  if(nvs_stats.free_entries<=130)
    LOG0("\n*** WARNING: NVS is running low on space.  Try erasing with 'E'.  If that fails, increase size of NVS partition or reduce NVS usage.\n\n");

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

list<Controller, Mallocator<Controller>>::const_iterator Span::controllerListBegin(){
  return(HAPClient::controllerList.cbegin());
}

///////////////////////////////

list<Controller, Mallocator<Controller>>::const_iterator Span::controllerListEnd(){
  return(HAPClient::controllerList.cend());
}

///////////////////////////////
//      SpanAccessory        //
///////////////////////////////

SpanAccessory::SpanAccessory(uint32_t aid){

  if(!homeSpan.Accessories.empty()){

    if(homeSpan.Accessories.size()==HAPClient::MAX_ACCESSORIES){
      LOG0("\n\n*** FATAL ERROR: Can't create more than %d Accessories.  Program Halting.\n\n",HAPClient::MAX_ACCESSORIES);
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
  LOG1("Deleted Accessory AID=%lu\n",aid);
}

///////////////////////////////

void SpanAccessory::printfAttributes(int flags){

  hapOut << "{\"aid\":" << aid << ",\"services\":[";

  for(int i=0;i<Services.size();i++){
    Services[i]->printfAttributes(flags);    
    if(i+1<Services.size())
      hapOut << ",";
    }
    
  hapOut << "]}";
}

///////////////////////////////
//       SpanService         //
///////////////////////////////

SpanService::SpanService(const char *type, const char *hapName, boolean isCustom){

  if(homeSpan.Accessories.empty()){
    LOG0("\nFATAL ERROR!  Can't create new Service '%s' without a defined Accessory ***\n",hapName);
    LOG0("\n=== PROGRAM HALTED ===");
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
  
  LOG1("Deleted Service AID=%lu IID=%lu\n",accessory->aid,iid); 
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

void SpanService::printfAttributes(int flags){

  hapOut << "{\"iid\":" << iid << ",\"type\":\"" << type << "\",";
  
  if(hidden)
    hapOut << "\"hidden\":true,";
    
  if(primary)
    hapOut << "\"primary\":true,";

  if(!linkedServices.empty()){
    hapOut << "\"linked\":[";
    for(int i=0;i<linkedServices.size();i++){
      hapOut << linkedServices[i]->iid;
      if(i+1<linkedServices.size())
        hapOut << ",";
    }
     hapOut << "],";
  }
    
  hapOut << "\"characteristics\":[";
  
  for(int i=0;i<Characteristics.size();i++){
    Characteristics[i]->printfAttributes(flags);    
    if(i+1<Characteristics.size())
      hapOut << ",";
  }
    
  hapOut << "]}";
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
    LOG0("\nFATAL ERROR!  Can't create new Characteristic '%s' without a defined Service ***\n",hapName);
    LOG0("\n=== PROGRAM HALTED ===");
    while(1);
  }

  homeSpan.Accessories.back()->Services.back()->Characteristics.push_back(this);  
  iid=++(homeSpan.Accessories.back()->iidCount);
  service=homeSpan.Accessories.back()->Services.back();
  aid=homeSpan.Accessories.back()->aid;
}

///////////////////////////////

SpanCharacteristic::~SpanCharacteristic(){

  auto chr=service->Characteristics.begin();              // find Characteristic in containing Service vector and erase entry
  while((*chr)!=this)
    chr++;
  service->Characteristics.erase(chr);

  free(desc);
  free(unit);
  free(validValues);
  free(nvsKey);

  if(format>=FORMAT::STRING){
    free(value.STRING);
    free(newValue.STRING);
  }
  
  LOG1("Deleted Characteristic AID=%lu IID=%lu\n",aid,iid);  
}

///////////////////////////////

String SpanCharacteristic::uvPrint(UVal &u){
  char c[64];
  switch(format){
    case FORMAT::BOOL:
      return(String(u.BOOL));      
    case FORMAT::INT:
      return(String(u.INT));
    case FORMAT::UINT8:
      return(String(u.UINT8));        
    case FORMAT::UINT16:
      return(String(u.UINT16));        
    case FORMAT::UINT32:
      return(String(u.UINT32));        
    case FORMAT::UINT64:
      sprintf(c,"%llu",u.UINT64);
      return(String(c));        
    case FORMAT::FLOAT:
      sprintf(c,"%g",u.FLOAT);
      return(String(c));        
    case FORMAT::STRING:
    case FORMAT::DATA:
    case FORMAT::TLV_ENC:
      return(String("\"") + String(u.STRING) + String("\""));        
  } // switch
  return(String());       // included to prevent compiler warnings
}

///////////////////////////////

void SpanCharacteristic::uvSet(UVal &dest, UVal &src){
  if(format>=FORMAT::STRING)
    uvSet(dest,(const char *)src.STRING);
  else
    dest=src;
}

///////////////////////////////

void SpanCharacteristic::uvSet(UVal &u, STRING_t val){
  u.STRING = (char *)HS_REALLOC(u.STRING, strlen(val) + 1);
  strcpy(u.STRING, val);
}

///////////////////////////////

void SpanCharacteristic::uvSet(UVal &u, DATA_t data){
  
  if(data.second>0){
    size_t olen;
    mbedtls_base64_encode(NULL,0,&olen,NULL,data.second);                              // get length of string buffer needed (mbedtls includes the trailing null in this size)
    value.STRING = (char *)HS_REALLOC(value.STRING,olen);                              // allocate sufficient size for storing value
    mbedtls_base64_encode((uint8_t*)value.STRING,olen,&olen,data.first,data.second );  // encode data into string buf
  } else {
    value.STRING = (char *)HS_REALLOC(value.STRING,1);                                 // allocate sufficient size for just trailing null character
    *value.STRING ='\0';
  }  
}

///////////////////////////////

void SpanCharacteristic::uvSet(UVal &u, TLV_ENC_t tlv){

  const size_t bufSize=36;                                  // maximum size of buffer to store packed TLV bytes before encoding directly into value; must be multiple of 3
  size_t nBytes=tlv.pack_size();                            // total size of packed TLV in bytes

  if(nBytes>0){
    size_t nChars;
    mbedtls_base64_encode(NULL,0,&nChars,NULL,nBytes);      // get length of string buffer needed (mbedtls includes the trailing null in this size)
    u.STRING = (char *)HS_REALLOC(u.STRING,nChars);         // allocate sufficient size for storing value
    TempBuffer<uint8_t> tBuf(bufSize);                      // create fixed-size buffer to store packed TLV bytes
    tlv.pack_init();                                        // initialize TLV packing
    uint8_t *p=(uint8_t *)u.STRING;                         // set pointer to beginning of value
    while((nBytes=tlv.pack(tBuf,bufSize))>0){               // pack the next set of TLV bytes, up to a maximum of bufSize, into tBuf
      size_t olen;                                          // number of characters written (excludes null character)
      mbedtls_base64_encode(p,nChars,&olen,tBuf,nBytes);    // encode data directly into value
      p+=olen;                                              // advance pointer to null character
      nChars-=olen;                                         // subtract number of characters remaining
    }
  } else {
    u.STRING = (char *)HS_REALLOC(u.STRING,1);              // allocate sufficient size for just trailing null character
    *u.STRING ='\0';
  }  
}

///////////////////////////////

char *SpanCharacteristic::getStringGeneric(UVal &val){
  if(format>=FORMAT::STRING)
      return val.STRING;

  return NULL;
}

///////////////////////////////

void SpanCharacteristic::setString(const char *val, boolean notify){ 

  setValCheck();
  uvSet(value,val);
  setValFinish(notify);    
}

///////////////////////////////

size_t SpanCharacteristic::getDataGeneric(uint8_t *data, size_t len, UVal &val){    
  if(format<FORMAT::DATA)
    return(0);

  size_t olen;
  int ret=mbedtls_base64_decode(data,len,&olen,(uint8_t *)val.STRING,strlen(val.STRING));
  
  if(data==NULL)
    return(olen);
    
  if(ret==MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL)
    LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Destination buffer is too small (%d out of %d bytes needed)!\n\n",hapName,len,olen);
  else if(ret==MBEDTLS_ERR_BASE64_INVALID_CHARACTER)
    LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getData().  Data is not in base-64 format!\n\n",hapName);
    
  return(olen);
}

///////////////////////////////

void SpanCharacteristic::setData(const uint8_t *data, size_t len, boolean notify){

  setValCheck();
  uvSet(value,{data,len});
  setValFinish(notify);
} 
  
///////////////////////////////

size_t SpanCharacteristic::getTLVGeneric(TLV8 &tlv, UVal &val){
   
  if(format<FORMAT::TLV_ENC)
    return(0);

  const size_t bufSize=36;                    // maximum size of buffer to store decoded bytes before unpacking into TLV; must be multiple of 3
  TempBuffer<uint8_t> tBuf(bufSize);          // create fixed-size buffer to store decoded bytes
  tlv.wipe();                                 // clear TLV completely

  size_t nChars=strlen(val.STRING);         // total characters to decode
  uint8_t *p=(uint8_t *)val.STRING;         // set pointer to beginning of value
  const size_t decodeSize=bufSize/3*4;      // number of characters to decode in each pass
  int status=0;
  
  while(nChars>0){
    size_t olen;
    size_t n=nChars<decodeSize?nChars:decodeSize;
    
    int ret=mbedtls_base64_decode(tBuf,tBuf.len(),&olen,p,n);
    if(ret==MBEDTLS_ERR_BASE64_INVALID_CHARACTER){
      LOG0("\n*** WARNING:  Can't decode Characteristic::%s with getTLV().  Data is not in base-64 format!\n\n",hapName);
      tlv.wipe();
      return(0);
    }
    status=tlv.unpack(tBuf,olen);
    p+=n;
    nChars-=n;
  }
  if(status>0){
    LOG0("\n*** WARNING:  Can't unpack Characteristic::%s with getTLV().  TLV record is incomplete or corrupted!\n\n",hapName);
    tlv.wipe();
    return(0);      
  }
return(tlv.pack_size());
}

///////////////////////////////

void SpanCharacteristic::setTLV(const TLV8 &tlv, boolean notify){

  setValCheck();
  uvSet(value,tlv);
  setValFinish(notify);
}

///////////////////////////////

void SpanCharacteristic::setValCheck(){
  if(updateFlag==1)
    LOG0("\n*** WARNING:  Attempt to set value of Characteristic::%s within update() while it is being simultaneously updated by Home App.  This may cause device to become non-responsive!\n\n",hapName);
}

///////////////////////////////

void SpanCharacteristic::setValFinish(boolean notify){

  uvSet(newValue,value);     
  updateTime=homeSpan.snapTime;

  if(notify){
    if((perms&EV) && (updateFlag!=2)){        // only broadcast notification if EV permission is set AND update is NOT being done in context of write-response    
      SpanBuf sb;                             // create SpanBuf object
      sb.characteristic=this;                 // set characteristic          
      sb.status=StatusCode::OK;               // set status
      char dummy[]="";
      sb.val=dummy;                           // set dummy "val" so that printfNotify knows to consider this "update"
      homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector
    }

    if(nvsKey){
      nvs_set_str(homeSpan.charNVS,nvsKey,value.STRING);    // store data
      nvs_commit(homeSpan.charNVS);
    }
  }      
}

///////////////////////////////

void SpanCharacteristic::printfAttributes(int flags){

  const char permCodes[][7]={"pr","pw","ev","aa","tw","hd","wr"};
  const char formatCodes[][9]={"bool","uint8","uint16","uint32","uint64","int","float","string","data","tlv8"};

  hapOut << "{\"iid\":" << iid;

  if(flags&GET_TYPE)
    hapOut << ",\"type\":\"" << type << "\"";

  if((perms&PR) && (flags&GET_VALUE)){    
    if(perms&NV && !(flags&GET_NV))
      hapOut << ",\"value\":null";
    else
      hapOut << ",\"value\":" << uvPrint(value).c_str();
  }

  if(flags&GET_META){
    hapOut << ",\"format\":\"" << formatCodes[format] << "\"";
    
    if(customRange && (flags&GET_META)){
      hapOut << ",\"minValue\":" << uvPrint(minValue).c_str() << ",\"maxValue\":" << uvPrint(maxValue).c_str();
        
      if(uvGet<float>(stepValue)>0)
        hapOut << ",\"minStep\":" << uvPrint(stepValue).c_str();
    }

    if(unit){
      if(strlen(unit)>0)
        hapOut << ",\"unit\":\"" << unit << "\"";
     else
        hapOut << ",\"unit\":null";
    }

    if(validValues){
      hapOut << ",\"valid-values\":" << validValues;
    }

    if(maxLen){
      hapOut << ",\"maxLen\":" << (int)maxLen;
    }
  }
    
  if(desc && (flags&GET_DESC)){
    hapOut << ",\"description\":\"" << desc << "\"";
  }

  if(flags&GET_PERMS){
    hapOut << ",\"perms\":[";
    for(int i=0;i<7;i++){
      if(perms&(1<<i)){
        hapOut << "\"" << permCodes[i] <<"\"";
        if(perms>=(1<<(i+1)))
          hapOut << ",";
      }
    }
    hapOut << "]";
  }

  if(flags&GET_AID)
    hapOut << ",\"aid\":" << aid;

  HAPClient *hc=&(*(homeSpan.currentClient));
  
  if(flags&GET_EV)
    hapOut << ",\"ev\":" << (evList.has(hc)?"true":"false");

  if(flags&GET_STATUS)
    hapOut << ",\"status\":0";    

  hapOut << "}";
}

///////////////////////////////

StatusCode SpanCharacteristic::loadUpdate(char *val, char *ev, boolean wr){

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
      
    LOG1("Notification Request for aid=%lu iid=%lu: %s\n",aid,iid,evFlag?"true":"false");
    HAPClient *hc=&(*(homeSpan.currentClient));
    
    if(evFlag)
      evList.add(hc);
    else
      evList.remove(hc);
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
      else if(!sscanf(val,"%ld",&newValue.INT))
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
      else if(!sscanf(val,"%lu",&newValue.UINT32))
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
      
    case DATA:
    case TLV_ENC:
      uvSet(newValue,(const char *)stripBackslash(val));
      break;

    default:
    break;

  } // switch

  updateFlag=1+wr;                // set flag to 1 if successful update or 2 if successful AND write-response flag is set
  updateTime=homeSpan.snapTime;
  return(StatusCode::TBD);
}

///////////////////////////////

unsigned long SpanCharacteristic::timeVal(){
  
  return(homeSpan.snapTime-updateTime);
}

///////////////////////////////

boolean SpanCharacteristic::updated(){
  
  return(updateFlag>0);
}

///////////////////////////////

uint32_t SpanCharacteristic::getIID(){
  
  return(iid);
}

///////////////////////////////

uint32_t SpanCharacteristic::getAID(){
  
  return(aid);
}

///////////////////////////////

boolean SpanCharacteristic::foundIn(const char *getCharList){

  char *charID;

  asprintf(&charID,"%lu.%lu",getAID(),getIID());
  boolean res=strstr(getCharList,charID);
  free(charID);
  return(res);
}

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::setPerms(uint8_t perms){
  perms&=0x7F;
  if(perms>0)
    this->perms=perms;
  return(this);
}

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::addPerms(uint8_t dPerms){
  return(setPerms(perms|dPerms));
}

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::removePerms(uint8_t dPerms){
  return(setPerms(perms&(~dPerms)));
}

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::setDescription(const char *c){
  desc = (char *)HS_REALLOC(desc, strlen(c) + 1);
  strcpy(desc, c);
  return(this);
}  

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::setUnit(const char *c){
  unit = (char *)HS_REALLOC(unit, strlen(c) + 1);
  strcpy(unit, c);
  return(this);
}  

///////////////////////////////

SpanCharacteristic *SpanCharacteristic::setMaxStringLength(uint8_t n){
  if(format==FORMAT::STRING)
    maxLen=n;
  return(this);
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

  validValues=(char *)HS_REALLOC(validValues, strlen(s.c_str()) + 1);
  strcpy(validValues,s.c_str());

  return(this);
}

///////////////////////////////

boolean SpanCharacteristic::EVLIST::has(HAPClient *hc){
  return(find_if(begin(), end(), [hc](const HAPClient *hcTemp){return(hc==hcTemp);}) != end());  
}

///////////////////////////////

void SpanCharacteristic::EVLIST::add(HAPClient *hc){
  if(!has(hc))
    push_back(hc);
}

///////////////////////////////

void SpanCharacteristic::EVLIST::remove(HAPClient *hc){
  auto it=remove_if(begin(), end(), [hc](const HAPClient *hcTemp){return(hc==hcTemp);});
  erase(it,end());
}

///////////////////////////////
//        SpanButton         //
///////////////////////////////

SpanButton::SpanButton(int pin, uint16_t longTime, uint16_t singleTime, uint16_t doubleTime, triggerType_t triggerType) : PushButton(pin, triggerType){

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    if(buttonType==HS_BUTTON)
      LOG0("\nFATAL ERROR!  Can't create new SpanButton(%d,%u,%u,%u) without a defined Service ***\n",pin,longTime,singleTime,doubleTime);
    else
      LOG0("\nFATAL ERROR!  Can't create new SpanToggle(%d,%u) without a defined Service ***\n",pin,longTime);
      
    LOG0("\n=== PROGRAM HALTED ===");
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

  if( (buttonType==HS_BUTTON && triggered(singleTime,longTime,doubleTime)) ||
      (buttonType==HS_TOGGLE && toggled(longTime)) )                             // if the underlying PushButton is triggered/toggled
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
  this->maxEntries=maxEntries;
  timeServer=serv;
  timeZone=tz;
  if(url){
    asprintf(&statusURL,"/%s",url);
    isEnabled=true;
  }
  log = (log_t *)HS_CALLOC(maxEntries,sizeof(log_t));
}

///////////////////////////////

void SpanWebLog::initTime(void *args){
  SpanWebLog *wLog = (SpanWebLog *)args;
  
  WEBLOG("Acquiring Time from %s (%s)",wLog->timeServer,wLog->timeZone,wLog->waitTime/1000);
  configTzTime(wLog->timeZone,wLog->timeServer);
  struct tm timeinfo;
  if(getLocalTime(&timeinfo,wLog->waitTime)){
    strftime(wLog->bootTime,sizeof(wLog->bootTime),"%c",&timeinfo);
    wLog->timeInit=true;
    WEBLOG("Time Acquired: %s",wLog->bootTime);
  } else {
    WEBLOG("Can't access Time Server after %d seconds",wLog->waitTime/1000);
  }

  vTaskDelete(NULL);
  
}

///////////////////////////////

int SpanWebLog::check(const char *uri){

  size_t n=strlen(statusURL);

  if(strncasecmp(uri,statusURL,n)!=0)        // no partial match of statusURL
    return(-1);
  if(uri[n]==' ')                            // match without query string
    return(0);
  if(uri[n]=='?'){                           // match with query string
    char val[6]="0";
    Network_HS::getFormValue(uri+n+1,"refresh",val,5);
    return(atoi(val)>0?atoi(val):0);
  }
  return(-1);                                // no match
}

///////////////////////////////

void SpanWebLog::vLog(boolean sysMsg, const char *fmt, va_list ap){

  std::unique_lock writeLock(mux);        // wait for mux to be unlocked and then lock *exclusively* so write can proceed uninterrupted
  
  char *buf;
  vasprintf(&buf,fmt,ap);

  if(sysMsg)
    LOG0("%s\n",buf);
  else
    LOG1("WEBLOG: %s\n",buf);
  
  if(maxEntries>0){
    int index=nEntries%maxEntries;
  
    log[index].upTime=esp_timer_get_time();
    if(timeInit)
      getLocalTime(&log[index].clockTime,10);
    else
      log[index].clockTime.tm_year=0;
  
    log[index].message=(char *)HS_REALLOC(log[index].message, strlen(buf) + 1);
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
    LOG0("\n*** WARNING: Can't start OTA Server - Partition table used to compile this sketch is not configured for OTA.\n\n");
    return(-1);     
  }
  
  enabled=true;
  safeLoad=_safeLoad;
  auth=_auth;
  if(pwd==NULL)
    return(0);
  return(setPassword(pwd));
}

///////////////////////////////

int SpanOTA::setPassword(const char *pwd){

#ifdef SHA2Builder_h
  const int hashSize=64;
#else
  const int hashSize=32;
#endif

  int x=0, y=0;
  sscanf(pwd,"0%*1[xX]%*[0-9A-Fa-f]%n%*c%n",&x,&y);

  if(x==hashSize+2 && y==0){
    for(int i=0;i<=hashSize;i++)
      homeSpan.spanOTA.otaPwd[i]=tolower(pwd[i+2]);
    return(0);
  }

  if(strlen(pwd)<1 || strlen(pwd)>32){
    LOG0("\n*** WARNING: Cannot change OTA password to '%s'. Password length must be between 1 and 32 characters, or a valid %s hash.\n\n",pwd,hashSize==32?"MD5":"SHA256");
    return(-1);
  }

  hsOtaHash_t otaPwdHash;
  otaPwdHash.begin();
  otaPwdHash.add(pwd);
  otaPwdHash.calculate();
  otaPwdHash.getChars(homeSpan.spanOTA.otaPwd);  
  return(0);
}

///////////////////////////////

void SpanOTA::start(){
  LOG0("\n*** Current Partition: %s\n*** New Partition: %s\n*** OTA Starting..",
    esp_ota_get_running_partition()->label,esp_ota_get_next_update_partition(NULL)->label);
  otaPercent=0;
  STATUS_UPDATE(start(LED_OTA_STARTED),HS_OTA_STARTED)
}

///////////////////////////////

void SpanOTA::end(){
  nvs_set_u8(homeSpan.otaNVS,"OTA_REQUIRED",safeLoad);
  nvs_commit(homeSpan.otaNVS);
  LOG0(" DONE!  Rebooting...\n");
  homeSpan.reboot();
}

///////////////////////////////

void SpanOTA::progress(uint32_t progress, uint32_t total){

  homeSpan.resetWatchdog();

  int percent=progress*100/total;
  if(percent/10 != otaPercent/10){
    otaPercent=percent;
    LOG0("%d%%..",percent);
  }

  if(safeLoad && progress==total && ArduinoOTA.getCommand() == U_FLASH){
    SpanPartition newSpanPartition;   
    esp_partition_read(esp_ota_get_next_update_partition(NULL), sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) + sizeof(esp_app_desc_t), &newSpanPartition, sizeof(newSpanPartition));
    LOG0("Checking for HomeSpan Magic Cookie: %s..",spanPartition.magicCookie);
    if(strcmp(newSpanPartition.magicCookie,spanPartition.magicCookie)){
      LOG0("  *** NOT FOUND!  ABORTING\n");
      Update.abort();
    }
  }
}

///////////////////////////////

void SpanOTA::error(ota_error_t err){
  LOG0("*** OTA Error[%u]: ", err);
  if (err == OTA_AUTH_ERROR) LOG0("Auth Failed\n\n");
    else if (err == OTA_BEGIN_ERROR) LOG0("Begin Failed\n\n");
    else if (err == OTA_CONNECT_ERROR) LOG0("Connect Failed\n\n");
    else if (err == OTA_RECEIVE_ERROR) LOG0("Receive Failed\n\n");
    else if (err == OTA_END_ERROR) LOG0("End Failed\n\n");
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
    LOG0("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\") - Invalid MAC Address ***\n",macAddress);
    LOG0("\n=== PROGRAM HALTED ===");
    while(1);
  }

  if(sendSize<0 || sendSize>200 || receiveSize<0 || receiveSize>200 || queueDepth<1 || (sendSize==0 && receiveSize==0)){
    LOG0("\nFATAL ERROR!  Can't create new SpanPoint(\"%s\",%d,%d,%d) - one or more invalid parameters ***\n",macAddress,sendSize,receiveSize,queueDepth);
    LOG0("\n=== PROGRAM HALTED ===");
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
  
  peerInfo.encrypt=useEncryption;     // set encryption for this peer
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
  mbedtls_sha256((const unsigned char *)password,strlen(password),hash,0);      // produce 256-bit bit hash from password

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
    LOG0("\nFATAL ERROR!  SpanPoint::setChannelMask(0x%04X) - mask must allow for at least one channel ***\n",mask);
    LOG0("\n=== PROGRAM HALTED ===");
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

void SpanPoint::dataReceived(const esp_now_recv_info *info, const uint8_t *incomingData, int len){

  const uint8_t *mac=info->src_addr;
  
  auto it=SpanPoints.begin();
  for(;it!=SpanPoints.end() && memcmp((*it)->peerInfo.peer_addr,mac,6)!=0; it++);
  
  if(it==SpanPoints.end())
    return;

  if((*it)->receiveSize==0)
    return;

  if(len!=(*it)->receiveSize){
    LOG0("SpanPoint Warning! %d bytes received from %02X:%02X:%02X:%02X:%02X:%02X does not match %d-byte queue size\n",len,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],(*it)->receiveSize);
    return;
  }

  (*it)->receiveTime=millis();                             // set time of receive
  xQueueSend((*it)->receiveQueue, incomingData, 0);        // send to queue - do not wait if queue is full and instead fail immediately since we need to return from this function ASAP
}

///////////////////////////////

uint8_t SpanPoint::lmk[16];
boolean SpanPoint::initialized=false;
boolean SpanPoint::isHub=false;
boolean SpanPoint::useEncryption=true;
vector<SpanPoint *, Mallocator<SpanPoint *>> SpanPoint::SpanPoints;
uint16_t SpanPoint::channelMask=0x3FFE;
QueueHandle_t SpanPoint::statusQueue;
nvs_handle SpanPoint::pointNVS;

///////////////////////////////
//          MISC             //
///////////////////////////////

void __attribute__((weak)) loop(){
  vTaskDelay(1);
}

///////////////////////////////



 
