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

* `Characteristic::ActiveIdentifier()` - this numerical Characteristic is used to control the input source for the TV (e.g. HDMI-1, HDMI-2, Netflix, etc.).  It is only used when input sources are defined and linked using `Service::InputSource()` (see below), in which case it is a *required* Characteristic

### `Service::InputSource()`

Use `Service::InputSource()` to create a new input source selection for the TV, such as HDMI-1, HDMI-2, Netflix, etc.  The use of `Service::InputSource()` is optional - it is perfectly okay to create a Television Service without the ability to select different Input Sources.  However, if used, each Input Source Service added should be defined in the *same* Accessory as the Television Service to which it applies, and ***must*** be linked to that Television Service using `addLink()`.  The Home App behaves unexpectedly if it finds any Input Source Services that are not linked to a Television Service.

Input Sources can appear in two places within the Home App.  The first is in the Input Source "Selector" that is shown below the On/Off power button when you open the controls for a TV (i.e. long-press the Accessory Tile).  This is how you change the Input Source for the TV.  The second place that Input Sources appear is on the Settings page for a TV Accessory.  This is where you can change the name of an Input Source, as well as configure whether to include or exclude a particular Input Source from the Input Source Selector.

The overall idea is that your sketch should implement a TV Accessory containing a full list of all potential inputs, using names that match the labels on the TV, such as "HDMI 1", "Component 1", "HDMI 2", etc.  If your TV Remote has dedicated buttons for Netflix, HBO Max, Amazon Prime, etc. you can add these to the list as well. Once this generic list is created, you can then rename and enable each Input Source directly from within the Home App.  For example you might rename "HDMI 1" to "Comcast Cable", and "HDMI 2" to "Sony Blue-Ray".  If you have nothing connected to the "Component 1", you can exclude it from the Input Source Selector.  This makes it easy to configure and re-configure your TV Input Sources without ever having to change or update your HomeSpan sketch.

All of this is accomplished by using a combination of some, or all, of the following Characteristics:

* `Characteristic::ConfiguredName()` - similar to how its used when applied to `Service::Television()`, this Characteristic allows you set the default name for an Input Source. Note that if you change the name of an Input Source in the Home App, an update will be sent to HomeSpan with the new name for you to use in your sketch if needed.  This is very different from the usual `Characteristic::Name()` used for many other Services, and for which name changes performed in the Home App are never communicated back to the Accessory

* `Characteristic::Identifier()` - this numerical Characteristic sets an ID for each Input Source.  Any unsigned 32-bit number can be used as an ID, provided it is *unique* and not used by any other Input Source in the same TV Service.  When you use the Input Source Selector in the Home App to choose a particular Input Source, the `Characteristic::ActiveIdentifier()` from the Television Service (see above) will be updated with a value that matches the ID corresponding to the chosen Input Source.  Within HomeSpan you simply use the `update()` method to determine when `Characteristic::ActiveIdentifer()` is updated, and, based on its value, which Input Source was chosen.  HomeKit does not seem to require `Characteristic::Identifier()` be defined for an Input Source.  However, if it not set, the Home App will not allow it to be displayed as a choice in the Input Source Selector, which defeats the purpose of creating an Input Source!

* `Characteristic::IsConfigured()` - this Characteristic determines whether an Input Source is allowed to appear as a choice in the Input Source Selector of the Home App.  If IsConfigured() is defined and set to 0, the Input Source will appear in the Settings page, but it will be excluded as a choice from the Input Source Selector.  If IsConfigured() is defined and set to 1, the Input Source will appear in the Settings page, and will also be included as a choice in the Input Source Selector.  If `Characteristic::IsConfigured()` is not defined  for an Input Source, that source will still appear as a choice in the Input Source Selector, but it will *not* appear in the list of Input Sources found on the Settings page.  This means you will not be able to rename the Input Source from the Home App, nor toggle it as an allowable choice in the Input Selector (see below)

* `Characteristic::CurrentVisibilityState()` and `Characteristic::TargetVisibilityState()` - these two Characteristics work in tandem much like any current-state/target-state pair.  When these are defined for an Input Source, a checkbox toggle appears next to the name of the Input Source on the Settings page, provided `Characteristic::IsConfigured()` has also been defined.  Clicking the checkbox causes the Home App to toggle the TargetVisibilityState between 0 to 1, where 0 ironically means the checkbox is *checked*, and 1 means it is *unchecked* (the reverse of what you might expect!).  If you read this update in HomeSpan you can then use `setVal()` to change the CurrentVisibiltyState() to match the TargetVisibilityState().  Setting CurrentVisibilityState() to 0 means the Input Source appears as a choice in the Input Source Selector.  Setting CurrentVisibilityState() to 1 means it does not appear as a selection.  Note these features only operate if an ID has been set for the Input Source with `Characteristic::Identifier()`, and IsConfigured() has been defined and set to 1

### `Service::TelevisionSpeaker()`

This Service allows you to change the volume of a television using the iPhone's physical volume control buttons when operating the TV via the iPhone's Remote Control widget (found in the iPhone Control Center).  Similar the Input Source Service above, the Television Speaker Service ***must*** be linked to a Television Service using `addLink()`.  The Television Speaker Service requires the following two Characteristics:

* `Characteristic::VolumeControlType()` - this read-only Characteristic seems to be required but there is uncertainty as to its purpose. In the example HomeSpan sketches I initialized this Characteristic with a value of 3 based on what others have done in scripts based on [HomeBridge](https://developers.homebridge.io/#/service/TelevisionSpeaker)

* `Characteristic::VolumeSelector()` - this write-only Characterstic is updated whenever the user is operating the TV via the iPhone's Remote Control widget and the physical volume control buttons are pressed.  The Home App sends a value of 0 when the up-volume button is pressed, and value of 1 when the down-volume button is pressed.  These values can be read the usual way by creating an `update()` method for a class that is derived from the Television Speaker Service

### Examples

Please see [*File → Examples → HomeSpan → Other Examples → Television*](../Other%20Examples/Television) for a complete worked example demonstrating the effects of using different combinations of the above Characteristics.  Also, don't forget to check out the [HomeSpan Projects](https://github.com/topics/homespan) page for some real-world examples of TV sketches and controllers.


### Credits

Much thanks to @unreality for the PR to include Television codes and associated functionality!





---

[↩️](../README.md) Back to the Welcome page
