# Addressable RGB LEDs

HomeSpan includes two dedicated classes that provide for easy control of "addressable" RGB LEDs.  The **Pixel()** class is used for RGB and RGBW LEDs that require only a single "data" control wire, such as this 8-pixel [NeoPixel RGB Stick](https://www.adafruit.com/product/1426) or this single-pixel [NeoPixel RGBW LED](https://www.adafruit.com/product/2759).  The **Dot()** class is used for RGB LEDs that require two control wires ("data" and "clock"), such as this 144-pixel [DotStar RGB Strip](https://www.adafruit.com/product/2241) or this 60-pixel [RGB LED Strip](https://www.sparkfun.com/products/14015).

Both classes allow you to individually set each of the "pixels" in a multi-pixel LED strip to a different 24-bit RGB color (or 32-bit color, if using RGBW LEDs).  Alternatively, the classes allow you to simply specify a single 24-bit (or 32-bit) color to duplicate across all pixels.

The methods for both classes are nearly identical, which allows you to readily interchange code written for single-wire devices to use with two-wire devices (and vice-versa) with only minor modifications.

## *Pixel(uint8_t pin, [boolean isRGBW])*

Creating an instance of this **class** configures the specified *pin* to output a waveform signal suitable for controlling a single-wire, addressable RGB or RGBW LED device with an arbitrary number of pixels.  Such devices typically contain SK6812 or WS2812 LEDs.  Arguments, along with their defaults if left unspecified, are as follows:

  * *pin* - the pin on which the RGB control signal will be output; normally connected to the "data" input of the addressable LED device
  * *isRGBW* - set to *true* for RGBW devices that contain 4-color (red/green/blue/white) LEDs; set to *false* for the more typical 3-color RGB devices.  Defaults to *false* if unspecified.  Note you must set the *isRGBW* flag to *true* if you are using an RGBW device, even if you do not intend on utilizing the white LED

The two main methods to set pixel colors are:

* `void set(Color color, int nPixels=1)`

  * sets the color of a pixel in a single-pixel device, or equivalently, the color of the first *nPixels* in a multi-pixel device, to *color*, where *color* is an object of type **Color** defined below.  If unspecified, *nPixels* defaults to 1 (i.e. a single pixel).  It is not a problem if the value specified for *nPixels* does not match the total number of actual RGB (or RGBW) pixels in your device; if *nPixels* is less than the total number of device pixels, only the first *nPixels* will be set to *color*;  if *nPixels* is greater than the total number of device pixels, the device will simply ignore the additional input
  
* `void set(Color *color, int nPixels)`

  * individually sets the color of each pixel in a multi-pixel device to the color values specified in the **Color** array *\*color*, of *nPixels* size, where the  first pixel of the device is set to the value in *color\[0\]*, the second pixel is set to the value in *color\[1\]* ... and the last pixel is set to the value in *color\[nPixels-1\]*.  Similar to above, it is not a problem if the value specified for *nPixels* does not match the total number of actual RGB (or RGBW) pixels in your device

In both of the methods above, colors are stored in a 32-bit **Color** object configured to hold four 8-bit RGBW values.  **Color** objects can be instantiated as single variables (e.g. `Pixel::Color myColor;`) or as arrays (e.g. `Pixel::Color myColors[8];`).  Note that the **Color** object used by the **Pixel** class is scoped to the **Pixel** class itself, so you need to use the fully-qualified class name "Pixel::Color".  Once a **Color** object is created, the color it stores can be set using one of the two following methods:
  
  * `Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0)`

    * where *r*, *g*, and *b*, represent 8-bit red, green, and blue values over the range 0-255, and *w* represents an 8-bit value [0-255] for the white LED.  The white value may be left unspecified, in which case it defaults to 0.  Also, the white value will be ignored by *set()* unless the *isRGBW* flag was specified as *true* in the constructor
    * example: `myColor.RGB(255,255,0)` sets myColor to bright yellow
      
  * `Color HSV(float h, float s, float v, double w=0)`
    
    * where *h*=Hue, over the range 0-360; *s*=Saturation percentage from 0-100; and *v*=Brightness percentage from 0-100.  These values are converted to equivalent 8-bit RGB values (0-255) for storage in the *Color* object.  Note the *w* value is treated separately and represents a percentage of brightness for the white LED (from 0-100) that is also converted into an 8-bit value from 0-255 for storage in the **Color** object.  Similar to above, the white value may be left unspecified, in which case it defaults to 0
    * example: `myColor.HSV(120,100,50)` sets myColor to fully-saturated green with 50% brightness
      
Note both methods above return the completed **Color** object itself and can thus be used wherever a **Color** object is required:  For example: `Pixel p(5); Pixel::Color myColor; p.set(myColor.RGB(255,215,0))` sets the color of a single pixel device attached to pin 5 to bright gold.

The **Pixel** class also supports the following class-level methods as a convenient alternative to creating colors:
  
* `static Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0)`
  * equivalent to `return(Color().RGB(r,g,b,w));`
  * example: `Pixel p(8);  p.set(Pixel::RGB(0,0,255),8);` sets the color of each pixel in an 8-pixel device to blue

* `static Color HSV(float h, float s, float v, double w=0)`
  * equivalent to `return(Color().HSV(h,s,v,w));`
  * example: `Pixel::Color c[]={Pixel::HSV(120,100,100),Pixel::HSV(60,100,100),Pixel::HSV(0,100,100)};` to create a red-yellow-green traffic light pattern

Finally, the **Pixel** class supports these two additional, but rarely-needed, methods:

* `int getPin()`

  * returns the pin number, or -1 if the instantiation failed due to lack of resources 

* `void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset)`

  * the default timing parameters used by the **Pixel** class to generate the "data" signal needed to set the colors of an RGB LED device should work with most commercial products based on SK6812 or WS2812 driver chips.  Use this method **ONLY** if you need to override the class defaults and replace them with your own timing parameters, where
    * *high0* and *low0* specify the duration (in microseconds) of the high phase and low phase for a pulse encoding a zero-bit;
    * *high1* and *low1* specify the duration (in microseconds) of the high phase and low phase for a pulse encoding a one-bit; and
    * *lowReset* specifies the delay (in microseconds) representing the end of a pulse stream
   * for reference, the **Pixel** class uses the following defaults: *high0=0.32𝛍s, low0=0.88𝛍s, high1=0.64𝛍s, low1=0.56𝛍s, lowReset=80.0𝛍s* 

### Resource Usage and Resource Conflicts

The **Pixel** class relies on the ESP32's RMT peripheral to create the precise pulse trains required to control single-wire addressable RGB LEDs.  Since each instantiation of **Pixel** consumes an RMT channel, the number of **Pixel** objects you can instantiate (each controlling a separate multi-pixel RGB LED device attached to a specific pin) is limited to the number of RMT available as follows: ESP32 - 8 instances; ESP32-S2 - 4 instances; ESP32-C3 - 2 instances.

Also, the **Pixel** class is optimized to handle arbitrarily-long LED strips containing hundreds of RGB or RGBW pixels.  To accomplish this efficiently, the **Pixel** class implements its own RMT driver, which conflicts with the default RMT driver used by HomeSpan's **RFControl** library.  Unfortunately this means you cannot use both the **Pixel** class library and **RFControl** class library in the same HomeSpan sketch.

## *Dot(uint8_t dataPin, uint8_t clockPin)*

Creating an instance of this **class** configures the specified pins to output waveform signals suitable for controlling a two-wire, addressable RGB LED device with an arbitrary number of pixels.  Such devices typically contain SK9822 or APA102 LEDs, or an embedded WS2801 driver chip.  Arguments, along with their defaults if left unspecified, are as follows:

  * *dataPin* - the pin on which the RGB data signal will be output; normally connected to the "data" input of the addressable LED device
  * *clockPin* - the pin on which the RGB clock signal will be output; normally connected to the "clock" input of the addressable LED device

The two main methods to set pixel colors are:

* `void set(Color color, int nPixels=1)`

  * sets the color of a pixel in a single-pixel device, or equivalently, the color of the first *nPixels* in a multi-pixel device, to *color*, where *color* is an object of type **Color** defined below.  If unspecified, *nPixels* defaults to 1 (i.e. a single pixel).  It is not a problem if the value specified for *nPixels* does not match the total number of actual RGB pixels in your device; if *nPixels* is less than the total number of device pixels, only the first *nPixels* will be set to *color*;  if *nPixels* is greater than the total number of device pixels, the device will simply ignore the additional input
  
* `void set(Color *color, int nPixels)`

  * individually sets the color of each pixel in a multi-pixel device to the color values specified in the **Color** array *\*color*, of *nPixels* size, where the  first pixel of the device is set to the value in *color\[0\]*, the second pixel is set to the value in *color\[1\]* ... and the last pixel is set to the value in *color\[nPixels-1\]*.  Similar to above, it is not a problem if the value specified for *nPixels* does not match the total number of actual RGB pixels in your device

In both of the methods above, colors are stored in a 32-bit **Color** object configured to hold three 8-bit RGB values plus a 5-bit value that can be used to limit the LED current.  **Color** objects can be instantiated as single variables (e.g. `Dot::Color myColor;`) or as arrays (e.g. `Dot::Color myColors\[8\];`).  Note that the **Color** object used by the **Dot** class is scoped to the **Dot** class itself, so you need to use the fully-qualified class name "Dot::Color".  Once a **Color** object is created, the color it stores can be set using one of the two following methods:
  
  * `Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t driveLevel=31)`

    * where *r*, *g*, and *b*, represent 8-bit red, green, and blue values over the range 0-255, and *driveLevel* represents an 5-bit value [0-31] used to limit the LED current from 0 (no current) to 31 (max current, which is the default).  Limiting the LED current by setting the *driveLevel* to a value of less than 31 provides a flicker-free way of controlling the brightness of the RGB LEDs for each pixel.
    * example: `myColor.RGB(128,128,0)` sets myColor to yellow at half-brightness using a 50% duty cycle for the red and green LEDs (i.e. 128/256)
    * example: `myColor.RGB(255,255,0,16)` sets myColor to yellow at half-brightness by limiting the LED current for the pixel to 50% of its max value (i.e. 16/32)
      
  * `Color HSV(float h, float s, float v, double drivePercent=100)`
    
    * where *h*=Hue, over the range 0-360; *s*=Saturation percentage from 0-100; and *v*=Brightness percentage from 0-100.  These values are converted to equivalent 8-bit RGB values (0-255) for storage in the *Color* object.  The *drivePercent* parameter controls the current in the same fashion as *driveLevel* above, except that instead of being specified as an absolute value from 0-31, it is specified as a percentage from 0 to 100 (the default)
    * example: `myColor.HSV(120,100,50)` sets myColor to fully-saturated green at half-brightness using a 50% duty cycle
    * example: `myColor.HSV(120,100,100,50)` sets myColor to fully-saturated green at half-brightness by limiting the LED current for the pixel to 50% of its max value
      
Note both methods above return the completed **Color** object itself and can thus be used wherever a **Color** object is required:  For example: `Dot p(5,6); Dot::Color myColor; p.set(myColor.RGB(255,215,0))` sets the color of a single pixel device attached to pins 5 and 6 to bright gold.

The **Pixel** class also supports the following class-level methods as a convenient alternative to creating colors:
  
* `static Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t driveLevel=31)`
  * equivalent to `return(Color().RGB(r,g,b,driveLevel));`
  * example: `Dot p(8,11);  p.set(Dot::RGB(0,0,255),8);` sets the color of each pixel in an 8-pixel device to blue

* `static Color HSV(float h, float s, float v, double drivePercent=100)`
  * equivalent to `return(Color().HSV(h,s,v,drivePercent));`
  * example: `Dot::Color c[]={Dot::HSV(120,100,100),Dot::HSV(60,100,100),Dot::HSV(0,100,100)};` to create a red-yellow-green traffic light pattern

Unlike the **Pixel** class, the **Dot** class does *not* utilize the ESP32's RMT peripheral and thus there are no limits to the number of **Dot** objects you can instantiate, nor are there any conflicts with using the **Dot** class and the **RFControl** library at the same time in the same sketch.  Also, since the clock signal is generated by the **Dot** class itself, there are no timing parameters to set and no need for a *setTiming()* method.

### Example Sketches

A fully worked example showing how to use the Pixel library within a HomeSpan sketch to control an RGB Pixel Device, an RGBW Pixel Device, and an RGB DotStar Device, all from the Home App on your iPhone, can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → Pixel*](../examples/Other%20Examples/Pixel).

For a more complete showcase of the Pixel library , check out [Holiday Lights](https://github.com/HomeSpan/HolidayLights) on the [HomeSpan Projects page](https://github.com/topics/homespan).  This sketch demonstrates how the Pixel library can be used to generate a variety of special effects with a 60-pixel RGBW strip.  The sketch also showcases the use of HomeSpan's [Custom Characteristic macro](Reference.md#custom-characteristics-and-custom-services-macros) to implement a special-effects "selector" button for use in the Eve for HomeKit App.

---

[↩️](../README.md) Back to the Welcome page
