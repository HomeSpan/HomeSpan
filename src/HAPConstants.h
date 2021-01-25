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
 
#pragma once

// HAP TLV Types (HAP Table 5-6)

typedef enum {
  kTLVType_Method=0x00,
  kTLVType_Identifier=0x01,
  kTLVType_Salt=0x02,
  kTLVType_PublicKey=0x03,
  kTLVType_Proof=0x04,
  kTLVType_EncryptedData=0x05,
  kTLVType_State=0x06,
  kTLVType_Error=0x07,
  kTLVType_RetryDelay=0x08,
  kTLVType_Certificate=0x09,
  kTLVType_Signature=0x0A,
  kTLVType_Permissions=0x0B,
  kTLVType_FragmentData=0x0C,
  kTLVType_FragmentLast=0x0D,
  kTLVType_Flags=0x13,
  kTLVType_Separator=0xFF
} kTLVType;


// HAP Error Codes (HAP Table 5-5)

typedef enum {
  tagError_Unknown=0x01,
  tagError_Authentication=0x02,
  tagError_Backoff=0x03,
  tagError_MaxPeers=0x04,
  tagError_MaxTries=0x05,
  tagError_Unavailable=0x06,
  tagError_Busy=0x07
} tagError;


// Pair-Setup and Pair-Verify States

typedef enum {
  pairState_M0=0,
  pairState_M1=1,
  pairState_M2=2,
  pairState_M3=3,
  pairState_M4=4,
  pairState_M5=5,
  pairState_M6=6
} pairState;

// HAP Status Codes (HAP Table 6-11)

enum class StatusCode {  
  OK=0,
  Unable=-70402,
  ReadOnly=-70404,
  WriteOnly=-70405,
  NotifyNotAllowed=-70406,
  UnknownResource=-70409,
  InvalidValue=-70410,  
  TBD=-1                       // status To-Be-Determined (TBD) once service.update() called - internal use only
};

///////////////////////////////

struct HapCharType {
  const char *id;
  const char *name;
  uint8_t perms;
};

///////////////////////////////

#define HAPCHAR(name,id,perms)  HapCharType name {#id,#name,perms}

  enum {          // create bitflags based on HAP Table 6-4
    PR=1,
    PW=2,
    EV=4,
    AA=8,
    TW=16,
    HD=32,
    WR=64,
    NV=128
  };

struct HapCharList {
  
  HAPCHAR( Active, B0, PW+PR+EV );
  HAPCHAR( AirQuality, 95, PR+EV );
  HAPCHAR( BatteryLevel, 68, PR+EV );
  HAPCHAR( Brightness, 8, PR+PW+EV );
  HAPCHAR( CarbonMonoxideLevel, 90, PR+EV );
  HAPCHAR( CarbonMonoxidePeakLevel, 91, PR+EV );
  HAPCHAR( CarbonDioxideDetected, 92, PR+EV );
  HAPCHAR( CarbonDioxideLevel, 93, PR+EV );
  HAPCHAR( CarbonDioxidePeakLevel, 94, PR+EV );
  HAPCHAR( CarbonMonoxideDetected, 69, PR+EV );
  HAPCHAR( ChargingState, 8F, PR+EV );
  HAPCHAR( CoolingThresholdTemperature, D, PR+PW+EV );
  HAPCHAR( ColorTemperature, CE, PR+PW+EV );
  HAPCHAR( ContactSensorState, 6A, PR+EV );
  HAPCHAR( CurrentAmbientLightLevel, 6B, PR+EV );
  HAPCHAR( CurrentHorizontalTiltAngle, 6C, PR+EV );  
  HAPCHAR( CurrentAirPurifierState, A9, PR+EV );
  HAPCHAR( CurrentSlatState, AA, PR+EV );
  HAPCHAR( CurrentPosition, 6D, PR+EV );
  HAPCHAR( CurrentVerticalTiltAngle, 6E, PR+EV );  
  HAPCHAR( CurrentHumidifierDehumidifierState, B3, PR+EV );    
  HAPCHAR( CurrentDoorState, E, PR+EV );
  HAPCHAR( CurrentFanState, AF, PR+EV );
  HAPCHAR( CurrentHeatingCoolingState, F, PR+EV );
  HAPCHAR( CurrentHeaterCoolerState, B1, PR+EV );
  HAPCHAR( CurrentRelativeHumidity, 10, PR+EV ); 
  HAPCHAR( CurrentTemperature, 11, PR+EV );
  HAPCHAR( CurrentTiltAngle, C1, PR+EV );
  HAPCHAR( FilterLifeLevel, AB, PR+EV );
  HAPCHAR( FilterChangeIndication, AC, PR+EV );
  HAPCHAR( FirmwareRevision, 52, PR );
  HAPCHAR( HardwareRevision, 53, PR );
  HAPCHAR( HeatingThresholdTemperature, 12, PR+PW+EV );
  HAPCHAR( HoldPosition, 6F, PW );
  HAPCHAR( Hue, 13, PR+PW+EV );
  HAPCHAR( Identify, 14, PW );
  HAPCHAR( InUse, D2, PR+EV );
  HAPCHAR( IsConfigured, D6, PR+EV );
  HAPCHAR( LeakDetected, 70, PR+EV );  
  HAPCHAR( LockCurrentState, 1D, PR+EV );  
  HAPCHAR( LockPhysicalControls, A7, PW+PR+EV );
  HAPCHAR( LockTargetState, 1E, PW+PR+EV );  
  HAPCHAR( Manufacturer, 20, PR );
  HAPCHAR( Model, 21, PR );
  HAPCHAR( MotionDetected, 22, PR+EV );
  HAPCHAR( Mute, 11A, PW+PR+EV );
  HAPCHAR( Name, 23, PR );
  HAPCHAR( NitrogenDioxideDensity, C4, PR+EV );
  HAPCHAR( ObstructionDetected, 24, PR+EV );
  HAPCHAR( PM25Density, C6, PR+EV );
  HAPCHAR( OccupancyDetected, 71, PR+EV );
  HAPCHAR( OutletInUse, 26, PR+EV );
  HAPCHAR( On, 25, PR+PW+EV );
  HAPCHAR( OzoneDensity, C3, PR+EV );
  HAPCHAR( PM10Density, C7, PR+EV );
  HAPCHAR( PositionState, 72, PR+EV  );
  HAPCHAR( ProgramMode, D1, PR+EV );
  HAPCHAR( ProgrammableSwitchEvent, 73, PR+EV+NV );                      // NV = flag to indicate that HomeSpan should always return a null value, as required by HAP for this Characteristic
  HAPCHAR( RelativeHumidityDehumidifierThreshold, C9, PR+PW+EV );
  HAPCHAR( RelativeHumidityHumidifierThreshold, CA, PR+PW+EV );
  HAPCHAR( RemainingDuration, D4, PR+EV );
  HAPCHAR( ResetFilterIndication, AD, PW );
  HAPCHAR( RotationDirection, 28, PR+PW+EV );
  HAPCHAR( RotationSpeed, 29, PR+PW+EV );
  HAPCHAR( Saturation , 2F, PR+PW+EV );  
  HAPCHAR( SecuritySystemAlarmType , 8E, PR+EV );  
  HAPCHAR( SecuritySystemCurrentState , 66, PR+EV );  
  HAPCHAR( SecuritySystemTargetState , 67, PW+PR+EV );  
  HAPCHAR( SerialNumber, 30, PR );
  HAPCHAR( ServiceLabelIndex, CB, PR );
  HAPCHAR( ServiceLabelNamespace, CD, PR );
  HAPCHAR( SlatType, C0, PR );
  HAPCHAR( SmokeDetected, 76, PR+EV );
  HAPCHAR( StatusActive, 75, PR+EV );
  HAPCHAR( StatusFault, 77, PR+EV );
  HAPCHAR( StatusJammed, 78, PR+EV );
  HAPCHAR( StatusLowBattery, 79, PR+EV );
  HAPCHAR( StatusTampered, 7A, PR+EV );
  HAPCHAR( SulphurDioxideDensity, C5, PR+EV );
  HAPCHAR( SwingMode, B6, PR+EV+PW );
  HAPCHAR( TargetAirPurifierState, A8, PW+PR+EV  );
  HAPCHAR( TargetFanState, BF, PW+PR+EV  );
  HAPCHAR( TargetTiltAngle, C2, PW+PR+EV  );
  HAPCHAR( TargetHeaterCoolerState, B2, PW+PR+EV  );
  HAPCHAR( SetDuration, D3, PW+PR+EV  );
  HAPCHAR( TargetHorizontalTiltAngle, 7B, PW+PR+EV  );
  HAPCHAR( TargetHumidifierDehumidifierState, B4, PW+PR+EV  );
  HAPCHAR( TargetPosition, 7C, PW+PR+EV );
  HAPCHAR( TargetDoorState, 32, PW+PR+EV );
  HAPCHAR( TargetHeatingCoolingState, 33, PW+PR+EV );
  HAPCHAR( TargetRelativeHumidity, 34, PW+PR+EV );
  HAPCHAR( TargetTemperature, 35, PW+PR+EV );
  HAPCHAR( TemperatureDisplayUnits, 36, PW+PR+EV );
  HAPCHAR( TargetVerticalTiltAngle, 7D, PW+PR+EV  );
  HAPCHAR( ValveType, D5, PR+EV  );  
  HAPCHAR( Version, 37, PR );
  HAPCHAR( VOCDensity, C8, PR+EV );   
  HAPCHAR( Volume, 119, PW+PR+EV  );
  HAPCHAR( WaterLevel, B5, PR+EV );   
  
};
