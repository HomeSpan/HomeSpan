<details><summary>AccessoryInformation (3E)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Identify (14)</b></td><td>BOOL</td><td>PW</td></tr>
<tr><td>FirmwareRevision (52)</td><td>STRING</td><td>PR+EV</td><td>-</td></tr>
<tr><td>Manufacturer (20)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Model (21)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>SerialNumber (30)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>HardwareRevision (53)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>AccessoryFlags (A6)</td><td>UINT32</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>AirPurifier (BB)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>CurrentAirPurifierState (A9)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetAirPurifierState (A8)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>RotationSpeed (29)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>SwingMode (B6)</td><td>UINT8</td><td>PR+EV+PW</td></tr>
<tr><td>LockPhysicalControls (A7)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>AirQualitySensor (8D)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>AirQuality (95)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>OzoneDensity (C3)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>NitrogenDioxideDensity (C4)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>SulphurDioxideDensity (C5)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>PM25Density (C6)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>PM10Density (C7)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>VOCDensity (C8)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>BatteryService (96)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>BatteryLevel (68)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>ChargingState (8F)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>StatusLowBattery (79)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>CarbonDioxideSensor (97)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CarbonDioxideDetected (92)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>CarbonDioxideLevel (93)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>CarbonDioxidePeakLevel (94)</td><td>FLOAT</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>CarbonMonoxideSensor (7F)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CarbonMonoxideDetected (69)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>CarbonMonoxideLevel (90)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>CarbonMonoxidePeakLevel (91)</td><td>FLOAT</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>ContactSensor (80)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>ContactSensorState (6A)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>Door (81)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentPosition (6D)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetPosition (7C)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>PositionState (72)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>HoldPosition (6F)</td><td>BOOL</td><td>PW</td></tr>
<tr><td>ObstructionDetected (24)</td><td>BOOL</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>Doorbell (121)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>ProgrammableSwitchEvent (73)</b></td><td>UINT8</td><td>PR+EV+NV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Volume (119)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Brightness (8)</td><td>INT</td><td>PR+PW+EV</td></tr>
</table><br></details>
<details><summary>Fan (B7)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>CurrentFanState (AF)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>TargetFanState (BF)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>RotationDirection (28)</td><td>INT</td><td>PR+PW+EV</td></tr>
<tr><td>RotationSpeed (29)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>SwingMode (B6)</td><td>UINT8</td><td>PR+EV+PW</td></tr>
<tr><td>LockPhysicalControls (A7)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>Faucet (D7)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>FilterMaintenance (BA)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>FilterChangeIndication (AC)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>FilterLifeLevel (AB)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>ResetFilterIndication (AD)</td><td>UINT8</td><td>PW</td></tr>
</table><br></details>
<details><summary>GarageDoorOpener (41)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentDoorState (E)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetDoorState (32)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>ObstructionDetected (24)</b></td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>LockCurrentState (1D)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>LockTargetState (1E)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>HAPProtocolInformation (A2)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Version (37)</b></td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>HeaterCooler (BC)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>CurrentTemperature (11)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td><b>CurrentHeaterCoolerState (B1)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetHeaterCoolerState (B2)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>RotationSpeed (29)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>TemperatureDisplayUnits (36)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>SwingMode (B6)</td><td>UINT8</td><td>PR+EV+PW</td></tr>
<tr><td>CoolingThresholdTemperature (D)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>HeatingThresholdTemperature (12)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>LockPhysicalControls (A7)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>HumidifierDehumidifier (BD)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>CurrentRelativeHumidity (10)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td><b>CurrentHumidifierDehumidifierState (B3)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetHumidifierDehumidifierState (B4)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>RelativeHumidityDehumidifierThreshold (C9)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>RelativeHumidityHumidifierThreshold (CA)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>RotationSpeed (29)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>SwingMode (B6)</td><td>UINT8</td><td>PR+EV+PW</td></tr>
<tr><td>WaterLevel (B5)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>LockPhysicalControls (A7)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>HumiditySensor (82)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentRelativeHumidity (10)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>InputSource (D9)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td>ConfiguredName (E3)</td><td>STRING</td><td>PW+PR+EV</td><td>-</td></tr>
<tr><td>IsConfigured (D6)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>Identifier (E6)</b></td><td>UINT32</td><td>PR</td></tr>
<tr><td>CurrentVisibilityState (135)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>TargetVisibilityState (134)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>IrrigationSystem (CF)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>ProgramMode (D1)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>InUse (D2)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>RemainingDuration (D4)</td><td>UINT32</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>LeakSensor (83)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>LeakDetected (70)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>LightBulb (43)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>On (25)</b></td><td>BOOL</td><td>PR+PW+EV</td></tr>
<tr><td>Brightness (8)</td><td>INT</td><td>PR+PW+EV</td></tr>
<tr><td>Hue (13)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Saturation (2F)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>ColorTemperature (CE)</td><td>UINT32</td><td>PR+PW+EV</td></tr>
</table><br></details>
<details><summary>LightSensor (84)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentAmbientLightLevel (6B)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>LockMechanism (45)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>LockCurrentState (1D)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>LockTargetState (1E)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>Microphone (112)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Mute (11A)</b></td><td>BOOL</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Volume (119)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>MotionSensor (85)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>MotionDetected (22)</b></td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>OccupancySensor (86)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>OccupancyDetected (71)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>Outlet (47)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>On (25)</b></td><td>BOOL</td><td>PR+PW+EV</td></tr>
<tr><td><b>OutletInUse (26)</b></td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>SecuritySystem (7E)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>SecuritySystemCurrentState (66)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>SecuritySystemTargetState (67)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>SecuritySystemAlarmType (8E)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>ServiceLabel (CC)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>ServiceLabelNamespace (CD)</b></td><td>UINT8</td><td>PR</td></tr>
</table><br></details>
<details><summary>Slat (B9)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentSlatState (AA)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>SlatType (C0)</b></td><td>UINT8</td><td>PR</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>SwingMode (B6)</td><td>UINT8</td><td>PR+EV+PW</td></tr>
<tr><td>CurrentTiltAngle (C1)</td><td>INT</td><td>PR+EV</td></tr>
<tr><td>TargetTiltAngle (C2)</td><td>INT</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>SmokeSensor (87)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>SmokeDetected (76)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>Speaker (113)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Mute (11A)</b></td><td>BOOL</td><td>PW+PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>Volume (119)</td><td>UINT8</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>StatelessProgrammableSwitch (89)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>ProgrammableSwitchEvent (73)</b></td><td>UINT8</td><td>PR+EV+NV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>ServiceLabelIndex (CB)</td><td>UINT8</td><td>PR</td></tr>
</table><br></details>
<details><summary>Switch (49)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>On (25)</b></td><td>BOOL</td><td>PR+PW+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>Television (D8)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>ConfiguredName (E3)</td><td>STRING</td><td>PW+PR+EV</td><td>-</td></tr>
<tr><td>ActiveIdentifier (E7)</td><td>UINT32</td><td>PW+PR+EV</td></tr>
<tr><td>RemoteKey (E1)</td><td>UINT8</td><td>PW</td></tr>
<tr><td>PowerModeSelection (DF)</td><td>UINT8</td><td>PW</td></tr>
</table><br></details>
<details><summary>TelevisionSpeaker (113)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>VolumeControlType (E9)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>VolumeSelector (EA)</b></td><td>UINT8</td><td>PW</td></tr>
</table><br></details>
<details><summary>TemperatureSensor (8A)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentTemperature (11)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>StatusActive (75)</td><td>BOOL</td><td>PR+EV</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusTampered (7A)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>StatusLowBattery (79)</td><td>UINT8</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>Thermostat (4A)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentHeatingCoolingState (F)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetHeatingCoolingState (33)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>CurrentTemperature (11)</b></td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td><b>TargetTemperature (35)</b></td><td>FLOAT</td><td>PW+PR+EV</td></tr>
<tr><td><b>TemperatureDisplayUnits (36)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td>CoolingThresholdTemperature (D)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>CurrentRelativeHumidity (10)</td><td>FLOAT</td><td>PR+EV</td></tr>
<tr><td>HeatingThresholdTemperature (12)</td><td>FLOAT</td><td>PR+PW+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>TargetRelativeHumidity (34)</td><td>FLOAT</td><td>PW+PR+EV</td></tr>
</table><br></details>
<details><summary>Valve (D0)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>Active (B0)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>InUse (D2)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>ValveType (D5)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>SetDuration (D3)</td><td>UINT32</td><td>PW+PR+EV</td></tr>
<tr><td>RemainingDuration (D4)</td><td>UINT32</td><td>PR+EV</td></tr>
<tr><td>IsConfigured (D6)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>ServiceLabelIndex (CB)</td><td>UINT8</td><td>PR</td></tr>
<tr><td>StatusFault (77)</td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
</table><br></details>
<details><summary>Window (8B)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>CurrentPosition (6D)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>TargetPosition (7C)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>PositionState (72)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>HoldPosition (6F)</td><td>BOOL</td><td>PW</td></tr>
<tr><td>ObstructionDetected (24)</td><td>BOOL</td><td>PR+EV</td></tr>
</table><br></details>
<details><summary>WindowCovering (8C)</summary><br><table>
<tr><td><i>Characteristic (UUID)</td><td><i>Format</td><td><i>Perms</td><td><i>Values</td></tr>
<tr><td><b>TargetPosition (7C)</b></td><td>UINT8</td><td>PW+PR+EV</td></tr>
<tr><td><b>CurrentPosition (6D)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td><b>PositionState (72)</b></td><td>UINT8</td><td>PR+EV</td></tr>
<tr><td>Name (23)</td><td>STRING</td><td>PR</td><td>-</td></tr>
<tr><td>HoldPosition (6F)</td><td>BOOL</td><td>PW</td></tr>
<tr><td>CurrentHorizontalTiltAngle (6C)</td><td>INT</td><td>PR+EV</td></tr>
<tr><td>TargetHorizontalTiltAngle (7B)</td><td>INT</td><td>PW+PR+EV</td></tr>
<tr><td>CurrentVerticalTiltAngle (6E)</td><td>INT</td><td>PR+EV</td></tr>
<tr><td>TargetVerticalTiltAngle (7D)</td><td>INT</td><td>PW+PR+EV</td></tr>
<tr><td>ObstructionDetected (24)</td><td>BOOL</td><td>PR+EV</td></tr>
</table><br></details>
