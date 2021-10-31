# Television Services and Characteristics

HomeSpan includes a number of undocumented Television Services and Characteristics that are not part of HAP-R2.  Though the UUID and specifications for each Television Service and Characteristic have been identified by the broader HomeKit community, it is only by trial-and-error that proper usage can be determined.  This page documents the results of experimenting with the different Television Services and Characteristics within HomeSpan using the Home App provided in iOS 15.1.  This documentaiton should be considered reliable, but Apple can of course change the behavior of such Service and Characteristics within the Home App at any time and without notice.

### `Category::Television`

Use `Category::Television` as the category in `homeSpan.begin()` to specify a Television Accessory.  This causes the Home App to display an  TV icon in the Accessory Tile.  However, this only seems to work for the first Accessory implemented.  If you create a device with multiple Television Accessories, or place a Television Accessory behind a Bridge Accessory, the icon for the TV Tile converts to a generic HomeKit symbol.

### `Service::Television()`

Use `Service::Television()` as the Service to create a Television Accessory Tile.  It support two primary Characteristics:

* `Characteristic::Active()` - this HAP-R2 standard Characteristic it used to turn the TV on or off.  It is a required Characteristic

* `Characteristic::ConfiguredName()` - this is an optional, TV-specific version of `Characteristic::Name()` that seems to be the only way to set the default name of the TV.  Unlike all other HomeKit Services, the Home App ignores any names specified with `Characeteristic::Name()` when used with `Service::Television()`

Based on the above, the following code snippet defines a simple TV Accessory with a basic on/off switch:

```C++
new Service::Television();
  new Characteristic::Active(0);                    // set power to OFF at start-up
  new Characteristic::ConfiguredName("Sony TV");    // optional Characteristic to set name of TV
```
More advanced control of a TV can enabled with two other optional Characteristics:

* `Characteristic::RemoteKey()` - this write-only numerical Characteristic enables HomeSpan to read button presses from the Remote Control widget on an iPhone that can be found under the Control Center.  This widget is normally used to control Apple TVs, but it seems any Television Accessory created per above can also be operated from the Remote Control widget.  The layout of the widget (which cannot be modified) includes 4 arrows, a central select button, a play/pause button, a large "back" button, and an "info" button.  When a "key" is pressed, the Home App sends an update to `Characteristic::RemoteKey()` that can be read by HomeSpan using the usual `update()` method.  Values are as follows:
  
  * 4 = up arrow  
  * 5 = down arrow
  * 6 = left arrow
  * 7 = right arrow
  * 8 = center select button
  * 9 = back button
  * 11 = play/pause button
  * 15 = info button
  
* `Characteristic::PowerModeSelection()` - this write-only Characteristic causes the text "View TV Settings" to appear in the Home App under the Settings page for a TV Accessory.  When this text is pressed, the Home App sends an update with value=0 to `Characteristic::PowerModeSelection()` that can be read by HomeSpan using the usual `update()` method










---

[↩️](README.md) Back to the Welcome page
