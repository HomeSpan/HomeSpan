/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2023 Gregg E. Berman
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

// Macros to define services, along with vectors of required and optional characteristics for each Span Service structure
// The names of the macros are picked up by external scripts to help generate documentation

#define CREATE_SERV(NAME,UUID) struct NAME : SpanService { NAME() : SpanService{#UUID,#NAME}{
#define END_SERV }};

#define REQ(HAPCHAR) req.insert(&hapChars.HAPCHAR)
#define OPT(HAPCHAR) opt.insert(&hapChars.HAPCHAR)

namespace Service {

  CREATE_SERV(AccessoryInformation,3E)
    REQ(Identify);
    OPT(FirmwareRevision);
    OPT(Manufacturer);
    OPT(Model);
    OPT(Name);
    OPT(SerialNumber);
    OPT(HardwareRevision);
    OPT(AccessoryFlags);    
  END_SERV

  CREATE_SERV(AirPurifier,BB)
    REQ(Active);
    REQ(CurrentAirPurifierState);
    REQ(TargetAirPurifierState);
    OPT(Name);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  END_SERV

  CREATE_SERV(AirQualitySensor,8D)
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
  END_SERV

  CREATE_SERV(BatteryService,96)
    REQ(BatteryLevel);
    REQ(ChargingState);
    REQ(StatusLowBattery);
    OPT(Name);
  END_SERV

  CREATE_SERV(CarbonDioxideSensor,97)
    REQ(CarbonDioxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonDioxideLevel);
    OPT(CarbonDioxidePeakLevel);
  END_SERV

  CREATE_SERV(CarbonMonoxideSensor,7F)
    REQ(CarbonMonoxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonMonoxideLevel);
    OPT(CarbonMonoxidePeakLevel);
    END_SERV

  CREATE_SERV(ContactSensor,80)
    REQ(ContactSensorState);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  END_SERV

  CREATE_SERV(Door,81)
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  END_SERV

  CREATE_SERV(Doorbell,121)
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(Volume);
    OPT(Brightness);
  END_SERV

  CREATE_SERV(Fan,B7)
    REQ(Active);
    OPT(Name);
    OPT(CurrentFanState);
    OPT(TargetFanState);
    OPT(RotationDirection);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  END_SERV

  CREATE_SERV(Faucet,D7)
    REQ(Active);
    OPT(StatusFault);
    OPT(Name);
  END_SERV

  CREATE_SERV(FilterMaintenance,BA)
    REQ(FilterChangeIndication);
    OPT(Name);
    OPT(FilterLifeLevel);
    OPT(ResetFilterIndication);
  END_SERV

  CREATE_SERV(GarageDoorOpener,41)
    REQ(CurrentDoorState);
    REQ(TargetDoorState);
    REQ(ObstructionDetected);
    OPT(LockCurrentState);
    OPT(LockTargetState);
    OPT(Name);
  END_SERV

  CREATE_SERV(HAPProtocolInformation,A2)
    REQ(Version);
  END_SERV

  CREATE_SERV(HeaterCooler,BC)
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
  END_SERV

  CREATE_SERV(HumidifierDehumidifier,BD)
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
  END_SERV

  CREATE_SERV(HumiditySensor,82)
    REQ(CurrentRelativeHumidity);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);   
  END_SERV

  CREATE_SERV(InputSource,D9)
      OPT(ConfiguredName);
      OPT(IsConfigured);
      REQ(Identifier);
      OPT(CurrentVisibilityState);
      OPT(TargetVisibilityState);
  END_SERV

  CREATE_SERV(IrrigationSystem,CF)
    REQ(Active);
    REQ(ProgramMode);
    REQ(InUse);
    OPT(RemainingDuration);
    OPT(StatusFault);
  END_SERV

  CREATE_SERV(LeakSensor,83)
    REQ(LeakDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  END_SERV

  CREATE_SERV(LightBulb,43)
    REQ(On);
    OPT(Brightness);
    OPT(Hue);
    OPT(Name);
    OPT(Saturation);
    OPT(ColorTemperature);
  END_SERV

  CREATE_SERV(LightSensor,84)
    REQ(CurrentAmbientLightLevel);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);          
  END_SERV

  CREATE_SERV(LockMechanism,45)
    REQ(LockCurrentState);
    REQ(LockTargetState);
    OPT(Name);
  END_SERV

  CREATE_SERV(Microphone,112)
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
  END_SERV

  CREATE_SERV(MotionSensor,85)
    REQ(MotionDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  END_SERV

  CREATE_SERV(OccupancySensor,86)
    REQ(OccupancyDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);         
  END_SERV

  CREATE_SERV(Outlet,47)
    REQ(On);
    REQ(OutletInUse);
    OPT(Name);
  END_SERV

  CREATE_SERV(SecuritySystem,7E)
    REQ(SecuritySystemCurrentState);
    REQ(SecuritySystemTargetState);
    OPT(Name);
    OPT(SecuritySystemAlarmType);
    OPT(StatusFault);
    OPT(StatusTampered);
  END_SERV  

  CREATE_SERV(ServiceLabel,CC)
    REQ(ServiceLabelNamespace);
  END_SERV  

  CREATE_SERV(Slat,B9)
    REQ(CurrentSlatState);
    REQ(SlatType);
    OPT(Name);
    OPT(SwingMode);
    OPT(CurrentTiltAngle);
    OPT(TargetTiltAngle);
  END_SERV

  CREATE_SERV(SmokeSensor,87)
    REQ(SmokeDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);             
  END_SERV

  CREATE_SERV(Speaker,113)
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
  END_SERV

  CREATE_SERV(StatelessProgrammableSwitch,89)
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(ServiceLabelIndex);
  END_SERV

  CREATE_SERV(Switch,49)
    REQ(On);
    OPT(Name);
  END_SERV

  CREATE_SERV(Television,D8)
      REQ(Active);
      OPT(ConfiguredName);
      OPT(ActiveIdentifier);
      OPT(RemoteKey);
      OPT(PowerModeSelection);      
  END_SERV

  CREATE_SERV(TelevisionSpeaker,113)
      REQ(VolumeControlType);
      REQ(VolumeSelector);      
  END_SERV

  CREATE_SERV(TemperatureSensor,8A)
    REQ(CurrentTemperature);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  END_SERV

  CREATE_SERV(Thermostat,4A)
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
  END_SERV

  CREATE_SERV(Valve,D0)
    REQ(Active);
    REQ(InUse);
    REQ(ValveType);
    OPT(SetDuration);
    OPT(RemainingDuration);
    OPT(IsConfigured);
    OPT(ServiceLabelIndex);
    OPT(StatusFault);
    OPT(Name);
  END_SERV

  CREATE_SERV(Window,8B)
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  END_SERV

  CREATE_SERV(WindowCovering,8C)
    REQ(TargetPosition);
    REQ(CurrentPosition);
    REQ(PositionState);   
    OPT(Name);
    OPT(HoldPosition);
    OPT(CurrentHorizontalTiltAngle);
    OPT(TargetHorizontalTiltAngle);
    OPT(CurrentVerticalTiltAngle);
    OPT(TargetVerticalTiltAngle);
    OPT(ObstructionDetected);
  END_SERV

}

//////////////////////////////////////////
// SPAN CHARACTERISTICS (HAP Chapter 9) //
//////////////////////////////////////////

// Macro to define Span Characteristic structures based on name of HAP Characteristic, default value, and min/max value (not applicable for STRING or BOOL which default to min=0, max=1)

#define CREATE_CHAR(TYPE,HAPCHAR,DEFVAL,MINVAL,MAXVAL) \
  struct HAPCHAR : SpanCharacteristic { HAPCHAR(TYPE val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&hapChars.HAPCHAR} { init(val,nvsStore,(TYPE)MINVAL,(TYPE)MAXVAL); } };

namespace Characteristic {

  CREATE_CHAR(uint32_t,AccessoryFlags,1,1,1);
  CREATE_CHAR(uint8_t,Active,0,0,1);
  CREATE_CHAR(uint32_t,ActiveIdentifier,0,0,255);
  CREATE_CHAR(uint8_t,AirQuality,0,0,5);
  CREATE_CHAR(uint8_t,BatteryLevel,0,0,100);
  CREATE_CHAR(int,Brightness,0,0,100);
  CREATE_CHAR(double,CarbonMonoxideLevel,0,0,100);
  CREATE_CHAR(double,CarbonMonoxidePeakLevel,0,0,100);
  CREATE_CHAR(uint8_t,CarbonMonoxideDetected,0,0,1);
  CREATE_CHAR(double,CarbonDioxideLevel,0,0,100000);
  CREATE_CHAR(double,CarbonDioxidePeakLevel,0,0,100000);
  CREATE_CHAR(uint8_t,CarbonDioxideDetected,0,0,1);
  CREATE_CHAR(uint8_t,ChargingState,0,0,2);
  CREATE_CHAR(uint8_t,ClosedCaptions,0,0,1);
  CREATE_CHAR(double,CoolingThresholdTemperature,10,10,35); 
  CREATE_CHAR(uint32_t,ColorTemperature,200,140,500);
  CREATE_CHAR(uint8_t,ContactSensorState,1,0,1);
  CREATE_CHAR(const char *,ConfiguredName,"unnamed",0,1);
  CREATE_CHAR(double,CurrentAmbientLightLevel,1,0.0001,100000);
  CREATE_CHAR(int,CurrentHorizontalTiltAngle,0,-90,90);
  CREATE_CHAR(uint8_t,CurrentAirPurifierState,1,0,2);
  CREATE_CHAR(uint8_t,CurrentSlatState,0,0,2);
  CREATE_CHAR(uint8_t,CurrentPosition,0,0,100);
  CREATE_CHAR(int,CurrentVerticalTiltAngle,0,-90,90);
  CREATE_CHAR(uint8_t,CurrentVisibilityState,0,0,1);
  CREATE_CHAR(uint8_t,CurrentHumidifierDehumidifierState,1,0,3);
  CREATE_CHAR(uint8_t,CurrentDoorState,1,0,4);
  CREATE_CHAR(uint8_t,CurrentFanState,1,0,2);
  CREATE_CHAR(uint8_t,CurrentHeatingCoolingState,0,0,2);
  CREATE_CHAR(uint8_t,CurrentHeaterCoolerState,1,0,3);
  CREATE_CHAR(uint8_t,CurrentMediaState,0,0,5);
  CREATE_CHAR(double,CurrentRelativeHumidity,0,0,100);
  CREATE_CHAR(double,CurrentTemperature,0,0,100);
  CREATE_CHAR(int,CurrentTiltAngle,0,-90,90);
  CREATE_CHAR(double,FilterLifeLevel,0,0,100);
  CREATE_CHAR(uint8_t,FilterChangeIndication,0,0,1);
  CREATE_CHAR(const char *,FirmwareRevision,"1.0.0",0,1);
  CREATE_CHAR(const char *,HardwareRevision,"1.0.0",0,1);
  CREATE_CHAR(double,HeatingThresholdTemperature,16,0,25);
  CREATE_CHAR(boolean,HoldPosition,false,0,1);
  CREATE_CHAR(double,Hue,0,0,360);
  CREATE_CHAR(boolean,Identify,false,0,1);
  CREATE_CHAR(uint32_t,Identifier,0,0,255);
  CREATE_CHAR(uint8_t,InputDeviceType,0,0,6);
  CREATE_CHAR(uint8_t,InputSourceType,0,0,10);
  CREATE_CHAR(uint8_t,InUse,0,0,1);
  CREATE_CHAR(uint8_t,IsConfigured,0,0,1);
  CREATE_CHAR(uint8_t,LeakDetected,0,0,1);
  CREATE_CHAR(uint8_t,LockCurrentState,0,0,3);
  CREATE_CHAR(uint8_t,LockPhysicalControls,0,0,1);
  CREATE_CHAR(uint8_t,LockTargetState,0,0,1);
  CREATE_CHAR(const char *,Manufacturer,"HomeSpan",0,1);
  CREATE_CHAR(const char *,Model,"HomeSpan-ESP32",0,1);
  CREATE_CHAR(boolean,MotionDetected,false,0,1);
  CREATE_CHAR(boolean,Mute,false,0,1);
  CREATE_CHAR(const char *,Name,"unnamed",0,1);
  CREATE_CHAR(double,NitrogenDioxideDensity,0,0,1000);
  CREATE_CHAR(boolean,ObstructionDetected,false,0,1);
  CREATE_CHAR(double,PM25Density,0,0,1000);
  CREATE_CHAR(uint8_t,OccupancyDetected,0,0,1);
  CREATE_CHAR(boolean,OutletInUse,false,0,1);
  CREATE_CHAR(boolean,On,false,0,1);
  CREATE_CHAR(double,OzoneDensity,0,0,1000);
  CREATE_CHAR(uint8_t,PictureMode,0,0,13);
  CREATE_CHAR(double,PM10Density,0,0,1000);
  CREATE_CHAR(uint8_t,PositionState,2,0,2);
  CREATE_CHAR(uint8_t,PowerModeSelection,0,0,1);
  CREATE_CHAR(uint8_t,ProgramMode,0,0,2);
  CREATE_CHAR(uint8_t,ProgrammableSwitchEvent,0,0,2);
  CREATE_CHAR(double,RelativeHumidityDehumidifierThreshold,50,0,100);
  CREATE_CHAR(double,RelativeHumidityHumidifierThreshold,50,0,100);
  CREATE_CHAR(uint32_t,RemainingDuration,60,0,3600);
  CREATE_CHAR(uint8_t,RemoteKey,0,0,16);
  CREATE_CHAR(uint8_t,ResetFilterIndication,0,1,1);
  CREATE_CHAR(int,RotationDirection,0,0,1);
  CREATE_CHAR(double,RotationSpeed,0,0,100);
  CREATE_CHAR(double,Saturation,0,0,100);
  CREATE_CHAR(uint8_t,SecuritySystemAlarmType,0,0,1);
  CREATE_CHAR(uint8_t,SecuritySystemCurrentState,3,0,4);
  CREATE_CHAR(uint8_t,SecuritySystemTargetState,3,0,3); 
  CREATE_CHAR(const char *,SerialNumber,"HS-12345",0,1);
  CREATE_CHAR(uint8_t,ServiceLabelIndex,1,1,255);
  CREATE_CHAR(uint8_t,ServiceLabelNamespace,1,0,1);
  CREATE_CHAR(uint8_t,SlatType,0,0,1);
  CREATE_CHAR(uint8_t,SleepDiscoveryMode,0,0,1);
  CREATE_CHAR(uint8_t,SmokeDetected,0,0,1);
  CREATE_CHAR(boolean,StatusActive,true,0,1);
  CREATE_CHAR(uint8_t,StatusFault,0,0,1);
  CREATE_CHAR(uint8_t,StatusJammed,0,0,1);
  CREATE_CHAR(uint8_t,StatusLowBattery,0,0,1);
  CREATE_CHAR(uint8_t,StatusTampered,0,0,1);
  CREATE_CHAR(double,SulphurDioxideDensity,0,0,1000);
  CREATE_CHAR(uint8_t,SwingMode,0,0,1);
  CREATE_CHAR(uint8_t,TargetAirPurifierState,1,0,1);
  CREATE_CHAR(uint8_t,TargetFanState,1,0,1);
  CREATE_CHAR(int,TargetTiltAngle,0,-90,90);
  CREATE_CHAR(uint8_t,TargetHeaterCoolerState,0,0,2);
  CREATE_CHAR(uint32_t,SetDuration,60,0,3600);
  CREATE_CHAR(int,TargetHorizontalTiltAngle,0,-90,90);
  CREATE_CHAR(uint8_t,TargetHumidifierDehumidifierState,0,0,2);
  CREATE_CHAR(uint8_t,TargetPosition,0,0,100);
  CREATE_CHAR(uint8_t,TargetDoorState,1,0,1);
  CREATE_CHAR(uint8_t,TargetHeatingCoolingState,0,0,3);
  CREATE_CHAR(uint8_t,TargetMediaState,0,0,2);
  CREATE_CHAR(double,TargetRelativeHumidity,0,0,100);
  CREATE_CHAR(double,TargetTemperature,16,10,38);
  CREATE_CHAR(uint8_t,TargetVisibilityState,0,0,1);
  CREATE_CHAR(uint8_t,TemperatureDisplayUnits,0,0,1);
  CREATE_CHAR(int,TargetVerticalTiltAngle,0,-90,90);
  CREATE_CHAR(uint8_t,ValveType,0,0,3);
  CREATE_CHAR(const char *,Version,"1.0.0",0,1);
  CREATE_CHAR(double,VOCDensity,0,0,1000);
  CREATE_CHAR(uint8_t,Volume,0,0,100);
  CREATE_CHAR(uint8_t,VolumeControlType,0,0,3);
  CREATE_CHAR(uint8_t,VolumeSelector,0,0,1);
  CREATE_CHAR(double,WaterLevel,0,0,100);

}

////////////////////////////////////////////////////////
// MACROS TO ADD CUSTOM SERVICES AND CHARACTERISTICS  //
////////////////////////////////////////////////////////

#ifndef CUSTOM_CHAR_HEADER

#define CUSTOM_CHAR(NAME,UUID,PERMISISONS,FORMAT,DEFVAL,MINVAL,MAXVAL,STATIC_RANGE) \
  HapChar _CUSTOM_##NAME {#UUID,#NAME,(PERMS)(PERMISISONS),FORMAT,STATIC_RANGE}; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(FORMAT##_t val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore,(FORMAT##_t)MINVAL,(FORMAT##_t)MAXVAL); } }; }

#define CUSTOM_CHAR_STRING(NAME,UUID,PERMISISONS,DEFVAL) \
  HapChar _CUSTOM_##NAME {#UUID,#NAME,(PERMS)(PERMISISONS),STRING,true}; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(const char * val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore); } }; }

#define CUSTOM_CHAR_DATA(NAME,UUID,PERMISISONS) \
  HapChar _CUSTOM_##NAME {#UUID,#NAME,(PERMS)(PERMISISONS),DATA,true}; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(const char * val="AA==", boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore); } }; }

#else

#define CUSTOM_CHAR(NAME,UUID,PERMISISONS,FORMAT,DEFVAL,MINVAL,MAXVAL,STATIC_RANGE) \
  extern HapChar _CUSTOM_##NAME; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(FORMAT##_t val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore,(FORMAT##_t)MINVAL,(FORMAT##_t)MAXVAL); } }; }

#define CUSTOM_CHAR_STRING(NAME,UUID,PERMISISONS,DEFVAL) \
  extern HapChar _CUSTOM_##NAME; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(const char * val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore); } }; }

#define CUSTOM_CHAR_DATA(NAME,UUID,PERMISISONS) \
  extern HapChar _CUSTOM_##NAME; \
  namespace Characteristic { struct NAME : SpanCharacteristic { NAME(const char * val="AA==", boolean nvsStore=false) : SpanCharacteristic {&_CUSTOM_##NAME,true} { init(val,nvsStore); } }; }

#endif

#define CUSTOM_SERV(NAME,UUID) \
  namespace Service { struct NAME : SpanService { NAME() : SpanService{#UUID,#NAME,true}{} }; }


////////////////////////////////////////////////////////
// MACROS TO ADD A NEW ACCESSORT WITH OPTIONAL NAME   //
////////////////////////////////////////////////////////

#define SPAN_ACCESSORY(...)    new SpanAccessory();  new Service::AccessoryInformation(); new Characteristic::Identify(); __VA_OPT__(new Characteristic::Name(__VA_ARGS__));




  
