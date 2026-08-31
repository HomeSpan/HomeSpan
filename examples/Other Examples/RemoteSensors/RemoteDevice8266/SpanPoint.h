
#include "Arduino.h"
#include <ESP8266WiFi.h>                 
#include <espnow.h>
#include <bearssl/bearssl.h>
#include <vector>

///////////////////////////////

class MasterKey {

  private:
  
  br_hkdf_context masterContext;

  public:

  MasterKey(const char *password, const char *salt){

	  br_hkdf_init(&masterContext, &br_sha256_vtable, salt, strlen(salt));
    br_hkdf_inject(&masterContext, password, strlen(password));
    br_hkdf_flip(&masterContext);
  }

  void create(const char *keyInfo, uint8_t *newKey, size_t newKeyLen){

    create(keyInfo,strlen(keyInfo),newKey,newKeyLen);   
  }

  void create(const void *keyInfo, size_t keyInfoLen, uint8_t *newKey, size_t newKeyLen){
  
    br_hkdf_context tempContext=masterContext;  
    br_hkdf_produce(&tempContext, keyInfo, keyInfoLen, newKey, newKeyLen);    
  }  
};

///////////////////////////////

class HMAC {

  private:

  br_hmac_key_context kc;

  public:
  
  HMAC(MasterKey *masterKey, const void *keyInfo, size_t keyInfoLen){

    uint8_t authKey[32];
    masterKey->create(keyInfo,keyInfoLen,authKey,32);
    br_hmac_key_init(&kc, &br_sha256_vtable, authKey, 32);
  }

  void create(const void *data, size_t dataLen, uint8_t *hmac){

    br_hmac_context mc;
    br_hmac_init(&mc, &kc, 32);
    br_hmac_update(&mc, data, dataLen);
    br_hmac_out(&mc, hmac);
  }

  boolean verify(const uint8_t *data,  size_t dataLen){

    if(dataLen<33)
      return(false);
    
    dataLen-=32;
    uint8_t hmac[32];
    create(data,dataLen,hmac);
    return(memcmp(data+dataLen,hmac,32)==0);
  }
};

///////////////////////////////

class SimpleQueue {

  size_t depth;
  size_t nBytes;
  uint8_t **queue;
  volatile int index=0;
  volatile int nEntries=0;
  
  public:

  SimpleQueue(size_t depth, size_t nBytes){

    this->depth=depth;
    this->nBytes=nBytes;
    
    queue=(uint8_t **)calloc(depth,sizeof(uint8_t *));
    for(int i=0;i<depth;i++)
      queue[i]=(uint8_t *)calloc(nBytes,sizeof(uint8_t));
  }

  void send(const void *data, boolean overWrite){

    if(nEntries<depth){
      memcpy(queue[index],data,nBytes);
      nEntries++;
      index=(index+1)%depth;
    } else if(depth==1 && overWrite) {
      memcpy(queue[index],data,nBytes);
    }
  }

  boolean receive(void *data){

    if(nEntries==0)
      return(false);

    memcpy(data,queue[(index-nEntries+depth)%depth],nBytes);
    nEntries--;

    return(true);
  }
};

using QueueHandle_t = SimpleQueue*;

#define xQueueCreate(depth, nBytes) new SimpleQueue(depth, nBytes)
#define xQueueSend(queue, data, unused_waitTime) queue->send(data,false)
#define xQueueOverwrite(queue, data, unused_waitTime) queue->send(data,true)
#define xQueueReceive(queue, data, unused_waitTime) queue->receive(data)

///////////////////////////////

class SpanPoint {

public:   // To Be DELETED

  static const int ESP_NOW_KEY_LEN        = 16;
  static const int crypto_auth_BYTES      = 32;
  static const int ESP_NOW_MAX_DATA_LEN   = 250;

  enum esp_now_send_status_t {
    ESP_NOW_SEND_SUCCESS,
    ESP_NOW_SEND_FAIL,
    ESP_NOW_SEND_IDLE
  };

  struct esp_now_peer_info_t {
    uint8_t peer_addr[6];
  };

  union SpAddress {
    struct {
      uint8_t firstByte=0xF2;
      uint8_t devID;
      uint16_t netID;
      uint16_t checkSum;
    };
    uint8_t mac[6];

    SpAddress(uint8_t deviceID, uint16_t networkID){
      devID=deviceID;
      netID=networkID;
      mac[4]=mac[0]^mac[2];
      mac[5]=mac[1]^mac[3];
    }

    boolean isValid() const {
      return(firstByte==0xF2 && mac[4]==mac[0]^mac[2] && mac[5]==mac[1]^mac[3]);
    }
  };

  struct SpConfig_t {
    uint16_t network=1;
    String password="HomeSpan";
    boolean encrypt=true;
    uint16_t channelMask=0x0FFE;
  };

  int receiveSize;                            // size (in bytes) of messages to receive
  int sendSize;                               // size (in bytes) of messages to send
  esp_now_peer_info_t peerInfo;               // structure for all ESP-NOW peer data
  QueueHandle_t receiveQueue;                 // queue to store data after it is received
  boolean overwriteQueue;                     // flag to indicate whether receiving queue should be overridden
  uint32_t receiveTime=0;                     // time (in millis) of most recent data received

  static MasterKey *mKey;
  static HMAC *localHMAC;
    
  static std::vector<SpanPoint *> SpanPoints;

  static SpAddress *deviceAddress;            // SpanPoint Address of this device (will be used for AP Mac)
  static SpConfig_t spConf;                   // stores all configuration settings
  static boolean configured;                  // flag indicating SpanPoint has been configured

  static volatile esp_now_send_status_t sendStatus;   
 
  static void dataReceived(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len);
  static uint8_t nextChannel(uint8_t channel);
  static void initializeChannels();
 
  public:

  static void configure(uint8_t deviceID, SpConfig_t cfg=spConf);
  SpanPoint(uint8_t deviceID, size_t sendSize, size_t receiveSize=0, size_t queueDepth=0);

  boolean send(const void *data);
  boolean get(void *dataBuf);

  uint32_t time(){return(millis()-receiveTime);}
  static const SpAddress *getAddress(){return(deviceAddress);}
};

