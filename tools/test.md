<details><summary>AccessoryInformation (3E)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Identify *</td><td>14</td><td>BOOL</td><td>PW</td><td>false (0) / true (1)</td></tr>
<tr><td>FirmwareRevision</td><td>52</td><td>STRING</td><td>PR+EV</td><td>(max 64 characters)</td></tr>
<tr><td>Manufacturer</td><td>20</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Model</td><td>21</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>SerialNumber</td><td>30</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>HardwareRevision</td><td>53</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>AccessoryFlags</td><td>A6</td><td>UINT32</td><td>PR+EV</td><td>1-1</td></tr>
</table><br></details>
<details><summary>AirPurifier (BB)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>CurrentAirPurifierState *</td><td>A9</td><td>UINT8</td><td>PR+EV</td><td>INACTIVE (0)<br>IDLE (1)<br>PURIFYING (2)</tr>
<tr><td>TargetAirPurifierState *</td><td>A8</td><td>UINT8</td><td>PW+PR+EV</td><td>MANUAL (0)<br>AUTO (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>RotationSpeed</td><td>29</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>SwingMode</td><td>B6</td><td>UINT8</td><td>PR+EV+PW</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</tr>
<tr><td>LockPhysicalControls</td><td>A7</td><td>UINT8</td><td>PW+PR+EV</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</tr>
</table><br></details>
<details><summary>AirQualitySensor (8D)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>AirQuality *</td><td>95</td><td>UINT8</td><td>PR+EV</td><td>UNKNOWN (0)<br>EXCELLENT (1)<br>GOOD (2)<br>FAIR (3)<br>INFERIOR (4)<br>POOR (5)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>OzoneDensity</td><td>C3</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>NitrogenDioxideDensity</td><td>C4</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>SulphurDioxideDensity</td><td>C5</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>PM25Density</td><td>C6</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>PM10Density</td><td>C7</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>VOCDensity</td><td>C8</td><td>FLOAT</td><td>PR+EV</td><td>0-1000</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>BatteryService (96)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>BatteryLevel *</td><td>68</td><td>UINT8</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>ChargingState *</td><td>8F</td><td>UINT8</td><td>PR+EV</td><td>NOT_CHARGING (0)<br>CHARGING (1)<br>NOT_CHARGEABLE (2)</tr>
<tr><td>StatusLowBattery *</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>CarbonDioxideSensor (97)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CarbonDioxideDetected *</td><td>92</td><td>UINT8</td><td>PR+EV</td><td>NORMAL (0)<br>ABNORMAL (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
<tr><td>CarbonDioxideLevel</td><td>93</td><td>FLOAT</td><td>PR+EV</td><td>0-100000</td></tr>
<tr><td>CarbonDioxidePeakLevel</td><td>94</td><td>FLOAT</td><td>PR+EV</td><td>0-100000</td></tr>
</table><br></details>
<details><summary>CarbonMonoxideSensor (7F)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CarbonMonoxideDetected *</td><td>69</td><td>UINT8</td><td>PR+EV</td><td>NORMAL (0)<br>ABNORMAL (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
<tr><td>CarbonMonoxideLevel</td><td>90</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>CarbonMonoxidePeakLevel</td><td>91</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
</table><br></details>
<details><summary>ContactSensor (80)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>ContactSensorState *</td><td>6A</td><td>UINT8</td><td>PR+EV</td><td>DETECTED (0)<br>NOT_DETECTED (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>Door (81)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentPosition *</td><td>6D</td><td>UINT8</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>TargetPosition *</td><td>7C</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
<tr><td>PositionState *</td><td>72</td><td>UINT8</td><td>PR+EV</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>HoldPosition</td><td>6F</td><td>BOOL</td><td>PW</td><td>false (0) / true (1)</td></tr>
<tr><td>ObstructionDetected</td><td>24</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
</table><br></details>
<details><summary>Doorbell (121)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td>73</td><td>UINT8</td><td>PR+EV+NV</td><td>SINGLE_PRESS (0)<br>DOUBLE_PRESS (1)<br>LONG_PRESS (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Volume</td><td>119</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
<tr><td>Brightness</td><td>8</td><td>INT</td><td>PR+PW+EV</td><td>0-100</td></tr>
</table><br></details>
<details><summary>Fan (B7)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>CurrentFanState</td><td>AF</td><td>UINT8</td><td>PR+EV</td><td>INACTIVE (0)<br>IDLE (1)<br>BLOWING (2)</tr>
<tr><td>TargetFanState</td><td>BF</td><td>UINT8</td><td>PW+PR+EV</td><td>MANUAL (0)<br>AUTO (1)</tr>
<tr><td>RotationDirection</td><td>28</td><td>INT</td><td>PR+PW+EV</td><td>CLOCKWISE (0)<br>COUNTERCLOCKWISE (1)</tr>
<tr><td>RotationSpeed</td><td>29</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>SwingMode</td><td>B6</td><td>UINT8</td><td>PR+EV+PW</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</tr>
<tr><td>LockPhysicalControls</td><td>A7</td><td>UINT8</td><td>PW+PR+EV</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</tr>
</table><br></details>
<details><summary>Faucet (D7)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>FilterMaintenance (BA)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>FilterChangeIndication *</td><td>AC</td><td>UINT8</td><td>PR+EV</td><td>NO_CHANGE_NEEDED (0)<br>CHANGE_NEEDED (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>FilterLifeLevel</td><td>AB</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>ResetFilterIndication</td><td>AD</td><td>UINT8</td><td>PW</td><td>1-1</td></tr>
</table><br></details>
<details><summary>GarageDoorOpener (41)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentDoorState *</td><td>E</td><td>UINT8</td><td>PR+EV</td><td>OPEN (0)<br>CLOSED (1)<br>OPENING (2)<br>CLOSING (3)<br>STOPPED (4)</tr>
<tr><td>TargetDoorState *</td><td>32</td><td>UINT8</td><td>PW+PR+EV</td><td>OPEN (0)<br>CLOSED (1)</tr>
<tr><td>ObstructionDetected *</td><td>24</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>LockCurrentState</td><td>1D</td><td>UINT8</td><td>PR+EV</td><td>UNLOCKED (0)<br>LOCKED (1)<br>JAMMED (2)<br>UNKNOWN (3)</tr>
<tr><td>LockTargetState</td><td>1E</td><td>UINT8</td><td>PW+PR+EV</td><td>UNLOCK (0)<br>LOCK (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>HAPProtocolInformation (A2)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Version *</td><td>37</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>HeaterCooler (BC)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>CurrentTemperature *</td><td>11</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>CurrentHeaterCoolerState *</td><td>B1</td><td>UINT8</td><td>PR+EV</td><td>INACTIVE (0)<br>IDLE (1)<br>HEATING (2)<br>COOLING (3)</tr>
<tr><td>TargetHeaterCoolerState *</td><td>B2</td><td>UINT8</td><td>PW+PR+EV</td><td>AUTO (0)<br>HEAT (1)<br>COOL (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>RotationSpeed</td><td>29</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>TemperatureDisplayUnits</td><td>36</td><td>UINT8</td><td>PW+PR+EV</td><td>CELSIUS (0)<br>FAHRENHEIT (1)</tr>
<tr><td>SwingMode</td><td>B6</td><td>UINT8</td><td>PR+EV+PW</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</tr>
<tr><td>CoolingThresholdTemperature</td><td>D</td><td>FLOAT</td><td>PR+PW+EV</td><td>10-35</td></tr>
<tr><td>HeatingThresholdTemperature</td><td>12</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-25</td></tr>
<tr><td>LockPhysicalControls</td><td>A7</td><td>UINT8</td><td>PW+PR+EV</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</tr>
</table><br></details>
<details><summary>HumidifierDehumidifier (BD)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>CurrentRelativeHumidity *</td><td>10</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>CurrentHumidifierDehumidifierState *</td><td>B3</td><td>UINT8</td><td>PR+EV</td><td>INACTIVE (0)<br>IDLE (1)<br>HUMIDIFYING (2)<br>DEHUMIDIFYING (3)</tr>
<tr><td>TargetHumidifierDehumidifierState *</td><td>B4</td><td>UINT8</td><td>PW+PR+EV</td><td>AUTO (0)<br>HUMIDIFY (1)<br>DEHUMIDIFY (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>RelativeHumidityDehumidifierThreshold</td><td>C9</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>RelativeHumidityHumidifierThreshold</td><td>CA</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>RotationSpeed</td><td>29</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>SwingMode</td><td>B6</td><td>UINT8</td><td>PR+EV+PW</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</tr>
<tr><td>WaterLevel</td><td>B5</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>LockPhysicalControls</td><td>A7</td><td>UINT8</td><td>PW+PR+EV</td><td>CONTROL_LOCK_DISABLED (0)<br>CONTROL_LOCK_ENABLED (1)</tr>
</table><br></details>
<details><summary>HumiditySensor (82)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentRelativeHumidity *</td><td>10</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>InputSource (D9)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>ConfiguredName</td><td>E3</td><td>STRING</td><td>PW+PR+EV</td><td>(max 64 characters)</td></tr>
<tr><td>IsConfigured</td><td>D6</td><td>UINT8</td><td>PR+EV</td><td>NOT_CONFIGURED (0)<br>CONFIGURED (1)</tr>
<tr><td>Identifier *</td><td>E6</td><td>UINT32</td><td>PR</td><td>0-255</td></tr>
<tr><td>CurrentVisibilityState</td><td>135</td><td>UINT8</td><td>PR+EV</td><td>0-1</td></tr>
<tr><td>TargetVisibilityState</td><td>134</td><td>UINT8</td><td>PW+PR+EV</td><td>0-1</td></tr>
</table><br></details>
<details><summary>IrrigationSystem (CF)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>ProgramMode *</td><td>D1</td><td>UINT8</td><td>PR+EV</td><td>NONE (0)<br>SCHEDULED (1)<br>SCHEDULE_OVERRIDEN (2)</tr>
<tr><td>InUse *</td><td>D2</td><td>UINT8</td><td>PR+EV</td><td>NOT_IN_USE (0)<br>IN_USE (1)</tr>
<tr><td>RemainingDuration</td><td>D4</td><td>UINT32</td><td>PR+EV</td><td>0-3600</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
</table><br></details>
<details><summary>LeakSensor (83)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>LeakDetected *</td><td>70</td><td>UINT8</td><td>PR+EV</td><td>NOT_DETECTED (0)<br>DETECTED (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>LightBulb (43)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>On *</td><td>25</td><td>BOOL</td><td>PR+PW+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Brightness</td><td>8</td><td>INT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>Hue</td><td>13</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-360</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Saturation</td><td>2F</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-100</td></tr>
<tr><td>ColorTemperature</td><td>CE</td><td>UINT32</td><td>PR+PW+EV</td><td>140-500</td></tr>
</table><br></details>
<details><summary>LightSensor (84)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentAmbientLightLevel *</td><td>6B</td><td>FLOAT</td><td>PR+EV</td><td>0.0001-100000</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>LockMechanism (45)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>LockCurrentState *</td><td>1D</td><td>UINT8</td><td>PR+EV</td><td>UNLOCKED (0)<br>LOCKED (1)<br>JAMMED (2)<br>UNKNOWN (3)</tr>
<tr><td>LockTargetState *</td><td>1E</td><td>UINT8</td><td>PW+PR+EV</td><td>UNLOCK (0)<br>LOCK (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>Microphone (112)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Mute *</td><td>11A</td><td>BOOL</td><td>PW+PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Volume</td><td>119</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
</table><br></details>
<details><summary>MotionSensor (85)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>MotionDetected *</td><td>22</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>OccupancySensor (86)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>OccupancyDetected *</td><td>71</td><td>UINT8</td><td>PR+EV</td><td>NOT_DETECTED (0)<br>DETECTED (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>Outlet (47)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>On *</td><td>25</td><td>BOOL</td><td>PR+PW+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>OutletInUse *</td><td>26</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>SecuritySystem (7E)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>SecuritySystemCurrentState *</td><td>66</td><td>UINT8</td><td>PR+EV</td><td>ARMED_STAY (0)<br>ARMED_AWAY (1)<br>ARMED_NIGHT (2)<br>DISARMED (3)<br>ALARM_TRIGGERED (4)</tr>
<tr><td>SecuritySystemTargetState *</td><td>67</td><td>UINT8</td><td>PW+PR+EV</td><td>ARM_STAY (0)<br>ARM_AWAY (1)<br>ARM_NIGHT (2)<br>DISARM (3)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>SecuritySystemAlarmType</td><td>8E</td><td>UINT8</td><td>PR+EV</td><td>KNOWN (0)<br>UNKNOWN (1)</tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
</table><br></details>
<details><summary>ServiceLabel (CC)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>ServiceLabelNamespace *</td><td>CD</td><td>UINT8</td><td>PR</td><td>DOTS (0)<br>NUMERALS (1)</tr>
</table><br></details>
<details><summary>Slat (B9)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentSlatState *</td><td>AA</td><td>UINT8</td><td>PR+EV</td><td>FIXED (0)<br>JAMMED (1)<br>SWINGING (2)</tr>
<tr><td>SlatType *</td><td>C0</td><td>UINT8</td><td>PR</td><td>HORIZONTAL (0)<br>VERTICAL (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>SwingMode</td><td>B6</td><td>UINT8</td><td>PR+EV+PW</td><td>SWING_DISABLED (0)<br>SWING_ENABLED (1)</tr>
<tr><td>CurrentTiltAngle</td><td>C1</td><td>INT</td><td>PR+EV</td><td>-90-90</td></tr>
<tr><td>TargetTiltAngle</td><td>C2</td><td>INT</td><td>PW+PR+EV</td><td>-90-90</td></tr>
</table><br></details>
<details><summary>SmokeSensor (87)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>SmokeDetected *</td><td>76</td><td>UINT8</td><td>PR+EV</td><td>NOT_DETECTED (0)<br>DETECTED (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>Speaker (113)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Mute *</td><td>11A</td><td>BOOL</td><td>PW+PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>Volume</td><td>119</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
</table><br></details>
<details><summary>StatelessProgrammableSwitch (89)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>ProgrammableSwitchEvent *</td><td>73</td><td>UINT8</td><td>PR+EV+NV</td><td>SINGLE_PRESS (0)<br>DOUBLE_PRESS (1)<br>LONG_PRESS (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>ServiceLabelIndex</td><td>CB</td><td>UINT8</td><td>PR</td><td>1-255</td></tr>
</table><br></details>
<details><summary>Switch (49)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>On *</td><td>25</td><td>BOOL</td><td>PR+PW+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>Television (D8)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>ConfiguredName</td><td>E3</td><td>STRING</td><td>PW+PR+EV</td><td>(max 64 characters)</td></tr>
<tr><td>ActiveIdentifier</td><td>E7</td><td>UINT32</td><td>PW+PR+EV</td><td>0-255</td></tr>
<tr><td>RemoteKey</td><td>E1</td><td>UINT8</td><td>PW</td><td>0-16</td></tr>
<tr><td>PowerModeSelection</td><td>DF</td><td>UINT8</td><td>PW</td><td>0-1</td></tr>
</table><br></details>
<details><summary>TelevisionSpeaker (113)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>VolumeControlType *</td><td>E9</td><td>UINT8</td><td>PR+EV</td><td>0-3</td></tr>
<tr><td>VolumeSelector *</td><td>EA</td><td>UINT8</td><td>PW</td><td>0-1</td></tr>
</table><br></details>
<details><summary>TemperatureSensor (8A)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentTemperature *</td><td>11</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>StatusActive</td><td>75</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>StatusTampered</td><td>7A</td><td>UINT8</td><td>PR+EV</td><td>NOT_TAMPERED (0)<br>TAMPERED (1)</tr>
<tr><td>StatusLowBattery</td><td>79</td><td>UINT8</td><td>PR+EV</td><td>NOT_LOW_BATTERY (0)<br>LOW_BATTERY (1)</tr>
</table><br></details>
<details><summary>Thermostat (4A)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentHeatingCoolingState *</td><td>F</td><td>UINT8</td><td>PR+EV</td><td>OFF (0)<br>HEATING (1)<br>COOLING (2)</tr>
<tr><td>TargetHeatingCoolingState *</td><td>33</td><td>UINT8</td><td>PW+PR+EV</td><td>OFF (0)<br>HEAT (1)<br>COOL (2)<br>AUTO (3)</tr>
<tr><td>CurrentTemperature *</td><td>11</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>TargetTemperature *</td><td>35</td><td>FLOAT</td><td>PW+PR+EV</td><td>10-38</td></tr>
<tr><td>TemperatureDisplayUnits *</td><td>36</td><td>UINT8</td><td>PW+PR+EV</td><td>CELSIUS (0)<br>FAHRENHEIT (1)</tr>
<tr><td>CoolingThresholdTemperature</td><td>D</td><td>FLOAT</td><td>PR+PW+EV</td><td>10-35</td></tr>
<tr><td>CurrentRelativeHumidity</td><td>10</td><td>FLOAT</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>HeatingThresholdTemperature</td><td>12</td><td>FLOAT</td><td>PR+PW+EV</td><td>0-25</td></tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>TargetRelativeHumidity</td><td>34</td><td>FLOAT</td><td>PW+PR+EV</td><td>0-100</td></tr>
</table><br></details>
<details><summary>Valve (D0)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>Active *</td><td>B0</td><td>UINT8</td><td>PW+PR+EV</td><td>INACTIVE (0)<br>ACIVE (1)</tr>
<tr><td>InUse *</td><td>D2</td><td>UINT8</td><td>PR+EV</td><td>NOT_IN_USE (0)<br>IN_USE (1)</tr>
<tr><td>ValveType *</td><td>D5</td><td>UINT8</td><td>PR+EV</td><td>0-3</td></tr>
<tr><td>SetDuration</td><td>D3</td><td>UINT32</td><td>PW+PR+EV</td><td>0-3600</td></tr>
<tr><td>RemainingDuration</td><td>D4</td><td>UINT32</td><td>PR+EV</td><td>0-3600</td></tr>
<tr><td>IsConfigured</td><td>D6</td><td>UINT8</td><td>PR+EV</td><td>NOT_CONFIGURED (0)<br>CONFIGURED (1)</tr>
<tr><td>ServiceLabelIndex</td><td>CB</td><td>UINT8</td><td>PR</td><td>1-255</td></tr>
<tr><td>StatusFault</td><td>77</td><td>UINT8</td><td>PR+EV</td><td>NO_FAULT (0)<br>FAULT (1)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
</table><br></details>
<details><summary>Window (8B)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>CurrentPosition *</td><td>6D</td><td>UINT8</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>TargetPosition *</td><td>7C</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
<tr><td>PositionState *</td><td>72</td><td>UINT8</td><td>PR+EV</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>HoldPosition</td><td>6F</td><td>BOOL</td><td>PW</td><td>false (0) / true (1)</td></tr>
<tr><td>ObstructionDetected</td><td>24</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
</table><br></details>
<details><summary>WindowCovering (8C)</summary><br><table>
<tr><th>Characteristic</th<th>UUID</th><th>Format</th><th>Perms</th><th>Values</th></tr>
<tr><td>TargetPosition *</td><td>7C</td><td>UINT8</td><td>PW+PR+EV</td><td>0-100</td></tr>
<tr><td>CurrentPosition *</td><td>6D</td><td>UINT8</td><td>PR+EV</td><td>0-100</td></tr>
<tr><td>PositionState *</td><td>72</td><td>UINT8</td><td>PR+EV</td><td>GOING_TO_MINIMUM (0)<br>GOING_TO_MAXIMUM (1)<br>STOPPED (2)</tr>
<tr><td>Name</td><td>23</td><td>STRING</td><td>PR</td><td>(max 64 characters)</td></tr>
<tr><td>HoldPosition</td><td>6F</td><td>BOOL</td><td>PW</td><td>false (0) / true (1)</td></tr>
<tr><td>CurrentHorizontalTiltAngle</td><td>6C</td><td>INT</td><td>PR+EV</td><td>-90-90</td></tr>
<tr><td>TargetHorizontalTiltAngle</td><td>7B</td><td>INT</td><td>PW+PR+EV</td><td>-90-90</td></tr>
<tr><td>CurrentVerticalTiltAngle</td><td>6E</td><td>INT</td><td>PR+EV</td><td>-90-90</td></tr>
<tr><td>TargetVerticalTiltAngle</td><td>7D</td><td>INT</td><td>PW+PR+EV</td><td>-90-90</td></tr>
<tr><td>ObstructionDetected</td><td>24</td><td>BOOL</td><td>PR+EV</td><td>false (0) / true (1)</td></tr>
</table><br></details>
