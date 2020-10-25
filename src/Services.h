
//////////////////////////////////
// HAP SERVICES (HAP Chapter 8) //
//////////////////////////////////

#define REQ(name) req.push_back(&homeSpan.chr.name)
#define OPT(name) opt.push_back(&homeSpan.chr.name)

namespace Service {

  struct AccessoryInformation : SpanService { AccessoryInformation() : SpanService{"3E","AccessoryInformation"}{
    REQ(FirmwareRevision);
    REQ(Identify);
    REQ(Manufacturer);
    REQ(Model);
    REQ(Name);
    REQ(SerialNumber);
    OPT(HardwareRevision);      
  }};

  struct AirPurifier : SpanService { AirPurifier() : SpanService{"BB","AirPurifier"}{
    REQ(Active);
    REQ(CurrentAirPurifierState);
    REQ(TargetAirPurifierState);
    OPT(Name);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  }};

  struct AirQualitySensor : SpanService { AirQualitySensor() : SpanService{"8D","AirQualitySensor"}{
    REQ(AirQuality);
    OPT(Name);
    OPT(OzoneDensity);
    OPT(NitrogenDioxideDensity);
    OPT(SulphurDioxideDensity);
    OPT(PM25Density);
    OPT(PM10Density);
    OPT(VOCDensity);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  }};

  struct BatteryService : SpanService { BatteryService() : SpanService{"96","BatteryService"}{
    REQ(BatteryLevel);
    REQ(ChargingState);
    REQ(StatusLowBattery);
    OPT(Name);
  }};

  struct CarbonDioxideSensor : SpanService { CarbonDioxideSensor() : SpanService{"97","CarbonDioxideSensor"}{
    REQ(CarbonDioxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonDioxideLevel);
    OPT(CarbonDioxidePeakLevel);
  }};

  struct CarbonMonoxideSensor : SpanService { CarbonMonoxideSensor() : SpanService{"7F","CarbonMonoxideSensor"}{
    REQ(CarbonMonoxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonMonoxideLevel);
    OPT(CarbonMonoxidePeakLevel);
    }};

  struct ContactSensor : SpanService { ContactSensor() : SpanService{"80","ContactSensor"}{
    REQ(ContactSensorState);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  }};

  struct Door : SpanService { Door() : SpanService{"81","Door"}{
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  }};

  struct Doorbell : SpanService { Doorbell() : SpanService{"121","Doorbell"}{
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(Volume);
    OPT(Brightness);
  }};

  struct Fan : SpanService { Fan() : SpanService{"B7","Fan"}{
    REQ(Active);
    OPT(Name);
    OPT(CurrentFanState);
    OPT(TargetFanState);
    OPT(RotationDirection);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  }};

  struct Faucet : SpanService { Faucet() : SpanService{"D7","Faucet"}{
    REQ(Active);
    OPT(StatusFault);
    OPT(Name);
  }};

  struct FilterMaintenance : SpanService { FilterMaintenance() : SpanService{"BA","FilterMaintenance"}{
    REQ(FilterChangeIndication);
    OPT(Name);
    OPT(FilterLifeLevel);
    OPT(ResetFilterIndication);
  }};

  struct GarageDoorOpener : SpanService { GarageDoorOpener() : SpanService{"41","GarageDoorOpener"}{
    REQ(CurrentDoorState);
    REQ(TargetDoorState);
    REQ(ObstructionDetected);
    OPT(LockCurrentState);
    OPT(LockTargetState);
    OPT(Name);
  }};

  struct HAPProtocolInformation : SpanService { HAPProtocolInformation() : SpanService{"A2","HAPProtocolInformation"}{
    REQ(Version);
  }};

  struct HeaterCooler : SpanService { HeaterCooler() : SpanService{"BC","HeaterCooler"}{
    REQ(Active);
    REQ(CurrentTemperature);
    REQ(CurrentHeaterCoolerState);
    REQ(TargetHeaterCoolerState);
    OPT(Name);
    OPT(RotationSpeed);
    OPT(TemperatureDisplayUnits);
    OPT(SwingMode);
    OPT(CoolingThesholdTemperature)
    OPT(HeatingThresholdTemperature);
    OPT(CHAR_LockPhysicalControls);
  }};

  struct HumidifierDehumidifier : SpanService { HumidifierDehumidifier() : SpanService{"BD","HumidifierDehumidifier"}{
    REQ(Active);
    REQ(CurrentRelativeHumidity);
    REQ(CurrentHumidifierDehumidifierState);
    REQ(TargetHumidifierDehumidifierState);
    OPT(Name);
    OPT(RelativeHumidityDehumidifierThreshold);
    OPT(RelativeHumidityHumnidifierThreshold);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(WaterLevel);
    OPT(LockPhysicalControls);
  }};

  struct HumiditySensor : SpanService { HumiditySensor() : SpanService{"82","HumiditySensor"}{
    REQ(CurrentRelativeHumidity);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);   
  }};

  struct IrrigationSystem : SpanService { IrrigationSystem() : SpanService{"CF","IrrigationSystem"}{
    REQ(Active);
    REQ(ProgramMode);
    REQ(InUse);
    OPT(RemainingDuration);
    OPT(Name);
    OPT(StatusFault);
  }};

  struct LeakSensor : SpanService { LeakSensor() : SpanService{"83","LeakSensor"}{
    REQ(LeakDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  }};

  struct LightBulb : SpanService { LightBulb() : SpanService{"43","LightBulb"}{
    REQ(On);
    OPT(Brightness);
    OPT(Hue);
    OPT(Name);
    OPT(Saturation);
    OPT(ColorTemperature);
  }};

  struct LightSensor : SpanService { LightSensor() : SpanService{"84","LightSensor"}{
    REQ(CurrentAmbientLightLevel);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);          
  }};

  struct MotionSensor : SpanService { MotionSensor() : SpanService{"85","MotionSensor"}{
    REQ(MotionDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  }};

  struct OccupancySensor : SpanService { OccupancySensor() : SpanService{"86","OccupancySensor"}{
    REQ(OccupancyDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);         
  }};

  struct Outlet : SpanService { Outlet() : SpanService{"47","Outlet"}{
    REQ(On);
    REQ(OutletInUse);
    OPT(Name);
  }};

  struct ServiceLabel : SpanService { ServiceLabel() : SpanService{"47","ServiceLabel"}{
    REQ(ServiceLabelNamespace);
  }};  

  struct Slat : SpanService { Slat() : SpanService{"B9","Slat"}{
    REQ(CurrentSlatState);
    REQ(SlatType);
    OPT(Name);
    OPT(SwingMode);
    OPT(CurrentTiltAngle);
    OPT(TargetTiltAngle);
  }};

  struct SmokeSensor : SpanService { SmokeSensor() : SpanService{"87","SmokeSensor"}{
    REQ(SmokeDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);             
  }};

  struct StatelessProgrammableSwitch : SpanService { StatelessProgrammableSwitch() : SpanService{"89","StatelessProgrammableSwitch"}{
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(ServiceLabelIndex);
  }};

  struct Switch : SpanService { Switch() : SpanService{"49","Switch"}{
    REQ(On);
    OPT(Name);
  }};

  struct TemperatureSensor : SpanService { TemperatureSensor() : SpanService{"8A","TemperatureSensor"}{
    REQ(CurrentTemperature);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  }};

  struct Thermostat : SpanService { Thermostat() : SpanService{"4A","Thermostat"}{
    REQ(CurrentHeatingCoolingState);
    REQ(TargetHeatingColingState);
    REQ(CurrentTemperature);
    REQ(TargetTemperature);
    REQ(TemperatureDisplayUnits);
    OPT(CoolingThresholdTemperature);
    OPT(CurrentRelativeHumidity);
    OPT(HeatingThresholdTemperature);
    OPT(Name);
    OPT(TargetRelativeHumidity);
  }};

  struct Valve : SpanService { Valve() : SpanService{"D0","Valve"}{
    REQ(Active);
    REQ(InUse);
    REQ(ValveType);
    OPT(SetDuration);
    OPT(RemainingDuration);
    OPT(IsConfigured);
    OPT(ServiceLabelIndex);
    OPT(StatusFault);
    OPT(Name);
  }};

  struct Window : SpanService { Window() : SpanService{"8B","Window"}{
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  }};

  struct WindowCovering : SpanService { WindowCovering() : SpanService{"8C","WindowCovering"}{
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(CurrentHorizontalTiltAngle);
    OPT(TargetHorizontalTiltAngle);
    OPT(CurrentVerticalTiltAngle);
    OPT(TargetVerticalTiltAngle);
    OPT(ObstructionDetected);
  }};
  
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
