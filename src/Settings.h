
// USER-DEFINED SETTINGS AND REFERENCE ENUMERATION CLASSES

#pragma once

//////////////////////////////////////////////////////
//              HomeSpan Version                    //

#define     HOMESPAN_VERSION      "1.0.0"

//////////////////////////////////////////////////////
//                DEFAULT SETTINGS                    

#define     DEFAULT_DISPLAY_NAME  "HomeSpan Server"   // change with optional second argument in homeSpan.begin()
#define     DEFAULT_HOST_NAME     "HomeSpan"          // change with optional third argument in homeSpan.begin()
#define     DEFAULT_MODEL_NAME    "HomeSpan-ESP32"    // change with optional fourth argument in homeSpan.begin()

#define     DEFAULT_SETUP_CODE    "46637726"          // changed during network setup or with 'S' command

#define     DEFAULT_CONTROL_PIN   21                  // change with homeSpan.setControlPin(pin)
#define     DEFAULT_STATUS_PIN    LED_BUILTIN         // change with homeSpan.setStatusPin(pin)

#define     DEFAULT_AP_PASSWORD   "homespan"          // change with homeSpan.setApPassword(pwd)

#define     DEFAULT_AP_TIMEOUT    120                 // change with homeSpan.setApTimeout(nSeconds)

#define     DEFAULT_LOG_LEVEL     0                   // change with homeSpan.setLogLevel(level)

#define     DEFAULT_MAX_CONNECTIONS 8                 // change with homeSpan.setMaxConnections(num);

/////////////////////////////////////////////////////
//      Message Log Level Control Macros           //
//       0=Minimal, 1=Informative, 2=All           //

#define LOG1(x) if(homeSpan.logLevel>0)Serial.print(x)
#define LOG2(x) if(homeSpan.logLevel>1)Serial.print(x)
   
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

//////////////////////////////////////////////////////
//   Types of Status Codes for use with update()    //
//   Reference: HAP Table 6-11                      //

enum class StatusCode {
  
  OK=0,                        // Can be returned by user in update() 
  Unable=-70402,               // Can be returned by user in update()
  Busy=-70403,                 // Can be returned by user in update()
  
  ReadOnly=-70404,             // HomeSpan takes care of this internally
  WriteOnly=-70405,            // HomeSpan takes care of this internally
  NotifyNotAllowed=-70406,     // HomeSpan takes care of this internally
  UnknownResource=-70409,      // HomeSpan takes care of this internally
  InvalidValue=-70410,         // HomeSpan takes care of this internally
  
  TBD=-1                       // status To-Be-Determined (TBD) once service.update() called - internal use only
};
