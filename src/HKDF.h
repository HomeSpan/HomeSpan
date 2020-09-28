
#pragma once

#include <Arduino.h>

/////////////////////////////////////////////////
// HKDF-SHA-512 Structure
//
// This is a wrapper around mbedtls_hkdf, which is NOT
// included in the normal Arduino-ESP32 library.
// Code was instead downloaded from MBED GitHub directly and 
// incorporated under hkdf.cpp, with a wrapper to always
// use SHA-512 with 32 bytes of output as required by HAP.

struct HKDF {
  int create(uint8_t *outputKey, uint8_t *inputKey, int inputLen, char *salt, char *info);    // output of HKDF is always a 32-byte key derived from an input key, a salt string, and an info string
};
