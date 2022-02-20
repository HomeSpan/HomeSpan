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

#include <mbedtls/sha512.h>
#include <mbedtls/bignum.h>
#include <mbedtls/base64.h>

#include "HAPConstants.h"

/////////////////////////////////////////////////
// SRP-6A Structure from RFC 5054 (Nov 2007)
// ** HAP uses N=3072-bit Group specified in RFC 5054
// ** HAP replaces H=SHA-1 with H=SHA-512 (HAP Section 5.5)
//
// I = SRP-6A username, defined by HAP to be the word "Pair-Setup"
// P = SRP-6A password, defined to be equal to the accessory's 8-digit setup code in the format "XXX-XX-XXX"

struct SRP6A {

  mbedtls_mpi N;          // N                            - 3072-bit Group pre-defined prime used for all SRP-6A calculations (384 bytes)
  mbedtls_mpi g;          // g                            - pre-defined generator for the specified 3072-bit Group (g=5)
  mbedtls_mpi k;          // k = H(N | PAD(g))            - SRP-6A multiplier (which is different from versions SRP-6 or SRP-3) 
  mbedtls_mpi s;          // s                            - randomly-generated salt (16 bytes)
  mbedtls_mpi x;          // x = H(s | H(I | ":" | P))    - salted, double-hash of username and password (64 bytes)
  mbedtls_mpi v;          // v = g^x %N                   - SRP-6A verifier (max 384 bytes)  
  mbedtls_mpi b;          // b                            - randomly-generated private key for this HAP accessory (i.e. the SRP Server) (32 bytes)
  mbedtls_mpi B;          // B = k*v + g^b %N             - public key for this accessory (max 384 bytes)
  mbedtls_mpi A;          // A                            - public key RECEIVED from HAP Client (max 384 bytes)
  mbedtls_mpi u;          // u = H(PAD(A) | PAB(B))       - "u-factor" (64 bytes)
  mbedtls_mpi S;          // S = (A*v^u)^b %N             - SRP shared "premaster" key, based on accessory private key and client public key (max 384 bytes)
  mbedtls_mpi K;          // K = H( S )                   - SRP SHARED SECRET KEY (64 bytes)
  mbedtls_mpi M1;         // M1                           - proof RECEIVED from HAP Client (64 bytes)
  mbedtls_mpi M1V;        // M1V                          - accessory's independent computation of M1 to verify proof (see code for details of computation)
  mbedtls_mpi M2;         // M2                           - accessory's counter-proof to send to HAP Client after M1=M1V has been verified (64 bytes)
  
  mbedtls_mpi t1;         // temporary mpi structures for intermediate results
  mbedtls_mpi t2;
  mbedtls_mpi t3;

  mbedtls_mpi _rr;        // _rr                          - temporary "helper" for large exponential modulus calculations

  char I[11]="Pair-Setup";  // I                          - userName pre-defined by HAP pairing setup protocol
  char g3072[2]="\x05";     // g                          - 3072-bit Group generator

  uint8_t sharedSecret[64];                        // permanent storage for binary version of SHARED SECRET KEY for ease of use upstream

  SRP6A();                                         // initializes N, G, and computes k
  
  void createVerifyCode(const char *setupCode, uint8_t *verifyCode, uint8_t *salt);
  void loadVerifyCode(uint8_t *verifyCode, uint8_t *salt);
  
  void getSalt();                                  // generates and stores random 16-byte salt, s
  void getPrivateKey();                            // generates and stores random 32-byte private key, b
  void getSetupCode(char *c);                      // generates and displays random 8-digit Pair-Setup code, P, in format XXX-XX-XXX
  void createPublicKey();                          // computes x, v, and B from random s, P, and b
  void createSessionKey();                         // computes u from A and B, and then S from A, v, u, and b
  
  int loadTLV(kTLVType tag, mbedtls_mpi *mpi, int nBytes);     // load binary contents of mpi into a TLV record and set its length
  int writeTLV(kTLVType tag, mbedtls_mpi *mpi);                // write binary contents of a TLV record into an mpi
  
  int verifyProof();                               // verify M1 SRP6A Proof received from HAP client (return 1 on success, 0 on failure)
  void createProof();                              // create M2 server-side SRP6A Proof based on M1 as received from HAP Client

  void print(mbedtls_mpi *mpi);                    // prints size of mpi (in bytes), followed by the mpi itself (as a hex charcter string) - for diagnostic purposes only
  
};
