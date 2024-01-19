<detail><summary>AccessoryInformation (3E)</summary>
  REQ:Identify
false 0 1

  OPT:FirmwareRevision
  OPT:Manufacturer
  OPT:Model
  OPT:Name
  OPT:SerialNumber
  OPT:HardwareRevision
  OPT:AccessoryFlags
</detail>
<detail><summary>AirPurifier (BB)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  REQ:CurrentAirPurifierState
1 0 2
INACTIVE
IDLE
PURIFYING

  REQ:TargetAirPurifierState
1 0 1
MANUAL
AUTO

  OPT:Name
  OPT:RotationSpeed
  OPT:SwingMode
  OPT:LockPhysicalControls
</detail>
<detail><summary>AirQualitySensor (8D)</summary>
  REQ:AirQuality
0 0 5
UNKNOWN
EXCELLENT
GOOD
FAIR
INFERIOR
POOR

  OPT:Name
  OPT:OzoneDensity
  OPT:NitrogenDioxideDensity
  OPT:SulphurDioxideDensity
  OPT:PM25Density
  OPT:PM10Density
  OPT:VOCDensity
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>BatteryService (96)</summary>
  REQ:BatteryLevel
0 0 100

  REQ:ChargingState
0 0 2
NOT_CHARGING
CHARGING
NOT_CHARGEABLE

  REQ:StatusLowBattery
0 0 1
NOT_LOW_BATTERY
LOW_BATTERY

  OPT:Name
</detail>
<detail><summary>CarbonDioxideSensor (97)</summary>
  REQ:CarbonDioxideDetected
0 0 1
NORMAL
ABNORMAL

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
  OPT:CarbonDioxideLevel
  OPT:CarbonDioxidePeakLevel
</detail>
<detail><summary>CarbonMonoxideSensor (7F)</summary>
  REQ:CarbonMonoxideDetected
0 0 1
NORMAL
ABNORMAL

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
  OPT:CarbonMonoxideLevel
  OPT:CarbonMonoxidePeakLevel
</detail>
<detail><summary>ContactSensor (80)</summary>
  REQ:ContactSensorState
1 0 1
DETECTED
NOT_DETECTED

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>Door (81)</summary>
  REQ:CurrentPosition
0 0 100

  REQ:TargetPosition
0 0 100

  REQ:PositionState
2 0 2
GOING_TO_MINIMUM
GOING_TO_MAXIMUM
STOPPED

  OPT:Name
  OPT:HoldPosition
  OPT:ObstructionDetected
</detail>
<detail><summary>Doorbell (121)</summary>
  REQ:ProgrammableSwitchEvent
0 0 2
SINGLE_PRESS
DOUBLE_PRESS
LONG_PRESS

  OPT:Name
  OPT:Volume
  OPT:Brightness
</detail>
<detail><summary>Fan (B7)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  OPT:Name
  OPT:CurrentFanState
  OPT:TargetFanState
  OPT:RotationDirection
  OPT:RotationSpeed
  OPT:SwingMode
  OPT:LockPhysicalControls
</detail>
<detail><summary>Faucet (D7)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  OPT:StatusFault
  OPT:Name
</detail>
<detail><summary>FilterMaintenance (BA)</summary>
  REQ:FilterChangeIndication
0 0 1
NO_CHANGE_NEEDED
CHANGE_NEEDED

  OPT:Name
  OPT:FilterLifeLevel
  OPT:ResetFilterIndication
</detail>
<detail><summary>GarageDoorOpener (41)</summary>
  REQ:CurrentDoorState
1 0 4
OPEN
CLOSED
OPENING
CLOSING
STOPPED

  REQ:TargetDoorState
1 0 1
OPEN
CLOSED

  REQ:ObstructionDetected
false 0 1

  OPT:LockCurrentState
  OPT:LockTargetState
  OPT:Name
</detail>
<detail><summary>HAPProtocolInformation (A2)</summary>
  REQ:Version
  
</detail>
<detail><summary>HeaterCooler (BC)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  REQ:CurrentTemperature
0 0 100

  REQ:CurrentHeaterCoolerState
1 0 3
INACTIVE
IDLE
HEATING
COOLING

  REQ:TargetHeaterCoolerState
0 0 2
AUTO
HEAT
COOL

  OPT:Name
  OPT:RotationSpeed
  OPT:TemperatureDisplayUnits
  OPT:SwingMode
  OPT:CoolingThresholdTemperature
  OPT:HeatingThresholdTemperature
  OPT:LockPhysicalControls
</detail>
<detail><summary>HumidifierDehumidifier (BD)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  REQ:CurrentRelativeHumidity
0 0 100

  REQ:CurrentHumidifierDehumidifierState
1 0 3
INACTIVE
IDLE
HUMIDIFYING
DEHUMIDIFYING

  REQ:TargetHumidifierDehumidifierState
0 0 2
AUTO
HUMIDIFY
DEHUMIDIFY

  OPT:Name
  OPT:RelativeHumidityDehumidifierThreshold
  OPT:RelativeHumidityHumidifierThreshold
  OPT:RotationSpeed
  OPT:SwingMode
  OPT:WaterLevel
  OPT:LockPhysicalControls
</detail>
<detail><summary>HumiditySensor (82)</summary>
  REQ:CurrentRelativeHumidity
0 0 100

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>InputSource (D9)</summary>
  REQ:Identifier
0 0 255

  OPT:ConfiguredName
  OPT:IsConfigured
  OPT:CurrentVisibilityState
  OPT:TargetVisibilityState
</detail>
<detail><summary>IrrigationSystem (CF)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  REQ:ProgramMode
0 0 2
NONE
SCHEDULED
SCHEDULE_OVERRIDEN

  REQ:InUse
0 0 1
NOT_IN_USE
IN_USE

  OPT:RemainingDuration
  OPT:StatusFault
</detail>
<detail><summary>LeakSensor (83)</summary>
  REQ:LeakDetected
0 0 1
NOT_DETECTED
DETECTED

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>LightBulb (43)</summary>
  REQ:On
false 0 1

  OPT:Brightness
  OPT:Hue
  OPT:Name
  OPT:Saturation
  OPT:ColorTemperature
</detail>
<detail><summary>LightSensor (84)</summary>
  REQ:CurrentAmbientLightLevel
1 0.0001 100000

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>LockMechanism (45)</summary>
  REQ:LockCurrentState
0 0 3
UNLOCKED
LOCKED
JAMMED
UNKNOWN

  REQ:LockTargetState
0 0 1
UNLOCK
LOCK

  OPT:Name
</detail>
<detail><summary>Microphone (112)</summary>
  REQ:Mute
false 0 1
OFF
ON

  OPT:Name
  OPT:Volume
</detail>
<detail><summary>MotionSensor (85)</summary>
  REQ:MotionDetected
false 0 1

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>OccupancySensor (86)</summary>
  REQ:OccupancyDetected
0 0 1
NOT_DETECTED
DETECTED

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>Outlet (47)</summary>
  REQ:On
false 0 1

  REQ:OutletInUse
false 0 1

  OPT:Name
</detail>
<detail><summary>SecuritySystem (7E)</summary>
  REQ:SecuritySystemCurrentState
3 0 4
ARMED_STAY
ARMED_AWAY
ARMED_NIGHT
DISARMED
ALARM_TRIGGERED

  REQ:SecuritySystemTargetState
3 0 3
ARM_STAY
ARM_AWAY
ARM_NIGHT
DISARM

  OPT:Name
  OPT:SecuritySystemAlarmType
  OPT:StatusFault
  OPT:StatusTampered
</detail>
<detail><summary>ServiceLabel (CC)</summary>
  REQ:ServiceLabelNamespace
1 0 1
DOTS
NUMERALS

</detail>
<detail><summary>Slat (B9)</summary>
  REQ:CurrentSlatState
0 0 2
FIXED
JAMMED
SWINGING

  REQ:SlatType
0 0 1
HORIZONTAL
VERTICAL

  OPT:Name
  OPT:SwingMode
  OPT:CurrentTiltAngle
  OPT:TargetTiltAngle
</detail>
<detail><summary>SmokeSensor (87)</summary>
  REQ:SmokeDetected
0 0 1
NOT_DETECTED
DETECTED

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>Speaker (113)</summary>
  REQ:Mute
false 0 1
OFF
ON

  OPT:Name
  OPT:Volume
</detail>
<detail><summary>StatelessProgrammableSwitch (89)</summary>
  REQ:ProgrammableSwitchEvent
0 0 2
SINGLE_PRESS
DOUBLE_PRESS
LONG_PRESS

  OPT:Name
  OPT:ServiceLabelIndex
</detail>
<detail><summary>Switch (49)</summary>
  REQ:On
false 0 1

  OPT:Name
</detail>
<detail><summary>Television (D8)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  OPT:ConfiguredName
  OPT:ActiveIdentifier
  OPT:RemoteKey
  OPT:PowerModeSelection
</detail>
<detail><summary>TelevisionSpeaker (113)</summary>
  REQ:VolumeControlType
0 0 3

  REQ:VolumeSelector
0 0 1

</detail>
<detail><summary>TemperatureSensor (8A)</summary>
  REQ:CurrentTemperature
0 0 100

  OPT:Name
  OPT:StatusActive
  OPT:StatusFault
  OPT:StatusTampered
  OPT:StatusLowBattery
</detail>
<detail><summary>Thermostat (4A)</summary>
  REQ:CurrentHeatingCoolingState
0 0 2
OFF
HEATING
COOLING

  REQ:TargetHeatingCoolingState
0 0 3
OFF
HEAT
COOL
AUTO

  REQ:CurrentTemperature
0 0 100

  REQ:TargetTemperature
16 10 38

  REQ:TemperatureDisplayUnits
0 0 1
CELSIUS
FAHRENHEIT

  OPT:CoolingThresholdTemperature
  OPT:CurrentRelativeHumidity
  OPT:HeatingThresholdTemperature
  OPT:Name
  OPT:TargetRelativeHumidity
</detail>
<detail><summary>Valve (D0)</summary>
  REQ:Active
0 0 1
INACTIVE
ACIVE

  REQ:InUse
0 0 1
NOT_IN_USE
IN_USE

  REQ:ValveType
0 0 3

  OPT:SetDuration
  OPT:RemainingDuration
  OPT:IsConfigured
  OPT:ServiceLabelIndex
  OPT:StatusFault
  OPT:Name
</detail>
<detail><summary>Window (8B)</summary>
  REQ:CurrentPosition
0 0 100

  REQ:TargetPosition
0 0 100

  REQ:PositionState
2 0 2
GOING_TO_MINIMUM
GOING_TO_MAXIMUM
STOPPED

  OPT:Name
  OPT:HoldPosition
  OPT:ObstructionDetected
</detail>
<detail><summary>WindowCovering (8C)</summary>
  REQ:TargetPosition
0 0 100

  REQ:CurrentPosition
0 0 100

  REQ:PositionState
2 0 2
GOING_TO_MINIMUM
GOING_TO_MAXIMUM
STOPPED

  OPT:Name
  OPT:HoldPosition
  OPT:CurrentHorizontalTiltAngle
  OPT:TargetHorizontalTiltAngle
  OPT:CurrentVerticalTiltAngle
  OPT:TargetVerticalTiltAngle
  OPT:ObstructionDetected
</detail>
