/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2021 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
///////////////////////////////////
// SPAN SERVICES (HAP Chapter 8) //
///////////////////////////////////

// Macros to define vectors of required and optional characteristics for each Span Service structure

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
    OPT(CoolingThresholdTemperature);
    OPT(HeatingThresholdTemperature);
    OPT(LockPhysicalControls);
  }};

  struct HumidifierDehumidifier : SpanService { HumidifierDehumidifier() : SpanService{"BD","HumidifierDehumidifier"}{
    REQ(Active);
    REQ(CurrentRelativeHumidity);
    REQ(CurrentHumidifierDehumidifierState);
    REQ(TargetHumidifierDehumidifierState);
    OPT(Name);
    OPT(RelativeHumidityDehumidifierThreshold);
    OPT(RelativeHumidityHumidifierThreshold);
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

  struct LockMechanism : SpanService { LockMechanism() : SpanService{"45","LockMechanism"}{
    REQ(LockCurrentState);
    REQ(LockTargetState);
    OPT(Name);
  }};

  struct Microphone : SpanService { Microphone() : SpanService{"112","Microphone"}{
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
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

  struct SecuritySystem : SpanService { SecuritySystem() : SpanService{"7E","SecuritySystem"}{
    REQ(SecuritySystemCurrentState);
    REQ(SecuritySystemTargetState);
    OPT(Name);
    OPT(SecuritySystemAlarmType);
    OPT(StatusFault);
    OPT(StatusTampered);
  }};  

  struct ServiceLabel : SpanService { ServiceLabel() : SpanService{"CC","ServiceLabel"}{
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

  struct Speaker : SpanService { Speaker() : SpanService{"113","Speaker"}{
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
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
    REQ(TargetHeatingCoolingState);
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
    REQ(TargetPosition);
    REQ(CurrentPosition);
    OPT(PositionState);   
    OPT(Name);
    OPT(HoldPosition);
    OPT(CurrentHorizontalTiltAngle);
    OPT(TargetHorizontalTiltAngle);
    OPT(CurrentVerticalTiltAngle);
    OPT(TargetVerticalTiltAngle);
    OPT(ObstructionDetected);
  }};

}

//////////////////////////////////////////
// SPAN CHARACTERISTICS (HAP Chapter 9) //
//////////////////////////////////////////

// Macro to define Span Characteristic structures based on name of HAP Characteristic (see HAPConstants.h), its type (e.g. int, double) and its default value

#define CREATE_CHAR(CHR,TYPE,DEFVAL) struct CHR : SpanCharacteristic { CHR(TYPE value=DEFVAL) : SpanCharacteristic{homeSpan.chr.CHR.id, homeSpan.chr.CHR.perms,(TYPE)value, homeSpan.chr.CHR.name}{} }

namespace Characteristic {
  
  CREATE_CHAR(Active,uint8_t,0);
  CREATE_CHAR(AirQuality,uint8_t,0);
  CREATE_CHAR(BatteryLevel,uint8_t,0);
  CREATE_CHAR(Brightness,int,0);
  CREATE_CHAR(CarbonMonoxideLevel,double,0);
  CREATE_CHAR(CarbonMonoxidePeakLevel,double,0);
  CREATE_CHAR(CarbonMonoxideDetected,uint8_t,0);
  CREATE_CHAR(CarbonDioxideLevel,double,0);
  CREATE_CHAR(CarbonDioxidePeakLevel,double,0);
  CREATE_CHAR(CarbonDioxideDetected,uint8_t,0);
  CREATE_CHAR(ChargingState,uint8_t,0);
  CREATE_CHAR(CoolingThresholdTemperature,double,10); 
  CREATE_CHAR(ColorTemperature,uint32_t,50);
  CREATE_CHAR(ContactSensorState,uint8_t,1);
  CREATE_CHAR(CurrentAmbientLightLevel,double,1);
  CREATE_CHAR(CurrentHorizontalTiltAngle,int,0);
  CREATE_CHAR(CurrentAirPurifierState,uint8_t,1);
  CREATE_CHAR(CurrentSlatState,uint8_t,0);
  CREATE_CHAR(CurrentPosition,uint8_t,0);
  CREATE_CHAR(CurrentVerticalTiltAngle,int,0);
  CREATE_CHAR(CurrentHumidifierDehumidifierState,uint8_t,1);
  CREATE_CHAR(CurrentDoorState,uint8_t,1);
  CREATE_CHAR(CurrentFanState,uint8_t,1);
  CREATE_CHAR(CurrentHeatingCoolingState,uint8_t,0);
  CREATE_CHAR(CurrentHeaterCoolerState,uint8_t,1);
  CREATE_CHAR(CurrentRelativeHumidity,double,0);
  CREATE_CHAR(CurrentTemperature,double,0);
  CREATE_CHAR(CurrentTiltAngle,int,0);
  CREATE_CHAR(FilterLifeLevel,double,0);
  CREATE_CHAR(FilterChangeIndication,uint8_t,0);
  CREATE_CHAR(FirmwareRevision,const char *,"1.0.0");
  CREATE_CHAR(HardwareRevision,const char *,"1.0.0");
  CREATE_CHAR(HeatingThresholdTemperature,double,16);
  CREATE_CHAR(HoldPosition,boolean,false);
  CREATE_CHAR(Hue,double,0);
  CREATE_CHAR(Identify,boolean,false);
  CREATE_CHAR(InUse,uint8_t,0);
  CREATE_CHAR(IsConfigured,uint8_t,0);
  CREATE_CHAR(LeakDetected,uint8_t,0);
  CREATE_CHAR(LockCurrentState,uint8_t,0);
  CREATE_CHAR(LockPhysicalControls,uint8_t,0);
  CREATE_CHAR(LockTargetState,uint8_t,0);
  CREATE_CHAR(Manufacturer,const char *,"HomeSpan");
  CREATE_CHAR(Model,const char *,"HomeSpan-ESP32");
  CREATE_CHAR(MotionDetected,boolean,false);
  CREATE_CHAR(Mute,boolean,false);
  CREATE_CHAR(Name,const char *,"unnamed");
  CREATE_CHAR(NitrogenDioxideDensity,double,0);
  CREATE_CHAR(ObstructionDetected,boolean,false);
  CREATE_CHAR(PM25Density,double,0);
  CREATE_CHAR(OccupancyDetected,uint8_t,0);
  CREATE_CHAR(OutletInUse,boolean,false);
  CREATE_CHAR(On,boolean,false);
  CREATE_CHAR(OzoneDensity,double,0);
  CREATE_CHAR(PM10Density,double,0);
  CREATE_CHAR(PositionState,uint8_t,2);
  CREATE_CHAR(ProgramMode,uint8_t,0);
  CREATE_CHAR(ProgrammableSwitchEvent,uint8_t,0);
  CREATE_CHAR(RelativeHumidityDehumidifierThreshold,double,50);
  CREATE_CHAR(RelativeHumidityHumidifierThreshold,double,50);
  CREATE_CHAR(RemainingDuration,uint32_t,60);
  CREATE_CHAR(ResetFilterIndication,uint8_t,0);
  CREATE_CHAR(RotationDirection,int,0);
  CREATE_CHAR(RotationSpeed,double,0);
  CREATE_CHAR(Saturation,double,0);
  CREATE_CHAR(SecuritySystemAlarmType,uint8_t,0);
  CREATE_CHAR(SecuritySystemCurrentState,uint8_t,3);
  CREATE_CHAR(SecuritySystemTargetState,uint8_t,3); 
  CREATE_CHAR(SerialNumber,const char *,"HS-12345");
  CREATE_CHAR(ServiceLabelIndex,uint8_t,1);
  CREATE_CHAR(ServiceLabelNamespace,uint8_t,1);
  CREATE_CHAR(SlatType,uint8_t,0);
  CREATE_CHAR(SmokeDetected,uint8_t,0);
  CREATE_CHAR(StatusActive,boolean,true);
  CREATE_CHAR(StatusFault,uint8_t,0);
  CREATE_CHAR(StatusJammed,uint8_t,0);
  CREATE_CHAR(StatusLowBattery,uint8_t,0);
  CREATE_CHAR(StatusTampered,uint8_t,0);
  CREATE_CHAR(SulphurDioxideDensity,double,0);
  CREATE_CHAR(SwingMode,uint8_t,0);
  CREATE_CHAR(TargetAirPurifierState,uint8_t,1);
  CREATE_CHAR(TargetFanState,uint8_t,1);
  CREATE_CHAR(TargetTiltAngle,int,0);
  CREATE_CHAR(SetDuration,uint32_t,60);
  CREATE_CHAR(TargetHorizontalTiltAngle,int,0);
  CREATE_CHAR(TargetHumidifierDehumidifierState,uint8_t,0);
  CREATE_CHAR(TargetPosition,uint8_t,0);
  CREATE_CHAR(TargetDoorState,uint8_t,1);
  CREATE_CHAR(TargetHeatingCoolingState,uint8_t,0);
  CREATE_CHAR(TargetRelativeHumidity,double,0);
  CREATE_CHAR(TargetTemperature,double,16);
  CREATE_CHAR(TemperatureDisplayUnits,uint8_t,0);
  CREATE_CHAR(TargetVerticalTiltAngle,int,0);
  CREATE_CHAR(ValveType,uint8_t,0);
  CREATE_CHAR(Version,const char *,"1.0.0");
  CREATE_CHAR(VOCDensity,double,0);
  CREATE_CHAR(Volume,uint8_t,0);
  CREATE_CHAR(WaterLevel,double,0);

}
