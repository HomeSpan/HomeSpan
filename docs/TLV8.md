# TLV8 Characteristics

Most HomeKit Characteristics store a single numerical value or simple string. However, HomeKit supports two additional storage formats - a simple list of bytes (the **DATA** format) and a structured list of tags and values (the **TLV8** format).  The DATA format is not used by any Apple-defined Characterstics but it is included in HomeSpan for use when creating Custom Characteristics for non-Apple applications.

In contrast, the TLV8 format is used extensively by HomeKit during the initial pairing process as well as whenever a new secure (verified) connection is established between HomeKit and a HomeSpan device.  There are also a variety of Apple-defined Characteristics that use the TLV8 format to store and transmit multiple sets of values, each represented as byte-arrays of arbitrary length.

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





🚧
