# TLV8 Characteristics

Most HomeKit Characteristics store a single numerical value or simple string. However, HomeKit supports two additional storage formats - a simple list of bytes (the **DATA** format) and a structured list of tags and values (the **TLV8** format).  The DATA format is not used by any Apple-defined Characterstics but it is included in HomeSpan for use when creating Custom Characteristics for non-Apple applications.

In contrast, the TLV8 format is used extensively by HomeKit during the initial pairing process as well as whenever a new secure (verified) connection is established between HomeKit and a HomeSpan device.  There are also a variety of Apple-defined Characteristics that use the TLV8 format to store and transmit multiple sets of values, each represented as byte-arrays of arbitrary length.

## Overview of TLV8 Format

The TLV8 format itself is quite simple.  A TLV8 object comprises one or more TLV8 *records*, where the first byte in a record represents an identifying TAG (from 0-255), the second byte represents the LENGTH of the value, and the remaining LENGTH-bytes represent the VALUE itself, which is always in the form of a *byte-array* (i.e. an array of 0 or more *uint8_t* elements).  Notable points about the TLV8 format are as follows:

* since the LENGTH is stored only as a single byte, VALUES requiring more than 255 bytes must be represented as sequential TLV8 records *with the same TAG*
* it is fine (and in fact common) for a TLV8 object to include multiple records with the same TAG, except that they must be *separated by a record with a different TAG*, otherwise the parser reading the data will concatenate the VALUES from sequential records having the same TAG into a single record (as per above)
* records representing a zero-LENGTH VALUE are allowed, and consist of only two bytes: a TAG and a zero (indicating a zero-length VALUE).  TAGS with a zero-LENGTH VALUE are often used to separate multiple records having the same TAG
* if the VALUE's byte-array is supposed to represent an single, unsigned integer, it should be arranged in little endian format (i.e. least-significant byte first) and padded with trailing zeros as needed to bring the total LENGTH of the VALUE to either 1, 2, 4, or 8 bytes (representing uint8_t, uint16_t, uint32_t, and uint64_t values)
* if the VALUE's byte-array is supposed to represent a string, it should not include the terminating null since the LENGTH tells you have many characters are in the string
* the bytes that make up a VALUE can themselves represent a separate, complete TLV8 object.  There is no limit on the number of "sub-TLVs" that can be recursively nested in a "parent" TLV8 object
* a parser reading TLV8 records should silently ignore any TAG it is not expecting.  It may be an error to omit a TAG that the parser requires, but it will be not an error to include a TAG it does not recognize
* it is **not** possible to unambigously determine whether the VALUE byte-array in a TLV8 record is supposed to represent an unsigned integer, a string, an arbitrary series of bytes, a sub-TLV object, or something else entirely.  The only identifying information for any given TLV8 record is its TAG number, which ranges from 0-255.  There is no general schema or TLV8 protocol that maps TAG types to VALUE types.  Rather, the TAG numbers are arbitrary and users must consult the documentation for each Characteristic to learn what TAG numbers are expected, and what their VALUEs are supposed to represent for that specific Characteristic
* since HomeKit data transmissions are often represented in JSON, and JSON is a text-only format, HomeKit requires that TLV8 records are first encoded in base-64 when transmitting JSON to and from Controllers to Accessories.

Fortunately, HomeSpan includes a dedicated TLV8 library (see below) that automatically takes care of many of the above points, which enables you to read, create, and process TLV8 data without worrying about parsing TLV8 records with more than 255 bytes, converting numerical values to little-endian, or encoding/decoding records into base-64.

## *TLV8()*

Creating an instance of HomeSpan's TLV8 **class** using the above constructor builds an empty TLV8 object into which you can add and process TLV8 records.  TLV8 objects are instantiated as standard C++ linked-list containers derived from `std::list<tlv8_t>`, where *tlv8_t* is an opaque structure used to store individual TLV8 records.[^opaque]

Also, as shown below, many of the TLV8 methods utilize linked-list *constant* iterators.  These are represented by the typedef *TLV8_itc*.[^iterators] 

[^opaque]:The *tlv8_t* structure is opaque because in general you will not have to create or interact directly with the structure or its data.  Note that in addition to the above TLV8-specific methods, you can use any `std::list` method with a TLV8 object if desired.

[^iterators]:You do not need expert knowledge of C++ containers and iterators in order to use the TLV8 library, but a basic understanding of containers and iterators will make the library much easier to learn and enjoyable to use.

The method for adding a generic TLV8 record to a TLV8 object is as follows:

* `TLV8_itc add(uint8_t tag, size_t len, const uint8_t *val)`

  * where *tag* is the TAG identifier for the record to add and *val* is a pointer to a byte-array containing *len* elements
  * example: `TLV8 myTLV; uint8_t v[]={0x01, 0x05, 0xE3, 0x4C}; tlv.add(1, sizeof(v), v);
  * setting *val* to NULL reserves *len* bytes of space for the TLV8 record within the TLV8 object, but does not copy any data
  * this method returns a TLV8 constant iterator to the resulting TLV8 record so you can reference the record at a later time if needed

In addition to the above generic method suitable for any type of data, the following methods make it easier to add TLV8 records with specific, frequently-used types of data:

* `TLV8_itc add(uint8_t tag, uintXX_t val)`
  * adds a TLV8 record containing a single, unsigned numeric value, *val* (i.e. uint8_t, uint16_t, uint32_t, or uint64_t)
    
* `TLV8_itc add(uint8_t tag, const char *val)`
  * adds a TLV8 record containing all the non-null bytes of a null-terminated character string, *val*
    
* `TLV8_itc add(uint8_t tag, TLV8 &subTLV)`
  * adds a TLV8 record containing all the bytes of an entire TLV8 object, *subTLV*
    
* `TLV8_itc add(uint8_t tag)`
  * adds a zero-length TLV8 record containing nothing but a TAG identifer

Note that if you *add* consecutive records with the same TAG identifier, the TLV8 library will concatenate their data and combine into a single record.  For example, `myTLV.add(1,13); myTLV.add(1,300)` will be combined to produce a single 3-byte recording containing the data 0x0D2C01, where the first byte represents from the number 13 and the second two bytes represent the number 300.  This may have been your desired outcome, but likely not what you wanted to happen.

Instead, to create two distinct records with the same tag value, simply interpose a zero-length record with a different TAG identifier between the two as a "separator" like this: `myTLV.add(1,13); myTLV.add(255); myTLV.add(1,300);`  Here we used a TAG identifer of 255 to represent the separator, but that choice is arbitrary, unless that TAG happens to be used by the Characteristic for something else (TAG identifiers of 0 or 255 are commonly used as separators).

The method for finding a TLV8 record within a TLV8 object that contains a specific TAG identifer is as follows:

* `TLV8_itc find(uint8_t tag)`

  * where *tag* is the TAG identifier you are seeking
  * returns a TLV8 constant iterator to *first* record that matches; returns *end()* if no records match
 
To restrict the search range to a limited set of records, add optional starting and ending iterators *it1* and *it2*:

* `TLV8_itc find(uint8_t tag [, TLV8_itc it1 [, TLV8_itc it2]])`
  
  * returns a TLV8 constant iterator to the *first* record within the range of constant iterators from *it1* to *it2* that matches the specified *tag*
  * search range is inclusive of *it1* but exclusive of *it2*
  * returns *it2* if no records match
  * if *it2* is unspecified, default is *end()*; if *it1* is unspecified, default is *begin()*
  * note `myTLV.find(tag)` is equivalent to `myTLV.find(tag, myTLV.begin(), myTLV.end())`
 
Use of the C++ `auto` keyword is generally the best way to save the TVL8_itc iterator that is returned from the `find()` and `add()` methods.  For example, `auto myIT = myTLV.find(6)` sets *myIT* to a constant iterator pointing to the first TLV8 record in *myTLV* that has a TAG identifer of 6.

The method for finding the LENGTH of the data VALUE stored in a particular TLV8 record is as follows:

* `int len(TLV8_itc it)`
  * where *it* is an constant iterator pointing to a specific TLV8 record
  * returns the length of the data VALUE stored in the associated record, which may be zero for a zero-LENGTH record
  * returns -1 if *it* points to the *end()* of the TLV8 object

A typical use of the `len()` method is to simultaneously check whether a TLV8 object contains a particular TAG identifier, and that the LENGTH of the TAG matches an expected value.  For example, if a certain Characteristic requires a TLV8 record with a TAG identifer of 6 to contain a 32-byte registration number, you can perform the following check:

```C++
auto myIT = myTLV.find(6);
if(myTLV.len(myIT)!=32)
  Serial.printf("Error: TAG 6 is either missing or of improper length\n");
else 
  Serial.printf("TAG 6 containing 32 bytes of data has been found\n");
```

The method for printing all of the records in a TLV8 object to the Serial Monitor is as follows:

* `void print()`
  
  * prints all TLV8 records, one per line, to the Serial Monitor
  * format of the output is: TAG(LENGTH) VALUE [NUMERIC], where
    * TAG = the TAG identifer (0-255)
    * LENGTH = length of the VALUE byte-array (may be zero)
    * VALUE = a sequential list, in hexadecimal, of all the bytes in the VALUE byte-array (only displayed if LENGTH>0)
    * NUMERIC = an unsigned-integer interpretation of the bytes in VALUE, assuming little-endian ordering
      * this decimal value is only displayed if LENGTH<=8
      * if LENGTH=0, the word "null" is displayed instead
     
To restrict the the printing range to a limited set of records, add optional starting and ending constant iterators *it1* and *it2*:

* `void print(TLV8_itc it1 [, TLV8_itc it2])`
  
  * prints all TLV8 records between constant iterators *it1* and *it2*
  * print range is inclusive of *it1* but exclusive of *it2*
  * if *it2* is unspecified, prints only the record pointed to by *it1*
  * note `myTLV.print()` is equivalent to `myTLV.print(myTLV.begin(), myTLV.end())`

The output generated by `print()` can contain some very long lines, especially if the VALUE of some of the TLV8 records represents other complete TLV8 objects (known as sub-TLVs or "nested" TLVs).  To recursively print all sub-TLV objects, use the following method:

* `void printAll()`
  
  * recursively prints all TLV8 records, one per line, to the Serial Monitor
  * inspects each TLV8 record and tries to parse as if the record represented a sub-TLV object
    * if parsing is successful, prints the record and then calls `printAll()` on the sub-TLV
    * if not, prints the record and ends this branch of the recursion
  * the format of each line is the same as that of `print()` except that TAG displays the full path of all TAGs through the branch
  * note that the output can be very voluminous if your TLV8 object contains many levels of nested sub-TLVs
  * warning: some care is required when interpretating the output[^subTLVs]
  
[^subTLVs]:The `printAll()` method assumes that any VALUE that is consistent with the format of a sub-TLV must be a sub-TLV, even if its just a simple numeric value.  For example, `add(10,65536)` yields a record with a TAG identifer of 10 and a 4-byte VALUE of 0x00000100.  The `printAll()` method will display this record along with NUMERIC=65536, but it will also then interpret (and thus display) this VALUE as a sub-TLV containing one zero-length record with TAG identifier=0 and another zero-length record with TAG identifer=1, since the VALUE can be successfully parsed as such.

TLV8 objects manage all of their internal memory requirements, and free up all resources and memory when they go out of scope or are otherwise deleted.  However, if you need to "erase" all the contents of a TLV8 object but stil retain the object so you can re-fill with new data, use the following method:

* `void wipe()`
  * erases all TLV8 records and frees all associated memory
  * leaves an empty TLV8 object ready for re-use
 
## *TLV8_itc()*  
  
Objects of type *TLV8_itc* are constant iterators that point to specific *tlv8_t* records in a TLV8 object (or to *end()*).  TLV8 iterators are used to access, read from, and write to, the data elements in any given TLV8 record, and are thus a critical part of the TLV8 library.  However, if you are using the TLV8 library correctly you should rarely, if ever, need to directly instantiate a *TLV8_itc* using its constructor.  Instead, simply use the C++ `auto` keyword as noted above.

TLV8_itc iterators can be dereferenced to work with data in an individual TLV8 record using the follow methods:

* `uint8_t getTag()`
  
  * returns the TAG identifier (0-255) of the TLV8 record
  * example: `uint8_t tag = myIT->getTag()` or, equivalently, `uint8_t tag = (*myIT).getTag()`

* `size_t getLen()`
  
  * returns the LENGTH of the VALUE byte-array of the TLV8 record
  * example: `size_t len = myIT->getLen()` or, equivalently, `size_t len = (*myIT).getLen()`
    
* `uint8_t *get()`
  
  * returns `uint8_t *` pointing to the first element of the VALUE byte-array of the TLV8 record
  * for zero-LENGTH TLV8 records, the return value is NULL
  * example: `uint8_t *v = myIT->get();` or, equivalently, `uint8_t *v = (*myIT).get();`
  * the `(uint8_t *)` casting operator has been overloaded so you can also obtain this same `uint8_t *` pointer by simply dereferencing the iterator
    * example: `auto myIT = myTLV.find(6); uint8_t *v = *myIT;`
    * note this only works if the compiler can determine the need to auto-cast into a `uint8_t *` pointer based on the context of the code

* `uint8_t get()[i]`
  * returns the *i<sup>th</sup>* element of the VALUE byte-array
  * example: `uint8_t n = myIT->get()[i]` or, equivalently, `uint8_t n = (*myIT).get()[i]`
  * the subscript operator has also been overloaded so you can obtain the *i<sup>th</sup>* element by simply dereferencing the iterator
    * example: `uint8_t n = (*myIT)[i]`
  * note there is no range-checking so make sure *i* does not try to reference an element beyond the end of the VALUE byte-array

* `T getVal<class T>()`
  * this template function returns a single numeric value of type *T* on the assumption that the VALUE byte-array is storing an unsigned integer in little endian format
  * *T* can be *uint8_t*, *uint16_t*, *uint32_t*, or *uint64_t* (if unspecified *T* defaults to *uint32_t*)
  * example: `auto myIT = myTLV.add(50,60000); uint16_t n = myIT->getVal<uint16_t>();`
  * this method returns the correct numeric value as long as sizeof(*T*) >= LENGTH of the byte-array. For example:
    * setting *T=uint64_t* with a VALUE byte-array containing 2 bytes returns the *correct* numeric value
    * setting *T=uint16_t* with a VALUE byte-array containing 4 bytes return an *incorrect* numeric value
  * this function returns zero for all zero-LENGTH TLV8 records 

### A detailed example using the above methods

The following code:

```C++
TLV8 myTLV;   // instantiates an empty TLV8 object

myTLV.add(1,8700);                       // add a TLV8 record with TAG=1 and VALUE=8700
auto it_A = myTLV.add(2,180);            // add a TLV8 record with TAG=2 and VALUE=180, and save the iterator that is returned

uint8_t v[32];                           // create a 32-byte array, v, and fill it with some data
for(int i=0;i<32;i++)
  v[i]=i;
  
myTLV.add(200,32,v);                     // add a TLV8 record with TAG=200 and copy all 32 bytes of array v into its VALUE

myTLV.add(50,60000);                     // add a TLV8 record with TAG=50 and VALUE=60000
myTLV.add(255);                          // add a zero-length TLV8 record with TAG=255 to act as separator
myTLV.add(50,120000);                    // add a TLV8 record with TAG=50 and VALUE=120000
myTLV.add(255);                          // add a zero-length TLV8 record with TAG=255 to act as separator
myTLV.add(50,180000);                    // add a TLV8 record with TAG=50 and VALUE=180000
myTLV.add(255);                          // add a zero-length TLV8 record with TAG=255 to act as separator
auto it_B = myTLV.add(50,240000);        // add a TLV8 record with TAG=50 and VALUE=240000, and save the iterator that is returned

auto it_C = myTLV.find(50);                   // find an iterator to the first TLV8 record with TAG=50;
auto it_D = myTLV.find(50,std::next(it_C));   // find an iterator to the first TLV8 record with TAG=50 that occurs AFTER it_C;

auto it_E = myTLV.find(200);             // find an iterator to first TLV8 record with TAG=200;

Serial.printf("results of myTLV.print():\n\n");

myTLV.print();                           // print the contents of myTLV to the Serial Monitor

Serial.printf("\n");

// print content of it_A:

Serial.printf("it_A: TAG=%d, LENGTH=%d, Value=%d\n", it_A->getTag(), it_A->getLen(), it_A->getVal());

// print content of it_B using alternative syntax:

Serial.printf("it_B: TAG=%d, LENGTH=%d, Value=%d\n", (*it_B).getTag(), (*it_B).getLen(), (*it_B).getVal());

// print contents of it_C and it_D, based on previous find() above:

Serial.printf("it_C TAG=%d, LENGTH=%d, Value=%d\n", (*it_C).getTag(), (*it_C).getLen(), (*it_C).getVal());
Serial.printf("it_D TAG=%d, LENGTH=%d, Value=%d\n", (*it_D).getTag(), (*it_D).getLen(), (*it_D).getVal());

// you can also use the results of find() directly without saving as a separate iterator, though this is computationally inefficient:

if(myTLV.find(1)!=myTLV.end())      // check for match
  Serial.printf("Found: TAG=%d, LENGTH=%d, Value=%d\n", myTLV.find(1)->getTag(), myTLV.find(1)->getLen(), myTLV.find(1)->getVal());

// sum up all the bytes in it_E:

int sum=0;
for(int i=0; i < it_E->getLen(); i++)
  sum+= (*it_E)[i];

Serial.printf("it_E TAG=%d, LENGTH=%d, Sum of all bytes = %d\n", (*it_E).getTag(), (*it_E).getLen(), sum);

// create a "blank" TLV8 record with TAG=90 and space for 16 bytes:

auto it_F = myTLV.add(90,16,NULL);

// copy the first 16 bytes of it_E into it_F and print the record:

memcpy(*it_F,*it_E,16);
myTLV.print(it_F);
```

produces the following output:

```C++
results of myTLV.print():

1(2) FC21 [8700]
2(1) B4 [180]
200(32) 000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F
50(2) 60EA [60000]
255(0)  [null]
50(4) C0D40100 [120000]
255(0)  [null]
50(4) 20BF0200 [180000]
255(0)  [null]
50(4) 80A90300 [240000]

it_A: TAG=2, LENGTH=1, Value=180
it_B: TAG=50, LENGTH=4, Value=240000
it_C TAG=50, LENGTH=2, Value=60000
it_D TAG=50, LENGTH=4, Value=120000
Found: TAG=1, LENGTH=2, Value=8700
it_E TAG=200, LENGTH=32, Sum of all bytes = 496
90(16) 000102030405060708090A0B0C0D0E0F
```

## Reading and Writing TLV8 Characteristics

As fully documented in the [API Reference](Reference.md), the following *SpanCharacteristic* methods are used to read and write TLV8 objects to TLV8 Characteristics:

* `getTLV(TLV8 &tlv)`
* `getNewTLV(TLV8 &tlv)`
* `setTLV(TLV8 &tlv)`

These are analagous to the `getVal()`, `getNewVal()` and `setVal()` methods used for numerical-based Characteristics. 

Note that using the above methods *do not* require you to create a separate byte-array that splits records into chunks of 255 bytes, nor does it require you to encode or decode anything into base-64.  Rather, you directly read and write to and from the Characteristic into a TLV8 object.[^getString]

For a detailed example of how TLV8 Characteristics are used in practice, see [Tutorial Example 22 - TLV8_Characteristics](../examples/22-TLV8_Characteristics) demonstrating how the **DisplayOrder** TLV8 Charactersitic can be used to set the order in which Input Sources for a TV Service are displayed in the Home App. 

[^getString]:Since TLV8 Characteristics are stored as base-64 encoded strings, you can always use `getString()` to read the base-64 text, or `getData()` to decode the string into the full byte-array that represents the entire TLV8 object, if you desire. Also, if you really don't want to use HomeSpan's TLV8 library to produce TLV8 objects, but instead prefer to use your own methods to create a TLV8-compliant byte-array, you can do so and then use `setData()` to save the byte-array you produced to the TLV8 Characteristic, which will perform the base-64 encoding for you. Or, if you want to additionally perform your own base-64 encoding (why?), you can do so and then simply use `setString()` to save the resulting encoded text to the TLV8 Characteristic.

### Write-Response Requests

For most Characteristics, when the Home App sends HomeSpan a request to update a value, it is instructing HomeSpan to perform some sort of action, such as "change the brightness of a lightbulb to 30%" or "change the target state of the door to open."  The only feedback the Home App expects to receive in response to such requests is basically an "OK" or "NOT OKAY" message, which is the purpose of the boolean return value in the `update()` method for every Service.

However, sometimes the Home App sends HomeSpan a request for information, rather than a direct instruction to perform a task.  In such instances, rather than sending back just an OK/NOT-OKAY message, the Home App expects the Accessory device to update the value of the Characteristic *not* with the new value that the Home App sent, but rather with the information it requested.  It then expects this information to be transmitted back to the Home App at the conclusion of the update.  

This procedure is known as a "Write-Response Request", and it is the primary purpose for having TLV8 Characteristics, since TLV8 objects are ideal for storing structured information. 

Though the procedure is complex, HomeSpan handles all of the protocol details.  You only need to focus on reading the TLV8 Characteristic and updating it with the required TLV8 response as follows:

* first, from within the `update()` loop of the applicable Service, check to see if the Home App has requested an update to the TLV8&nbsp;Characteristic;
* if so, create a new TLV8 object and use `getNewTLV()` to load the contents of the updated Characteristic into that TLV8 object;
* then, use the TLV8 library methods described above to read through the TAGS and VALUES in the TLV8 object to determine what data the Home App is conveying and what information it wants returned (based on the specs for the Characteristic);
* next, create a *second* TLV8 object and use the TLV8 library methods above to create the appropriate TAGS and VALUES needed to respond to the information request (again, based on the on the specs for the Characteristic);
* finally, use `setVal()` to update the TLV8 Characteristic with the second TLV8 object

HomeSpan will automatically send the new TLV8 data you placed in the TLV8 Characteristic back to the Home App in its response at the conclusion of the `update()` loop. 

---

[↩️](../README.md) Back to the Welcome page


