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

template <class tagType, int maxTags>
class TLV {

  int cLen;              // total number of bytes in all defined TLV records, including TAG andf LEN (suitable for use as Content-Length in HTTP Body)
  int numTags;           // actual number of tags defined
  
  struct tlv_t {
    tagType tag;         // TAG
    int len;             // LENGTH
    uint8_t *val;        // VALUE buffer
    int maxLen;          // maximum length of VALUE buffer
    const char *name;          // abbreviated name of this TAG
  };

  tlv_t tlv[maxTags];           // pointer to array of TLV record structures
  tlv_t *find(tagType tag);     // returns pointer to TLV record with matching TAG (or NULL if no match)

public:

  TLV();
  
  int create(tagType tag, int maxLen, const char *name);   // creates a new TLV record of type 'tag' with 'maxLen' bytes and display 'name'
  
  void clear();                             // clear all TLV structures
  int val(tagType tag);                     // returns VAL for TLV with matching TAG (or -1 if no match)
  int val(tagType tag, uint8_t val);        // sets and returns VAL for TLV with matching TAG (or -1 if no match)    
  uint8_t *buf(tagType tag);                // returns VAL Buffer for TLV with matching TAG (or NULL if no match)
  uint8_t *buf(tagType tag, int len);       // set length and returns VAL Buffer for TLV with matching TAG (or NULL if no match or if LEN>MAX)
  int len(tagType tag);                     // returns LEN for TLV matching TAG (or 0 if TAG is found but LEN not yet set; -1 if no match at all)
  void print();                             // prints all defined TLVs (those with length>0). For diagnostics/debugging only
  int unpack(uint8_t *tlvBuf, int nBytes);  // unpacks nBytes of TLV content from single byte buffer into individual TLV records (return 1 on success, 0 if fail) 
  int pack(uint8_t *tlvBuf);                // if tlvBuf!=NULL, packs all defined TLV records (LEN>0) into a single byte buffer, spitting large TLVs into separate 255-byte chunks.  Returns number of bytes (that would be) stored in buffer
  int pack_old(uint8_t *buf);               // packs all defined TLV records (LEN>0) into a single byte buffer, spitting large TLVs into separate 255-byte records.  Returns number of bytes stored in buffer
  
}; // TLV

//////////////////////////////////////
// TLV contructor()

template<class tagType, int maxTags>
TLV<tagType, maxTags>::TLV(){
  numTags=0;
}

//////////////////////////////////////
// TLV create(tag, maxLen, name)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::create(tagType tag, int maxLen, const char *name){
  
  if(numTags==maxTags){
    Serial.print("\n*** ERROR: Can't create new TLC tag type with name='");
    Serial.print(name);
    Serial.print("' - exceeded number of records reserved\n\n");
    return(0);
  }

  tlv[numTags].tag=tag;
  tlv[numTags].maxLen=maxLen;
  tlv[numTags].name=name;
  tlv[numTags].len=-1;
  tlv[numTags].val=(uint8_t *)malloc(maxLen);
  numTags++;

  return(1);
}

//////////////////////////////////////
// TLV find(tag)

template<class tagType, int maxTags>
typename TLV<tagType, maxTags>::tlv_t *TLV<tagType, maxTags>::find(tagType tag){

  for(int i=0;i<numTags;i++){
    if(tlv[i].tag==tag)
      return(tlv+i);
  }
  
  return(NULL);
}

//////////////////////////////////////
// TLV clear()

template<class tagType, int maxTags>
void TLV<tagType, maxTags>::clear(){

  cLen=0;

  for(int i=0;i<numTags;i++)
    tlv[i].len=-1;

}

//////////////////////////////////////
// TLV val(tag)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::val(tagType tag){

  tlv_t *tlv=find(tag);

  if(tlv && tlv->len>0)
    return(tlv->val[0]);

  return(-1);
}

//////////////////////////////////////
// TLV val(tag, val)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::val(tagType tag, uint8_t val){

  tlv_t *tlv=find(tag);
  
  if(tlv){
    tlv->val[0]=val;
    tlv->len=1;
    cLen+=tlv->len+2;
    return(val);
  }
  
  return(-1);
}

//////////////////////////////////////
// TLV buf(tag)

template<class tagType, int maxTags>
uint8_t *TLV<tagType, maxTags>::buf(tagType tag){

  tlv_t *tlv=find(tag);

  if(tlv)
    return(tlv->val);
    
  return(NULL);
}

//////////////////////////////////////
// TLV buf(tag, len)

template<class tagType, int maxTags>
uint8_t *TLV<tagType, maxTags>::buf(tagType tag, int len){

  tlv_t *tlv=find(tag);
  
  if(tlv && len<=tlv->maxLen){
    tlv->len=len;
    cLen+=tlv->len;

    for(int i=0;i<tlv->len;i+=255)
      cLen+=2;
    
    return(tlv->val);
  }
  
  return(NULL);
}

//////////////////////////////////////
// TLV print()

template<class tagType, int maxTags>
void TLV<tagType, maxTags>::print(){

  char buf[3];

  for(int i=0;i<numTags;i++){
    
    if(tlv[i].len>0){
      Serial.print(tlv[i].name);
      Serial.print("(");
      Serial.print(tlv[i].len);
      Serial.print(") ");
      
      for(int j=0;j<tlv[i].len;j++){
        sprintf(buf,"%02X",tlv[i].val[j]);
        Serial.print(buf);
       }

      Serial.print("\n");

    } // len>0
  } // loop over all TLVs
}

//////////////////////////////////////
// TLV pack(tlvBuf)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::pack(uint8_t *tlvBuf){

  int n=0;
  int nBytes;

  for(int i=0;i<numTags;i++){    
    
    if((nBytes=tlv[i].len)>0){
      for(int j=0;j<tlv[i].len;j+=255,nBytes-=255){
        if(tlvBuf!=NULL){
          *tlvBuf++=tlv[i].tag;
          *tlvBuf++=nBytes>255?255:nBytes;
          memcpy(tlvBuf,tlv[i].val+j,nBytes>255?255:nBytes);
          tlvBuf+=nBytes>255?255:nBytes;
        }
        n+=(nBytes>255?255:nBytes)+2;      
      } // j-loop
    } // len>0
    
  } // loop over all TLVs

return(n);  
}

//////////////////////////////////////
// TLV len(tag)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::len(tagType tag){
  
  tlv_t *tlv=find(tag);

  if(tlv)
    return(tlv->len>0?tlv->len:0);
    
  return(-1);
}

//////////////////////////////////////
// TLV unpack(tlvBuf, nBytes)

template<class tagType, int maxTags>
int TLV<tagType, maxTags>::unpack(uint8_t *tlvBuf, int nBytes){

  clear();

  tagType tag;
  int tagLen;
  uint8_t *val;
  int currentLen;
  int state=0;

  for(int i=0;i<nBytes;i++){
    
    switch(state){
      
      case 0:                                     // ready to read next tag
        if((tag=(tagType)tlvBuf[i])==-1){         // read TAG; return with error if not found
          clear();
          return(0);
        }
        state=1;
      break;

      case 1:                                     // ready to read tag length
        tagLen=tlvBuf[i];                         // read LEN
        currentLen=len(tag);                      // get current length of existing tag
        if(!(val=buf(tag,tagLen+currentLen))){    // get VAL Buffer for TAG and set LEN (returns NULL if LEN > maxLen)
          clear();
          return(0);
        }

        val+=currentLen;                          // move val to end of current length (tag repeats to load more than 255 bytes)
          
        if(tagLen==0)                             // no bytes to read
          state=0;
        else                                      // move to next state
          state=2;
      break;

      case 2:                                     // ready to read another byte into VAL
        *val=tlvBuf[i];                           // copy byte into VAL buffer
        val++;                                    // increment VAL buffer (already checked for sufficient length above)
        tagLen--;                                 // decrement number of bytes to continue copying
        if(tagLen==0)                             // no more bytes to copy
          state=0;
      break;

    } // switch
  } // for-loop

  if(state==0)            // should always end back in state=0
    return(1);            // return success

  clear();
  return(0);              // return fail
}
