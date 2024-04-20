# TLV8 Characteristics

Most HomeKit Characteristics store a single numerical value or simple string. However, HomeKit supports two additional storage formats - a simple list of bytes (the **DATA** format) and a structured list of tags and values (the **TLV8** format).  The DATA format is not used by any Apple-defined Characterstics but it is included in HomeSpan for use when creating Custom Characteristics for non-Apple applications.

In contrast, the TLV8 format is used extensively by HomeKit during the initial pairing process as well as whenever a new secure (verified) connection is established between HomeKit and a HomeSpan device.  There are also a variety of Apple-defined Characteristics that use the TLV8 format to store and transmit multiple sets of values, each represented as byte-arrays of arbitrary length.

## Overview of TLV8 Format

The TLV8 format itself is quite simple.  A TLV8 Characteristic comprises one or more byte-arrays (sometimes called TLV8 records), where the first byte in a record represents an identifying TAG (from 0-255), the second byte represents the LENGTH of the value, and the remaining LENGTH-bytes represent the VALUE itself (hence the acronym TLV).  Notable points about the TLV8 format are as follows:

* since the LENGTH is stored only as a single byte, VALUES requiring more than 255 bytes must be represented as sequential TLV8 records *with the same TAG*
* it is fine (and in fact common) for a Characteristic to include multiple records with the same TAG, except that they must be *separated by a record with a different TAG*, otherwise the parser reading the data will concatenate the VALUES from sequential records having the same TAG into a single record (as per above)
* records representing a zero-LENGTH VALUE are allowed, and consist of only two bytes: a TAG and a zero (indicating a zero-length VALUE).  TAGS with a zero-LENGTH VALUE are often used to separate multiple records having the same TAG
* if the VALUE stored in TLV8 format represents an integer, it should be presented in little endian format (least-significant byte first) and padded with trailing zeros as needed to bring the total LENGTH of the VALUE to either 1, 2, 4, or 8 bytes (representing uint8_t, uint16_t, uint32_t, and uint64_t values)
* if the VALUE stored in TLV8 format represents a string, it should not include the terminating null since the LENGTH tells you have many characters are in the string
* the bytes that make up a VALUE can themselves represent a separate, complete TLV8 structure.  There is no limit on the number of "nested" TLV8 records that may be embedded in TLV8 Characteristic 
* a parser reading TLV8 records should silently ignore any TAG it is not expecting.  It may be an error to omit a TAG that the parser requires, but it is not an error to include a TAG it does not recognize
* it is not possible to determine whether any given VALUE in a TLV8 record represents an unsigned integer, a string, an arbitrary series of bytes, a separate TLV8 structure, or something else entirely.  The only identifying information for any given TLV8 record is the TAG number, which ranges from 0-255.  There is no general schema or TLV8 protocol that maps TAG types to VALUE types.  Rather, the TAG numbers are arbitrary and users must consult the documentation for each Characteristic to learn what TAG numbers are expected, and what their VALUEs are supposed to represent for that specific Characteristic
* since HomeKit data transmissions are often represented in JSON, and JSON is a text-only format, HomeKit requires that TLV8 records are first encoded in base-64 when transmitting JSON to and from Controllers to Accessories.

Fortunately, HomeSpan includes a dedicated TLV8 library (see below) that automatically takes care of many of the above points, which enables you to read, create, and process TLV8 data without worrying about parsing TLV8 records with more than 255 bytes, converting numerical values to little-endian, or encoding/decoding records into base-64.

## *TLV8()*

Creating an instance of HomeSpan's TLV8 **class** using the above constructor builds an empty TLV8 object into which you can add and process TLV8 records.  TLV8 objects are instantiated as standard C++ linked-list containers derived from `std::list<tlv8_t>`, where *tlv8_t* is an opaque structure used to store individual TLV8 records.[^1]

Also, as shown below, many of the TLV8 methods utilize linked-list iterators.  These are represented by the typedef *TLV8_it*.[^2] 

[^1]:The *tlv8_t* structure is opaque because in general you will not have to create or interact directly with the structure or its data.  Note that in addition to the above TLV8-specific methods, you can use any `std::list` method with a TLV8 object if desired.

[^2]:You do not need expert knowledge of C++ containers and iterators in order to use the TLV8 library, but a basic understanding of containers and iterators will make the library much easier to learn and enjoyable to use.

The method for adding a generic TLV8 record to a TLV8 object is as follows:

* `TLV8_it add(uint8_t tag, size_t len, const uint8_t *val)`

  * where *tag* is the TAG identifier for the record to add and *val* is a pointer to a byte-array containing *len* elements
  * example: `TLV8 myTLV; uint8_t v[]={0x01, 0x05, 0xE3, 0x4C}; tlv.add(1, sizeof(v), v);
  * setting *val* to NULL reserves *len* bytes of space for the TLV8 record within the TLV8 object, but does not copy any data
  * this method returns an *iterator* to the resulting TLV8 record so you can reference the record at a later time if needed

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
  * 






 




🚧
