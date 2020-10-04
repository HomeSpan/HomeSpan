
//////////////////////////////////
// HAP SERVICES (HAP Chapter 8) //
//////////////////////////////////

namespace Service {

  struct AccessoryInformation : SpanService { AccessoryInformation() : SpanService{"3E","AccessoryInformation","52,14,20,21,23,30","53"}{} };

  struct AirPurifier : SpanService { AirPurifier() : SpanService{"BB","AirPurifier","B0,A9,A8","23,29,B6,A7"}{} };

  struct AirQualitySensor : SpanService { AirQualitySensor() : SpanService{"8D","AirQualitySensor","95","23,C3,C4,C5,C6,C7,C8,75,77,7A,79"}{} };

  struct BatteryService : SpanService { BatteryService() : SpanService{"96","BatteryService"}{} };

  struct CarbonDioxideSensor : SpanService { CarbonDioxideSensor() : SpanService{"97","CarbonDioxideSensor"}{} };

  struct CarbonMonoxideSensor : SpanService { CarbonMonoxideSensor() : SpanService{"7F","CarbonMonoxideSensor"}{} };

  struct ContactSensor : SpanService { ContactSensor() : SpanService{"80","ContactSensor"}{} };

  struct Door : SpanService { Door() : SpanService{"81","Door"}{} };

  struct Doorbell : SpanService { Doorbell() : SpanService{"121","Doorbell"}{} };

  struct Fan : SpanService { Fan() : SpanService{"B7","Fan"}{} };

  struct Faucet : SpanService { Faucet() : SpanService{"D7","Faucet"}{} };

  struct FilterMaintenance : SpanService { FilterMaintenance() : SpanService{"BA","FilterMaintenance"}{} };

  struct GarageDoorOpener : SpanService { GarageDoorOpener() : SpanService{"41","GarageDoorOpener"}{} };

  struct HAPProtocolInformation : SpanService { HAPProtocolInformation() : SpanService{"A2","HAPProtocolInformation"}{} };

  struct HeaterCooler : SpanService { HeaterCooler() : SpanService{"BC","HeaterCooler"}{} };

  struct HumidifierDehumidifier : SpanService { HumidifierDehumidifier() : SpanService{"BD","HumidifierDehumidifier"}{} };

  struct HumiditySensor : SpanService { HumiditySensor() : SpanService{"82","HumiditySensor"}{} };

  struct IrrigationSystem : SpanService { IrrigationSystem() : SpanService{"CF","IrrigationSystem"}{} };

  struct LeakSensor : SpanService { LeakSensor() : SpanService{"83","LeakSensor"}{} };

  struct LightBulb : SpanService { LightBulb() : SpanService{"43","LightBulb"}{} };

  struct LightSensor : SpanService { LightSensor() : SpanService{"84","LightSensor"}{} };

  struct MotionSensor : SpanService { MotionSensor() : SpanService{"85","MotionSensor"}{} };

  struct OccupancySensor : SpanService { OccupancySensor() : SpanService{"86","OccupancySensor"}{} };

  struct Outlet : SpanService { Outlet() : SpanService{"47","Outlet"}{} };

  struct ServiceLabel : SpanService { ServiceLabel() : SpanService{"47","ServiceLabel"}{} };

  struct Slat : SpanService { Slat() : SpanService{"B9","Slat"}{} };

  struct SmokeSensor : SpanService { SmokeSensor() : SpanService{"87","SmokeSensor"}{} };

  struct StatelessProgrammableSwitch : SpanService { StatelessProgrammableSwitch() : SpanService{"89","StatelessProgrammableSwitch"}{} };

  struct Switch : SpanService { Switch() : SpanService{"49","Switch"}{} };

  struct TemperatureSensor : SpanService { TemperatureSensor() : SpanService{"8A","TemperatureSensor"}{} };

  struct Thermostat : SpanService { Thermostat() : SpanService{"4A","Thermostat"}{} };

  struct Valve : SpanService { Valve() : SpanService{"D0","Valve"}{} };

  struct Window : SpanService { Window() : SpanService{"8B","Window"}{} };

  struct WindowCovering : SpanService { WindowCovering() : SpanService{"8C","WindowCovering"}{} };
  
}

/////////////////////////////////////////
// HAP CHARACTERISTICS (HAP Chapter 9) //
/////////////////////////////////////////

namespace Characteristic {
  
  struct Active : SpanCharacteristic { Active(uint8_t value=0) : SpanCharacteristic{"B0",PR+PW+EV,(uint8_t)value,"Active"}{} };

  struct AirQuality : SpanCharacteristic { AirQuality(uint8_t value=0) : SpanCharacteristic{"95",PR+EV,(uint8_t)value,"AirQuality"}{} };

  struct Brightness : SpanCharacteristic { Brightness(int value=0) : SpanCharacteristic{"8",PR+PW+EV,(int)value,"Brightness"}{} };

  struct ColorTemperature : SpanCharacteristic { ColorTemperature(uint32_t value=50) : SpanCharacteristic{"CE",PR+PW+EV,(uint32_t)value,"ColorTemperature"}{} };

  struct CurrentDoorState : SpanCharacteristic { CurrentDoorState(uint8_t value=1) : SpanCharacteristic{"E",PR+EV,(uint8_t)value,"CurrentDoorState"}{} };

  struct CurrentPosition : SpanCharacteristic { CurrentPosition(uint8_t value=0) : SpanCharacteristic{"6D",PR+EV,(uint8_t)value,"CurrentPosition"}{} };

  struct CurrentTemperature : SpanCharacteristic { CurrentTemperature(double value=0) : SpanCharacteristic{"11",PR+EV,(double)value,"CurrentTemperature"}{} };

  struct FirmwareRevision : SpanCharacteristic { FirmwareRevision(char *value) : SpanCharacteristic{"52",PR,(char *)value,"FirmwareRevision"}{} };
  
  struct HoldPosition : SpanCharacteristic { HoldPosition(boolean value=false) : SpanCharacteristic{"6F",PW,(boolean)value,"HoldPosition"}{} };

  struct Hue : SpanCharacteristic { Hue(double value=0) : SpanCharacteristic{"13",PR+PW+EV,(double)value,"Hue"}{} };
  
  struct Identify : SpanCharacteristic { Identify() : SpanCharacteristic{"14",PW,(boolean)false,"Identify"}{} };
  
  struct Manufacturer : SpanCharacteristic { Manufacturer(char *value) : SpanCharacteristic{"20",PR,(char *)value,"Manufacturer"}{} };

  struct Model : SpanCharacteristic { Model(char *value) : SpanCharacteristic{"21",PR,(char *)value,"Model"}{} };

  struct Name : SpanCharacteristic { Name(char *value) : SpanCharacteristic{"23",PR,(char *)value,"Name"}{} };

  struct NitrogenDioxideDensity : SpanCharacteristic { NitrogenDioxideDensity(double value=0) : SpanCharacteristic{"C4",PR+EV,(double)value,"NitrogenDioxideDensity"}{} };

  struct ObstructionDetected : SpanCharacteristic { ObstructionDetected(boolean value=false) : SpanCharacteristic{"24",PR+EV,(boolean)value,"ObstructionDetected"}{} };

  struct On : SpanCharacteristic { On(boolean value=false) : SpanCharacteristic{"25",PR+PW+EV,(boolean)value,"On"}{} };
  
  struct OutletInUse : SpanCharacteristic { OutletInUse(boolean value=false) : SpanCharacteristic{"26",PR+EV,(boolean)value,"OutletInUse"}{} };

  struct OzoneDensity : SpanCharacteristic { OzoneDensity(double value=0) : SpanCharacteristic{"C3",PR+EV,(double)value,"OzoneDensity"}{} };

  struct PM10Density : SpanCharacteristic { PM10Density(double value=0) : SpanCharacteristic{"C7",PR+EV,(double)value,"PM10Density"}{} };

  struct PM25Density : SpanCharacteristic { PM25Density(double value=0) : SpanCharacteristic{"C6",PR+EV,(double)value,"PM25Density"}{} };

  struct PositionState : SpanCharacteristic { PositionState(uint8_t value=2) : SpanCharacteristic{"72",PR+EV,(uint8_t)value,"PositionState"}{} };

  struct RotationDirection : SpanCharacteristic { RotationDirection(int value=0) : SpanCharacteristic{"28",PR+PW+EV,(int)value,"RotationDirection"}{} };

  struct RotationSpeed : SpanCharacteristic { RotationSpeed(double value=0) : SpanCharacteristic{"29",PR+PW+EV,(double)value,"RotationSpeed"}{} };

  struct Saturation : SpanCharacteristic { Saturation(double value=0) : SpanCharacteristic{"2F",PR+PW+EV,(double)value,"Saturation"}{} };

  struct SerialNumber : SpanCharacteristic { SerialNumber(char *value) : SpanCharacteristic{"30",PR,(char *)value,"SerialNumber"}{} };

  struct SlatType : SpanCharacteristic { SlatType(uint8_t value=0) : SpanCharacteristic{"C0",PR,(uint8_t)value,"SlatType"}{} };

  struct SmokeDetected : SpanCharacteristic { SmokeDetected(uint8_t value=0) : SpanCharacteristic{"76",PR+EV,(uint8_t)value,"SmokeDetected"}{} };

  struct StatusActive : SpanCharacteristic { StatusActive(boolean value=true) : SpanCharacteristic{"75",PR+EV,(boolean)value,"StatusActive"}{} };

  struct StatusFault : SpanCharacteristic { StatusFault(uint8_t value=0) : SpanCharacteristic{"77",PR+EV,(uint8_t)value,"StatusFault"}{} };

  struct StatusJammed : SpanCharacteristic { StatusJammed(uint8_t value=0) : SpanCharacteristic{"78",PR+EV,(uint8_t)value,"StatusJammed"}{} };

  struct StatusLowBattery : SpanCharacteristic { StatusLowBattery(uint8_t value=0) : SpanCharacteristic{"79",PR+EV,(uint8_t)value,"StatusLowBattery"}{} };

  struct StatusTampered : SpanCharacteristic { StatusTampered(uint8_t value=0) : SpanCharacteristic{"7A",PR+EV,(uint8_t)value,"StatusTampered"}{} };

  struct SulphurDioxideDensity : SpanCharacteristic { SulphurDioxideDensity(double value=0) : SpanCharacteristic{"C5",PR+EV,(double)value,"SulphurDioxideDensity"}{} };

  struct SwingMode : SpanCharacteristic { SwingMode(uint8_t value=0) : SpanCharacteristic{"B6",PR+PW+EV,(uint8_t)value,"SwingMode"}{} };

  struct TargetDoorState : SpanCharacteristic { TargetDoorState(uint8_t value=1) : SpanCharacteristic{"32",PR+PW+EV,(uint8_t)value,"TargetDoorState"}{} };

  struct TargetPosition : SpanCharacteristic { TargetPosition(uint8_t value=0) : SpanCharacteristic{"7C",PR+PW+EV,(uint8_t)value,"TargetPosition"}{} };

  struct TemperatureDisplayUnits : SpanCharacteristic { TemperatureDisplayUnits(uint8_t value=0) : SpanCharacteristic{"36",PR+PW+EV,(uint8_t)value,"TemperatureDisplayUnits"}{} };

  struct Version : SpanCharacteristic { Version(char *value) : SpanCharacteristic{"37",PR,(char *)value,"Version"}{} };

  struct VOCDensity : SpanCharacteristic { VOCDensity(double value=0) : SpanCharacteristic{"C8",PR+EV,(double)value,"VOCDensity"}{} };

}
