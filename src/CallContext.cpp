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

#include <Arduino.h>
#include "CallContext.h"
#include "HomeSpan.h"
#include "HAP.h"
#include "Settings.h"
#include <sodium.h>

CallContext::CallContext()
    : stringBuffer(1024, "CallContext stringBuffer") 
{
}

void CallContext::printf(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    int len = vsnprintf(NULL, 0, format, ap);
    char* buffer = reserveStringBuffer(len);
    vsnprintf(buffer, len + 1, format, ap); // the reserved buffer is one byte larger because it contains the terminating '\0'
    LOG2(buffer);
    va_end(ap);
}


char* CallContext::reserveStringBuffer(int stringLength)
{
    int requiredNewBufferLength = stringLength + 1; // reserve buffer for characters and string terminator '\0'
    if (stringBuffer.len() - usedStringLength < requiredNewBufferLength)
    {
        // to less memory, send first exitsting buffer
        if (usedStringLength > 0)
            flush();

        // Check if still more memory is needed
        if (stringBuffer.len() < requiredNewBufferLength)
            stringBuffer = TempBuffer<char>(requiredNewBufferLength, "SendEncryptedCallContext::reserveMemory");
    }
    int alreadyInUse = usedStringLength;
    usedStringLength += stringLength;
    char* result = stringBuffer.get() + alreadyInUse;
    result[stringLength] = '\0'; // add string terminator
    return result;
}

void CallContext::flush()
{
    handlePage(stringBuffer.get(), usedStringLength);
    usedStringLength = 0;
}

void CallContext::print(const char* text)
{
    auto len = strlen(text);
    auto buffer = reserveStringBuffer(len);
    memcpy(buffer, text, len);
    LOG2(buffer);
}

SendEncryptedCallContext::SendEncryptedCallContext(HAPClient& hapClient)
:   hapClient(hapClient),
    sendBuffer(FRAME_SIZE + 2 + 16, "SendEncryptedCallContext sendBuffer") // FRAME_SIZE + AAD + Auth 
{
    LOG2("\n>>>>>>>>>> ");
    LOG2(hapClient.client.remoteIP());
    LOG2(" >>>>>>>>>>\n");
}



void SendEncryptedCallContext::handlePage(const char* buffer, int length)
{
    if (length == 0)
        return;
    LOG2(buffer);

    for(int i=0;i<length;i+=FRAME_SIZE){      // encrypt FRAME_SIZE number of bytes in dataBuf in sequential frames

        int n=length-i;           // number of bytes remaining
        
        if(n>FRAME_SIZE)           // maximum number of bytes to encrypt=FRAME_SIZE
          n=FRAME_SIZE;                                     
        
        sendBuffer.get()[0]=n%256;    // store number of bytes that encrypts this frame (AAD bytes)
        sendBuffer.get()[1]=n/256;

        unsigned long long nBytes;
        crypto_aead_chacha20poly1305_ietf_encrypt(sendBuffer.get()+2,&nBytes, (uint8_t*) buffer+i,n,sendBuffer.get(),2,NULL,hapClient.a2cNonce.get(),hapClient.a2cKey);   // encrypt the next portion of dataBuf with authentication tag appended

        hapClient.client.write(sendBuffer.get(),nBytes+2);   // transmit encrypted frame
        hapClient.a2cNonce.inc();                      // increment nonce
    }
}

SendEncryptedCallContext::~SendEncryptedCallContext()
{
    flush();
    LOG2("-------- SENT ENCRYPTED! --------\n");
}


CalcHashCallContext::CalcHashCallContext()
:   CallContext()
{
    memset(&tHash, 0, HASH_SIZE);
}

void CalcHashCallContext::handlePage(const char* buffer, int length)
{
    uint8_t newHash[HASH_SIZE];
    // create SHA-384 hash of JSON and xor with previous part (can be any hash - just looking for a unique key)
    mbedtls_sha512_ret((uint8_t *)buffer,length, newHash,1); 
    for (int i=0; i<48; i++)
        tHash[i] = tHash[i] ^ newHash[i];
}

const uint8_t* CalcHashCallContext::getHashCode()
{
    flush();
    return tHash;
}