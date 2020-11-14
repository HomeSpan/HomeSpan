| Service | Required Characteristics | Optional Characteristics |
| ------- | -------------------- | ------------------- |
| AccessoryInformation| FirmwareRevision<br>Identity<br>Manufacturer<br>Model<br>Name<br>SerialNumber | HardwareRevision |
| AirPurifier | Active<br>CurrentAirPurifierState<br>TargetAirPurifierState | Name<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls |
| AirQualitySensor | AirQuality | Name<br>OzoneDensity<br>NitrogenDioxideDensity<br>SulphurDioxideDensity<br>PM25Density<br>PM10Density<br>VOCDensity<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| BatteryService | BatteryLevel<br>ChargingState<br>StatusLowBattery | Name |
| CarbonDioxideSensor | CarbonDioxideDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonDioxideLevel<br>CarbonDioxidePeakLevel |
| CarbonMonoxideSensor | CarbonMonoxideDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonMonoxideLevel<br>CarbonMonoxidePeakLevel |
| ContactSensor | ContactSensorState | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Door | CurrentPosition | <br>TargetPosition<br>PositionState | Name<br>HoldPosition<br>ObstructionDetected |
| Doorbell | ProgrammableSwitchEvent | Name<br>Volume<br>Brightness |
| Fan | Active | Name<br>CurrentFanState<br>TargetFanState<br>RotationDirection<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls |
| Faucet | Active | StatusFault<br>Name |
| FilterMaintenance | FilterChangeIndication | Name<br>FilterLifeLevel<br>ResetFilterIndication |
| GarageDoorOpener | CurrentDoorState<br>TargetDoorState<br>ObstructionDetected | LockCurrentState<br>LockTargetState<br>Name |
| HAPProtocolInformation | Version | |
| HeaterCooler | Active<br>CurrentTemperature<br>CurrentHeaterCoolerState<br>TargetHeaterCoolerState | Name<br>RotationSpeed<br>TemperatureDisplayUnits<br>SwingMode<br>CoolingThresholdTemperature<br>HeatingThresholdTemperature<br>LockPhysicalControls |
| HumidifierDehumidifier | Active<br>CurrentRelativeHumidity<br>CurrentHumidifierDehumidifierState<br>TargetHumidifierDehumidifierState | Name<br>RelativeHumidityDehumidifierThreshold<br>RelativeHumidityHumidifierThreshold<br>RotationSpeed<br>SwingMode<br>WaterLevel<br>LockPhysicalControls |
| HumiditySensor | CurrentRelativeHumidity | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| IrrigationSystem | Active<br>ProgramMode<br>InUse | RemainingDuration<br>Name<br>StatusFault |
| LeakSensor | LeakDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| LightBulb | On | Brightness<br>Hue<br>Name<br>Saturation<br>ColorTemperature |
| LightSensor | CurrentAmbientLightLevel | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| LockMechanism | LockCurrentState<br>LockTargetState | Name |
| Microphone | Mute | Name<br>Volume |
| MotionSensor | MotionDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| OccupancySensor | OccupancyDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Outlet | On<br>OutletInUse | Name |
| SecuritySystem | SecuritySystemCurrentState<br>SecuritySystemTargetState | Name<br>SecuritySystemAlarmType<br>StatusFault<br>StatusTampered |
| ServiceLabel | ServiceLabelNamespace | |
| Slat | CurrentSlatState<br>SlatType | Name<br>SwingMode<br>CurrentTiltAngle<br>TargetTiltAngle |
| SmokeSensor | SmokeDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Speaker | Mute | Name<br>Volume |
| StatelessProgrammableSwitch | ProgrammableSwitchEvent | Name<br>ServiceLabelIndex |
| Switch | On | Name |
| TemperatureSensor | CurrentTemperature | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Thermostat | CurrentHeatingCoolingState<br>TargetHeatingCoolingState<br>CurrentTemperature<br>TargetTemperature<br>TemperatureDisplayUnits | CoolingThresholdTemperature<br>CurrentRelativeHumidity<br>HeatingThresholdTemperature<br>Name<br>TargetRelativeHumidity | 
| Valve | Active<br>InUse<br>ValveType | SetDuration<br>RemainingDuration<br>IsConfigured<br>ServiceLabelIndex<br>StatusFault<br>Name |
| Window | CurrentPosition<br>TargetPosition<br>PositionState | Name<br>HoldPosition<br>ObstructionDetected |
| WindowCovering | CurrentPosition<br>TargetPosition<br>PositionState | Name<br>HoldPosition<br>CurrentHorizontalTiltAngle<br>TargetHorizontalTiltAngle<br>CurrentVerticalTiltAngle<br>TargetVerticalTiltAngle<br>ObstructionDetected |
