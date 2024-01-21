  CREATE_CHAR(uint32_t,AccessoryFlags,1,1,1);   // not applicable for HomeSpan COM:  not applicable for HomeSpan
  CREATE_CHAR(uint8_t,Active,0,0,1,INACTIVE,ACTIVE);  // indicates if the Service is active/on COM:  indicates if the Service is active/on
  CREATE_CHAR(uint32_t,ActiveIdentifier,0,0,255);     // the Identifier of the current Input Source COM:  the Identifier of the current Input Source
  CREATE_CHAR(uint8_t,AirQuality,0,0,5,UNKNOWN,EXCELLENT,GOOD,FAIR,INFERIOR,POOR);   // a subjective description COM:  a subjective description
  CREATE_CHAR(uint8_t,BatteryLevel,0,0,100);  // measured as a percentage COM:  measured as a percentage
  CREATE_CHAR(int,Brightness,0,0,100);  // measured as a percentage COM:  measured as a percentage
  CREATE_CHAR(double,CarbonMonoxideLevel,0,0,100);  // measured in parts per million (ppm) COM:  measured in parts per million (ppm)
  CREATE_CHAR(double,CarbonMonoxidePeakLevel,0,0,100);  // measured in parts per million (ppm) COM:  measured in parts per million (ppm)
  CREATE_CHAR(uint8_t,CarbonMonoxideDetected,0,0,1,NORMAL,ABNORMAL);  // indicates if abnormal level is detected COM:  indicates if abnormal level is detected
  CREATE_CHAR(double,CarbonDioxideLevel,0,0,100000);  // measured on parts per million (ppm) COM:  measured on parts per million (ppm)
  CREATE_CHAR(double,CarbonDioxidePeakLevel,0,0,100000);  // measured in parts per million (ppm) COM:  measured in parts per million (ppm)
  CREATE_CHAR(uint8_t,CarbonDioxideDetected,0,0,1,NORMAL,ABNORMAL); // indicates if abnormal level is detected COM:  indicates if abnormal level is detected
  CREATE_CHAR(uint8_t,ChargingState,0,0,2,NOT_CHARGING,CHARGING,NOT_CHARGEABLE); // indicates state of battery charging COM:  indicates state of battery charging
  CREATE_CHAR(uint8_t,ClosedCaptions,0,0,1);  // unused by any Service COM:  unused by any Service
  CREATE_CHAR(double,CoolingThresholdTemperature,10,10,35);   // cooling turns on when temperature (in Celsius) rises above this threshold COM:  cooling turns on when temperature (in Celsius) rises above this threshold
  CREATE_CHAR(uint32_t,ColorTemperature,200,140,500);  // measured in inverse megaKelvin (= 1,000,000 / Kelvin) COM:  measured in inverse megaKelvin (= 1,000,000 / Kelvin)
  CREATE_CHAR(uint8_t,ContactSensorState,1,0,1,DETECTED,NOT_DETECTED);  // indictates if contact is detected (i.e. closed) COM:  indictates if contact is detected (i.e. closed)
  CREATE_CHAR(const char *,ConfiguredName,"unnamed",0,1); COM: 
  CREATE_CHAR(double,CurrentAmbientLightLevel,1,0.0001,100000);   // measured in Lux (lumens/m<sup>2</sup> COM:  measured in Lux (lumens/m<sup>2</sup>
  CREATE_CHAR(int,CurrentHorizontalTiltAngle,0,-90,90);  // current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90)  COM:  current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90) 
  CREATE_CHAR(uint8_t,CurrentAirPurifierState,1,0,2,INACTIVE,IDLE,PURIFYING);  // indicates current state of air purification COM:  indicates current state of air purification
  CREATE_CHAR(uint8_t,CurrentSlatState,0,0,2,FIXED,JAMMED,SWINGING);  // indicates current state of slats COM:  indicates current state of slats
  CREATE_CHAR(uint8_t,CurrentPosition,0,0,100); // current position (as a percentage) from fully closed (0) to full open (100) COM:  current position (as a percentage) from fully closed (0) to full open (100)
  CREATE_CHAR(int,CurrentVerticalTiltAngle,0,-90,90);  // current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90) COM:  current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)
  CREATE_CHAR(uint8_t,CurrentVisibilityState,0,0,1); COM: 
  CREATE_CHAR(uint8_t,CurrentHumidifierDehumidifierState,1,0,3,INACTIVE,IDLE,HUMIDIFYING,DEHUMIDIFYING); // indicates current state of humidifier/dehumidifer COM:  indicates current state of humidifier/dehumidifer
  CREATE_CHAR(uint8_t,CurrentDoorState,1,0,4,OPEN,CLOSED,OPENING,CLOSING,STOPPED);  // indicates current state of a door COM:  indicates current state of a door
  CREATE_CHAR(uint8_t,CurrentFanState,1,0,2,INACTIVE,IDLE,BLOWING);  // indicates current state of a fan COM:  indicates current state of a fan
  CREATE_CHAR(uint8_t,CurrentHeatingCoolingState,0,0,2,IDLE,HEATING,COOLING); // indicates whether appliance is currently heating, cooling, or just idle COM:  indicates whether appliance is currently heating, cooling, or just idle
  CREATE_CHAR(uint8_t,CurrentHeaterCoolerState,1,0,3,INACTIVE,IDLE,HEATING,COOLING);  // indicates whether appliance is currently heating, cooling, idle, or off COM:  indicates whether appliance is currently heating, cooling, idle, or off
  CREATE_CHAR(uint8_t,CurrentMediaState,0,0,5); COM: 
  CREATE_CHAR(double,CurrentRelativeHumidity,0,0,100);  //current humidity measured as a percentage COM: current humidity measured as a percentage
  CREATE_CHAR(double,CurrentTemperature,0,0,100);   // current temperature measured in Celsius COM:  current temperature measured in Celsius
  CREATE_CHAR(int,CurrentTiltAngle,0,-90,90);  // angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90) COM:  angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90)
  CREATE_CHAR(double,FilterLifeLevel,0,0,100); // measures as a percentage of remaining life COM:  measures as a percentage of remaining life
  CREATE_CHAR(uint8_t,FilterChangeIndication,0,0,1,NO_CHANGE_NEEDED,CHANGE_NEEDED);  // indicates state of filter COM:  indicates state of filter
  CREATE_CHAR(const char *,FirmwareRevision,"1.0.0",0,1);  // must be in form x[.y[.z]] - informational only COM:  must be in form x[.y[.z]] - informational only
  CREATE_CHAR(const char *,HardwareRevision,"1.0.0",0,1);  // must be in form x[.y[.z]] - informational only COM:  must be in form x[.y[.z]] - informational only
  CREATE_CHAR(double,HeatingThresholdTemperature,16,0,25); // heating turns on when temperature (in Celsius) falls below this threshold COM:  heating turns on when temperature (in Celsius) falls below this threshold
  CREATE_CHAR(boolean,HoldPosition,false,0,1);  // deprecated COM:  deprecated
  CREATE_CHAR(double,Hue,0,0,360);  // color (in degrees) from red (0) to green (120) to blue (240) and back to red (360) COM:  color (in degrees) from red (0) to green (120) to blue (240) and back to red (360)
  CREATE_CHAR(boolean,Identify,false,0,1,IDLE,RUN_ID);  // the Home App set this to RUN_ID when it wants the device to run its identification routine COM:  the Home App set this to RUN_ID when it wants the device to run its identification routine
  CREATE_CHAR(uint32_t,Identifier,0,0,255); COM: 
  CREATE_CHAR(uint8_t,InputDeviceType,0,0,6); COM: 
  CREATE_CHAR(uint8_t,InputSourceType,0,0,10); COM: 
  CREATE_CHAR(uint8_t,InUse,0,0,1,NOT_IN_USE,IN_USE);   // if Service is set to active, this indictes whether it is currently in use COM:  if Service is set to active, this indictes whether it is currently in use
  CREATE_CHAR(uint8_t,IsConfigured,0,0,1,NOT_CONFIGURED,CONFIGURED);  // indicates if a predefined Service has been configured COM:  indicates if a predefined Service has been configured
  CREATE_CHAR(uint8_t,LeakDetected,0,0,1,NOT_DETECTED,DETECTED);  // indictates if a leak is detected COM:  indictates if a leak is detected
  CREATE_CHAR(uint8_t,LockCurrentState,0,0,3,UNLOCKED,LOCKED,JAMMED,UNKNOWN);  // indictates state of a lock COM:  indictates state of a lock
  CREATE_CHAR(uint8_t,LockPhysicalControls,0,0,1,CONTROL_LOCK_DISABLED,CONTROL_LOCK_ENABLED);  // indicates if local control lock is enabled COM:  indicates if local control lock is enabled
  CREATE_CHAR(uint8_t,LockTargetState,0,0,1,UNLOCK,LOCK);   // indicates desired state of lock COM:  indicates desired state of lock
  CREATE_CHAR(const char *,Manufacturer,"HomeSpan",0,1);  // any string - informational only COM:  any string - informational only
  CREATE_CHAR(const char *,Model,"HomeSpan-ESP32",0,1);  // any string - informational only COM:  any string - informational only
  CREATE_CHAR(boolean,MotionDetected,false,0,1,NOT_DETECTED,DETECTED);  // indicates if motion is detected COM:  indicates if motion is detected
  CREATE_CHAR(boolean,Mute,false,0,1,OFF,ON); // not used COM:  not used
  CREATE_CHAR(const char *,Name,"unnamed",0,1); // default name of a Service used <i>only</i> during initial pairing COM:  default name of a Service used <i>only</i> during initial pairing
  CREATE_CHAR(double,NitrogenDioxideDensity,0,0,1000);  // measured in micrograms/m<sup>3</sup> COM:  measured in micrograms/m<sup>3</sup>
  CREATE_CHAR(boolean,ObstructionDetected,false,0,1,NOT_DETECTED,DETECTED);  // indicates if obstruction is detected COM:  indicates if obstruction is detected
  CREATE_CHAR(double,PM25Density,0,0,1000); // 2.5-micron particulate density, measured in micrograms/m<sup>3</sup> COM:  2.5-micron particulate density, measured in micrograms/m<sup>3</sup>
  CREATE_CHAR(uint8_t,OccupancyDetected,0,0,1,NOT_DETECTED,DETECTED);  // indicates if occupanccy is detected COM:  indicates if occupanccy is detected
  CREATE_CHAR(boolean,OutletInUse,false,0,1,NOT_IN_USE,IN_USE); // indicates if an appliance or light is plugged into the outlet, regardless of whether on or off  COM:  indicates if an appliance or light is plugged into the outlet, regardless of whether on or off 
  CREATE_CHAR(boolean,On,false,0,1,OFF,ON);  // indicates if the Service is active/on COM:  indicates if the Service is active/on
  CREATE_CHAR(double,OzoneDensity,0,0,1000);  // measured in micrograms/m<sup>3</sup> COM:  measured in micrograms/m<sup>3</sup>
  CREATE_CHAR(uint8_t,PictureMode,0,0,13); COM: 
  CREATE_CHAR(double,PM10Density,0,0,1000);  // 10-micron particulate density, measured in micrograms/m<sup>3</sup> COM:  10-micron particulate density, measured in micrograms/m<sup>3</sup>
  CREATE_CHAR(uint8_t,PositionState,2,0,2,GOING_TO_MINIMUM,GOING_TO_MAXIMUM,STOPPED);  // deprecated COM:  deprecated
  CREATE_CHAR(uint8_t,PowerModeSelection,0,0,1); COM: 
  CREATE_CHAR(uint8_t,ProgramMode,0,0,2,NONE,SCHEDULED,SCHEDULE_OVERRIDEN); COM: 
  CREATE_CHAR(uint8_t,ProgrammableSwitchEvent,0,0,2,SINGLE_PRESS,DOUBLE_PRESS,LONG_PRESS); COM: 
  CREATE_CHAR(double,RelativeHumidityDehumidifierThreshold,50,0,100); COM: 
  CREATE_CHAR(double,RelativeHumidityHumidifierThreshold,50,0,100); COM: 
  CREATE_CHAR(uint32_t,RemainingDuration,60,0,3600); COM: 
  CREATE_CHAR(uint8_t,RemoteKey,0,0,16); COM: 
  CREATE_CHAR(uint8_t,ResetFilterIndication,0,1,1); COM: 
  CREATE_CHAR(int,RotationDirection,0,0,1,CLOCKWISE,COUNTERCLOCKWISE); COM: 
  CREATE_CHAR(double,RotationSpeed,0,0,100); COM: 
  CREATE_CHAR(double,Saturation,0,0,100); COM: 
  CREATE_CHAR(uint8_t,SecuritySystemAlarmType,0,0,1,KNOWN,UNKNOWN); COM: 
  CREATE_CHAR(uint8_t,SecuritySystemCurrentState,3,0,4,ARMED_STAY,ARMED_AWAY,ARMED_NIGHT,DISARMED,ALARM_TRIGGERED); COM: 
  CREATE_CHAR(uint8_t,SecuritySystemTargetState,3,0,3,ARM_STAY,ARM_AWAY,ARM_NIGHT,DISARM);  COM: 
  CREATE_CHAR(const char *,SerialNumber,"HS-12345",0,1); COM: 
  CREATE_CHAR(uint8_t,ServiceLabelIndex,1,1,255); COM: 
  CREATE_CHAR(uint8_t,ServiceLabelNamespace,1,0,1,DOTS,NUMERALS); COM: 
  CREATE_CHAR(uint8_t,SlatType,0,0,1,HORIZONTAL,VERTICAL); COM: 
  CREATE_CHAR(uint8_t,SleepDiscoveryMode,0,0,1); COM: 
  CREATE_CHAR(uint8_t,SmokeDetected,0,0,1,NOT_DETECTED,DETECTED); COM: 
  CREATE_CHAR(boolean,StatusActive,true,0,1); COM: 
  CREATE_CHAR(uint8_t,StatusFault,0,0,1,NO_FAULT,FAULT); COM: 
  CREATE_CHAR(uint8_t,StatusJammed,0,0,1,NOT_JAMMED,JAMMED); COM: 
  CREATE_CHAR(uint8_t,StatusLowBattery,0,0,1,NOT_LOW_BATTERY,LOW_BATTERY); COM: 
  CREATE_CHAR(uint8_t,StatusTampered,0,0,1,NOT_TAMPERED,TAMPERED); COM: 
  CREATE_CHAR(double,SulphurDioxideDensity,0,0,1000); COM: 
  CREATE_CHAR(uint8_t,SwingMode,0,0,1,SWING_DISABLED,SWING_ENABLED); COM: 
  CREATE_CHAR(uint8_t,TargetAirPurifierState,1,0,1,MANUAL,AUTO); COM: 
  CREATE_CHAR(uint8_t,TargetFanState,1,0,1,MANUAL,AUTO); COM: 
  CREATE_CHAR(int,TargetTiltAngle,0,-90,90); COM: 
  CREATE_CHAR(uint8_t,TargetHeaterCoolerState,0,0,2,AUTO,HEAT,COOL); COM: 
  CREATE_CHAR(uint32_t,SetDuration,60,0,3600); COM: 
  CREATE_CHAR(int,TargetHorizontalTiltAngle,0,-90,90); COM: 
  CREATE_CHAR(uint8_t,TargetHumidifierDehumidifierState,0,0,2,AUTO,HUMIDIFY,DEHUMIDIFY); COM: 
  CREATE_CHAR(uint8_t,TargetPosition,0,0,100); COM: 
  CREATE_CHAR(uint8_t,TargetDoorState,1,0,1,OPEN,CLOSED); COM: 
  CREATE_CHAR(uint8_t,TargetHeatingCoolingState,0,0,3,OFF,HEAT,COOL,AUTO); COM: 
  CREATE_CHAR(uint8_t,TargetMediaState,0,0,2); COM: 
  CREATE_CHAR(double,TargetRelativeHumidity,0,0,100); COM: 
  CREATE_CHAR(double,TargetTemperature,16,10,38); COM: 
  CREATE_CHAR(uint8_t,TargetVisibilityState,0,0,1); COM: 
  CREATE_CHAR(uint8_t,TemperatureDisplayUnits,0,0,1,CELSIUS,FAHRENHEIT); COM: 
  CREATE_CHAR(int,TargetVerticalTiltAngle,0,-90,90); COM: 
  CREATE_CHAR(uint8_t,ValveType,0,0,3); COM: 
  CREATE_CHAR(const char *,Version,"1.0.0",0,1); COM: 
  CREATE_CHAR(double,VOCDensity,0,0,1000); COM: 
  CREATE_CHAR(uint8_t,Volume,0,0,100); COM: 
  CREATE_CHAR(uint8_t,VolumeControlType,0,0,3); COM: 
  CREATE_CHAR(uint8_t,VolumeSelector,0,0,1); COM: 
  CREATE_CHAR(double,WaterLevel,0,0,100); COM: 
## AccessoryInformation (3E)
<details><summary> Required Identification Information.  For each Accessory in a HomeSpan device this <i>must</i> be included as the first Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Identify</td><td align="center">14</td><td align="center">bool</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>IDLE (0)</li><li>RUN_ID (1)</li></ul></td><td> the Home App set this to RUN_ID when it wants the device to run its identification routine</td></tr>
<tr><td>FirmwareRevision</td><td align="center">52</td><td align="center">string</td><td align="center">PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> must be in form x[.y[.z]] - informational only</td></tr>
<tr><td>Manufacturer</td><td align="center">20</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> any string - informational only</td></tr>
<tr><td>Model</td><td align="center">21</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> any string - informational only</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>SerialNumber</td><td align="center">30</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td></td></tr>
<tr><td>HardwareRevision</td><td align="center">53</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> must be in form x[.y[.z]] - informational only</td></tr>
</table></details>

## AirPurifier (BB)
<details><summary> Defines a basic Air Purifier with an optional fan.  Optional Linked Services: <b>FilterMaintenance</b>, <b>AirQualitySensor</b>, <b>Fan</b>, and <b>Slat</b></summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>CurrentAirPurifierState</td><td align="center">A9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>PURIFYING (2)</li></ul></td><td> indicates current state of air purification</td></tr>
<tr><td><b>TargetAirPurifierState</td><td align="center">A8</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL (0)</li><li>AUTO (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td><td></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td><td> indicates if local control lock is enabled</td></tr>
</table></details>

## AirQualitySensor (8D)
<details><summary> Defines an Air Quality Sensor. </summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>AirQuality</td><td align="center">95</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">5</td><td><ul><li>UNKNOWN (0)</li><li>EXCELLENT (1)</li><li>GOOD (2)</li><li>FAIR (3)</li><li>INFERIOR (4)</li><li>POOR (5)</li></ul></td><td> a subjective description</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>OzoneDensity</td><td align="center">C3</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td> measured in micrograms/m<sup>3</sup></td></tr>
<tr><td>NitrogenDioxideDensity</td><td align="center">C4</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td> measured in micrograms/m<sup>3</sup></td></tr>
<tr><td>SulphurDioxideDensity</td><td align="center">C5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td></td></tr>
<tr><td>PM25Density</td><td align="center">C6</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td> 2.5-micron particulate density, measured in micrograms/m<sup>3</sup></td></tr>
<tr><td>PM10Density</td><td align="center">C7</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td> 10-micron particulate density, measured in micrograms/m<sup>3</sup></td></tr>
<tr><td>VOCDensity</td><td align="center">C8</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1000</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## BatteryService (96)
<details><summary> Defines a standalone Battery Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>BatteryLevel</td><td align="center">68</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> measured as a percentage</td></tr>
<tr><td><b>ChargingState</td><td align="center">8F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NOT_CHARGING (0)</li><li>CHARGING (1)</li><li>NOT_CHARGEABLE (2)</li></ul></td><td> indicates state of battery charging</td></tr>
<tr><td><b>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## CarbonDioxideSensor (97)
<details><summary> Defines a Carbon Dioxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CarbonDioxideDetected</td><td align="center">92</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NORMAL (0)</li><li>ABNORMAL (1)</li></ul></td><td> indicates if abnormal level is detected</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
<tr><td>CarbonDioxideLevel</td><td align="center">93</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td><ul></ul></td><td> measured on parts per million (ppm)</td></tr>
<tr><td>CarbonDioxidePeakLevel</td><td align="center">94</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100000</td><td><ul></ul></td><td> measured in parts per million (ppm)</td></tr>
</table></details>

## CarbonMonoxideSensor (7F)
<details><summary> Defines a Carbon Monoxide Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CarbonMonoxideDetected</td><td align="center">69</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NORMAL (0)</li><li>ABNORMAL (1)</li></ul></td><td> indicates if abnormal level is detected</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
<tr><td>CarbonMonoxideLevel</td><td align="center">90</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> measured in parts per million (ppm)</td></tr>
<tr><td>CarbonMonoxidePeakLevel</td><td align="center">91</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> measured in parts per million (ppm)</td></tr>
</table></details>

## ContactSensor (80)
<details><summary> Defines a Contact Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>ContactSensorState</td><td align="center">6A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>DETECTED (0)</li><li>NOT_DETECTED (1)</li></ul></td><td> indictates if contact is detected (i.e. closed)</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## Door (81)
<details><summary> Defines a motorized Door.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentPosition</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current position (as a percentage) from fully closed (0) to full open (100)</td></tr>
<tr><td><b>TargetPosition</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if obstruction is detected</td></tr>
</table></details>

## Doorbell (121)
<details><summary> Defines a Doorbell.  Can be used on a standalone basis or in conjunction with a <b>LockMechanism</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>ProgrammableSwitchEvent</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## Fan (B7)
<details><summary> Defines a Fan.  Can be used in conjunction with a <b>LightBulb</b> Service to create a Lighted Ceiling Fan.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>CurrentFanState</td><td align="center">AF</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>BLOWING (2)</li></ul></td><td> indicates current state of a fan</td></tr>
<tr><td>TargetFanState</td><td align="center">BF</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>MANUAL (0)</li><li>AUTO (1)</li></ul></td><td></td></tr>
<tr><td>RotationDirection</td><td align="center">28</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CLOCKWISE (0)</li><li>COUNTERCLOCKWISE (1)</li></ul></td><td></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td><td></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td><td> indicates if local control lock is enabled</td></tr>
</table></details>

## Faucet (D7)
<details><summary> Defines the master control for a multi-Valve appliance.  Linked Services: <b>Valve</b> (at least one <i>required</i>), and <b>HeaterCooler</b> (optional).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## FilterMaintenance (BA)
<details><summary> Defines a Filter Maintainence check.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>FilterChangeIndication</td><td align="center">AC</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_CHANGE_NEEDED (0)</li><li>CHANGE_NEEDED (1)</li></ul></td><td> indicates state of filter</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>FilterLifeLevel</td><td align="center">AB</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> measures as a percentage of remaining life</td></tr>
<tr><td>ResetFilterIndication</td><td align="center">AD</td><td align="center">uint8</td><td align="center">PW</td><td align="center">1</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
</table></details>

## GarageDoorOpener (41)
<details><summary> Defines a motorized Garage Door Opener.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentDoorState</td><td align="center">E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li><li>OPENING (2)</li><li>CLOSING (3)</li><li>STOPPED (4)</li></ul></td><td> indicates current state of a door</td></tr>
<tr><td><b>TargetDoorState</td><td align="center">32</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OPEN (0)</li><li>CLOSED (1)</li></ul></td><td></td></tr>
<tr><td><b>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if obstruction is detected</td></tr>
<tr><td>LockCurrentState</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td><td> indictates state of a lock</td></tr>
<tr><td>LockTargetState</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td><td> indicates desired state of lock</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td><b>Version</td><td align="center">37</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td></td></tr>
</table></details>

## HeaterCooler (BC)
<details><summary> Defines a standalone Heater, Cooler, or combined Heater/Cooler.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>CurrentTemperature</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current temperature measured in Celsius</td></tr>
<tr><td><b>CurrentHeaterCoolerState</td><td align="center">B1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>HEATING (2)</li><li>COOLING (3)</li></ul></td><td> indicates whether appliance is currently heating, cooling, idle, or off</td></tr>
<tr><td><b>TargetHeaterCoolerState</td><td align="center">B2</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>AUTO (0)</li><li>HEAT (1)</li><li>COOL (2)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>TemperatureDisplayUnits</td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CELSIUS (0)</li><li>FAHRENHEIT (1)</li></ul></td><td></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td><td></td></tr>
<tr><td>CoolingThresholdTemperature</td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td><ul></ul></td><td> cooling turns on when temperature (in Celsius) rises above this threshold</td></tr>
<tr><td>HeatingThresholdTemperature</td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td><ul></ul></td><td> heating turns on when temperature (in Celsius) falls below this threshold</td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td><td> indicates if local control lock is enabled</td></tr>
</table></details>

## HumidifierDehumidifier (BD)
<details><summary> Defines a Humidifer, Dehumidifier, or combined Humidifer/Dehumidifier.  Can be used with a separate <b>Fan</b> Service and/or <b>Slat</b> Service to extend functionality.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>CurrentRelativeHumidity</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td>current humidity measured as a percentage</td></tr>
<tr><td><b>CurrentHumidifierDehumidifierState</td><td align="center">B3</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>INACTIVE (0)</li><li>IDLE (1)</li><li>HUMIDIFYING (2)</li><li>DEHUMIDIFYING (3)</li></ul></td><td> indicates current state of humidifier/dehumidifer</td></tr>
<tr><td><b>TargetHumidifierDehumidifierState</td><td align="center">B4</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>AUTO (0)</li><li>HUMIDIFY (1)</li><li>DEHUMIDIFY (2)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>RelativeHumidityDehumidifierThreshold</td><td align="center">C9</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>RelativeHumidityHumidifierThreshold</td><td align="center">CA</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>RotationSpeed</td><td align="center">29</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td><td></td></tr>
<tr><td>WaterLevel</td><td align="center">B5</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>LockPhysicalControls</td><td align="center">A7</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CONTROL_LOCK_DISABLED (0)</li><li>CONTROL_LOCK_ENABLED (1)</li></ul></td><td> indicates if local control lock is enabled</td></tr>
</table></details>

## HumiditySensor (82)
<details><summary> Defines a Humidity Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentRelativeHumidity</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td>current humidity measured as a percentage</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## InputSource (D9)
<details><summary> Defines an Input Source for a TV.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td></td></tr>
<tr><td>IsConfigured</td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_CONFIGURED (0)</li><li>CONFIGURED (1)</li></ul></td><td> indicates if a predefined Service has been configured</td></tr>
<tr><td><b>Identifier</td><td align="center">E6</td><td align="center">uint32</td><td align="center">PR</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td><td></td></tr>
<tr><td>CurrentVisibilityState</td><td align="center">135</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>TargetVisibilityState</td><td align="center">134</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
</table></details>

## IrrigationSystem (CF)
<details><summary> Defines an Irrigation System.  Linked Services: <b>Valve</b> Service (at least one <i>required</i>).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>ProgramMode</td><td align="center">D1</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>NONE (0)</li><li>SCHEDULED (1)</li><li>SCHEDULE_OVERRIDEN (2)</li></ul></td><td></td></tr>
<tr><td><b>InUse</td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td><td> if Service is set to active, this indictes whether it is currently in use</td></tr>
<tr><td>RemainingDuration</td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
</table></details>

## LeakSensor (83)
<details><summary> Defines a Leak Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>LeakDetected</td><td align="center">70</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indictates if a leak is detected</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## LightBulb (43)
<details><summary> Defines any type of Light.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>On</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td>Brightness</td><td align="center">8</td><td align="center">int</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> measured as a percentage</td></tr>
<tr><td>Hue</td><td align="center">13</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">360</td><td><ul></ul></td><td> color (in degrees) from red (0) to green (120) to blue (240) and back to red (360)</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>Saturation</td><td align="center">2F</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>ColorTemperature</td><td align="center">CE</td><td align="center">uint32</td><td align="center">PR+PW+EV</td><td align="center">140</td><td align="center">500</td><td><ul></ul></td><td> measured in inverse megaKelvin (= 1,000,000 / Kelvin)</td></tr>
</table></details>

## LightSensor (84)
<details><summary> Defines a Light Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentAmbientLightLevel</td><td align="center">6B</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0.0001</td><td align="center">100000</td><td><ul></ul></td><td> measured in Lux (lumens/m<sup>2</sup></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## LockMechanism (45)
<details><summary> Defines an electronic Lock.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>LockCurrentState</td><td align="center">1D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>UNLOCKED (0)</li><li>LOCKED (1)</li><li>JAMMED (2)</li><li>UNKNOWN (3)</li></ul></td><td> indictates state of a lock</td></tr>
<tr><td><b>LockTargetState</td><td align="center">1E</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>UNLOCK (0)</li><li>LOCK (1)</li></ul></td><td> indicates desired state of lock</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td><b>Mute</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td><td> not used</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
</table></details>

## MotionSensor (85)
<details><summary> Defines a Motion Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>MotionDetected</td><td align="center">22</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if motion is detected</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## OccupancySensor (86)
<details><summary> Defines and Occupancy Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>OccupancyDetected</td><td align="center">71</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if occupanccy is detected</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## Outlet (47)
<details><summary> Defines an controllable Outlet used to power any light or appliance.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>On</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>OutletInUse</td><td align="center">26</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td><td> indicates if an appliance or light is plugged into the outlet, regardless of whether on or off </td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## SecuritySystem (7E)
<details><summary> Defines a Security System.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>SecuritySystemCurrentState</td><td align="center">66</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">4</td><td><ul><li>ARMED_STAY (0)</li><li>ARMED_AWAY (1)</li><li>ARMED_NIGHT (2)</li><li>DISARMED (3)</li><li>ALARM_TRIGGERED (4)</li></ul></td><td></td></tr>
<tr><td><b>SecuritySystemTargetState</td><td align="center">67</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>ARM_STAY (0)</li><li>ARM_AWAY (1)</li><li>ARM_NIGHT (2)</li><li>DISARM (3)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>SecuritySystemAlarmType</td><td align="center">8E</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>KNOWN (0)</li><li>UNKNOWN (1)</li></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td><b>ServiceLabelNamespace</td><td align="center">CD</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>DOTS (0)</li><li>NUMERALS (1)</li></ul></td><td></td></tr>
</table></details>

## Slat (B9)
<details><summary> Defines a motorized ventilation Slat(s).</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentSlatState</td><td align="center">AA</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>FIXED (0)</li><li>JAMMED (1)</li><li>SWINGING (2)</li></ul></td><td> indicates current state of slats</td></tr>
<tr><td><b>SlatType</td><td align="center">C0</td><td align="center">uint8</td><td align="center">PR</td><td align="center">0</td><td align="center">1</td><td><ul><li>HORIZONTAL (0)</li><li>VERTICAL (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>SwingMode</td><td align="center">B6</td><td align="center">uint8</td><td align="center">PR+EV+PW</td><td align="center">0</td><td align="center">1</td><td><ul><li>SWING_DISABLED (0)</li><li>SWING_ENABLED (1)</li></ul></td><td></td></tr>
<tr><td>CurrentTiltAngle</td><td align="center">C1</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td> angle (in degrees) of slats from fully up or left (-90) to fully open (0) to fully down or right (90)</td></tr>
<tr><td>TargetTiltAngle</td><td align="center">C2</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td></td></tr>
</table></details>

## SmokeSensor (87)
<details><summary> Defines a Smoke Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>SmokeDetected</td><td align="center">76</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
<tr><td><b>Mute</td><td align="center">11A</td><td align="center">bool</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td><td> not used</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>Volume</td><td align="center">119</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
</table></details>

## StatelessProgrammableSwitch (89)
<details><summary> Defines a "Stateless" Programmable Switch that can be used to trigger actions in the Home App.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>ProgrammableSwitchEvent</td><td align="center">73</td><td align="center">uint8</td><td align="center">PR+EV+NV</td><td align="center">0</td><td align="center">2</td><td><ul><li>SINGLE_PRESS (0)</li><li>DOUBLE_PRESS (1)</li><li>LONG_PRESS (2)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## Switch (49)
<details><summary> Defines a generic Switch.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>On</td><td align="center">25</td><td align="center">bool</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>OFF (0)</li><li>ON (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## Television (D8)
<details><summary> Defines a TV.  Optional Linked Services: <b>InputSource</b> and <b>TelevisionSpeaker</b>.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td>ConfiguredName</td><td align="center">E3</td><td align="center">string</td><td align="center">PW+PR+EV</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td></td></tr>
<tr><td>ActiveIdentifier</td><td align="center">E7</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">255</td><td><ul></ul></td><td> the Identifier of the current Input Source</td></tr>
<tr><td>RemoteKey</td><td align="center">E1</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">16</td><td><ul></ul></td><td></td></tr>
<tr><td>PowerModeSelection</td><td align="center">DF</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
</table></details>

## TelevisionSpeaker (113)
<details><summary> Defines a Television Speaker that can be controlled via the Remote Control widget on an iPhone.  Use <i>only</i> as a Linked Service for the <b>Television</b> Service.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>VolumeControlType</td><td align="center">E9</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul></ul></td><td></td></tr>
<tr><td><b>VolumeSelector</td><td align="center">EA</td><td align="center">uint8</td><td align="center">PW</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
</table></details>

## TemperatureSensor (8A)
<details><summary> Defines a Temperature Sensor.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentTemperature</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current temperature measured in Celsius</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>StatusActive</td><td align="center">75</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul></ul></td><td></td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>StatusTampered</td><td align="center">7A</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_TAMPERED (0)</li><li>TAMPERED (1)</li></ul></td><td></td></tr>
<tr><td>StatusLowBattery</td><td align="center">79</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_LOW_BATTERY (0)</li><li>LOW_BATTERY (1)</li></ul></td><td></td></tr>
</table></details>

## Thermostat (4A)
<details><summary> Defines a Thermostat used to control a furnace, air conditioner, or both.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentHeatingCoolingState</td><td align="center">F</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">2</td><td><ul><li>IDLE (0)</li><li>HEATING (1)</li><li>COOLING (2)</li></ul></td><td> indicates whether appliance is currently heating, cooling, or just idle</td></tr>
<tr><td><b>TargetHeatingCoolingState</td><td align="center">33</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul><li>OFF (0)</li><li>HEAT (1)</li><li>COOL (2)</li><li>AUTO (3)</li></ul></td><td></td></tr>
<tr><td><b>CurrentTemperature</td><td align="center">11</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current temperature measured in Celsius</td></tr>
<tr><td><b>TargetTemperature</td><td align="center">35</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">10</td><td align="center">38</td><td><ul></ul></td><td></td></tr>
<tr><td><b>TemperatureDisplayUnits</td><td align="center">36</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>CELSIUS (0)</li><li>FAHRENHEIT (1)</li></ul></td><td></td></tr>
<tr><td>CoolingThresholdTemperature</td><td align="center">D</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">10</td><td align="center">35</td><td><ul></ul></td><td> cooling turns on when temperature (in Celsius) rises above this threshold</td></tr>
<tr><td>CurrentRelativeHumidity</td><td align="center">10</td><td align="center">float</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td>current humidity measured as a percentage</td></tr>
<tr><td>HeatingThresholdTemperature</td><td align="center">12</td><td align="center">float</td><td align="center">PR+PW+EV</td><td align="center">0</td><td align="center">25</td><td><ul></ul></td><td> heating turns on when temperature (in Celsius) falls below this threshold</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>TargetRelativeHumidity</td><td align="center">34</td><td align="center">float</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
</table></details>

## Valve (D0)
<details><summary> Defines an electronic Valve.  Can be used standalone or as a Linked Service in conjunction with the <b>Faucet</b> and <b>IrrigationSystem</b> Services.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>Active</td><td align="center">B0</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>INACTIVE (0)</li><li>ACTIVE (1)</li></ul></td><td> indicates if the Service is active/on</td></tr>
<tr><td><b>InUse</td><td align="center">D2</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_IN_USE (0)</li><li>IN_USE (1)</li></ul></td><td> if Service is set to active, this indictes whether it is currently in use</td></tr>
<tr><td><b>ValveType</td><td align="center">D5</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3</td><td><ul></ul></td><td></td></tr>
<tr><td>SetDuration</td><td align="center">D3</td><td align="center">uint32</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td><td></td></tr>
<tr><td>RemainingDuration</td><td align="center">D4</td><td align="center">uint32</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">3600</td><td><ul></ul></td><td></td></tr>
<tr><td>IsConfigured</td><td align="center">D6</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_CONFIGURED (0)</li><li>CONFIGURED (1)</li></ul></td><td> indicates if a predefined Service has been configured</td></tr>
<tr><td>StatusFault</td><td align="center">77</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NO_FAULT (0)</li><li>FAULT (1)</li></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
</table></details>

## Window (8B)
<details><summary> Defines a motorized Window.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>CurrentPosition</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current position (as a percentage) from fully closed (0) to full open (100)</td></tr>
<tr><td><b>TargetPosition</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if obstruction is detected</td></tr>
</table></details>

## WindowCovering (8C)
<details><summary> Defines a motorized Window Shade, Screen, Awning, etc.</summary><br><table>
<tr><th>Characteristic</th><th>UUID</th><th>Format</th><th>Perms</th><th>Min</th><th>Max</th><th>Constants</th><th>Notes</th></tr>
<tr><td><b>TargetPosition</td><td align="center">7C</td><td align="center">uint8</td><td align="center">PW+PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td></td></tr>
<tr><td><b>CurrentPosition</td><td align="center">6D</td><td align="center">uint8</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">100</td><td><ul></ul></td><td> current position (as a percentage) from fully closed (0) to full open (100)</td></tr>
<tr><td>Name</td><td align="center">23</td><td align="center">string</td><td align="center">PR</td><td align="center">-</td><td align="center">-</td><td><ul></ul></td><td> default name of a Service used <i>only</i> during initial pairing</td></tr>
<tr><td>CurrentHorizontalTiltAngle</td><td align="center">6C</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td> current angle (in degrees) of slats from fully up (-90) to fully open (0) to fully down (90) </td></tr>
<tr><td>TargetHorizontalTiltAngle</td><td align="center">7B</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td></td></tr>
<tr><td>CurrentVerticalTiltAngle</td><td align="center">6E</td><td align="center">int</td><td align="center">PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td> current angle (in degrees) of slats from fully left (-90) to fully open (0) to fully right (90)</td></tr>
<tr><td>TargetVerticalTiltAngle</td><td align="center">7D</td><td align="center">int</td><td align="center">PW+PR+EV</td><td align="center">-90</td><td align="center">90</td><td><ul></ul></td><td></td></tr>
<tr><td>ObstructionDetected</td><td align="center">24</td><td align="center">bool</td><td align="center">PR+EV</td><td align="center">0</td><td align="center">1</td><td><ul><li>NOT_DETECTED (0)</li><li>DETECTED (1)</li></ul></td><td> indicates if obstruction is detected</td></tr>
</table></details>

