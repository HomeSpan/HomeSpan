/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
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

//////////////////////////////////////////
// HAP CHARACTERISTICS (HAP Chapter 9) //
//////////////////////////////////////////

enum PERMS{       // create bitflags based on HAP Table 6-4
  PR=1,
  PW=2,
  EV=4,
  AA=8,
  TW=16,
  HD=32,
  WR=64,
  NV=128          // this is a non-HAP flag used to specify that no value should be provided (should be a HAP flag!)
};

enum FORMAT {     // HAP Table 6-5
  BOOL=0,
  UINT8=1,
  UINT16=2,
  UINT32=3,
  UINT64=4,
  INT=5,
  FLOAT=6,
  STRING=7,
  DATA=8,
  TLV_ENC=9
};

///////////////////////////////

struct HapChar {
  const char *type;
  const char *hapName;
  PERMS perms;
  FORMAT format;
  boolean staticRange;
};

///////////////////////////////

#define HAPCHAR(hapName,type,perms,format,staticRange)  HapChar hapName {#type,#hapName,(PERMS)(perms),format,staticRange}

struct HapCharacteristics {
  
  HAPCHAR( AccessoryFlags, A6, PR+EV, UINT32, true );
  HAPCHAR( Active, B0, PW+PR+EV, UINT8, true );
  HAPCHAR( ActiveIdentifier, E7, PW+PR+EV, UINT32, true );
  HAPCHAR( AirQuality, 95, PR+EV, UINT8, true );
  HAPCHAR( BatteryLevel, 68, PR+EV, UINT8, false );
  HAPCHAR( Brightness, 8, PR+PW+EV, INT, false );
  HAPCHAR( CarbonMonoxideLevel, 90, PR+EV, FLOAT, false );
  HAPCHAR( CarbonMonoxidePeakLevel, 91, PR+EV, FLOAT, false );
  HAPCHAR( CarbonDioxideDetected, 92, PR+EV, UINT8, true );
  HAPCHAR( CarbonDioxideLevel, 93, PR+EV, FLOAT, false );
  HAPCHAR( CarbonDioxidePeakLevel, 94, PR+EV, FLOAT, false );
  HAPCHAR( CarbonMonoxideDetected, 69, PR+EV, UINT8, true );
  HAPCHAR( ChargingState, 8F, PR+EV, UINT8, true );
  HAPCHAR( ClosedCaptions, DD, PW+PR+EV, UINT8, true );
  HAPCHAR( CoolingThresholdTemperature, D, PR+PW+EV, FLOAT, false );
  HAPCHAR( ColorTemperature, CE, PR+PW+EV, UINT32, false );
  HAPCHAR( ConfiguredName, E3, PW+PR+EV, STRING, false );
  HAPCHAR( ContactSensorState, 6A, PR+EV, UINT8, true );
  HAPCHAR( CurrentAmbientLightLevel, 6B, PR+EV, FLOAT, false );
  HAPCHAR( CurrentHorizontalTiltAngle, 6C, PR+EV, INT, false );  
  HAPCHAR( CurrentAirPurifierState, A9, PR+EV, UINT8, true  );
  HAPCHAR( CurrentSlatState, AA, PR+EV, UINT8, true );
  HAPCHAR( CurrentPosition, 6D, PR+EV, UINT8, false );
  HAPCHAR( CurrentVerticalTiltAngle, 6E, PR+EV, INT, false );  
  HAPCHAR( CurrentHumidifierDehumidifierState, B3, PR+EV, UINT8, true );    
  HAPCHAR( CurrentDoorState, E, PR+EV, UINT8, true );
  HAPCHAR( CurrentFanState, AF, PR+EV, UINT8, true );
  HAPCHAR( CurrentHeatingCoolingState, F, PR+EV, UINT8, true );
  HAPCHAR( CurrentHeaterCoolerState, B1, PR+EV, UINT8, true );
  HAPCHAR( CurrentMediaState, E0, PR+EV, UINT8, true );
  HAPCHAR( CurrentRelativeHumidity, 10, PR+EV, FLOAT, false ); 
  HAPCHAR( CurrentTemperature, 11, PR+EV, FLOAT, false );
  HAPCHAR( CurrentTiltAngle, C1, PR+EV, INT, false );
  HAPCHAR( CurrentVisibilityState, 135, PR+EV, UINT8, true );
  HAPCHAR( DisplayOrder, 136, PR+EV, TLV_ENC, true );
  HAPCHAR( FilterLifeLevel, AB, PR+EV, FLOAT, false );
  HAPCHAR( FilterChangeIndication, AC, PR+EV, UINT8, true );
  HAPCHAR( FirmwareRevision, 52, PR+EV, STRING, true );
  HAPCHAR( HardwareRevision, 53, PR, STRING, true );
  HAPCHAR( HeatingThresholdTemperature, 12, PR+PW+EV, FLOAT, false );
  HAPCHAR( HoldPosition, 6F, PW, BOOL, true );
  HAPCHAR( Hue, 13, PR+PW+EV, FLOAT, false );
  HAPCHAR( Identify, 14, PW, BOOL, true );
  HAPCHAR( Identifier, E6, PR, UINT32, true );
  HAPCHAR( InputDeviceType, DC, PR+EV, UINT8, true );
  HAPCHAR( InputSourceType, DB, PR+EV, UINT8, true );
  HAPCHAR( InUse, D2, PR+EV, UINT8, true );
  HAPCHAR( IsConfigured, D6, PR+EV, UINT8, true );
  HAPCHAR( LeakDetected, 70, PR+EV, UINT8, true );  
  HAPCHAR( LockCurrentState, 1D, PR+EV, UINT8, true );  
  HAPCHAR( LockPhysicalControls, A7, PW+PR+EV, UINT8, true );
  HAPCHAR( LockTargetState, 1E, PW+PR+EV, UINT8, true );  
  HAPCHAR( Manufacturer, 20, PR, STRING, true );
  HAPCHAR( Model, 21, PR, STRING, true );
  HAPCHAR( MotionDetected, 22, PR+EV, BOOL, true );
  HAPCHAR( Mute, 11A, PW+PR+EV, BOOL, true );
  HAPCHAR( Name, 23, PR, STRING, true );
  HAPCHAR( NitrogenDioxideDensity, C4, PR+EV, FLOAT, false );
  HAPCHAR( ObstructionDetected, 24, PR+EV, BOOL, true );
  HAPCHAR( PM25Density, C6, PR+EV, FLOAT, false );
  HAPCHAR( OccupancyDetected, 71, PR+EV, UINT8, true );
  HAPCHAR( OutletInUse, 26, PR+EV, BOOL, true );
  HAPCHAR( On, 25, PR+PW+EV, BOOL, true );
  HAPCHAR( OzoneDensity, C3, PR+EV, FLOAT, false );
  HAPCHAR( PM10Density, C7, PR+EV, FLOAT, false );
  HAPCHAR( PictureMode, E2, PW+PR+EV, UINT8, true );
  HAPCHAR( PositionState, 72, PR+EV, UINT8, true );
  HAPCHAR( PowerModeSelection, DF, PW, UINT8, true );
  HAPCHAR( ProgramMode, D1, PR+EV, UINT8, true );
  HAPCHAR( ProgrammableSwitchEvent, 73, PR+EV+NV, UINT8, true );
  HAPCHAR( RelativeHumidityDehumidifierThreshold, C9, PR+PW+EV, FLOAT, false );
  HAPCHAR( RelativeHumidityHumidifierThreshold, CA, PR+PW+EV, FLOAT, false );
  HAPCHAR( RemainingDuration, D4, PR+EV, UINT32, false );
  HAPCHAR( RemoteKey, E1, PW, UINT8, true );
  HAPCHAR( ResetFilterIndication, AD, PW, UINT8, true );
  HAPCHAR( RotationDirection, 28, PR+PW+EV, INT, true );
  HAPCHAR( RotationSpeed, 29, PR+PW+EV, FLOAT, false );
  HAPCHAR( Saturation, 2F, PR+PW+EV, FLOAT, false );  
  HAPCHAR( SecuritySystemAlarmType, 8E, PR+EV, UINT8, true );  
  HAPCHAR( SecuritySystemCurrentState, 66, PR+EV, UINT8, true );  
  HAPCHAR( SecuritySystemTargetState, 67, PW+PR+EV, UINT8, true  );  
  HAPCHAR( SerialNumber, 30, PR, STRING, true );
  HAPCHAR( ServiceLabelIndex, CB, PR, UINT8, true );
  HAPCHAR( ServiceLabelNamespace, CD, PR, UINT8, true );
  HAPCHAR( SlatType, C0, PR, UINT8, true );
  HAPCHAR( SleepDiscoveryMode, E8, PR+EV, UINT8, true );
  HAPCHAR( SmokeDetected, 76, PR+EV, UINT8, true );
  HAPCHAR( StatusActive, 75, PR+EV, BOOL, true );
  HAPCHAR( StatusFault, 77, PR+EV, UINT8, true );
  HAPCHAR( StatusJammed, 78, PR+EV, UINT8, true );
  HAPCHAR( StatusLowBattery, 79, PR+EV, UINT8, true );
  HAPCHAR( StatusTampered, 7A, PR+EV, UINT8, true );
  HAPCHAR( SulphurDioxideDensity, C5, PR+EV, FLOAT, false );
  HAPCHAR( SwingMode, B6, PR+EV+PW, UINT8, true );
  HAPCHAR( TargetAirPurifierState, A8, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetFanState, BF, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetTiltAngle, C2, PW+PR+EV, INT, false );
  HAPCHAR( TargetHeaterCoolerState, B2, PW+PR+EV, UINT8, true );
  HAPCHAR( SetDuration, D3, PW+PR+EV, UINT32, false );
  HAPCHAR( TargetHorizontalTiltAngle, 7B, PW+PR+EV, INT, false );
  HAPCHAR( TargetHumidifierDehumidifierState, B4, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetPosition, 7C, PW+PR+EV, UINT8, false );
  HAPCHAR( TargetDoorState, 32, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetHeatingCoolingState, 33, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetMediaState, 137, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetRelativeHumidity, 34, PW+PR+EV, FLOAT, false );
  HAPCHAR( TargetTemperature, 35, PW+PR+EV, FLOAT, false );
  HAPCHAR( TargetVisibilityState, 134, PW+PR+EV, UINT8, true );
  HAPCHAR( TemperatureDisplayUnits, 36, PW+PR+EV, UINT8, true );
  HAPCHAR( TargetVerticalTiltAngle, 7D, PW+PR+EV, INT, false );
  HAPCHAR( ValveType, D5, PR+EV, UINT8, true );  
  HAPCHAR( Version, 37, PR, STRING, true );
  HAPCHAR( VOCDensity, C8, PR+EV, FLOAT, false );   
  HAPCHAR( Volume, 119, PW+PR+EV, UINT8, false );
  HAPCHAR( VolumeControlType, E9, PR+EV, UINT8, true );
  HAPCHAR( VolumeSelector, EA, PW, UINT8, true );
  HAPCHAR( WaterLevel, B5, PR+EV, FLOAT, false );

};

extern HapCharacteristics hapChars;
