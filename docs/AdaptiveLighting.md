# Adaptive Lighting

*Adaptive Lighting* is an Apple HomeKit feature that lets a Home Hub (typically a HomePod or Apple TV) upload a **color-temperature schedule** to a compatible LightBulb.  The bulb then automatically shifts its color temperature over the course of the day - warm in the morning, cool during the middle of the day, and warm again in the evening - without any further intervention from the Home Hub.

The Adaptive Lighting protocol itself is *undocumented by Apple*.  HomeSpan's implementation is based on the reverse-engineered specification published in the [HAP-NodeJS project](https://github.com/homebridge/HAP-NodeJS), and has been verified against the traffic produced by real HomeKit controllers.

## Requirements

Adaptive Lighting is only offered by the Home App on a LightBulb Service that satisfies **all** of the following:

* the Service is `Service::LightBulb`
* the Service contains a `Characteristic::On`
* the Service contains a `Characteristic::Brightness`
* the Service contains a `Characteristic::ColorTemperature`
* the Service contains the three (undocumented) Adaptive-Lighting Characteristics:
  * `Characteristic::SupportedCharacteristicValueTransitionConfiguration`  (TAG 0x144)
  * `Characteristic::CharacteristicValueTransitionControl`                 (TAG 0x143)
  * `Characteristic::CharacteristicValueActiveTransitionCount`             (TAG 0x24B)

You do **not** have to instantiate the last three Characteristics yourself.  HomeSpan provides the `AdaptiveLightingController` helper class (see below) that creates and manages them for you.

## Overview of the Protocol

When Adaptive Lighting is enabled from the Home App, the Home Hub uploads a 24-hour *schedule* to the LightBulb by writing to the `CharacteristicValueTransitionControl` Characteristic.  The schedule consists of:

* a monotonically-increasing list of **curve entries**, each of which specifies:
  * a target color temperature (in [mireds](https://en.wikipedia.org/wiki/Mired))
  * a *brightness adjustment factor* (mireds per Brightness percent)
  * a *transition time* (how long to spend interpolating from the previous entry)
  * an optional *duration* (how long to hold the entry before moving on to the next)
* a **brightness range** (typically 10..100) over which the adjustment factor applies
* an **update interval** (typically 60000 ms) at which the bulb should re-evaluate its position on the curve
* an **notification interval** (typically 600000 ms) at which the bulb should broadcast its new `ColorTemperature` value to HomeKit controllers

At every *update interval* the bulb linearly interpolates the two curve entries surrounding the current point in time, applies the brightness adjustment, and writes the resulting mired value to its `ColorTemperature` Characteristic.  The bulb also emits an EV notification every *notification interval* (or immediately if the mired value changes by a significant amount) so the Home App can reflect the current color.

If the user manually changes the ColorTemperature slider in the Home App the schedule is automatically cancelled - Apple treats a manual color change as an explicit override.

## *AdaptiveLightingController(SpanCharacteristic \*brightness, SpanCharacteristic \*colorTemperature)*

Creates an instance of the Adaptive-Lighting controller.  The two arguments are pointers to the `Brightness` and `ColorTemperature` Characteristics of the LightBulb Service that will support Adaptive Lighting.

This class **must** be instantiated **inside a LightBulb Service constructor** and **after** the `Brightness` and `ColorTemperature` Characteristics of that Service have themselves been constructed.  The controller internally creates the three Adaptive-Lighting Characteristics listed above; they attach to the currently-constructing Service just like any other Characteristic.

The class provides the following methods:

* `boolean handleUpdate()`

  * Call from your Service's `update()` override.  Returns *true* if a schedule was loaded, refreshed, or disabled by the Home Controller during this cycle.

* `void poll()`

  * Call from your Service's `loop()` override (or from the main Arduino `loop()`).  Cheap when no schedule is loaded; otherwise re-evaluates the current point on the curve every *update interval* milliseconds and writes the resulting value to the `ColorTemperature` Characteristic.

* `boolean isActive() const`

  * Returns *true* if a schedule is currently loaded, otherwise *false*.

* `void disable()`

  * Cancels any active schedule and notifies HomeKit.  Called automatically if the user manually writes a new value to `ColorTemperature`.

* `void printSchedule() const`

  * Prints the currently-loaded schedule to `Serial`, one row per curve entry, with columns for the cumulative wall-clock offset, mired value, equivalent color temperature in Kelvin, brightness adjustment factor, resolved mired at the minimum/maximum brightness, transition time, and hold duration.  Useful for inspecting what the Home Controller has uploaded.

* `void setColorTemperatureCallback(std::function<void(uint32_t newMired)> cb)`

  * Registers a callback invoked whenever the controller writes a new value to `ColorTemperature`.  Useful for driving external hardware that does not read directly from `ColorTemperature->getVal()` (for example, an addressable RGBW LED strip that computes its own drive values).

* `void setStateCallback(std::function<void(bool active)> cb)`

  * Registers a callback invoked with *true* whenever a new schedule is accepted, and with *false* whenever Adaptive Lighting becomes inactive (either because the user disabled it or the schedule ran out).

## Example Sketch

The simplest possible integration looks like this:

```C++
#include "HomeSpan.h"

struct AdaptiveLight : Service::LightBulb {
  SpanCharacteristic *power       = new Characteristic::On(0);
  SpanCharacteristic *brightness  = new Characteristic::Brightness(100);
  SpanCharacteristic *colorTemp   = new Characteristic::ColorTemperature(250);
  AdaptiveLightingController adaptiveLighting{brightness, colorTemp};

  boolean update() override {
    adaptiveLighting.handleUpdate();
    // ...drive your bulb hardware here using power/brightness/colorTemp->getNewVal()...
    return true;
  }

  void loop() override {
    adaptiveLighting.poll();
  }
};

void setup() {
  Serial.begin(115200);
  homeSpan.begin(Category::Lighting, "Adaptive Light");
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
    new AdaptiveLight();
}

void loop() { homeSpan.poll(); }
```

A full worked example - complete with a `SpanUserCommand` that prints the currently-loaded schedule to the Serial Monitor - can be found in [Example&nbsp;23 - AdaptiveLighting](../examples/23-AdaptiveLighting).

## Inspecting the Schedule

Calling `printSchedule()` on an active controller produces output similar to:

```
Adaptive Lighting Schedule: 24 entries, update=60000ms, notify=600000ms, bRange=10..100
  idx    t+(h:mm:ss)   mired      K   bFactor    mired@b10   mired@b100    trans     hold
    0     0:00:00    154.00   6493   +0.4500        158          199         0        0
    1     0:30:00    154.00   6493   +0.4500        158          199    1800000  1800000
    2     3:30:00    250.00   4000   +0.7000        257          320    1800000        0
    ...
```

* **t+(h:mm:ss)** - cumulative wall-clock offset (relative to when the schedule was loaded)
* **mired** / **K** - the target color temperature for that curve point, in [mireds](https://en.wikipedia.org/wiki/Mired) and the equivalent temperature in Kelvin
* **bFactor** - the brightness adjustment factor (mireds per brightness percent)
* **mired@b10** / **mired@b100** - the resolved mired value at the minimum and maximum brightness values from the uploaded range
* **trans** - the time to spend linearly interpolating from the previous entry to this one, in milliseconds
* **hold** - the time to hold this entry before moving on to the next, in milliseconds

## Notes on Time

The Adaptive-Lighting protocol delivers a schedule anchored to a wall-clock timestamp (milliseconds since 2001-01-01 UTC).  The HomeSpan implementation treats that timestamp as opaque and simply advances the schedule using the ESP32's monotonic `millis()` clock, starting from the moment the schedule is received.  In practice this is close enough that a HomePod-generated schedule uploaded at 6:00 AM local time will produce a fairly natural warm-to-cool-to-warm cycle over the following 24 hours.

If you want stricter alignment with the real wall clock, initialize an NTP client after WiFi is up and offset the local start of the schedule accordingly.  The relevant Arduino calls are:

```C++
#include <time.h>
configTime(0, 0, "pool.ntp.org");   // pull UTC from NTP
```

The 8-byte `startTimeBuf` in each accepted schedule is a little-endian `int64_t` count of milliseconds since 2001-01-01 UTC (the Apple "reference date"), so:

```C++
int64_t startMsSince2001;
memcpy(&startMsSince2001, adaptiveLighting.getTransitionControl()->..., 8);   // see source
int64_t startUnixMs = startMsSince2001 + 978307200000LL;                       // 2001-01-01 UTC
```

can be compared against the current time from `time(nullptr)` to compute a wall-clock offset.  Extending the controller to consume this timestamp is straightforward; the default behavior is chosen to keep the reference implementation dependency-free.

## Multiple Bulbs

Each LightBulb Service that wants Adaptive Lighting needs its own `AdaptiveLightingController` instance (the three Adaptive-Lighting Characteristics are per-Service).  There is no shared state across bulbs.

## Notes and Caveats

* Adaptive Lighting is a *client feature*: whether the Home App offers the toggle at all depends on the Controller (iOS/macOS version, hub type) and on the Home App having recently pushed a schedule.  If the toggle does not appear immediately after re-pairing, wait a minute for HomeKit to notice the new capability.
* HomeSpan does not persist an active schedule across reboots.  If the device restarts the Home Hub will re-upload the schedule the next time it is contacted.
* Because the controller writes to `ColorTemperature` internally, your Service's `update()` will *not* be called for those writes.  If your bulb drives its hardware from `getNewVal()` inside `update()`, use `setColorTemperatureCallback()` to be notified of AL-driven changes as well.

---

[↩️](../README.md) Back to the Welcome page
