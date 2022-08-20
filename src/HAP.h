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
 
#pragma once

#include <WiFi.h>

#include "HomeSpan.h"
#include "TLV.h"
#include "HAPConstants.h"
#include "HKDF.h"
#include "SRP.h"

/////////////////////////////////////////////////
// NONCE Structure (HAP used last 64 of 96 bits)

struct Nonce {
  uint8_t x[12];
  Nonce();
  void zero();
  uint8_t *get();
  void inc();
};

/////////////////////////////////////////////////
// Paired Controller Structure for Permanently-Stored Data

struct Controller {                
  boolean allocated=false;  // slot is allocated with Controller data
  boolean admin;            // Controller has admin privileges
  uint8_t ID[36];           // Pairing ID
  uint8_t LTPK[32];         // Long Term Ed2519 Public Key
};

/////////////////////////////////////////////////
// Accessory Structure for Permanently-Stored Data

struct Accessory {
  uint8_t ID[17];          // Pairing ID in form "XX:XX:XX:XX:XX:XX"
  uint8_t LTSK[64];        // secret key for Ed25519 signatures
  uint8_t LTPK[32];        // public key for Ed25519 signatures
};

/////////////////////////////////////////////////
// HAPClient Structure
// Reads and Writes from each HAP Client connection

struct HAPClient {

  // common structures and data shared across all HAP Clients

  static const int MAX_HTTP=8095;                     // max number of bytes in HTTP message buffer
  static const int MAX_CONTROLLERS=16;                // maximum number of paired controllers (HAP requires at least 16)
  static const int MAX_ACCESSORIES=41;                // maximum number of allowed Acessories (HAP limit=150, but not enough memory in ESP32 to run that many)
  
  static TLV<kTLVType,10> tlv8;                       // TLV8 structure (HAP Section 14.1) with space for 10 TLV records of type kTLVType (HAP Table 5-6)
  static nvs_handle hapNVS;                           // handle for non-volatile-storage of HAP data
  static nvs_handle srpNVS;                           // handle for non-volatile-storage of SRP data
  static uint8_t httpBuf[MAX_HTTP+1];                 // buffer to store HTTP messages (+1 to leave room for storing an extra 'overflow' character)
  static HKDF hkdf;                                   // generates (and stores) HKDF-SHA-512 32-byte keys derived from an inputKey of arbitrary length, a salt string, and an info string
  static pairState pairStatus;                        // tracks pair-setup status
  static SRP6A srp;                                   // stores all SRP-6A keys used for Pair-Setup
  static Accessory accessory;                         // Accessory ID and Ed25519 public and secret keys- permanently stored
  static Controller controllers[MAX_CONTROLLERS];     // Paired Controller IDs and ED25519 long-term public keys - permanently stored
  static int conNum;                                  // connection number - used to keep track of per-connection EV notifications

  // individual structures and data defined for each Hap Client connection
  
  WiFiClient client=0;            // handle to client
  Controller *cPair;              // pointer to info on current, session-verified Paired Controller (NULL=un-verified, and therefore un-encrypted, connection)
   
  // These keys are generated in the first call to pair-verify and used in the second call to pair-verify so must persist for a short period
    
  uint8_t publicCurveKey[32];     // public key for Curve25519 encryption
  uint8_t sharedCurveKey[32];     // Pair-Verfied Shared Secret key derived from Accessory's epehmeral secretCurveKey and Controller's iosCurveKey
  uint8_t sessionKey[32];         // shared Session Key (derived with various HKDF calls)
  uint8_t iosCurveKey[32];        // Curve25519 public key for associated paired controller

  // CurveKey and CurveKey Nonces are created once each new session is verified in /pair-verify.  Keys persist for as long as connection is open
  
  uint8_t a2cKey[32];             // AccessoryToControllerKey derived from HKDF-SHA-512 of sharedCurveKey (HAP Section 6.5.2)
  uint8_t c2aKey[32];             // ControllerToAccessoryKey derived from HKDF-SHA-512 of sharedCurveKey (HAP Section 6.5.2)
  Nonce a2cNonce;                 // encryption nonce (starts at zero at end of each Pair-Verify and increment every encryption - NOT DOCUMENTED)
  Nonce c2aNonce;                 // decryption nonce (starts at zero at end of each Pair-Verify and increment every encryption - NOT DOCUMENTED)

  // define member methods

  void processRequest();                       // process HAP request  
  int postPairSetupURL();                      // POST /pair-setup (HAP Section 5.6)
  int postPairVerifyURL();                     // POST /pair-verify (HAP Section 5.7)
  int getAccessoriesURL();                     // GET /accessories (HAP Section 6.6)
  int postPairingsURL();                       // POST /pairings (HAP Sections 5.10-5.12)  
  int getCharacteristicsURL(char *urlBuf);     // GET /characteristics (HAP Section 6.7.4)  
  int putCharacteristicsURL(char *json);       // PUT /characteristics (HAP Section 6.7.2)
  int putPrepareURL(char *json);               // PUT /prepare (HAP Section 6.7.2.4)
  int getStatusURL();                          // GET / status (an optional, non-HAP feature)

  void tlvRespond();                                                // respond to client with HTTP OK header and all defined TLV data records (those with length>0)
  void sendEncrypted(char *body, uint8_t *dataBuf, int dataLen);    // send client complete ChaCha20-Poly1305 encrypted HTTP mesage comprising a null-terminated 'body' and 'dataBuf' with 'dataLen' bytes
  int receiveEncrypted();                                           // decrypt HTTP request (HAP Section 6.5)

  int notFoundError();           // return 404 error
  int badRequestError();         // return 400 error
  int unauthorizedError();       // return 470 error

  // define static methods
    
  static void init();                                  // initialize HAP after start-up
    
  static void hexPrintColumn(uint8_t *buf, int n);     // prints 'n' bytes of *buf as HEX, one byte per row.  For diagnostics/debugging only
  static void hexPrintRow(uint8_t *buf, int n);        // prints 'n' bytes of *buf as HEX, all on one row
  static void charPrintRow(uint8_t *buf, int n);       // prints 'n' bytes of *buf as CHAR, all on one row
  
  static Controller *findController(uint8_t *id);                                      // returns pointer to controller with mathching ID (or NULL if no match)
  static Controller *getFreeController();                                              // return pointer to next free controller slot (or NULL if no free slots)
  static Controller *addController(uint8_t *id, uint8_t *ltpk, boolean admin);         // stores data for new Controller with specified data.  Returns pointer to Controller slot on success, else NULL
  static int nAdminControllers();                                                      // returns number of admin Controllers stored
  static void removeControllers();                                                     // removes all Controllers (sets allocated flags to false for all slots)
  static void removeController(uint8_t *id);                                           // removes specific Controller.  If no remaining admin Controllers, remove all others (if any) as per HAP requirements.
  static void printControllers();                                                      // prints IDs of all allocated (paired) Controller
  static void checkNotifications();                                                    // checks for Event Notifications and reports to controllers as needed (HAP Section 6.8)
  static void checkTimedWrites();                                                      // checks for expired Timed Write PIDs, and clears any found (HAP Section 6.7.2.4)
  static void eventNotify(SpanBuf *pObj, int nObj, int ignoreClient=-1);               // transmits EVENT Notifications for nObj SpanBuf objects, pObj, with optional flag to ignore a specific client
};

/////////////////////////////////////////////////
// Extern Variables

extern HAPClient **hap;
