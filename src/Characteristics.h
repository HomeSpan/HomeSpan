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
  BOOL,
  UINT8,
  UINT16,
  UINT32,
  UINT64,
  INT,
  FLOAT,
  STRING
};

///////////////////////////////

struct HapChar {
  const char *type;
  const char *hapName;
  PERMS perms;
  FORMAT format;
};

///////////////////////////////

#define HAPCHAR(hapName,type,perms,format)  HapChar hapName {#type,#hapName,(PERMS)(perms),format}

struct HapCharacteristics {
  
  HAPCHAR( Active, B0, PW+PR+EV, UINT8 );
  HAPCHAR( AirQuality, 95, PR+EV, UINT8 );
  HAPCHAR( BatteryLevel, 68, PR+EV, UINT8 );
  HAPCHAR( Brightness, 8, PR+PW+EV, INT );
  HAPCHAR( CarbonMonoxideLevel, 90, PR+EV, FLOAT );
  HAPCHAR( CarbonMonoxidePeakLevel, 91, PR+EV, FLOAT );
  HAPCHAR( CarbonDioxideDetected, 92, PR+EV, UINT8 );
  HAPCHAR( CarbonDioxideLevel, 93, PR+EV, FLOAT );
  HAPCHAR( CarbonDioxidePeakLevel, 94, PR+EV, FLOAT );
  HAPCHAR( CarbonMonoxideDetected, 69, PR+EV, UINT8 );
  HAPCHAR( ChargingState, 8F, PR+EV, UINT8  );
  HAPCHAR( CoolingThresholdTemperature, D, PR+PW+EV, FLOAT );
  HAPCHAR( ColorTemperature, CE, PR+PW+EV, UINT32 );
  HAPCHAR( ContactSensorState, 6A, PR+EV, UINT8 );
  HAPCHAR( CurrentAmbientLightLevel, 6B, PR+EV, FLOAT );
  HAPCHAR( CurrentHorizontalTiltAngle, 6C, PR+EV, INT );  
  HAPCHAR( CurrentAirPurifierState, A9, PR+EV, UINT8  );
  HAPCHAR( CurrentSlatState, AA, PR+EV, UINT8 );
  HAPCHAR( CurrentPosition, 6D, PR+EV, UINT8 );
  HAPCHAR( CurrentVerticalTiltAngle, 6E, PR+EV, INT );  
  HAPCHAR( CurrentHumidifierDehumidifierState, B3, PR+EV, UINT8 );    
  HAPCHAR( CurrentDoorState, E, PR+EV, UINT8 );
  HAPCHAR( CurrentFanState, AF, PR+EV, UINT8 );
  HAPCHAR( CurrentHeatingCoolingState, F, PR+EV, UINT8 );
  HAPCHAR( CurrentHeaterCoolerState, B1, PR+EV, UINT8 );
  HAPCHAR( CurrentRelativeHumidity, 10, PR+EV, FLOAT ); 
  HAPCHAR( CurrentTemperature, 11, PR+EV, FLOAT );
  HAPCHAR( CurrentTiltAngle, C1, PR+EV, INT );
  HAPCHAR( FilterLifeLevel, AB, PR+EV, FLOAT );
  HAPCHAR( FilterChangeIndication, AC, PR+EV, UINT8 );
  HAPCHAR( FirmwareRevision, 52, PR, STRING );
  HAPCHAR( HardwareRevision, 53, PR, STRING );
  HAPCHAR( HeatingThresholdTemperature, 12, PR+PW+EV, FLOAT );
  HAPCHAR( HoldPosition, 6F, PW, BOOL );
  HAPCHAR( Hue, 13, PR+PW+EV, FLOAT );
  HAPCHAR( Identify, 14, PW, BOOL );
  HAPCHAR( InUse, D2, PR+EV, UINT8 );
  HAPCHAR( IsConfigured, D6, PR+EV, UINT8 );
  HAPCHAR( LeakDetected, 70, PR+EV, UINT8 );  
  HAPCHAR( LockCurrentState, 1D, PR+EV, UINT8 );  
  HAPCHAR( LockPhysicalControls, A7, PW+PR+EV, UINT8 );
  HAPCHAR( LockTargetState, 1E, PW+PR+EV, UINT8 );  
  HAPCHAR( Manufacturer, 20, PR, STRING );
  HAPCHAR( Model, 21, PR, STRING );
  HAPCHAR( MotionDetected, 22, PR+EV, BOOL );
  HAPCHAR( Mute, 11A, PW+PR+EV, BOOL );
  HAPCHAR( Name, 23, PR, STRING );
  HAPCHAR( NitrogenDioxideDensity, C4, PR+EV, FLOAT );
  HAPCHAR( ObstructionDetected, 24, PR+EV, BOOL );
  HAPCHAR( PM25Density, C6, PR+EV, FLOAT  );
  HAPCHAR( OccupancyDetected, 71, PR+EV, UINT8 );
  HAPCHAR( OutletInUse, 26, PR+EV, BOOL );
  HAPCHAR( On, 25, PR+PW+EV, BOOL );
  HAPCHAR( OzoneDensity, C3, PR+EV, FLOAT );
  HAPCHAR( PM10Density, C7, PR+EV, FLOAT );
  HAPCHAR( PositionState, 72, PR+EV, UINT8 );
  HAPCHAR( ProgramMode, D1, PR+EV, UINT8 );
  HAPCHAR( ProgrammableSwitchEvent, 73, PR+EV+NV, UINT8 );                      
  HAPCHAR( RelativeHumidityDehumidifierThreshold, C9, PR+PW+EV, FLOAT );
  HAPCHAR( RelativeHumidityHumidifierThreshold, CA, PR+PW+EV, FLOAT );
  HAPCHAR( RemainingDuration, D4, PR+EV, UINT32 );
  HAPCHAR( ResetFilterIndication, AD, PW, UINT8 );
  HAPCHAR( RotationDirection, 28, PR+PW+EV, INT );
  HAPCHAR( RotationSpeed, 29, PR+PW+EV, FLOAT );
  HAPCHAR( Saturation, 2F, PR+PW+EV, FLOAT );  
  HAPCHAR( SecuritySystemAlarmType, 8E, PR+EV, UINT8 );  
  HAPCHAR( SecuritySystemCurrentState, 66, PR+EV, UINT8 );  
  HAPCHAR( SecuritySystemTargetState, 67, PW+PR+EV, UINT8 );  
  HAPCHAR( SerialNumber, 30, PR, STRING );
  HAPCHAR( ServiceLabelIndex, CB, PR, UINT8 );
  HAPCHAR( ServiceLabelNamespace, CD, PR, UINT8 );
  HAPCHAR( SlatType, C0, PR, UINT8 );
  HAPCHAR( SmokeDetected, 76, PR+EV, UINT8 );
  HAPCHAR( StatusActive, 75, PR+EV, BOOL );
  HAPCHAR( StatusFault, 77, PR+EV, UINT8 );
  HAPCHAR( StatusJammed, 78, PR+EV, UINT8 );
  HAPCHAR( StatusLowBattery, 79, PR+EV, UINT8 );
  HAPCHAR( StatusTampered, 7A, PR+EV, UINT8 );
  HAPCHAR( SulphurDioxideDensity, C5, PR+EV, FLOAT );
  HAPCHAR( SwingMode, B6, PR+EV+PW, UINT8 );
  HAPCHAR( TargetAirPurifierState, A8, PW+PR+EV, UINT8  );
  HAPCHAR( TargetFanState, BF, PW+PR+EV, UINT8  );
  HAPCHAR( TargetTiltAngle, C2, PW+PR+EV, INT  );
  HAPCHAR( TargetHeaterCoolerState, B2, PW+PR+EV, UINT8  );
  HAPCHAR( SetDuration, D3, PW+PR+EV, UINT32  );
  HAPCHAR( TargetHorizontalTiltAngle, 7B, PW+PR+EV, INT  );
  HAPCHAR( TargetHumidifierDehumidifierState, B4, PW+PR+EV, UINT8  );
  HAPCHAR( TargetPosition, 7C, PW+PR+EV, UINT8 );
  HAPCHAR( TargetDoorState, 32, PW+PR+EV, UINT8 );
  HAPCHAR( TargetHeatingCoolingState, 33, PW+PR+EV, UINT8 );
  HAPCHAR( TargetRelativeHumidity, 34, PW+PR+EV, FLOAT );
  HAPCHAR( TargetTemperature, 35, PW+PR+EV, FLOAT );
  HAPCHAR( TemperatureDisplayUnits, 36, PW+PR+EV, UINT8 );
  HAPCHAR( TargetVerticalTiltAngle, 7D, PW+PR+EV, INT );
  HAPCHAR( ValveType, D5, PR+EV, UINT8 );  
  HAPCHAR( Version, 37, PR, STRING );
  HAPCHAR( VOCDensity, C8, PR+EV, FLOAT );   
  HAPCHAR( Volume, 119, PW+PR+EV, UINT8 );
  HAPCHAR( WaterLevel, B5, PR+EV, FLOAT );   
};

extern HapCharacteristics hapChars;
