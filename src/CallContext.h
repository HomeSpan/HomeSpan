/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Michael Geramb
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
#include "Utils.h"

struct HAPClient;

/////////////////////////////////////////////////////////////////////////////////
// CallContext class
// Store strings off a call and call handlePage after the page limit was reached

class CallContext
{
  static const int MINIMUM_STRING_BUFFER_SIZE=1006;  // Same as framesize for SendEncryptedCallContext  
private:
  TempBuffer<char> stringBuffer;
  int usedStringLength = 0;
public:
  CallContext();
  void printf(const char* format, ...);
  void print(const char* text);
  char* reserveStringBuffer(int stringLength);
  void flush();
protected:
  virtual void handlePage(const char* buffer, int length) = 0;
};

/////////////////////////////////////////////////////////////////////////////////
// SendEncryptedCallContext class
// Sends the pages encrypted to the network

class SendEncryptedCallContext : public CallContext
{
  static const int FRAME_SIZE=1006;  // 1024 - AAD - Auth  // number of bytes to use in each ChaCha20-Poly1305 encrypted frame when sending encrypted JSON content to Client

  HAPClient& hapClient;
  TempBuffer<uint8_t> sendBuffer;
public:
  SendEncryptedCallContext(HAPClient& hapClient);
  ~SendEncryptedCallContext();
 
 private:
  virtual void handlePage(const char* buffer, int length) override;
};

/////////////////////////////////////////////////////////////////////////////////
// CalcHashCallContext class
// Calc an hash code out of the provided strings

class CalcHashCallContext : public CallContext
{
public:
  static const int HASH_SIZE=48;

private:
  uint8_t tHash[CalcHashCallContext::HASH_SIZE];
public:
  CalcHashCallContext();
  const uint8_t* getHashCode();
protected:
  void handlePage(const char* buffer, int length) override;
};