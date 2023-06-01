/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023 Gregg E. Berman
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
 
// This example demonstrates how to control a real-world Stepper Motor using HomeSpan's
// StepperControl Class.  Note the StepperControl Class serves as a generic interface
// and cannot be instantiatiated directly.  Rather, you must create a child class derived
// from StepperClass that implements the details of a specific Stepper Motor Driver Board.

// In this example we will implement a child class of StepperControl designed to operate
// the Adafruit TB6612 1.2A DC/Stepper Motor Driver Breakout Board using only the 4 control
// pins AIN1, AIN2, BIN1, and BIN2 (https://www.adafruit.com/product/2448)

// The implementation of this class is found in the file "Stepper_TB6612.h" included
// with this sketch.  By separating the details of the Stepper Motor Driver Board from the
// HomeSpan logic below, you can easily change the code that operates the Stepper Motor Driver Board
// without very little modification to the HomeSpan sketch itself.

// Note this sketch is based on the WindowShade Accessory from Example 13.  Please review
// that Example first if new to HomeSpan since it is fully commented.

#include "HomeSpan.h"
#include "Stepper_TB6612.h"             // Here we include the implementation of thge Adafruit TB6612 Stepper Motor Driver Board

////////////////////////////////////

struct DEV_WindowShade : Service::WindowCovering {

  SpanCharacteristic *current;
  SpanCharacteristic *target;
  
  Stepper_TB6612 *motor;          // create pointer to TB6612 motor controller

  DEV_WindowShade(int a1, int a2, int b1, int b2) : Service::WindowCovering(){
        
    current=new Characteristic::CurrentPosition(0);    
    target=new Characteristic::TargetPosition(0);
    
    motor=new Stepper_TB6612(a1, a2, b1, b2);    // instantiate motor using pins specified in set-up below
    motor->setAccel(10,20);                      // set acceleration parameters
        
    LOG0("Configuring Motorized Window Shade with input pins: A1=%d, A2=%d, B1=%d, B2=%d\n",a1,a2,b1,b2);
  }

  ///////////
  
  boolean update(){

    // Move motor to absolute position, assuming 200 steps per revolution and 20 revolutions for full up/travel travel.
    // Specify that motor should enter the BRAKE state upon reaching to desired position.
    
    motor->moveTo(target->getNewVal()*20,5,Stepper_TB6612::BRAKE);
    LOG1("Setting Shade Position=%d\n",target->getNewVal());
    return(true);  
  }

  ///////////

  void loop(){

    // If the current window shade position does NOT equal the target position, BUT the motor has stopped moving,
    // we must have reached the target position, so set the current position equal to the target position
    
    if(current->getVal()!=target->getVal() && !motor->stepsRemaining()){
      current->setVal(target->getVal());
      LOG1("Motor Stopped at Shade Position=%d\n",current->getVal());
    }    
  }
  
};

////////////////////////////////////

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::WindowCoverings,"Motorized Shade");

  new SpanAccessory();                                                          
    new Service::AccessoryInformation();
      new Characteristic::Identify(); 
    new DEV_WindowShade(23,32,22,14);               // The TB6612 driver requires 4 digital pins: AIN1, AIN2, BIN1, and BIN2.  Change these pin numbers to match YOUR board!
}

//////////////////////////////////////

void loop(){
  
  homeSpan.poll();  
}
