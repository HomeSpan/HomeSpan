# TLV8 Characteristics

Most HomeKit Characteristics store a single numerical value or simple string. However, HomeKit supports two additional storage formats - a simple list of bytes (the **DATA** format) and a structured list of tags and values (the **TLV8** format).  The DATA format is not used by any Apple-defined Characterstics but it is included in HomeSpan for use when creating Custom Characteristics for non-Apple applications.

In contrast, The TLV8 format is used extensively by HomeKit during the initial pairing process as well as whenever a new secure (verified) connection is established between HomeKit and a HomeSpan device.  There are also a variety of Apple-defined Characteristics that use the TLV8 format to store and transmit multiple sets of values, each represented as byte-arrays of arbitrary length.

The TLV8 format itself is quite simple.  A TLV8 Characteristic comprises one or more byte-arrays (sometimes called TLV8 records), where the first byte in a record represents an identifying TAG (from 0-255), the second byte represents the LENGTH of the value, and the remaining LENGTH-bytes represent the VALUE itself (hence the acronym TLV).  Some notable points include:

* since the LENGTH is stored only as a single byte, VALUES requiring more than 255 bytes must be represented as sequential TLV8 records *with the same TAG*
* it is fine (and in fact common) for a Characteristic to include multiple records with the same TAG, except that they must be *separated by a record with a different TAG*, otherwise the parser reading the data will concatenate the VALUES from sequential records having the same TAG into a single record (as per above)
* records representing zero-LENGTH values are allowed, and consist of only two bytes: a TAG and a zero (indicating a zero-length VALUE).  TAGS with zero-LENGTH values are often used to separate multiple records having the same TAG
* if the VALUE stored in TLV8 format represents an integer, it should be presented in little endian format (least-significant byte first) and padded with trailing zeros as needed to bring the total LENGTH of the value to 1, 2, 4, or 8 bytes (representing uint8_t, uint16_t, uint32_t, and uint64_t values)
* if the value stored in TLV8 format represents a string, it should not include the terminating null since the LENGTH tells you have many characters are in the string
* a parser reading TLV8 records should silently ignore any TAG it is not expecting.  It may be an error to omit a TAG that the parser requires, but it is not an error to include a TAG it does not recognize
* since HomeKit data transmissions are often represented in JSON, and JSON is a text-only format, HomeKit requires that TLV8 records are first encoded in base-64 when transmitting JSON to and from Controllers to Accessories.

Fortunately, HomeSpan includes a dedicated TLV8 library (see below) that takes care of everything above automatically, which enables you to read, create, and process TLV8 data without worrying about TLV8 records with more than 255 bytes, converting numerical values to little-endian, or encoding/decoding records into base-64.





🚧
