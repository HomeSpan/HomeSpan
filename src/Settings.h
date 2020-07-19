
// USER-DEFINED SETTINGS AND REFERENCE ENUMERATION CLASSES

#pragma once

//////////////////////////////////////////////////////
//   Maximum number of simultaenous IP connections  //
//   HAP requires at least 8                        //

const int MAX_CONNECTIONS=8;  

/////////////////////////////////////////////////////
//    Debug level -- controls message output       //
//       0=Minimal, 1=Informative, 2=All           //

#define DEBUG_LEVEL   1

//-------------------------------------------------//

#if DEBUG_LEVEL>1
  #define LOG2(x) Serial.print(x)
#else
  #define LOG2(x)
#endif

#if DEBUG_LEVEL>0
  #define LOG1(x) Serial.print(x)
#else
  #define LOG1(x)
#endif

//////////////////////////////////////////////////////
//   Types of Services (default is Regular)         //
//   Reference: HAP Table 6-2                       //

enum ServiceType {
  Regular,
  Hidden,
  Primary
};  
   
//////////////////////////////////////////////////////
//   Types of Accessory Categories                  //
//   Reference: HAP Section 13                      //

enum class Category {
  Other=1,
  Bridges=2,
  Fans=3,
  GarageDoorOpeners=4,
  Lighting=5,
  Locks=6,
  Outlets=7,
  Switches=8,
  Thermostats=9,
  Sensors=10,
  SecuritySystems=11,
  Doors=12,
  Windows=13,
  WindowCoverings=14,
  ProgrammableSwitches=15,
  IPCameras=17,
  VideoDoorbells=18,
  AirPurifiers=19,
  Heaters=20,
  AirConditioners=21,
  Humidifiers=22,
  Dehumidifiers=23,
  Sprinklers=28,
  Faucets=29,
  ShowerSystems=30
};
