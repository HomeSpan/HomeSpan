Service | Req. Characteristics | Opt Characteristics
------- | -------------------- | -------------------
AccessoryInformation | FirmwareRevision | HardwareRevision
 | Identify |
 | Manufacturer |
 | Model |
 | Name |
 | SerialNumber |

   AirPurifier
    REQ(Active);
    REQ(CurrentAirPurifierState);
    REQ(TargetAirPurifierState);
    OPT(Name);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  }};

   AirQualitySensor
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

   BatteryService
    REQ(BatteryLevel);
    REQ(ChargingState);
    REQ(StatusLowBattery);
    OPT(Name);
  }};

   CarbonDioxideSensor
    REQ(CarbonDioxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonDioxideLevel);
    OPT(CarbonDioxidePeakLevel);
  }};

   CarbonMonoxideSensor
    REQ(CarbonMonoxideDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
    OPT(CarbonMonoxideLevel);
    OPT(CarbonMonoxidePeakLevel);
    }};

   ContactSensor
    REQ(ContactSensorState);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  }};

   Door
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  }};

   Doorbell
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(Volume);
    OPT(Brightness);
  }};

   Fan
    REQ(Active);
    OPT(Name);
    OPT(CurrentFanState);
    OPT(TargetFanState);
    OPT(RotationDirection);
    OPT(RotationSpeed);
    OPT(SwingMode);
    OPT(LockPhysicalControls);
  }};

   Faucet
    REQ(Active);
    OPT(StatusFault);
    OPT(Name);
  }};

   FilterMaintenance
    REQ(FilterChangeIndication);
    OPT(Name);
    OPT(FilterLifeLevel);
    OPT(ResetFilterIndication);
  }};

   GarageDoorOpener
    REQ(CurrentDoorState);
    REQ(TargetDoorState);
    REQ(ObstructionDetected);
    OPT(LockCurrentState);
    OPT(LockTargetState);
    OPT(Name);
  }};

   HAPProtocolInformation
    REQ(Version);
  }};

   HeaterCooler
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

   HumidifierDehumidifier
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

   HumiditySensor
    REQ(CurrentRelativeHumidity);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);   
  }};

   IrrigationSystem
    REQ(Active);
    REQ(ProgramMode);
    REQ(InUse);
    OPT(RemainingDuration);
    OPT(Name);
    OPT(StatusFault);
  }};

   LeakSensor
    REQ(LeakDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  }};

   LightBulb
    REQ(On);
    OPT(Brightness);
    OPT(Hue);
    OPT(Name);
    OPT(Saturation);
    OPT(ColorTemperature);
  }};

   LightSensor
    REQ(CurrentAmbientLightLevel);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);          
  }};

   LockMechanism
    REQ(LockCurrentState);
    REQ(LockTargetState);
    OPT(Name);
  }};

   Microphone
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
  }};

   MotionSensor
    REQ(MotionDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);       
  }};

   OccupancySensor
    REQ(OccupancyDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);         
  }};

   Outlet
    REQ(On);
    REQ(OutletInUse);
    OPT(Name);
  }};

   SecuritySystem
    REQ(SecuritySystemCurrentState);
    REQ(SecuritySystemTargetState);
    OPT(Name);
    OPT(SecuritySystemAlarmType);
    OPT(StatusFault);
    OPT(StatusTampered);
  }};  

   ServiceLabel
    REQ(ServiceLabelNamespace);
  }};  

   Slat
    REQ(CurrentSlatState);
    REQ(SlatType);
    OPT(Name);
    OPT(SwingMode);
    OPT(CurrentTiltAngle);
    OPT(TargetTiltAngle);
  }};

   SmokeSensor
    REQ(SmokeDetected);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);             
  }};

   Speaker
    REQ(Mute);
    OPT(Name);
    OPT(Volume);
  }};

   StatelessProgrammableSwitch
    REQ(ProgrammableSwitchEvent);
    OPT(Name);
    OPT(ServiceLabelIndex);
  }};

   Switch
    REQ(On);
    OPT(Name);
  }};

   TemperatureSensor
    REQ(CurrentTemperature);
    OPT(Name);
    OPT(StatusActive);
    OPT(StatusFault);
    OPT(StatusTampered);
    OPT(StatusLowBattery);
  }};

   Thermostat
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

   Valve
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

   Window
    REQ(CurrentPosition);
    REQ(TargetPosition);
    REQ(PositionState);
    OPT(Name);
    OPT(HoldPosition);
    OPT(ObstructionDetected);
  }};

   WindowCovering
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
