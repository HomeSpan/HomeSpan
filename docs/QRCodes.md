# Pairing with QR Codes versus Setup Codes

Pairing a HomeKit device with your Home App using its 9-digit *Setup Code* requires you to both:

* Select the specific HomeKit device you wish to pair from a list of unpaired HomeKit devices the Home App finds on your local network
* Input the *Setup Code* for that device.

The order of the steps above depends on whether you input the *Setup Code* by manually typing it, or scan it from a printed tag (see the User Guide for details on how to create your own scannable *Setup Code* tags)

Pairing a HomeKit device with your Home App using a QR Code instead of the its 9-digit *Setup Code* requires you only scan the QR Code.  The Home App automatically finds the device on your local network and pairs it for you.

This is possible because the QR Code contains within it the intended device's 9-digit *Setup Code* as well as a 4-character *Setup ID* broadcast by the device.  Note the *Setup ID* is distinct from the device's 6-byte Accessory ID, and is only used to faciliate pairing with a QR Code.  Every HomeKit device on your network that supports pairing with a QR code requires a unique *Setup ID*.  A device that does not support pairing with a QR code does not need a *Setup ID*.

HomeSpan supports pairing with QR codes and uses "HSPN" as its default *Setup ID*.  If you have more than one device that intend on pairing with a QR Code, you'll need to change the *Setup IDs* to ensure they are all uniq 
