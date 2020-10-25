
/////////////////////////////////////////
// HAP CHARACTERISTICS (HAP Chapter 9) //
/////////////////////////////////////////

#define CHAR_Active "B0"
#define CHAR_CarbonDioxideDetected ""
#define CHAR_ChargingState ""
#define CHAR_CurrentAirPurifierState ""
#define CHAR_AirQuality "95"
#define CHAR_BatteryLevel ""
#define CHAR_Brightness "8"
#define CHAR_ColorTemperature "CE"
#define CHAR_CurrentDoorState "E"
#define CHAR_CurrentPosition "6D"
#define CHAR_CurrentTemperature "11"
#define CHAR_FirmwareRevision "52"
#define CHAR_HardwareRevision "53"
#define CHAR_HoldPosition "6F"
#define CHAR_Hue "13"
#define CHAR_Identify "14"
#define CHAR_LockPhysicalControls ""
#define CHAR_Manufacturer "20"
#define CHAR_Model "21"
#define CHAR_Name "23"
#define CHAR_NitrogenDioxideDensity "C4"
#define CHAR_ObstructionDetected "24"
#define CHAR_On "25"
#define CHAR_OutletInUse "26"
#define CHAR_OzoneDensity "C3"
#define CHAR_PM10Density "C7"
#define CHAR_PM25Density "C6"
#define CHAR_PositionState "72"
#define CHAR_RotationDirection "28"
#define CHAR_RotationSpeed "29"
#define CHAR_Saturation  "2F"
#define CHAR_SerialNumber "30"
#define CHAR_SlatType "C0"
#define CHAR_SmokeDetected "76"
#define CHAR_StatusActive "75"
#define CHAR_StatusFault "77"
#define CHAR_StatusJammed "78"
#define CHAR_StatusLowBattery "79"
#define CHAR_StatusTampered "7A"
#define CHAR_SulphurDioxideDensity "C5"
#define CHAR_SwingMode "B6"
#define CHAR_TargetAirPurifierState ""
#define CHAR_TargetDoorState "32"
#define CHAR_TargetPosition "7C"
#define CHAR_TemperatureDisplayUnits "36"
#define CHAR_Version "37"
#define CHAR_VOCDensity "C8"

#define IDNAME(id) id,#id

//////////////////////////////////
// HAP SERVICES (HAP Chapter 8) //
//////////////////////////////////

#define SVC_AccessoryInformation "3E"
#define SVC_AccessoryInformation_Required CHAR_FirmwareRevision " " CHAR_Identify " " CHAR_Manufacturer " " CHAR_Model " " CHAR_Name " " CHAR_SerialNumber
#define SVC_AccessoryInformation_Optional CHAR_HardwareRevision

#define SVC_AirPurifier "BB"
#define SVC_AirPurifier_Required CHAR_Active " " CHAR_CurrentAirPurifierState " " CHAR_TargetAirPurifierState
#define SVC_AirPurifier_Optional CHAR_Name " " CHAR_RotationalSpeed " " CHAR_SwingMode " " CHAR_LockPhysicalControls

#define SVC_AirQualitySensor "8D"
#define SVC_AirQualitySensor_Required CHAR_AirQuality
#define SVC_AirQualitySensor_Optional CHAR_Name " " CHAR_OzoneDensity " " CHAR_NitrogenDioxideDensity " " CHAR_SulphurDioxideDensity " " CHAR_PM25Density " " \
  CHAR_PM10Density " " CHAR_VOCDensity " " CHAR_StatusActive " " CHAR_StatusFault " " CHAR_StatusTampered " " CHAR_StatusLowBattery

#define SVC_BatteryService "96"
#define SVC_BatteryService_Required CHAR_BatteryLevel " " CHAR_ChargingState " " CHAR_StatusLowBattery
#define SVC_BatteryService_Optional CHAR_NAME

#define SVC_CarbonDioxideSensor "92"
#define SVC_CarbonDioxideSensor_Required CHAR_CarbonDioxideDetected
#define SVC_CarbonDioxideSensor_Optional CHAR_Name " " CHAR_StatusActive " " CHAR_StatusFault " " CHAR_StatusTampered " " CHAR_StatusLowBattery " " \
  CHAR_CarbonDioxideLevel " " CHAR_CarbonDioxidePeakLevel

#define SVC_CarbonMonoxideSensor "7F"
#define SVC_CarbonMonoxideSensor_Required CHAR_CarbonMonoxideDetected
#define SVC_CarbonMonoxideSensor_Optional CHAR_Name " " CHAR_StatusActive " " CHAR_StatusFault " " CHAR_StatusTampered " " CHAR_StatusLowBattery " " \
  CHAR_CarbonMonoxideLevel " " CHAR_CarbonMonoxidePeakLevel

#define SVC_ContactSensor "80"
#define SVC_ContactSensor_Required CHAR_ContactSensorState
#define SVC_ContactSensor_Optional CHAR_Name " " CHAR_StatusActive " " CHAR_StatusFault " " CHAR_StatusTampered " " CHAR_StatusLowBattery

#define SVC_Door "81"
#define SVC_Door_Required CHAR_CurrentPosition " " CHAR_TargetPosition " " CHAR_PositionState
#define SVC_Door_Optional CHAR_Name " " CHAR_HoldPosition " " CHAR_ObstructionDetected

#define SVC_Doorbell "121"
#define SVC_Doorbell_Required CHAR_ProgrammableSwitchEvent
#define SVC_Doorbell_Optional CHAR_Name " " CHAR_Volume " " CHAR_Brightness

#define SVC_Fan "B7"
#define SVC_Fan_Required CHAR_Active
#define SVC_Fan_Optional CHAR_Name " " CHAR_CurrentFanState " " CHAR_TargetFanState " " CHAR_RotationDirection  " " CHAR_RotationSpeed " " \
  CHAR_SwingMode " " CHAR_LockPhysicalControls

#define SVC_Faucet "D7"
#define SVC_Faucet_Required CHAR_Active
#define SVC_Faucet_Optional CHAR_StatusFault " " CHAR_Name

#define SVC_FilterMaintenance "BA"
#define SVC_FilterMaintenance_Required CHAR_FilterChangeIndication
#define SVC_FilterMaintenance_Optional CHAR_Name " " CHAR_FilterLifeLevel " " CHAR_ResetFilterIndication

#define SVC_GarageDoorOpener "41"
#define SVC_GarageDoorOpener_Required CHAR_CurrentDoorState " " CHAR_TargetDoorState " " CHAR_ObstructionDetected
#define SVC_GarageDoorOpener_Optional CHAR_LockCurrentState " " CHAR_LockTargetState " " CHAR_Name

#define SVC_HAPProtocolInformation "A2"
#define SVC_HAPProtocolInformation_Required CHAR_Version
#define SVC_HAPProtocolInformation_Optional ""

#define SVC_HeaterCooler "BC"
#define SVC_HeaterCooler_Required CHAR_Active " " CHAR_CurrentTemperature " " CHAR_CurrentHeaterCoolerState " " CHAR_TargetHeaterCoolerState " " \  
#define SVC_HeaterCooler_Optional CHAR_Name " " CHAR_RotationSpeed " " CHAR_TemperatureDisplayUnits " " CHAR_SwingMode " " CHAR_CoolingThesholdTemperature  " " \
  CHAR_HeatingThresholdTemperature " " CHAR_LockPhysicalControls


#define SVC_HumidifierDehumidifier "BD"
#define SVC_HumiditySensor "82"
#define SVC_IrrigationSystem "CF"
#define SVC_LeakSensor "83"
#define SVC_LightBulb "43"
#define SVC_LightSensor "84"
#define SVC_MotionSensor "85"
#define SVC_OccupancySensor "86"
#define SVC_Outlet "47"
#define SVC_ServiceLabel "47"
#define SVC_Slat "B9"
#define SVC_SmokeSensor "87"
#define SVC_StatelessProgrammableSwitch "89"
#define SVC_Switch "49"
#define SVC_TemperatureSensor "8A"
#define SVC_Thermostat "4A"
#define SVC_Valve "D0"
#define SVC_Window "8B"
#define SVC_WindowCovering "8C"






namespace Service {

  struct AccessoryInformation : SpanService { AccessoryInformation() : SpanService{"3E","AccessoryInformation","52,14,20,21,23,30","53"}{} };

  struct AirPurifier : SpanService { AirPurifier() : SpanService{"BB","AirPurifier","B0,A9,A8","23,29,B6,A7"}{} };

  struct AirQualitySensor : SpanService { AirQualitySensor() : SpanService{"8D","AirQualitySensor","95","23,C3,C4,C5,C6,C7,C8,75,77,7A,79"}{} };

  struct BatteryService : SpanService { BatteryService() : SpanService{"96","BatteryService","68,8F,79","23"}{} };

  struct CarbonDioxideSensor : SpanService { CarbonDioxideSensor() : SpanService{"97","CarbonDioxideSensor","92","23,75,77,7A,79,93,94"}{} };

  struct CarbonMonoxideSensor : SpanService { CarbonMonoxideSensor() : SpanService{"7F","CarbonMonoxideSensor","69","23,75,77,7A,79,90,91"}{} };

  struct ContactSensor : SpanService { ContactSensor() : SpanService{"80","ContactSensor","6A","23,75,77,7A,79"}{} };

  struct Door : SpanService { Door() : SpanService{"81","Door","6D,7C,72","23,6F,24"}{} };

  struct Doorbell : SpanService { Doorbell() : SpanService{"121","Doorbell","73","23,119,8"}{} };

  struct Fan : SpanService { Fan() : SpanService{"B7","Fan","B0","23,AF,BF,28,29,B6,A7"}{} };

  struct Faucet : SpanService { Faucet() : SpanService{"D7","Faucet","B0","23,77"}{} };

  struct FilterMaintenance : SpanService { FilterMaintenance() : SpanService{"BA","FilterMaintenance","AC","23,AB,AD"}{} };

  struct GarageDoorOpener : SpanService { GarageDoorOpener() : SpanService{"41","GarageDoorOpener","E,32,24","23,1D,1E"}{} };

  struct HAPProtocolInformation : SpanService { HAPProtocolInformation() : SpanService{"A2","HAPProtocolInformation","37"}{} };

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
