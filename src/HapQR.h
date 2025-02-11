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

class HapQR {
  private:
   char qrCode[21];

  public:
    enum {
      NFC=1,
      IP=2,
      BLTE=4
    };
    
    char *get(uint32_t setupCode, const char *setupID, uint8_t category, uint8_t protocols=IP, uint8_t qVersion=0, uint8_t qReserved=0){
     
      setupCode&=0x07FFFFFF;     // valid values: 0-99999999
      qVersion&=0x7;             // valid values: 0-7
      qReserved&=0xF;            // valid values: 0-15
      protocols&=0x7;            // valid values: 0-7
      
      uint64_t n=((uint64_t) qVersion<<43) | ((uint64_t) qReserved<<39) | ((uint64_t) category<<31) | (protocols<<27) | setupCode;
      sprintf(qrCode,"X-HM://");
  
      for(int i=15;i>=7;i--){
        qrCode[i]=n%36+48;
        if(qrCode[i]>57)
          qrCode[i]+=7;
        n/=36;
      }

      sprintf(qrCode+16,"%-4.4s",setupID);
      return(qrCode);
    } // create
};
