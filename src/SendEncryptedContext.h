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


/////////////////////////////////////////////////
// SendEncryptedContext class
// Write encrypted data as response to a web call

class SendEncryptedContext
{
  const int FRAME_SIZE=1024;          // number of bytes to use in each ChaCha20-Poly1305 encrypted frame when sending encrypted JSON content to Client

  TempBuffer<char> stringBuffer;
  int usedStringLength = 0;
  int sentLength = 0;
  HAPClient& hapClient;
  TempBuffer<uint8_t> sendBuffer;
public:
  SendEncryptedContext(HAPClient& hapClient);
  ~SendEncryptedContext();
 
  void printf(const char* format, ...);
  void print(const char* text);
  char* reserveStringBuffer(int stringLength);
private:
  void sendEncrypted(const char* buffer, int length);
};