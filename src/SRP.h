/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
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

#include "Utils.h"

/////////////////////////////////////////////////
// Pair-Setup Code Verification Data and Salt

struct Verification {
  uint8_t salt[16];
  uint8_t verifyCode[384];
};

/////////////////////////////////////////////////
// SRP-6A Structure from RFC 5054 (Nov 2007)
// ** HAP uses N=3072-bit Group specified in RFC 5054 with Generator g=5
// ** HAP replaces H=SHA-1 with H=SHA-512 (HAP Section 5.5)
//
// I = SRP-6A username, defined by HAP to be the word "Pair-Setup"
// P = SRP-6A password, defined to be equal to the accessory's 8-digit setup code in the format "XXX-XX-XXX"

struct SRP6A {

  static constexpr char N3072[]="FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74"
                                "020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F1437"
                                "4FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7ED"
                                "EE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF05"
                                "98DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB"
                                "9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3B"
                                "E39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718"
                                "3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33"
                                "A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7"
                                "ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864"
                                "D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E2"
                                "08E24FA074E5AB3143DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF";

  static const uint8_t g3072=5;
  static constexpr char I[]="Pair-Setup";

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
  uint8_t K[64];          // K = H(S)                     - SRP SHARED SECRET KEY (64 bytes)
  uint8_t M1[64];         // M1                           - proof RECEIVED from HAP Client (64 bytes)
  mbedtls_mpi t1;         // temp1                        - temporary mpi structures for intermediate results
  mbedtls_mpi t2;         // temp2                        - temporary mpi structures for intermediate results
  mbedtls_mpi t3;         // temp3                        - temporary mpi structures for intermediate results
  mbedtls_mpi _rr;        // _rr                          - temporary "helper" for large exponential modulus calculations


  SRP6A();                                         // initializes N, G, and computes k
  ~SRP6A();

  void *operator new(size_t size){return(HS_MALLOC(size));}     // override new operator to use PSRAM when available
  
  void createVerifyCode(const char *setupCode, Verification *vData);                    // generates random s and computes v; writes back resulting Verification Data
  void createPublicKey(const Verification *vData, uint8_t *publicKey);                  // generates random b and computes k and B; writes back resulting Accessory Public Key 
  void createSessionKey(const uint8_t *publicKey, size_t len);                          // computes u, S, and K from Client Public Key, A (of variable length)
  int verifyClientProof(const uint8_t *proof);                                          // verifies Client Proof, M1, received from HAP client (return 1 on success, 0 on failure)
  void createAccProof(uint8_t *proof);                                                  // computes M2; write back resulting Accessory Proof

  void print(mbedtls_mpi *mpi);   // prints size of mpi (in bytes), followed by the mpi itself (as a hex character string)
  
};
