
#include <DNSServer.h>

#include "Network.h"
#include "Utils.h"

///////////////////////////////

void Network::configure(char *apName){

  Serial.print("WiFi Configuration required.  Please enter details here, or connect to Access Point: ");
  Serial.print(apName);
  Serial.print("\n");

  WiFiServer apServer(80);

  TempBuffer <uint8_t> tempBuffer(MAX_HTTP+1);
  uint8_t *httpBuf=tempBuffer.buf;
  
  const byte DNS_PORT = 53;
  DNSServer dnsServer;
  IPAddress apIP(192, 168, 4, 1);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(apName,"homespan");             // start access point
  dnsServer.start(DNS_PORT, "*", apIP);       // start DNS server that resolves every request to the address of this device
  apServer.begin();

  boolean needsConfiguration=true;

  while(needsConfiguration){

    dnsServer.processNextRequest();

    if(client=apServer.available()){                    // found a new HTTP client
      LOG2("=======================================\n");
      LOG1("** Access Point Client Connected: (");
      LOG1(millis()/1000);
      LOG1(" sec) ");
      LOG1(client.remoteIP());
      LOG1("\n");
      LOG2("\n");
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

      delay(1);
      client.stop();

      /*
      if(!client){                                      // client disconnected by server
        LOG1("** Disconnecting AP Client (");
        LOG1(millis()/1000);
        LOG1(" sec)\n");
      }
      */
      
      LOG2("\n");

    } // process HAP Client

  }

  while(1);
  
}

///////////////////////////////

void Network::processRequest(char *body, char *formData){

  boolean restart=false;

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
    getFormValue(formData,"code",setupCode,8);
    
    timer=millis();
    WiFi.begin(wifiData.ssid,wifiData.pwd);

    responseBody+="<meta http-equiv = \"refresh\" content = \"5; url = /wifi-status\" />"
                  "<p>Initiating WiFi connection to:</p><p><b>" + String(wifiData.ssid) + "</p>";
    responseBody+="<center><button onclick=\"document.location='/'\">Cancel</button></center>";
  
  } else

  if(!strncmp(body,"GET /confirm-restart ",21)){                          // GET CONFIRM-RESTART 
    responseBody+="<p>Settings saved!</p><p>Restarting HomeSpan in 2 seconds...</p>";
    restart=true;    
  } else

  if(!strncmp(body,"GET /cancel-restart ",20)){                          // GET CANCEL-RESTART 
    responseBody+="<p>Update canceled!</p><p>Restarting HomeSpan in 2 seconds...</p>";
    restart=true;        
  } else

  if(!strncmp(body,"GET /wifi-status ",17)){                              // GET WIFI-STATUS

    LOG1("In Get WiFi Status...\n");

    if(WiFi.status()!=WL_CONNECTED){
      responseHead+="Refresh: 5\r\n";     
      responseBody+="<p>Re-trying (" + String((millis()-timer)/1000) + " seconds) connection to:</p><p><b>" + String(wifiData.ssid) + "</p>";
      responseBody+="<center><button onclick=\"document.location='/'\">Cancel</button></center>";
    } else {
      responseBody+="<p>SUCCESS! Connected to:</p><p><b>" + String(wifiData.ssid) + "</p>";
      responseBody+="<center><button onclick=\"document.location='/confirm-restart'\">SAVE Settings and Re-Start</button></center>"
                    "<center><button onclick=\"document.location='/cancel-restart'\">CANCEL Changes and Re-Start</button></center>";
    }
  
  } else {                                                                // LOGIN PAGE

    LOG1("In Captive Access...\n");

    int n=WiFi.scanNetworks();

    responseBody+="<p>Welcome to HomeSpan! This page allows you to configure the above HomeSpan device to connect to your WiFi network, and (if needed) to create a Setup Code for pairing this device to HomeKit.</p>"
                  "<p>The LED on this device should be <em>double-blinking</em> during this configuration.</p>"
                  "<form action=\"/configure\" method=\"post\">"
                  "<label for=\"ssid\">WiFi Network:</label>"
                  "<center><input size=\"32\" list=\"network\" name=\"network\" placeholder=\"Choose or Type\" required maxlength=" + String(MAX_SSID) + "></center>"
                  "<datalist id=\"network\">";

    for(int i=0;i<n;i++){
      if(responseBody.indexOf(WiFi.SSID(i))==-1)                                                   // first time this SSID found
        responseBody+="<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }  
    
    responseBody+="</datalist><br><br>"
                  "<label for=\"pwd\">WiFi Password:</label>"
                  "<center><input size=\"32\" type=\"password\" id=\"pwd\" name=\"pwd\" maxlength=" + String(MAX_PWD) + "></center>"
                  "<br><br>"
                  "<label for=\"code\">Setup Code:</label>"
                  "<center><input size=\"32\" type=\"tel\" id=\"code\" name=\"code\" placeholder=\"12345678\" pattern=\"[0-9]{8}\" maxlength=8></center>";

    responseBody+="<center><input style=\"font-size:300%\" type=\"submit\" value=\"SUBMIT\"></center>"
                  "</form>";
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

  if(restart){
    //delay(2000);
    //ESP.restart();
  }
    
} // processRequest

//////////////////////////////////////

int Network::getFormValue(char *formData, char *tag, char *value, int maxSize){

  char *s=strstr(formData,tag);     // find start of tag
  
  if(!s)                            // if not found, return -1
    return(-1);

  char *v=index(s,'=');             // find '='

  if(!v)                            // if not found, return -1 (this should not happen)
    return(-1);

  v++;                              // point to begining of value 
  int len=0;                        // track length of value
  
  while(*v!='\0' && *v!='&' && len<maxSize){      // copy the value until null, '&', or maxSize is reached
    len++;
    *value++=*v++;
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
