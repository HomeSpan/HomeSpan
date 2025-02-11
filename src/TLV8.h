/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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
  void osprint(std::ostream& os) const;
  
  operator uint8_t*() const {
    return(val.get());
  }

  uint8_t & operator[](int index) const {
    return(val.get()[index]);
  }

  uint8_t *get() const {
    return(val.get());
  }

  size_t getLen() const {
    return(len);
  }

  uint8_t getTag() const {
    return(tag);
  }

  template<class T=uint32_t> T getVal() const {
    T iVal=0;
    for(int i=0;i<len;i++)
      iVal|=static_cast<T>(val.get()[i])<<(i*8);
    return(iVal);
  }
  
};

/////////////////////////////////////

typedef std::list<tlv8_t, Mallocator<tlv8_t>>::const_iterator TLV8_itc;
typedef struct { const uint8_t tag; const char *name; } TLV8_names;

/////////////////////////////////////

class TLV8 : public std::list<tlv8_t, Mallocator<tlv8_t>> {

  TLV8_itc mutable currentPackIt;
  TLV8_itc mutable endPackIt;
  uint8_t mutable *currentPackBuf;
  uint8_t mutable *endPackBuf;
  int mutable currentPackPhase;
  size_t mutable currentPackLen;
 
  uint8_t unpackTag;
  size_t unpackBytes;
  int unpackPhase;

  const TLV8_names *names=NULL;
  int nNames=0;

  void printAll_r(String label) const;

  public:

  TLV8(){};
  TLV8(const TLV8_names *names, int nNames) : names{names}, nNames{nNames} {};

  TLV8_itc add(uint8_t tag, size_t len, const uint8_t *val);
  TLV8_itc add(uint8_t tag, uint64_t val);
  TLV8_itc add(uint8_t tag, TLV8 &subTLV);
  TLV8_itc add(uint8_t tag){return(add(tag, 0, NULL));}
  TLV8_itc add(uint8_t tag, const char *val){return(add(tag, strlen(val), reinterpret_cast<const uint8_t*>(val)));}

  TLV8_itc find(uint8_t tag, TLV8_itc it1, TLV8_itc it2) const;
  TLV8_itc find(uint8_t tag, TLV8_itc it1) const {return(find(tag, it1, end()));}
  TLV8_itc find(uint8_t tag) const {return(find(tag, begin(), end()));}

  int len(TLV8_itc it) const {return(it==end()?-1:(*it).getLen());} 

  size_t pack_size(TLV8_itc it1, TLV8_itc it2) const;
  size_t pack_size() const {return(pack_size(begin(), end()));}

  void pack_init(TLV8_itc it1, TLV8_itc it2) const {currentPackIt=it1; endPackIt=it2; currentPackPhase=0;}
  void pack_init(TLV8_itc it1) const {pack_init(it1, std::next(it1));}
  void pack_init() const {pack_init(begin(),end());}
  
  size_t pack(uint8_t *buf, size_t bufSize) const;
  size_t pack(uint8_t *buf) const {pack_init(); return(pack(buf,pack_size()));}

  const char *getName(uint8_t tag) const;
  
  void print(TLV8_itc it1, TLV8_itc it2) const;
  void print(TLV8_itc it1) const {print(it1, std::next(it1));}
  void print() const {print(begin(), end());}
  void printAll() const {printAll_r("");}

  void osprint(std::ostream& os, TLV8_itc it1, TLV8_itc it2) const;
  void osprint(std::ostream& os, TLV8_itc it1) const {osprint(os, it1, std::next(it1));}
  void osprint(std::ostream& os) const {osprint(os, begin(), end());}

  int unpack(uint8_t *buf, size_t bufSize);
  int unpack(TLV8_itc it);
  
  void wipe() {std::list<tlv8_t, Mallocator<tlv8_t>>().swap(*this);}
};
