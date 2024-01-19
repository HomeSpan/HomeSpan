<details><summary>AccessoryInformation (3E)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Identify *</td><td align=>14</td><td align=>BOOL</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>FirmwareRevision</td><td align=>52</td><td align=>STRING</td><td align=>PR+EV</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Manufacturer</td><td align=>20</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Model</td><td align=>21</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>SerialNumber</td><td align=>30</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>HardwareRevision</td><td align=>53</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>AccessoryFlags</td><td align=>A6</td><td align=>UINT32</td><td align=>PR+EV</td><td align=>1</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>AirPurifier (BB)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>CurrentAirPurifierState *</td><td align=>A9</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>INACTIVE (0)<br>IDLE (1)<br>PURIFYING (2)</td></tr>
<tr><td>TargetAirPurifierState *</td><td align=>A8</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>MANUAL (0)<br>AUTO (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>RotationSpeed</td><td align=>29</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>SwingMode</td><td align=>B6</td><td align=>UINT8</td><td align=>PR+EV+PW</td><td align=>0</td><td align=>1</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</td></tr>
<tr><td>LockPhysicalControls</td><td align=>A7</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</td></tr>
</table><br></details>
<details><summary>AirQualitySensor (8D)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>AirQuality *</td><td align=>95</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>5</td><td>UNKNOWN (0)<br>EXCELLENT (1)<br>GOOD (2)<br>FAIR (3)<br>INFERIOR (4)<br>POOR (5)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>OzoneDensity</td><td align=>C3</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>NitrogenDioxideDensity</td><td align=>C4</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>SulphurDioxideDensity</td><td align=>C5</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>PM25Density</td><td align=>C6</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>PM10Density</td><td align=>C7</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>VOCDensity</td><td align=>C8</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>1000</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>BatteryService (96)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>BatteryLevel *</td><td align=>68</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>ChargingState *</td><td align=>8F</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>NOT_CHARGING (0)<br>CHARGING (1)<br>NOT_CHARGEABLE (2)</td></tr>
<tr><td>StatusLowBattery *</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>CarbonDioxideSensor (97)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CarbonDioxideDetected *</td><td align=>92</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NORMAL (0)<br>ABNORMAL (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
<tr><td>CarbonDioxideLevel</td><td align=>93</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100000</td><td></td></tr>
<tr><td>CarbonDioxidePeakLevel</td><td align=>94</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100000</td><td></td></tr>
</table><br></details>
<details><summary>CarbonMonoxideSensor (7F)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CarbonMonoxideDetected *</td><td align=>69</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NORMAL (0)<br>ABNORMAL (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
<tr><td>CarbonMonoxideLevel</td><td align=>90</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>CarbonMonoxidePeakLevel</td><td align=>91</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
</table><br></details>
<details><summary>ContactSensor (80)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>ContactSensorState *</td><td align=>6A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>DETECTED (0)<br>NOT_DETECTED (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>Door (81)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentPosition *</td><td align=>6D</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>TargetPosition *</td><td align=>7C</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>PositionState *</td><td align=>72</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>HoldPosition</td><td align=>6F</td><td align=>BOOL</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>ObstructionDetected</td><td align=>24</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>Doorbell (121)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align=>73</td><td align=>UINT8</td><td align=>PR+EV+NV</td><td align=>0</td><td align=>2</td><td>SINGLE_PRESS (0)<br>DOUBLE_PRESS (1)<br>LONG_PRESS (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Volume</td><td align=>119</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>Brightness</td><td align=>8</td><td align=>INT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
</table><br></details>
<details><summary>Fan (B7)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>CurrentFanState</td><td align=>AF</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>INACTIVE (0)<br>IDLE (1)<br>BLOWING (2)</td></tr>
<tr><td>TargetFanState</td><td align=>BF</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>MANUAL (0)<br>AUTO (1)</td></tr>
<tr><td>RotationDirection</td><td align=>28</td><td align=>INT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>1</td><td>CLOCKWISE (0)<br>COUNTERCLOCKWISE (1)</td></tr>
<tr><td>RotationSpeed</td><td align=>29</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>SwingMode</td><td align=>B6</td><td align=>UINT8</td><td align=>PR+EV+PW</td><td align=>0</td><td align=>1</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</td></tr>
<tr><td>LockPhysicalControls</td><td align=>A7</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</td></tr>
</table><br></details>
<details><summary>Faucet (D7)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>FilterMaintenance (BA)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>FilterChangeIndication *</td><td align=>AC</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_CHANGE_NEEDED (0)<br>CHANGE_NEEDED (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>FilterLifeLevel</td><td align=>AB</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>ResetFilterIndication</td><td align=>AD</td><td align=>UINT8</td><td align=>PW</td><td align=>1</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>GarageDoorOpener (41)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentDoorState *</td><td align=>E</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>4</td><td>OPEN (0)<br>CLOSED (1)<br>OPENING (2)<br>CLOSING (3)<br>STOPPED (4)</td></tr>
<tr><td>TargetDoorState *</td><td align=>32</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>OPEN (0)<br>CLOSED (1)</td></tr>
<tr><td>ObstructionDetected *</td><td align=>24</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>LockCurrentState</td><td align=>1D</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td>UNLOCKED (0)<br>LOCKED (1)<br>JAMMED (2)<br>UNKNOWN (3)</td></tr>
<tr><td>LockTargetState</td><td align=>1E</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>UNLOCK (0)<br>LOCK (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>HAPProtocolInformation (A2)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Version *</td><td align=>37</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>HeaterCooler (BC)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>CurrentTemperature *</td><td align=>11</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>CurrentHeaterCoolerState *</td><td align=>B1</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td>INACTIVE (0)<br>IDLE (1)<br>HEATING (2)<br>COOLING (3)</td></tr>
<tr><td>TargetHeaterCoolerState *</td><td align=>B2</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>2</td><td>AUTO (0)<br>HEAT (1)<br>COOL (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>RotationSpeed</td><td align=>29</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>TemperatureDisplayUnits</td><td align=>36</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CELSIUS (0)<br>FAHRENHEIT (1)</td></tr>
<tr><td>SwingMode</td><td align=>B6</td><td align=>UINT8</td><td align=>PR+EV+PW</td><td align=>0</td><td align=>1</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</td></tr>
<tr><td>CoolingThresholdTemperature</td><td align=>D</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>10</td><td align=>35</td><td></td></tr>
<tr><td>HeatingThresholdTemperature</td><td align=>12</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>25</td><td></td></tr>
<tr><td>LockPhysicalControls</td><td align=>A7</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</td></tr>
</table><br></details>
<details><summary>HumidifierDehumidifier (BD)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>CurrentRelativeHumidity *</td><td align=>10</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>CurrentHumidifierDehumidifierState *</td><td align=>B3</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td>INACTIVE (0)<br>IDLE (1)<br>HUMIDIFYING (2)<br>DEHUMIDIFYING (3)</td></tr>
<tr><td>TargetHumidifierDehumidifierState *</td><td align=>B4</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>2</td><td>AUTO (0)<br>HUMIDIFY (1)<br>DEHUMIDIFY (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>RelativeHumidityDehumidifierThreshold</td><td align=>C9</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>RelativeHumidityHumidifierThreshold</td><td align=>CA</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>RotationSpeed</td><td align=>29</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>SwingMode</td><td align=>B6</td><td align=>UINT8</td><td align=>PR+EV+PW</td><td align=>0</td><td align=>1</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</td></tr>
<tr><td>WaterLevel</td><td align=>B5</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>LockPhysicalControls</td><td align=>A7</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</td></tr>
</table><br></details>
<details><summary>HumiditySensor (82)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentRelativeHumidity *</td><td align=>10</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>InputSource (D9)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>ConfiguredName</td><td align=>E3</td><td align=>STRING</td><td align=>PW+PR+EV</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>IsConfigured</td><td align=>D6</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_CONFIGURED (0)<br>CONFIGURED (1)</td></tr>
<tr><td>Identifier *</td><td align=>E6</td><td align=>UINT32</td><td align=>PR</td><td align=>0</td><td align=>255</td><td></td></tr>
<tr><td>CurrentVisibilityState</td><td align=>135</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>TargetVisibilityState</td><td align=>134</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>IrrigationSystem (CF)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>ProgramMode *</td><td align=>D1</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>NONE (0)<br>SCHEDULED (1)<br>SCHEDULE_OVERRIDEN (2)</td></tr>
<tr><td>InUse *</td><td align=>D2</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_IN_USE (0)<br>IN_USE (1)</td></tr>
<tr><td>RemainingDuration</td><td align=>D4</td><td align=>UINT32</td><td align=>PR+EV</td><td align=>0</td><td align=>3600</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
</table><br></details>
<details><summary>LeakSensor (83)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>LeakDetected *</td><td align=>70</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_DETECTED (0)<br>DETECTED (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>LightBulb (43)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>On *</td><td align=>25</td><td align=>BOOL</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>Brightness</td><td align=>8</td><td align=>INT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>Hue</td><td align=>13</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>360</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Saturation</td><td align=>2F</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>ColorTemperature</td><td align=>CE</td><td align=>UINT32</td><td align=>PR+PW+EV</td><td align=>140</td><td align=>500</td><td></td></tr>
</table><br></details>
<details><summary>LightSensor (84)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentAmbientLightLevel *</td><td align=>6B</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0.0001</td><td align=>100000</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>LockMechanism (45)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>LockCurrentState *</td><td align=>1D</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td>UNLOCKED (0)<br>LOCKED (1)<br>JAMMED (2)<br>UNKNOWN (3)</td></tr>
<tr><td>LockTargetState *</td><td align=>1E</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>UNLOCK (0)<br>LOCK (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>Microphone (112)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Mute *</td><td align=>11A</td><td align=>BOOL</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>OFF (0)<br>ON (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Volume</td><td align=>119</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
</table><br></details>
<details><summary>MotionSensor (85)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>MotionDetected *</td><td align=>22</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>OccupancySensor (86)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>OccupancyDetected *</td><td align=>71</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_DETECTED (0)<br>DETECTED (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>Outlet (47)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>On *</td><td align=>25</td><td align=>BOOL</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>OutletInUse *</td><td align=>26</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>SecuritySystem (7E)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>SecuritySystemCurrentState *</td><td align=>66</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>4</td><td>ARMED_STAY (0)<br>ARMED_AWAY (1)<br>ARMED_NIGHT (2)<br>DISARMED (3)<br>ALARM_TRIGGERED (4)</td></tr>
<tr><td>SecuritySystemTargetState *</td><td align=>67</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>3</td><td>ARM_STAY (0)<br>ARM_AWAY (1)<br>ARM_NIGHT (2)<br>DISARM (3)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>SecuritySystemAlarmType</td><td align=>8E</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>KNOWN (0)<br>UNKNOWN (1)</td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
</table><br></details>
<details><summary>ServiceLabel (CC)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>ServiceLabelNamespace *</td><td align=>CD</td><td align=>UINT8</td><td align=>PR</td><td align=>0</td><td align=>1</td><td>DOTS (0)<br>NUMERALS (1)</td></tr>
</table><br></details>
<details><summary>Slat (B9)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentSlatState *</td><td align=>AA</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>FIXED (0)<br>JAMMED (1)<br>SWINGING (2)</td></tr>
<tr><td>SlatType *</td><td align=>C0</td><td align=>UINT8</td><td align=>PR</td><td align=>0</td><td align=>1</td><td>HORIZONTAL (0)<br>VERTICAL (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>SwingMode</td><td align=>B6</td><td align=>UINT8</td><td align=>PR+EV+PW</td><td align=>0</td><td align=>1</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</td></tr>
<tr><td>CurrentTiltAngle</td><td align=>C1</td><td align=>INT</td><td align=>PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
<tr><td>TargetTiltAngle</td><td align=>C2</td><td align=>INT</td><td align=>PW+PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
</table><br></details>
<details><summary>SmokeSensor (87)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>SmokeDetected *</td><td align=>76</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_DETECTED (0)<br>DETECTED (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>Speaker (113)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Mute *</td><td align=>11A</td><td align=>BOOL</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>OFF (0)<br>ON (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>Volume</td><td align=>119</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
</table><br></details>
<details><summary>StatelessProgrammableSwitch (89)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td align=>73</td><td align=>UINT8</td><td align=>PR+EV+NV</td><td align=>0</td><td align=>2</td><td>SINGLE_PRESS (0)<br>DOUBLE_PRESS (1)<br>LONG_PRESS (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>ServiceLabelIndex</td><td align=>CB</td><td align=>UINT8</td><td align=>PR</td><td align=>1</td><td align=>255</td><td></td></tr>
</table><br></details>
<details><summary>Switch (49)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>On *</td><td align=>25</td><td align=>BOOL</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>Television (D8)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>ConfiguredName</td><td align=>E3</td><td align=>STRING</td><td align=>PW+PR+EV</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>ActiveIdentifier</td><td align=>E7</td><td align=>UINT32</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>255</td><td></td></tr>
<tr><td>RemoteKey</td><td align=>E1</td><td align=>UINT8</td><td align=>PW</td><td align=>0</td><td align=>16</td><td></td></tr>
<tr><td>PowerModeSelection</td><td align=>DF</td><td align=>UINT8</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>TelevisionSpeaker (113)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>VolumeControlType *</td><td align=>E9</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td></td></tr>
<tr><td>VolumeSelector *</td><td align=>EA</td><td align=>UINT8</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>TemperatureSensor (8A)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentTemperature *</td><td align=>11</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>StatusActive</td><td align=>75</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>StatusTampered</td><td align=>7A</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</td></tr>
<tr><td>StatusLowBattery</td><td align=>79</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</td></tr>
</table><br></details>
<details><summary>Thermostat (4A)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentHeatingCoolingState *</td><td align=>F</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>OFF (0)<br>HEATING (1)<br>COOLING (2)</td></tr>
<tr><td>TargetHeatingCoolingState *</td><td align=>33</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>3</td><td>OFF (0)<br>HEAT (1)<br>COOL (2)<br>AUTO (3)</td></tr>
<tr><td>CurrentTemperature *</td><td align=>11</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>TargetTemperature *</td><td align=>35</td><td align=>FLOAT</td><td align=>PW+PR+EV</td><td align=>10</td><td align=>38</td><td></td></tr>
<tr><td>TemperatureDisplayUnits *</td><td align=>36</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>CELSIUS (0)<br>FAHRENHEIT (1)</td></tr>
<tr><td>CoolingThresholdTemperature</td><td align=>D</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>10</td><td align=>35</td><td></td></tr>
<tr><td>CurrentRelativeHumidity</td><td align=>10</td><td align=>FLOAT</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>HeatingThresholdTemperature</td><td align=>12</td><td align=>FLOAT</td><td align=>PR+PW+EV</td><td align=>0</td><td align=>25</td><td></td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>TargetRelativeHumidity</td><td align=>34</td><td align=>FLOAT</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
</table><br></details>
<details><summary>Valve (D0)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>Active *</td><td align=>B0</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>1</td><td>INACTIVE (0)<br>ACTIVE (1)</td></tr>
<tr><td>InUse *</td><td align=>D2</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_IN_USE (0)<br>IN_USE (1)</td></tr>
<tr><td>ValveType *</td><td align=>D5</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>3</td><td></td></tr>
<tr><td>SetDuration</td><td align=>D3</td><td align=>UINT32</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>3600</td><td></td></tr>
<tr><td>RemainingDuration</td><td align=>D4</td><td align=>UINT32</td><td align=>PR+EV</td><td align=>0</td><td align=>3600</td><td></td></tr>
<tr><td>IsConfigured</td><td align=>D6</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NOT_CONFIGURED (0)<br>CONFIGURED (1)</td></tr>
<tr><td>ServiceLabelIndex</td><td align=>CB</td><td align=>UINT8</td><td align=>PR</td><td align=>1</td><td align=>255</td><td></td></tr>
<tr><td>StatusFault</td><td align=>77</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td>NO_FAULT (0)<br>FAULT (1)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
</table><br></details>
<details><summary>Window (8B)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>CurrentPosition *</td><td align=>6D</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>TargetPosition *</td><td align=>7C</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>PositionState *</td><td align=>72</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>HoldPosition</td><td align=>6F</td><td align=>BOOL</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>ObstructionDetected</td><td align=>24</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
<details><summary>WindowCovering (8C)</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Values</th></tr>
<tr><td>TargetPosition *</td><td align=>7C</td><td align=>UINT8</td><td align=>PW+PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>CurrentPosition *</td><td align=>6D</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>100</td><td></td></tr>
<tr><td>PositionState *</td><td align=>72</td><td align=>UINT8</td><td align=>PR+EV</td><td align=>0</td><td align=>2</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</td></tr>
<tr><td>Name</td><td align=>23</td><td align=>STRING</td><td align=>PR</td><td colspan=2>(max 64 characters)</td><td></td></tr>
<tr><td>HoldPosition</td><td align=>6F</td><td align=>BOOL</td><td align=>PW</td><td align=>0</td><td align=>1</td><td></td></tr>
<tr><td>CurrentHorizontalTiltAngle</td><td align=>6C</td><td align=>INT</td><td align=>PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
<tr><td>TargetHorizontalTiltAngle</td><td align=>7B</td><td align=>INT</td><td align=>PW+PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
<tr><td>CurrentVerticalTiltAngle</td><td align=>6E</td><td align=>INT</td><td align=>PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
<tr><td>TargetVerticalTiltAngle</td><td align=>7D</td><td align=>INT</td><td align=>PW+PR+EV</td><td align=>-90</td><td align=>90</td><td></td></tr>
<tr><td>ObstructionDetected</td><td align=>24</td><td align=>BOOL</td><td align=>PR+EV</td><td align=>0</td><td align=>1</td><td></td></tr>
</table><br></details>
