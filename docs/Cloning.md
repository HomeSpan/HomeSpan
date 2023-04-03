# Cloning Pairing Data from one Device to another

### HomeSpan Pairing Data

Every HomeSpan Accessory has unique 17-character Device ID, a unique 32-byte long-term public key (LTPK), and a unique 64-byte long-term secret key (LTSK).  these keys are randomly generated for permanent stoorage in a new HomeSpan device, but can be subsequently erased and randomly re-generated with the 'H' CLI Command.

Similarly, every HomeKit Controller (e.g. the Home App on an iPhone) has unique 36-character Device ID and a unique 32-byte LTPK.

When a HomeSpan Accessory is first paired with a HomeKit Controller (using either the Accessory's QR code or its 8-digit Pairing Code), the Accessory and the Controller securely transmit to eachother their Device IDs and LTPKs, which are in turn also permanently stored.  Thus, after pairing, HomeSpan will have in storage its own Device ID, LTPK and LTSK, and the Device ID and LTPK of the paired Controller.  Collectively this information is known as *Pairing Data*.

Pairing Data is used every time a HomeKit Controller opens up a new secure connection with a HomeKit Accessory.  Without this data, the Home App will no longer be able to open a connection to HomeSpan.

Once an Accessory is paired to a Controller, if cannot be paired to a second Controller.  But yet you can operate a HomeSpan device from multiple iPhones, HomePods, Macs, etc., as long as they are on the same network.  This is because in the background HomeKit shares the Pairing Data of a HomeSpan Accessory with other Controllers.  When opening their own connections to HomeSpan, those Controllers use the same Pairing Data.  However, sometimes HomeKit decides to create one or more additional sets of Pairing Data for other Controllers to use by sending such new Pairing Data securely to a HomeSpan Accessory.  HomeSpan can store up to 16 sets of Pairing Data.

You can view most of the Pairing Data for any HomeSpan Accessory by typing 'S' into the CLI.  Here is an example:

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

The only Pairing Data not shown on the CLI is the Accessory's LTSK (the long-term secret key).

### Cloning a HomeSpan Device

Though you can run the same sketch on two different HomeSpan devices, each device will have a unique Device ID, will need to be separately paired with HomeKit, and each will operate as an independent device (perhaps controlling separate lights).  This is the normal use case for running the same sketch on more than one device.

However, sometimes you may want to *replace* one HomeSpan device with another running the exact same sketch.  Maybe the first device has malfunctioned or was damaged.  Typically when this occurs, you unpair the first (broken) device from HomeKit and then pair the new device to HomeKit.  Unfortunately, once you unpair the old device, HomeKit forgets any automations, scenes, customizatons, etc., related to this device that you may have made using the Home App.  When you pair the new device, even though it is running the same sketch, HomeKit will treat it as a completely new device.  It will not apply any name-changes you may have made in the Home App, nor recreate any of the prior device's automations.

To solve this problem, you need to be able to replace the broken device with a new device, but *without* unpairing the old device or re-pairing the new device.  This would require the new device to be initialized with the same Pairing Data as the old device, including having the same long-term secret key.

Fortunately, HomeSpan provides a methods for "cloning" the Pairing Data from one device to another.  This means you can swap out a broken device for a new device without HomeKit knowing the difference (provided it is running the same sketch of course).  In fact, you can even swap out an ESP32 for an ESP32-S2, or ESP32-C3, etc.  As long as the sketch is the same, once you clone the Pairing Data the devices are effectively hot-swappable.  

Cloning HomeSpan's pairing data is a two-step process.  First, type 'P' into the CLI for the device you wish to clone.  This necessarily means the device must still be working well enough for HomeSpan to run (if the device is completely dead, you will not be able to clone its Pairing Data).




---

[↩️](../README.md) Back to the Welcome page


