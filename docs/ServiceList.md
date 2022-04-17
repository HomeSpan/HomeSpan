# HomeSpan Services and Characteristics

HomeSpan implements all [HAP-R2](https://developer.apple.com/homekit/specification/) Services and Characteristics except for those that involve video or audio streaming, Apple TV, or advanced lock management (i.e. all HAP Services except those that require Characteristics with a TLV8 data type).

HomeSpan Services and Characteristics are implemented as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Sections 8  and 9 of [HAP-R2](https://developer.apple.com/homekit/specification/), but without any spaces.  HomeSpan Services are defined in HomeSpan's `Service` namespace.  HomeSpan Characteristics are defined in HomeSpan's `Characteristic` namespace.  For example, HomeSpan defines the *Carbon Dioxide Sensor* Service (HAP Service 8.7) as `Service::CarbonDioxideSensor`, and the *Carbon Dioxide Detected* Characteristic (HAP Characteristic 9.16) as `Characteristic::CarbonDioxideDetected`.

HomeSpan Services and Characteristics are instantiated with a C++ `new` command.  Services do not take any arguments, whereas Characteristics take a single, optional argument that is used to initialize the value of the Characteristic at startup.  If this argument is not specified, HomeSpan will apply a reasonable [default value](#characteristic-types-and-defaults) based on the Characteristic's type and allowed range.

A list of all HomeSpan Services is provided in the table below.  For each Service the table also indicates which Characteristics are required and which are optional.  For example, a dimmable light bulb could be configured in HomeSpan as such:

```C++
new Service::LightBulb();                         // instantiate a Light Bulb Service
  new Characteristics:On();                         // instantiate the required On Characteristic without setting initial value
  new Characteristic::Brightness(50);               // instantiate an optional Brightness Characteristic and set initial value to 50%
  new Characteristic::Name("Living Room Lamp");     // instantiate an optional Name Characteristic for this Service, and set to "Living Room Lamp"
```

Please see Sections 8 and 9 of [HAP-R2](https://developer.apple.com/homekit/specification/) for a complete description of all HAP Services and Characteristics.  Note that HomeSpan's Service and Characteristic Classes already contain all the required HAP fields, such as the UUID, Format, and Permissions, so you don't need to specify any of these parameters.

Additionally, when first starting up, HomeSpan begins by validating the device's configuration to ensure each Service you instantiate includes all required Characteristics, but does not include any Characteristics that are neither required nor optional.  If any errors are found, HomeSpan reports them to the Arduino Serial Monitor and halts the program.

### Service List

| Service | Required Characteristics | Optional Characteristics |
| ------- | -------------------- | ------------------- |
| AccessoryInformation| FirmwareRevision<br>Identity<br>Manufacturer<br>Model<br>Name<br>SerialNumber | HardwareRevision |
| AirPurifier | Active<br>CurrentAirPurifierState<br>TargetAirPurifierState | Name<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls |
| AirQualitySensor | AirQuality | Name<br>OzoneDensity<br>NitrogenDioxideDensity<br>SulphurDioxideDensity<br>PM25Density<br>PM10Density<br>VOCDensity<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| BatteryService | BatteryLevel<br>ChargingState<br>StatusLowBattery | Name |
| CarbonDioxideSensor | CarbonDioxideDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonDioxideLevel<br>CarbonDioxidePeakLevel |
| CarbonMonoxideSensor | CarbonMonoxideDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonMonoxideLevel<br>CarbonMonoxidePeakLevel |
| ContactSensor | ContactSensorState | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Door | CurrentPosition<br>TargetPosition<br>PositionState | Name<br>HoldPosition<br>ObstructionDetected |
| Doorbell | ProgrammableSwitchEvent | Name<br>Volume<br>Brightness |
| Fan | Active | Name<br>CurrentFanState<br>TargetFanState<br>RotationDirection<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls |
| Faucet | Active | StatusFault<br>Name |
| FilterMaintenance | FilterChangeIndication | Name<br>FilterLifeLevel<br>ResetFilterIndication |
| GarageDoorOpener | CurrentDoorState<br>TargetDoorState<br>ObstructionDetected | LockCurrentState<br>LockTargetState<br>Name |
| HAPProtocolInformation | Version | *none* |
| HeaterCooler | Active<br>CurrentTemperature<br>CurrentHeaterCoolerState<br>TargetHeaterCoolerState | Name<br>RotationSpeed<br>TemperatureDisplayUnits<br>SwingMode<br>CoolingThresholdTemperature<br>HeatingThresholdTemperature<br>LockPhysicalControls |
| HumidifierDehumidifier | Active<br>CurrentRelativeHumidity<br>CurrentHumidifierDehumidifierState<br>TargetHumidifierDehumidifierState | Name<br>RelativeHumidityDehumidifierThreshold<br>RelativeHumidityHumidifierThreshold<br>RotationSpeed<br>SwingMode<br>WaterLevel<br>LockPhysicalControls |
| HumiditySensor | CurrentRelativeHumidity | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| InputSource | Identifier | ConfiguredName<br>IsConfigured<br>CurrentVisibilityState<br>TargetVisibilityState |
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
| ServiceLabel | ServiceLabelNamespace | *none* |
| Slat | CurrentSlatState<br>SlatType | Name<br>SwingMode<br>CurrentTiltAngle<br>TargetTiltAngle |
| SmokeSensor | SmokeDetected | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Speaker | Mute | Name<br>Volume |
| StatelessProgrammableSwitch | ProgrammableSwitchEvent | Name<br>ServiceLabelIndex |
| Switch | On | Name |
| Television | Active | ConfiguredName<br>ActiveIdentifier<br>RemoteKey<br>PowerModeSelection | 
| TemperatureSensor | CurrentTemperature | Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery |
| Thermostat | CurrentHeatingCoolingState<br>TargetHeatingCoolingState<br>CurrentTemperature<br>TargetTemperature<br>TemperatureDisplayUnits | CoolingThresholdTemperature<br>CurrentRelativeHumidity<br>HeatingThresholdTemperature<br>Name<br>TargetRelativeHumidity | 
| Valve | Active<br>InUse<br>ValveType | SetDuration<br>RemainingDuration<br>IsConfigured<br>ServiceLabelIndex<br>StatusFault<br>Name |
| Window | CurrentPosition<br>TargetPosition<br>PositionState | Name<br>HoldPosition<br>ObstructionDetected |
| WindowCovering | CurrentPosition<br>TargetPosition | Name<br>PositionState<br>HoldPosition<br>CurrentHorizontalTiltAngle<br>TargetHorizontalTiltAngle<br>CurrentVerticalTiltAngle<br>TargetVerticalTiltAngle<br>ObstructionDetected |


### Characteristic Types and Defaults

|Characteristic|Type|Default
|---|---|---|
|Active|uint8_t|0|
|ActiveIdentifier|uint32_t|0|
|AirQuality|uint8_t|0|
|BatteryLevel|uint8_t|0|
|Brightness|int|0|
|CarbonMonoxideLevel|double|0|
|CarbonMonoxidePeakLevel|double|0|
|CarbonMonoxideDetected|uint8_t|0|
|CarbonDioxideLevel|double|0|
|CarbonDioxidePeakLevel|double|0|
|CarbonDioxideDetected|uint8_t|0|
|ChargingState|uint8_t|0|
|CoolingThresholdTemperature|double|10| 
|ColorTemperature|uint32_t|50|
|ConfiguredName|char \*|"unnamed"|
|ContactSensorState|uint8_t|1|
|CurrentAmbientLightLevel|double|1|
|CurrentHorizontalTiltAngle|int|0|
|CurrentAirPurifierState|uint8_t|1|
|CurrentSlatState|uint8_t|0|
|CurrentPosition|uint8_t|0|
|CurrentVerticalTiltAngle|int|0|
|CurrentHumidifierDehumidifierState|uint8_t|1|
|CurrentDoorState|uint8_t|1|
|CurrentFanState|uint8_t|1|
|CurrentHeatingCoolingState|uint8_t|0|
|CurrentHeaterCoolerState|uint8_t|1|
|CurrentRelativeHumidity|double|0|
|CurrentTemperature|double|0|
|CurrentTiltAngle|int|0|
|CurrentVisibilityState|uint8_t|0|
|FilterLifeLevel|double|0|
|FilterChangeIndication|uint8_t|0|
|FirmwareRevision|char \*|"1.0.0"|
|HardwareRevision|char \*|"1.0.0"|
|HeatingThresholdTemperature|double|16|
|HoldPosition|boolean|false|
|Hue|double|0|
|Identifier|uint32_t|0|
|Identify|boolean|false|
|InUse|uint8_t|0|
|IsConfigured|uint8_t|0|
|LeakDetected|uint8_t|0|
|LockCurrentState|uint8_t|0|
|LockPhysicalControls|uint8_t|0|
|LockTargetState|uint8_t|0|
|Manufacturer|char \*|"HomeSpan"|
|Model|char \*|"HomeSpan-ESP32"|
|MotionDetected|boolean|false|
|Mute|boolean|false|
|Name|char \*|"unnamed"|
|NitrogenDioxideDensity|double|0|
|ObstructionDetected|boolean|false|
|PM25Density|double|0|
|OccupancyDetected|uint8_t|0|
|OutletInUse|boolean|false|
|On|boolean|false|
|OzoneDensity|double|0|
|PM10Density|double|0|
|PositionState|uint8_t|2|
|PowerModeSelection|uint8_t|0|
|ProgramMode|uint8_t|0|
|ProgrammableSwitchEvent|uint8_t|0|
|RelativeHumidityDehumidifierThreshold|double|50|
|RelativeHumidityHumidifierThreshold|double|50|
|RemainingDuration|uint32_t|60|
|RemoteKey|uint8_t|0|
|ResetFilterIndication|uint8_t|0|
|RotationDirection|int|0|
|RotationSpeed|double|0|
|Saturation|double|0|
|SecuritySystemAlarmType|uint8_t|0|
|SecuritySystemCurrentState|uint8_t|3|
|SecuritySystemTargetState|uint8_t|3| 
|SerialNumber|char \*|"HS-12345"|
|ServiceLabelIndex|uint8_t|1|
|ServiceLabelNamespace|uint8_t|1|
|SlatType|uint8_t|0|
|SmokeDetected|uint8_t|0|
|StatusActive|boolean|true|
|StatusFault|uint8_t|0|
|StatusJammed|uint8_t|0|
|StatusLowBattery|uint8_t|0|
|StatusTampered|uint8_t|0|
|SulphurDioxideDensity|double|0|
|SwingMode|uint8_t|0|
|TargetAirPurifierState|uint8_t|1|
|TargetFanState|uint8_t|1|
|TargetTiltAngle|int|0|
|SetDuration|uint32_t|60|
|TargetHorizontalTiltAngle|int|0|
|TargetHumidifierDehumidifierState|uint8_t|0|
|TargetPosition|uint8_t|0|
|TargetDoorState|uint8_t|1|
|TargetHeaterCoolerState|uint8_t|0|
|TargetHeatingCoolingState|uint8_t|0|
|TargetRelativeHumidity|double|0|
|TargetTemperature|double|16|
|TemperatureDisplayUnits|uint8_t|0|
|TargetVerticalTiltAngle|int|0|
|TargetVisibilityState|uint8_t|0|
|ValveType|uint8_t|0|
|Version|char \*|"1.0.0"|
|VOCDensity|double|0|
|Volume|uint8_t|0|
|WaterLevel|double|0|


### HAP Format Codes (HAP-R2 Table 6-5)

|HAP-R2 Format Code|HomeSpan C++ Type|
|------------------|-----------------|
|BOOL|boolean|
|UINT8|uint8_t|
|UINT16|uint16_t|
|UINT32|uint32_t|
|UINT64|uint64_t|
|INT|int|
|FLOAT|double|
|STRING|char \*|
|TLV8|(not implemented)|
|DATA|(not implemented)|

---

[↩️](README.md) Back to the Welcome page
