/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2024 Gregg E. Berman
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

// Note: These macros below are also parsed by an external awk script to auto-generate Services and Characteristics documentation.
//
// The CREATE_SERV_DEP() macro is the same as the CREATE_SERV() macro, except that it is used for deprecated Services that will not
// be included in documentation. The OPT_DEP() macro is that same as the OPT() macro, except that it is used for deprecated Characteristics
// that will not be included in documentation.

#define CREATE_SERV(NAME,UUID) struct NAME : SpanService { NAME() : SpanService{#UUID,#NAME}{
#define CREATE_SERV_DEP(NAME,UUID) struct NAME : SpanService { NAME() : SpanService{#UUID,#NAME}{
#define END_SERV }};

#define REQ(HAPCHAR) req.push_back(&hapChars.HAPCHAR)
#define REQ_DEP(HAPCHAR) req.push_back(&hapChars.HAPCHAR)
#define OPT(HAPCHAR) opt.push_back(&hapChars.HAPCHAR)
#define OPT_DEP(HAPCHAR) opt.push_back(&hapChars.HAPCHAR)

#define SERVICES_GROUP

namespace Service {

  SERVICES_GROUP // Mandatory Services
  
  CREATE_SERV(AccessoryInformation,3E)  // Required Identification Information.  For each Accessory in a HomeSpan device this must be included as the first Service.
    REQ(Identify);
    OPT(Name);
    OPT(FirmwareRevision);
    OPT(Manufacturer);
    OPT(Model);
    OPT(SerialNumber);
    OPT(HardwareRevision);
    OPT_DEP(AccessoryFlags);
  END_SERV

  SERVICES_GROUP // Lights, Power, and Switches

  CREATE_SERV(BatteryService,96)  // Defines a standalone Battery Service.
    REQ(BatteryLevel);
    REQ(ChargingState);
    REQ(StatusLowBattery);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV  

  CREATE_SERV(LightBulb,43)   // Defines any type of Light.
    REQ(On);
    OPT(Brightness);
    OPT(Hue);
    OPT(Saturation);
    OPT(ColorTemperature);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(Outlet,47)    // Defines a controllable Outlet used to power any light or appliance.
    REQ(On);
    REQ(OutletInUse);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(StatelessProgrammableSwitch,89)   // Defines a "Stateless" Programmable Switch that can be used to trigger actions in the Home App.
    REQ(ProgrammableSwitchEvent);
    OPT(ServiceLabelIndex);
    OPT_DEP(Name);
  END_SERV
  
  CREATE_SERV(Switch,49)    // Defines a generic Switch.
    REQ(On);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV
 
  SERVICES_GROUP // Heating, Ventilation, and Air Conditioning (HVAC)
  
  CREATE_SERV(AirPurifier,BB)   // Defines a basic Air Purifier with an optional fan and swing mode.  Optional Linked Services: <b>FilterMaintenance</b>.  Combine with an <b>AirSensor</b> Service for automated operations.
    REQ(Active);
    REQ(CurrentAirPurifierState);
    REQ(TargetAirPurifierState);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV  

  CREATE_SERV(Fan,B7)     // Defines a Fan.  Combine with a <b>LightBulb</b> Service to create a Lighted Ceiling Fan.
    REQ(Active);
    OPT(CurrentFanState);
    OPT(TargetFanState);
    OPT(RotationDirection);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(FilterMaintenance,BA)   // Defines a Filter Maintainence check.  Use only as a Linked Service for the <b>AirPurifier</b> Service.
    REQ(FilterChangeIndication);
    OPT(FilterLifeLevel);
    OPT(ResetFilterIndication);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV  

  CREATE_SERV(HeaterCooler,BC)  // Defines a standalone Heater, Cooler, or combined Heater/Cooler.
    REQ(Active);
    REQ(CurrentTemperature);
    REQ(CurrentHeaterCoolerState);
    REQ(TargetHeaterCoolerState);
    OPT(RotationSpeed);
    OPT(TemperatureDisplayUnits);
    OPT(SwingMode);
    OPT(CoolingThresholdTemperature);
    OPT(HeatingThresholdTemperature);
    OPT(LockPhysicalControls);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(HumidifierDehumidifier,BD)  // Defines a Humidifer, Dehumidifier, or combined Humidifer/Dehumidifier.
    REQ(Active);
    REQ(CurrentRelativeHumidity);
    REQ(CurrentHumidifierDehumidifierState);
    REQ(TargetHumidifierDehumidifierState);
    OPT(RelativeHumidityDehumidifierThreshold);
    OPT(RelativeHumidityHumidifierThreshold);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(WaterLevel);
    OPT(LockPhysicalControls);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(Slat,B9)    // Defines a motorized ventilation Slat(s).
    REQ(CurrentSlatState);
    REQ(SlatType);
    OPT(SwingMode);
    OPT(CurrentTiltAngle);
    OPT(TargetTiltAngle);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(Thermostat,4A)      // Defines a Thermostat used to control a furnace, air conditioner, or both.
    REQ(CurrentHeatingCoolingState);
    REQ(TargetHeatingCoolingState);
    REQ(CurrentTemperature);
    REQ(TargetTemperature);
    REQ(TemperatureDisplayUnits);
    OPT(CoolingThresholdTemperature);
    OPT(CurrentRelativeHumidity);
    OPT(HeatingThresholdTemperature);
    OPT(TargetRelativeHumidity);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV  
  
  SERVICES_GROUP // Standalone Sensors

  CREATE_SERV(AirQualitySensor,8D)  // Defines an Air Quality Sensor. 
    REQ(AirQuality);
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
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV
  
  CREATE_SERV(CarbonDioxideSensor,97) // Defines a Carbon Dioxide Sensor.
    REQ(CarbonDioxideDetected);
    OPT(CarbonDioxideLevel);
    OPT(CarbonDioxidePeakLevel);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(CarbonMonoxideSensor,7F)  // Defines a Carbon Monoxide Sensor.
    REQ(CarbonMonoxideDetected);
    OPT(CarbonMonoxideLevel);
    OPT(CarbonMonoxidePeakLevel);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(ConfiguredName);
    OPT_DEP(Name);
    END_SERV

  CREATE_SERV(ContactSensor,80)   // Defines a Contact Sensor.
    REQ(ContactSensorState);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(HumiditySensor,82)    // Defines a Humidity Sensor.
    REQ(CurrentRelativeHumidity);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);   
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(LeakSensor,83)    // Defines a Leak Sensor.
    REQ(LeakDetected);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(LightSensor,84)   // Defines a Light Sensor.
    REQ(CurrentAmbientLightLevel);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);          
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(MotionSensor,85)    // Defines a Motion Sensor.
    REQ(MotionDetected);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(OccupancySensor,86)   // Defines and Occupancy Sensor.
    REQ(OccupancyDetected);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);         
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(SmokeSensor,87)   // Defines a Smoke Sensor.
    REQ(SmokeDetected);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);             
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(TemperatureSensor,8A)   // Defines a Temperature Sensor.
    REQ(CurrentTemperature);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(ConfiguredName);
  END_SERV       
  
  SERVICES_GROUP // Doors, Locks, and Windows

  CREATE_SERV(Door,81)    // Defines a motorized Door.
    REQ(CurrentPosition);
    REQ(TargetPosition);
    OPT(ObstructionDetected);
    OPT(ConfiguredName);
    OPT_DEP(Name);
    OPT_DEP(PositionState);
    OPT_DEP(HoldPosition);
  END_SERV

  CREATE_SERV(Doorbell,121)   // Defines a Doorbell.  Can be used on a standalone basis or in conjunction with a <b>LockMechanism</b> Service.
    REQ(ProgrammableSwitchEvent);
    OPT_DEP(Volume);
    OPT_DEP(Brightness);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(GarageDoorOpener,41)  // Defines a motorized Garage Door Opener.
    REQ(CurrentDoorState);
    REQ(TargetDoorState);
    REQ(ObstructionDetected);
    OPT(LockCurrentState);
    OPT(LockTargetState);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(LockMechanism,45)   // Defines an electronic Lock.
    REQ(LockCurrentState);
    REQ(LockTargetState);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(Window,8B)    // Defines a motorized Window.
    REQ(CurrentPosition);
    REQ(TargetPosition);
    OPT(ObstructionDetected);
    OPT(ConfiguredName);
    OPT_DEP(Name);
    OPT_DEP(PositionState);
    OPT_DEP(HoldPosition);
  END_SERV

  CREATE_SERV(WindowCovering,8C)  // Defines a motorized Window Shade, Screen, Awning, etc.
    REQ(TargetPosition);
    REQ(CurrentPosition);
    OPT(CurrentHorizontalTiltAngle);
    OPT(TargetHorizontalTiltAngle);
    OPT(CurrentVerticalTiltAngle);
    OPT(TargetVerticalTiltAngle);
    OPT(ObstructionDetected);
    OPT(ConfiguredName);
    OPT_DEP(Name);
    OPT_DEP(PositionState);   
    OPT_DEP(HoldPosition);
  END_SERV   

  SERVICES_GROUP // Water Systems

  CREATE_SERV(Faucet,D7)    // Defines the master control for a multi-Valve appliance.  Linked Services: <b>Valve</b> (at least one required), and <b>HeaterCooler</b> (optional).
    REQ(Active);
    OPT(StatusFault);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(IrrigationSystem,CF)  // Defines an Irrigation System.  Linked Services: <b>Valve</b> Service (at least one required).
    REQ(Active);
    REQ(ProgramMode);
    REQ(InUse);
    OPT(RemainingDuration);
    OPT(StatusFault);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV(Valve,D0)     // Defines an electronic Valve.  Can be used standalone or as a Linked Service for either a <b>Faucet</b> or <b>IrrigationSystem</b> Service.
    REQ(Active);
    REQ(InUse);
    REQ(ValveType);
    OPT(SetDuration);
    OPT(RemainingDuration);
    OPT(IsConfigured);
    OPT(ServiceLabelIndex);
    OPT(StatusFault);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  SERVICES_GROUP // Security Systems

  CREATE_SERV(SecuritySystem,7E)    // Defines a Security System.  Often used in combination with <b>MotionSensor</b> and <b>ContactSensor</b> Services.
    REQ(SecuritySystemCurrentState);
    REQ(SecuritySystemTargetState);
    OPT(SecuritySystemAlarmType);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  SERVICES_GROUP // Televisions

  CREATE_SERV(InputSource,D9)   // Defines an Input Source for a TV.  Use only as a Linked Service for the <b>Television</b> Service.
      REQ(Identifier);
      OPT(ConfiguredName);
      OPT(IsConfigured);
      OPT(CurrentVisibilityState);
      OPT(TargetVisibilityState);
  END_SERV

  CREATE_SERV(Television,D8)    // Defines a TV.  Optional Linked Services: <b>InputSource</b> and <b>TelevisionSpeaker</b>.
    REQ(Active);
    OPT(ActiveIdentifier);
    OPT(RemoteKey);
    OPT(PowerModeSelection);      
    OPT(ConfiguredName);
  END_SERV

  CREATE_SERV(TelevisionSpeaker,113)    // Defines a Television Speaker that can be controlled via the Remote Control widget on an iPhone.  Use only as a Linked Service for the <b>Television</b> Service.
    REQ(VolumeControlType);
    REQ(VolumeSelector);      
    OPT(ConfiguredName);
  END_SERV  

  SERVICES_GROUP // Miscellaneous

  CREATE_SERV(ServiceLabel,CC)    // Defines a naming scheme for un-nameable Services, such as a <b>StatelessProgrammableSwitch</b>, by Linking them to this Service.  When used, those other Services must each include a <b>ServiceLabelIndex</b> Characteristic with a unique value.
    REQ(ServiceLabelNamespace);
  END_SERV     

  // Deprecated or unsupported Services
  
  CREATE_SERV_DEP(HAPProtocolInformation,A2)
    REQ_DEP(Version);
  END_SERV

  CREATE_SERV_DEP(Microphone,112)
    REQ_DEP(Mute);
    OPT_DEP(Volume);
    OPT_DEP(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

  CREATE_SERV_DEP(Speaker,113)
    REQ_DEP(Mute);
    OPT_DEP(Volume);
    OPT_DEP(ConfiguredName);
    OPT_DEP(Name);
  END_SERV

}

//////////////////////////////////////////
// SPAN CHARACTERISTICS (HAP Chapter 9) //
//////////////////////////////////////////

// Macro to define Span Characteristic structures based on name of HAP Characteristic, default value, and min/max value (not applicable for STRING or BOOL which default to min=0, max=1)

#define CREATE_CHAR(TYPE,HAPCHAR,DEFVAL,MINVAL,MAXVAL,...) \
  struct HAPCHAR : SpanCharacteristic { __VA_OPT__(enum{) __VA_ARGS__ __VA_OPT__(};) HAPCHAR(TYPE val=DEFVAL, boolean nvsStore=false) : SpanCharacteristic {&hapChars.HAPCHAR} { init(val,nvsStore,(TYPE)MINVAL,(TYPE)MAXVAL); } };

namespace Characteristic {

  CREATE_CHAR(uint32_t,AccessoryFlags,1,1,1);   // not applicable for HomeSpan
  CREATE_CHAR(uint8_t,Active,0,0,1,INACTIVE,ACTIVE);  // indicates if the Service is active/on
  CREATE_CHAR(uint32_t,ActiveIdentifier,0,0,255);     // numerical Identifier of the <b>InputSource</b> selected in the Home App.
  CREATE_CHAR(uint8_t,AirQuality,0,0,5,UNKNOWN,EXCELLENT,GOOD,FAIR,INFERIOR,POOR);   // a subjective description
  CREATE_CHAR(uint8_t,BatteryLevel,100,0,100);  // measured as a percentage
  CREATE_CHAR(int,Brightness,0,0,100);  // measured as a percentage
  CREATE_CHAR(double,CarbonMonoxideLevel,0,0,100);  // measured in parts per million (ppm)
  CREATE_CHAR(double,CarbonMonoxidePeakLevel,0,0,100);  // measured in parts per million (ppm)
  CREATE_CHAR(uint8_t,CarbonMonoxideDetected,0,0,1,NORMAL,ABNORMAL);  // indicates if abnormal level is detected
  CREATE_CHAR(double,CarbonDioxideLevel,0,0,100000);  // measured on parts per million (ppm)
  CREATE_CHAR(double,CarbonDioxidePeakLevel,0,0,100000);  // measured in parts per million (ppm)
  CREATE_CHAR(uint8_t,CarbonDioxideDetected,0,0,1,NORMAL,ABNORMAL); // indicates if abnormal level is detected
  CREATE_CHAR(uint8_t,ChargingState,0,0,2,NOT_CHARGING,CHARGING,NOT_CHARGEABLE); // indicates state of battery charging
  CREATE_CHAR(uint8_t,ClosedCaptions,0,0,1);  // unused by any Service
  CREATE_CHAR(double,CoolingThresholdTemperature,10,10,35);   // cooling turns on when temperature (in Celsius) rises above this threshold
  CREATE_CHAR(uint32_t,ColorTemperature,200,140,500);  // measured in inverse megaKelvin (= 1,000,000 / Kelvin)
  CREATE_CHAR(uint8_t,ContactSensorState,1,0,1,DETECTED,NOT_DETECTED);  // indictates if contact is detected (i.e. closed)
  CREATE_CHAR(const char *,ConfiguredName,"unnamed",0,1);   // default display name of this Service
  CREATE_CHAR(double,CurrentAmbientLightLevel,1,0.0001,100000);   // measured in Lux (lumens/m<sup>2</sup>
  CREATE_CHAR(int,CurrentHorizontalTiltAngle,0,-90,90);  // current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90) 
  CREATE_CHAR(uint8_t,CurrentAirPurifierState,0,0,2,INACTIVE,IDLE,PURIFYING);  // indicates current state of air purification
  CREATE_CHAR(uint8_t,CurrentSlatState,0,0,2,FIXED,JAMMED,SWINGING);  // indicates current state of slats
  CREATE_CHAR(uint8_t,CurrentPosition,0,0,100); // current position (as a percentage) from fully closed (0) to full open (100)
  CREATE_CHAR(int,CurrentVerticalTiltAngle,0,-90,90);  // current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)
  CREATE_CHAR(uint8_t,CurrentVisibilityState,0,0,1,VISIBLE,NOT_VISIBLE);  // current visibility of the Service, as selectable on the Settings Page of the Home App
  CREATE_CHAR(uint8_t,CurrentHumidifierDehumidifierState,1,0,3,INACTIVE,IDLE,HUMIDIFYING,DEHUMIDIFYING); // indicates current state of humidifier/dehumidifer
  CREATE_CHAR(uint8_t,CurrentDoorState,1,0,4,OPEN,CLOSED,OPENING,CLOSING,STOPPED);  // indicates current state of a door
  CREATE_CHAR(uint8_t,CurrentFanState,1,0,2,INACTIVE,IDLE,BLOWING);  // indicates current state of a fan
  CREATE_CHAR(uint8_t,CurrentHeatingCoolingState,0,0,2,IDLE,HEATING,COOLING); // indicates whether appliance is currently heating, cooling, or just idle
  CREATE_CHAR(uint8_t,CurrentHeaterCoolerState,1,0,3,INACTIVE,IDLE,HEATING,COOLING);  // indicates whether appliance is currently heating, cooling, idle, or off
  CREATE_CHAR(uint8_t,CurrentMediaState,0,0,5);  // not used
  CREATE_CHAR(double,CurrentRelativeHumidity,0,0,100);  // current humidity measured as a percentage
  CREATE_CHAR(double,CurrentTemperature,0,0,100);   // current temperature measured in Celsius
  CREATE_CHAR(int,CurrentTiltAngle,0,-90,90);  // current angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90)
  CREATE_CHAR(double,FilterLifeLevel,100,0,100); // measured as a percentage of remaining life
  CREATE_CHAR(uint8_t,FilterChangeIndication,0,0,1,NO_CHANGE_NEEDED,CHANGE_NEEDED);  // indicates state of filter
  CREATE_CHAR(const char *,FirmwareRevision,"1.0.0",0,1);  // must be in form x[.y[.z]] - informational only
  CREATE_CHAR(const char *,HardwareRevision,"1.0.0",0,1);  // must be in form x[.y[.z]] - informational only
  CREATE_CHAR(double,HeatingThresholdTemperature,16,0,25); // heating turns on when temperature (in Celsius) falls below this threshold
  CREATE_CHAR(boolean,HoldPosition,false,0,1);  // deprecated
  CREATE_CHAR(double,Hue,0,0,360);  // color (in degrees) from red (0) to green (120) to blue (240) and back to red (360)
  CREATE_CHAR(boolean,Identify,1,1,1,RUN_ID=1);  // triggers an update when HomeKit wants HomeSpan to run its identification routine for an Accessory
  CREATE_CHAR(uint32_t,Identifier,0,0,255); // numerical Identifer of the <b>InputSource</b>.
  CREATE_CHAR(uint8_t,InputDeviceType,0,0,6); // not used
  CREATE_CHAR(uint8_t,InputSourceType,0,0,10);  // not used
  CREATE_CHAR(uint8_t,InUse,0,0,1,NOT_IN_USE,IN_USE);   // if Service is set to active, this indictes whether it is currently in use
  CREATE_CHAR(uint8_t,IsConfigured,0,0,1,NOT_CONFIGURED,CONFIGURED);  // indicates if a predefined Service has been configured
  CREATE_CHAR(uint8_t,LeakDetected,0,0,1,NOT_DETECTED,DETECTED);  // indictates if a leak is detected
  CREATE_CHAR(uint8_t,LockCurrentState,0,0,3,UNLOCKED,LOCKED,JAMMED,UNKNOWN);  // indicates state of a lock
  CREATE_CHAR(uint8_t,LockPhysicalControls,0,0,1,CONTROL_LOCK_DISABLED,CONTROL_LOCK_ENABLED);  // indicates if local control lock is enabled
  CREATE_CHAR(uint8_t,LockTargetState,0,0,1,UNLOCK,LOCK);   // indicates desired state of lock
  CREATE_CHAR(const char *,Manufacturer,"HomeSpan",0,1);  // any string - informational only
  CREATE_CHAR(const char *,Model,"HomeSpan-ESP32",0,1);  // any string - informational only
  CREATE_CHAR(boolean,MotionDetected,0,0,1,NOT_DETECTED,DETECTED);  // indicates if motion is detected
  CREATE_CHAR(boolean,Mute,0,0,1,OFF,ON); // not used
  CREATE_CHAR(const char *,Name,"unnamed",0,1); // default display name of the Accessory
  CREATE_CHAR(double,NitrogenDioxideDensity,0,0,1000);  // measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(boolean,ObstructionDetected,0,0,1,NOT_DETECTED,DETECTED);  // indicates if obstruction is detected
  CREATE_CHAR(double,PM25Density,0,0,1000); // 2.5-micron particulate density, measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(uint8_t,OccupancyDetected,0,0,1,NOT_DETECTED,DETECTED);  // indicates if occupanccy is detected
  CREATE_CHAR(boolean,OutletInUse,0,0,1,NOT_IN_USE,IN_USE); // indicates if an appliance or light is plugged into the outlet, regardless of whether on or off 
  CREATE_CHAR(boolean,On,0,0,1,OFF,ON);  // indicates if the Service is active/on
  CREATE_CHAR(double,OzoneDensity,0,0,1000);  // measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(uint8_t,PictureMode,0,0,13);  // not used
  CREATE_CHAR(double,PM10Density,0,0,1000);  // 10-micron particulate density, measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(uint8_t,PositionState,2,0,2,GOING_TO_MINIMUM,GOING_TO_MAXIMUM,STOPPED);  // deprecated
  CREATE_CHAR(uint8_t,PowerModeSelection,0,0,0,VIEW_SETTINGS);  // when defined, creates a "View TV Settings" button in the Home App that triggers an update to this Characteristic when pressed 
  CREATE_CHAR(uint8_t,ProgramMode,0,0,2,NONE,SCHEDULED,SCHEDULE_OVERRIDEN);  // indicates if pre-scheduled program is running
  CREATE_CHAR(uint8_t,ProgrammableSwitchEvent,0,0,2,SINGLE_PRESS,DOUBLE_PRESS,LONG_PRESS);  // specifies type of button press
  CREATE_CHAR(double,RelativeHumidityDehumidifierThreshold,50,0,100);  // dehumidfier turns on when humidity rises above this threshold
  CREATE_CHAR(double,RelativeHumidityHumidifierThreshold,50,0,100);  // humidfier turns on when humidity falls below this threshold
  CREATE_CHAR(uint32_t,RemainingDuration,60,0,3600);  // duration (in seconds) remaining for Service to be active/on
  CREATE_CHAR(uint8_t,RemoteKey,4,4,15,UP=4,DOWN,LEFT,RIGHT,CENTER,BACK,PLAY_PAUSE=11,INFO=15);  // triggers an update when the corresponding key is pressed in the Remote Control widget on an iPhone 
  CREATE_CHAR(uint8_t,ResetFilterIndication,1,1,1,RESET_FILTER=1);  // triggers an update when the user chooses to reset the <b>FilterChangeIndication</b> (only appears in Eve App, not Home App)
  CREATE_CHAR(int,RotationDirection,0,0,1,CLOCKWISE,COUNTERCLOCKWISE);  // indicates the rotation direction of a fan
  CREATE_CHAR(double,RotationSpeed,0,0,100);  // measured as a percentage
  CREATE_CHAR(double,Saturation,0,0,100);  // color saturation, measured as a percentage
  CREATE_CHAR(uint8_t,SecuritySystemAlarmType,0,0,1,KNOWN,UNKNOWN);  // indicates whether alarm was triggered for known reason
  CREATE_CHAR(uint8_t,SecuritySystemCurrentState,3,0,4,ARMED_STAY,ARMED_AWAY,ARMED_NIGHT,DISARMED,ALARM_TRIGGERED);  // indicates current state of the security system 
  CREATE_CHAR(uint8_t,SecuritySystemTargetState,3,0,3,ARM_STAY,ARM_AWAY,ARM_NIGHT,DISARM); // indicates desired state of the security system
  CREATE_CHAR(const char *,SerialNumber,"HS-12345",0,1);  // any string - informational only
  CREATE_CHAR(uint8_t,ServiceLabelIndex,1,1,255);   // numerical index used to distinguish multiple copies of the same Service within an Accessory
  CREATE_CHAR(uint8_t,ServiceLabelNamespace,1,0,1,DOTS,NUMERALS);  // indicates how un-named Services linked together with a <b>ServiceLabel</b> Service should be displayed in the Home App  
  CREATE_CHAR(uint8_t,SlatType,0,0,1,HORIZONTAL,VERTICAL); // indicates the direction of a slat or group of slats
  CREATE_CHAR(uint8_t,SleepDiscoveryMode,0,0,1);  // not used
  CREATE_CHAR(uint8_t,SmokeDetected,0,0,1,NOT_DETECTED,DETECTED); // indicates if smoke is detected
  CREATE_CHAR(boolean,StatusActive,1,0,1,NOT_FUNCTIONING,FUNCTIONING);  // indicates whether the Service is properly functioning 
  CREATE_CHAR(uint8_t,StatusFault,0,0,1,NO_FAULT,FAULT);  // indicates whether the Service has a fault (only appears in Eve App, not Home App)
  CREATE_CHAR(uint8_t,StatusJammed,0,0,1,NOT_JAMMED,JAMMED);  // indicates whether the Service has been "jammed"
  CREATE_CHAR(uint8_t,StatusLowBattery,0,0,1,NOT_LOW_BATTERY,LOW_BATTERY);  // indicates state of battery
  CREATE_CHAR(uint8_t,StatusTampered,0,0,1,NOT_TAMPERED,TAMPERED);  // indicates whether the Service has been tampered with
  CREATE_CHAR(double,SulphurDioxideDensity,0,0,1000);  // measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(uint8_t,SwingMode,0,0,1,SWING_DISABLED,SWING_ENABLED);  // indicates whether swing-mode is enabled
  CREATE_CHAR(uint8_t,TargetAirPurifierState,1,0,1,MANUAL,AUTO);  // indicates desired state of air purifier
  CREATE_CHAR(uint8_t,TargetFanState,1,0,1,MANUAL,AUTO);  // indicates desired state of fan
  CREATE_CHAR(int,TargetTiltAngle,0,-90,90);  // indicated desired angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90) 
  CREATE_CHAR(uint8_t,TargetHeaterCoolerState,0,0,2,AUTO,HEAT,COOL); // indicates desired state of heater/cooler
  CREATE_CHAR(uint32_t,SetDuration,60,0,3600);  // specifies the duration (in seconds) for a Service to remain on once activated
  CREATE_CHAR(int,TargetHorizontalTiltAngle,0,-90,90);  // indicates desired angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90)
  CREATE_CHAR(uint8_t,TargetHumidifierDehumidifierState,0,0,2,AUTO,HUMIDIFY,DEHUMIDIFY);  // indicates desired state of humidifier/dehumidifier
  CREATE_CHAR(uint8_t,TargetPosition,0,0,100);  // indicates target position (as a percentage) from fully closed (0) to full open (100)
  CREATE_CHAR(uint8_t,TargetDoorState,1,0,1,OPEN,CLOSED);   // indicates desired state of door
  CREATE_CHAR(uint8_t,TargetHeatingCoolingState,0,0,3,OFF,HEAT,COOL,AUTO);  // indicates desired state of appliance
  CREATE_CHAR(uint8_t,TargetMediaState,0,0,2);  // unused
  CREATE_CHAR(double,TargetRelativeHumidity,0,0,100);   // indicates desired humidity measured as a percentage
  CREATE_CHAR(double,TargetTemperature,16,10,38);   // indicates desired temperature measures in Celsius
  CREATE_CHAR(uint8_t,TargetVisibilityState,0,0,1,VISIBLE,NOT_VISIBLE);  // indicates desired visibility of the Service, as selectable on the Settings Page of the Home App
  CREATE_CHAR(uint8_t,TemperatureDisplayUnits,0,0,1,CELSIUS,FAHRENHEIT);   // indicates the desired units to display the temperature on the device itself (has no effect on Home App)
  CREATE_CHAR(int,TargetVerticalTiltAngle,0,-90,90);  // indicates desired angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)
  CREATE_CHAR(uint8_t,ValveType,0,0,3,GENERIC,IRRIGATION,SHOWER_HEAD,FAUCET);  // indicates the type of valve
  CREATE_CHAR(const char *,Version,"1.0.0",0,1);  // unused
  CREATE_CHAR(double,VOCDensity,0,0,1000);  // measured in &micro;g/m<sup>3</sup>
  CREATE_CHAR(uint8_t,Volume,0,0,100);  // unused
  CREATE_CHAR(uint8_t,VolumeControlType,3,0,3,NONE,RELATIVE,RELATIVE_CURRENT,ABSOLUTE); // indicates the type of volume control
  CREATE_CHAR(uint8_t,VolumeSelector,0,0,1,VOLUME_UP,VOLUME_DOWN); // triggered by presses to the iPhone's volume up/down buttons when TV is selected in the Remote Control widget
  CREATE_CHAR(double,WaterLevel,0,0,100);  // measured as a percentage

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




  
