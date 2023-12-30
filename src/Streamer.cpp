/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
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

//#include "HAP.h"
//  
//StreamBuffer::StreamBuffer(){
//
//  buffer=(char *)HS_MALLOC(bufSize);
//  setp(buffer, buffer+bufSize-1);
//}
//
////////////////////////////////////////
//
//StreamBuffer::~StreamBuffer(){
//  
//  free(buffer);
//  sync();
//}
//
////////////////////////////////////////
//
//int StreamBuffer::flushBuffer(){
//  int num=pptr()-pbase();
//
//  write(1,buffer,num);
//  client->write(buffer,num);
//  delay(1);
//  
//  pbump(-num);
//  return num;
//}
//
////////////////////////////////////////
//        
//StreamBuffer::int_type StreamBuffer::overflow(StreamBuffer::int_type c){
//  
//  if(c!=EOF){
//    *pptr() = c;
//    pbump(1);
//  }
//
//  if(flushBuffer()==EOF)
//    return EOF;    
//  return c;
//}
//
////////////////////////////////////////
//
//int StreamBuffer::sync(){
//  
//  if(flushBuffer()==EOF)
//    return -1;  
//  return 0;
//}
// 
////////////////////////////////////////
