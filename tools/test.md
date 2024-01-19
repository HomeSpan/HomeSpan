## AccessoryInformation (3E)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Identify *</td><td align="center">14</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>FirmwareRevision</td><td align="center">52</td><td align="center">string</td><td align="center">PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Manufacturer</td><td align="center">20</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Model</td><td align="center">21</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>SerialNumber</td><td align="center">30</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HardwareRevision</td><td align="center">53</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>AccessoryFlags</td><td align="center">A6</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">1</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## AirPurifier (BB)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>CurrentAirPurifierState *</td><td align="center">A9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>PURIFYING (2)</li></ul></td></tr>
<tr><td>TargetAirPurifierState *</td><td align="center">A8</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL (0)</li><li>AUTO (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td></tr>
</table><br></details>

## AirQualitySensor (8D)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>AirQuality *</td><td align="center">95</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">5</td><td><ul><li>UNKNOWN (0)</li><li>EXCELLENT (1)</li><li>GOOD (2)</li><li>FAIR (3)</li><li>INFERIOR (4)</li><li>POOR (5)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>OzoneDensity</td><td align="center">C3</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>NitrogenDioxideDensity</td><td align="center">C4</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>SulphurDioxideDensity</td><td align="center">C5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>PM25Density</td><td align="center">C6</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>PM10Density</td><td align="center">C7</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>VOCDensity</td><td align="center">C8</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## BatteryService (96)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>BatteryLevel *</td><td align="center">68</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ChargingState *</td><td align="center">8F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NOT_CHARGING (0)</li><li>CHARGING (1)</li><li>NOT_CHARGEABLE (2)</li></ul></td></tr>
<tr><td>StatusLowBattery *</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## CarbonDioxideSensor (97)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CarbonDioxideDetected *</td><td align="center">92</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NORMAL (0)</li><li>ABNORMAL (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
<tr><td>CarbonDioxideLevel</td><td align="center">93</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td><ul></ul></td></tr>
<tr><td>CarbonDioxidePeakLevel</td><td align="center">94</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td><ul></ul></td></tr>
</table><br></details>

## CarbonMonoxideSensor (7F)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CarbonMonoxideDetected *</td><td align="center">69</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NORMAL (0)</li><li>ABNORMAL (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
<tr><td>CarbonMonoxideLevel</td><td align="center">90</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>CarbonMonoxidePeakLevel</td><td align="center">91</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

## ContactSensor (80)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ContactSensorState *</td><td align="center">6A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>DETECTED (0)</li><li>NOT_DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## Door (81)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentPosition *</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TargetPosition *</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>PositionState *</td><td align="center">72</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>GOING_TO_MINIMUM (0)</li><li>GOING_TO_MAXIMUM (1)</li><li>STOPPED (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HoldPosition</td><td align="center">6F</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## Doorbell (121)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Brightness</td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

## Fan (B7)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>CurrentFanState</td><td align="center">AF</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>BLOWING (2)</li></ul></td></tr>
<tr><td>TargetFanState</td><td align="center">BF</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL (0)</li><li>AUTO (1)</li></ul></td></tr>
<tr><td>RotationDirection</td><td align="center">28</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CLOCKWISE (0)</li><li>COUNTERCLOCKWISE (1)</li></ul></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td></tr>
</table><br></details>

## Faucet (D7)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## FilterMaintenance (BA)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>FilterChangeIndication *</td><td align="center">AC</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_CHANGE_NEEDED (0)</li><li>CHANGE_NEEDED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>FilterLifeLevel</td><td align="center">AB</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ResetFilterIndication</td><td align="center">AD</td><td align="center">uint8</td><td align="center">PW</td><td align="center">1</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## GarageDoorOpener (41)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentDoorState *</td><td align="center">E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li><li>OPENING (2)</li><li>CLOSING (3)</li><li>STOPPED (4)</li></ul></td></tr>
<tr><td>TargetDoorState *</td><td align="center">32</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li></ul></td></tr>
<tr><td>ObstructionDetected *</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>LockCurrentState</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td></tr>
<tr><td>LockTargetState</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## HAPProtocolInformation (A2)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Version *</td><td align="center">37</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## HeaterCooler (BC)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>CurrentTemperature *</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>CurrentHeaterCoolerState *</td><td align="center">B1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>HEATING (2)</li><li>COOLING (3)</li></ul></td></tr>
<tr><td>TargetHeaterCoolerState *</td><td align="center">B2</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>AUTO (0)</li><li>HEAT (1)</li><li>COOL (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TemperatureDisplayUnits</td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CELSIUS (0)</li><li>FAHRENHEIT (1)</li></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>CoolingThresholdTemperature</td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td><ul></ul></td></tr>
<tr><td>HeatingThresholdTemperature</td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td><ul></ul></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td></tr>
</table><br></details>

## HumidifierDehumidifier (BD)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>CurrentRelativeHumidity *</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>CurrentHumidifierDehumidifierState *</td><td align="center">B3</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>HUMIDIFYING (2)</li><li>DEHUMIDIFYING (3)</li></ul></td></tr>
<tr><td>TargetHumidifierDehumidifierState *</td><td align="center">B4</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>AUTO (0)</li><li>HUMIDIFY (1)</li><li>DEHUMIDIFY (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>RelativeHumidityDehumidifierThreshold</td><td align="center">C9</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>RelativeHumidityHumidifierThreshold</td><td align="center">CA</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>WaterLevel</td><td align="center">B5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td></tr>
</table><br></details>

## HumiditySensor (82)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentRelativeHumidity *</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## InputSource (D9)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>IsConfigured</td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_CONFIGURED (0)</li><li>CONFIGURED (1)</li></ul></td></tr>
<tr><td>Identifier *</td><td align="center">E6</td><td align="center">uint32</td><td align="center">PR</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td></tr>
<tr><td>CurrentVisibilityState</td><td align="center">135</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>TargetVisibilityState</td><td align="center">134</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## IrrigationSystem (CF)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>ProgramMode *</td><td align="center">D1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NONE (0)</li><li>SCHEDULED (1)</li><li>SCHEDULE_OVERRIDEN (2)</li></ul></td></tr>
<tr><td>InUse *</td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td></tr>
<tr><td>RemainingDuration</td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
</table><br></details>

## LeakSensor (83)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>LeakDetected *</td><td align="center">70</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## LightBulb (43)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Brightness</td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Hue</td><td align="center">13</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">360</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Saturation</td><td align="center">2F</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ColorTemperature</td><td align="center">CE</td><td align="center">uint32</td><td align="center">PR+PW+EV</td><td align="center">140</td><td align="center">500</td><td><ul></ul></td></tr>
</table><br></details>

## LightSensor (84)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentAmbientLightLevel *</td><td align="center">6B</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0.0001</td><td align="center">100000</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## LockMechanism (45)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>LockCurrentState *</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td></tr>
<tr><td>LockTargetState *</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## Microphone (112)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Mute *</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

## MotionSensor (85)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>MotionDetected *</td><td align="center">22</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## OccupancySensor (86)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>OccupancyDetected *</td><td align="center">71</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## Outlet (47)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>OutletInUse *</td><td align="center">26</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## SecuritySystem (7E)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>SecuritySystemCurrentState *</td><td align="center">66</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>ARMED_STAY (0)</li><li>ARMED_AWAY (1)</li><li>ARMED_NIGHT (2)</li><li>DISARMED (3)</li><li>ALARM_TRIGGERED (4)</li></ul></td></tr>
<tr><td>SecuritySystemTargetState *</td><td align="center">67</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>ARM_STAY (0)</li><li>ARM_AWAY (1)</li><li>ARM_NIGHT (2)</li><li>DISARM (3)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>SecuritySystemAlarmType</td><td align="center">8E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>KNOWN (0)</li><li>UNKNOWN (1)</li></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
</table><br></details>

## ServiceLabel (CC)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ServiceLabelNamespace *</td><td align="center">CD</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>DOTS (0)</li><li>NUMERALS (1)</li></ul></td></tr>
</table><br></details>

## Slat (B9)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentSlatState *</td><td align="center">AA</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>FIXED (0)</li><li>JAMMED (1)</li><li>SWINGING (2)</li></ul></td></tr>
<tr><td>SlatType *</td><td align="center">C0</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>HORIZONTAL (0)</li><li>VERTICAL (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>CurrentTiltAngle</td><td align="center">C1</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>TargetTiltAngle</td><td align="center">C2</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
</table><br></details>

## SmokeSensor (87)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>SmokeDetected *</td><td align="center">76</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## Speaker (113)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Mute *</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

## StatelessProgrammableSwitch (89)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>ServiceLabelIndex</td><td align="center">CB</td><td align="center">uint8</td><td align="center">PR</td><td align="center">1</td><td align="center">255</td><td><ul></ul></td></tr>
</table><br></details>

## Switch (49)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## Television (D8)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>ActiveIdentifier</td><td align="center">E7</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td></tr>
<tr><td>RemoteKey</td><td align="center">E1</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">16</td><td><ul></ul></td></tr>
<tr><td>PowerModeSelection</td><td align="center">DF</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## TelevisionSpeaker (113)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>VolumeControlType *</td><td align="center">E9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul></ul></td></tr>
<tr><td>VolumeSelector *</td><td align="center">EA</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## TemperatureSensor (8A)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentTemperature *</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

## Thermostat (4A)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentHeatingCoolingState *</td><td align="center">F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>OFF (0)</li><li>HEATING (1)</li><li>COOLING (2)</li></ul></td></tr>
<tr><td>TargetHeatingCoolingState *</td><td align="center">33</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>OFF (0)</li><li>HEAT (1)</li><li>COOL (2)</li><li>AUTO (3)</li></ul></td></tr>
<tr><td>CurrentTemperature *</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TargetTemperature *</td><td align="center">35</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">10</td><td align="center">38</td><td><ul></ul></td></tr>
<tr><td>TemperatureDisplayUnits *</td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CELSIUS (0)</li><li>FAHRENHEIT (1)</li></ul></td></tr>
<tr><td>CoolingThresholdTemperature</td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td><ul></ul></td></tr>
<tr><td>CurrentRelativeHumidity</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>HeatingThresholdTemperature</td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>TargetRelativeHumidity</td><td align="center">34</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

## Valve (D0)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>InUse *</td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td></tr>
<tr><td>ValveType *</td><td align="center">D5</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul></ul></td></tr>
<tr><td>SetDuration</td><td align="center">D3</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td></tr>
<tr><td>RemainingDuration</td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td></tr>
<tr><td>IsConfigured</td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_CONFIGURED (0)</li><li>CONFIGURED (1)</li></ul></td></tr>
<tr><td>ServiceLabelIndex</td><td align="center">CB</td><td align="center">uint8</td><td align="center">PR</td><td align="center">1</td><td align="center">255</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

## Window (8B)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentPosition *</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TargetPosition *</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>PositionState *</td><td align="center">72</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>GOING_TO_MINIMUM (0)</li><li>GOING_TO_MAXIMUM (1)</li><li>STOPPED (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HoldPosition</td><td align="center">6F</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

## WindowCovering (8C)
<details><summary>Placeholder for description of Service</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>TargetPosition *</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>CurrentPosition *</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>PositionState *</td><td align="center">72</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>GOING_TO_MINIMUM (0)</li><li>GOING_TO_MAXIMUM (1)</li><li>STOPPED (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HoldPosition</td><td align="center">6F</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>CurrentHorizontalTiltAngle</td><td align="center">6C</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>TargetHorizontalTiltAngle</td><td align="center">7B</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>CurrentVerticalTiltAngle</td><td align="center">6E</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>TargetVerticalTiltAngle</td><td align="center">7D</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

