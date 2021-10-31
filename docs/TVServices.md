# Television Services and Characteristics

HomeSpan includes a number of undocumented Television Services and Characteristics that are not part of HAP-R2.  Though the UUID and specifications for each Television Service and Characteristic have been identified by the broader HomeKit community, it is only by trial-and-error that proper usage can be determined.  This page documents the results of experimenting with the different Television Services and Characteristics within HomeSpan using the Home App provided in iOS 15.1.  This documentaiton should be considered reliable, but Apple can of course change the behavior of such Service and Characteristics within the Home App at any time and without notice.

### `Category::Television`

Use `Category::Television` as the category in `homeSpan.begin()` to specify a Television Accessory.  This causes the Home App to display an  TV icon in the Accessory Tile.  However, this only seems to work for the first Accessory implemented.  If you create a device with multiple Television Accessories, or place a Television Accessory behind a Bridge Accessory, the icon for the TV Tile converts to a generic HomeKit symbol.

### `Service::Television()`

Use `Service::Television()` as the Service to create a Television Accessory Tile.  It support two primary Characteristics:

* `Characteristic::Active()` - this is the standard HomeKit Active Characteristic used to turn the TV on or off.  It is a required Characteristic
* `Characteristic::ConfiguredName()` - this is an optional, TV-specific version of `Characteristic::Name()` that seems to be the only way to set the default name of the TV.  Unlike all other HomeKit Services, the Home App ignores any names specified with `Characeteristic::Name()` when used with `Service::Television()`

Based on the above, the following code snippet defines a simple TV Accessory with a basic on/off switch:

```C++
new Service::Television();
  new Characteristic::Active(0);                    // set power to OFF at start-up
  new Characteristic::ConfiguredName("My TV");      // optional Characteristic to set name of TV
```








---

[↩️](README.md) Back to the Welcome page
