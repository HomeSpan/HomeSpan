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

#include <Arduino.h>
#include <sstream>
#include <list>
#include <memory>

#include "PSRAM.h"

class tlv8_t {
  
  private:
  
  uint8_t tag;
  size_t len;
  std::unique_ptr<uint8_t> val;

  public:
 
  tlv8_t(uint8_t tag, size_t len, const uint8_t* val);
  void update(size_t addLen, const uint8_t *addVal);
  void osprint(std::ostream& os);
  
  operator uint8_t*() const{
    return(val.get());
  }

  uint8_t & operator[](int index){
    return(val.get()[index]);
  }

  uint8_t *get(){
    return(val.get());
  }

  size_t getLen(){
    return(len);
  }

  uint8_t getTag(){
    return(tag);
  }

  template<class T=uint32_t> T getVal(){
    T iVal=0;
    for(int i=0;i<len;i++)
      iVal|=static_cast<T>(val.get()[i])<<(i*8);
    return(iVal);
  }
  
};

/////////////////////////////////////

typedef std::list<tlv8_t, Mallocator<tlv8_t>>::iterator TLV8_it;
typedef struct { const uint8_t tag; const char *name; } TLV8_names;

/////////////////////////////////////

class TLV8 : public std::list<tlv8_t, Mallocator<tlv8_t>> {

  TLV8_it currentPackIt;
  TLV8_it endPackIt;
  uint8_t *currentPackBuf;
  uint8_t *endPackBuf;
  int currentPackPhase;
  size_t currentPackLen;
 
  uint8_t unpackTag;
  size_t unpackBytes;
  int unpackPhase;

  const TLV8_names *names=NULL;
  int nNames=0;

  void printAll_r(String label);

  public:

  static TLV8 NULL_TLV;
 
  TLV8(){};
  TLV8(const TLV8_names *names, int nNames) : names{names}, nNames{nNames} {};

  TLV8_it add(uint8_t tag, size_t len, const uint8_t *val);
  TLV8_it add(uint8_t tag, uint64_t val);
  TLV8_it add(uint8_t tag, TLV8 &subTLV);
  TLV8_it add(uint8_t tag){return(add(tag, 0, NULL));}
  TLV8_it add(uint8_t tag, const char *val){return(add(tag, strlen(val), reinterpret_cast<const uint8_t*>(val)));}

  TLV8_it find(uint8_t tag, TLV8_it it1, TLV8_it it2);
  TLV8_it find(uint8_t tag, TLV8_it it1){return(find(tag, it1, end()));}
  TLV8_it find(uint8_t tag){return(find(tag, begin(), end()));}

  int len(TLV8_it it){return(it==end()?-1:(*it).getLen());} 

  size_t pack_size(TLV8_it it1, TLV8_it it2);
  size_t pack_size(){return(pack_size(begin(), end()));}

  void pack_init(TLV8_it it1, TLV8_it it2){currentPackIt=it1; endPackIt=it2; currentPackPhase=0;}
  void pack_init(TLV8_it it1){pack_init(it1, std::next(it1));}
  void pack_init(){pack_init(begin(),end());}
  
  size_t pack(uint8_t *buf, size_t bufSize);
  size_t pack(uint8_t *buf){pack_init(); return(pack(buf,pack_size()));}

  const char *getName(uint8_t tag);
  
  void print(TLV8_it it1, TLV8_it it2);
  void print(TLV8_it it1){print(it1, std::next(it1));}
  void print(){print(begin(), end());}
  void printAll(){printAll_r("");}

  void osprint(std::ostream& os, TLV8_it it1, TLV8_it it2);
  void osprint(std::ostream& os, TLV8_it it1){osprint(os, it1, std::next(it1));}
  void osprint(std::ostream& os){osprint(os, begin(), end());}

  int unpack(uint8_t *buf, size_t bufSize);
  int unpack(TLV8_it it);
  
  void wipe(){std::list<tlv8_t, Mallocator<tlv8_t>>().swap(*this);}
  
};
