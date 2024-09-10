# Addressable RGB LEDs

HomeSpan includes two dedicated classes that provide for easy control of "addressable" RGB LEDs.  The **Pixel()** class is used for RGB and RGBW LEDs that require only a single "data" control wire, such as this 8-pixel [NeoPixel RGB Stick](https://www.adafruit.com/product/1426) or this single-pixel [NeoPixel RGBW LED](https://www.adafruit.com/product/2759).  The **Dot()** class is used for RGB LEDs that require two control wires ("data" and "clock"), such as this 144-pixel [DotStar RGB Strip](https://www.adafruit.com/product/2241) or this 60-pixel [RGB LED Strip](https://www.sparkfun.com/products/14015).

Both classes allow you to individually set each of the "pixels" in a multi-pixel LED strip to a different 24-bit RGB (red/green/blue) color.  The Pixel class also supports 16-bit WC (warm-white/cool-white) pixels, 32-bit RGBW (red/green/blue/white) pixels, and 40-bit RGBWC pixels (i.e. red/green/blue/warm-white/cool-white).  Alternatively, the classes allow you to simply specify a single color to duplicate across all pixels.

The methods for both classes are nearly identical, which allows you to readily interchange code written for single-wire devices to use with two-wire devices (and vice-versa) with only minor modifications.

## *Pixel(uint8_t pin, [const char *pixelType])*

Creating an instance of this **class** configures the specified *pin* to output a waveform signal suitable for controlling a single-wire, addressable RGB, WC, RGBW, or RGBWC LED device with an arbitrary number of pixels.  Arguments, along with their defaults if left unspecified, are as follows:

  * *pin* - the pin on which the RGB control signal will be output; normally connected to the "data" input of the addressable LED device
    
  * *pixelType* - a character string that specifies the type of LEDs to be controlled (e.g. WC, or RBGW) as well as the order in which color data is transmitted (e.g. RGB vs GBR).
    * for 3-color devices (e.g. an RGB LED), *pixelType* should be set to the characters "RGB" in whatever order required by your device
    * for 4-color devices (e.g. an RGB LED plus a White LED), *pixelType* should be set to the characters "RGBW" in whatever order required by your device
    * for 5-color devices (e.g. an RGB LED plus separate Warm-White and Cool-White LEDs), *pixelType* should be set to the characters "RGBWC" in whatever order required by your device
    * for 2-color devices (e.g. Warm-White and Cool-White LEDs only), *pixelType* should be set to the characters "WC-" in whatever order required by your device, where "-" represents a dummy byte since manufacturers of warm-white/cool-white LED strips generally use 3-channel device drivers (designed to operate RGB LEDs) but only connect 2 of the channels, say (Red and Green) to the Warm-White and Cool-White LEDs, leaving a third channel (in this example Blue), unconnected.
  * example: `Pixel myDevice(26, "WBRG");` creates a 4-color RGBW device attached to pin 26 where the colors are transmitted in the order white, blue, red, green
  * note that *pixelType* is optional.  If left unspecified, the default value is "GRB"

> [!TIP]
> Since it is often not obvious which type of LED your specific device may have, HomeSpan includes a sketch designed to help you determine the correct value of *pixelType*.  See [*File → Examples → HomeSpan → Other Examples → PixelTester*](../examples/Other%20Examples/PixelTester) under the Arduino IDE for detailed instructions.  Please use this tester sketch if you find the colors of your Pixel device are not matching what appears in the Home App.

The two main methods to set pixel colors are:

* `void set(Color color, int nPixels=1)`

  * sets the color of a pixel in a single-pixel device, or equivalently, the color of the first *nPixels* in a multi-pixel device, to *color*, where *color* is an object of type **Color** defined below.  If unspecified, *nPixels* defaults to 1 (i.e. a single pixel).  It is not a problem if the value specified for *nPixels* does not match the total number of actual RGB (or RGBW) pixels in your device; if *nPixels* is less than the total number of device pixels, only the first *nPixels* will be set to *color*;  if *nPixels* is greater than the total number of device pixels, the device will simply ignore the additional input
  
* `void set(Color *color, int nPixels)`

  * individually sets the color of each pixel in a multi-pixel device to the color values specified in the **Color** array *\*color*, of *nPixels* size, where the  first pixel of the device is set to the value in *color\[0\]*, the second pixel is set to the value in *color\[1\]* ... and the last pixel is set to the value in *color\[nPixels-1\]*.  Similar to above, it is not a problem if the value specified for *nPixels* does not match the total number of actual RGB (or RGBW, etc.) pixels in your device

  * note that the number of LEDs on a light strip may be more than the total number of controllable pixels.  For 5-volt light strips there is usually just one LED per driver chip.  For 12-volt and 24-volt light strips, manufacturers often connect multiple LEDs to each pixel-driver chip.  For instance, a 300-LED strip may contain 100 pixel-driver chips, where each chip controls three LEDs (set to the same color).  Always set *nPixels* to the number of pixel-driver chips in your light strip, NOT to the number of LEDs

In both of the methods above, colors are natively stored by the Pixel class as 40-bit **Color** object configured to hold five 8-bit values in the order RGBWC (regardless of whether *pixelType* is set to "RGB", "WGRB", "W-C", etc.).  **Color** objects can be instantiated as single variables (e.g. `Pixel::Color myColor;`) or as arrays (e.g. `Pixel::Color myColors[8];`).  Note that the **Color** object used by the **Pixel** class is scoped to the **Pixel** class itself, so you need to use the fully-qualified class name "Pixel::Color".
Once a **Color** object is created, the color it stores can be set using one of the two following methods:
  
  * `Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint8_t c=0)`

    * where *r*, *g*, and *b*, represent 8-bit red, green, and blue values over the range 0-255, *w* represents an optional 8-bit value for warm-white, and *c* represents an optional 8-bit value for cool-white.  The white values may be left unspecified, in which case they default to 0      
    * returns a **Color** object
    * example: `myColor.RGB(255,255,0)` sets myColor to bright yellow
    * example: `myColor.RGB(255,0,0,128,128) sets myColor to a a combination maximium-intensity red with half-intensity warm- and cool-white
      
  * `Color HSV(float h, float s, float v, double w=0, double c=0)`
    
    * where *h*=Hue, over the range 0-360; *s*=Saturation percentage from 0-100; and *v*=Brightness percentage from 0-100.  These values are converted to equivalent 8-bit RGB values (0-255) for storage in the **Color** object.  Note the *w* and *c* values are treated separately and represent optional brightness percentages the warm-white and cool-white LEDs.  Similar to above, the warm-white and cool-white values may be left unspecified, in which case they default to 0
    * returns a **Color** object
    * example: `myColor.HSV(120,100,50)` sets myColor to fully-saturated green with 50% brightness

  * `Color WC(uint8_t w, uint8_t c)`
    
    * sets just the warm-white and cool-white potions of a **Color** object to *w* and *c*, respectively, where values range from 0-255
    * this method is identical to calling `RGB(0,0,0,w,c);`
    * returns a **Color** object
   
  * `Color CCT(float temp, float v, float wTemp, float cTemp)`

    * sets the warm-white and cool-white portions of a **Color** object to the Correlated-Color Temperature *temp* (in Kelvins) with Brightness percentage *v* (0-100), based on user-specified values of the warm-white (*wTemp*) and cool-white (*cTemp*) LEDs (in Kelvins)
    * returns a **Color** object
    * example: `myColor.CCT(4000,100,3000,6500)` sets myColor to a CCT-equivalent of 4000K with 100% brightness by interpolating the user-specified temperatures of the warm-white (3000K) and cool-white (6500K) LEDs.  Note the result of this calculation is equivalent to `myColor.WC(183,72)`
    * example: `myColor.CCT(4000,25,3000,6500)` sets myColor to a CCT-equivalent of 4000K with 25% brightness by interpolating the user-specified temperatures of the warm-white (3000K) and cool-white (6500K) LEDs, and then scaling these values by a factor of 25%.  Note the result of this calculation is equivalent to `myColor.WC(45,18)`
    

Note both methods above return the completed **Color** object itself and can thus be used wherever a **Color** object is required:  For example: `Pixel p(5); Pixel::Color myColor; p.set(myColor.RGB(255,215,0))` sets the color of a single pixel device attached to pin 5 to bright gold.

The **Pixel** class also supports the following class-level methods as a convenient alternative to creating colors:
  
* `static Color RGB(uint8_t r, uint8_t g, uint8_t b, uint8_t w=0, uint8_t c=0)`
  * equivalent to `return(Color().RGB(r,g,b,w,c));`
  * returns a **Color** object
  * example: `Pixel p(5,8);  p.set(Pixel::RGB(0,0,255),8);` sets the color of each pixel in an 8-pixel device (attached to pin 5) to blue

* `static Color HSV(float h, float s, float v, double w=0, double c=0)`
  * equivalent to `return(Color().HSV(h,s,v,w,c));`
  * returns a **Color** object
  * example: `Pixel::Color c[]={Pixel::HSV(120,100,100),Pixel::HSV(60,100,100),Pixel::HSV(0,100,100)};` to create a red-yellow-green traffic light pattern

* `static Color WC(uint8_t w, uint8_t c)`
  * equivalent to `return(Color().WC(w,c));`
   * returns a **Color** object
  * example: `Pixel p(5);  p.set(Pixel::WC(0,128));` sets the color of a single pixel-device (attached to pin 5) to half-intensity cool-white

Finally, the **Pixel** class supports these additional object-level methods:

* `Color CCT(float temp, float v)`
  * an object-level method that is equivilant to `return(Color().CCT(temp,v,warmTemp,coolTemp));`, where *warmTemp* and *coolTemp* are device-specific warm-white and cool-white LED temperatures set using the `setTemperatures()` method below
  * returns a **Color** object
 
  ...TBD...

* `int getPin()`

  * returns the pin number, or -1 if the instantiation failed due to lack of resources (see below) or for any other reason
    
* `void setTiming(float high0, float low0, float high1, float low1, uint32_t lowReset)`

  * the default timing parameters used by the **Pixel** class to generate the "data" signal needed to set the colors of an RGB LED device should work with most commercial products based on SK6812 or WS2812 driver chips.  Use this method **ONLY** if you need to override the class defaults and replace them with your own timing parameters, where
    * *high0* and *low0* specify the duration (in microseconds) of the high phase and low phase for a pulse encoding a zero-bit;
    * *high1* and *low1* specify the duration (in microseconds) of the high phase and low phase for a pulse encoding a one-bit; and
    * *lowReset* specifies the delay (in microseconds) representing the end of a pulse stream
   * for reference, the **Pixel** class uses the following defaults: *high0=0.32𝛍s, low0=0.88𝛍s, high1=0.64𝛍s, low1=0.56𝛍s, lowReset=80.0𝛍s*
 
> [!TIP]
> If your LED colors are flickering, this is likely due to a mismatch in timing parameters.  To solve, please search the web for a specifications document for the exact **chip** used by your specific device.  These documents generally provide a table listing the durations of the HIGH and LOW periods required to transmit a binary 1 or binary 0.  You can then use the `setTiming` method above to set the timing parameters accordingly.

### Resource Usage

The **Pixel** class relies on the ESP32's RMT peripheral to create the precise pulse trains required to control single-wire addressable RGB LEDs.  Since each instantiation of **Pixel** consumes an RMT channel, the number of **Pixel** objects you can instantiate (each controlling a separate multi-pixel RGB LED device attached to a specific pin) is limited to the number of RMT available as follows: ESP32 - 8 instances; ESP32-S2 - 4 instances; ESP32-C3 and ESP32-C6 - 2 instances.

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

Unlike the **Pixel** class, the **Dot** class does *not* utilize the ESP32's RMT peripheral and thus there are no limits to the number of **Dot** objects you can instantiate.  Also, since the clock signal is generated by the **Dot** class itself, there are no timing parameters to set and no need for a *setTiming()* method.

### Example Sketches

A fully worked example showing how to use the Pixel library within a HomeSpan sketch to control an RGB Pixel Device, an RGBW Pixel Device, and an RGB DotStar Device, all from the Home App on your iPhone, can be found in the Arduino IDE under [*File → Examples → HomeSpan → Other Examples → Pixel*](../examples/Other%20Examples/Pixel).

For a more complete showcase of the Pixel library , check out [Holiday Lights](https://github.com/HomeSpan/HolidayLights) on the [HomeSpan Projects page](https://github.com/topics/homespan).  This sketch demonstrates how the Pixel library can be used to generate a variety of special effects with a 60-pixel RGBW strip.  The sketch also showcases the use of HomeSpan's [Custom Characteristic macro](Reference.md#custom-characteristics-and-custom-services-macros) to implement a special-effects "selector" button for use in the Eve for HomeKit App.

---

[↩️](../README.md) Back to the Welcome page
