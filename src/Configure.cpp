
#include "Configure.h"
#include "Settings.h"

///////////////////////////////

void Configure::processRequest(WiFiClient &client, char *body, char *formData){

  String s;
  String header;

  header+="<html><head><style>p{font-size:300%; margin:25px}label{font-size:300%; margin:25px}input{font-size:250%; margin:25px}</style></head>";
  header+="<body style=\"background-color:lightyellow;\"><center><p><b>HomeSpan Setup</b></p></center>";

  if(!strncmp(body,"POST /configure ",16) &&                              // POST CONFIGURE
     strstr(body,"Content-Type: application/x-www-form-urlencoded")){     // check that content is from a form

    LOG2(formData);                                                       // print form data
    LOG2("\n------------ END DATA! ------------\n");
               
    LOG1("In Post Configure...\n");

    s+="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n" + header;
    s+="<meta http-equiv = \"refresh\" content = \"2; url = /wifi-status\" />";    
    s+="<p>Initiating WiFi Connection...</p></body></html>";
  
  } else

  if(!strncmp(body,"GET /wifi-status ",17)){                              // GET WIFI-STATUS

    LOG1("In Get WiFi Status...\n");

    s+="HTTP/1.1 200 OK\r\nContent-type: text/html\r\nRefresh: 5\r\n\r\n" + header;
    s+="<p>Trying to Connect (";
    s+=String(millis()/1000) + "sec)...</p></body></html>";
  
  } else {                                                                // LOGIN PAGE

    LOG1("In Captive Access...\n");

    int n=WiFi.scanNetworks();

    s+="HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n" + header;
    s+="<p>Welcome to HomeSpan! This page allows you to configure the above HomeSpan device to connect to your WiFi network, and (if needed) to create a Setup Code for pairing this device to HomeKit.</p>";
    s+="<p>The LED on this device should be <em>double-blinking</em> during this configuration.<p>";

    s+="<form action=\"/configure\" method=\"post\">";
    s+="<label for=\"ssid\">WiFi Network:</label>";
    s+="<input list=\"network\" name=\"network\" placeholder=\"Choose or Type\" required>";
    s+="<datalist id=\"network\">";

    for(int i=0;i<n;i++){
      if(s.indexOf(WiFi.SSID(i))==-1)                                                   // first time this SSID founx
        s+="<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }  
    s+="</datalist><br><br>";
  
    s+="<label for=\"pwd\">WiFi Password:</label>";
    s+="<input type=\"password\" id=\"pwd\" name=\"pwd\">";
    s+="<br><br>";

    s+="<label for=\"code\">Setup Code:</label>";
    s+="<input type=\"tel\" id=\"code\" name=\"code\" placeholder=\"12345678\" pattern=\"[0-9]{8}\">";

  /*
          apClient.print("<p><em>");
          apClient.print("This device already has a Setup Code.  You may leave Setup Code blank to retain the current one, or type a new Setup Code to change.");
          apClient.print("This device does not yet have a Setup Code.  You must create one above.");
          apClient.print("</p>");
*/

    s+="<center><input style=\"font-size:300%\" type=\"submit\" value=\"SUBMIT\"></center>";
    s+="</form></body></html>";

    
  }

  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(s);
  LOG2("\n");
  client.print(s);
  LOG2("------------ SENT! --------------\n");
    
} // processRequest

//////////////////////////////////////
