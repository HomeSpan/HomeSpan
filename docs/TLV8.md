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

Also, as shown below, many of the TLV8 methods utilize linked-list iterators.  These are represented by the typedef *TLV8_it*.[^iterators] 

[^opaque]:The *tlv8_t* structure is opaque because in general you will not have to create or interact directly with the structure or its data.  Note that in addition to the above TLV8-specific methods, you can use any `std::list` method with a TLV8 object if desired.

[^iterators]:You do not need expert knowledge of C++ containers and iterators in order to use the TLV8 library, but a basic understanding of containers and iterators will make the library much easier to learn and enjoyable to use.

The method for adding a generic TLV8 record to a TLV8 object is as follows:

* `TLV8_it add(uint8_t tag, size_t len, const uint8_t *val)`

  * where *tag* is the TAG identifier for the record to add and *val* is a pointer to a byte-array containing *len* elements
  * example: `TLV8 myTLV; uint8_t v[]={0x01, 0x05, 0xE3, 0x4C}; tlv.add(1, sizeof(v), v);
  * setting *val* to NULL reserves *len* bytes of space for the TLV8 record within the TLV8 object, but does not copy any data
  * this method returns a TLV8 *iterator* to the resulting TLV8 record so you can reference the record at a later time if needed

In addition to the above generic method suitable for any type of data, the following methods make it easier to add TLV8 records with specific, frequently-used types of data:

* `TLV8_it add(uint8_t tag, uintXX_t val)`
  * adds a TLV8 record containing a single, unsigned numeric value, *val* (i.e. uint8_t, uint16_t, uint32_t, or uint64_t)
    
* `TLV8_it add(uint8_t tag, const char *val)`
  * adds a TLV8 record containing all the non-null bytes of a null-terminated character string, *val*
    
* `TLV8_it add(uint8_t tag, TLV8 &subTLV)`
  * adds a TLV8 record containing all the bytes of an entire TLV8 object, *subTLV*
    
* `TLV8_it add(uint8_t tag)`
  * adds a zero-length TLV8 record containing nothing but a TAG identifer

Note that if you *add* consecutive records with the same TAG identifier, the TLV8 library will concatenate their data and combine into a single record.  For example, `myTLV.add(1,13); myTLV.add(1,300)` will be combined to produce a single 3-byte recording containing the data 0x0D2C01, where the first byte represents from the number 13 and the second two bytes represent the number 300.  This may have been your desired outcome, but likely not what you wanted to happen.

Instead, to create two distinct records with the same tag value, simply interpose a zero-length record with a different TAG identifier between the two as a "separator" like this: `myTLV.add(1,13); myTLV.add(255); myTLV.add(1,300);`  Here we used a TAG identifer of 255 to represent the separator, but that choice is arbitrary, unless that TAG happens to be used by the Characteristic for something else (TAG identifiers of 0 or 255 are commonly used as separators).

The method for finding a TLV8 record within a TLV8 object that contains a specific TAG identifer is as follows:

* `TLV8_it find(uint8_t tag)`

  * where *tag* is the TAG identifier you are seeking
  * returns a TLV8 iterator to *first* record that matches; returns *end()* if no records match
 
To restrict the search range to a limited set of records, add optional starting and ending iterators *it1* and *it2*:

* `TLV8_it find(uint8_t tag [, TLV8_it it1 [, TLV8_it it2]])`
  
  * returns a TLV8 iterator to the *first* record within the range of iterators from *it1* to *it2* that matches the specified *tag*
  * search range is inclusive of *it1* but exclusive of *it2*
  * returns *it2* if no records match
  * if *it2* is unspecified, default is *end()*; if *it1* is unspecified, default is *begin()*
  * note `myTLV.find(tag)` is equivalent to `myTLV.find(tag, myTLV.begin(), myTLV.end())`
 
Use of the C++ `auto` keyword is generally the best way to save the TVL8_it iterator that is returned from the `find()` and `add()` methods.  For example, `auto myIT = myTLV.find(6)` sets *myIT* to an iterator pointing to the first TLV8 record in *myTLV* that has a TAG identifer of 6.

The method for finding the LENGTH of the data VALUE stored in a particular TLV8 record is as follows:

* `int len(TLV8_it it)`
  * where *it* is an iterator pointing to a specific TLV8 record
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
     
To restrict the the printing range to a limited set of records, add optional starting and ending iterators *it1* and *it2*:

* `void print(TLV8_it it1 [, TLV8_it it2])`
  
  * prints all TLV8 records between iterators *it1* and *it2*
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
 
## *TLV8_it()*  
  
Objects of type *TLV8_it* are iterators that point to specific records in a TLV8 object (or to *end()*).  TLV8 iterators are used to access, read from, and write to, the data elements in any given TLV8 record, and are thus a critical part of the TLV8 library.  However, if you are using the TLV8 library correctly you should rarely, if ever, need to directly instantiate a *TLV8_it* using its constructor.  Instead, simply use the C++ `auto` keyword as noted above.

TLV8_it supports the following methods:

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
  * this template function interprets the VALUE byte-array as a single unsigned integer of type T
  * example: 

```C++
TLV8 myTLV;   // instantiates an empty TLV8 object

myTLV.add(1,32000);               // add a TLV8 record with TAG=1 and VALUE=32000
auto it_A = myTLV.add(2,180);     // add a TLV8 record with TAG=2 and VALUE=18, and save the iterator that is returned

uint8_t v[]={0x01, 0x05, 0xE3, 0x4C};    // create a fixed array, v, of 4 bytes
myTLV.add(200,4,v);                      // add a TLV8 record with TAG=200 and copy all 4 bytes of array v into its VALUE

myTLV.add(50,60000);   // add a TLV8 record with TAG=50 and VALUE=60000
myTLV.add(255);        // add a zero-length TLV8 record with TAG=255 to act as separator
myTLV.add(50,120000);  // add a TLV8 record with TAG=50 and VALUE=120000
myTLV.add(255);        // add a zero-length TLV8 record with TAG=255 to act as separator
auto it_B = myTLV.add(50,30000);   // add a TLV8 record with TAG=50 and VALUE=30000, and save the iterator that is returned

auto it_C = myTLV.find(200);  // find an iterator to first TLV8 record with TAG=200;
auto it_D = myTLV.find(50);   // find an iterator to first TLV8 record with TAG=50;
auto it_E = myTLV.find(200);  // find an iterator to first TLV8 record with TAG=200;

myTLV.print();         // prints the contents of myTLV to the Serial Monitor
```









 


