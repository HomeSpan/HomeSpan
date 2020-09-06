
#include <Arduino.h>

namespace Utils {

char *readSerial(char *c, int max);   // read serial port into 'c' until <newline>, but storing only first 'max' characters (the rest are discarded)
String mask(char *c, int n);          // simply utility that creates a String from 'c' with all except the first and last 'n' characters replaced by '*'
  
}

/////////////////////////////////////////////////
// Creates a temporary buffer that is freed after
// going out of scope

template <class bufType>
struct TempBuffer {
  bufType *buf;
  int nBytes;
  
  TempBuffer(size_t len){
    nBytes=len*sizeof(bufType);
    buf=(bufType *)heap_caps_malloc(nBytes,MALLOC_CAP_8BIT);      
   }

  ~TempBuffer(){
    heap_caps_free(buf);
  }

  int len(){
    return(nBytes);
  }
  
};
