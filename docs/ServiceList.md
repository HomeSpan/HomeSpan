# HomeSpan Services and Characteristics

HomeSpan implements all [HAP-R2](https://developer.apple.com/support/homekit-accessory-protocol/) Services and Characteristics except for those that involve video or audio streaming, Apple TV, or advanced lock management (i.e. all HAP Services except those that require Characteristics with a TLV8 data type).

HomeSpan Services and Characteristics are implemented as C++ Classes with names that exactly match the spelling and capitalization specified by Apple in Sections 8  and 9 of [HAP-R2](https://developer.apple.com/support/homekit-accessory-protocol/), but without any spaces.  HomeSpan Services are defined in HomeSpan's `Service` namespace.  HomeSpan Characteristics are defined in HomeSpan's `Characteristic` namespace.  For example, HomeSpan defines the *Carbon Dioxide Sensor* Service (HAP Service 8.7) as `Service::CarbonDioxideSensor`, and the *Carbon Dioxide Detected* Characteristic (HAP Characteristic 9.16) as `Characteristic::CarbonDioxideDetected`.

HomeSpan Services and Characteristics are instantiated with a C++ `new` command.  Services do not take any arguments, whereas Characteristics take a single, optional argument that is used to initialize the value of the Characteristic at startup.  If this argument is not specified, HomeSpan will apply a reasonable default value based on the Characteristic's type and allowed range.

A list of all HomeSpan Services is provided in the table below.  For each Service the table also indicates which Characteristics are required and which are optional.  For example, a dimmable light bulb could be configured in HomeSpan as such:

```C++
new Service::LightBulb();                         // instantiate a Light Bulb Service
  new Characteristics:On();                         // instantiate the required On Characteristic without setting initial value
  new Characteristic::Brightness(50);               // instantiate an optional Brightness Characteristic and set initial value to 50%
  new Characteristic::Name("Living Room Lamp");     // instantiate an optional Name Characteristic for this Service, and set to "Living Room Lamp"
```

Please see Sections 8 and 9 of [HAP-R2](https://developer.apple.com/support/homekit-accessory-protocol/) for a complete description of all HAP Services and Characteristics.  Note that HomeSpan's Service and Characteristic Classes already contain all the required HAP fields, such as the UUID, Format, and Permissions, so you don't need to specify any of these parameters.

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
| Door | CurrentPosition | <br>TargetPosition<br>PositionState<br>Name<br>HoldPosition<br>ObstructionDetected |
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
