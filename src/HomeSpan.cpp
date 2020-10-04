
#include <ESPmDNS.h>
#include <nvs_flash.h>
#include <sodium.h>
#include <WiFi.h>

#include "HomeSpan.h"
#include "HAP.h"

using namespace Utils;

WiFiServer hapServer(80);           // HTTP Server (i.e. this acccesory) running on usual port 80 (local-scoped variable to this file only)

HAPClient **hap;                    // HAP Client structure containing HTTP client connections, parsing routines, and state variables (global-scoped variable)
Span homeSpan;                      // HAP Attributes database and all related control functions for this Accessory (global-scoped variable)

///////////////////////////////
//         Span              //
///////////////////////////////

void Span::begin(Category catID, char *displayName, char *hostNameBase, char *modelName){
  
  this->displayName=displayName;
  this->hostNameBase=hostNameBase;
  this->modelName=modelName;
  sprintf(this->category,"%d",catID);

  controlButton.init(controlPin);
  statusLED.init(statusPin);

  hap=(HAPClient **)calloc(maxConnections,sizeof(HAPClient *));
  for(int i=0;i<maxConnections;i++)
    hap[i]=new HAPClient;
  
  delay(2000);
 
  Serial.print("\n************************************************************\n"
                 "Welcome to HomeSpan!\n"
                 "Apple HomeKit for the Espressif ESP-32 WROOM and Arduino IDE\n"
                 "************************************************************\n\n"
                 "** Please ensure serial monitor is set to transmit <newlines>\n\n");

  Serial.print("Message Logs:     Level ");
  Serial.print(homeSpan.logLevel);  
  Serial.print("\nStatus LED:       Pin ");
  Serial.print(statusPin);  
  Serial.print("\nDevice Control:   Pin ");
  Serial.print(controlPin);  
  Serial.print("\nHomeSpan Version: ");
  Serial.print(HOMESPAN_VERSION);
  Serial.print("\nESP-IDF Version:  ");
  Serial.print(esp_get_idf_version());
  Serial.print("\nSketch Compiled:  ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.print("\n\n");

  if(!digitalRead(controlPin)){                     // factory reset pin is low upon start-up
    Serial.print("** CONTROL BUTTON PRESSED DURING STARTUP!  PERFORMING FACTORY RESET **\n\n");
    statusLED.start(LED_ALERT);
    nvs_flash_erase();                              // erase NVS storage
    delay(5000);
    Serial.print("Re-starting...\n\n");
    statusLED.off();
    ESP.restart();
  }
      
}  // begin

///////////////////////////////

void Span::poll() {

  if(!strlen(category)){
    Serial.print("\n** FATAL ERROR: Cannot run homeSpan.poll() without an initial call to homeSpan.begin()!\n** PROGRAM HALTED **\n\n");
    while(1);    
  }

  if(!isInitialized){

    if(logLevel>0 || nFatalErrors>0){
      Serial.print(configLog);
      Serial.print("\n*** End Config Log ***\n");
    }
  
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
        
    nvs_flash_init();         // initialize non-volatile-storage partition in flash 
    HAPClient::init();        // read NVS and load HAP settings  
    initWifi();               // initialize WiFi

    if(!HAPClient::nAdminControllers()){
      Serial.print("DEVICE NOT YET PAIRED -- PLEASE PAIR WITH HOMEKIT APP\n\n");
      statusLED.start(LED_PAIRING_NEEDED);
    } else {
      statusLED.on();
    }

    controlButton.reset();        

    Serial.print(displayName);
    Serial.print(" is READY!\n\n");
    isInitialized=true;
  }

  if(WiFi.status()!=WL_CONNECTED){
      Serial.print("*** LOST WIFI CONNECTION! ***\n\n");    
      initWifi();
  }

  char cBuf[17]="?";
  
  if(Serial.available()){
    readSerial(cBuf,16);
    processSerialCommand(cBuf);
  }

  WiFiClient newClient;

  if(newClient=hapServer.available()){         // found a new HTTP client
    int freeSlot=getFreeSlot();                 // get next free slot

    if(freeSlot==-1){                           // no available free slots
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

  if(controlButton.primed()){
    statusLED.start(LED_ALERT);
  }
  
  if(controlButton.triggered(5000,10000)){
    statusLED.off();
    if(controlButton.longPress()){
      processSerialCommand("W");        // DELETE WiFi Data      
    } else {
      controlButton.reset();
      processSerialCommand("U");        // UNPAIR Device
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

void Span::initWifi(){

  char id[18];                              // create string version of Accessory ID for MDNS broadcast
  memcpy(id,HAPClient::accessory.ID,17);    // copy ID bytes
  id[17]='\0';                              // add terminating null

  // create broadcaset name from server base name plus accessory ID (without ':')
  
  int nChars=snprintf(NULL,0,"%s-%.2s%.2s%.2s%.2s%.2s%.2s",hostNameBase,id,id+3,id+6,id+9,id+12,id+15);       
  char hostName[nChars+1];
  sprintf(hostName,"%s-%.2s%.2s%.2s%.2s%.2s%.2s",hostNameBase,id,id+3,id+6,id+9,id+12,id+15);

  size_t len;             // not used but required to read blobs from NVS
  
  if(!nvs_get_blob(HAPClient::wifiNVS,"WIFIDATA",NULL,&len)){                   // if found WiFi data in NVS
    nvs_get_blob(HAPClient::wifiNVS,"WIFIDATA",&network.wifiData,&len);         // retrieve data
    
  } else {                                                              // configure network and setup code
    
    network.scan();         // scan for networks    
    
    int status=-1;
    char key[2];

    controlButton.reset();
    
    while(status<1){                   // loop until a configuration method is chosen and completed

      if(status==-1){
        if(WiFi.status()==WL_CONNECTED)
          WiFi.disconnect();
          
        Serial.print("Network configuration required!  Found the following SSIDs:\n\n");
        statusLED.start(LED_INPUT_NEEDED);   // rapidly blink Status LED
      
        for(int i=0;i<network.numSSID;i++){
          Serial.print("  ");
          Serial.print(i+1);
          Serial.print(") ");
          Serial.print(network.ssidList[i]);
          Serial.print("\n");
        }

        Serial.print("\nType 'W <return>' to set WiFi credentials or press Control Button for 3 seconds to start Access Point...\n\n");
        status=0;
        sprintf(key,"");
      }

      if(controlButton.triggered(9999,3000)){    
        network.apConfigure(hostName);       
        status=1;
      }

      if(Serial.available() && *readSerial(key,1)=='W'){
        status=network.serialConfigure()?1:-1;         
      }
      
    } // while loop  

    Serial.print("Saving WiFi credentials for: ");
    Serial.print(network.wifiData.ssid);
    Serial.print("...\n");

    nvs_set_blob(HAPClient::wifiNVS,"WIFIDATA",&network.wifiData,sizeof(network.wifiData));    // update data
    nvs_commit(HAPClient::wifiNVS);                                                            // commit to NVS

    if(strlen(network.setupCode)){

      char buf[128];

      struct {                                      // temporary structure to hold SRP verification code and salt stored in NVS
        uint8_t salt[16];
        uint8_t verifyCode[384];
      } verifyData;      

      sprintf(buf,"\nGenerating SRP verification data for new Setup Code: %.3s-%.2s-%.3s ... ",network.setupCode,network.setupCode+3,network.setupCode+5);
      Serial.print(buf);
      HAPClient::srp.createVerifyCode(network.setupCode,verifyData.verifyCode,verifyData.salt);                 // create verification code from default Setup Code and random salt
      nvs_set_blob(HAPClient::srpNVS,"VERIFYDATA",&verifyData,sizeof(verifyData));                              // update data
      nvs_commit(HAPClient::srpNVS);                                                                            // commit to NVS
      Serial.print("New Code Saved!\n");
    }

    Serial.print("\n*** Re-starting ***\n\n");
    delay(500);
    ESP.restart();                                  // re-start device   

  } // configure network
  
  int nTries=0;
  
  statusLED.start(LED_WIFI_CONNECTING);
  controlButton.reset();
  
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("Connecting to: ");
    Serial.print(network.wifiData.ssid);
    Serial.print("... ");
    nTries++;
    
    if(WiFi.begin(network.wifiData.ssid,network.wifiData.pwd)!=WL_CONNECTED){
      int delayTime=nTries%6?5000:60000;
      char buf[8]="";
      Serial.print("Can't connect. Re-trying in ");
      Serial.print(delayTime/1000);
      Serial.print(" seconds. Type 'W <return>' or press Control Button for 3 seconds to reset WiFi data...\n");
      long sTime=millis();
      
      while(millis()-sTime<delayTime){        
        if(controlButton.triggered(9999,3000) || (Serial.available() && readSerial(buf,1) && (buf[0]=='W'))){
          processSerialCommand("W");        // DELETE WiFi Data      
          return;
        }
      }
    }
  } // WiFi not yet connected

  Serial.print("Success!  IP:  ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");

  Serial.print("\nStarting MDNS...\n");
  Serial.print("Broadcasting as: ");
  Serial.print(hostName);
  Serial.print(".local (");
  Serial.print(displayName);
  Serial.print(" / ");
  Serial.print(modelName);
  Serial.print(")\n");

  MDNS.begin(hostName);                     // set server host name (.local implied)
  MDNS.setInstanceName(displayName);        // set server display name
  MDNS.addService("_hap","_tcp",80);        // advertise HAP service on HTTP port (80)

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

  Serial.print("\nStarting Web (HTTP) Server supporting up to ");
  Serial.print(maxConnections);
  Serial.print(" simultaneous connections...\n\n");
  hapServer.begin();

  statusLED.stop();
  
} // initWiFi

///////////////////////////////

void Span::processSerialCommand(char *c){

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
          Serial.print(" ");
      
          if(hap[i]->cPair){
            Serial.print("ID=");
            HAPClient::charPrintRow(hap[i]->cPair->ID,36);
            Serial.print(hap[i]->cPair->admin?"   (admin)":" (regular)");
          } else {
            Serial.print("(unverified)");
          }
      
        } else {
          Serial.print("(unconnected)");
        }

        Serial.print("\n");
      }

      Serial.print("\n*** End Status ***\n");
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
      }            
    }
    break;

    case 'U': {

      HAPClient::removeControllers();                                                                           // clear all Controller data  
      nvs_set_blob(HAPClient::hapNVS,"CONTROLLERS",HAPClient::controllers,sizeof(HAPClient::controllers));      // update data
      nvs_commit(HAPClient::hapNVS);                                                                            // commit to NVS
      Serial.print("\n** HomeSpan Pairing Data DELETED **\n\n");
      
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
      statusLED.start(LED_PAIRING_NEEDED);
    }
    break;

    case 'W': {

      nvs_erase_all(HAPClient::wifiNVS);
      nvs_commit(HAPClient::wifiNVS);      
      WiFi.disconnect();
      Serial.print("\n** WIFI Network Data DELETED **\n\n");
    }
    break;

    case 'H': {
      
      nvs_erase_all(HAPClient::hapNVS);
      nvs_commit(HAPClient::hapNVS);      
      Serial.print("\n** HomeSpan Device ID and Pairing Data DELETED **\n** Restarting...\n\n");
      delay(1000);
      ESP.restart();
    }
    break;

    case 'F': {
      
      nvs_flash_erase();
      Serial.print("\n** FACTORY RESET **\n** Restarting...\n\n");
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

      char d[]="------------------------------";
      Serial.print("\n*** HomeSpan Info ***\n\n");
      char cBuf[256];
      sprintf(cBuf,"%-30s  %s  %s  %s  %s  %s  %s\n","Service","Type","AID","IID","Update","Loop","Button");
      Serial.print(cBuf);
      sprintf(cBuf,"%.30s  %.4s  %.3s  %.3s  %.6s  %.4s  %.6s\n",d,d,d,d,d,d,d);
      Serial.print(cBuf);
      for(int i=0;i<Accessories.size();i++){                             // identify all services with over-ridden loop() methods
        for(int j=0;j<Accessories[i]->Services.size();j++){
          SpanService *s=Accessories[i]->Services[j];
          sprintf(cBuf,"%-30s  %4s  %3d  %3d  %6s  %4s  %6s\n",s->hapName,s->type,Accessories[i]->aid,s->iid, 
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
      Serial.print("  d - print attributes database\n");
      Serial.print("  i - print detailed info about configuration\n");
      Serial.print("  U - unpair device by deleting all Controller data\n");
      Serial.print("  W - delete stored WiFi data\n");      
      Serial.print("  H - delete stored HomeKit Pairing data and restart\n");      
      Serial.print("  F - delete all stored data (Factory Reset) and restart\n");      
      Serial.print("  ? - print this list of commands\n");
      Serial.print("  L <level> - change Log Level to <level>\n");
      Serial.print("  S <code>  - change Setup Code to 8-digit <code>\n");
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

SpanCharacteristic *Span::find(int aid, int iid){

  if(aid<1 || aid>Accessories.size())     // aid out of range
    return(NULL);

  aid--;                    // convert from aid to array index number
  
  for(int i=0;i<Accessories[aid]->Services.size();i++){                   // loop over all Services in this Accessory
    for(int j=0;j<Accessories[aid]->Services[i]->Characteristics.size();j++){     // loop over all Characteristics in this Service
      
      if(iid == Accessories[aid]->Services[i]->Characteristics[j]->iid)     // if matching iid
        return(Accessories[aid]->Services[i]->Characteristics[j]);          // return pointer to Characteristic
    }
  }

  return(NULL);
}

///////////////////////////////

int Span::countCharacteristics(char *buf){

  int nObj=0;
  
  const char tag[]="\"aid\"";
  while(buf=strstr(buf,tag)){         // count number of characteristic objects in PUT JSON request
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
        pObj[nObj].aid=atoi(t3);
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
        
        nChars+=pObj[i].characteristic->sprintfAttributes(cBuf?(cBuf+nChars):NULL,GET_AID);    // get JSON attributes for characteristic
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
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?128:0,"{\"aid\":%d,\"iid\":%d,\"status\":%d}",pObj[i].aid,pObj[i].iid,pObj[i].status);
      if(i+1<nObj)
        nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",");
  }

  nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"]}");

  return(nChars);    
}

///////////////////////////////

int Span::sprintfAttributes(char **ids, int numIDs, int flags, char *cBuf){

  int nChars=0;
  int aid, iid;
  
  SpanCharacteristic *Characteristics[numIDs];
  StatusCode status[numIDs];
  boolean sFlag=false;

  for(int i=0;i<numIDs;i++){              // PASS 1: loop over all ids requested to check status codes - only errors are if characteristic not found, or not readable
    sscanf(ids[i],"%d.%d",&aid,&iid);     // parse aid and iid
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
      sscanf(ids[i],"%d.%d",&aid,&iid);     // parse aid and iid                        
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,"{\"iid\":%d,\"aid\":%d}",iid,aid);      // else create JSON attributes based on requested aid/iid
    }
    
    if(sFlag){                                                                                    // status flag is needed - overlay at end
      nChars--;
      nChars+=snprintf(cBuf?(cBuf+nChars):NULL,cBuf?64:0,",\"status\":%d}",status[i]);
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

SpanAccessory::SpanAccessory(){
  
  homeSpan.Accessories.push_back(this);
  aid=homeSpan.Accessories.size();

  homeSpan.configLog+="+Accessory " + String(aid) + "\n";
}

///////////////////////////////

int SpanAccessory::sprintfAttributes(char *cBuf){
  int nBytes=0;

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"aid\":%d,\"services\":[",aid);

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

SpanService::SpanService(const char *type, const char *hapName, char *requiredChars, char *optionalChars){

  this->type=type;
  this->hapName=hapName;

  homeSpan.configLog+="-->Service " + String(hapName);
  
  if(homeSpan.Accessories.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new Service without a defined Accessory! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  homeSpan.configLog+="\n";
  homeSpan.Accessories.back()->Services.push_back(this);  
  iid=++(homeSpan.Accessories.back()->iidCount);  
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
//    SpanCharacteristic     //
///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, char *hapName){
  this->type=type;
  this->perms=perms;
  this->hapName=hapName;

  homeSpan.configLog+="---->Characteristic " + String(hapName);

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new Characteristic without a defined Service! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  homeSpan.configLog+="\n";
  homeSpan.Accessories.back()->Services.back()->Characteristics.push_back(this);  
  iid=++(homeSpan.Accessories.back()->iidCount);
  service=homeSpan.Accessories.back()->Services.back();
  aid=homeSpan.Accessories.back()->aid;

  ev=(boolean *)calloc(homeSpan.maxConnections,sizeof(boolean));
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, boolean value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=BOOL;
  this->value.BOOL=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, int32_t value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=INT;
  this->value.INT=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, uint8_t value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT8;
  this->value.UINT8=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, uint16_t value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT16;
  this->value.UINT16=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, uint32_t value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT32;
  this->value.UINT32=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, uint64_t value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=UINT64;
  this->value.UINT64=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, double value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=FLOAT;
  this->value.FLOAT=value;
}

///////////////////////////////

SpanCharacteristic::SpanCharacteristic(char *type, uint8_t perms, const char* value, char *hapName) : SpanCharacteristic(type, perms, hapName) {
  this->format=STRING;
  this->value.STRING=value;
}

///////////////////////////////

int SpanCharacteristic::sprintfAttributes(char *cBuf, int flags){
  int nBytes=0;

  const char permCodes[][7]={"pr","pw","ev","aa","tw","hd","wr"};

  nBytes+=snprintf(cBuf,cBuf?64:0,"{\"iid\":%d",iid);

  if(flags&GET_TYPE)  
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"type\":\"%s\"",type);

  switch(format){

    case BOOL:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%s",value.BOOL?"true":"false");
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"bool\"");
      break;

    case INT:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%d",value.INT);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"int\"");
      break;

    case UINT8:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%u",value.UINT8);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"uint8\"");
      break;
      
    case UINT16:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%u",value.UINT16);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"uint16\"");
      break;
      
    case UINT32:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%lu",value.UINT32);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"uint32\"");
      break;
      
    case UINT64:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%llu",value.UINT64);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"uint64\"");
      break;
      
    case FLOAT:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":%lg",value.FLOAT);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"float\"");
      break;
      
    case STRING:
      if(perms&PR)
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"value\":\"%s\"",value.STRING);
      if(flags&GET_META)  
        nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"format\":\"string\"");
      break;
      
  } // switch

  if(range && (flags&GET_META)){
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
    nBytes+=snprintf(cBuf?(cBuf+nBytes):NULL,cBuf?64:0,",\"aid\":%d",aid);
  
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
      if(!sscanf(val,"%u",&newValue.UINT8))
        return(StatusCode::InvalidValue);
      break;
            
    case UINT16:
      if(!sscanf(val,"%u",&newValue.UINT16))
        return(StatusCode::InvalidValue);
      break;
      
    case UINT32:
      if(!sscanf(val,"%llu",&newValue.UINT32))
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
      break;

      case INT:
        value.INT=(int)val;
      break;

      case UINT8:
        value.UINT8=(uint8_t)val;
      break;

      case UINT16:
        value.UINT16=(uint16_t)val;
      break;

      case UINT32:
        value.UINT32=(uint32_t)val;
      break;

      case UINT64:
        value.UINT64=(uint64_t)val;
      break;
    }

    updateTime=homeSpan.snapTime;

    SpanBuf sb;                             // create SpanBuf object
    sb.characteristic=this;                 // set characteristic          
    sb.status=StatusCode::OK;               // set status
    sb.val="";                              // set dummy "val" so that sprintfNotify knows to consider this "update"
    homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector
}

///////////////////////////////

void SpanCharacteristic::setVal(double val){
  
    value.FLOAT=(double)val;  
    updateTime=homeSpan.snapTime;

    SpanBuf sb;                             // create SpanBuf object
    sb.characteristic=this;                 // set characteristic          
    sb.status=StatusCode::OK;               // set status
    sb.val="";                              // set dummy "val" so that sprintfNotify knows to consider this "update"
    homeSpan.Notifications.push_back(sb);   // store SpanBuf in Notifications vector
}

///////////////////////////////

int SpanCharacteristic::timeVal(){
  
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

SpanButton::SpanButton(int pin, unsigned long longTime, unsigned long shortTime){

  homeSpan.configLog+="---->SpanButton: Pin " + String(pin);

  if(homeSpan.Accessories.empty() || homeSpan.Accessories.back()->Services.empty()){
    homeSpan.configLog+=" *** ERROR!  Can't create new PushButton without a defined Service! ***\n";
    homeSpan.nFatalErrors++;
    return;
  }

  Serial.print("Configuring PushButton: Pin=");     // initialization message
  Serial.print(pin);
  Serial.print("\n");

  this->pin=pin;
  this->shortTime=shortTime;
  this->longTime=longTime;
  service=homeSpan.Accessories.back()->Services.back();

  if((void(*)(int,boolean))(service->*(&SpanService::button))==(void(*)(int,boolean))(&SpanService::button))
    homeSpan.configLog+=" *** WARNING:  No button() method defined for this PushButton! ***";

  pushButton=new PushButton(pin);         // create underlying PushButton
  
  homeSpan.configLog+="\n";  
  homeSpan.PushButtons.push_back(this);
}

///////////////////////////////
