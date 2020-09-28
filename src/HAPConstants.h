
#pragma once

// HAP TLV Types (HAP Table 5-6)

typedef enum {
  kTLVType_Method=0x00,
  kTLVType_Identifier=0x01,
  kTLVType_Salt=0x02,
  kTLVType_PublicKey=0x03,
  kTLVType_Proof=0x04,
  kTLVType_EncryptedData=0x05,
  kTLVType_State=0x06,
  kTLVType_Error=0x07,
  kTLVType_RetryDelay=0x08,
  kTLVType_Certificate=0x09,
  kTLVType_Signature=0x0A,
  kTLVType_Permissions=0x0B,
  kTLVType_FragmentData=0x0C,
  kTLVType_FragmentLast=0x0D,
  kTLVType_Flags=0x13,
  kTLVType_Separator=0xFF
} kTLVType;


// HAP Error Codes (HAP Table 5-5)

typedef enum {
  tagError_Unknown=0x01,
  tagError_Authentication=0x02,
  tagError_Backoff=0x03,
  tagError_MaxPeers=0x04,
  tagError_MaxTries=0x05,
  tagError_Unavailable=0x06,
  tagError_Busy=0x07
} tagError;


// Pair-Setup and Pair-Verify States

typedef enum {
  pairState_M0=0,
  pairState_M1=1,
  pairState_M2=2,
  pairState_M3=3,
  pairState_M4=4,
  pairState_M5=5,
  pairState_M6=6
} pairState;
