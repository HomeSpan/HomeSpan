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

template <typename T> class DynamicPointerArray {
private:
    union element
    {
      T* singleElement;
      T** multipleElements;
    } u;
    uint16_t arraySize = 0;
public:
    ~DynamicPointerArray()
    {
      if (arraySize > 2)
        free(u.multipleElements);
    }

    void clear()
    {
      if (arraySize > 2)
        free(u.multipleElements);
      arraySize = 0;
      u.singleElement = nullptr;
    }
  
    uint16_t size() const { return arraySize; }

    void push_back(T* element){
      uint16_t currentSize = arraySize;
      arraySize++;
      if (currentSize == 0)
      {
        u.singleElement = element; 
      }
      else if (currentSize == 1)
      {
        T** newBuffer = (T**) malloc(arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        newBuffer[0] = u.singleElement;
        newBuffer[1] = element;
        u.multipleElements = newBuffer;
      }
      else
      {
        T** newBuffer = (T**) realloc(u.multipleElements, arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        newBuffer[arraySize - 1] = element;
        u.multipleElements = newBuffer;
      }
    }
    T* front() const
    {
        return get(0);
    }
    T* back() const
    {
      return get(arraySize - 1);
    }
    bool empty() const
    {
      return arraySize == 0;
    }
    T* operator[](uint16_t index) const
    {
      return get(index);
    }

    T* get(uint16_t index) const
    {
      if (index >= arraySize)
      {
        throw "Fatal error. Index does not exist.";
      }
      if (arraySize == 1)
        return u.singleElement;
      else
        return u.multipleElements[index];
    }


    void remove(uint16_t index)
    {
      if (index >= arraySize)
      {
        throw "Fatal error. Index does not exist.";
      }
      uint16_t currentSize = arraySize;
      if (currentSize == 1)
      {
        u.singleElement = nullptr;
      }
      else if (currentSize == 2)
      {

        if (index == 1)
        {
          T* firstElement = u.multipleElements[0];
          free(u.multipleElements);
          u.singleElement = firstElement;
        }
        else
        {
          T* secondElement = u.multipleElements[1];
          free(u.multipleElements);
          u.singleElement = secondElement;
        }
      }
      else
      {
        for (int i = index; i < currentSize - 1; i++)
        {
          u.multipleElements[i] = u.multipleElements[i + 1];
        }
        T** newBuffer = (T**) realloc(u.multipleElements, arraySize * sizeof(T*));
        if (newBuffer == NULL)
          throw "Fatal error, no more memory";
        u.multipleElements = newBuffer;
      }
      arraySize--;
    
    }

    class DynamicPointerArrayIterator
    {
        const DynamicPointerArray* data;
        uint32_t currentIndex;
        friend class DynamicPointerArray;
    public:
        DynamicPointerArrayIterator(const DynamicPointerArrayIterator& r) :
        data(r.data),currentIndex(r.currentIndex) { }
       
        DynamicPointerArrayIterator(const DynamicPointerArray* arr, uint32_t startIndex): data(arr),currentIndex(startIndex) { }
        T* operator*() const
        {
            return data->get(currentIndex);
        }
        const DynamicPointerArrayIterator operator++(int)
        {
            DynamicPointerArrayIterator temp = *this;
            ++*this;
            return temp;
        }
        DynamicPointerArrayIterator& operator++()
        {
            ++currentIndex;
            return *this;
        }
        bool operator==(const DynamicPointerArrayIterator& lhs)
        {
            return currentIndex==lhs.currentIndex;
        }
        bool operator!=(const DynamicPointerArrayIterator& lhs)
        {
            return currentIndex!=lhs.currentIndex;
        }
    };
    DynamicPointerArrayIterator begin() const{ return DynamicPointerArrayIterator(this,0); }
    DynamicPointerArrayIterator end() const{ return DynamicPointerArrayIterator(this, arraySize); }

    class DynamicPointerArrayReverseIterator
    {
        const DynamicPointerArray* data;
        uint32_t currentIndex;
        friend class DynamicPointerArray;
    public:
        DynamicPointerArrayReverseIterator(const DynamicPointerArrayReverseIterator& r) :
        data(r.data),currentIndex(r.currentIndex) { }
        
        DynamicPointerArrayReverseIterator(const DynamicPointerArray* arr, uint32_t startIndex): data(arr),currentIndex(startIndex) { }
        T* operator*() const
        {
            return data->get(currentIndex);
        }
        const DynamicPointerArrayReverseIterator operator++(int)
        {
            DynamicPointerArrayReverseIterator temp = *this;
            ++*this;
            return temp;
        }
        DynamicPointerArrayReverseIterator& operator++()
        {
            currentIndex--;
            return *this;
        }
        bool operator==(const DynamicPointerArrayReverseIterator& lhs)
        {
            return currentIndex==lhs.currentIndex;
        }
        bool operator!=(const DynamicPointerArrayReverseIterator& lhs)
        {
            return currentIndex!=lhs.currentIndex;
        }
    };
    DynamicPointerArrayReverseIterator rbegin() const{ return DynamicPointerArrayReverseIterator(this,arraySize - 1); }
    DynamicPointerArrayReverseIterator rend() const{ return DynamicPointerArrayReverseIterator(this,  0 - 1); }

    void erase(DynamicPointerArrayIterator iterator)
    {
      remove(iterator.currentIndex);
    }
};