# Cloning Pairing Data from one Device to another

### HomeSpan Pairing Data

Even though two different ESP32 devices may be running the exact same sketch, they are nevertheless distinct.  This is because every HomeSpan Accessory has unique 17-character Device ID, a unique 32-byte long-term public key (LTPK), and a unique 64-byte long-term secret key (LTSK).  When HomeSpan is run for the first time on a new device, it looks for these data in the device's non-volatile storage (NVS) memory.  If it is found, the data is loaded for use.  If not found, HomeSpan generates a new set of random keys, and saves this data in the NVS.  The data is permanently stored, though can be erased by typing 'H' into the CLI, which causes HomeSpan to generate a new set of random keys upon the next reboot.

When HomeSpan is initially paired to HomeKit, the 36-character Device ID and 32-byte LTPK for one or more HomeKit Controllers is securely transmitted to the HomeSpan Accessory.  These keys are then saved in the device's NVS for permanent retention (though can be erased by the 'H' command).

Collectively, the Accessory Device ID, LTPK and LTSK, along with the Device ID and LTPK for each paired Controller, is known as the device's *Pairing Data*.  You can view the Pairing Data (except for the LTSK) for any HomeSpan Accessory by typing 'S' into the CLI.  Here is an example:

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

### Cloning a HomeSpan Device

Because every device has a unique set of Pairing Data, it is not possible to simply swap a device that is already paired to HomeKit with another one that is not yet paired - the Device IDs will be different and the second device will need to be separately paired.

This can present a problem if you need to swap out a device (perhaps because it has malfunctioned or was damaged) and you have created a lot of custom automations and scenes for the device from within the Home App.  The new device you use to replace the old device will be seen as a completely new Accessory by HomeKit, and will not be connected with any automations or scenes associated with the old device. In fact, if your unpair the old device, automations and scenes specific to that device will be lost.

To solve this problem you need to be able to replace the broken device with a new device, but *without* unpairing the old device or re-pairing the new device.  This requires the new device to be initialized not with a new set of randomly-generated Device IDs, LTPKs and LTSKs, but rather with the *same* Pairing Data as the old device.

Fortunately, HomeSpan provides a methods for "cloning" the Pairing Data from one device to another.  This means you can swap out a broken device for a new device without HomeKit knowing the difference (provided it is running the same sketch of course).  In fact, you can even swap out an ESP32 for an ESP32-S2, or ESP32-C3.  As long as the sketch is the same, once you clone the Pairing Data the devices are effectively hot-swappable.  

Cloning HomeSpan's Pairing Data is a two-step process.  First you output the Pairing Data from one device to the Serial Monitor, then you copy and paste this data into the Serial Monitor of the second device.  Of course if the first device is completely broken you will not be able to output its Pairing Data.  If you create a lot of automations in HomeKit you may want to output the Pairing Data from each of your devices and save it in a plain text file for later use should any device need to be replaced in the future.

#### Step 1: Type 'P' into the Serial Monitor CLI of the first device to output its Pairing Data

Unlike the 'S' command, the 'P' command compresses all the Pairing Data into *base-64* chunks to make it easier to copy and paste as follows:

```
*** Pairing Data used for Cloning another Device

Accessory data:  ZzbH11I8uNx47Y3Bapq3axQfY5uPOrDfC8D2Q6ke2NwWqat/IGa/6ll8xyY8AShMYO2q6h8gZr/qWXzHJjwBKExg7arqFnNsfXUjy43HgNzc6RDI6RjY6OTk6Q0U6NjUb7mHwbmWzrEWca+5frayfmp=
Controller data: YaNJH5JYDAQE4NjI0NTAwNy02Mi1FRUY4ODNENTA2NjdDvTRGLTRBRDEtQjkwRXFM1On32PKvumS+0YgVMaEo53X/TYNzg==
Controller data: MEUwLTREMEUtODk3Ni0yMjBDREQ2RDUxMjjmah3s+Je0GkmAQE0NDQ1NUE2Ni1ExIUkujzeyWfCCRWol/xecsVkjAIYDRQ==

*** End Pairing Data
```

The first line completely encodes the Pairing Data for the HomeSpan Accessory.  The second two lines encode the Pairing Data for two Controllers that HomeKit is using to control the HomeSpan device.  Note your system may only have one Controller, or it may have more than two.  The number of Controllers depends on your HomeKit network, how it is configured, what devices you have (Apple TVs, HomePods, etc.) and what version of iOS you are running.

Copy this data, exactly as is, from the CLI and save it in a text file.  Make sure not to lose any trailing equal signs as they are part of the base&#8209;64 data!

Next, power down the first device, or at least remove it from the WiFi network to avoid potential duplications of two devices running on the same network with identical Pairing Data (HomeKit will likely not behave if this occurs).  If the second device is not plugged in, do so now and open its Serial Monitor.

#### Step 2: Type 'C' into the Serial Monitor CLI of the second device to input the Pairing Data you just saved from the first device

HomeSpan will begin by asking you for the Accessory Pairing Data. Copy and paste this data (it's the first set of base-64 data output in Step 1 above) directly into the Serial Monitor input window and hit return.  If you copied the data correctly it will be accepted and HomeSpan will display the Device ID that was encoded in the data (it does not bother to display the LTPK and LTSK data).  The Device ID should match that of the orignal device.

If you copied or pasted the data incorrectly, HomeSpan will let you know there is a problem, cancel the process, and reboot without making any changes.  You can also cancel the process by simply hitting return after typing 'P' *without* entering any data (this does not cause a reboot, since no data was changed).

After the Accessory data is accepted, HomeSpan will then ask for Controller data.  Copy and paste this base-64 data from one of the Controllers in the saved text file directly into the Serial Monitor input window and hit return.  As before, if you copied and pasted correctly, HomeSpan will accept the data and display the Device ID of the Controller.  If you copied and pasted incorrectly, HomeSpan will inform you of the error, cancel the process, and reboot without making any changes.

Assuming the data for the first Controller has been accepted, HomeSpan will ask you to repeat the process for any other Controllers you may have.  Keep repeating the process for copying and pasting the Pairing Data for each Controller.  When you have input the Pairing Data for all Controllers, simply hit return without entering any data when asked for the next Controller.  An empty response tells HomeSpan you are done adding Controller data.

Finally, HomeSpan will ask you to confirm saving the new data.  Type either 'y' to confirm (yes) or 'n' to cancel (no).  If you type 'n', HomeSpan will reboot without saving any of the changes.

If you type 'y', HomeSpan will save all of the new Pairing Data in the device's NVS and reboot.  Upon restarting, this second device will be a perfect clone of the first device and HomeKit should recognize it as if it were the original.  You will not need to re-pair the device or make any other changes to the Home App. 
  
❗Caution: Do NOT run two devices on the same HomeKit network with the same Pairing Data.  If you want to experiment by Cloning a working device onto a second device, make sure to unplug the first device before cloning the data onto the second device.  When you are finished experimenting, type 'H' into the CLI of one of the devices so the cloned Pairing Data will be erased and re-generated into something once again unique, allowing you to plug both devices in at the same time without conflict.
  
---

[↩️](../README.md) Back to the Welcome page


