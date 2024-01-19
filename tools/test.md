<details><summary>AccessoryInformation (3E)</summary><br><table>
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

<details><summary>AirPurifier (BB)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>CurrentAirPurifierState *</td><td align="center">A9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>PURIFYING (2)</li></ul></td></tr>
<tr><td>TargetAirPurifierState *</td><td align="center">A8</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL (0)</li><li>AUTO (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td></tr>
</table><br></details>

<details><summary>AirQualitySensor (8D)</summary><br><table>
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

<details><summary>BatteryService (96)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>BatteryLevel *</td><td align="center">68</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ChargingState *</td><td align="center">8F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NOT_CHARGING (0)</li><li>CHARGING (1)</li><li>NOT_CHARGEABLE (2)</li></ul></td></tr>
<tr><td>StatusLowBattery *</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>CarbonDioxideSensor (97)</summary><br><table>
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

<details><summary>CarbonMonoxideSensor (7F)</summary><br><table>
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

<details><summary>ContactSensor (80)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ContactSensorState *</td><td align="center">6A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>DETECTED (0)</li><li>NOT_DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>Door (81)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentPosition *</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TargetPosition *</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>PositionState *</td><td align="center">72</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>GOING_TO_MINIMUM (0)</li><li>GOING_TO_MAXIMUM (1)</li><li>STOPPED (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HoldPosition</td><td align="center">6F</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>Doorbell (121)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Brightness</td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>Fan (B7)</summary><br><table>
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

<details><summary>Faucet (D7)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>FilterMaintenance (BA)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>FilterChangeIndication *</td><td align="center">AC</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_CHANGE_NEEDED (0)</li><li>CHANGE_NEEDED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>FilterLifeLevel</td><td align="center">AB</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ResetFilterIndication</td><td align="center">AD</td><td align="center">uint8</td><td align="center">PW</td><td align="center">1</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>GarageDoorOpener (41)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentDoorState *</td><td align="center">E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li><li>OPENING (2)</li><li>CLOSING (3)</li><li>STOPPED (4)</li></ul></td></tr>
<tr><td>TargetDoorState *</td><td align="center">32</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li></ul></td></tr>
<tr><td>ObstructionDetected *</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>LockCurrentState</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td></tr>
<tr><td>LockTargetState</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>HAPProtocolInformation (A2)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Version *</td><td align="center">37</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>HeaterCooler (BC)</summary><br><table>
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

<details><summary>HumidifierDehumidifier (BD)</summary><br><table>
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

<details><summary>HumiditySensor (82)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentRelativeHumidity *</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>InputSource (D9)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>IsConfigured</td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_CONFIGURED (0)</li><li>CONFIGURED (1)</li></ul></td></tr>
<tr><td>Identifier *</td><td align="center">E6</td><td align="center">uint32</td><td align="center">PR</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td></tr>
<tr><td>CurrentVisibilityState</td><td align="center">135</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>TargetVisibilityState</td><td align="center">134</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>IrrigationSystem (CF)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>ProgramMode *</td><td align="center">D1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NONE (0)</li><li>SCHEDULED (1)</li><li>SCHEDULE_OVERRIDEN (2)</li></ul></td></tr>
<tr><td>InUse *</td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td></tr>
<tr><td>RemainingDuration</td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
</table><br></details>

<details><summary>LeakSensor (83)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>LeakDetected *</td><td align="center">70</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>LightBulb (43)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Brightness</td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Hue</td><td align="center">13</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">360</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Saturation</td><td align="center">2F</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>ColorTemperature</td><td align="center">CE</td><td align="center">uint32</td><td align="center">PR+PW+EV</td><td align="center">140</td><td align="center">500</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>LightSensor (84)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentAmbientLightLevel *</td><td align="center">6B</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0.0001</td><td align="center">100000</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>LockMechanism (45)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>LockCurrentState *</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td></tr>
<tr><td>LockTargetState *</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>Microphone (112)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Mute *</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>MotionSensor (85)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>MotionDetected *</td><td align="center">22</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>OccupancySensor (86)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>OccupancyDetected *</td><td align="center">71</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>Outlet (47)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>OutletInUse *</td><td align="center">26</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>SecuritySystem (7E)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>SecuritySystemCurrentState *</td><td align="center">66</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>ARMED_STAY (0)</li><li>ARMED_AWAY (1)</li><li>ARMED_NIGHT (2)</li><li>DISARMED (3)</li><li>ALARM_TRIGGERED (4)</li></ul></td></tr>
<tr><td>SecuritySystemTargetState *</td><td align="center">67</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>ARM_STAY (0)</li><li>ARM_AWAY (1)</li><li>ARM_NIGHT (2)</li><li>DISARM (3)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>SecuritySystemAlarmType</td><td align="center">8E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>KNOWN (0)</li><li>UNKNOWN (1)</li></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
</table><br></details>

<details><summary>ServiceLabel (CC)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ServiceLabelNamespace *</td><td align="center">CD</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>DOTS (0)</li><li>NUMERALS (1)</li></ul></td></tr>
</table><br></details>

<details><summary>Slat (B9)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentSlatState *</td><td align="center">AA</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>FIXED (0)</li><li>JAMMED (1)</li><li>SWINGING (2)</li></ul></td></tr>
<tr><td>SlatType *</td><td align="center">C0</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>HORIZONTAL (0)</li><li>VERTICAL (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td></tr>
<tr><td>CurrentTiltAngle</td><td align="center">C1</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
<tr><td>TargetTiltAngle</td><td align="center">C2</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>SmokeSensor (87)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>SmokeDetected *</td><td align="center">76</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>Speaker (113)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Mute *</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>StatelessProgrammableSwitch (89)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>ServiceLabelIndex</td><td align="center">CB</td><td align="center">uint8</td><td align="center">PR</td><td align="center">1</td><td align="center">255</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>Switch (49)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>On *</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>Television (D8)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>Active *</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>ActiveIdentifier</td><td align="center">E7</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td></tr>
<tr><td>RemoteKey</td><td align="center">E1</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">16</td><td><ul></ul></td></tr>
<tr><td>PowerModeSelection</td><td align="center">DF</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>TelevisionSpeaker (113)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>VolumeControlType *</td><td align="center">E9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul></ul></td></tr>
<tr><td>VolumeSelector *</td><td align="center">EA</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>TemperatureSensor (8A)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentTemperature *</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td></tr>
</table><br></details>

<details><summary>Thermostat (4A)</summary><br><table>
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

<details><summary>Valve (D0)</summary><br><table>
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

<details><summary>Window (8B)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th></tr>
<tr><td>CurrentPosition *</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>TargetPosition *</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td></tr>
<tr><td>PositionState *</td><td align="center">72</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>GOING_TO_MINIMUM (0)</li><li>GOING_TO_MAXIMUM (1)</li><li>STOPPED (2)</li></ul></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td></tr>
<tr><td>HoldPosition</td><td align="center">6F</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td></tr>
</table><br></details>

<details><summary>WindowCovering (8C)</summary><br><table>
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

