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

////////////////////////////////////////////////////////////////////
//                                                                //
//        HomeSpan: A HomeKit implementation for the ESP32        //
//        ------------------------------------------------        //
//                                                                //
// Example 23: Demonstrates how to use the AdaptiveLighting       //
//             controller to implement Apple's HomeKit Adaptive   //
//             Lighting feature on a LightBulb Service that       //
//             supports both Brightness and ColorTemperature.     //
//                                                                //
////////////////////////////////////////////////////////////////////

#include "HomeSpan.h"

// Apple's Adaptive Lighting feature lets a HomeKit Controller (typically an
// iPhone, iPad, or HomePod acting as your home hub) upload a 24-hour color-
// temperature schedule to a compatible LightBulb.  HomeSpan implements the
// (undocumented) protocol via the AdaptiveLightingController class defined
// in "AdaptiveLighting.h" and included automatically from "HomeSpan.h".
//
// Prerequisites for a LightBulb to be eligible for Adaptive Lighting:
//   * It must expose an On Characteristic
//   * It must expose a Brightness Characteristic
//   * It must expose a ColorTemperature Characteristic
//   * The three Adaptive-Lighting Characteristics must be part of the Service.
//     (The AdaptiveLightingController creates them for you automatically!)
//
// See docs/AdaptiveLighting.md for full details on how the schedule is
// constructed and applied.

struct AdaptiveLight : Service::LightBulb {

  // The three "regular" LightBulb Characteristics.  These MUST be instantiated
  // BEFORE the AdaptiveLightingController below so the controller can capture
  // pointers to Brightness and ColorTemperature.
  SpanCharacteristic *power       = new Characteristic::On(0);
  SpanCharacteristic *brightness  = new Characteristic::Brightness(100);
  SpanCharacteristic *colorTemp   = new Characteristic::ColorTemperature(250);

  // The Adaptive Lighting controller.  Its constructor internally creates the
  // three (undocumented) Adaptive-Lighting Characteristics required by the
  // HAP protocol and attaches them to this Service.
  AdaptiveLightingController adaptiveLighting{brightness, colorTemp};

  AdaptiveLight() : Service::LightBulb() {

    Serial.printf("Configuring Adaptive-Lighting LightBulb\n");

    brightness->setRange(5, 100, 1);        // 5..100% in 1% steps
    colorTemp->setRange(140, 500, 1);       // full range: 7143 K ... 2000 K

    // Optional: get notified whenever the controller writes a new mired.
    adaptiveLighting.setColorTemperatureCallback([this](uint32_t newMired){
      LOG1("[AL] color temp -> %u mired (%d K)\n",
           (unsigned)newMired, (int)(1000000UL / newMired));
    });

    // Optional: get notified when Adaptive Lighting is enabled/disabled.
    adaptiveLighting.setStateCallback([](bool active){
      LOG0("[AL] Adaptive Lighting %s\n", active ? "ENABLED" : "DISABLED");
    });
  }

  // Called by HomeSpan whenever the Home App writes to one of our
  // Characteristics.  Simply forward the notification to the Adaptive-Lighting
  // controller (it will react to writes on its own control characteristic
  // and will disable itself if the user manually changes ColorTemperature).
  boolean update() override {
    adaptiveLighting.handleUpdate();

    // Your usual output logic goes here.  For demonstration purposes we just
    // log the resulting state.
    LOG1("[Light] on=%d  brightness=%d%%  colorTemp=%d mired\n",
         power->getNewVal<int>(),
         brightness->getNewVal<int>(),
         colorTemp->getNewVal<int>());

    return true;
  }

  // Called continuously by HomeSpan.  Delegate to the Adaptive-Lighting
  // controller so it can advance the schedule.
  void loop() override {
    adaptiveLighting.poll();
  }
};

// Global pointer so our CLI hook below can reach the controller.
AdaptiveLight *myLight = nullptr;

// Custom CLI command: type "@p" in the Serial Monitor to print the currently-
// loaded Adaptive-Lighting schedule.  Useful for inspecting what the Home
// Controller has uploaded to your device.
void printSchedule(const char *) {
  if (myLight) myLight->adaptiveLighting.printSchedule();
}

// Custom CLI command: type "@x" in the Serial Monitor to manually disable
// any active Adaptive-Lighting schedule.
void cancelSchedule(const char *) {
  if (myLight) myLight->adaptiveLighting.disable();
}

void setup() {

  Serial.begin(115200);

  homeSpan.begin(Category::Lighting, "Adaptive Light");

  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Adaptive Light");
    myLight = new AdaptiveLight();

  new SpanUserCommand('p', "print the current Adaptive-Lighting schedule", printSchedule);
  new SpanUserCommand('x', "cancel the active Adaptive-Lighting schedule",  cancelSchedule);
}

void loop() {
  homeSpan.poll();
}

////////////////////////////////////////////////////////////////////
//
// NOTES ON TIME:
//
// The Adaptive-Lighting protocol delivers a schedule anchored to a wall-clock
// timestamp (milliseconds since 2001-01-01 UTC).  The reference implementation
// used here treats that timestamp as opaque and simply advances the schedule
// using the ESP32's monotonic millis() clock, starting from the moment the
// schedule is received.  In practice this is close enough that a HomePod-
// generated schedule uploaded at 6:00 AM local time will produce a fairly
// natural warm-to-cool-to-warm cycle over the next 24 hours.
//
// If you want stricter alignment with the real wall clock, initialize an NTP
// client after WiFi is up and offset the local start time accordingly:
//
//     #include <time.h>
//     configTime(0, 0, "pool.ntp.org");    // set system time from NTP
//
// then decode the 8-byte startTimeBuf inside the schedule (little-endian
// int64_t milliseconds since 2001-01-01 UTC) and compare against the current
// UTC time from time().  See docs/AdaptiveLighting.md for a worked example.
//
////////////////////////////////////////////////////////////////////
