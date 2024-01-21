## AccessoryInformation (3E)
<details><summary> Required Identification Information.  For each Accessory in a HomeSpan device this <i>must</i> be included as the first Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Identify</b></summary> the Home App set this to RUN_ID when it wants the device to run its identification routine</details></td><td align="center">14</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>IDLE&nbsp(0)</li><li>RUN_ID&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>FirmwareRevision</summary> must be in form x[.y[.z]] - informational only</details></td><td align="center">52</td><td align="center">string</td><td align="center">PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
<tr><td><details><summary>Manufacturer</summary> any string - informational only</details></td><td align="center">20</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HomeSpan"</td></tr>
<tr><td><details><summary>Model</summary> any string - informational only</details></td><td align="center">21</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HomeSpan-ESP32"</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>SerialNumber</summary></details></td><td align="center">30</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"HS-12345"</td></tr>
<tr><td><details><summary>HardwareRevision</summary> must be in form x[.y[.z]] - informational only</details></td><td align="center">53</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
</table></details>

## AirPurifier (BB)
<details><summary> Defines a basic Air Purifier with an optional fan.  Optional Linked Services: <b>FilterMaintenance</b>, <b>AirQualitySensor</b>, <b>Fan</b>, and <b>Slat</b></summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>CurrentAirPurifierState</b></summary> indicates current state of air purification</details></td><td align="center">A9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>PURIFYING&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary><b>TargetAirPurifierState</b></summary></details></td><td align="center">A8</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL&nbsp(0)</li><li><b>AUTO&nbsp(1)</b></li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>RotationSpeed</summary></details></td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>SwingMode</summary></details></td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>LockPhysicalControls</summary> indicates if local control lock is enabled</details></td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## AirQualitySensor (8D)
<details><summary> Defines an Air Quality Sensor. </summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>AirQuality</b></summary> a subjective description</details></td><td align="center">95</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">5</td><td><ul><li><b>UNKNOWN&nbsp(0)</b></li><li>EXCELLENT&nbsp(1)</li><li>GOOD&nbsp(2)</li><li>FAIR&nbsp(3)</li><li>INFERIOR&nbsp(4)</li><li>POOR&nbsp(5)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>OzoneDensity</summary> measured in &micro;g/m<sup>3</sup></details></td><td align="center">C3</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>NitrogenDioxideDensity</summary> measured in &micro;g/m<sup>3</sup></details></td><td align="center">C4</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>SulphurDioxideDensity</summary></details></td><td align="center">C5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>PM25Density</summary> 2.5-micron particulate density, measured in &micro;g/m<sup>3</sup></details></td><td align="center">C6</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>PM10Density</summary> 10-micron particulate density, measured in &micro;g/m<sup>3</sup></details></td><td align="center">C7</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>VOCDensity</summary></details></td><td align="center">C8</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td align="center">0</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## BatteryService (96)
<details><summary> Defines a standalone Battery Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>BatteryLevel</b></summary> measured as a percentage</details></td><td align="center">68</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>ChargingState</b></summary> indicates state of battery charging</details></td><td align="center">8F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>NOT_CHARGING&nbsp(0)</b></li><li>CHARGING&nbsp(1)</li><li>NOT_CHARGEABLE&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary><b>StatusLowBattery</b></summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## CarbonDioxideSensor (97)
<details><summary> Defines a Carbon Dioxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CarbonDioxideDetected</b></summary> indicates if abnormal level is detected</details></td><td align="center">92</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NORMAL&nbsp(0)</b></li><li>ABNORMAL&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>CarbonDioxideLevel</summary> measured on parts per million (ppm)</details></td><td align="center">93</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td align="center">0</td></tr>
<tr><td><details><summary>CarbonDioxidePeakLevel</summary> measured in parts per million (ppm)</details></td><td align="center">94</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td align="center">0</td></tr>
</table></details>

## CarbonMonoxideSensor (7F)
<details><summary> Defines a Carbon Monoxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CarbonMonoxideDetected</b></summary> indicates if abnormal level is detected</details></td><td align="center">69</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NORMAL&nbsp(0)</b></li><li>ABNORMAL&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>CarbonMonoxideLevel</summary> measured in parts per million (ppm)</details></td><td align="center">90</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>CarbonMonoxidePeakLevel</summary> measured in parts per million (ppm)</details></td><td align="center">91</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## ContactSensor (80)
<details><summary> Defines a Contact Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>ContactSensorState</b></summary> indictates if contact is detected (i.e. closed)</details></td><td align="center">6A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>DETECTED&nbsp(0)</li><li><b>NOT_DETECTED&nbsp(1)</b></li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Door (81)
<details><summary> Defines a motorized Door.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentPosition</b></summary> current position (as a percentage) from fully closed (0) to full open (100)</details></td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>TargetPosition</b></summary></details></td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>ObstructionDetected</summary> indicates if obstruction is detected</details></td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED&nbsp(0)</li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

## Doorbell (121)
<details><summary> Defines a Doorbell.  Can be used on a standalone basis or in conjunction with a <b>LockMechanism</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>ProgrammableSwitchEvent</b></summary></details></td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>SINGLE_PRESS&nbsp(0)</b></li><li>DOUBLE_PRESS&nbsp(1)</li><li>LONG_PRESS&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Fan (B7)
<details><summary> Defines a Fan.  Can be used in conjunction with a <b>LightBulb</b> Service to create a Lighted Ceiling Fan.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>CurrentFanState</summary> indicates current state of a fan</details></td><td align="center">AF</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>BLOWING&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary>TargetFanState</summary></details></td><td align="center">BF</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL&nbsp(0)</li><li><b>AUTO&nbsp(1)</b></li></ul></td></tr>
<tr><td><details><summary>RotationDirection</summary></details></td><td align="center">28</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CLOCKWISE&nbsp(0)</b></li><li>COUNTERCLOCKWISE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>RotationSpeed</summary></details></td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>SwingMode</summary></details></td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>LockPhysicalControls</summary> indicates if local control lock is enabled</details></td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## Faucet (D7)
<details><summary> Defines the master control for a multi-Valve appliance.  Linked Services: <b>Valve</b> (at least one <i>required</i>), and <b>HeaterCooler</b> (optional).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## FilterMaintenance (BA)
<details><summary> Defines a Filter Maintainence check.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>FilterChangeIndication</b></summary> indicates state of filter</details></td><td align="center">AC</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_CHANGE_NEEDED&nbsp(0)</b></li><li>CHANGE_NEEDED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>FilterLifeLevel</summary> measures as a percentage of remaining life</details></td><td align="center">AB</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>ResetFilterIndication</summary></details></td><td align="center">AD</td><td align="center">uint8</td><td align="center">PW</td><td align="center">1</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## GarageDoorOpener (41)
<details><summary> Defines a motorized Garage Door Opener.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentDoorState</b></summary> indicates current state of a door</details></td><td align="center">E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>OPEN&nbsp(0)</li><li><b>CLOSED&nbsp(1)</b></li><li>OPENING&nbsp(2)</li><li>CLOSING&nbsp(3)</li><li>STOPPED&nbsp(4)</li></ul></td></tr>
<tr><td><details><summary><b>TargetDoorState</b></summary></details></td><td align="center">32</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OPEN&nbsp(0)</li><li><b>CLOSED&nbsp(1)</b></li></ul></td></tr>
<tr><td><details><summary><b>ObstructionDetected</b></summary> indicates if obstruction is detected</details></td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED&nbsp(0)</li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>LockCurrentState</summary> indictates state of a lock</details></td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>UNLOCKED&nbsp(0)</b></li><li>LOCKED&nbsp(1)</li><li>JAMMED&nbsp(2)</li><li>UNKNOWN&nbsp(3)</li></ul></td></tr>
<tr><td><details><summary>LockTargetState</summary> indicates desired state of lock</details></td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>UNLOCK&nbsp(0)</b></li><li>LOCK&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary><b>Version</b></summary></details></td><td align="center">37</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"1.0.0"</td></tr>
</table></details>

## HeaterCooler (BC)
<details><summary> Defines a standalone Heater, Cooler, or combined Heater/Cooler.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>CurrentTemperature</b></summary> current temperature measured in Celsius</details></td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>CurrentHeaterCoolerState</b></summary> indicates whether appliance is currently heating, cooling, idle, or off</details></td><td align="center">B1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>HEATING&nbsp(2)</li><li>COOLING&nbsp(3)</li></ul></td></tr>
<tr><td><details><summary><b>TargetHeaterCoolerState</b></summary></details></td><td align="center">B2</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>AUTO&nbsp(0)</b></li><li>HEAT&nbsp(1)</li><li>COOL&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>RotationSpeed</summary></details></td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>TemperatureDisplayUnits</summary></details></td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CELSIUS&nbsp(0)</b></li><li>FAHRENHEIT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>SwingMode</summary></details></td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>CoolingThresholdTemperature</summary> cooling turns on when temperature (in Celsius) rises above this threshold</details></td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td align="center">10</td></tr>
<tr><td><details><summary>HeatingThresholdTemperature</summary> heating turns on when temperature (in Celsius) falls below this threshold</details></td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td align="center">16</td></tr>
<tr><td><details><summary>LockPhysicalControls</summary> indicates if local control lock is enabled</details></td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## HumidifierDehumidifier (BD)
<details><summary> Defines a Humidifer, Dehumidifier, or combined Humidifer/Dehumidifier.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>CurrentRelativeHumidity</b></summary>current humidity measured as a percentage</details></td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>CurrentHumidifierDehumidifierState</b></summary> indicates current state of humidifier/dehumidifer</details></td><td align="center">B3</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE&nbsp(0)</li><li><b>IDLE&nbsp(1)</b></li><li>HUMIDIFYING&nbsp(2)</li><li>DEHUMIDIFYING&nbsp(3)</li></ul></td></tr>
<tr><td><details><summary><b>TargetHumidifierDehumidifierState</b></summary></details></td><td align="center">B4</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>AUTO&nbsp(0)</b></li><li>HUMIDIFY&nbsp(1)</li><li>DEHUMIDIFY&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>RelativeHumidityDehumidifierThreshold</summary></details></td><td align="center">C9</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">50</td></tr>
<tr><td><details><summary>RelativeHumidityHumidifierThreshold</summary></details></td><td align="center">CA</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">50</td></tr>
<tr><td><details><summary>RotationSpeed</summary></details></td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>SwingMode</summary></details></td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>WaterLevel</summary></details></td><td align="center">B5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>LockPhysicalControls</summary> indicates if local control lock is enabled</details></td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CONTROL_LOCK_DISABLED&nbsp(0)</b></li><li>CONTROL_LOCK_ENABLED&nbsp(1)</li></ul></td></tr>
</table></details>

## HumiditySensor (82)
<details><summary> Defines a Humidity Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentRelativeHumidity</b></summary>current humidity measured as a percentage</details></td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## InputSource (D9)
<details><summary> Defines an Input Source for a TV.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary>ConfiguredName</summary></details></td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>IsConfigured</summary> indicates if a predefined Service has been configured</details></td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_CONFIGURED&nbsp(0)</b></li><li>CONFIGURED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>Identifier</b></summary></details></td><td align="center">E6</td><td align="center">uint32</td><td align="center">PR</td><td align="center">0</td><td align="center">255</td><td align="center">0</td></tr>
<tr><td><details><summary>CurrentVisibilityState</summary></details></td><td align="center">135</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
<tr><td><details><summary>TargetVisibilityState</summary></details></td><td align="center">134</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## IrrigationSystem (CF)
<details><summary> Defines an Irrigation System.  Linked Services: <b>Valve</b> Service (at least one <i>required</i>).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>ProgramMode</b></summary></details></td><td align="center">D1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>NONE&nbsp(0)</b></li><li>SCHEDULED&nbsp(1)</li><li>SCHEDULE_OVERRIDEN&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary><b>InUse</b></summary> if Service is set to active, this indictes whether it is currently in use</details></td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_IN_USE&nbsp(0)</b></li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>RemainingDuration</summary></details></td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
</table></details>

## LeakSensor (83)
<details><summary> Defines a Leak Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>LeakDetected</b></summary> indictates if a leak is detected</details></td><td align="center">70</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## LightBulb (43)
<details><summary> Defines any type of Light.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>On</b></summary> indicates if the Service is active/on</details></td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF&nbsp(0)</li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Brightness</summary> measured as a percentage</details></td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Hue</summary> color (in degrees) from red (0) to green (120) to blue (240) and back to red (360)</details></td><td align="center">13</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">360</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>Saturation</summary></details></td><td align="center">2F</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>ColorTemperature</summary> measured in inverse megaKelvin (= 1,000,000 / Kelvin)</details></td><td align="center">CE</td><td align="center">uint32</td><td align="center">PR+PW+EV</td><td align="center">140</td><td align="center">500</td><td align="center">200</td></tr>
</table></details>

## LightSensor (84)
<details><summary> Defines a Light Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentAmbientLightLevel</b></summary> measured in Lux (lumens/m<sup>2</sup></details></td><td align="center">6B</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0.0001</td><td align="center">100000</td><td align="center">1</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## LockMechanism (45)
<details><summary> Defines an electronic Lock.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>LockCurrentState</b></summary> indictates state of a lock</details></td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>UNLOCKED&nbsp(0)</b></li><li>LOCKED&nbsp(1)</li><li>JAMMED&nbsp(2)</li><li>UNKNOWN&nbsp(3)</li></ul></td></tr>
<tr><td><details><summary><b>LockTargetState</b></summary> indicates desired state of lock</details></td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>UNLOCK&nbsp(0)</b></li><li>LOCK&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary><b>Mute</b></summary> not used</details></td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF&nbsp(0)</li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>Volume</summary></details></td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## MotionSensor (85)
<details><summary> Defines a Motion Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>MotionDetected</b></summary> indicates if motion is detected</details></td><td align="center">22</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED&nbsp(0)</li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## OccupancySensor (86)
<details><summary> Defines and Occupancy Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>OccupancyDetected</b></summary> indicates if occupanccy is detected</details></td><td align="center">71</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Outlet (47)
<details><summary> Defines an controllable Outlet used to power any light or appliance.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>On</b></summary> indicates if the Service is active/on</details></td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF&nbsp(0)</li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>OutletInUse</b></summary> indicates if an appliance or light is plugged into the outlet, regardless of whether on or off </details></td><td align="center">26</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE&nbsp(0)</li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## SecuritySystem (7E)
<details><summary> Defines a Security System.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>SecuritySystemCurrentState</b></summary></details></td><td align="center">66</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>ARMED_STAY&nbsp(0)</li><li>ARMED_AWAY&nbsp(1)</li><li>ARMED_NIGHT&nbsp(2)</li><li><b>DISARMED&nbsp(3)</b></li><li>ALARM_TRIGGERED&nbsp(4)</li></ul></td></tr>
<tr><td><details><summary><b>SecuritySystemTargetState</b></summary></details></td><td align="center">67</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>ARM_STAY&nbsp(0)</li><li>ARM_AWAY&nbsp(1)</li><li>ARM_NIGHT&nbsp(2)</li><li><b>DISARM&nbsp(3)</b></li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>SecuritySystemAlarmType</summary></details></td><td align="center">8E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>KNOWN&nbsp(0)</b></li><li>UNKNOWN&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>ServiceLabelNamespace</b></summary></details></td><td align="center">CD</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>DOTS&nbsp(0)</li><li><b>NUMERALS&nbsp(1)</b></li></ul></td></tr>
</table></details>

## Slat (B9)
<details><summary> Defines a motorized ventilation Slat(s).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentSlatState</b></summary> indicates current state of slats</details></td><td align="center">AA</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>FIXED&nbsp(0)</b></li><li>JAMMED&nbsp(1)</li><li>SWINGING&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary><b>SlatType</b></summary></details></td><td align="center">C0</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>HORIZONTAL&nbsp(0)</b></li><li>VERTICAL&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>SwingMode</summary></details></td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>SWING_DISABLED&nbsp(0)</b></li><li>SWING_ENABLED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>CurrentTiltAngle</summary> angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90)</details></td><td align="center">C1</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><details><summary>TargetTiltAngle</summary></details></td><td align="center">C2</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
</table></details>

## SmokeSensor (87)
<details><summary> Defines a Smoke Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>SmokeDetected</b></summary></details></td><td align="center">76</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_DETECTED&nbsp(0)</b></li><li>DETECTED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>Mute</b></summary> not used</details></td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF&nbsp(0)</li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>Volume</summary></details></td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## StatelessProgrammableSwitch (89)
<details><summary> Defines a "Stateless" Programmable Switch that can be used to trigger actions in the Home App.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>ProgrammableSwitchEvent</b></summary></details></td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>SINGLE_PRESS&nbsp(0)</b></li><li>DOUBLE_PRESS&nbsp(1)</li><li>LONG_PRESS&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Switch (49)
<details><summary> Defines a generic Switch.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>On</b></summary> indicates if the Service is active/on</details></td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF&nbsp(0)</li><li>ON&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Television (D8)
<details><summary> Defines a TV.  Optional Linked Services: <b>InputSource</b> and <b>TelevisionSpeaker</b>.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>ConfiguredName</summary></details></td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>ActiveIdentifier</summary> the Identifier of the current Input Source</details></td><td align="center">E7</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">255</td><td align="center">0</td></tr>
<tr><td><details><summary>RemoteKey</summary></details></td><td align="center">E1</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">16</td><td align="center">0</td></tr>
<tr><td><details><summary>PowerModeSelection</summary></details></td><td align="center">DF</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## TelevisionSpeaker (113)
<details><summary> Defines a Television Speaker that can be controlled via the Remote Control widget on an iPhone.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>VolumeControlType</b></summary></details></td><td align="center">E9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td align="center">0</td></tr>
<tr><td><details><summary><b>VolumeSelector</b></summary></details></td><td align="center">EA</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td align="center">0</td></tr>
</table></details>

## TemperatureSensor (8A)
<details><summary> Defines a Temperature Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentTemperature</b></summary> current temperature measured in Celsius</details></td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>StatusActive</summary></details></td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td align="center">true</td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusTampered</summary></details></td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_TAMPERED&nbsp(0)</b></li><li>TAMPERED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusLowBattery</summary></details></td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_LOW_BATTERY&nbsp(0)</b></li><li>LOW_BATTERY&nbsp(1)</li></ul></td></tr>
</table></details>

## Thermostat (4A)
<details><summary> Defines a Thermostat used to control a furnace, air conditioner, or both.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentHeatingCoolingState</b></summary> indicates whether appliance is currently heating, cooling, or just idle</details></td><td align="center">F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li><b>IDLE&nbsp(0)</b></li><li>HEATING&nbsp(1)</li><li>COOLING&nbsp(2)</li></ul></td></tr>
<tr><td><details><summary><b>TargetHeatingCoolingState</b></summary></details></td><td align="center">33</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li><b>OFF&nbsp(0)</b></li><li>HEAT&nbsp(1)</li><li>COOL&nbsp(2)</li><li>AUTO&nbsp(3)</li></ul></td></tr>
<tr><td><details><summary><b>CurrentTemperature</b></summary> current temperature measured in Celsius</details></td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>TargetTemperature</b></summary></details></td><td align="center">35</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">10</td><td align="center">38</td><td align="center">16</td></tr>
<tr><td><details><summary><b>TemperatureDisplayUnits</b></summary></details></td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>CELSIUS&nbsp(0)</b></li><li>FAHRENHEIT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>CoolingThresholdTemperature</summary> cooling turns on when temperature (in Celsius) rises above this threshold</details></td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td align="center">10</td></tr>
<tr><td><details><summary>CurrentRelativeHumidity</summary>current humidity measured as a percentage</details></td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>HeatingThresholdTemperature</summary> heating turns on when temperature (in Celsius) falls below this threshold</details></td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td align="center">16</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>TargetRelativeHumidity</summary></details></td><td align="center">34</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
</table></details>

## Valve (D0)
<details><summary> Defines an electronic Valve.  Can be used standalone or as a Linked Service in conjunction with the <b>Faucet</b> and <b>IrrigationSystem</b> Services.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>Active</b></summary> indicates if the Service is active/on</details></td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>INACTIVE&nbsp(0)</b></li><li>ACTIVE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>InUse</b></summary> if Service is set to active, this indictes whether it is currently in use</details></td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_IN_USE&nbsp(0)</b></li><li>IN_USE&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary><b>ValveType</b></summary></details></td><td align="center">D5</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td align="center">0</td></tr>
<tr><td><details><summary>SetDuration</summary></details></td><td align="center">D3</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><details><summary>RemainingDuration</summary></details></td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td align="center">60</td></tr>
<tr><td><details><summary>IsConfigured</summary> indicates if a predefined Service has been configured</details></td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NOT_CONFIGURED&nbsp(0)</b></li><li>CONFIGURED&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>StatusFault</summary></details></td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li><b>NO_FAULT&nbsp(0)</b></li><li>FAULT&nbsp(1)</li></ul></td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
</table></details>

## Window (8B)
<details><summary> Defines a motorized Window.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>CurrentPosition</b></summary> current position (as a percentage) from fully closed (0) to full open (100)</details></td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>TargetPosition</b></summary></details></td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>ObstructionDetected</summary> indicates if obstruction is detected</details></td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED&nbsp(0)</li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

## WindowCovering (8C)
<details><summary> Defines a motorized Window Shade, Screen, Awning, etc.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Defaults</th><th>Notes</th></tr>
<tr><td><details><summary><b>TargetPosition</b></summary></details></td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary><b>CurrentPosition</b></summary> current position (as a percentage) from fully closed (0) to full open (100)</details></td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td align="center">0</td></tr>
<tr><td><details><summary>Name</summary> default name of a Service used <i>only</i> during initial pairing</details></td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td align="center">"unnamed"</td></tr>
<tr><td><details><summary>CurrentHorizontalTiltAngle</summary> current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90) </details></td><td align="center">6C</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><details><summary>TargetHorizontalTiltAngle</summary></details></td><td align="center">7B</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><details><summary>CurrentVerticalTiltAngle</summary> current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)</details></td><td align="center">6E</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><details><summary>TargetVerticalTiltAngle</summary></details></td><td align="center">7D</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td align="center">0</td></tr>
<tr><td><details><summary>ObstructionDetected</summary> indicates if obstruction is detected</details></td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED&nbsp(0)</li><li>DETECTED&nbsp(1)</li></ul></td></tr>
</table></details>

