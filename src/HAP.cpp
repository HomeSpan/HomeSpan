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
#include <sodium.h>
#include <MD5Builder.h>
#include <mbedtls/version.h>

#include "HAP.h"

//////////////////////////////////////

void HAPClient::init(){

  size_t len;                                   // not used but required to read blobs from NVS

  if(strlen(homeSpan.spanOTA.otaPwd)==0){                                 // OTA password has not been specified in sketch
    if(!nvs_get_str(homeSpan.otaNVS,"OTADATA",NULL,&len)){                // if found OTA data in NVS...
    nvs_get_str(homeSpan.otaNVS,"OTADATA",homeSpan.spanOTA.otaPwd,&len);  // ...retrieve data.
    } else {                                                              // otherwise...
    homeSpan.spanOTA.setPassword(DEFAULT_OTA_PASSWORD);                   // ...use default password
    }
  }
  
  if(!strlen(homeSpan.qrID)){                                             // if Setup ID has not been specified in sketch
    if(!nvs_get_str(homeSpan.hapNVS,"SETUPID",NULL,&len)){                // check for saved value
      nvs_get_str(homeSpan.hapNVS,"SETUPID",homeSpan.qrID,&len);          // retrieve data
    } else {
      sprintf(homeSpan.qrID,"%s",DEFAULT_QR_ID);                          // use default
   }
  }

  if(nvs_get_blob(homeSpan.srpNVS,"VERIFYDATA",NULL,&len))                // if Pair-Setup verification code data not found in NVS
    homeSpan.setPairingCode(DEFAULT_SETUP_CODE,false);                    // create and save verification from default Pairing Setup Code 
  
  if(!nvs_get_blob(homeSpan.hapNVS,"ACCESSORY",NULL,&len)){               // if found long-term Accessory data in NVS
    nvs_get_blob(homeSpan.hapNVS,"ACCESSORY",&accessory,&len);            // retrieve data
  } else {      
    LOG0("Generating new random Accessory ID and Long-Term Ed25519 Signature Keys...\n\n");
    uint8_t buf[6];
    char cBuf[18];
    
    randombytes_buf(buf,6);                                              // generate 6 random bytes using libsodium (which uses the ESP32 hardware-based random number generator)
    sprintf(cBuf,"%02X:%02X:%02X:%02X:%02X:%02X",                        // create ID in form "XX:XX:XX:XX:XX:XX" (HAP Table 6-7)
      buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);

    memcpy(accessory.ID,cBuf,17);                                        // copy into Accessory ID for permanent storage
    crypto_sign_keypair(accessory.LTPK,accessory.LTSK);                  // generate new random set of keys using libsodium public-key signature
    
    nvs_set_blob(homeSpan.hapNVS,"ACCESSORY",&accessory,sizeof(accessory));       // update data
    nvs_commit(homeSpan.hapNVS);                                                  // commit to NVS
  }

  if(!nvs_get_blob(homeSpan.hapNVS,"CONTROLLERS",NULL,&len)){            // if found long-term Controller Pairings data from NVS
    TempBuffer<Controller> tBuf(len/sizeof(Controller));
    nvs_get_blob(homeSpan.hapNVS,"CONTROLLERS",tBuf,&len);               // retrieve data
    for(int i=0;i<tBuf.size();i++){
      if(tBuf[i].allocated)
        controllerList.push_back(tBuf[i]);
    }
  }
  
  LOG0("Accessory ID:      ");
  charPrintRow(accessory.ID,17);
  LOG0("                               LTPK: ");
  hexPrintRow(accessory.LTPK,32);
  LOG0("\n");

  printControllers();                                                         

  if(!nvs_get_blob(homeSpan.hapNVS,"HAPHASH",NULL,&len)){                 // if found HAP HASH structure
    nvs_get_blob(homeSpan.hapNVS,"HAPHASH",&homeSpan.hapConfig,&len);     // retrieve data    
  } else {
    LOG0("Resetting Database Hash...\n");
    nvs_set_blob(homeSpan.hapNVS,"HAPHASH",&homeSpan.hapConfig,sizeof(homeSpan.hapConfig));     // save data (will default to all zero values, which will then be updated below)
    nvs_commit(homeSpan.hapNVS);                                                                // commit to NVS
  }

  if(homeSpan.updateDatabase(false)){       // create Configuration Number and Loop vector
    LOG0("\nAccessory configuration has changed.  Updating configuration number to %d\n",homeSpan.hapConfig.configNumber);
  }
  else{
    LOG0("\nAccessory configuration number: %d\n",homeSpan.hapConfig.configNumber);
  }

  LOG0("\n");

}

//////////////////////////////////////

void HAPClient::processRequest(){

  int nBytes, messageSize;

  messageSize=client.available();        

  if(messageSize>MAX_HTTP){                         // exceeded maximum number of bytes allowed
    badRequestError();
    LOG0("\n*** ERROR:  HTTP message of %d bytes exceeds maximum allowed (%d)\n\n",messageSize,MAX_HTTP);
    return;
  }
 
  TempBuffer<uint8_t> httpBuf(messageSize+1);      // leave room for null character added below
  
  if(cPair){                                       // expecting encrypted message
    LOG2("<<<< #### ");
    LOG2(client.remoteIP());
    LOG2(" #### <<<<\n");

    nBytes=receiveEncrypted(httpBuf,messageSize);  // decrypt and return number of bytes read      
        
    if(!nBytes){                                   // decryption failed (error message already printed in function)
      badRequestError();              
      return;          
    }
        
  } else {                                         // expecting plaintext message  
    LOG2("<<<<<<<<< ");
    LOG2(client.remoteIP());
    LOG2(" <<<<<<<<<\n");
    
    nBytes=client.read(httpBuf,messageSize);       // read expected number of bytes

    if(nBytes!=messageSize || client.available()!=0){
      badRequestError();
      LOG0("\n*** ERROR:  HTTP message not read correctly.  Expected %d bytes, read %d bytes, %d bytes remaining\n\n",messageSize,nBytes,client.available());
      return;
    }
               
  } // encrypted/plaintext
      
  httpBuf[nBytes]='\0';   // add null character to enable string functions
      
  char *body=(char *)httpBuf.get();   // char pointer to start of HTTP Body
  char *p;                            // char pointer used for searches
     
  if(!(p=strstr((char *)httpBuf.get(),"\r\n\r\n"))){
    badRequestError();
    LOG0("\n*** ERROR:  Malformed HTTP request (can't find blank line indicating end of BODY)\n\n");
    return;      
  }

  *p='\0';                            // null-terminate end of HTTP Body to faciliate additional string processing
  uint8_t *content=(uint8_t *)p+4;    // byte pointer to start of optional HTTP Content
  int cLen=0;                         // length of optional HTTP Content

  if((p=strstr(body,"Content-Length: ")))       // Content-Length is specified
    cLen=atoi(p+16);
  if(nBytes!=strlen(body)+4+cLen){
    badRequestError();
    LOG0("\n*** ERROR:  Malformed HTTP request (Content-Length plus Body Length does not equal total number of bytes read)\n\n");
    return;        
  }

  LOG2(body);
  LOG2("\n------------ END BODY! ------------\n");

  if(!strncmp(body,"POST ",5)){                                                                                        // this is a POST request

    if(cLen==0){
      badRequestError();
      LOG0("\n*** ERROR:  HTTP POST request contains no Content\n\n");
    }
           
    else if(!strncmp(body,"POST /pair-setup ",17) && strstr(body,"Content-Type: application/pairing+tlv8"))            // POST PAIR-SETUP               
      postPairSetupURL(content,cLen);

    else if(!strncmp(body,"POST /pair-verify ",18) && strstr(body,"Content-Type: application/pairing+tlv8"))           // POST PAIR-VERIFY 
      postPairVerifyURL(content,cLen);
            
    else if(!strncmp(body,"POST /pairings ",15) && strstr(body,"Content-Type: application/pairing+tlv8"))              // POST PAIRINGS                
      postPairingsURL(content,cLen);

    else {
      notFoundError();
      LOG0("\n*** ERROR:  Bad POST request - URL not found\n\n");
    }
    
    return;                          
  } // POST request
          
  if(!strncmp(body,"PUT ",4)){                                                                                         // this is a PUT request

    if(cLen==0){
      badRequestError();
      LOG0("\n*** ERROR:  HTTP PUT request contains no Content\n\n");
      return;
    }

    LOG2((char *)content);
    LOG2("\n------------ END JSON! ------------\n");    
           
    if(!strncmp(body,"PUT /characteristics ",21) && strstr(body,"Content-Type: application/hap+json"))                 // PUT CHARACTERISTICS              
      putCharacteristicsURL((char *)content);

    else if(!strncmp(body,"PUT /prepare ",13) && strstr(body,"Content-Type: application/hap+json"))                    // PUT PREPARE
      putPrepareURL((char *)content);

    else {
      notFoundError();
      LOG0("\n*** ERROR:  Bad PUT request - URL not found\n\n");
    }
    
    return;                  
        
  } // PUT request           
      
  if(!strncmp(body,"GET ",4)){                                                                                         // this is a GET request

    int refreshTime;
                    
    if(!strncmp(body,"GET /accessories ",17))                                                                          // GET ACCESSORIES
      getAccessoriesURL();

    else if(!strncmp(body,"GET /characteristics?",21))                                                                 // GET CHARACTERISTICS
      getCharacteristicsURL(body+21);

    else if(homeSpan.webLog.isEnabled && (refreshTime=homeSpan.webLog.check(body+4))>=0)                               // OPTIONAL (NON-HAP) STATUS REQUEST
      getStatusURL(this,NULL,NULL,refreshTime);

    else {
      notFoundError();
      LOG0("\n*** ERROR:  Bad GET request - URL not found\n\n");
    }
    
    return;                  

  } // GET request
      
  badRequestError();
  LOG0("\n*** ERROR:  Unknown or malformed HTTP request\n\n");
                        
} // processHAP

//////////////////////////////////////

int HAPClient::notFoundError(){

  char s[]="HTTP/1.1 404 Not Found\r\n\r\n";
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

int HAPClient::badRequestError(){

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

int HAPClient::unauthorizedError(){

  char s[]="HTTP/1.1 470 Connection Authorization Required\r\n\r\n";
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

int HAPClient::postPairSetupURL(uint8_t *content, size_t len){

  static SRP6A *srp=NULL;   // must persist across multiple calls to postPairSetupURL

  HAPTLV iosTLV;
  HAPTLV responseTLV;
  HAPTLV subTLV;

  iosTLV.unpack(content,len);
  if(homeSpan.getLogLevel()>1)
    iosTLV.print();
  LOG2("------------ END TLVS! ------------\n");

  LOG1("In Pair Setup #%d (%s)...",clientNumber,client.remoteIP().toString().c_str());
  
  auto itState=iosTLV.find(kTLVType_State);

  if(iosTLV.len(itState)!=1){                                   // missing STATE TLV
    LOG0("\n*** ERROR: Missing or invalid 'State' TLV\n\n");
    badRequestError();                                          // return with 400 error, which closes connection      
    return(0);
  }

  int tlvState=itState->getVal();

  if(nAdminControllers()){                                  // error: Device already paired (i.e. there is at least one admin Controller). We should not be receiving any requests for Pair-Setup!
    LOG0("\n*** ERROR: Device already paired!\n\n");
    responseTLV.add(kTLVType_State,tlvState+1);             // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    responseTLV.add(kTLVType_Error,tagError_Unavailable);   // set Error=Unavailable
    tlvRespond(responseTLV);                                // send response to client
    return(0);
  };

  LOG1("Found <M%d>.  Expected <M%d>.\n",tlvState,pairStatus);

  if(tlvState!=pairStatus){                                         // error: Device is not yet paired, but out-of-sequence pair-setup STATE was received
    LOG0("\n*** ERROR: Out-of-Sequence Pair-Setup request!\n\n");
    responseTLV.add(kTLVType_State,tlvState+1);                     // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    responseTLV.add(kTLVType_Error,tagError_Unknown);               // set Error=Unknown (there is no specific error type for out-of-sequence steps)
    tlvRespond(responseTLV);                                        // send response to client
    pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired accessory (M1)
    return(0);
  };
   
  switch(tlvState){                                         // valid and in-sequence Pair-Setup STATE received -- process request!  (HAP Section 5.6)

    case pairState_M1:{                                     // 'SRP Start Request'

      responseTLV.add(kTLVType_State,pairState_M2);                             // set State=<M2>

      auto itMethod=iosTLV.find(kTLVType_Method);

      if(iosTLV.len(itMethod)!=1 || itMethod->getVal()!=0){                     // error: "Pair Setup" method must always be 0 to indicate setup without MiFi Authentification (HAP Table 5-3)
        LOG0("\n*** ERROR: Pair 'Method' missing or not set to 0\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unavailable);                   // set Error=Unavailable
        tlvRespond(responseTLV);                                                // send response to client
        return(0);
      };

      auto itPublicKey=responseTLV.add(kTLVType_PublicKey,384,NULL);                // create blank PublicKey TLV with space for 384 bytes

      if(srp==NULL)                                                                 // create instance of SRP (if not already created) to persist until Pairing-Setup M5 completes
        srp=new SRP6A;
        
      TempBuffer<Verification> verifyData;                                          // retrieve verification data (should already be stored in NVS)
      size_t len=verifyData.len();
      nvs_get_blob(homeSpan.srpNVS,"VERIFYDATA",verifyData,&len);

      responseTLV.add(kTLVType_Salt,16,verifyData.get()->salt);                     // write Salt from verification data into TLV
      
      srp->createPublicKey(verifyData,*itPublicKey);                                // create accessory Public Key from stored verification data and write result into PublicKey TLV
      
      tlvRespond(responseTLV);                                                      // send response to client
      pairStatus=pairState_M3;                                                      // set next expected pair-state request from client
      return(1);
    } 
    break;

    case pairState_M3:{                                     // 'SRP Verify Request'

      responseTLV.add(kTLVType_State,pairState_M4);                     // set State=<M4>

      auto itPublicKey=iosTLV.find(kTLVType_PublicKey);
      auto itClientProof=iosTLV.find(kTLVType_Proof);

      if(iosTLV.len(itPublicKey)<=0 || iosTLV.len(itClientProof)!=64){
        LOG0("\n*** ERROR: One or both of the required 'PublicKey' and 'Proof' TLV records for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unknown);               // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                                        // send response to client
        pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired
        return(0);
      };

      srp->createSessionKey(*itPublicKey,itPublicKey->getLen());              // create session key, K, from client Public Key, A

      if(!srp->verifyClientProof(*itClientProof)){                            // verify client Proof, M1
        LOG0("\n*** ERROR: SRP Proof Verification Failed\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);              // set Error=Authentication
        tlvRespond(responseTLV);                                              // send response to client
        pairStatus=pairState_M1;                                              // reset pairStatus to first step of unpaired
        return(0);        
      };

      auto itAccProof=responseTLV.add(kTLVType_Proof,64,NULL);                // create blank accessory Proof TLV with space for 64 bytes

      srp->createAccProof(*itAccProof);                                       // M1 has been successully verified; now create accessory Proof M2

      tlvRespond(responseTLV);                                                // send response to client
      pairStatus=pairState_M5;                                                // set next expected pair-state request from client
     
      return(1);        
    }
    break;
    
    case pairState_M5:{                                     // 'Exchange Request'

      responseTLV.add(kTLVType_State,pairState_M6);                     // set State=<M6>

      auto itEncryptedData=iosTLV.find(kTLVType_EncryptedData);

      if(iosTLV.len(itEncryptedData)<=0){            
        LOG0("\n*** ERROR: Required 'EncryptedData' TLV record for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unknown);               // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                                        // send response to client
        pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired
        return(0);
      };

      // THIS NEXT STEP IS MISSING FROM HAP DOCUMENTATION!
      //
      // Must FIRST use HKDF to create a Session Key from the SRP Shared Secret for use in subsequent ChaCha20-Poly1305 decryption
      // of the encrypted data TLV (HAP Sections 5.6.5.2 and 5.6.6.1).
      //
      // Note the SALT and INFO text fields used by HKDF to create this Session Key are NOT the same as those for creating iosDeviceX.
      // The iosDeviceX HKDF calculations are separate and will be performed further below with the SALT and INFO as specified in the HAP docs.

      HKDF::create(temp.sessionKey,srp->K,64,"Pair-Setup-Encrypt-Salt","Pair-Setup-Encrypt-Info");               // create SessionKey

      LOG2("------- DECRYPTING SUB-TLVS -------\n");
      
      // use SessionKey to decrypt encryptedData TLV with padded nonce="PS-Msg05"
                                  
      TempBuffer<uint8_t> decrypted(itEncryptedData->getLen()-crypto_aead_chacha20poly1305_IETF_ABYTES);  // temporary storage for decrypted data
       
      if(crypto_aead_chacha20poly1305_ietf_decrypt(decrypted, NULL, NULL, *itEncryptedData, itEncryptedData->getLen(), NULL, 0, (unsigned char *)"\x00\x00\x00\x00PS-Msg05", temp.sessionKey)==-1){          
        LOG0("\n*** ERROR: Exchange-Request Authentication Failed\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);        // set Error=Authentication
        tlvRespond(responseTLV);                                        // send response to client
        pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired
        return(0);        
      }

      subTLV.unpack(decrypted,decrypted.len());                         // unpack TLV      
      if(homeSpan.getLogLevel()>1)
        subTLV.print();                                                 // print decrypted TLV data
      
      LOG2("---------- END SUB-TLVS! ----------\n");

      auto itIdentifier=subTLV.find(kTLVType_Identifier);
      auto itSignature=subTLV.find(kTLVType_Signature);
      auto itPublicKey=subTLV.find(kTLVType_PublicKey);

      if(subTLV.len(itIdentifier)!=hap_controller_IDBYTES || subTLV.len(itSignature)!=crypto_sign_BYTES || subTLV.len(itPublicKey)!=crypto_sign_PUBLICKEYBYTES){ 
        LOG0("\n*** ERROR: One or more of required 'Identifier,' 'PublicKey,' and 'Signature' TLV records for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unknown);               // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                                        // send response to client
        pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired
        return(0);
      };

      // Next, verify the authenticity of the TLV Records using the Signature provided by the Client.
      // But the Client does not send the entire message that was used to generate the Signature.
      // Rather, it purposely does not transmit "iosDeviceX", which is derived from the SRP Shared Secret that only the Client and this Server know.
      // Note that the SALT and INFO text fields now match those in HAP Section 5.6.6.1

      TempBuffer<uint8_t> iosDeviceX(32);
      HKDF::create(iosDeviceX,srp->K,64,"Pair-Setup-Controller-Sign-Salt","Pair-Setup-Controller-Sign-Info");     // derive iosDeviceX (32 bytes) from SRP Shared Secret using HKDF 

      // Concatenate iosDeviceX, IOS ID, and IOS PublicKey into iosDeviceInfo
      
      TempBuffer<uint8_t> iosDeviceInfo(iosDeviceX,iosDeviceX.len(),(uint8_t *)(*itIdentifier),itIdentifier->getLen(),(uint8_t *)(*itPublicKey),itPublicKey->getLen(),NULL);

      if(crypto_sign_verify_detached(*itSignature, iosDeviceInfo, iosDeviceInfo.len(), *itPublicKey) != 0){      // verify signature of iosDeviceInfo using iosDeviceLTPK   
        LOG0("\n*** ERROR: LPTK Signature Verification Failed\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);        // set Error=Authentication
        tlvRespond(responseTLV);                                        // send response to client
        pairStatus=pairState_M1;                                        // reset pairStatus to first step of unpaired
        return(0);                
      }

      addController(*itIdentifier,*itPublicKey,true);                   // save Pairing ID and LTPK for this Controller with admin privileges

      // Now perform the above steps in reverse to securely transmit the AccessoryLTPK to the Controller (HAP Section 5.6.6.2)

      TempBuffer<uint8_t> accessoryX(32);
      HKDF::create(accessoryX,srp->K,64,"Pair-Setup-Accessory-Sign-Salt","Pair-Setup-Accessory-Sign-Info");       // derive accessoryX from SRP Shared Secret using HKDF 
      
      // Concatenate accessoryX, Accessory ID, and Accessory PublicKey into accessoryInfo

      TempBuffer<uint8_t> accessoryInfo(accessoryX,accessoryX.len(),accessory.ID,hap_accessory_IDBYTES,accessory.LTPK,crypto_sign_PUBLICKEYBYTES,NULL);

      subTLV.clear();                                                                            // clear existing SUBTLV records

      itSignature=subTLV.add(kTLVType_Signature,64,NULL);                                        // create blank Signature TLV with space for 64 bytes

      crypto_sign_detached(*itSignature,NULL,accessoryInfo,accessoryInfo.len(),accessory.LTSK);  // produce signature of accessoryInfo using AccessoryLTSK (Ed25519 long-term secret key)

      subTLV.add(kTLVType_Identifier,hap_accessory_IDBYTES,accessory.ID);                        // set Identifier TLV record as accessoryPairingID
      subTLV.add(kTLVType_PublicKey,crypto_sign_PUBLICKEYBYTES,accessory.LTPK);                  // set PublicKey TLV record as accessoryLTPK

      LOG2("------- ENCRYPTING SUB-TLVS -------\n");

      if(homeSpan.getLogLevel()>1)
        subTLV.print();

      TempBuffer<uint8_t> subPack(subTLV.pack_size());                                           // create sub-TLV by packing Identifier, PublicKey and Signature TLV records together
      subTLV.pack(subPack);      

      // Encrypt the subTLV data using the same SRP Session Key as above with ChaCha20-Poly1305

      itEncryptedData=responseTLV.add(kTLVType_EncryptedData,subPack.len()+crypto_aead_chacha20poly1305_IETF_ABYTES,NULL);     //create blank EncryptedData TLV with space for subTLV + Authentication Tag

      crypto_aead_chacha20poly1305_ietf_encrypt(*itEncryptedData,NULL,subPack,subPack.len(),NULL,0,NULL,(unsigned char *)"\x00\x00\x00\x00PS-Msg06",temp.sessionKey);
                                                   
      LOG2("---------- END SUB-TLVS! ----------\n");
      
      tlvRespond(responseTLV);                              // send response to client

      delete srp;                                           // delete SRP - no longer needed once pairing is completed
      srp=NULL;                                             // reset to NULL

      mdns_service_txt_item_set("_hap","_tcp","sf","0");    // broadcast new status
      
      LOG1("\n*** ACCESSORY PAIRED! ***\n");

      STATUS_UPDATE(on(),HS_PAIRED)      
            
      if(homeSpan.pairCallback)                             // if set, invoke user-defined Pairing Callback to indicate device has been paired
        homeSpan.pairCallback(true);
      
      return(1);        
    }       
    break;

  } // switch

  return(1);

} // postPairSetup

//////////////////////////////////////

int HAPClient::postPairVerifyURL(uint8_t *content, size_t len){

  HAPTLV iosTLV;
  HAPTLV responseTLV;
  HAPTLV subTLV;

  iosTLV.unpack(content,len);
  if(homeSpan.getLogLevel()>1)
    iosTLV.print();
  LOG2("------------ END TLVS! ------------\n");

  LOG1("In Pair Verify #%d (%s)...",clientNumber,client.remoteIP().toString().c_str());
  
  auto itState=iosTLV.find(kTLVType_State);

  if(iosTLV.len(itState)!=1){                                   // missing STATE TLV
    LOG0("\n*** ERROR: Missing or invalid 'State' TLV\n\n");
    badRequestError();                                          // return with 400 error, which closes connection      
    return(0);
  }

  int tlvState=itState->getVal();

  if(!nAdminControllers()){                             // error: Device not yet paired - we should not be receiving any requests for Pair-Verify!
    LOG0("\n*** ERROR: Device not yet paired!\n\n");
    responseTLV.add(kTLVType_State,tlvState+1);         // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    responseTLV.add(kTLVType_Error,tagError_Unknown);   // set Error=Unknown
    tlvRespond(responseTLV);                            // send response to client
    return(0);
  };

  LOG1("Found <M%d>\n",tlvState);          // unlike pair-setup, out-of-sequencing can be handled gracefully for pair-verify (HAP requirement). No need to keep track of pairStatus

  switch(tlvState){                        // Pair-Verify STATE received -- process request!  (HAP Section 5.7)

    case pairState_M1:{                    // 'Verify Start Request'

      auto itPublicKey=iosTLV.find(kTLVType_PublicKey);

      if(iosTLV.len(itPublicKey)!=crypto_box_PUBLICKEYBYTES){            
        LOG0("\n*** ERROR: Required 'PublicKey' TLV record for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_State,pairState_M2);        // set State=<M2>
        responseTLV.add(kTLVType_Error,tagError_Unknown);    // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                             // send response to client
        return(0);        
      }

      TempBuffer<uint8_t> secretCurveKey(crypto_box_SECRETKEYBYTES);                // temporary space - used only in this block     
      crypto_box_keypair(temp.publicCurveKey,secretCurveKey);                       // generate Accessory's random Curve25519 Public/Secret Key Pair

      memcpy(temp.iosCurveKey,*itPublicKey,crypto_box_PUBLICKEYBYTES);              // save Controller's Curve25519 Public Key
            
      // concatenate Accessory's Curve25519 Public Key, Accessory's Pairing ID, and Controller's Curve25519 Public Key into accessoryInfo
      
      TempBuffer<uint8_t> accessoryInfo(temp.publicCurveKey,crypto_box_PUBLICKEYBYTES,accessory.ID,hap_accessory_IDBYTES,temp.iosCurveKey,crypto_box_PUBLICKEYBYTES,NULL);

      subTLV.add(kTLVType_Identifier,hap_accessory_IDBYTES,accessory.ID);                         // set Identifier subTLV record as Accessory's Pairing ID
      auto itSignature=subTLV.add(kTLVType_Signature,crypto_sign_BYTES,NULL);                     // create blank Signature subTLV
      crypto_sign_detached(*itSignature,NULL,accessoryInfo,accessoryInfo.len(),accessory.LTSK);   // produce Signature of accessoryInfo using Accessory's LTSK

      LOG2("------- ENCRYPTING SUB-TLVS -------\n");

      if(homeSpan.getLogLevel()>1)
        subTLV.print();

      TempBuffer<uint8_t> subPack(subTLV.pack_size());                                                    // create sub-TLV by packing Identifier and Signature TLV records together
      subTLV.pack(subPack);                                

      crypto_scalarmult_curve25519(temp.sharedCurveKey,secretCurveKey,temp.iosCurveKey);                  // generate Shared-Secret Curve25519 Key from Accessory's Curve25519 Secret Key and Controller's Curve25519 Public Key

      HKDF::create(temp.sessionKey,temp.sharedCurveKey,crypto_box_PUBLICKEYBYTES,"Pair-Verify-Encrypt-Salt","Pair-Verify-Encrypt-Info");   // create Session Curve25519 Key from Shared-Secret Curve25519 Key using HKDF-SHA-512  

      auto itEncryptedData=responseTLV.add(kTLVType_EncryptedData,subPack.len()+crypto_aead_chacha20poly1305_IETF_ABYTES,NULL);                                         // create blank EncryptedData subTLV
      crypto_aead_chacha20poly1305_ietf_encrypt(*itEncryptedData,NULL,subPack,subPack.len(),NULL,0,NULL,(unsigned char *)"\x00\x00\x00\x00PV-Msg02",temp.sessionKey);   // encrypt data with Session Curve25519 Key and padded nonce="PV-Msg02"
                                            
      LOG2("---------- END SUB-TLVS! ----------\n");
      
      responseTLV.add(kTLVType_State,pairState_M2);                                        // set State=<M2>
      responseTLV.add(kTLVType_PublicKey,crypto_box_PUBLICKEYBYTES,temp.publicCurveKey);        // set PublicKey to Accessory's Curve25519 Public Key
    
      tlvRespond(responseTLV);                                      // send response to client  
    }
    break;  
   
    case pairState_M3:{                     // 'Verify Finish Request'

      auto itEncryptedData=iosTLV.find(kTLVType_EncryptedData);

      if(iosTLV.len(itEncryptedData)<=0){            
        LOG0("\n*** ERROR: Required 'EncryptedData' TLV record for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_State,pairState_M4);               // set State=<M4>
        responseTLV.add(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                                    // send response to client
        return(0);
      };

      LOG2("------- DECRYPTING SUB-TLVS -------\n");

      // use Session Curve25519 Key (from previous step) to decrypt encrypytedData TLV with padded nonce="PV-Msg03"

      TempBuffer<uint8_t> decrypted((*itEncryptedData).getLen()-crypto_aead_chacha20poly1305_IETF_ABYTES);        // temporary storage for decrypted data
      
      if(crypto_aead_chacha20poly1305_ietf_decrypt(decrypted, NULL, NULL, *itEncryptedData, itEncryptedData->getLen(), NULL, 0, (unsigned char *)"\x00\x00\x00\x00PV-Msg03", temp.sessionKey)==-1){          
        LOG0("\n*** ERROR: Verify Authentication Failed\n\n");
        responseTLV.add(kTLVType_State,pairState_M4);               // set State=<M4>
        responseTLV.add(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond(responseTLV);                                    // send response to client
        return(0);        
      }

      subTLV.unpack(decrypted,decrypted.len());                     // unpack TLV     
      if(homeSpan.getLogLevel()>1)
        subTLV.print();                                             // print decrypted TLV data
      
      LOG2("---------- END SUB-TLVS! ----------\n");

      auto itIdentifier=subTLV.find(kTLVType_Identifier);
      auto itSignature=subTLV.find(kTLVType_Signature);

      if(subTLV.len(itIdentifier)!=hap_controller_IDBYTES || subTLV.len(itSignature)!=crypto_sign_BYTES){ 
        LOG0("\n*** ERROR: One or more of required 'Identifier,' and 'Signature' TLV records for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_State,pairState_M4);               // set State=<M4>
        responseTLV.add(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond(responseTLV);                                    // send response to client
        return(0);
      }

      Controller *tPair;                                            // temporary pointer to Controller
      
      if(!(tPair=findController(*itIdentifier))){
        LOG1("\n*** WARNING: Unrecognized Controller ID: ");
        charPrintRow(*itIdentifier,hap_controller_IDBYTES,1);
        LOG1("\n\n");
        responseTLV.add(kTLVType_State,pairState_M4);               // set State=<M4>
        responseTLV.add(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond(responseTLV);                                    // send response to client
        return(0);
      }

      LOG2("\n*** Verifying session with Controller ID: ");
      charPrintRow(tPair->ID,hap_controller_IDBYTES,2);
      LOG2("...\n");

      // concatenate Controller's Curve25519 Public Key (from previous step), Controller's Pairing ID, and Accessory's Curve25519 Public Key (from previous step) into iosDeviceInfo     

      TempBuffer<uint8_t> iosDeviceInfo(temp.iosCurveKey,crypto_box_PUBLICKEYBYTES,tPair->ID,hap_controller_IDBYTES,temp.publicCurveKey,crypto_box_PUBLICKEYBYTES,NULL);
      
      if(crypto_sign_verify_detached(*itSignature, iosDeviceInfo, iosDeviceInfo.len(), tPair->LTPK) != 0){         // verify signature of iosDeviceInfo using Controller's LTPK   
        LOG0("\n*** ERROR: LPTK Signature Verification Failed\n\n");
        responseTLV.add(kTLVType_State,pairState_M4);               // set State=<M4>
        responseTLV.add(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond(responseTLV);                                    // send response to client
        return(0);                
      }

      responseTLV.add(kTLVType_State,pairState_M4);                 // set State=<M4>
      tlvRespond(responseTLV);                                      // send response to client (unencrypted since cPair=NULL)

      cPair=tPair;        // save Controller for this connection slot - connection is now verified and should be encrypted going forward

      HKDF::create(a2cKey,temp.sharedCurveKey,32,"Control-Salt","Control-Read-Encryption-Key");        // create AccessoryToControllerKey from (previously-saved) Shared-Secret Curve25519 Key (HAP Section 6.5.2)
      HKDF::create(c2aKey,temp.sharedCurveKey,32,"Control-Salt","Control-Write-Encryption-Key");       // create ControllerToAccessoryKey from (previously-saved) Shared-Secret Curve25519 Key (HAP Section 6.5.2)
      
      a2cNonce.zero();         // reset Nonces for this session to zero
      c2aNonce.zero();

      LOG2("\n*** SESSION VERIFICATION COMPLETE *** \n");
    }
    break;
  
  } // switch

  return(1);
  
} // postPairVerify

//////////////////////////////////////

int HAPClient::postPairingsURL(uint8_t *content, size_t len){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  HAPTLV iosTLV;
  HAPTLV responseTLV;

  iosTLV.unpack(content,len);
  if(homeSpan.getLogLevel()>1)
    iosTLV.print();
  LOG2("------------ END TLVS! ------------\n");

  LOG1("In Post Pairings #%d (%s)...",clientNumber,client.remoteIP().toString().c_str());
  
  auto itState=iosTLV.find(kTLVType_State);
  auto itMethod=iosTLV.find(kTLVType_Method);
    
  if(iosTLV.len(itState)!=1 || itState->getVal()!=1){           // missing STATE TLV
    LOG0("\n*** ERROR: Parirings 'State' is either missing or not set to <M1>\n\n");
    badRequestError();                                          // return with 400 error, which closes connection      
    return(0);
  }

  if(iosTLV.len(itMethod)!=1){                                  // missing METHOD TLV
    LOG0("\n*** ERROR: Missing or invalid 'Method' TLV\n\n");
    badRequestError();                                          // return with 400 error, which closes connection      
    return(0);
  }

  int tlvMethod=itMethod->getVal();

  responseTLV.add(kTLVType_State,pairState_M2);                 // all responses include State=M2
  
  switch(tlvMethod){                        // List-Pairings received -- process request!  (HAP Sections 5.10-5.12)

    case 3: {
      LOG1("Add...\n");

      auto itIdentifier=iosTLV.find(kTLVType_Identifier);
      auto itPublicKey=iosTLV.find(kTLVType_PublicKey);
      auto itPermissions=iosTLV.find(kTLVType_Permissions);
      
      if(iosTLV.len(itIdentifier)!=hap_controller_IDBYTES || iosTLV.len(itPublicKey)!=crypto_sign_PUBLICKEYBYTES || iosTLV.len(itPermissions)!=1){            
        LOG0("\n*** ERROR: One or more of required 'Identifier,' 'PublicKey,' and 'Permissions' TLV records for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unknown);
        tlvRespond(responseTLV);
        return(0);
      }
      
      if(!cPair->admin){
        LOG0("\n*** ERROR: Controller making request does not have admin privileges to add/update other Controllers\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);
        tlvRespond(responseTLV);
        return(0);
      } 
             
      tagError err=addController(*itIdentifier,*itPublicKey,itPermissions->getVal());
      if(err!=tagError_None)
        responseTLV.add(kTLVType_Error,err);
      
      tlvRespond(responseTLV);
      return(1);
    }
    break;

    case 4: {
      LOG1("Remove...\n");

      auto itIdentifier=iosTLV.find(kTLVType_Identifier);

      if(iosTLV.len(itIdentifier)!=hap_controller_IDBYTES){            
        LOG0("\n*** ERROR: Required 'Identifier' TLV record for this step is bad or missing\n\n");
        responseTLV.add(kTLVType_Error,tagError_Unknown);
        tlvRespond(responseTLV);
        return(0);
      }
      
      if(!cPair->admin){
        LOG0("\n*** ERROR: Controller making request does not have admin privileges to remove Controllers\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);
        tlvRespond(responseTLV);
        return(0);
      }
      
      tlvRespond(responseTLV);           // must send response before removing Controller     
      removeController(*itIdentifier);
      
      return(1);
    } 
    break;
      
    case 5: {
      LOG1("List...\n");

      if(!cPair->admin){
        LOG0("\n*** ERROR: Controller making request does not have admin privileges to remove Controllers\n\n");
        responseTLV.add(kTLVType_Error,tagError_Authentication);
        tlvRespond(responseTLV);
        return(0);
      }      

      boolean addSeparator=false;
      
      for(auto it=controllerList.begin();it!=controllerList.end();it++){
        if((*it).allocated){
          if(addSeparator)         
            responseTLV.add(kTLVType_Separator);                                        
          responseTLV.add(kTLVType_Permissions,(*it).admin);      
          responseTLV.add(kTLVType_Identifier,hap_controller_IDBYTES,(*it).ID);
          responseTLV.add(kTLVType_PublicKey,crypto_sign_PUBLICKEYBYTES,(*it).LTPK);
          addSeparator=true;
        }
      }

      tlvRespond(responseTLV);
      return(1);
    }
    break;

    default: {
      LOG0("\n*** ERROR: Undefined List-Pairings Method: %d.  Must be 3, 4, or 5\n\n",tlvMethod);
      badRequestError();                                    // return with 400 error, which closes connection      
      return(0);
    }
  } // switch
  
  return(1);
}

//////////////////////////////////////

int HAPClient::getAccessoriesURL(){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Get Accessories #%d (%s)...\n",clientNumber,client.remoteIP().toString().c_str());

  homeSpan.printfAttributes();
  size_t nBytes=hapOut.getSize();
  hapOut.flush();

  LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",client.remoteIP().toString().c_str());

  hapOut.setLogLevel(2).setHapClient(this);    
  hapOut << "HTTP/1.1 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: " << nBytes << "\r\n\r\n";
  homeSpan.printfAttributes();
  hapOut.flush();

  LOG2("\n-------- SENT ENCRYPTED! --------\n");
         
  return(1);
  
} // getAccessories

//////////////////////////////////////

int HAPClient::getCharacteristicsURL(char *urlBuf){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Get Characteristics #%d (%s)...\n",clientNumber,client.remoteIP().toString().c_str());

  if(homeSpan.getCharacteristicsCallback)
    homeSpan.getCharacteristicsCallback(urlBuf);

  int len=strlen(urlBuf);           // determine number of IDs specified by counting commas in URL
  int numIDs=1;
  for(int i=0;i<len;i++)
    if(urlBuf[i]==',')
      numIDs++;
  
  TempBuffer<char *> ids(numIDs);   // reserve space for number of IDs found
  int flags=GET_VALUE|GET_AID;      // flags indicating which characteristic fields to include in response (HAP Table 6-13)
  numIDs=0;                         // reset number of IDs found

  char *lastSpace=strchr(urlBuf,' ');
  if(lastSpace)
    lastSpace[0]='\0';
    
  char *p1;
  while(char *t1=strtok_r(urlBuf,"&",&p1)){      // parse request into major tokens
    urlBuf=NULL;

    if(!strcmp(t1,"meta=1")){
      flags|=GET_META;
    } else 
    if(!strcmp(t1,"perms=1")){
      flags|=GET_PERMS;
    } else 
    if(!strcmp(t1,"type=1")){
      flags|=GET_TYPE;
    } else 
    if(!strcmp(t1,"ev=1")){
      flags|=GET_EV;
    } else
    if(!strncmp(t1,"id=",3)){   
      t1+=3;
      char *p2;
      while(char *t2=strtok_r(t1,",",&p2)){      // parse IDs
        t1=NULL;
        ids[numIDs++]=t2;
      }
    }
  } // parse URL

  if(!numIDs)           // could not find any IDs
    return(0);

  LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",client.remoteIP().toString().c_str());

  boolean statusFlag=homeSpan.printfAttributes(ids,numIDs,flags);     // get statusFlag returned to use below
  size_t nBytes=hapOut.getSize();
  hapOut.flush();

  hapOut.setLogLevel(2).setHapClient(this);
  hapOut << "HTTP/1.1 " << (!statusFlag?"200 OK":"207 Multi-Status") << "\r\nContent-Type: application/hap+json\r\nContent-Length: " << nBytes << "\r\n\r\n";
  homeSpan.printfAttributes(ids,numIDs,flags);
  hapOut.flush();

  LOG2("\n-------- SENT ENCRYPTED! --------\n");
        
  return(1);
}

//////////////////////////////////////

int HAPClient::putCharacteristicsURL(char *json){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Put Characteristics #%d (%s)...\n",clientNumber,client.remoteIP().toString().c_str());

  SpanBufVec pVec;
   
  if(!homeSpan.updateCharacteristics(json, pVec))         // perform update and check for success
    return(0);                                            // return if failed to update (error message will have been printed in update)

  boolean multiCast=false;
  for(auto it=pVec.begin();it!=pVec.end() && !multiCast;it++)   // for each characteristic, check if any status is either NOT OKAY, or if WRITE-RESPONSE is requested
    if((*it).status!=StatusCode::OK || (*it).wr)                // if so, must use multicast response
      multiCast=true;    

  LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",client.remoteIP().toString().c_str());

  if(!multiCast){                                         // JSON object has no content

    hapOut.setLogLevel(2).setHapClient(this);    
    hapOut << "HTTP/1.1 204 No Content\r\n\r\n";
    hapOut.flush();
        
  } else {                                                // multicast response is required

    homeSpan.printfAttributes(pVec);
    size_t nBytes=hapOut.getSize();
    hapOut.flush();
  
    hapOut.setLogLevel(2).setHapClient(this);
    hapOut << "HTTP/1.1 207 Multi-Status\r\nContent-Type: application/hap+json\r\nContent-Length: " << nBytes << "\r\n\r\n";
    homeSpan.printfAttributes(pVec);
    hapOut.flush(); 
  }

  LOG2("\n-------- SENT ENCRYPTED! --------\n");

  // Create and send Event Notifications if needed

  eventNotify(pVec,this);                                 // transmit EVENT Notification for objects, except DO NOT notify client making request
    
  return(1);
}

//////////////////////////////////////

int HAPClient::putPrepareURL(char *json){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Put Prepare #%d (%s)...\n",clientNumber,client.remoteIP().toString().c_str());

  char ttlToken[]="\"ttl\":";
  char pidToken[]="\"pid\":";
  
  char *cBuf;
  uint32_t ttl=0;
  uint64_t pid=0;
   
  if((cBuf=strstr(json,ttlToken)))
    sscanf(cBuf+strlen(ttlToken),"%lu",&ttl);

  if((cBuf=strstr(json,pidToken)))
    sscanf(cBuf+strlen(ttlToken),"%llu",&pid);

  StatusCode status=StatusCode::OK;

  if(ttl>0 && pid>0){                           // found required elements
    homeSpan.TimedWrites[pid]=ttl+millis();     // store this pid/alarmTime combination 
  } else {                                      // problems parsing request
    status=StatusCode::InvalidValue;
  }

  LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",client.remoteIP().toString().c_str());

  hapOut << "{\"status\":" << (int)status << "}";
  size_t nBytes=hapOut.getSize();
  hapOut.flush();

  hapOut.setLogLevel(2).setHapClient(this);    
  hapOut << "HTTP/1.1 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: " << nBytes << "\r\n\r\n";
  hapOut << "{\"status\":" << (int)status << "}";
  hapOut.flush();

  LOG2("\n-------- SENT ENCRYPTED! --------\n");
         
  return(1);
}

//////////////////////////////////////

void HAPClient::getStatusURL(HAPClient *hapClient, void (*callBack)(const char *, void *), void *user_data, int refreshTime){

  std::shared_lock readLock(homeSpan.webLog.mux);        // wait for mux to be unlocked, or already locked non-exclusively, and then lock *non-exclusively* to prevent writing in vLog

  char clocktime[33];

  if(homeSpan.webLog.timeInit){
    struct tm timeinfo;
    getLocalTime(&timeinfo,10);
    strftime(clocktime,sizeof(clocktime),"%c",&timeinfo);
  } else {
    sprintf(clocktime,"Unknown");        
  }

  char uptime[32];
  int seconds=esp_timer_get_time()/1e6;
  int secs=seconds%60;
  int mins=(seconds/=60)%60;
  int hours=(seconds/=60)%24;
  int days=(seconds/=24);
    
  sprintf(uptime,"%d:%02d:%02d:%02d",days,hours,mins,secs);

  if(hapClient)
    LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",hapClient->client.remoteIP().toString().c_str());
    
  hapOut.setHapClient(hapClient).setLogLevel(2).setCallback(callBack).setCallbackUserData(user_data);

  if(!callBack){
    hapOut << "HTTP/1.1 200 OK\r\nContent-type: text/html; charset=utf-8\r\n";
    if(refreshTime>0)
      hapOut << "Refresh: " << refreshTime << "\r\n";
    hapOut << "\r\n";
  }
    
  hapOut << "<html><head><title>" << homeSpan.displayName << "</title>\n";

  if(homeSpan.webLog.faviconURL)
    hapOut << "<link rel=\"icon\" href=\"" << homeSpan.webLog.faviconURL << "\" type=\"image/png\" />\n";
    
  hapOut << "<style>body {background-color:lightblue;} th, td {padding-right: 10px; padding-left: 10px; border:1px solid black;}" << homeSpan.webLog.css.c_str() << "</style></head>\n";
  hapOut << "<body class=bod1><h2>" << homeSpan.displayName << "</h2>\n";
  
  hapOut << "<table class=tab1>\n";
  hapOut << "<tr><td>Up Time:</td><td>" << uptime << "</td></tr>\n";
  hapOut << "<tr><td>Current Time:</td><td>" << clocktime << "</td></tr>\n";
  hapOut << "<tr><td>Boot Time:</td><td>" << homeSpan.webLog.bootTime << "</td></tr>\n"; 
  hapOut << "<tr><td>Reset Reason:</td><td>" << Utils::resetReason() << " (" << esp_reset_reason() << ")</td></tr>\n";

  if(homeSpan.compileTime)
    hapOut << "<tr><td>Compile Time:</td><td>" << homeSpan.compileTime << "</td></tr>\n";

  if(!homeSpan.ethernetEnabled){
    hapOut << "<tr><td>WiFi Disconnects:</td><td>" << homeSpan.connected/2 << "</td></tr>\n";
    hapOut << "<tr><td>WiFi Signal:</td><td>" << (int)WiFi.RSSI() << " dBm</td></tr>\n";
    if(homeSpan.bssidNames.count(WiFi.BSSIDstr().c_str()))
      hapOut << "<tr><td>BSSID:</td><td>" << WiFi.BSSIDstr().c_str() << " \"" << homeSpan.bssidNames[WiFi.BSSIDstr().c_str()].c_str() << "\"" << "</td></tr>\n";
    else
      hapOut << "<tr><td>BSSID:</td><td>" << WiFi.BSSIDstr().c_str() << "</td></tr>\n";
    hapOut << "<tr><td>WiFi Local IPv4:</td><td>" << WiFi.localIP().toString().c_str() << "</td></tr>\n";
    hapOut << "<tr><td>WiFi Local IPv6:</td><td>" << homeSpan.getUniqueLocalIPv6(WiFi).toString().c_str() << "</td></tr>\n";
    hapOut << "<tr><td>WiFi Gateway:</td><td>" << WiFi.gatewayIP().toString().c_str() << "</td></tr>\n";
  } else {
    hapOut << "<tr><td>Ethernet Disconnects:</td><td>" << homeSpan.connected/2 << "</td></tr>\n";
    hapOut << "<tr><td>Ethernet Local IPv4:</td><td>" << ETH.localIP().toString().c_str() << "</td></tr>\n";    
    hapOut << "<tr><td>Ethernet Local IPv6:</td><td>" << homeSpan.getUniqueLocalIPv6(ETH).toString().c_str() << "</td></tr>\n";
    hapOut << "<tr><td>Ethernet Gateway:</td><td>" << ETH.gatewayIP().toString().c_str() << "</td></tr>\n";    
  }
  
  hapOut << "<tr><td>ESP32 Board:</td><td>" << ARDUINO_BOARD << "</td></tr>\n";
  hapOut << "<tr><td>Arduino-ESP Version:</td><td>" << ARDUINO_ESP_VERSION << "</td></tr>\n";
  hapOut << "<tr><td>ESP-IDF Version:</td><td>" << ESP_IDF_VERSION_MAJOR << "." << ESP_IDF_VERSION_MINOR << "." << ESP_IDF_VERSION_PATCH << "</td></tr>\n";
  hapOut << "<tr><td>HomeSpan Version:</td><td>" << HOMESPAN_VERSION << "</td></tr>\n";
  hapOut << "<tr><td>Sketch Version:</td><td>" << homeSpan.getSketchVersion() << "</td></tr>\n"; 
  hapOut << "<tr><td>Sodium Version:</td><td>" << sodium_version_string() << " Lib " << sodium_library_version_major() << "." << sodium_library_version_minor() << "</td></tr>\n"; 

  char mbtlsv[64];
  mbedtls_version_get_string_full(mbtlsv);
  hapOut << "<tr><td>MbedTLS Version:</td><td>" << mbtlsv << "</td></tr>\n";
  
  hapOut << "<tr><td>HomeKit Status:</td><td>" << (HAPClient::nAdminControllers()?"PAIRED":"NOT PAIRED") << "</td></tr>\n";   
  hapOut << "<tr><td>Max Log Entries:</td><td>" << homeSpan.webLog.maxEntries << "</td></tr>\n"; 

  if(homeSpan.weblogCallback){
    String usrString;
    homeSpan.weblogCallback(usrString); // Callback to add user-defined html in top table.
    hapOut << usrString.c_str();    
  }
    
  hapOut << "</table>\n";
  hapOut << "<p></p>";
  
  if(homeSpan.webLog.maxEntries>0){
    hapOut << "<table class=tab2><tr><th>Entry</th><th>Up Time</th><th>Log Time</th><th>Client</th><th>Message</th></tr>\n";
    int lastIndex=homeSpan.webLog.nEntries-homeSpan.webLog.maxEntries;
    if(lastIndex<0)
      lastIndex=0;
    
    for(int i=homeSpan.webLog.nEntries-1;i>=lastIndex;i--){
      int index=i%homeSpan.webLog.maxEntries;
      seconds=homeSpan.webLog.log[index].upTime/1e6;
      secs=seconds%60;
      mins=(seconds/=60)%60;
      hours=(seconds/=60)%24;
      days=(seconds/=24);   
      sprintf(uptime,"%d:%02d:%02d:%02d",days,hours,mins,secs);

      if(homeSpan.webLog.log[index].clockTime.tm_year>0)
        strftime(clocktime,sizeof(clocktime),"%c",&homeSpan.webLog.log[index].clockTime);
      else
        sprintf(clocktime,"Unknown");        
      
      hapOut << "<tr><td>" << i+1 << "</td><td>" << uptime << "</td><td>" << clocktime << "</td><td>" << homeSpan.webLog.log[index].clientIP.c_str() << "</td><td>" << homeSpan.webLog.log[index].message << "</td></tr>\n";
    }
    hapOut << "</table>\n";
  }
 
  hapOut << "</body></html>\n";
  hapOut.flush();

  if(hapClient){
    hapClient->client.stop();
    delay(1);
    LOG2("------------ SENT! --------------\n");
  }
}

//////////////////////////////////////

void HAPClient::checkNotifications(){

  if(!homeSpan.Notifications.empty()){       // if there are Notifications to process    
    eventNotify(homeSpan.Notifications);     // transmit EVENT Notifications
    homeSpan.Notifications.clear();          // clear Notifications vector
  }
}

//////////////////////////////////////

void HAPClient::checkTimedWrites(){

  unsigned long cTime=millis();                                       // get current time

  auto tw=homeSpan.TimedWrites.begin();
  while(tw!=homeSpan.TimedWrites.end()){
    if(cTime>tw->second){                                             // timer has expired
       LOG2("Removing PID=%llu  ALARM=%lu\n",tw->first,tw->second);
       tw=homeSpan.TimedWrites.erase(tw);
      }
    else
      tw++; 
  }
}

//////////////////////////////////////

void HAPClient::eventNotify(SpanBufVec &pVec, HAPClient *ignore){

  for(auto it=homeSpan.hapList.begin(); it!=homeSpan.hapList.end(); ++it){          // loop over all connection slots
    if(&(*it)!=ignore){                                                             // if NOT flagged to be ignored (in cases where it is the client making a PUT request)

      homeSpan.printfNotify(pVec,&(*it));              // create JSON (which may be of zero length if there are no applicable notifications for this cNum)
      size_t nBytes=hapOut.getSize();
      hapOut.flush();

      if(nBytes>0){                                    // if there ARE notifications to send to client cNum
        
        LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",it->client.remoteIP().toString().c_str());

        hapOut.setLogLevel(2).setHapClient(&(*it));    
        hapOut << "EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: " << nBytes << "\r\n\r\n";
        homeSpan.printfNotify(pVec,&(*it));
        hapOut.flush();

        LOG2("\n-------- SENT ENCRYPTED! --------\n");
      }
    }
  }         
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void HAPClient::tlvRespond(TLV8 &tlv8){

  tlv8.osprint(hapOut);
  size_t nBytes=hapOut.getSize();
  hapOut.flush();
  
  char *body;
  asprintf(&body,"HTTP/1.1 200 OK\r\nContent-Type: application/pairing+tlv8\r\nContent-Length: %d\r\n\r\n",nBytes);      // create Body with Content Length = size of TLV data

  LOG2("\n>>>>>>>>>> %s >>>>>>>>>>\n",client.remoteIP().toString().c_str());
  LOG2(body);
  if(homeSpan.getLogLevel()>1)
    tlv8.print();

  hapOut.setHapClient(this);
  hapOut << body;
  tlv8.osprint(hapOut);
  hapOut.flush();

  if(!cPair)
    LOG2("------------ SENT! --------------\n");
  else
    LOG2("-------- SENT ENCRYPTED! --------\n");

  free(body);
  
} // tlvRespond

//////////////////////////////////////

int HAPClient::receiveEncrypted(uint8_t *httpBuf, int messageSize){

  uint8_t aad[2];
  int nBytes=0;

  while(client.read(aad,2)==2){    // read initial 2-byte AAD record

    int n=aad[0]+aad[1]*256;                // compute number of bytes expected in message after decoding

    if(nBytes+n>messageSize){      // exceeded maximum number of bytes allowed in plaintext message
      LOG0("\n\n*** ERROR:  Decrypted message of %d bytes exceeded maximum expected message length of %d bytes\n\n",nBytes+n,messageSize);
      return(0);
      }

    TempBuffer<uint8_t> tBuf(n+16);      // expected number of total bytes = n bytes in encoded message + 16 bytes for appended authentication tag      

    if(client.read(tBuf,tBuf.len())!=tBuf.len()){      
      LOG0("\n\n*** ERROR: Malformed encrypted message frame\n\n");
      return(0);      
    }                

    if(crypto_aead_chacha20poly1305_ietf_decrypt(httpBuf+nBytes, NULL, NULL, tBuf, tBuf.len(), aad, 2, c2aNonce.get(), c2aKey)==-1){
      LOG0("\n\n*** ERROR: Can't Decrypt Message\n\n");
      return(0);        
    }

    c2aNonce.inc();

    nBytes+=n;          // increment total number of bytes in plaintext message
    
  } // while

  return(nBytes);
    
} // receiveEncrypted

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void HAPClient::hexPrintColumn(const uint8_t *buf, int n, int minLogLevel){

  if(homeSpan.logLevel<minLogLevel)
    return;
  
  for(int i=0;i<n;i++)
    Serial.printf("%d) %02X\n",i,buf[i]);
}

//////////////////////////////////////

void HAPClient::hexPrintRow(const uint8_t *buf, int n, int minLogLevel){

  if(homeSpan.logLevel<minLogLevel)
    return;

  for(int i=0;i<n;i++)
    Serial.printf("%02X",buf[i]);
}

//////////////////////////////////////

void HAPClient::charPrintRow(const uint8_t *buf, int n, int minLogLevel){

  if(homeSpan.logLevel<minLogLevel)
    return;
  
  for(int i=0;i<n;i++)
    Serial.printf("%c",buf[i]);
}

//////////////////////////////////////

Controller *HAPClient::findController(uint8_t *id){

  for(auto it=controllerList.begin();it!=controllerList.end();it++){
    if(!memcmp((*it).ID,id,hap_controller_IDBYTES))
      return(&*it);
  }

  return(NULL);       // no match
}

//////////////////////////////////////

int HAPClient::nAdminControllers(){

  int n=0;
  for(auto it=controllerList.begin();it!=controllerList.end();it++)
    n+=((*it).admin);
  return(n);
}

//////////////////////////////////////

tagError HAPClient::addController(uint8_t *id, uint8_t *ltpk, boolean admin){

  Controller *cTemp=findController(id);

  tagError err=tagError_None;
  
  if(!cTemp){                                            // new controller    
    if(controllerList.size()<MAX_CONTROLLERS){
      controllerList.emplace_back(id,ltpk,admin);        // create and store data
      LOG2("\n*** Added Controller: ");
      charPrintRow(id,hap_controller_IDBYTES,2);
      LOG2(admin?" (admin)\n\n":" (regular)\n\n");
      saveControllers();
    } else {
      LOG0("\n*** ERROR: Can't pair more than %d Controllers\n\n",MAX_CONTROLLERS);
      err=tagError_MaxPeers;
    }    
  } else if(!memcmp(ltpk,cTemp->LTPK,crypto_sign_PUBLICKEYBYTES)){   // existing controller with same LTPK
    LOG2("\n*** Updated Controller: ");
    charPrintRow(id,hap_controller_IDBYTES,2);
    LOG2(" from %s to %s\n\n",cTemp->admin?"(admin)":"(regular)",admin?"(admin)":"(regular)");
    cTemp->admin=admin;
    saveControllers();    
  } else {
    LOG0("\n*** ERROR: Invalid request to update the LTPK of an existing Controller\n\n");
    err=tagError_Unknown;    
  }

  return(err);
}          

//////////////////////////////////////

void HAPClient::removeController(uint8_t *id){

  auto it=std::find_if(controllerList.begin(), controllerList.end(), [id](const Controller& cTemp){return(!memcmp(cTemp.ID,id,hap_controller_IDBYTES));});

  if(it==controllerList.end()){
    LOG2("\n*** Request to Remove Controller Ignored - Controller Not Found: ");
    charPrintRow(id,hap_controller_IDBYTES,2);
    LOG2("\n");
    return;
  }

  LOG1("\n*** Removing Controller: ");
  charPrintRow((*it).ID,hap_controller_IDBYTES,2);
  LOG1((*it).admin?" (admin)\n":" (regular)\n");
  
  tearDown((*it).ID);         // teardown any connections using this Controller
  controllerList.erase(it);   // remove Controller

  if(!nAdminControllers()){   // no more admin Controllers
    
    LOG1("That was last Admin Controller!  Removing any remaining Regular Controllers and unpairing Accessory\n");    
    
    tearDown(NULL);                                              // teardown all remaining connections
    controllerList.clear();                                      // remove all remaining Controllers
    mdns_service_txt_item_set("_hap","_tcp","sf","1");           // set Status Flag = 1 (Table 6-8)
    STATUS_UPDATE(start(LED_PAIRING_NEEDED),HS_PAIRING_NEEDED)   // set optional Status LED
    if(homeSpan.pairCallback)                                    // if set, invoke user-defined Pairing Callback to indicate device has been un-paired
      homeSpan.pairCallback(false);    
  }

  saveControllers();
}

//////////////////////////////////////

void HAPClient::tearDown(uint8_t *id){

  for(HAPClient &hc : homeSpan.hapList){
    if(id==NULL || (hc.cPair && !memcmp(id,hc.cPair->ID,hap_controller_IDBYTES))){
      LOG1("*** Terminating Client #%d\n",hc.clientNumber);
      hc.client.stop();
    }
  }
}

//////////////////////////////////////

void HAPClient::printControllers(int minLogLevel){

  if(homeSpan.logLevel<minLogLevel)
    return;

  if(controllerList.empty()){
    Serial.printf("No Paired Controllers\n");
    return;    
  }
  
  for(auto it=controllerList.begin();it!=controllerList.end();it++){
    Serial.printf("Paired Controller: ");
    charPrintRow((*it).ID,hap_controller_IDBYTES);
    Serial.printf("%s  LTPK: ",(*it).admin?"   (admin)":" (regular)");
    hexPrintRow((*it).LTPK,crypto_sign_PUBLICKEYBYTES);
    Serial.printf("\n");    
  }
}

//////////////////////////////////////

void HAPClient::saveControllers(){

  if(homeSpan.controllerCallback)
    homeSpan.controllerCallback();

  if(controllerList.empty()){
    nvs_erase_key(homeSpan.hapNVS,"CONTROLLERS");
    return;
  }

  TempBuffer<Controller> tBuf(controllerList.size());                    // create temporary buffer to hold Controller data
  std::copy(controllerList.begin(),controllerList.end(),tBuf.get());     // copy data from linked list to buffer
  
  nvs_set_blob(homeSpan.hapNVS,"CONTROLLERS",tBuf,tBuf.len());           // update data
  nvs_commit(homeSpan.hapNVS);                                           // commit to NVS  
}


//////////////////////////////////////
//////////////////////////////////////

Nonce::Nonce(){
  zero();
}

//////////////////////////////////////

void Nonce::zero(){
  memset(x,0,12);
}

//////////////////////////////////////

uint8_t *Nonce::get(){
  return(x);
}

//////////////////////////////////////

void Nonce::inc(){
  x[4]++;
  if(x[4]==0)
    x[5]++;
}

//////////////////////////////////////
//////////////////////////////////////

HapOut::HapStreamBuffer::HapStreamBuffer(){

  // note - must require all memory allocation to be pulled from INTERNAL heap only

  const uint32_t caps=MALLOC_CAP_DEFAULT | MALLOC_CAP_INTERNAL;

  buffer=(char *)heap_caps_malloc(bufSize+1,caps);                                          // add 1 for adding null terminator when printing text
  encBuf=(uint8_t *)heap_caps_malloc(bufSize+18,caps);                                      // 2-byte AAD + encrypted data + 16-byte authentication tag 
  hash=(uint8_t *)heap_caps_malloc(48,caps);                                                // space for SHA-384 hash output
  ctx = (mbedtls_sha512_context *)heap_caps_malloc(sizeof(mbedtls_sha512_context),caps);    // space for hash context
  
  mbedtls_sha512_init(ctx);                 // initialize context
  mbedtls_sha512_starts(ctx,1);             // start SHA-384 hash (note second argument=1)
  
  setp(buffer, buffer+bufSize-1);           // assign buffer pointers
}

//////////////////////////////////////

HapOut::HapStreamBuffer::~HapStreamBuffer(){

  sync();
  free(buffer);
  free(encBuf);
  free(hash);
  free(ctx);
}

//////////////////////////////////////

void HapOut::HapStreamBuffer::flushBuffer(){
  
  int num=pptr()-pbase();

  byteCount+=num;

  buffer[num]='\0';                               // add null terminator but DO NOT increment num (we don't want terminator considered as part of buffer)

  if(callBack)
    callBack(buffer,callBackUserData);

  if(logLevel<=homeSpan.getLogLevel()){
    if(enablePrettyPrint)                         // if pretty print needed, use formatted method
      printFormatted(buffer,num,2);
    else                                          // if not, just print
    Serial.print(buffer);         
  }
  
  if(hapClient!=NULL){
    if(!hapClient->cPair){                        // if not encrypted 
      hapClient->client.write(buffer,num);        // transmit data buffer
      
    } else {                                      // if encrypted
      
      encBuf[0]=num%256;                          // store number of bytes that encrypts this frame (AAD bytes)
      encBuf[1]=num/256;
      crypto_aead_chacha20poly1305_ietf_encrypt(encBuf+2,NULL,(uint8_t *)buffer,num,encBuf,2,NULL,hapClient->a2cNonce.get(),hapClient->a2cKey);   // encrypt buffer with AAD prepended and authentication tag appended
      
      hapClient->client.write(encBuf,num+18);     // transmit encrypted frame
      hapClient->a2cNonce.inc();                  // increment nonce
    }
    delay(1);
  }

  mbedtls_sha512_update(ctx,(uint8_t *)buffer,num);       // update hash

  pbump(-num);                                            // reset buffer pointers
}

//////////////////////////////////////
        
std::streambuf::int_type HapOut::HapStreamBuffer::overflow(std::streambuf::int_type c){
  
  if(c!=EOF){
    *pptr() = c;
    pbump(1);
  }

  flushBuffer();
  return(c);
}

//////////////////////////////////////

int HapOut::HapStreamBuffer::sync(){

  flushBuffer();
  
  logLevel=255;
  hapClient=NULL;
  enablePrettyPrint=false;
  byteCount=0;
  indent=0;
  
  if(callBack){
    callBack(NULL,callBackUserData);
    callBack=NULL;
    callBackUserData=NULL;
  }

  mbedtls_sha512_finish(ctx,hash);    // finish SHA-384 and store hash
  mbedtls_sha512_starts(ctx,1);       // re-start hash for next time

  return(0);
}

//////////////////////////////////////

void HapOut::HapStreamBuffer::printFormatted(char *buf, size_t nChars, size_t nsp){
  
  for(int i=0;i<nChars;i++){
    switch(buf[i]){
      
      case '{':
      case '[':
        Serial.printf("%c\n",buf[i]);
        indent+=nsp;
        for(int j=0;j<indent;j++)
          Serial.printf(" ");
        break;

      case '}':
      case ']':
        Serial.printf("\n");
        indent-=nsp;
        for(int j=0;j<indent;j++)
          Serial.printf(" ");
        Serial.printf("%c",buf[i]);
        break;

      case ',':
        Serial.printf("%c\n",buf[i]);
        for(int j=0;j<indent;j++)
          Serial.printf(" ");
        break;

      default:
        Serial.printf("%c",buf[i]);           
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// instantiate all static HAP Client structures and data

pairState HAPClient::pairStatus;                        
Accessory HAPClient::accessory;                         
list<Controller, Mallocator<Controller>> HAPClient::controllerList;
 
