
//////////////////////////////////
// HAP SERVICES (HAP Chapter 8) //
//////////////////////////////////

namespace Service {

  struct AccessoryInformation : SpanService { AccessoryInformation(ServiceType mod=ServiceType::Regular) : SpanService{"3E", mod}{} };

  struct AirPurifier : SpanService { AirPurifier(ServiceType mod=ServiceType::Regular) : SpanService{"BB", mod}{} };

  struct AirQualitySensor : SpanService { AirQualitySensor(ServiceType mod=ServiceType::Regular) : SpanService{"8D", mod}{} };

  struct BatteryService : SpanService { BatteryService(ServiceType mod=ServiceType::Regular) : SpanService{"96", mod}{} };

  struct CarbonDioxideSensor : SpanService { CarbonDioxideSensor(ServiceType mod=ServiceType::Regular) : SpanService{"97", mod}{} };

  struct CarbonMonoxideSensor : SpanService { CarbonMonoxideSensor(ServiceType mod=ServiceType::Regular) : SpanService{"7F", mod}{} };

  struct ContactSensor : SpanService { ContactSensor(ServiceType mod=ServiceType::Regular) : SpanService{"80", mod}{} };

  struct Door : SpanService { Door(ServiceType mod=ServiceType::Regular) : SpanService{"81", mod}{} };

  struct Doorbell : SpanService { Doorbell(ServiceType mod=ServiceType::Regular) : SpanService{"121", mod}{} };

  struct Fan : SpanService { Fan(ServiceType mod=ServiceType::Regular) : SpanService{"B7", mod}{} };

  struct Faucet : SpanService { Faucet(ServiceType mod=ServiceType::Regular) : SpanService{"D7", mod}{} };

  struct FilterMaintenance : SpanService { FilterMaintenance(ServiceType mod=ServiceType::Regular) : SpanService{"BA", mod}{} };

  struct GarageDoorOpener : SpanService { GarageDoorOpener(ServiceType mod=ServiceType::Regular) : SpanService{"41", mod}{} };

  struct HAPProtocolInformation : SpanService { HAPProtocolInformation(ServiceType mod=ServiceType::Regular) : SpanService{"A2", mod}{} };

  struct HeaterCooler : SpanService { HeaterCooler(ServiceType mod=ServiceType::Regular) : SpanService{"BC", mod}{} };

  struct HumidifierDehumidifier : SpanService { HumidifierDehumidifier(ServiceType mod=ServiceType::Regular) : SpanService{"BD", mod}{} };

  struct HumiditySensor : SpanService { HumiditySensor(ServiceType mod=ServiceType::Regular) : SpanService{"82", mod}{} };

  struct IrrigationSystem : SpanService { IrrigationSystem(ServiceType mod=ServiceType::Regular) : SpanService{"CF", mod}{} };

  struct LeakSensor : SpanService { LeakSensor(ServiceType mod=ServiceType::Regular) : SpanService{"83", mod}{} };

  struct LightBulb : SpanService { LightBulb(ServiceType mod=ServiceType::Regular) : SpanService{"43", mod}{} };

  struct LightSensor : SpanService { LightSensor(ServiceType mod=ServiceType::Regular) : SpanService{"84", mod}{} };

  struct MotionSensor : SpanService { MotionSensor(ServiceType mod=ServiceType::Regular) : SpanService{"85", mod}{} };

  struct OccupancySensor : SpanService { OccupancySensor(ServiceType mod=ServiceType::Regular) : SpanService{"86", mod}{} };

  struct Outlet : SpanService { Outlet(ServiceType mod=ServiceType::Regular) : SpanService{"47", mod}{} };

  struct ServiceLabel : SpanService { ServiceLabel(ServiceType mod=ServiceType::Regular) : SpanService{"47", mod}{} };

  struct Slat : SpanService { Slat(ServiceType mod=ServiceType::Regular) : SpanService{"B9", mod}{} };

  struct SmokeSensor : SpanService { SmokeSensor(ServiceType mod=ServiceType::Regular) : SpanService{"87", mod}{} };

  struct StatelessProgrammableSwitch : SpanService { StatelessProgrammableSwitch(ServiceType mod=ServiceType::Regular) : SpanService{"89", mod}{} };

  struct Switch : SpanService { Switch(ServiceType mod=ServiceType::Regular) : SpanService{"49", mod}{} };

  struct TemperatureSensor : SpanService { TemperatureSensor(ServiceType mod=ServiceType::Regular) : SpanService{"8A", mod}{} };

  struct Thermostat : SpanService { Thermostat(ServiceType mod=ServiceType::Regular) : SpanService{"4A", mod}{} };

  struct Valve : SpanService { Valve(ServiceType mod=ServiceType::Regular) : SpanService{"D0", mod}{} };

  struct Window : SpanService { Window(ServiceType mod=ServiceType::Regular) : SpanService{"8B", mod}{} };

  struct WindowCovering : SpanService { WindowCovering(ServiceType mod=ServiceType::Regular) : SpanService{"8C", mod}{} };
  
}

/////////////////////////////////////////
// HAP CHARACTERISTICS (HAP Chapter 9) //
/////////////////////////////////////////

namespace Characteristic {
  
  struct Active : SpanCharacteristic { Active(uint8_t value=0) : SpanCharacteristic{"B0",PR+PW+EV,(uint8_t)value}{} };

  struct Brightness : SpanCharacteristic { Brightness(int value=0) : SpanCharacteristic{"8",PR+PW+EV,(int)value}{} };

  struct ColorTemperature : SpanCharacteristic { ColorTemperature(uint32_t value=50) : SpanCharacteristic{"CE",PR+PW+EV,(uint32_t)value}{} };

  struct FirmwareRevision : SpanCharacteristic { FirmwareRevision(char *value) : SpanCharacteristic{"52",PR,(char *)value}{} };
  
  struct Hue : SpanCharacteristic { Hue(double value=0) : SpanCharacteristic{"13",PR+PW+EV,(double)value}{} };
  
  struct Identify : SpanCharacteristic { Identify() : SpanCharacteristic{"14",PW,(boolean)false}{} };
  
  struct Manufacturer : SpanCharacteristic { Manufacturer(char *value) : SpanCharacteristic{"20",PR,(char *)value}{} };

  struct Model : SpanCharacteristic { Model(char *value) : SpanCharacteristic{"21",PR,(char *)value}{} };

  struct Name : SpanCharacteristic { Name(char *value) : SpanCharacteristic{"23",PR,(char *)value}{} };

  struct On : SpanCharacteristic { On(boolean value=false) : SpanCharacteristic{"25",PR+PW+EV,(boolean)value}{} };
  
  struct OutletInUse : SpanCharacteristic { OutletInUse(boolean value=false) : SpanCharacteristic{"26",PR+EV,(boolean)value}{} };

  struct RotationDirection : SpanCharacteristic { RotationDirection(int value=0) : SpanCharacteristic{"28",PR+PW+EV,(int)value}{} };

  struct RotationSpeed : SpanCharacteristic { RotationSpeed(double value=0) : SpanCharacteristic{"29",PR+PW+EV,(double)value}{} };

  struct Saturation : SpanCharacteristic { Saturation(double value=0) : SpanCharacteristic{"2F",PR+PW+EV,(double)value}{} };

  struct SerialNumber : SpanCharacteristic { SerialNumber(char *value) : SpanCharacteristic{"30",PR,(char *)value}{} };

  struct SwingMode : SpanCharacteristic { SwingMode(uint8_t value=0) : SpanCharacteristic{"B6",PR+PW+EV,(uint8_t)value}{} };

  struct CurrentTemperature : SpanCharacteristic { CurrentTemperature(double value=0) : SpanCharacteristic{"11",PR+EV,(double)value}{} };

  struct Version : SpanCharacteristic { Version(char *value) : SpanCharacteristic{"37",PR,(char *)value}{} };
  
}
