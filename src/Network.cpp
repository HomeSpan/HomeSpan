/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2022 Gregg E. Berman
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
 
#include <DNSServer.h>

#include "Network.h"
#include "HomeSpan.h"
#include "Utils.h"

using namespace Utils;

///////////////////////////////

void Network::scan(){

  int n=WiFi.scanNetworks();

  free(ssidList);
  ssidList=(char **)calloc(n,sizeof(char *));
  numSSID=0;

  for(int i=0;i<n;i++){
    boolean found=false;
    for(int j=0;j<numSSID;j++){
      if(!strcmp(WiFi.SSID(i).c_str(),ssidList[j]))
        found=true;
    }
    if(!found){
      ssidList[numSSID]=(char *)calloc(WiFi.SSID(i).length()+1,sizeof(char));
      sprintf(ssidList[numSSID],"%s",WiFi.SSID(i).c_str());
      numSSID++;
    }
  }

}

///////////////////////////////

void Network::serialConfigure(){

  wifiData.ssid[0]='\0';
  wifiData.pwd[0]='\0';

  Serial.print("*** WiFi Setup - Scanning for Networks...\n\n");
  
  scan();         // scan for networks    

  for(int i=0;i<numSSID;i++){
    Serial.print("  ");
    Serial.print(i+1);
    Serial.print(") ");
    Serial.print(ssidList[i]);
    Serial.print("\n");
  }
         
  while(!strlen(wifiData.ssid)){
    Serial.print("\n>>> WiFi SSID: ");
    readSerial(wifiData.ssid,MAX_SSID);
    if(atoi(wifiData.ssid)>0 && atoi(wifiData.ssid)<=numSSID){
      strcpy(wifiData.ssid,ssidList[atoi(wifiData.ssid)-1]);
    }
    Serial.print(wifiData.ssid);
    Serial.print("\n");
  }
  
  while(!strlen(wifiData.pwd)){
    Serial.print(">>> WiFi PASS: ");
    readSerial(wifiData.pwd,MAX_PWD);    
    Serial.print(mask(wifiData.pwd,2));
    Serial.print("\n");
  }

  return;
}

///////////////////////////////

boolean Network::allowedCode(char *s){
  return(
    strcmp(s,"00000000") && strcmp(s,"11111111") && strcmp(s,"22222222") && strcmp(s,"33333333") && 
    strcmp(s,"44444444") && strcmp(s,"55555555") && strcmp(s,"66666666") && strcmp(s,"77777777") &&
    strcmp(s,"88888888") && strcmp(s,"99999999") && strcmp(s,"12345678") && strcmp(s,"87654321"));
}

///////////////////////////////

void Network::apConfigure(){

  Serial.print("*** Starting Access Point: ");
  Serial.print(apSSID);
  Serial.print(" / ");
  Serial.print(apPassword);
  Serial.print("\n");

  homeSpan.statusLED.start(LED_AP_STARTED);

  Serial.print("\nScanning for Networks...\n\n");
  
  scan();                   // scan for networks    

  for(int i=0;i<numSSID;i++){
    Serial.print("  ");
    Serial.print(i+1);
    Serial.print(") ");
    Serial.print(ssidList[i]);
    Serial.print("\n");
  }  

  WiFiServer apServer(80);
  client=0;
  
  TempBuffer <uint8_t> tempBuffer(MAX_HTTP+1);
  uint8_t *httpBuf=tempBuffer.buf;
  
  const byte DNS_PORT = 53;
  DNSServer dnsServer;
  IPAddress apIP(192, 168, 4, 1);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID,apPassword);             // start access point
  dnsServer.start(DNS_PORT, "*", apIP);       // start DNS server that resolves every request to the address of this device
  apServer.begin();

  alarmTimeOut=millis()+lifetime;            // Access Point will shut down when alarmTimeOut is reached
  apStatus=0;                                // status will be "timed out" unless changed

  Serial.print("\nReady.\n");

  while(1){                                  // loop until we get timed out (which will be accelerated if save/cancel selected)

    if(homeSpan.controlButton && homeSpan.controlButton->triggered(9999,3000)){
      Serial.print("\n*** Access Point Terminated.");
      homeSpan.statusLED.start(LED_ALERT);
      homeSpan.controlButton->wait();
      Serial.print("  Restarting... \n\n");
      homeSpan.statusLED.off();        
      ESP.restart();      
    }

    if(millis()>alarmTimeOut){
      WiFi.softAPdisconnect(true);           // terminate connections and shut down captive access point
      delay(100);
      if(apStatus==1){
        Serial.print("\n*** Access Point: Exiting and Saving Settings\n\n");
        return;
      } else {
        if(apStatus==0){
          Serial.print("\n*** Access Point: Timed Out (");
          Serial.print(lifetime/1000);
          Serial.print(" seconds).");
        } else {
          Serial.print("\n*** Access Point: Configuration Canceled.");
        }
        Serial.print("  Restarting...\n\n");
        homeSpan.statusLED.start(LED_ALERT);
        delay(1000);
        homeSpan.statusLED.off();        
        ESP.restart();
      }
    }

    dnsServer.processNextRequest();

    if(client=apServer.available()){                    // found a new HTTP client
      LOG2("=======================================\n");
      LOG1("** Access Point Client Connected: (");
      LOG1(millis()/1000);
      LOG1(" sec) ");
      LOG1(client.remoteIP());
      LOG1("\n");
      LOG2("\n");
      delay(50);                                        // pause to allow data buffer to begin to populate
    }
    
    if(client && client.available()){                   // if connection exists and data is available

      LOG2("<<<<<<<<< ");
      LOG2(client.remoteIP());
      LOG2(" <<<<<<<<<\n");
    
      int nBytes=client.read(httpBuf,MAX_HTTP+1);       // read all available bytes up to maximum allowed+1
       
      if(nBytes>MAX_HTTP){                              // exceeded maximum number of bytes allowed
        badRequestError();
        Serial.print("\n*** ERROR:  Exceeded maximum HTTP message length\n\n");
        continue;
      }

      httpBuf[nBytes]='\0';                             // add null character to enable string functions    
      char *body=(char *)httpBuf;                       // char pointer to start of HTTP Body
      char *p;                                          // char pointer used for searches
      
      if(!(p=strstr((char *)httpBuf,"\r\n\r\n"))){
        badRequestError();
        Serial.print("\n*** ERROR:  Malformed HTTP request (can't find blank line indicating end of BODY)\n\n");
        continue;      
      }

      *p='\0';                                          // null-terminate end of HTTP Body to faciliate additional string processing
      uint8_t *content=(uint8_t *)p+4;                  // byte pointer to start of optional HTTP Content
      int cLen=0;                                       // length of optional HTTP Content

      if((p=strstr(body,"Content-Length: ")))           // Content-Length is specified
        cLen=atoi(p+16);
      if(nBytes!=strlen(body)+4+cLen){
        badRequestError();
        Serial.print("\n*** ERROR:  Malformed HTTP request (Content-Length plus Body Length does not equal total number of bytes read)\n\n");
        continue;        
      }

      LOG2(body);
      LOG2("\n------------ END BODY! ------------\n");

      content[cLen]='\0';                               // add a trailing null on end of any contents, which should always be text-based

      processRequest(body, (char *)content);            // process request
      
      LOG2("\n");

    } // process Client

  } // while 1

}

///////////////////////////////

void Network::processRequest(char *body, char *formData){
  
  String responseHead="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n";
  
  String responseBody="<html><head><style>"
                        "p{font-size:300%; margin:1em}"
                        "label{font-size:300%; margin:1em}"
                        "input{font-size:250%; margin:1em}"
                        "button{font-size:250%; margin:1em}"
                      "</style></head>"
                      "<body style=\"background-color:lightyellow;\">"
                      "<center><p><b>HomeSpan Setup</b></p></center>";

  if(!strncmp(body,"POST /configure ",16) &&                              // POST CONFIGURE
     strstr(body,"Content-Type: application/x-www-form-urlencoded")){     // check that content is from a form

    LOG2(formData);                                                       // print form data
    LOG2("\n------------ END DATA! ------------\n");
               
    LOG1("In Post Configure...\n");

    getFormValue(formData,"network",wifiData.ssid,MAX_SSID);
    getFormValue(formData,"pwd",wifiData.pwd,MAX_PWD);
    
    homeSpan.statusLED.start(LED_WIFI_CONNECTING);

    responseBody+="<meta http-equiv = \"refresh\" content = \"" + String(waitTime) + "; url = /wifi-status\" />"
                  "<p>Initiating WiFi connection to:</p><p><b>" + String(wifiData.ssid) + "</p>";

    WiFi.begin(wifiData.ssid,wifiData.pwd);              
  
  } else

  if(!strncmp(body,"POST /save ",11)){                                    // GET SAVE
    getFormValue(formData,"code",setupCode,8);

    if(allowedCode(setupCode)){
      responseBody+="<p><b>Settings saved!</b></p><p>Restarting HomeSpan.</p><p>Closing window...</p>";
      alarmTimeOut=millis()+2000;
      apStatus=1;
      
    } else {
    responseBody+="<meta http-equiv = \"refresh\" content = \"4; url = /wifi-status\" />"
                  "<p><b>Disallowed Setup Code - too simple!</b></p><p>Returning to configuration page...</p>";      
    }
    
  } else

  if(!strncmp(body,"GET /cancel ",12)){                                   // GET CANCEL
    responseBody+="<p><b>Configuration Canceled!</b></p><p>Restarting HomeSpan.</p><p>Closing window...</p>";
    alarmTimeOut=millis()+2000;
    apStatus=-1;
  } else

  if(!strncmp(body,"GET /wifi-status ",17)){                              // GET WIFI-STATUS

    LOG1("In Get WiFi Status...\n");

    if(WiFi.status()!=WL_CONNECTED){
      waitTime+=2;
      if(waitTime==12)
        waitTime=2;
      responseHead+="Refresh: " + String(waitTime) + "\r\n";     
      responseBody+="<p>Re-initiating connection to:</p><p><b>" + String(wifiData.ssid) + "</b></p>";
      responseBody+="<p>(waiting " + String(waitTime) + " seconds to check for response)</p>";
      responseBody+="<p>Access Point termination in " + String((alarmTimeOut-millis())/1000) + " seconds.</p>";
      responseBody+="<center><button onclick=\"document.location='/hotspot-detect.html'\">Cancel</button></center>";
      WiFi.begin(wifiData.ssid,wifiData.pwd);
      
    } else {
      
      homeSpan.statusLED.start(LED_AP_CONNECTED);   // slow double-blink
      
      responseBody+="<p>SUCCESS! Connected to:</p><p><b>" + String(wifiData.ssid) + "</b></p>";
      responseBody+="<p>You may enter new 8-digit Setup Code below, or leave blank to retain existing code.</p>";

      responseBody+="<form action=\"/save\" method=\"post\">"
                    "<label for=\"code\">Setup Code:</label>"
                    "<center><input size=\"32\" type=\"tel\" id=\"code\" name=\"code\" placeholder=\"12345678\" pattern=\"[0-9]{8}\" maxlength=8></center>"
                    "<center><input style=\"font-size:300%\" type=\"submit\" value=\"SAVE Settings\"></center>"
                    "</form>";
                    
      responseBody+="<center><button style=\"font-size:300%\" onclick=\"document.location='/cancel'\">CANCEL Configuration</button></center>";
    }
  
  } else                                                                

  if(!strstr(body,"wispr") && !strncmp(body,"GET /hotspot-detect.html ",25)){                             // GET LANDING-PAGE, but only if request does NOT contain "wispr" user agent

    LOG1("In Landing Page...\n");

    homeSpan.statusLED.start(LED_AP_CONNECTED);
    waitTime=2;

    responseBody+="<p>Welcome to HomeSpan! This page allows you to configure the above HomeSpan device to connect to your WiFi network.</p>"
                  "<p>The LED on this device should be <em>double-blinking</em> during this configuration.</p>"
                  "<form action=\"/configure\" method=\"post\">"
                  "<label for=\"ssid\">WiFi Network:</label>"
                  "<center><input size=\"32\" list=\"network\" name=\"network\" placeholder=\"Choose or Type\" required maxlength=" + String(MAX_SSID) + "></center>"
                  "<datalist id=\"network\">";

    for(int i=0;i<numSSID;i++)
        responseBody+="<option value=\"" + String(ssidList[i]) + "\">" + String(ssidList[i]) + "</option>";  
    
    responseBody+="</datalist><br><br>"
                  "<label for=\"pwd\">WiFi Password:</label>"
                  "<center><input size=\"32\" type=\"password\" id=\"pwd\" name=\"pwd\" required maxlength=" + String(MAX_PWD) + "></center>"
                  "<br><br>";
                  
    responseBody+="<center><input style=\"font-size:300%\" type=\"submit\" value=\"SUBMIT\"></center>"
                  "</form>";

    responseBody+="<center><button style=\"font-size:300%\" onclick=\"document.location='/cancel'\">CANCEL Configuration</button></center>";                  
                  
  }

  responseHead+="\r\n";               // add blank line between reponse header and body
  responseBody+="</body></html>";     // close out body and html tags

  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(responseHead);
  LOG2(responseBody);
  LOG2("\n");
  client.print(responseHead);
  client.print(responseBody);
  LOG2("------------ SENT! --------------\n");
    
} // processRequest

//////////////////////////////////////

int Network::getFormValue(char *formData, const char *tag, char *value, int maxSize){

  char *s=strstr(formData,tag);     // find start of tag
  
  if(!s)                            // if not found, return -1
    return(-1);

  char *v=index(s,'=');             // find '='

  if(!v)                            // if not found, return -1 (this should not happen)
    return(-1);

  v++;                              // point to begining of value 
  int len=0;                        // track length of value
  
  while(*v!='\0' && *v!='&' && len<maxSize){      // copy the value until null, '&', or maxSize is reached
    if(*v=='%'){                                  // this is an escaped character of form %XX
      v++;
      sscanf(v,"%2x",(unsigned int *)value++);
      v+=2;
    } else {
      *value++=(*v=='+'?' ':*v);                  // HTML Forms use '+' for spaces (and '+' signs are escaped)
      v++;
    }
    len++;
  }

  *value='\0';                      // add terminating null
  return(len);
  
}

//////////////////////////////////////

int Network::badRequestError(){

  char s[]="HTTP/1.1 400 Bad Request\r\n\r\n";
  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(s);
  client.print(s);
  LOG2("------------ SENT! --------------\n");
  
  delay(1);
  client.stop();

  return(-1);
}

//////////////////////////////////////
