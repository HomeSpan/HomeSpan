# HomeSpan Services and Characteristics

HomeSpan implements all HAP-R2 Services and Characteristics except for those that involve video or audio streaming, Apple TV, or advanced lock management (i.e. all HAP Services except those that require Characteristics with a TLV8 data type).

HomeSpan Services and Characteristics are implemented as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Sections 8  and 9 of HAP-R2, but without any spaces.  HomeSpan Services are defined in HomeSpan's `Service` namespace.  HomeSpan Characteristics are defined in HomeSpan's `Characteristic` namespace.  For example, HomeSpan defines the *Carbon Dioxide Sensor* Service (HAP Service 8.7) as `Service::CarbonDioxideSensor`, and the *Carbon Dioxide Detected* Characteristic (HAP Characteristic 9.16) as `Characteristic::CarbonDioxideDetected`.

HomeSpan Services and Characteristics are instantiated with a C++ `new` command.  Services do not take any arguments, whereas Characteristics take a single, optional argument that is used to initialize the value of the Characteristic at startup.  If this argument is not specified, HomeSpan will apply a reasonable [default value](#characteristic-types-and-defaults) based on the Characteristic's type and allowed range.

A list of all HomeSpan Services is provided in the table below.  For each Service the table also indicates which Characteristics are required and which are optional.  For example, a dimmable light bulb could be configured in HomeSpan as such:

```C++
new Service::LightBulb();                         // instantiate a Light Bulb Service
  new Characteristic:On();                          // instantiate the required On Characteristic without setting initial value
  new Characteristic::Brightness(50);               // instantiate an optional Brightness Characteristic and set initial value to 50%
  new Characteristic::Name("Living Room Lamp");     // instantiate an optional Name Characteristic for this Service, and set to "Living Room Lamp"
```

Please see Sections 8 and 9 of HAP-R2 for a complete description of all HAP Services and Characteristics.  Note that HomeSpan's Service and Characteristic Classes already contain all the required HAP fields, such as the UUID, Format, and Permissions, so you don't need to specify any of these parameters.

Additionally, when first starting up, HomeSpan begins by validating the device's configuration to ensure each Service you instantiate includes all required Characteristics, but does not include any Characteristics that are neither required nor optional.  If any errors are found, HomeSpan reports them to the Arduino Serial Monitor.

### Service List

|Service|Required Characteristics|Optional Characteristics|
|-|-|-|
|AccessoryInformation|Identify|FirmwareRevision<br>Manufacturer<br>Model<br>Name<br>SerialNumber<br>HardwareRevision<br>AccessoryFlags|
|AirPurifier|Active<br>CurrentAirPurifierState<br>TargetAirPurifierState|Name<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls|
|AirQualitySensor|AirQuality|Name<br>OzoneDensity<br>NitrogenDioxideDensity<br>SulphurDioxideDensity<br>PM25Density<br>PM10Density<br>VOCDensity<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|BatteryService|BatteryLevel<br>ChargingState<br>StatusLowBattery|Name|
|CarbonDioxideSensor|CarbonDioxideDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonDioxideLevel<br>CarbonDioxidePeakLevel|
|CarbonMonoxideSensor|CarbonMonoxideDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery<br>CarbonMonoxideLevel<br>CarbonMonoxidePeakLevel|
|ContactSensor|ContactSensorState|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|Door|CurrentPosition<br>TargetPosition<br>PositionState|Name<br>HoldPosition<br>ObstructionDetected|
|Doorbell|ProgrammableSwitchEvent|Name<br>Volume<br>Brightness|
|Fan|Active|Name<br>CurrentFanState<br>TargetFanState<br>RotationDirection<br>RotationSpeed<br>SwingMode<br>LockPhysicalControls|
|Faucet|Active|StatusFault<br>Name|
|FilterMaintenance|FilterChangeIndication|Name<br>FilterLifeLevel<br>ResetFilterIndication|
|GarageDoorOpener|CurrentDoorState<br>TargetDoorState<br>ObstructionDetected|LockCurrentState<br>LockTargetState<br>Name|
|HAPProtocolInformation|Version||HeaterCooler|Active<br>CurrentTemperature<br>CurrentHeaterCoolerState<br>TargetHeaterCoolerState|Name<br>RotationSpeed<br>TemperatureDisplayUnits<br>SwingMode<br>CoolingThresholdTemperature<br>HeatingThresholdTemperature<br>LockPhysicalControls|
|HumidifierDehumidifier|Active<br>CurrentRelativeHumidity<br>CurrentHumidifierDehumidifierState<br>TargetHumidifierDehumidifierState|Name<br>RelativeHumidityDehumidifierThreshold<br>RelativeHumidityHumidifierThreshold<br>RotationSpeed<br>SwingMode<br>WaterLevel<br>LockPhysicalControls|
|HumiditySensor|CurrentRelativeHumidity|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|InputSource|Identifier|ConfiguredName<br>IsConfigured<br>CurrentVisibilityState<br>TargetVisibilityState|
|IrrigationSystem|Active<br>ProgramMode<br>InUse|RemainingDuration<br>StatusFault|
|LeakSensor|LeakDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|LightBulb|On|Brightness<br>Hue<br>Name<br>Saturation<br>ColorTemperature|
|LightSensor|CurrentAmbientLightLevel|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|LockMechanism|LockCurrentState<br>LockTargetState|Name|
|Microphone|Mute|Name<br>Volume|
|MotionSensor|MotionDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|OccupancySensor|OccupancyDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|Outlet|On<br>OutletInUse|Name|
|SecuritySystem|SecuritySystemCurrentState<br>SecuritySystemTargetState|Name<br>SecuritySystemAlarmType<br>StatusFault<br>StatusTampered|
|ServiceLabel|ServiceLabelNamespace||Slat|CurrentSlatState<br>SlatType|Name<br>SwingMode<br>CurrentTiltAngle<br>TargetTiltAngle|
|SmokeSensor|SmokeDetected|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|Speaker|Mute|Name<br>Volume|
|StatelessProgrammableSwitch|ProgrammableSwitchEvent|Name<br>ServiceLabelIndex|
|Switch|On|Name|
|Television|Active|ConfiguredName<br>ActiveIdentifier<br>RemoteKey<br>PowerModeSelection|
|TelevisionSpeaker|VolumeControlType<br>VolumeSelector||TemperatureSensor|CurrentTemperature|Name<br>StatusActive<br>StatusFault<br>StatusTampered<br>StatusLowBattery|
|Thermostat|CurrentHeatingCoolingState<br>TargetHeatingCoolingState<br>CurrentTemperature<br>TargetTemperature<br>TemperatureDisplayUnits|CoolingThresholdTemperature<br>CurrentRelativeHumidity<br>HeatingThresholdTemperature<br>Name<br>TargetRelativeHumidity|
|Valve|Active<br>InUse<br>ValveType|SetDuration<br>RemainingDuration<br>IsConfigured<br>ServiceLabelIndex<br>StatusFault<br>Name|
|Window|CurrentPosition<br>TargetPosition<br>PositionState|Name<br>HoldPosition<br>ObstructionDetected|
|WindowCovering|TargetPosition<br>CurrentPosition<br>PositionState|Name<br>HoldPosition<br>CurrentHorizontalTiltAngle<br>TargetHorizontalTiltAngle<br>CurrentVerticalTiltAngle<br>TargetVerticalTiltAngle<br>ObstructionDetected|

### Characteristic Types and Defaults

|Characteristic|Type|Default|Range|
|-|-|-|-|
AccessoryFlags|uint32_t|1|[1,1]|
Active|uint8_t|0|[0,1]|
ActiveIdentifier|uint32_t|0|[0,255]|
AirQuality|uint8_t|0|[0,5]|
BatteryLevel|uint8_t|0|[0,100]|
Brightness|int|0|[0,100]|
CarbonMonoxideLevel|double|0|[0,100]|
CarbonMonoxidePeakLevel|double|0|[0,100]|
CarbonMonoxideDetected|uint8_t|0|[0,1]|
CarbonDioxideLevel|double|0|[0,100000]|
CarbonDioxidePeakLevel|double|0|[0,100000]|
CarbonDioxideDetected|uint8_t|0|[0,1]|
ChargingState|uint8_t|0|[0,2]|
ClosedCaptions|uint8_t|0|[0,1]|
CoolingThresholdTemperature|double|10|[10,35]|
ColorTemperature|uint32_t|200|[140,500]|
ContactSensorState|uint8_t|1|[0,1]|
ConfiguredName|char \*|"unnamed"|||
CurrentAmbientLightLevel|double|1|[0.0001,100000]|
CurrentHorizontalTiltAngle|int|0|[-90,90]|
CurrentAirPurifierState|uint8_t|1|[0,2]|
CurrentSlatState|uint8_t|0|[0,2]|
CurrentPosition|uint8_t|0|[0,100]|
CurrentVerticalTiltAngle|int|0|[-90,90]|
CurrentVisibilityState|uint8_t|0|[0,1]|
CurrentHumidifierDehumidifierState|uint8_t|1|[0,3]|
CurrentDoorState|uint8_t|1|[0,4]|
CurrentFanState|uint8_t|1|[0,2]|
CurrentHeatingCoolingState|uint8_t|0|[0,2]|
CurrentHeaterCoolerState|uint8_t|1|[0,3]|
CurrentMediaState|uint8_t|0|[0,5]|
CurrentRelativeHumidity|double|0|[0,100]|
CurrentTemperature|double|0|[0,100]|
CurrentTiltAngle|int|0|[-90,90]|
FilterLifeLevel|double|0|[0,100]|
FilterChangeIndication|uint8_t|0|[0,1]|
FirmwareRevision|char \*|"1.0.0"|||
HardwareRevision|char \*|"1.0.0"|||
HeatingThresholdTemperature|double|16|[0,25]|
HoldPosition|boolean|false|[0,1]|
Hue|double|0|[0,360]|
Identify|boolean|false|[0,1]|
Identifier|uint32_t|0|[0,255]|
InputDeviceType|uint8_t|0|[0,6]|
InputSourceType|uint8_t|0|[0,10]|
InUse|uint8_t|0|[0,1]|
IsConfigured|uint8_t|0|[0,1]|
LeakDetected|uint8_t|0|[0,1]|
LockCurrentState|uint8_t|0|[0,3]|
LockPhysicalControls|uint8_t|0|[0,1]|
LockTargetState|uint8_t|0|[0,1]|
Manufacturer|char \*|"HomeSpan"|||
Model|char \*|"HomeSpan-ESP32"|||
MotionDetected|boolean|false|[0,1]|
Mute|boolean|false|[0,1]|
Name|char \*|"unnamed"|||
NitrogenDioxideDensity|double|0|[0,1000]|
ObstructionDetected|boolean|false|[0,1]|
PM25Density|double|0|[0,1000]|
OccupancyDetected|uint8_t|0|[0,1]|
OutletInUse|boolean|false|[0,1]|
On|boolean|false|[0,1]|
OzoneDensity|double|0|[0,1000]|
PictureMode|uint8_t|0|[0,13]|
PM10Density|double|0|[0,1000]|
PositionState|uint8_t|2|[0,2]|
PowerModeSelection|uint8_t|0|[0,1]|
ProgramMode|uint8_t|0|[0,2]|
ProgrammableSwitchEvent|uint8_t|0|[0,2]|
RelativeHumidityDehumidifierThreshold|double|50|[0,100]|
RelativeHumidityHumidifierThreshold|double|50|[0,100]|
RemainingDuration|uint32_t|60|[0,3600]|
RemoteKey|uint8_t|0|[0,16]|
ResetFilterIndication|uint8_t|0|[1,1]|
RotationDirection|int|0|[0,1]|
RotationSpeed|double|0|[0,100]|
Saturation|double|0|[0,100]|
SecuritySystemAlarmType|uint8_t|0|[0,1]|
SecuritySystemCurrentState|uint8_t|3|[0,4]|
SecuritySystemTargetState|uint8_t|3|[0,3]|
SerialNumber|char \*|"HS-12345"|||
ServiceLabelIndex|uint8_t|1|[1,255]|
ServiceLabelNamespace|uint8_t|1|[0,1]|
SlatType|uint8_t|0|[0,1]|
SleepDiscoveryMode|uint8_t|0|[0,1]|
SmokeDetected|uint8_t|0|[0,1]|
StatusActive|boolean|true|[0,1]|
StatusFault|uint8_t|0|[0,1]|
StatusJammed|uint8_t|0|[0,1]|
StatusLowBattery|uint8_t|0|[0,1]|
StatusTampered|uint8_t|0|[0,1]|
SulphurDioxideDensity|double|0|[0,1000]|
SwingMode|uint8_t|0|[0,1]|
TargetAirPurifierState|uint8_t|1|[0,1]|
TargetFanState|uint8_t|1|[0,1]|
TargetTiltAngle|int|0|[-90,90]|
TargetHeaterCoolerState|uint8_t|0|[0,2]|
SetDuration|uint32_t|60|[0,3600]|
TargetHorizontalTiltAngle|int|0|[-90,90]|
TargetHumidifierDehumidifierState|uint8_t|0|[0,2]|
TargetPosition|uint8_t|0|[0,100]|
TargetDoorState|uint8_t|1|[0,1]|
TargetHeatingCoolingState|uint8_t|0|[0,3]|
TargetMediaState|uint8_t|0|[0,2]|
TargetRelativeHumidity|double|0|[0,100]|
TargetTemperature|double|16|[10,38]|
TargetVisibilityState|uint8_t|0|[0,1]|
TemperatureDisplayUnits|uint8_t|0|[0,1]|
TargetVerticalTiltAngle|int|0|[-90,90]|
ValveType|uint8_t|0|[0,3]|
Version|char \*|"1.0.0"|||
VOCDensity|double|0|[0,1000]|
Volume|uint8_t|0|[0,100]|
VolumeControlType|uint8_t|0|[0,3]|
VolumeSelector|uint8_t|0|[0,1]|
WaterLevel|double|0|[0,100]|

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
|DATA|uint8_t *|

---

[↩️](../README.md) Back to the Welcome page
