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

#include "TLV8.h"

//////////////////////////////////////

tlv8_t::tlv8_t(uint8_t tag, size_t len, const uint8_t* val) : tag{tag}, len{len} {       
  if(len>0){
    this->val=std::unique_ptr<uint8_t>((uint8_t *)HS_MALLOC(len));
    if(val!=NULL)
      memcpy((this->val).get(),val,len);      
  }
}

//////////////////////////////////////

void tlv8_t::update(size_t addLen, const uint8_t *addVal){
  if(addLen>0){
    uint8_t *p=val.release();
    p=(uint8_t *)HS_REALLOC(p,len+addLen);
    val=std::unique_ptr<uint8_t>(p);
    if(addVal!=NULL)
      memcpy(p+len,addVal,addLen);
    len+=addLen;        
  }
}

/////////////////////////////////////

void tlv8_t::osprint(std::ostream& os){

  uint8_t *p=val.get();       // starting pointer
  uint8_t *pend=p+len;        // ending pointer (may equal starting if len=0)

  do{
    uint8_t nBytes=(pend-p)>255?255:(pend-p);   // max is 255 bytes per TLV record
    os.write((char *)&tag,1);
    os.write((char *)&nBytes,1);
    os.write((char *)p,nBytes);
    p+=nBytes;
  } while(p<pend);
}

/////////////////////////////////////

TLV8_it TLV8::add(uint8_t tag, size_t len, const uint8_t* val){

  if(!empty() && front().tag==tag)
    front().update(len,val);
  else
    emplace_front(tag,len,val);

  return(begin());
}

/////////////////////////////////////

TLV8_it TLV8::find(uint8_t tag, TLV8_it it1, TLV8_it it2){

  auto it=it1;
  while(it!=it2 && (*it).tag!=tag)
    it++;
  return(it==it2?end():it);
}

/////////////////////////////////////

size_t TLV8::pack_size(TLV8_it it1, TLV8_it it2){

  size_t nBytes=0;

  while(it1!=it2){
    nBytes+=2+(*it1).len;
    if((*it1).len>255)
      nBytes+=2*(((*it1).len-1)/255);
    it1++;
  }

  return(nBytes);
}

/////////////////////////////////////

size_t TLV8::pack(uint8_t *buf, size_t bufSize){

  size_t nBytes=0;

  while(nBytes<bufSize && currentPackIt!=endPackIt){
    switch(currentPackPhase){

      case 0:
        currentPackBuf=(*currentPackIt).val.get();
        endPackBuf=(*currentPackIt).val.get()+(*currentPackIt).len;
        currentPackPhase=1;
        break;
        
      case 1:
        *buf++=(*currentPackIt).tag;
        nBytes++;
        currentPackPhase=2;
        break;
        
      case 2:
        currentPackLen=endPackBuf-currentPackBuf;
        if(currentPackLen>255)
          currentPackLen=255;
        *buf++=currentPackLen;
        nBytes++;
        currentPackPhase=3;
        break;

      case 3:
        if(currentPackLen==0){
          if(endPackBuf==currentPackBuf){
            currentPackIt++;
            currentPackPhase=0;
          } else {
            currentPackPhase=1;
          }
          break;
        }
        
        size_t copyBytes=(currentPackLen<(bufSize-nBytes)) ? currentPackLen : (bufSize-nBytes);
        memcpy(buf,currentPackBuf,copyBytes);
        buf+=copyBytes;
        currentPackBuf+=copyBytes;
        currentPackLen-=copyBytes;
        nBytes+=copyBytes;
        break;    
    }
  }
  
  return(nBytes);
}

/////////////////////////////////////

void TLV8::unpack(uint8_t *buf, size_t bufSize){

  if(empty())
    unpackPhase=0;

  while(bufSize>0){
    switch(unpackPhase){

      case 0:
        unpackTag=*buf++;
        bufSize--;
        unpackPhase=1;
        break;

      case 1:
        unpackBytes=*buf++;
        bufSize--;
        if(unpackBytes==0){
          add(unpackTag);
          unpackPhase=0;          
        } else {
          unpackPhase=2;
        }
        break;

      case 2:
       size_t copyBytes=unpackBytes<bufSize ? unpackBytes : bufSize;
       add(unpackTag,copyBytes,buf);
       buf+=copyBytes;
       unpackBytes-=copyBytes;
       bufSize-=copyBytes;
       if(unpackBytes==0)
        unpackPhase=0;
      break;        
    }
  }
}


/////////////////////////////////////

const char *TLV8::getName(uint8_t tag){

  if(names==NULL)
    return(NULL);

  for(int i=0;i<nNames;i++){
    if(names[i].tag==tag)
      return(names[i].name);
  }

  return(NULL);
}

/////////////////////////////////////

void TLV8::print(TLV8_it it1, TLV8_it it2){

  while(it1!=it2){
    const char *name=getName((*it1).tag);
    if(name)
      Serial.printf("%s",name);
    else
      Serial.printf("%d",(*it1).tag);
    Serial.printf("(%d) ",(*it1).len);
    for(int i=0;i<(*it1).len;i++)
      Serial.printf("%02X",(*it1).val.get()[i]);
    Serial.printf("\n");
    it1++;
  }
}
  
//////////////////////////////////////

void TLV8::osprint(std::ostream& os, TLV8_it it1, TLV8_it it2){

  for(auto it=it1;it!=it2;it++)
    (*it).osprint(os);
}

//////////////////////////////////////
