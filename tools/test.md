## AccessoryInformation (3E)
<details><summary> Required Identification Information.  For each Accessory in a HomeSpan device this <i>must</i> be included as the first Service.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Identify (14) :small_blue_diamond</b><ul><li> the Home App sets this to RUN_ID when it wants the device to run its identification routine</li></ul></td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>IDLE&nbsp(0)</b></li><li>RUN_ID&nbsp(1)</li></ul></td></tr>
<tr><td><b>FirmwareRevision (52) </b><ul><li> must be in form x[.y[.z]] - informational only</li></ul></td><td align="center">string</td><td align="center">PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
<tr><td><b>Manufacturer (20) </b><ul><li> any string - informational only</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HomeSpan"</td></tr>
<tr><td><b>Model (21) </b><ul><li> any string - informational only</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HomeSpan-ESP32"</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>SerialNumber (30) </b><ul><li></li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HS-12345"</td></tr>
<tr><td><b>HardwareRevision (53) </b><ul><li> must be in form x[.y[.z]] - informational only</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
</table></details>

## AirPurifier (BB)
<details><summary> Defines a basic Air Purifier with an optional fan.  Optional Linked Services: <b>FilterMaintenance</b>, <b>AirQualitySensor</b>, <b>Fan</b>, and <b>Slat</b></summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>CurrentAirPurifierState (A9) :small_blue_diamond</b><ul><li> indicates current state of air purification</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>PURIFYING&nbsp(2)</li></ul></td></tr>
<tr><td><b>TargetAirPurifierState (A8) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL&nbsp(0)</li><li><b>AUTO&nbsp(1)</b></li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>RotationSpeed (29) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>SwingMode (B6) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><b>LockPhysicalControls (A7) </b><ul><li> indicates if local control lock is enabled</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## AirQualitySensor (8D)
<details><summary> Defines an Air Quality Sensor. </summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>AirQuality (95) :small_blue_diamond</b><ul><li> a subjective description</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">5</td><td><ul><li><b>UNKNOWN&nbsp(0)</b></li><li>EXCELLENT&nbsp(1)</li><li>GOOD&nbsp(2)</li><li>FAIR&nbsp(3)</li><li>INFERIOR&nbsp(4)</li><li>POOR&nbsp(5)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>OzoneDensity (C3) </b><ul><li> measured in &micro;g/m<sup>3</sup></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>NitrogenDioxideDensity (C4) </b><ul><li> measured in &micro;g/m<sup>3</sup></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>SulphurDioxideDensity (C5) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>PM25Density (C6) </b><ul><li> 2.5-micron particulate density, measured in &micro;g/m<sup>3</sup></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>PM10Density (C7) </b><ul><li> 10-micron particulate density, measured in &micro;g/m<sup>3</sup></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>VOCDensity (C8) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## BatteryService (96)
<details><summary> Defines a standalone Battery Service.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>BatteryLevel (68) :small_blue_diamond</b><ul><li> measured as a percentage</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>ChargingState (8F) :small_blue_diamond</b><ul><li> indicates state of battery charging</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>NOT_CHARGING&nbsp(0)</b></li><li>CHARGING&nbsp(1)</li><li>NOT_CHARGEABLE&nbsp(2)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## CarbonDioxideSensor (97)
<details><summary> Defines a Carbon Dioxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CarbonDioxideDetected (92) :small_blue_diamond</b><ul><li> indicates if abnormal level is detected</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NORMAL&nbsp(0)</b></li><li>ABNORMAL&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><b>CarbonDioxideLevel (93) </b><ul><li> measured on parts per million (ppm)</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td align="center">0</td></tr>
<tr><td><b>CarbonDioxidePeakLevel (94) </b><ul><li> measured in parts per million (ppm)</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td align="center">0</td></tr>
</table></details>

## CarbonMonoxideSensor (7F)
<details><summary> Defines a Carbon Monoxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CarbonMonoxideDetected (69) :small_blue_diamond</b><ul><li> indicates if abnormal level is detected</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NORMAL&nbsp(0)</b></li><li>ABNORMAL&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><b>CarbonMonoxideLevel (90) </b><ul><li> measured in parts per million (ppm)</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>CarbonMonoxidePeakLevel (91) </b><ul><li> measured in parts per million (ppm)</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## ContactSensor (80)
<details><summary> Defines a Contact Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>ContactSensorState (6A) :small_blue_diamond</b><ul><li> indictates if contact is detected (i.e. closed)</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>DETECTED&nbsp(0)</li><li><b>NOT_DETECTED&nbsp(1)</b></li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Door (81)
<details><summary> Defines a motorized Door.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentPosition (6D) :small_blue_diamond</b><ul><li> current position (as a percentage) from fully closed (0) to full open (100)</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>TargetPosition (7C) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>ObstructionDetected (24) </b><ul><li> indicates if obstruction is detected</li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

## Doorbell (121)
<details><summary> Defines a Doorbell.  Can be used on a standalone basis or in conjunction with a <b>LockMechanism</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>ProgrammableSwitchEvent (73) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>SINGLE_PRESS&nbsp(0)</b></li><li>DOUBLE_PRESS&nbsp(1)</li><li>LONG_PRESS&nbsp(2)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Fan (B7)
<details><summary> Defines a Fan.  Can be used in conjunction with a <b>LightBulb</b> Service to create a Lighted Ceiling Fan.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>CurrentFanState (AF) </b><ul><li> indicates current state of a fan</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>BLOWING&nbsp(2)</li></ul></td></tr>
<tr><td><b>TargetFanState (BF) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL&nbsp(0)</li><li><b>AUTO&nbsp(1)</b></li></ul></td></tr>
<tr><td><b>RotationDirection (28) </b><ul><li></li></ul></td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CLOCKWISE&nbsp(0)</b></li><li>COUNTERCLOCKWISE&nbsp(1)</li></ul></td></tr>
<tr><td><b>RotationSpeed (29) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>SwingMode (B6) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><b>LockPhysicalControls (A7) </b><ul><li> indicates if local control lock is enabled</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## Faucet (D7)
<details><summary> Defines the master control for a multi-Valve appliance.  Linked Services: <b>Valve</b> (at least one <i>required</i>), and <b>HeaterCooler</b> (optional).</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## FilterMaintenance (BA)
<details><summary> Defines a Filter Maintainence check.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>FilterChangeIndication (AC) :small_blue_diamond</b><ul><li> indicates state of filter</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_CHANGE_NEEDED&nbsp(0)</b></li><li>CHANGE_NEEDED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>FilterLifeLevel (AB) </b><ul><li> measures as a percentage of remaining life</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>ResetFilterIndication (AD) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW</td><td align="center">1</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## GarageDoorOpener (41)
<details><summary> Defines a motorized Garage Door Opener.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentDoorState (E) :small_blue_diamond</b><ul><li> indicates current state of a door</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>OPEN&nbsp(0)</li><li><b>CLOSED&nbsp(1)</b></li><li>OPENING&nbsp(2)</li><li>CLOSING&nbsp(3)</li><li>STOPPED&nbsp(4)</li></ul></td></tr>
<tr><td><b>TargetDoorState (32) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OPEN&nbsp(0)</li><li><b>CLOSED&nbsp(1)</b></li></ul></td></tr>
<tr><td><b>ObstructionDetected (24) :small_blue_diamond</b><ul><li> indicates if obstruction is detected</li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><b>LockCurrentState (1D) </b><ul><li> indictates state of a lock</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>UNLOCKED&nbsp(0)</b></li><li>LOCKED&nbsp(1)</li><li>JAMMED&nbsp(2)</li><li>UNKNOWN&nbsp(3)</li></ul></td></tr>
<tr><td><b>LockTargetState (1E) </b><ul><li> indicates desired state of lock</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>UNLOCK&nbsp(0)</b></li><li>LOCK&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>Version (37) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
</table></details>

## HeaterCooler (BC)
<details><summary> Defines a standalone Heater, Cooler, or combined Heater/Cooler.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>CurrentTemperature (11) :small_blue_diamond</b><ul><li> current temperature measured in Celsius</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>CurrentHeaterCoolerState (B1) :small_blue_diamond</b><ul><li> indicates whether appliance is currently heating, cooling, idle, or off</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>HEATING&nbsp(2)</li><li>COOLING&nbsp(3)</li></ul></td></tr>
<tr><td><b>TargetHeaterCoolerState (B2) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>AUTO&nbsp(0)</b></li><li>HEAT&nbsp(1)</li><li>COOL&nbsp(2)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>RotationSpeed (29) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>TemperatureDisplayUnits (36) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CELSIUS&nbsp(0)</b></li><li>FAHRENHEIT&nbsp(1)</li></ul></td></tr>
<tr><td><b>SwingMode (B6) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><b>CoolingThresholdTemperature (D) </b><ul><li> cooling turns on when temperature (in Celsius) rises above this threshold</li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td align="center">10</td></tr>
<tr><td><b>HeatingThresholdTemperature (12) </b><ul><li> heating turns on when temperature (in Celsius) falls below this threshold</li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td align="center">16</td></tr>
<tr><td><b>LockPhysicalControls (A7) </b><ul><li> indicates if local control lock is enabled</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## HumidifierDehumidifier (BD)
<details><summary> Defines a Humidifer, Dehumidifier, or combined Humidifer/Dehumidifier.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>CurrentRelativeHumidity (10) :small_blue_diamond</b><ul><li>current humidity measured as a percentage</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>CurrentHumidifierDehumidifierState (B3) :small_blue_diamond</b><ul><li> indicates current state of humidifier/dehumidifer</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>HUMIDIFYING&nbsp(2)</li><li>DEHUMIDIFYING&nbsp(3)</li></ul></td></tr>
<tr><td><b>TargetHumidifierDehumidifierState (B4) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>AUTO&nbsp(0)</b></li><li>HUMIDIFY&nbsp(1)</li><li>DEHUMIDIFY&nbsp(2)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>RelativeHumidityDehumidifierThreshold (C9) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">50</td></tr>
<tr><td><b>RelativeHumidityHumidifierThreshold (CA) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">50</td></tr>
<tr><td><b>RotationSpeed (29) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>SwingMode (B6) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><b>WaterLevel (B5) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>LockPhysicalControls (A7) </b><ul><li> indicates if local control lock is enabled</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## HumiditySensor (82)
<details><summary> Defines a Humidity Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentRelativeHumidity (10) :small_blue_diamond</b><ul><li>current humidity measured as a percentage</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## InputSource (D9)
<details><summary> Defines an Input Source for a TV.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>ConfiguredName (E3) </b><ul><li></li></ul></td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>IsConfigured (D6) </b><ul><li> indicates if a predefined Service has been configured</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_CONFIGURED&nbsp(0)</b></li><li>CONFIGURED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Identifier (E6) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint32</td><td align="center">PR</td><td align="center">0</td><td align="center">255</td><td align="center">0</td></tr>
<tr><td><b>CurrentVisibilityState (135) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
<tr><td><b>TargetVisibilityState (134) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## IrrigationSystem (CF)
<details><summary> Defines an Irrigation System.  Linked Services: <b>Valve</b> Service (at least one <i>required</i>).</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>ProgramMode (D1) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>NONE&nbsp(0)</b></li><li>SCHEDULED&nbsp(1)</li><li>SCHEDULE_OVERRIDEN&nbsp(2)</li></ul></td></tr>
<tr><td><b>InUse (D2) :small_blue_diamond</b><ul><li> if Service is set to active, this indictes whether it is currently in use</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_IN_USE&nbsp(0)</b></li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><b>RemainingDuration (D4) </b><ul><li></li></ul></td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
</table></details>

## LeakSensor (83)
<details><summary> Defines a Leak Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>LeakDetected (70) :small_blue_diamond</b><ul><li> indictates if a leak is detected</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## LightBulb (43)
<details><summary> Defines any type of Light.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>On (25) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><b>Brightness (8) </b><ul><li> measured as a percentage</li></ul></td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Hue (13) </b><ul><li> color (in degrees) from red (0) to green (120) to blue (240) and back to red (360)</li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">360</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>Saturation (2F) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>ColorTemperature (CE) </b><ul><li> measured in inverse megaKelvin (= 1,000,000 / Kelvin)</li></ul></td><td align="center">uint32</td><td align="center">PR+PW+EV</td><td align="center">140</td><td align="center">500</td><td align="center">200</td></tr>
</table></details>

## LightSensor (84)
<details><summary> Defines a Light Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentAmbientLightLevel (6B) :small_blue_diamond</b><ul><li> measured in Lux (lumens/m<sup>2</sup></li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0.0001</td><td align="center">100000</td><td align="center">1</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## LockMechanism (45)
<details><summary> Defines an electronic Lock.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>LockCurrentState (1D) :small_blue_diamond</b><ul><li> indictates state of a lock</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>UNLOCKED&nbsp(0)</b></li><li>LOCKED&nbsp(1)</li><li>JAMMED&nbsp(2)</li><li>UNKNOWN&nbsp(3)</li></ul></td></tr>
<tr><td><b>LockTargetState (1E) :small_blue_diamond</b><ul><li> indicates desired state of lock</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>UNLOCK&nbsp(0)</b></li><li>LOCK&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>Mute (11A) :small_blue_diamond</b><ul><li> not used</li></ul></td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>Volume (119) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## MotionSensor (85)
<details><summary> Defines a Motion Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>MotionDetected (22) :small_blue_diamond</b><ul><li> indicates if motion is detected</li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## OccupancySensor (86)
<details><summary> Defines and Occupancy Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>OccupancyDetected (71) :small_blue_diamond</b><ul><li> indicates if occupanccy is detected</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Outlet (47)
<details><summary> Defines an controllable Outlet used to power any light or appliance.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>On (25) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><b>OutletInUse (26) :small_blue_diamond</b><ul><li> indicates if an appliance or light is plugged into the outlet, regardless of whether on or off </li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_IN_USE&nbsp(0)</b></li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## SecuritySystem (7E)
<details><summary> Defines a Security System.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>SecuritySystemCurrentState (66) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>ARMED_STAY&nbsp(0)</li><li>ARMED_AWAY&nbsp(1)</li><li>ARMED_NIGHT&nbsp(2)</li><li><b>DISARMED&nbsp(3)</b></li><li>ALARM_TRIGGERED&nbsp(4)</li></ul></td></tr>
<tr><td><b>SecuritySystemTargetState (67) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>ARM_STAY&nbsp(0)</li><li>ARM_AWAY&nbsp(1)</li><li>ARM_NIGHT&nbsp(2)</li><li><b>DISARM&nbsp(3)</b></li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>SecuritySystemAlarmType (8E) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>KNOWN&nbsp(0)</b></li><li>UNKNOWN&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>ServiceLabelNamespace (CD) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>DOTS&nbsp(0)</li><li><b>NUMERALS&nbsp(1)</b></li></ul></td></tr>
</table></details>

## Slat (B9)
<details><summary> Defines a motorized ventilation Slat(s).</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentSlatState (AA) :small_blue_diamond</b><ul><li> indicates current state of slats</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>FIXED&nbsp(0)</b></li><li>JAMMED&nbsp(1)</li><li>SWINGING&nbsp(2)</li></ul></td></tr>
<tr><td><b>SlatType (C0) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>HORIZONTAL&nbsp(0)</b></li><li>VERTICAL&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>SwingMode (B6) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><b>CurrentTiltAngle (C1) </b><ul><li> angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90)</li></ul></td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><b>TargetTiltAngle (C2) </b><ul><li></li></ul></td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
</table></details>

## SmokeSensor (87)
<details><summary> Defines a Smoke Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>SmokeDetected (76) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><b>Mute (11A) :small_blue_diamond</b><ul><li> not used</li></ul></td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>Volume (119) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## StatelessProgrammableSwitch (89)
<details><summary> Defines a "Stateless" Programmable Switch that can be used to trigger actions in the Home App.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>ProgrammableSwitchEvent (73) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>SINGLE_PRESS&nbsp(0)</b></li><li>DOUBLE_PRESS&nbsp(1)</li><li>LONG_PRESS&nbsp(2)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Switch (49)
<details><summary> Defines a generic Switch.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>On (25) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Television (D8)
<details><summary> Defines a TV.  Optional Linked Services: <b>InputSource</b> and <b>TelevisionSpeaker</b>.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>ConfiguredName (E3) </b><ul><li></li></ul></td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>ActiveIdentifier (E7) </b><ul><li> the Identifier of the current Input Source</li></ul></td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">255</td><td align="center">0</td></tr>
<tr><td><b>RemoteKey (E1) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">16</td><td align="center">0</td></tr>
<tr><td><b>PowerModeSelection (DF) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## TelevisionSpeaker (113)
<details><summary> Defines a Television Speaker that can be controlled via the Remote Control widget on an iPhone.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>VolumeControlType (E9) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td align="center">0</td></tr>
<tr><td><b>VolumeSelector (EA) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## TemperatureSensor (8A)
<details><summary> Defines a Temperature Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentTemperature (11) :small_blue_diamond</b><ul><li> current temperature measured in Celsius</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>StatusActive (75) </b><ul><li></li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">1</td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusTampered (7A) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusLowBattery (79) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Thermostat (4A)
<details><summary> Defines a Thermostat used to control a furnace, air conditioner, or both.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentHeatingCoolingState (F) :small_blue_diamond</b><ul><li> indicates whether appliance is currently heating, cooling, or just idle</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>IDLE&nbsp(0)</b></li><li>HEATING&nbsp(1)</li><li>COOLING&nbsp(2)</li></ul></td></tr>
<tr><td><b>TargetHeatingCoolingState (33) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>HEAT&nbsp(1)</li><li>COOL&nbsp(2)</li><li>AUTO&nbsp(3)</li></ul></td></tr>
<tr><td><b>CurrentTemperature (11) :small_blue_diamond</b><ul><li> current temperature measured in Celsius</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>TargetTemperature (35) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">10</td><td align="center">38</td><td align="center">16</td></tr>
<tr><td><b>TemperatureDisplayUnits (36) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CELSIUS&nbsp(0)</b></li><li>FAHRENHEIT&nbsp(1)</li></ul></td></tr>
<tr><td><b>CoolingThresholdTemperature (D) </b><ul><li> cooling turns on when temperature (in Celsius) rises above this threshold</li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td align="center">10</td></tr>
<tr><td><b>CurrentRelativeHumidity (10) </b><ul><li>current humidity measured as a percentage</li></ul></td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>HeatingThresholdTemperature (12) </b><ul><li> heating turns on when temperature (in Celsius) falls below this threshold</li></ul></td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td align="center">16</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>TargetRelativeHumidity (34) </b><ul><li></li></ul></td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## Valve (D0)
<details><summary> Defines an electronic Valve.  Can be used standalone or as a Linked Service in conjunction with the <b>Faucet</b> and <b>IrrigationSystem</b> Services.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>Active (B0) :small_blue_diamond</b><ul><li> indicates if the Service is active/on</li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><b>InUse (D2) :small_blue_diamond</b><ul><li> if Service is set to active, this indictes whether it is currently in use</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_IN_USE&nbsp(0)</b></li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><b>ValveType (D5) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td align="center">0</td></tr>
<tr><td><b>SetDuration (D3) </b><ul><li></li></ul></td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><b>RemainingDuration (D4) </b><ul><li></li></ul></td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><b>IsConfigured (D6) </b><ul><li> indicates if a predefined Service has been configured</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_CONFIGURED&nbsp(0)</b></li><li>CONFIGURED&nbsp(1)</li></ul></td></tr>
<tr><td><b>StatusFault (77) </b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Window (8B)
<details><summary> Defines a motorized Window.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>CurrentPosition (6D) :small_blue_diamond</b><ul><li> current position (as a percentage) from fully closed (0) to full open (100)</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>TargetPosition (7C) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>ObstructionDetected (24) </b><ul><li> indicates if obstruction is detected</li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

## WindowCovering (8C)
<details><summary> Defines a motorized Window Shade, Screen, Awning, etc.</summary><br><table>
<tr><th>Characteristic</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants/Defaults</th></tr>
<tr><td><b>TargetPosition (7C) :small_blue_diamond</b><ul><li></li></ul></td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>CurrentPosition (6D) :small_blue_diamond</b><ul><li> current position (as a percentage) from fully closed (0) to full open (100)</li></ul></td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><b>Name (23) </b><ul><li> default name of a Service used <i>only</i> during initial pairing</li></ul></td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><b>CurrentHorizontalTiltAngle (6C) </b><ul><li> current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90) </li></ul></td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><b>TargetHorizontalTiltAngle (7B) </b><ul><li></li></ul></td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><b>CurrentVerticalTiltAngle (6E) </b><ul><li> current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)</li></ul></td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><b>TargetVerticalTiltAngle (7D) </b><ul><li></li></ul></td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><b>ObstructionDetected (24) </b><ul><li> indicates if obstruction is detected</li></ul></td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

