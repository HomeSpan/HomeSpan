# Pairing with QR Codes versus Setup Codes

Instructing the Home App on your iPhone to pair a HomeSpan device to HomeKit by manually typing its 9-digit *Setup Code* generally involves the following:

1. The Home App searches your local network for unpaired HomeKit accessory devices
2. Each device available to be paired is displayed as a small tile along with its name and icon reflecting its category (e.g. light, faucet, door)
3. **You** select the device you want to pair to HomeKit
4. **You** provide the Home App with the device's *Setup Code*
5. The Home App connects to the device you selected and tries to pair using that *Setup Code*

If instead you use the Home App to scan the device's *Setup Code* from a printed tag, step #4 occurs first, followed by the Home App searching for unpaired devices and then asking you to select one for pairing.  In either case, your input is required twice:  once to select the device you want to pair, and once to type (or scan) the *Setup Code*.

Pairing using a *QR Code* as an alternative to using a *Setup Code* results in a simplified series of steps:

1. *You* scan the *QR Code*
2. The Home App validates the code and displays an icon showing you the category of the device it intends to pair (but **not** the name of the device)
3. *You* confirm you'd like to continue the process
4. The Home App searches your local network for a "matching" device, and if found pairs the device*

---


Note that the reason the Home App can't display the name of the device in step #2, is that it does not search the network for available devices until step #4, which is after you've confirmed the process in step #2.  This seems a bit backwards to me.  Ideally, steps #4 and #3 would be reversed so that the Home App can display the full name of the device to you and you could confirm that it is indeed the device you want to pair.  In the above process, you are really only confirming that you would like the Home App to begin its search and pairing based on information in the *QR Code* you scanned.

Also note that even the category icon displayed in step #2 is derived from the *QR Code* as opposed to being read from the device itself.  More so, the icon is for display purposes only - the Home App does not actually check to see that the category embedded in the *QR Code* matches the category broadcast by the device it has found to pair.  But since the manufacturer of device is presumably not trying to fool you, it will embed the correct category into the *QR Code*.

