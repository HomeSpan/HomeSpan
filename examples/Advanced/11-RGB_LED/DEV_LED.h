
////////////////////////////////////
//   DEVICE-SPECIFIC LED SERVICES //
////////////////////////////////////

#include "extras/PwmPin.h"                          // allows PWM control of LED brightness

struct DEV_LED : Service::LightBulb {               // ON/OFF LED

  int ledPin;                                       // pin number defined for this LED
  SpanCharacteristic *power;                        // reference to the On Characteristic
  
  DEV_LED(int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                 
    this->ledPin=ledPin;                            
    pinMode(ledPin,OUTPUT);                         
    
    Serial.print("Configuring On/Off LED: Pin=");   // initialization message
    Serial.print(ledPin);
    Serial.print("\n");

  } // end constructor

  StatusCode update(){                              // update() method

    LOG1("Updating On/Off LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->value.BOOL?"true":"false");
    LOG1("  New Power=");
    LOG1(power->newValue.BOOL?"true":"false");
    LOG1("\n");

    digitalWrite(ledPin,power->newValue.BOOL);      
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////

struct DEV_DimmableLED : Service::LightBulb {       // Dimmable LED

  PwmPin *pwmPin;                                   // reference to PWM Pin
  int ledPin;                                       // pin number defined for this LED <- NEW!!
  int channel;                                      // PWM channel used for this LED (should be unique for each LED)
  SpanCharacteristic *power;                        // reference to the On Characteristic
  SpanCharacteristic *level;                        // reference to the Brightness Characteristic
  
  DEV_DimmableLED(int channel, int ledPin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();     
                
    level=new Characteristic::Brightness(50);       // Brightness Characteristic with an initial value of 50%
    new SpanRange(5,100,1);                         // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->channel=channel;                          // save the channel number (from 0-15)
    this->ledPin=ledPin;                            // save LED pin number
    this->pwmPin=new PwmPin(channel, ledPin);       // configure the PWM channel and attach the specified ledPin

    Serial.print("Configuring Dimmable LED: Pin="); // initialization message
    Serial.print(ledPin);
    Serial.print(" Channel=");
    Serial.print(channel);
    Serial.print("\n");
    
  } // end constructor

  StatusCode update(){                              // update() method

    LOG1("Updating Dimmable LED on pin=");
    LOG1(ledPin);
    LOG1(":  Current Power=");
    LOG1(power->value.BOOL?"true":"false");
    LOG1(power->getVal<boolean>()?"true":"false");
    LOG1("  Current Brightness=");
    LOG1(level->getVal<int>());
  
    if(power->isUpdated){
      LOG1("  New Power=");
      LOG1(power->getNewVal<boolean>()?"true":"false");
    }

    if(level->isUpdated){
      LOG1("  New Brightness=");
      LOG1(level->getNewVal<boolean>());
    } 

    LOG1("\n");
    
    pwmPin->set(channel,power->getNewVal<boolean>()*level->getNewVal<boolean>());    
   
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////
struct DEV_RgbLED : Service::LightBulb {            // RGB LED (Command Cathode)

  PwmPin *redPin;                                   
  PwmPin *greenPin;                                 
  PwmPin *bluePin;
  int redChannel;                                   
  int greenChannel;                                   
  int blueChannel;                                   
  SpanCharacteristic *power;                   // reference to the On Characteristic
  SpanCharacteristic *H;                       // reference to the Hue Characteristic
  SpanCharacteristic *S;                       // reference to the Saturation Characteristic
  SpanCharacteristic *V;                       // reference to the Brightness Characteristic
  
  DEV_RgbLED(int redChannel, int greenChannel, int blueChannel, int redPin, int greenPin, int bluePin) : Service::LightBulb(){       // constructor() method

    power=new Characteristic::On();                    
    H=new Characteristic::Hue(0);              // instantiate the Hue Characteristic with an initial value of 0 out of 360
    S=new Characteristic::Saturation(0);       // instantiate the Saturation Characteristic with an initial value of 0%
    V=new Characteristic::Brightness(100);     // instantiate the Brightness Characteristic with an initial value of 100%
    new SpanRange(5,100,1);                    // sets the range of the Brightness to be from a min of 5%, to a max of 100%, in steps of 1%

    this->redChannel=redChannel;               // save the channel number (from 0-15)
    this->greenChannel=greenChannel;           // save the channel number (from 0-15)
    this->blueChannel=blueChannel;             // save the channel number (from 0-15)
    
    this->redPin=new PwmPin(redChannel, redPin);        // configure the PWM channel and attach the specified pin
    this->greenPin=new PwmPin(greenChannel, greenPin);  // configure the PWM channel and attach the specified pin
    this->bluePin=new PwmPin(blueChannel, bluePin);     // configure the PWM channel and attach the specified pin

    char cBuf[128];
    sprintf(cBuf,"Configuring RGB LED: Pins=(%d,%d,%d)  Channels=(%d,%d,%d)\n",redPin,greenPin,bluePin,redChannel,greenChannel,blueChannel);
    Serial.print(cBuf);
    
  } // end constructor

  StatusCode update(){                              // update() method

    boolean p;
    int v;
    double h, s, r, g, b;

    h=H->getVal<double>();       // get all current values
    s=S->getVal<double>();
    v=V->getVal<int>();
    p=power->getVal<boolean>();

    char cBuf[128];
    sprintf(cBuf,"Updating RGB LED on pins=(%d,%d,%d): ",redPin->getPin(),greenPin->getPin(),bluePin->getPin());
    LOG1(cBuf);

    if(power->isUpdated){
      p=power->getNewVal<boolean>();
      sprintf(cBuf,"Power=%s->%s, ",power->getVal<boolean>()?"true":"false",p?"true":"false");
    } else {
      sprintf(cBuf,"Power=%s, ",p?"true":"false");
    }
    LOG1(cBuf);
      
    if(H->isUpdated){
      h=H->getNewVal<double>();
      sprintf(cBuf,"H=%d->%d, ",(int)H->getVal<double>(),(int)h);
    } else {
      sprintf(cBuf,"H=%d, ",(int)h);
    }
    LOG1(cBuf);

    if(S->isUpdated){
      s=S->getNewVal<double>();
      sprintf(cBuf,"S=%d->%d, ",(int)S->getVal<double>(),(int)s);
    } else {
      sprintf(cBuf,"S=%d, ",(int)s);
    }
    LOG1(cBuf);

    if(V->isUpdated){
      v=V->getNewVal<int>();
      sprintf(cBuf,"V=%d->%d ",V->getVal<int>(),v);
    } else {
      sprintf(cBuf,"V=%d  ",v);
    }
    LOG1(cBuf);

    PwmPin::HSVtoRGB(h,s/100.0,v/100.0,&r,&g,&b);

    int R, G, B;

    R=p*r*100;
    G=p*g*100;
    B=p*b*100;

    sprintf(cBuf,"RGB=(%d,%d,%d)\n",R,G,B);
    LOG1(cBuf);

    redPin->set(redChannel,R);    
    greenPin->set(greenChannel,G);    
    bluePin->set(blueChannel,B);    
      
    return(StatusCode::OK);                         // return OK status code
  
  } // update
};
      
//////////////////////////////////
