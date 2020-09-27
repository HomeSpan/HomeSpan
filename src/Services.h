
//////////////////////////////////
// HAP SERVICES (HAP Chapter 8) //
//////////////////////////////////

namespace Service {

  struct AccessoryInformation : SpanService { AccessoryInformation() : SpanService{"3E"}{} };

  struct AirPurifier : SpanService { AirPurifier() : SpanService{"BB"}{} };

  struct AirQualitySensor : SpanService { AirQualitySensor() : SpanService{"8D"}{} };

  struct BatteryService : SpanService { BatteryService() : SpanService{"96"}{} };

  struct CarbonDioxideSensor : SpanService { CarbonDioxideSensor() : SpanService{"97"}{} };

  struct CarbonMonoxideSensor : SpanService { CarbonMonoxideSensor() : SpanService{"7F"}{} };

  struct ContactSensor : SpanService { ContactSensor() : SpanService{"80"}{} };

  struct Door : SpanService { Door() : SpanService{"81"}{} };

  struct Doorbell : SpanService { Doorbell() : SpanService{"121"}{} };

  struct Fan : SpanService { Fan() : SpanService{"B7"}{} };

  struct Faucet : SpanService { Faucet() : SpanService{"D7"}{} };

  struct FilterMaintenance : SpanService { FilterMaintenance() : SpanService{"BA"}{} };

  struct GarageDoorOpener : SpanService { GarageDoorOpener() : SpanService{"41"}{} };

  struct HAPProtocolInformation : SpanService { HAPProtocolInformation() : SpanService{"A2"}{} };

  struct HeaterCooler : SpanService { HeaterCooler() : SpanService{"BC"}{} };

  struct HumidifierDehumidifier : SpanService { HumidifierDehumidifier() : SpanService{"BD"}{} };

  struct HumiditySensor : SpanService { HumiditySensor() : SpanService{"82"}{} };

  struct IrrigationSystem : SpanService { IrrigationSystem() : SpanService{"CF"}{} };

  struct LeakSensor : SpanService { LeakSensor() : SpanService{"83"}{} };

  struct LightBulb : SpanService { LightBulb() : SpanService{"43"}{} };

  struct LightSensor : SpanService { LightSensor() : SpanService{"84"}{} };

  struct MotionSensor : SpanService { MotionSensor() : SpanService{"85"}{} };

  struct OccupancySensor : SpanService { OccupancySensor() : SpanService{"86"}{} };

  struct Outlet : SpanService { Outlet() : SpanService{"47"}{} };

  struct ServiceLabel : SpanService { ServiceLabel() : SpanService{"47"}{} };

  struct Slat : SpanService { Slat() : SpanService{"B9"}{} };

  struct SmokeSensor : SpanService { SmokeSensor() : SpanService{"87"}{} };

  struct StatelessProgrammableSwitch : SpanService { StatelessProgrammableSwitch() : SpanService{"89"}{} };

  struct Switch : SpanService { Switch() : SpanService{"49"}{} };

  struct TemperatureSensor : SpanService { TemperatureSensor() : SpanService{"8A"}{} };

  struct Thermostat : SpanService { Thermostat() : SpanService{"4A"}{} };

  struct Valve : SpanService { Valve() : SpanService{"D0"}{} };

  struct Window : SpanService { Window() : SpanService{"8B"}{} };

  struct WindowCovering : SpanService { WindowCovering() : SpanService{"8C"}{} };
  
}

/////////////////////////////////////////
// HAP CHARACTERISTICS (HAP Chapter 9) //
/////////////////////////////////////////

namespace Characteristic {
  
  struct Active : SpanCharacteristic { Active(uint8_t value=0) : SpanCharacteristic{"B0",PR+PW+EV,(uint8_t)value}{} };

  struct AirQuality : SpanCharacteristic { AirQuality(uint8_t value=0) : SpanCharacteristic{"95",PR+EV,(uint8_t)value}{} };

  struct Brightness : SpanCharacteristic { Brightness(int value=0) : SpanCharacteristic{"8",PR+PW+EV,(int)value}{} };

  struct ColorTemperature : SpanCharacteristic { ColorTemperature(uint32_t value=50) : SpanCharacteristic{"CE",PR+PW+EV,(uint32_t)value}{} };

  struct CurrentDoorState : SpanCharacteristic { CurrentDoorState(uint8_t value=1) : SpanCharacteristic{"E",PR+EV,(uint8_t)value}{} };

  struct CurrentPosition : SpanCharacteristic { CurrentPosition(uint8_t value=0) : SpanCharacteristic{"6D",PR+EV,(uint8_t)value}{} };

  struct CurrentTemperature : SpanCharacteristic { CurrentTemperature(double value=0) : SpanCharacteristic{"11",PR+EV,(double)value}{} };

  struct FirmwareRevision : SpanCharacteristic { FirmwareRevision(char *value) : SpanCharacteristic{"52",PR,(char *)value}{} };
  
  struct HoldPosition : SpanCharacteristic { HoldPosition(boolean value=false) : SpanCharacteristic{"6F",PW,(boolean)value}{} };

  struct Hue : SpanCharacteristic { Hue(double value=0) : SpanCharacteristic{"13",PR+PW+EV,(double)value}{} };
  
  struct Identify : SpanCharacteristic { Identify() : SpanCharacteristic{"14",PW,(boolean)false}{} };
  
  struct Manufacturer : SpanCharacteristic { Manufacturer(char *value) : SpanCharacteristic{"20",PR,(char *)value}{} };

  struct Model : SpanCharacteristic { Model(char *value) : SpanCharacteristic{"21",PR,(char *)value}{} };

  struct Name : SpanCharacteristic { Name(char *value) : SpanCharacteristic{"23",PR,(char *)value}{} };

  struct NitrogenDioxideDensity : SpanCharacteristic { NitrogenDioxideDensity(double value=0) : SpanCharacteristic{"C4",PR+EV,(double)value}{} };

  struct ObstructionDetected : SpanCharacteristic { ObstructionDetected(boolean value=false) : SpanCharacteristic{"24",PR+EV,(boolean)value}{} };

  struct On : SpanCharacteristic { On(boolean value=false) : SpanCharacteristic{"25",PR+PW+EV,(boolean)value}{} };
  
  struct OutletInUse : SpanCharacteristic { OutletInUse(boolean value=false) : SpanCharacteristic{"26",PR+EV,(boolean)value}{} };

  struct OzoneDensity : SpanCharacteristic { OzoneDensity(double value=0) : SpanCharacteristic{"C3",PR+EV,(double)value}{} };

  struct PM10Density : SpanCharacteristic { PM10Density(double value=0) : SpanCharacteristic{"C7",PR+EV,(double)value}{} };

  struct PM25Density : SpanCharacteristic { PM25Density(double value=0) : SpanCharacteristic{"C6",PR+EV,(double)value}{} };

  struct PositionState : SpanCharacteristic { PositionState(uint8_t value=2) : SpanCharacteristic{"72",PR+EV,(uint8_t)value}{} };

  struct RotationDirection : SpanCharacteristic { RotationDirection(int value=0) : SpanCharacteristic{"28",PR+PW+EV,(int)value}{} };

  struct RotationSpeed : SpanCharacteristic { RotationSpeed(double value=0) : SpanCharacteristic{"29",PR+PW+EV,(double)value}{} };

  struct Saturation : SpanCharacteristic { Saturation(double value=0) : SpanCharacteristic{"2F",PR+PW+EV,(double)value}{} };

  struct SerialNumber : SpanCharacteristic { SerialNumber(char *value) : SpanCharacteristic{"30",PR,(char *)value}{} };

  struct SlatType : SpanCharacteristic { SlatType(uint8_t value=0) : SpanCharacteristic{"C0",PR,(uint8_t)value}{} };

  struct SmokeDetected : SpanCharacteristic { SmokeDetected(uint8_t value=0) : SpanCharacteristic{"76",PR+EV,(uint8_t)value}{} };

  struct StatusActive : SpanCharacteristic { StatusActive(boolean value=true) : SpanCharacteristic{"75",PR+EV,(boolean)value}{} };

  struct StatusFault : SpanCharacteristic { StatusFault(uint8_t value=0) : SpanCharacteristic{"77",PR+EV,(uint8_t)value}{} };

  struct StatusJammed : SpanCharacteristic { StatusJammed(uint8_t value=0) : SpanCharacteristic{"78",PR+EV,(uint8_t)value}{} };

  struct StatusLowBattery : SpanCharacteristic { StatusLowBattery(uint8_t value=0) : SpanCharacteristic{"79",PR+EV,(uint8_t)value}{} };

  struct StatusTampered : SpanCharacteristic { StatusTampered(uint8_t value=0) : SpanCharacteristic{"7A",PR+EV,(uint8_t)value}{} };

  struct SulphurDioxideDensity : SpanCharacteristic { SulphurDioxideDensity(double value=0) : SpanCharacteristic{"C5",PR+EV,(double)value}{} };

  struct SwingMode : SpanCharacteristic { SwingMode(uint8_t value=0) : SpanCharacteristic{"B6",PR+PW+EV,(uint8_t)value}{} };

  struct TargetDoorState : SpanCharacteristic { TargetDoorState(uint8_t value=1) : SpanCharacteristic{"32",PR+PW+EV,(uint8_t)value}{} };

  struct TargetPosition : SpanCharacteristic { TargetPosition(uint8_t value=0) : SpanCharacteristic{"7C",PR+PW+EV,(uint8_t)value}{} };

  struct TemperatureDisplayUnits : SpanCharacteristic { TemperatureDisplayUnits(uint8_t value=0) : SpanCharacteristic{"36",PR+PW+EV,(uint8_t)value}{} };

  struct Version : SpanCharacteristic { Version(char *value) : SpanCharacteristic{"37",PR,(char *)value}{} };

  struct VOCDensity : SpanCharacteristic { VOCDensity(double value=0) : SpanCharacteristic{"C8",PR+EV,(double)value}{} };

}
