# Cloning Pairing Data from one Device to another

Every HomeSpan Accessory has unique 17-character Device ID, a unique 32-byte long-term public key (LTPK), and a unique 64-byte long-term secret key (LTSK).  these keys are randomly generated for permanent stoorage in a new HomeSpan device, but can be subsequently erased and randomly re-generated with the 'H' CLI Command.

Similarly, every HomeKit Controller (e.g. the Home App on an iPhone) has unique 36-character Device ID and a unique 32-byte LTPK.

When a HomeSpan Accessory is first paired with a HomeKit Controller (using either the Accessory's QR code or its 8-digit Pairing Code), the Accessory and the Controller securely transmit to eachother their Device IDs and LTPKs, which are in turn also permanently stored.  Thus, after pairing, HomeSpan will have in storage its own Device ID, LTPK and LTSK, and the Device ID and LTPK of the paired Controller.  Collectively this information is known as *Pairing Data*.

Pairing Data is used every time a HomeKit Controller opens up a new secure connection with a HomeKit Accessory.  Without this data, the Home App will no longer be able to open a connection to HomeSpan.

Once an Accessory is paired to a Controller, if cannot be paired to a second Controller.  But yet you can operate a HomeSpan device from multiple iPhones, HomePods, Macs, etc., as long as they are on the same network.  This is because in the background HomeKit shares the Pairing Data of a HomeSpan Accessory with other Controllers.  When opening their own connections to HomeSpan, those Controllers use the same Pairing Data.  However, sometimes HomeKit decides to create one or more additional sets of Pairing Data for other Controllers to use by sending such new Pairing Data securely to a HomeSpan Accessory.  HomeSpan can store up to 16 sets of Pairing Data.

You can view the Pairing Data for any HomeSpan Accessory by typing 'S' into the CLI.  Here is an example:

```
*** HomeSpan Status ***

IP Address:        192.168.1.11

Accessory ID:      77:D2:F6:99:CE:65                               LTPK: 346A544A876B124E50F9E3CC276A29D23E8B5DD0590138AA59C833A0D2096E37
Paired Controller: A487DE69-81C3-B5ED-8762-C3B9A987F967   (admin)  LTPK: EE12A678DD56C4E9C0D935A341B8E6C6C098A6B3E6D4C5F5F914A54C9E85BA76
Paired Controller: 449AD09E-109D-3EB5-25B4-8A04E5C57D65   (admin)  LTPK: 34A6B57DE881A75B647D2C9C68E76745A3B466577D19E4C78A67A68C4ED959B8

Connection #0 192.168.1.29 on Socket 3/16  ID=A487DE69-81C3-B5ED-8762-C3B9A987F967   (admin)
Connection #1 (unconnected)
Connection #2 (unconnected)
Connection #3 (unconnected)
Connection #4 (unconnected)
Connection #5 (unconnected)
Connection #6 (unconnected)
Connection #7 (unconnected)
Connection #8 (unconnected)
Connection #9 (unconnected)
Connection #10 (unconnected)
Connection #11 (unconnected)

*** End Status ***
```


### Creating Scannable QR Codes



---

[↩️](../README.md) Back to the Welcome page


