
#include <ESPmDNS.h>
#include <sodium.h>

#include "HomeSpan.h"
#include "HAP.h"

//////////////////////////////////////

void HAPClient::init(){

  size_t len;             // not used but required to read blobs from NVS

  Serial.print("\n");

  nvs_open("HAP",NVS_READWRITE,&nvsHandle);     // open HAP data namespace in NVS
  
  if(!nvs_get_blob(nvsHandle,"ACCESSORY",NULL,&len)){                    // if found long-term Accessory data in NVS
    nvs_get_blob(nvsHandle,"ACCESSORY",&accessory,&len);                 // retrieve data
  } else {      
    Serial.print("Generating new random Accessory ID and Long-Term Ed25519 Signature Keys...\n");
    uint8_t buf[6];
    char cBuf[18];
    
    randombytes_buf(buf,6);                                              // generate 6 random bytes using libsodium (which uses the ESP32 hardware-based random number generator)
    sprintf(cBuf,"%02X:%02X:%02X:%02X:%02X:%02X",                        // create ID in form "XX:XX:XX:XX:XX:XX" (HAP Table 6-7)
      buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);

    memcpy(accessory.ID,cBuf,17);                                        // copy into Accessory ID for permanent storage
    crypto_sign_keypair(accessory.LTPK,accessory.LTSK);                  // generate new random set of keys using libsodium public-key signature
    
    nvs_set_blob(nvsHandle,"ACCESSORY",&accessory,sizeof(accessory));    // update data
    nvs_commit(nvsHandle);                                               // commit to NVS
  }

  if(!nvs_get_blob(nvsHandle,"CONTROLLERS",NULL,&len)){                 // if found long-term Controller Pairings data from NVS
    nvs_get_blob(nvsHandle,"CONTROLLERS",controllers,&len);             // retrieve data
  } else {
    Serial.print("Initializing storage for Paired Controllers data...\n\n");               
    
    HAPClient::removeControllers();                                             // clear all Controller data
        
    nvs_set_blob(nvsHandle,"CONTROLLERS",controllers,sizeof(controllers));      // update data
    nvs_commit(nvsHandle);                                                      // commit to NVS
  }

  Serial.print("Accessory ID:      ");
  charPrintRow(accessory.ID,17);
  Serial.print("                               LTPK: ");
  hexPrintRow(accessory.LTPK,32);
  Serial.print("\n");

  printControllers();                                                         

  tlv8.create(kTLVType_State,1,"STATE");                 // define each the actual TLV records needed for the implementation of HAP; one for each kTLVType needed (HAP Table 5-6)
  tlv8.create(kTLVType_PublicKey,384,"PUBKEY");
  tlv8.create(kTLVType_Method,1,"METHOD");
  tlv8.create(kTLVType_Salt,16,"SALT");
  tlv8.create(kTLVType_Error,1,"ERROR");
  tlv8.create(kTLVType_Proof,64,"PROOF");
  tlv8.create(kTLVType_EncryptedData,1024,"ENC.DATA");
  tlv8.create(kTLVType_Signature,64,"SIGNATURE");
  tlv8.create(kTLVType_Identifier,64,"IDENTIFIER");
  tlv8.create(kTLVType_Permissions,1,"PERMISSION");

  if(!nvs_get_blob(nvsHandle,"HAPHASH",NULL,&len)){                 // if found HAP HASH structure
    nvs_get_blob(nvsHandle,"HAPHASH",&homeSpan.hapConfig,&len);     // retrieve data    
  } else {
    Serial.print("Resetting Accessory Configuration number...\n");
    nvs_set_blob(nvsHandle,"HAPHASH",&homeSpan.hapConfig,sizeof(homeSpan.hapConfig));     // update data
    nvs_commit(nvsHandle);                                                                // commit to NVS
  }

  Serial.print("\n");

  uint8_t tHash[48];
  TempBuffer <char> tBuf(homeSpan.sprintfAttributes(NULL)+1);
  homeSpan.sprintfAttributes(tBuf.buf);  
  mbedtls_sha512_ret((uint8_t *)tBuf.buf,tBuf.len(),tHash,1);     // create SHA-384 hash of JSON (can be any hash - just looking for a unique key)

  if(memcmp(tHash,homeSpan.hapConfig.hashCode,48)){           // if hash code of current HAP database does not match stored hash code
    memcpy(homeSpan.hapConfig.hashCode,tHash,48);             // update stored hash code
    homeSpan.hapConfig.configNumber++;                        // increment configuration number
    if(homeSpan.hapConfig.configNumber==65536)                // reached max value
      homeSpan.hapConfig.configNumber=1;                      // reset to 1
                   
    Serial.print("Accessory configuration has changed.  Updating configuration number to ");
    Serial.print(homeSpan.hapConfig.configNumber);
    Serial.print("\n\n");
    nvs_set_blob(nvsHandle,"HAPHASH",&homeSpan.hapConfig,sizeof(homeSpan.hapConfig));     // update data
    nvs_commit(nvsHandle);                                                                // commit to NVS
  } else {
    Serial.print("Accessory configuration number: ");
    Serial.print(homeSpan.hapConfig.configNumber);
    Serial.print("\n\n");    
  }
  
}

//////////////////////////////////////

void HAPClient::processRequest(){

  int nBytes;
  
  if(cPair){                           // expecting encrypted message
    LOG2("<<<< #### ");
    LOG2(client.remoteIP());
    LOG2(" #### <<<<\n");

    nBytes=receiveEncrypted();           // decrypt and return number of bytes       
        
    if(!nBytes){                        // decryption failed (error message already printed in function)
      badRequestError();              
      return;          
    }
        
  } else {                                            // expecting plaintext message  
    LOG2("<<<<<<<<< ");
    LOG2(client.remoteIP());
    LOG2(" <<<<<<<<<\n");
    
    nBytes=client.read(httpBuf,MAX_HTTP+1);   // read all available bytes up to maximum allowed+1
       
    if(nBytes>MAX_HTTP){                              // exceeded maximum number of bytes allowed
      badRequestError();
      Serial.print("\n*** ERROR:  Exceeded maximum HTTP message length\n\n");
      return;
    }
        
  } // encrypted/plaintext
      
  httpBuf[nBytes]='\0';         // add null character to enable string functions
      
  char *body=(char *)httpBuf;   // char pointer to start of HTTP Body
  char *p;                          // char pointer used for searches
      
  if(!(p=strstr((char *)httpBuf,"\r\n\r\n"))){
    badRequestError();
    Serial.print("\n*** ERROR:  Malformed HTTP request (can't find blank line indicating end of BODY)\n\n");
    return;      
  }

  *p='\0';                            // null-terminate end of HTTP Body to faciliate additional string processing
  uint8_t *content=(uint8_t *)p+4;    // byte pointer to start of optional HTTP Content
  int cLen=0;                         // length of optional HTTP Content

  if((p=strstr(body,"Content-Length: ")))       // Content-Length is specified
    cLen=atoi(p+16);
  if(nBytes!=strlen(body)+4+cLen){
    badRequestError();
    Serial.print("\n*** ERROR:  Malformed HTTP request (Content-Length plus Body Length does not equal total number of bytes read)\n\n");
    return;        
  }

  LOG2(body);
  LOG2("\n------------ END BODY! ------------\n");

  if(!strncmp(body,"POST ",5)){                       // this is a POST request

    if(cLen==0){
      badRequestError();
      Serial.print("\n*** ERROR:  HTTP POST request contains no Content\n\n");
      return;      
    }
           
    if(!strncmp(body,"POST /pair-setup ",17) &&                              // POST PAIR-SETUP
       strstr(body,"Content-Type: application/pairing+tlv8") &&              // check that content is TLV8
       tlv8.unpack(content,cLen)){                                          // read TLV content
       tlv8.print();                                                        // print TLV records in form "TAG(INT) LENGTH(INT) VALUES(HEX)"
      LOG2("------------ END TLVS! ------------\n");
               
      postPairSetupURL();                   // process URL
      return;
    }

    if(!strncmp(body,"POST /pair-verify ",18) &&                             // POST PAIR-VERIFY
       strstr(body,"Content-Type: application/pairing+tlv8") &&              // check that content is TLV8
       tlv8.unpack(content,cLen)){                                          // read TLV content
       tlv8.print();                                                        // print TLV records in form "TAG(INT) LENGTH(INT) VALUES(HEX)"
      LOG2("------------ END TLVS! ------------\n");
               
      postPairVerifyURL();                  // process URL    
      return;
    }
            
    if(!strncmp(body,"POST /pairings ",15) &&                                // POST PAIRINGS
       strstr(body,"Content-Type: application/pairing+tlv8") &&              // check that content is TLV8
       tlv8.unpack(content,cLen)){                                          // read TLV content
       tlv8.print();                                                        // print TLV records in form "TAG(INT) LENGTH(INT) VALUES(HEX)"
      LOG2("------------ END TLVS! ------------\n");
               
      postPairingsURL();                  // process URL    
      return;
    }

    if(!strncmp(body,"POST /pairings ",15) &&                                // POST PAIRINGS
       strstr(body,"Content-Type: application/pairing+tlv8") &&              // check that content is TLV8
       tlv8.unpack(content,cLen)){                                          // read TLV content
       tlv8.print();                                                        // print TLV records in form "TAG(INT) LENGTH(INT) VALUES(HEX)"
      LOG2("------------ END TLVS! ------------\n");
               
      postPairingsURL();                  // process URL    
      return;
    }

    notFoundError();
    Serial.print("\n*** ERROR:  Bad POST request - URL not found\n\n");
    return;                  
        
  } // POST request
          
  if(!strncmp(body,"PUT ",4)){                       // this is a PUT request

    if(cLen==0){
      badRequestError();
      Serial.print("\n*** ERROR:  HTTP PUT request contains no Content\n\n");
      return;      
    }
           
    if(!strncmp(body,"PUT /characteristics ",21) &&                          // PUT CHARACTERISTICS
       strstr(body,"Content-Type: application/hap+json")){                   // check that content is JSON

      content[cLen]='\0';                                                    // add a trailing null on end of JSON
      LOG2((char *)content);                                         // print JSON
      LOG2("\n------------ END JSON! ------------\n");
               
      putCharacteristicsURL((char *)content);                           // process URL
      return;
    }
      
    notFoundError();
    Serial.print("\n*** ERROR:  Bad PUT request - URL not found\n\n");
    return;                  
        
  } // PUT request           
      
  if(!strncmp(body,"GET ",4)){                       // this is a GET request
                    
    if(!strncmp(body,"GET /accessories ",17)){       // GET ACCESSORIES
      getAccessoriesURL();
      return;
    }

    if(!strncmp(body,"GET /characteristics?",21)){   // GET CHARACTERISTICS
      getCharacteristicsURL(body+21);
      return;
    }

    notFoundError();
    Serial.print("\n*** ERROR:  Bad GET request - URL not found\n\n");
    return;                  

  } // GET request
      
  badRequestError();
  Serial.print("\n*** ERROR:  Unknown or malformed HTTP request\n\n");
                        
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

int HAPClient::postPairSetupURL(){

  LOG1("In Pair Setup...");
  
  int tlvState=tlv8.val(kTLVType_State);
  char buf[64];

  if(tlvState==-1){                                           // missing STATE TLV
    Serial.print("\n*** ERROR: Missing <M#> State TLV\n\n");
    badRequestError();                                        // return with 400 error, which closes connection      
    return(0);
  }

  if(nAdminControllers()){                              // error: Device already paired (i.e. there is at least one admin Controller). We should not be receiving any requests for Pair-Setup!
    Serial.print("\n*** ERROR: Device already paired!\n\n");
    tlv8.clear();                                         // clear TLV records
    tlv8.val(kTLVType_State,tlvState+1);                  // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    tlv8.val(kTLVType_Error,tagError_Unavailable);       // set Error=Unavailable
    tlvRespond();                                       // send response to client
    return(0);
  };

  sprintf(buf,"Found <M%d>.  Expected <M%d>\n",tlvState,pairStatus);
  LOG2(buf);

  if(tlvState!=pairStatus){                             // error: Device is not yet paired, but out-of-sequence pair-setup STATE was received
    Serial.print("\n*** ERROR: Out-of-Sequence Pair-Setup request!\n\n");
    tlv8.clear();                                         // clear TLV records
    tlv8.val(kTLVType_State,tlvState+1);                  // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for out-of-sequence steps)
    tlvRespond();                                       // send response to client
    pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired accessory (M1)
    return(0);
  };
   
  switch(tlvState){          // valid and in-sequence Pair-Setup STATE received -- process request!  (HAP Section 5.6)

    case pairState_M1:                     // 'SRP Start Request'

      if(tlv8.val(kTLVType_Method)!=0){                       // error: "Pair Setup" method must always be 0 to indicate setup without MiFi Authentification (HAP Table 5-3)
        Serial.print("\n*** ERROR: Pair Method not set to 0\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Unavailable);       // set Error=Unavailable
        tlvRespond();                                       // send response to client
        return(0);
      };

      tlv8.clear();
      tlv8.val(kTLVType_State,pairState_M2);            // set State=<M2>
      srp.createPublicKey();                          // create accessory public key from random Pair-Setup code (displayed to user)
      srp.loadTLV(kTLVType_PublicKey,&srp.B);         // load server public key, B (MUST MAKE THIS A LIVE CALCULATION TO GENERATE RANDOM SET-UP CODE)
      srp.loadTLV(kTLVType_Salt,&srp.s);              // load salt, s (MUST MAKE THIS RANDOM AS WELL)
      tlvRespond();                                   // send response to client

      pairStatus=pairState_M3;                        // set next expected pair-state request from client
      return(1);
      
    break;

    case pairState_M3:                     // 'SRP Verify Request'

      if(!srp.writeTLV(kTLVType_PublicKey,&srp.A) ||    // try to write TLVs into mpi structures
         !srp.writeTLV(kTLVType_Proof,&srp.M1)){
            
        Serial.print("\n*** ERROR: One or both of the required 'PublicKey' and 'Proof' TLV records for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);
      };

      srp.createSessionKey();                               // create session key, K, from receipt of HAP Client public key, A

      if(!srp.verifyProof()){                               // verify proof, M1, received from HAP Client
        Serial.print("\n*** ERROR: SRP Proof Verification Failed\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);        
      };

      srp.createProof();                                  // M1 has been successully verified; now create accessory proof M2
      tlv8.clear();                                         // clear TLV records
      tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
      srp.loadTLV(kTLVType_Proof,&srp.M2);                // load M2 counter-proof
      tlvRespond();                                       // send response to client

      pairStatus=pairState_M5;                            // set next expected pair-state request from client
      return(1);        
        
    break;
    
    case pairState_M5:                     // 'Exchange Request'

      if(!tlv8.buf(kTLVType_EncryptedData)){            
        Serial.print("\n*** ERROR: Required 'EncryptedData' TLV record for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M6);                // set State=<M6>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);
      };

      // THIS NEXT STEP IS MISSING FROM HAP DOCUMENTATION!
      //
      // Must FIRST use HKDF to create a Session Key from the SRP Shared Secret for use in subsequent ChaCha20-Poly1305 decryption
      // of the encrypted data TLV (HAP Sections 5.6.5.2 and 5.6.6.1).
      //
      // Note the SALT and INFO text fields used by HKDF to create this Session Key are NOT the same as those for creating iosDeviceX.
      // The iosDeviceX HKDF calculations are separate and will be performed further below with the SALT and INFO as specified in the HAP docs.

      hkdf.create(sessionKey, srp.sharedSecret,64,"Pair-Setup-Encrypt-Salt","Pair-Setup-Encrypt-Info");       // create SessionKey

      uint8_t decrypted[1024];                    // temporary storage for decrypted data
      unsigned long long decryptedLen;            // length (in bytes) of decrypted data
      
      if(crypto_aead_chacha20poly1305_ietf_decrypt(                                  // use SessionKey to decrypt encryptedData TLV with padded nonce="PS-Msg05"
        decrypted, &decryptedLen, NULL,
        tlv8.buf(kTLVType_EncryptedData), tlv8.len(kTLVType_EncryptedData), NULL, 0,
        (unsigned char *)"\x00\x00\x00\x00PS-Msg05", sessionKey)==-1){
          
        Serial.print("\n*** ERROR: Exchange-Request Authentication Failed\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M6);                // set State=<M6>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);        
      }

      if(!tlv8.unpack(decrypted,decryptedLen)){
        Serial.print("\n*** ERROR: Can't parse decrypted data into separate TLV records\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M6);                // set State=<M6>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);
      }

      tlv8.print();             // print decrypted TLV data
      LOG2("------- END DECRYPTED TLVS! -------\n");
       
      if(!tlv8.buf(kTLVType_Identifier) || !tlv8.buf(kTLVType_PublicKey) || !tlv8.buf(kTLVType_Signature)){            
        Serial.print("\n*** ERROR: One or more of required 'Identifier,' 'PublicKey,' and 'Signature' TLV records for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M6);                // set State=<M6>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);
      };

      // Next, verify the authenticity of the TLV Records using the Signature provided by the Client.
      // But the Client does not send the entire message that was used to generate the Signature.
      // Rather, it purposely does not transmit "iosDeviceX", which is derived from the SRP Shared Secret that only the Client and this Server know.
      // Note that the SALT and INFO text fields now match those in HAP Section 5.6.6.1

      uint8_t iosDeviceX[32];
      hkdf.create(iosDeviceX,srp.sharedSecret,64,"Pair-Setup-Controller-Sign-Salt","Pair-Setup-Controller-Sign-Info");       // derive iosDeviceX from SRP Shared Secret using HKDF 
      size_t iosDeviceXLen=32;

      uint8_t *iosDevicePairingID = tlv8.buf(kTLVType_Identifier);        // set iosDevicePairingID from TLV record
      size_t iosDevicePairingIDLen = tlv8.len(kTLVType_Identifier);

      uint8_t *iosDeviceLTPK = tlv8.buf(kTLVType_PublicKey);              // set iosDeviceLTPK (Ed25519 long-term public key) from TLV record
      size_t iosDeviceLTPKLen = tlv8.len(kTLVType_PublicKey);

      size_t iosDeviceInfoLen=iosDeviceXLen+iosDevicePairingIDLen+iosDeviceLTPKLen;             // total size of re-constituted message, iosDeviceInfo
      uint8_t iosDeviceInfo[iosDeviceInfoLen];

      memcpy(iosDeviceInfo,iosDeviceX,iosDeviceXLen);                                                      // iosDeviceInfo = iosDeviceX
      memcpy(iosDeviceInfo+iosDeviceXLen,iosDevicePairingID,iosDevicePairingIDLen);                        // +iosDevicePairingID
      memcpy(iosDeviceInfo+iosDeviceXLen+iosDevicePairingIDLen,iosDeviceLTPK,iosDeviceLTPKLen);            // +iosDeviceLTPK

      uint8_t *iosDeviceSignature = tlv8.buf(kTLVType_Signature);                               // set iosDeviceSignature from TLV record (an Ed25519 should always be 64 bytes)

      if(crypto_sign_verify_detached(iosDeviceSignature, iosDeviceInfo, iosDeviceInfoLen, iosDeviceLTPK) != 0){         // verify signature of iosDeviceInfo using iosDeviceLTPK   
        Serial.print("\n*** ERROR: LPTK Signature Verification Failed\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M6);                // set State=<M6>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        pairStatus=pairState_M1;                            // reset pairStatus to first step of unpaired
        return(0);                
      }

      addController(iosDevicePairingID,iosDeviceLTPK,true);        // save Pairing ID and LTPK for this Controller with admin privileges

      nvs_set_blob(nvsHandle,"CONTROLLERS",controllers,sizeof(controllers));      // update data
      nvs_commit(nvsHandle);                                                      // commit to NVS

      // Now perform the above steps in reverse to securely transmit the AccessoryLTPK to the Controller (HAP Section 5.6.6.2)

      uint8_t accessoryX[32];
      hkdf.create(accessoryX,srp.sharedSecret,64,"Pair-Setup-Accessory-Sign-Salt","Pair-Setup-Accessory-Sign-Info");       // derive accessoryX from SRP Shared Secret using HKDF 
      size_t accessoryXLen=32;
      
      uint8_t *accessoryPairingID=accessory.ID;                    // set accessoryPairingID from storage
      size_t accessoryPairingIDLen=17;

      uint8_t *accessoryLTPK=accessory.LTPK;                       // set accessoryLTPK (Ed25519 long-term public key) from storage
      size_t accessoryLTPKLen=32;

      size_t accessoryInfoLen=accessoryXLen+accessoryPairingIDLen+accessoryLTPKLen;             // total size of accessoryInfo
      uint8_t accessoryInfo[accessoryInfoLen];

      memcpy(accessoryInfo,accessoryX,accessoryXLen);                                                      // accessoryInfo = accessoryX
      memcpy(accessoryInfo+accessoryXLen,accessoryPairingID,accessoryPairingIDLen);                        // +accessoryPairingID
      memcpy(accessoryInfo+accessoryXLen+accessoryPairingIDLen,accessoryLTPK,accessoryLTPKLen);            // +accessoryLTPK

      tlv8.clear();       // clear existing TLV records

      crypto_sign_detached(tlv8.buf(kTLVType_Signature,64),NULL,accessoryInfo,accessoryInfoLen,accessory.LTSK);  // produce signature of accessoryInfo using AccessoryLTSK (Ed25519 long-term secret key)

      memcpy(tlv8.buf(kTLVType_Identifier,accessoryPairingIDLen),accessoryPairingID,accessoryPairingIDLen);   // set Identifier TLV record as accessoryPairingID
      memcpy(tlv8.buf(kTLVType_PublicKey,accessoryLTPKLen),accessoryLTPK,accessoryLTPKLen);                   // set PublicKey TLV record as accessoryLTPK

      LOG2("------- ENCRYPTING SUB-TLVS -------\n");

      tlv8.print();

      size_t subTLVLen=tlv8.pack(NULL);                 // get size of buffer needed to store sub-TLV 
      uint8_t subTLV[subTLVLen];
      subTLVLen=tlv8.pack(subTLV);                      // create sub-TLV by packing Identifier, PublicKey, and Signature TLV records together

      tlv8.clear();         // clear existing TLV records

      // Final step is to encrypt the subTLV data using the same sessionKey as above with ChaCha20-Poly1305 
      
      unsigned long long edLen;

      crypto_aead_chacha20poly1305_ietf_encrypt(tlv8.buf(kTLVType_EncryptedData),&edLen,subTLV,subTLVLen,NULL,0,NULL,(unsigned char *)"\x00\x00\x00\x00PS-Msg06",sessionKey);
                                              
      LOG2("---------- END SUB-TLVS! ----------\n");

      tlv8.buf(kTLVType_EncryptedData,edLen);       // set length of EncryptedData TLV record, which should now include the Authentication Tag at the end as required by HAP
      tlv8.val(kTLVType_State,pairState_M6);        // set State=<M6>
      
      tlvRespond();                        // send response to client

      mdns_service_txt_item_set("_hap","_tcp","sf","0");           // broadcast new status
      
      LOG1("\n*** ACCESSORY PAIRED! ***\n");
      
      return(1);        
             
    break;

  } // switch

} // postPairSetup

//////////////////////////////////////

int HAPClient::postPairVerifyURL(){

  LOG2("In Pair Verify #");
  LOG2(conNum);
  LOG2(" (");
  LOG2(client.remoteIP());
  LOG2(")...");
  
  char buf[64];
  
  int tlvState=tlv8.val(kTLVType_State);

  if(tlvState==-1){                                           // missing STATE TLV
    Serial.print("\n*** ERROR: Missing <M#> State TLV\n\n");
    badRequestError();                                        // return with 400 error, which closes connection      
    return(0);
  }

  if(!nAdminControllers()){                             // error: Device not yet paired - we should not be receiving any requests for Pair-Verify!
    Serial.print("\n*** ERROR: Device not yet paired!\n\n");
    tlv8.clear();                                         // clear TLV records
    tlv8.val(kTLVType_State,tlvState+1);                  // set response STATE to requested state+1 (which should match the state that was expected by the controller)
    tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown
    tlvRespond();                                       // send response to client
    return(0);
  };

  sprintf(buf,"Found <M%d>\n",tlvState);                 // unlike pair-setup, out-of-sequencing can be handled gracefully for pair-verify (HAP requirement). No need to keep track of pairStatus
  LOG2(buf);

  switch(tlvState){          // Pair-Verify STATE received -- process request!  (HAP Section 5.7)

    case pairState_M1:                     // 'Verify Start Request'

      if(!tlv8.buf(kTLVType_PublicKey)){            
        Serial.print("\n*** ERROR: Required 'PublicKey' TLV record for this step is bad or missing\n\n");
        tlv8.clear();                                     // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);            // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Unknown);       // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                   // send response to client
        return(0);
        
      } else {

        uint8_t secretCurveKey[32];     // Accessory's secret key for Curve25519 encryption (32 bytes).  Ephemeral usage - created below and used only in this block

        crypto_box_keypair(publicCurveKey,secretCurveKey);         // generate Curve25519 public key pair (will persist until end of verification process)

        memcpy(iosCurveKey,tlv8.buf(kTLVType_PublicKey),32);       // save iosCurveKey (will persist until end of verification process)

        crypto_scalarmult_curve25519(sharedCurveKey,secretCurveKey,iosCurveKey);      // generate (and persist) Pair Verify SharedSecret CurveKey from Accessory's Curve25519 secret key and Controller's Curve25519 public key (32 bytes)

        uint8_t *accessoryPairingID = accessory.ID;                    // set accessoryPairingID
        size_t accessoryPairingIDLen = 17;

        size_t accessoryInfoLen=32+accessoryPairingIDLen+32;           // total size of accessoryInfo
        uint8_t accessoryInfo[accessoryInfoLen];           

        memcpy(accessoryInfo,publicCurveKey,32);                                        // accessoryInfo = Accessory's Curve25519 public key
        memcpy(accessoryInfo+32,accessoryPairingID,accessoryPairingIDLen);              // +accessoryPairingID
        memcpy(accessoryInfo+32+accessoryPairingIDLen,iosCurveKey,32);                  // +Controller's Curve25519 public key

        tlv8.clear();       // clear existing TLV records

        crypto_sign_detached(tlv8.buf(kTLVType_Signature,64),NULL,accessoryInfo,accessoryInfoLen,accessory.LTSK);  // produce signature of accessoryInfo using AccessoryLTSK (Ed25519 long-term secret key)

        memcpy(tlv8.buf(kTLVType_Identifier,accessoryPairingIDLen),accessoryPairingID,accessoryPairingIDLen);   // set Identifier TLV record as accessoryPairingID

        LOG2("------- ENCRYPTING SUB-TLVS -------\n");

        tlv8.print();

        size_t subTLVLen=tlv8.pack(NULL);                 // get size of buffer needed to store sub-TLV 
        uint8_t subTLV[subTLVLen];
        subTLVLen=tlv8.pack(subTLV);                      // create sub-TLV by packing Identifier and Signature TLV records together

        tlv8.clear();         // clear existing TLV records

        // create SessionKey from Curve25519 SharedSecret using HKDF-SHA-512, then encrypt subTLV data with SessionKey using ChaCha20-Poly1305.  Output stored in EncryptedData TLV
      
        unsigned long long edLen;

        hkdf.create(sessionKey,sharedCurveKey,32,"Pair-Verify-Encrypt-Salt","Pair-Verify-Encrypt-Info");       // create SessionKey (32 bytes)

        crypto_aead_chacha20poly1305_ietf_encrypt(tlv8.buf(kTLVType_EncryptedData),&edLen,subTLV,subTLVLen,NULL,0,NULL,(unsigned char *)"\x00\x00\x00\x00PV-Msg02",sessionKey);
                                              
        LOG2("---------- END SUB-TLVS! ----------\n");
        
        tlv8.buf(kTLVType_EncryptedData,edLen);                           // set length of EncryptedData TLV record, which should now include the Authentication Tag at the end as required by HAP
        tlv8.val(kTLVType_State,pairState_M2);                            // set State=<M2>
        memcpy(tlv8.buf(kTLVType_PublicKey,32),publicCurveKey,32);        // set PublicKey to Accessory's Curve25519 public key
      
        tlvRespond();                        // send response to client
        return(1);        
      }
      
    break;  
   
    case pairState_M3:                     // 'Verify Finish Request'

      if(!tlv8.buf(kTLVType_EncryptedData)){            
        Serial.print("\n*** ERROR: Required 'EncryptedData' TLV record for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        return(0);
      };

      uint8_t decrypted[1024];                    // temporary storage for decrypted data
      unsigned long long decryptedLen;            // length (in bytes) of decrypted data
      
      if(crypto_aead_chacha20poly1305_ietf_decrypt(                                            // use SessionKey to decrypt encrypytedData TLV with padded nonce="PV-Msg03"
        decrypted, &decryptedLen, NULL,
        tlv8.buf(kTLVType_EncryptedData), tlv8.len(kTLVType_EncryptedData), NULL, 0,
        (unsigned char *)"\x00\x00\x00\x00PV-Msg03", sessionKey)==-1){
          
        Serial.print("\n*** ERROR: Verify Authentication Failed\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        return(0);        
      }

      if(!tlv8.unpack(decrypted,decryptedLen)){
        Serial.print("\n*** ERROR: Can't parse decrypted data into separate TLV records\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        return(0);
      }

      tlv8.print();             // print decrypted TLV data
      LOG2("------- END DECRYPTED TLVS! -------\n");

      if(!tlv8.buf(kTLVType_Identifier) || !tlv8.buf(kTLVType_Signature)){            
        Serial.print("\n*** ERROR: One or more of required 'Identifier,' and 'Signature' TLV records for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        tlvRespond();                                       // send response to client
        return(0);
      };

      Controller *tPair;                                  // temporary pointer to Controller

      if(!(tPair=findController(tlv8.buf(kTLVType_Identifier)))){
        Serial.print("\n*** ERROR: Unrecognized Controller PairingID\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        return(0);
      }

      size_t iosDeviceInfoLen=32+36+32;
      uint8_t iosDeviceInfo[iosDeviceInfoLen];

      memcpy(iosDeviceInfo,iosCurveKey,32);
      memcpy(iosDeviceInfo+32,tPair->ID,36);
      memcpy(iosDeviceInfo+32+36,publicCurveKey,32);
      
      if(crypto_sign_verify_detached(tlv8.buf(kTLVType_Signature), iosDeviceInfo, iosDeviceInfoLen, tPair->LTPK) != 0){         // verify signature of iosDeviceInfo using iosDeviceLTPK   
        Serial.print("\n*** ERROR: LPTK Signature Verification Failed\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        tlvRespond();                                       // send response to client
        return(0);                
      }

      tlv8.clear();                                         // clear TLV records
      tlv8.val(kTLVType_State,pairState_M4);                // set State=<M4>
      tlvRespond();                                       // send response to client (unencrypted since cPair=NULL)

      cPair=tPair;        // save Controller for this connection slot - connection is not verified and should be encrypted going forward

      hkdf.create(a2cKey,sharedCurveKey,32,"Control-Salt","Control-Read-Encryption-Key");        // create AccessoryToControllerKey (HAP Section 6.5.2)
      hkdf.create(c2aKey,sharedCurveKey,32,"Control-Salt","Control-Write-Encryption-Key");       // create ControllerToAccessoryKey (HAP Section 6.5.2)
      
      a2cNonce.zero();         // reset Nonces for this session to zero
      c2aNonce.zero();

      LOG2("\n*** SESSION VERIFICATION COMPLETE *** \n");
      return(1);

    break;
  
  } // switch
  
} // postPairVerify

//////////////////////////////////////

int HAPClient::getAccessoriesURL(){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Get Accessories #");
  LOG1(conNum);
  LOG1(" (");
  LOG1(client.remoteIP());
  LOG1(")...\n");

  int nBytes = homeSpan.sprintfAttributes(NULL);        // get size of HAP attributes JSON
  TempBuffer <char> jBuf(nBytes+1);
  homeSpan.sprintfAttributes(jBuf.buf);                  // create JSON database (will need to re-cast to uint8_t* below)

  int nChars=snprintf(NULL,0,"HTTP/1.1 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);      // create '200 OK' Body with Content Length = size of JSON Buf
  char body[nChars+1];
  sprintf(body,"HTTP/1.1 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);
  
  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(body);
  LOG2(jBuf.buf);
  LOG2("\n");
  
  sendEncrypted(body,(uint8_t *)jBuf.buf,nBytes);
  
  LOG2("-------- SENT ENCRYPTED! --------\n");    
      
  return(1);
  
} // getAccessories

//////////////////////////////////////

int HAPClient::postPairingsURL(){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  Controller *newCont;

  LOG1("In Post Pairings #");
  LOG1(conNum);
  LOG1(" (");  
  LOG1(client.remoteIP());
  LOG1(")...");

  if(tlv8.val(kTLVType_State)!=1){
    Serial.print("\n*** ERROR: 'State' TLV record is either missing or not set to <M1> as required\n\n");
    badRequestError();                                        // return with 400 error, which closes connection      
    return(0);
  }

  switch(tlv8.val(kTLVType_Method)){

    case 3:
      LOG1("Add...\n");

      if(!tlv8.buf(kTLVType_Identifier) || !tlv8.buf(kTLVType_PublicKey) || !tlv8.buf(kTLVType_Permissions)){            
        Serial.print("\n*** ERROR: One or more of required 'Identifier,' 'PublicKey,' and 'Permissions' TLV records for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        break;
      }

      if(!cPair->admin){
        Serial.print("\n*** ERROR: Controller making request does not have admin privileges to add/update other Controllers\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        break;        
      }

      if(newCont=findController(tlv8.buf(kTLVType_Identifier))){
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        if(!memcmp(cPair->LTPK,newCont->LTPK,32)){                       // requested Controller already exists and LTPK matches
          newCont->admin=tlv8.val(kTLVType_Permissions)==1?true:false;     // update permission of matching Controller
        } else {
          tlv8.val(kTLVType_Error,tagError_Unknown);         // set Error=Unknown
        }
        break;
      }
      
      if(!(newCont=getFreeController())){
        Serial.print("\n*** ERROR: Can't pair more than ");
        Serial.print(MAX_CONTROLLERS);
        Serial.print(" Controllers\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_MaxPeers);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        break;        
      }

      addController(tlv8.buf(kTLVType_Identifier),tlv8.buf(kTLVType_PublicKey),tlv8.val(kTLVType_Permissions)==1?true:false);
      
      tlv8.clear();                                         // clear TLV records
      tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
      break;

    case 4:
      LOG1("Remove...\n");

      if(!tlv8.buf(kTLVType_Identifier)){            
        Serial.print("\n*** ERROR: Required 'Identifier' TLV record for this step is bad or missing\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Unknown);           // set Error=Unknown (there is no specific error type for missing/bad TLV data)
        break;
      }

      if(!cPair->admin){
        Serial.print("\n*** ERROR: Controller making request does not have admin privileges to remove Controllers\n\n");
        tlv8.clear();                                         // clear TLV records
        tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
        tlv8.val(kTLVType_Error,tagError_Authentication);    // set Error=Authentication
        break;        
      }

      removeController(tlv8.buf(kTLVType_Identifier));
      
      tlv8.clear();                                         // clear TLV records
      tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
      break;
      
    case 5:                     
      LOG1("List...\n");

      // NEEDS TO BE IMPLEMENTED

      tlv8.clear();                                         // clear TLV records
      tlv8.val(kTLVType_State,pairState_M2);                // set State=<M2>
      break;

    default:
      Serial.print("\n*** ERROR: 'Method' TLV record is either missing or not set to either 3, 4, or 5 as required\n\n");
      badRequestError();                                    // return with 400 error, which closes connection      
      return(0);
      break;      
  }

  nvs_set_blob(nvsHandle,"CONTROLLERS",controllers,sizeof(controllers));      // update Controller data
  nvs_commit(nvsHandle);                                                      // commit to NVS

  tlvRespond();

  // re-check connections and close any (or all) clients as a result of controllers that were removed above
  // must be performed AFTER sending the TLV response, since that connection itself may be terminated below

  for(int i=0;i<MAX_CONNECTIONS;i++){     // loop over all connection slots
    if(hap[i].client){                    // if slot is connected
      
      if(!nAdminControllers() || (hap[i].cPair && !hap[i].cPair->allocated)){    // accessory unpaired, OR client connection is verified but points to an unallocated controller
        LOG1("*** Terminating Client #");
        LOG1(i);
        LOG1("\n");
        hap[i].client.stop();
      }
      
    } // if client connected
  } // loop over all connection slots
  
  return(1);
}

//////////////////////////////////////

int HAPClient::getCharacteristicsURL(char *urlBuf){


  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Get Characteristics #");
  LOG1(conNum);
  LOG1(" (");
  LOG1(client.remoteIP());
  LOG1(")...\n");

  int len=strlen(urlBuf);       // determine number of IDs specificed by counting commas in URL
  int numIDs=1;
  for(int i=0;i<len;i++)
    if(urlBuf[i]==',')
      numIDs++;
  
  char *ids[numIDs];            // reserve space for number of IDs found
  int flags=GET_AID;            // flags indicating which characteristic fields to include in response (HAP Table 6-13)
  numIDs=0;                     // reset number of IDs found

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

  int nBytes=homeSpan.sprintfAttributes(ids,numIDs,flags,NULL);          // get JSON response - includes terminating null (will be recast to uint8_t* below)
  char jsonBuf[nBytes+1];
  homeSpan.sprintfAttributes(ids,numIDs,flags,jsonBuf);

  boolean sFlag=strstr(jsonBuf,"status");          // status attribute found?

  int nChars=snprintf(NULL,0,"HTTP/1.1 %s\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",!sFlag?"200 OK":"207 Multi-Status",nBytes);   
  char body[nChars+1];    
  sprintf(body,"HTTP/1.1 %s\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",!sFlag?"200 OK":"207 Multi-Status",nBytes);
    
  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");    
  LOG2(body);
  LOG2(jsonBuf);
  LOG2("\n");
  
  sendEncrypted(body,(uint8_t *)jsonBuf,nBytes);        // note recasting of jsonBuf into uint8_t*
  
  LOG2("-------- SENT ENCRYPTED! --------\n");
    
  return(1);
}

//////////////////////////////////////

int HAPClient::putCharacteristicsURL(char *json){

  if(!cPair){                       // unverified, unencrypted session
    unauthorizedError();
    return(0);
  }

  LOG1("In Put Characteristics #");
  LOG1(conNum);
  LOG1(" (");
  LOG1(client.remoteIP());
  LOG1(")...\n");

  int n=homeSpan.countCharacteristics(json);    // count number of objects in JSON request
  if(n==0)                                      // if no objects found, return
    return(0);
 
  SpanPut pObj[n];                                        // reserve space for objects
  if(!homeSpan.updateCharacteristics(json, pObj))         // perform update
    return(0);                                            // return if failed to update (error message will have been printed in update)

  int multiCast=0;                                        // check if all status is OK, or if multicast response is request
  for(int i=0;i<n;i++)
    if(pObj[i].status!=StatusCode::OK)
      multiCast=1;    

  if(!multiCast){                                         // JSON object has no content
    
    char body[]="HTTP/1.1 204 No Content\r\n\r\n";
    
    LOG2("\n>>>>>>>>>> ");
    LOG2(client.remoteIP());
    LOG2(" >>>>>>>>>>\n");
    LOG2(body);  

    sendEncrypted(body,NULL,0);  
    
    LOG2("-------- SENT ENCRYPTED! --------\n");        
    
  } else {                                                       // multicast respose is required

    int nBytes=homeSpan.sprintfAttributes(pObj,n,NULL);          // get JSON response - includes terminating null (will be recast to uint8_t* below)
    char jsonBuf[nBytes+1];
    homeSpan.sprintfAttributes(pObj,n,jsonBuf);

    int nChars=snprintf(NULL,0,"HTTP/1.1 207 Multi-Status\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);      // create Body with Content Length = size of JSON Buf
    char body[nChars+1];
    sprintf(body,"HTTP/1.1 207 Multi-Status\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);
  
    LOG2("\n>>>>>>>>>> ");
    LOG2(client.remoteIP());
    LOG2(" >>>>>>>>>>\n");    
    LOG2(body);
    LOG2(jsonBuf);
    LOG2("\n");
  
    sendEncrypted(body,(uint8_t *)jsonBuf,nBytes);        // note recasting of jsonBuf into uint8_t*
  
    LOG2("-------- SENT ENCRYPTED! --------\n");
  }

  // Create and send Event Notifications if needed

  for(int i=0;i<MAX_CONNECTIONS;i++){                 // loop over all connection slots
    if(hap[i].client && i!=HAPClient::conNum){        // if there is a client connected to this slot and it is NOT the current client requesting this update

      int numNotify=0;
      int nBytes=homeSpan.sprintfNotify(pObj,n,NULL,i,numNotify);          // get JSON response - includes terminating null (will be recast to uint8_t* below)

      if(numNotify){
        numNotify=0;
        char jsonBuf[nBytes+1];
        homeSpan.sprintfNotify(pObj,n,jsonBuf,i,numNotify);

        int nChars=snprintf(NULL,0,"EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);      // create Body with Content Length = size of JSON Buf
        char body[nChars+1];
        sprintf(body,"EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);

        LOG2("\n>>>>>>>>>> ");
        LOG2(hap[i].client.remoteIP());
        LOG2(" >>>>>>>>>>\n");    
        LOG2(body);
        LOG2(jsonBuf);
        LOG2("\n");
  
        hap[i].sendEncrypted(body,(uint8_t *)jsonBuf,nBytes);        // note recasting of jsonBuf into uint8_t*

      } // if there are characteristic updates to notify     
    } // if client exists
  }
    
  return(1);
}

//////////////////////////////////////

void HAPClient::checkNotifications(){

  SpanPBList *pb=homeSpan.pbHead;

  int n=0;

  while(pb){                                    // PASS 1: loop through all characteristics registered as Push Buttons
    if(!pb->characteristic->value.BOOL){        // characteristic is off
      pb->start=false;                          // ensure timer is not started
      pb->trigger=false;                        // turn off trigger
    }
    else if(!pb->start){                        // else characteristic is on but timer is not started
      pb->start=true;                           // start timer
      pb->alarmTime=millis()+pb->waitTime;      // set alarm time
    }
    else if(millis()>pb->alarmTime){            // else characteristic is on, timer is started, and timer is expired
      pb->trigger=true;                         // set trigger
      n++;                                      // increment number of Push Buttons found that need to be turned off
    }
    pb=pb->next;
  }

  if(!n)                                        // nothing to do (either no Push Button characteristics, or none that need to be turned off)
    return;

  SpanPut pObj[n];                              // use a SpanPut object (for convenience) to load characteristics to be updated
  pb=homeSpan.pbHead;                           // reset Push Button list
  n=0;                                          // reset number of PBs found that need to be turned off
  
  while(pb){                                    // PASS 2: loop through all characteristics registered as Push Buttons
    if(pb->trigger){                            // characteristic is triggered
      pb->characteristic->value.BOOL=false;     // turn off characteristic
      pObj[n].status=StatusCode::OK;                     // populate pObj
      pObj[n].characteristic=pb->characteristic;                   
      pObj[n].val="";                           // dummy object needed to ensure sprintfNotify knows to consider this "update"                         
      n++;                                      // increment number of Push Buttons found that need to be turned off
    }
    pb=pb->next;
  }

  for(int i=0;i<MAX_CONNECTIONS;i++){           // loop over all connection slots
    if(hap[i].client){                          // if there is a client connected to this slot

      int numNotify=0;
      int nBytes=homeSpan.sprintfNotify(pObj,n,NULL,i,numNotify);          // get JSON response - includes terminating null (will be recast to uint8_t* below)

      if(numNotify){
        numNotify=0;
        char jsonBuf[nBytes+1];
        homeSpan.sprintfNotify(pObj,n,jsonBuf,i,numNotify);

        int nChars=snprintf(NULL,0,"EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);      // create Body with Content Length = size of JSON Buf
        char body[nChars+1];
        sprintf(body,"EVENT/1.0 200 OK\r\nContent-Type: application/hap+json\r\nContent-Length: %d\r\n\r\n",nBytes);

        LOG2("\n>>>>>>>>>> ");
        LOG2(hap[i].client.remoteIP());
        LOG2(" >>>>>>>>>>\n");    
        LOG2(body);
        LOG2(jsonBuf);
        LOG2("\n");
  
        hap[i].sendEncrypted(body,(uint8_t *)jsonBuf,nBytes);        // note recasting of jsonBuf into uint8_t*

      } // if there are characteristic updates to notify     
    } // if client exists
  }

}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void HAPClient::tlvRespond(){

  int nBytes=tlv8.pack(NULL);      // return number of bytes needed to pack TLV records into a buffer
  uint8_t tlvData[nBytes];         // create buffer
  tlv8.pack(tlvData);              // pack TLV records into buffer

  int nChars=snprintf(NULL,0,"HTTP/1.1 200 OK\r\nContent-Type: application/pairing+tlv8\r\nContent-Length: %d\r\n\r\n",nBytes);      // create Body with Content Length = size of TLV data
  char body[nChars+1];
  sprintf(body,"HTTP/1.1 200 OK\r\nContent-Type: application/pairing+tlv8\r\nContent-Length: %d\r\n\r\n",nBytes);
  
  LOG2("\n>>>>>>>>>> ");
  LOG2(client.remoteIP());
  LOG2(" >>>>>>>>>>\n");
  LOG2(body);
  tlv8.print();

  if(!cPair){                       // unverified, unencrypted session
    client.print(body);
    client.write(tlvData,nBytes);      
    LOG2("------------ SENT! --------------\n");
  } else {
    sendEncrypted(body,tlvData,nBytes);
    LOG2("-------- SENT ENCRYPTED! --------\n");    
  }

} // tlvRespond

//////////////////////////////////////

int HAPClient::receiveEncrypted(){

  uint8_t buf[1042];               // maximum size of encoded message = 2+1024+16 bytes (HAP Section 6.5.2)
  int nFrames=0;
  int nBytes=0;

  while(client.read(buf,2)==2){    // read initial 2-byte AAD record

    int n=buf[0]+buf[1]*256;                // compute number of bytes expected in encoded message

    if(nBytes+n>MAX_HTTP){                  // exceeded maximum number of bytes allowed in plaintext message
      Serial.print("\n\n*** ERROR:  Exceeded maximum HTTP message length\n\n");
      return(0);
      }

    if(client.read(buf+2,n+16)!=n+16){      // read expected number of total bytes = n bytes in encoded message + 16 bytes for appended authentication tag      
      Serial.print("\n\n*** ERROR: Malformed encrypted message frame\n\n");
      return(0);      
    }                

    if(crypto_aead_chacha20poly1305_ietf_decrypt(httpBuf+nBytes, NULL, NULL, buf+2, n+16, buf, 2, c2aNonce.get(), c2aKey)==-1){
      Serial.print("\n\n*** ERROR: Can't Decrypt Message\n\n");
      return(0);        
    }

    c2aNonce.inc();

    nBytes+=n;          // increment total number of bytes in plaintext message
    
  } // while

  return(nBytes);
    
} // receiveEncrypted

//////////////////////////////////////

void HAPClient::sendEncrypted(char *body, uint8_t *dataBuf, int dataLen){

  const int FRAME_SIZE=1024;          // number of bytes to use in each ChaCha20-Poly1305 encrypted frame when sending encrypted JSON content to Client
  
  int bodyLen=strlen(body);

  int count=0;
  unsigned long long nBytes;

  httpBuf[count]=bodyLen%256;         // store number of bytes in first frame that encrypts the Body (AAD bytes)
  httpBuf[count+1]=bodyLen/256;
  
  crypto_aead_chacha20poly1305_ietf_encrypt(httpBuf+count+2,&nBytes,(uint8_t *)body,bodyLen,httpBuf+count,2,NULL,a2cNonce.get(),a2cKey);   // encrypt the Body with authentication tag appended

  a2cNonce.inc();                 // increment nonce
  
  count+=2+bodyLen+16;            // increment count by 2-byte AAD record + length of Body + 16-byte authentication tag

  for(int i=0;i<dataLen;i+=FRAME_SIZE){      // encrypt FRAME_SIZE number of bytes in dataBuf in sequential frames
    
    int n=dataLen-i;           // number of bytes remaining
    
    if(n>FRAME_SIZE)           // maximum number of bytes to encrypt=FRAME_SIZE
      n=FRAME_SIZE;                                     
    
    httpBuf[count]=n%256;    // store number of bytes that encrypts this frame (AAD bytes)
    httpBuf[count+1]=n/256;

    crypto_aead_chacha20poly1305_ietf_encrypt(httpBuf+count+2,&nBytes,dataBuf+i,n,httpBuf+count,2,NULL,a2cNonce.get(),a2cKey);   // encrypt the next portion of dataBuf with authentication tag appended

    a2cNonce.inc();            // increment nonce

    count+=2+n+16;             // increment count by 2-byte AAD record + length of JSON + 16-byte authentication tag
  }
    
  client.write(httpBuf,count);   // transmit all encrypted frames to Client
        
} // sendEncrypted

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void HAPClient::hexPrintColumn(uint8_t *buf, int n){

  char c[16];
  
  for(int i=0;i<n;i++){
    sprintf(c,"%d) %02X",i,buf[i]);
    Serial.println(c);
  }

}

//////////////////////////////////////

void HAPClient::hexPrintRow(uint8_t *buf, int n){

  char c[16];
  
  for(int i=0;i<n;i++){
    sprintf(c,"%02X",buf[i]);
    Serial.print(c);
  }

}

//////////////////////////////////////

void HAPClient::charPrintRow(uint8_t *buf, int n){

  char c[16];
  
  for(int i=0;i<n;i++){
    sprintf(c,"%c",buf[i]);
    Serial.print(c);
  }

}

//////////////////////////////////////

Controller *HAPClient::findController(uint8_t *id){
  
  for(int i=0;i<MAX_CONTROLLERS;i++){         // loop over all controller slots
    
    if(controllers[i].allocated && !memcmp(controllers[i].ID,id,36)){     // found matching ID
      LOG2("Found Controller: ");
      if(DEBUG_LEVEL>1)
        charPrintRow(id,36);
      LOG2(controllers[i].admin?" (admin)\n":" (regular)\n");    
      return(controllers+i);                                              // return with pointer to matching controller
    }
  } // loop

  return(NULL);       // no match
}

//////////////////////////////////////

Controller *HAPClient::getFreeController(){
  
  for(int i=0;i<MAX_CONTROLLERS;i++){     // loop over all controller slots
    
    if(!controllers[i].allocated)         // found free slot
      return(controllers+i);              // return with pointer to free slot
  }

  return(NULL);       // no free slots
}

//////////////////////////////////////

Controller *HAPClient::addController(uint8_t *id, uint8_t *ltpk, boolean admin){

  Controller *slot;

  if(slot=findController(id)){
    memcpy(slot->LTPK,ltpk,32);
    slot->admin=admin;
    LOG2("\n*** Updated Controller: ");
    if(DEBUG_LEVEL>1)
      charPrintRow(id,36);
    LOG2(slot->admin?" (admin)\n\n":" (regular)\n\n");
    return(slot);    
  }

  if(slot=getFreeController()){
    slot->allocated=true;
    memcpy(slot->ID,id,36);
    memcpy(slot->LTPK,ltpk,32);
    slot->admin=admin;
    LOG2("\n*** Added Controller: ");
    if(DEBUG_LEVEL>1)
      charPrintRow(id,36);
    LOG2(slot->admin?" (admin)\n\n":" (regular)\n\n");
    return(slot);       
  }

  Serial.print("\n*** WARNING: No open slots.  Can't add Controller: ");
  hexPrintRow(id,36);
  Serial.print(admin?" (admin)\n\n":" (regular)\n\n\n");
  return(NULL);
}       
  
//////////////////////////////////////

int HAPClient::nAdminControllers(){

  int n=0;
  
  for(int i=0;i<MAX_CONTROLLERS;i++){                         // loop over all controller slots
    n+=(controllers[i].allocated && controllers[i].admin);    // count number of allocated controllers with admin privileges
  }

  return(n);
}
    
//////////////////////////////////////

void HAPClient::removeControllers(){
  
  for(int i=0;i<MAX_CONTROLLERS;i++)
    controllers[i].allocated=false;
}    

//////////////////////////////////////

void HAPClient::removeController(uint8_t *id){

  Controller *slot;

  if(slot=findController(id)){      // remove controller if found
    LOG2("\n***Removed Controller: ");
    if(DEBUG_LEVEL>1)
      charPrintRow(id,36);
    LOG2(slot->admin?" (admin)\n":" (regular)\n");
    slot->allocated=false;

    if(nAdminControllers()==0){       // if no more admins, remove all controllers
      removeControllers();
      LOG1("That was last Admin Controller!  Removing any remaining Regular Controllers and unpairing Accessory\n");  
      mdns_service_txt_item_set("_hap","_tcp","sf","1");           // set Status Flag = 1 (Table 6-8)
    }

    LOG2("\n");
  }

}    

//////////////////////////////////////

void HAPClient::printControllers(){

  int n=0;
  
  for(int i=0;i<MAX_CONTROLLERS;i++){           // loop over all controller slots
    if(controllers[i].allocated){
      Serial.print("Paired Controller: ");
      charPrintRow(controllers[i].ID,36);
      Serial.print(controllers[i].admin?"   (admin)":" (regular)");
      Serial.print("  LTPK: ");
      hexPrintRow(controllers[i].LTPK,32);
      Serial.print("\n");
      n++;
    }
  }

  if(n==0)
    Serial.print("No Paired Controllers\n");
}

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

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// instantiate all static HAP Client structures and data

TLV<kTLVType,10> HAPClient::tlv8;
nvs_handle HAPClient::nvsHandle;                        
uint8_t HAPClient::httpBuf[MAX_HTTP+1];                 
HKDF HAPClient::hkdf;                                   
pairState HAPClient::pairStatus;                        
Accessory HAPClient::accessory;                         
Controller HAPClient::controllers[MAX_CONTROLLERS];    
SRP6A HAPClient::srp;
int HAPClient::conNum;

 
