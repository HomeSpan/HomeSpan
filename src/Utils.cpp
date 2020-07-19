
#include "Utils.h"

char *Utils::readSerial(char *c, int max){
  int i=0;
  char buf;

  long sTime=millis();
 
  while(1){

    while(!Serial.available()){       // wait until there is a new character
      digitalWrite(LED_BUILTIN,((millis()-sTime)/200)%2);
    }
    
    buf=Serial.read();
    
    if(buf=='\n'){         // exit upon newline
      if(i>0)              // characters have been typed
        c[i]='\0';            // replace newline with string terminator
      return(c);           // return updated string
    }

    c[i]=buf;               // store new character
    
    if(i<max)               // do not store more than max characters (excluding string terminator)
      i++;
  
  } // while(1)

  digitalWrite(LED_BUILTIN,LOW);
  
} // readSerial

//////////////////////////////////////

String Utils::mask(char *c, int n){
  String s="";
  int len=strlen(c);
  
  for(int i=0;i<len;i++){
    if(i<n || i>=len-n)
      s+=c[i];
    else
      s+='*';
  }
  
  return(s);  
} // mask
