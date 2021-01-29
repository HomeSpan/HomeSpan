# QR Codes versus Setup Codes

Pairing a HomeKit device using its 9-digit *Setup Code* requires you to:

* Select the specific HomeKit device you wish to pair from a list of unpaired HomeKit devices the Home App finds on your local network
* Input the *Setup Code* for that device into the Home App.

The order of the steps above depends on whether you input the *Setup Code* by manually typing it, or scan it from a printed tag (see the User Guide for details on how to create your own scannable *Setup Code* tags)

On the other hand, pairing a HomeKit device with a QR Code requires you only scan the QR Code.  You do not need to identify the device you want to pair - the Home App automatically searches for it on your local network and, if found, pairs to it.

This is possible because the QR Code contains within it a 4-character *Setup ID* that is unique to the device associated with that QR Code.  The QR Code also contains other informattion about the device, such as its type (e.g. Light, Fan, Door) as well as its 9-digit *Setup Code*.

HomeSpan supports pairing with QR codes and uses "HSPN" as its default *Setup ID*.  If you have more than one device that you intend on pairing with a QR Code, you'll need to change their default *Setup IDs* from "HSPN" to something unique using the method `homeSpan.setQRID(const char *)` (see the API Reference for details).  If you do not intend on pairing your devices with QR Codes, it is fine fore them to all have the same *Setup ID*, as this ID is only used to initiate the pairing process via a QR code and serves no other purpose.

### Creating QR Codes

Just as you can create your own scannable tags for a device's *Setup Code*, you can also create your own QR Codes.

