
#include "Configure.h"

///////////////////////////////

void Configure::processRequest(WiFiClient &client, char *body, char *formData){

  String responseHead="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n";
  
  String responseBody="<html><head><style>p{font-size:300%; margin:25px}label{font-size:300%; margin:25px}input{font-size:250%; margin:25px}</style></head>"
                      "<body style=\"background-color:lightyellow;\"><center><p><b>HomeSpan Setup</b></p></center>";

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
                  "<p>Initiating WiFi connection to:</p><p>" + String(wifiData.ssid) + "</p>";
  
  } else

  if(!strncmp(body,"GET /wifi-status ",17)){                              // GET WIFI-STATUS

    LOG1("In Get WiFi Status...\n");

    if(WiFi.status()!=WL_CONNECTED){
      responseHead+="Refresh: 5\r\n";     
      responseBody+="<p>Re-trying (" + String((millis()-timer)/1000) + "sec) connection to:</p><p>" + String(wifiData.ssid) + "</p>";
    } else {
      responseBody+="<p>SUCCESS! Connected to:</p><p>" + String(wifiData.ssid) + "</p>";      
    }
  
  } else {                                                                // LOGIN PAGE

    LOG1("In Captive Access...\n");

    int n=WiFi.scanNetworks();

    responseBody+="<p>Welcome to HomeSpan! This page allows you to configure the above HomeSpan device to connect to your WiFi network, and (if needed) to create a Setup Code for pairing this device to HomeKit.</p>"
                  "<p>The LED on this device should be <em>double-blinking</em> during this configuration.<p>"
                  "<form action=\"/configure\" method=\"post\">"
                  "<label for=\"ssid\">WiFi Network:</label>"
                  "<input list=\"network\" name=\"network\" placeholder=\"Choose or Type\" required maxlength=" + String(MAX_SSID) + ">"
                  "<datalist id=\"network\">";

    for(int i=0;i<n;i++){
      if(responseBody.indexOf(WiFi.SSID(i))==-1)                                                   // first time this SSID found
        responseBody+="<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }  
    
    responseBody+="</datalist><br><br>"
                  "<label for=\"pwd\">WiFi Password:</label>"
                  "<input type=\"password\" id=\"pwd\" name=\"pwd\" maxlength=" + String(MAX_PWD) + ">"
                  "<br><br>"
                  "<label for=\"code\">Setup Code:</label>"
                  "<input type=\"tel\" id=\"code\" name=\"code\" placeholder=\"12345678\" pattern=\"[0-9]{8}\" maxlength=8>";

  /*
          apClient.print("<p><em>");
          apClient.print("This device already has a Setup Code.  You may leave Setup Code blank to retain the current one, or type a new Setup Code to change.");
          apClient.print("This device does not yet have a Setup Code.  You must create one above.");
          apClient.print("</p>");
*/

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
    
} // processRequest

//////////////////////////////////////

int Configure::getFormValue(char *formData, char *tag, char *value, int maxSize){

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
