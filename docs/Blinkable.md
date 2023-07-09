# Creating a Custom Status LED with the Blinkable Interface

The HomeSpan Status LED conveys information about the state of HomeSpan to the user through different blinking patterns.  The *homeSpan* `setStatusPin()` and `setStatusPixel()` methods allow you to choose, respectively, either a standard LED or a NeoPixel LED as the Status LED.  However, the Status LED can be set to any object that implements the **Blinkable** interface[^1] using the *homeSpan* method `setStatusDevice(Blinkable *sDev)`, where *sDev* is a Blinkable object.

To create your own Blinkable object, start by creating a child class derived from **Blinkable**.  Next, add a constructor that defines the pins and performs any initializations if needed.  Finally, define the following *required* methods that **Blinkable** calls to blink the device:

* `void on()` - turns on the device (e.g. causes an LED to light)
* `void off()` - turns off the device (e.g. causes an LED to go dark)
* `int getPin()` - returns the pin number of the device (any number is fine; does not have to be an actual ESP32 pin)

For example, the following defines a Blinkable object for an inverted LED that turns *on* when an ESP32 pin is LOW, and turns *off* when the ESP32 pin is HIGH:

```C++

// CREATE THIS STRUCTURE SOMEWHERE NEAR TOP OF SKETCH

struct invertedLED : Blinkable {        // create a child class derived from Blinkable

  int pin;                              // variable to store the pin number
  
  invertedLED(int pin) : pin{pin} {     // constructor that initializes the pin parameter
    pinMode(pin,OUTPUT);                // set the pin to OUTPUT
    digitalWrite(pin,HIGH);             // set pin HIGH (which is off for an inverted LED)
  }

  void on() override { digitalWrite(pin,LOW); }        // required function on() - sets pin LOW
  void off() override { digitalWrite(pin,HIGH); }      // required function off() - sets pin HIGH
  int getPin() override { return(pin); }               // required function getPin() - returns pin number
};

...

// THEN USE THE STRUCTURE IN SETUP() TO SET THE STATUS LED

void setup(){

  homeSpan.setStatusDevice(new invertedLED(13));    // set Status LED to be a new Blinkable device attached to pin 13

...
}
```

[^1]: In C++, an *interface* is any abstract class that contains only pure virtual functions.  You cannot instantiate an interface, but you can instantiate any derived child classes from the interface provided that you define each of the required virtual functions.

---

[↩️](Reference.md) Back to the Reference API page


