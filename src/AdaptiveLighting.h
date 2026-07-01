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

/*********************************************************************************
 *
 *  HomeSpan: Adaptive Lighting Controller
 *  --------------------------------------
 *
 *  Implements Apple's (undocumented) HomeKit Adaptive Lighting protocol on top
 *  of a LightBulb Service that exposes both `Brightness` and `ColorTemperature`
 *  Characteristics.  See docs/AdaptiveLighting.md for a full description of the
 *  protocol, the required Characteristics, and example usage.
 *
 *  The three (also undocumented) TLV8/UINT8 Characteristics that HomeKit uses
 *  to negotiate Adaptive Lighting are registered as first-class HomeSpan
 *  Characteristics elsewhere in this library:
 *
 *    Characteristic::SupportedCharacteristicValueTransitionConfiguration   0x144
 *    Characteristic::CharacteristicValueTransitionControl                  0x143
 *    Characteristic::CharacteristicValueActiveTransitionCount              0x24B
 *
 *  The AdaptiveLightingController creates those three Characteristics itself
 *  (they attach to whatever Service is currently being constructed), so from
 *  the sketch's perspective the only integration needed is to instantiate an
 *  AdaptiveLightingController inside a LightBulb Service and call its
 *  `handleUpdate()` from Service::update() and `poll()` from Service::loop().
 *
 ********************************************************************************/

#pragma once

#include <Arduino.h>
#include <cmath>
#include <cstring>
#include <functional>
#include <vector>

class AdaptiveLightingController {

public:

  // Callback signature invoked on every AL tick with the newly computed color
  // temperature (in mireds).  Set with setColorTemperatureCallback() and
  // useful for driving external hardware that does not read directly from
  // ColorTemperature->getVal().
  typedef std::function<void(uint32_t newMired)> ColorTemperatureCallback;

  // Callback signature invoked whenever a new schedule is accepted (arg=true)
  // or Adaptive Lighting is disabled (arg=false).  Set with setStateCallback().
  typedef std::function<void(bool active)> StateCallback;

  ////////////////////////////////////////////////////////////////////////////
  // Construction
  //
  // Instantiate this object *inside* a Service::LightBulb (or derived Service)
  // AFTER the Brightness and ColorTemperature Characteristics for that Service
  // have been constructed.  The controller instantiates the three Apple
  // Adaptive-Lighting Characteristics itself; they attach to the same Service.
  ////////////////////////////////////////////////////////////////////////////

  AdaptiveLightingController(SpanCharacteristic *brightness,
                             SpanCharacteristic *colorTemperature);

  ////////////////////////////////////////////////////////////////////////////
  // Sketch hooks
  ////////////////////////////////////////////////////////////////////////////

  // Call from your Service::update() override (in any order relative to your
  // own logic).  Returns true if a control-point write from the HomeKit
  // controller was processed this cycle (i.e. a schedule was loaded, refreshed,
  // or disabled).
  boolean handleUpdate();

  // Call from your Service::loop() override or from the main Arduino loop().
  // Cheap when no schedule is loaded; otherwise re-evaluates the current
  // point on the curve every updateInterval ms (typically once every minute).
  void poll();

  ////////////////////////////////////////////////////////////////////////////
  // Optional configuration
  ////////////////////////////////////////////////////////////////////////////

  // Register a callback invoked whenever the controller writes a new color
  // temperature to the ColorTemperature Characteristic.
  void setColorTemperatureCallback(ColorTemperatureCallback cb) { onColorTemp=cb; }

  // Register a callback invoked when Adaptive Lighting becomes active
  // (arg=true, after a new schedule is loaded) or inactive (arg=false).
  void setStateCallback(StateCallback cb) { onState=cb; }

  ////////////////////////////////////////////////////////////////////////////
  // Query / control
  ////////////////////////////////////////////////////////////////////////////

  // Returns true if a schedule is currently loaded.
  boolean isActive() const { return !active.curve.empty(); }

  // Cancel any active schedule and notify HomeKit.
  void disable();

  // Print the currently-loaded schedule to Serial (does nothing if inactive).
  void printSchedule() const;

  // Access to the underlying Characteristic objects (also created by this
  // controller) in case the sketch wishes to interact with them directly.
  SpanCharacteristic *getSupportedConfig()      const { return supp;  }
  SpanCharacteristic *getTransitionControl()    const { return ctl;   }
  SpanCharacteristic *getActiveTransitionCount() const { return count; }

private:

  // -------- TLV8 tag definitions (mirrors HAP-NodeJS) ----------------------
  //
  // Apple has never published the schema for these Characteristics.  These
  // tag numbers are taken from Homebridge's HAP-NodeJS reference
  // implementation and have been verified against the traffic produced by
  // real HomeKit controllers.

  enum TransitionType : uint8_t {
    TT_BRIGHTNESS        = 0x01,
    TT_COLOR_TEMPERATURE = 0x02,
  };

  enum SuppCfgTag : uint8_t {
    SUPP_TRANSITION           = 0x01,   // outer, one sub-TLV per supported Characteristic
  };
  enum SuppInnerTag : uint8_t {
    SUPP_CHAR_IID             = 0x01,
    SUPP_TRANSITION_TYPE      = 0x02,
  };

  enum ControlTag : uint8_t {
    CTL_READ_CURRENT          = 0x01,
    CTL_UPDATE                = 0x02,
  };
  enum ReadReqTag : uint8_t {
    READ_CHAR_IID             = 0x01,
  };
  enum UpdateReqTag : uint8_t {
    UPD_VALUE_CFG             = 0x01,
  };
  enum ValueCfgTag : uint8_t {
    VC_CHAR_IID               = 0x01,
    VC_TRANSITION_PARAMS      = 0x02,
    VC_ENABLE_FLAG            = 0x03,   // uint8; 1 == enable, absent/0 == disable
    VC_CURVE_CFG              = 0x05,
    VC_UPDATE_INTERVAL        = 0x06,   // uint16 (ms)
    VC_NOTIFY_THRESHOLD       = 0x08,   // uint32 (ms)
  };
  enum ParamsTag : uint8_t {
    P_TRANSITION_ID           = 0x01,   // 16 bytes
    P_START_TIME              = 0x02,   // 8 bytes (ms since 2001-01-01 UTC, LE)
    P_START_ID3               = 0x03,   // 8 bytes (optional)
  };
  enum CurveCfgTag : uint8_t {
    CC_ENTRY                  = 0x01,
    CC_ADJUSTMENT_CHAR_IID    = 0x02,
    CC_ADJUSTMENT_RANGE       = 0x03,
  };
  enum EntryTag : uint8_t {
    E_ADJUSTMENT_FACTOR       = 0x01,   // float32 LE
    E_VALUE                   = 0x02,   // float32 LE (mireds)
    E_TRANSITION_OFFSET       = 0x03,   // varuint (ms)
    E_DURATION                = 0x04,   // varuint (ms, optional)
  };
  enum RangeTag : uint8_t {
    R_MIN                     = 0x01,   // uint32
    R_MAX                     = 0x02,   // uint32
  };

  enum RespTag : uint8_t {
    R_VALUE_CFG_STATUS        = 0x01,
  };
  enum StatusTag : uint8_t {
    S_CHAR_IID                = 0x01,
    S_TRANSITION_PARAMS       = 0x02,
    S_TIME_SINCE_START        = 0x03,
  };

  // -------- In-memory schedule ---------------------------------------------

  struct CurveEntry {
    float    temperature      = 0;      // mireds
    float    brightnessFactor = 0;      // mireds per brightness percent
    uint32_t transitionTimeMs = 0;      // interpolate over this many ms
    uint32_t durationMs       = 0;      // hold this value for this many ms before next transition
  };

  struct ActiveSchedule {
    uint32_t colorTempIID    = 0;
    uint32_t brightnessIID   = 0;

    uint8_t  transitionId[16] = {0};    // opaque 16-byte id from controller
    uint8_t  startTimeBuf[8]  = {0};    // opaque 8-byte "ms since 2001-01-01 UTC"
    boolean  hasId3           = false;
    uint8_t  id3[8]           = {0};

    uint32_t minBrightness    = 10;
    uint32_t maxBrightness    = 100;
    uint16_t updateIntervalMs = 60000;      // typical: 60 s
    uint32_t notifyIntervalMs = 600000;     // typical: 10 min

    uint32_t localStartMs     = 0;          // millis() at moment of schedule load

    std::vector<CurveEntry> curve;
  };

  // -------- Helpers --------------------------------------------------------

  static uint64_t readVarUInt(const uint8_t *p, size_t len);
  static float    readFloat32LE(const uint8_t *p, size_t len);

  void publishSupportedConfig();
  void clearControlValue();

  void processControlWrite();
  void handleUpdateTransition(TLV8 &updateTlv, TLV8 &response);
  void appendReadCurrentResponse(TLV8 &response);
  void buildTransitionParams(TLV8 &out) const;
  void buildValueConfigurationStatus(TLV8 &out, int64_t timeSinceStartMs) const;

  boolean interpolate(int64_t timeSinceStartMs,
                      float &outTemperature,
                      float &outFactor) const;
  void computeAndApply();

  // -------- Members --------------------------------------------------------

  SpanCharacteristic *bright = nullptr;   // borrowed
  SpanCharacteristic *temp   = nullptr;   // borrowed
  SpanCharacteristic *supp   = nullptr;   // owned (attached to service)
  SpanCharacteristic *ctl    = nullptr;   // owned
  SpanCharacteristic *count  = nullptr;   // owned

  ColorTemperatureCallback onColorTemp;
  StateCallback            onState;

  ActiveSchedule active;

  uint32_t lastTickMs   = 0;
  uint32_t lastNotifyMs = 0;
  boolean  forceTick    = false;
  boolean  internalTempWrite = false;   // guard so we don't self-disable
};

// ============================================================================
// Implementation
// ============================================================================
//
// This is a header-only implementation (marked "inline") so that the library
// remains drop-in with no compilation unit changes required.  Users that want
// a strictly separate .cpp file can add one and remove the "inline" markers.

inline AdaptiveLightingController::AdaptiveLightingController(
    SpanCharacteristic *brightness,
    SpanCharacteristic *colorTemperature) {

  bright = brightness;
  temp   = colorTemperature;

  // Instantiate the three Adaptive-Lighting Characteristics.  They attach to
  // whatever Service is currently being constructed (i.e. the same one that
  // holds Brightness and ColorTemperature).
  supp  = new Characteristic::SupportedCharacteristicValueTransitionConfiguration();
  ctl   = new Characteristic::CharacteristicValueTransitionControl();
  count = new Characteristic::CharacteristicValueActiveTransitionCount(0);

  // Advertise our capabilities so a GET returns the right thing immediately.
  publishSupportedConfig();
  clearControlValue();
}

// ----------------------------------------------------------------------------
// Sketch hooks
// ----------------------------------------------------------------------------

inline boolean AdaptiveLightingController::handleUpdate() {

  boolean did = false;

  if (ctl->updated()) {
    processControlWrite();
    did = true;
  }

  // A manual write to ColorTemperature (from the user, not from us) MUST
  // cancel any active Adaptive-Lighting schedule.
  if (temp->updated() && isActive() && !internalTempWrite) {
    LOG1("Adaptive Lighting: manual ColorTemperature write detected -- disabling schedule\n");
    disable();
    did = true;
  }

  // A brightness change while AL is active should trigger a re-evaluation on
  // the next tick (the mired value depends on brightness).
  if (bright->updated() && isActive())
    forceTick = true;

  return did;
}

inline void AdaptiveLightingController::poll() {

  if (!isActive()) return;

  uint32_t now = millis();
  if (!forceTick && (uint32_t)(now - lastTickMs) < active.updateIntervalMs) return;

  forceTick  = false;
  lastTickMs = now;
  computeAndApply();
}

// ----------------------------------------------------------------------------
// Query / control
// ----------------------------------------------------------------------------

inline void AdaptiveLightingController::disable() {

  boolean wasActive = isActive();
  active = ActiveSchedule{};
  count->setVal<uint8_t>(0, true);
  clearControlValue();

  if (wasActive && onState) onState(false);
}

inline void AdaptiveLightingController::printSchedule() const {

  if (!isActive()) {
    Serial.printf("Adaptive Lighting: no schedule loaded\n");
    return;
  }

  Serial.printf("Adaptive Lighting Schedule: %u entries, update=%ums, notify=%ums, bRange=%u..%u\n",
                (unsigned)active.curve.size(),
                (unsigned)active.updateIntervalMs,
                (unsigned)active.notifyIntervalMs,
                (unsigned)active.minBrightness,
                (unsigned)active.maxBrightness);

  Serial.printf("  idx    t+(h:mm:ss)   mired      K   bFactor    mired@b%-3u  mired@b%-3u   trans     hold\n",
                (unsigned)active.minBrightness,
                (unsigned)active.maxBrightness);

  uint64_t offsetMs = 0;
  for (size_t i = 0; i < active.curve.size(); i++) {
    const CurveEntry &e = active.curve[i];
    offsetMs += e.transitionTimeMs;

    uint64_t secs = offsetMs / 1000ULL;
    unsigned hh   = (unsigned)(secs / 3600ULL);
    unsigned mm   = (unsigned)((secs / 60ULL) % 60ULL);
    unsigned ss   = (unsigned)(secs % 60ULL);

    int miredAtMin = (int)lroundf(e.temperature + e.brightnessFactor * (float)active.minBrightness);
    int miredAtMax = (int)lroundf(e.temperature + e.brightnessFactor * (float)active.maxBrightness);
    int kelvin     = (int)lroundf(1.0e6f / e.temperature);

    Serial.printf("  %3u   %3u:%02u:%02u   %7.2f  %5d   %+7.4f       %4d         %4d    %6u   %6u\n",
                  (unsigned)i, hh, mm, ss,
                  e.temperature, kelvin, e.brightnessFactor,
                  miredAtMin, miredAtMax,
                  (unsigned)e.transitionTimeMs,
                  (unsigned)e.durationMs);

    offsetMs += e.durationMs;
  }
}

// ----------------------------------------------------------------------------
// Static helpers
// ----------------------------------------------------------------------------

inline uint64_t AdaptiveLightingController::readVarUInt(const uint8_t *p, size_t len) {
  uint64_t v = 0;
  if (len > 8) len = 8;
  for (size_t i = 0; i < len; i++) v |= ((uint64_t)p[i]) << (i * 8);
  return v;
}

inline float AdaptiveLightingController::readFloat32LE(const uint8_t *p, size_t len) {
  if (len < 4) return 0.0f;
  float f;
  memcpy(&f, p, 4);   // ESP32 is little-endian
  return f;
}

// ----------------------------------------------------------------------------
// Supported config (static advertisement)
// ----------------------------------------------------------------------------

inline void AdaptiveLightingController::publishSupportedConfig() {

  TLV8 outer;

  // Brightness support
  {
    TLV8 inner;
    inner.add(SUPP_CHAR_IID,        (uint64_t)bright->getIID());
    inner.add(SUPP_TRANSITION_TYPE, (uint64_t)TT_BRIGHTNESS);
    outer.add(SUPP_TRANSITION, inner);
  }
  // Separator (two same-tag records are otherwise concatenated by the TLV8 lib)
  outer.add(0x00);
  // Color temperature support
  {
    TLV8 inner;
    inner.add(SUPP_CHAR_IID,        (uint64_t)temp->getIID());
    inner.add(SUPP_TRANSITION_TYPE, (uint64_t)TT_COLOR_TEMPERATURE);
    outer.add(SUPP_TRANSITION, inner);
  }

  supp->setTLV(outer, false);
}

inline void AdaptiveLightingController::clearControlValue() {
  TLV8 empty;
  ctl->setTLV(empty, false);
}

// ----------------------------------------------------------------------------
// Inbound control-point write
// ----------------------------------------------------------------------------

inline void AdaptiveLightingController::processControlWrite() {

  TLV8 incoming;
  ctl->getNewTLV(incoming);

  TLV8 response;

  // READ_CURRENT?
  auto itRead = incoming.find(CTL_READ_CURRENT);
  if (itRead != incoming.end()) {
    TLV8 inner;
    inner.unpack(itRead);
    auto iidIt = inner.find(READ_CHAR_IID);
    uint64_t askedIID = (iidIt != inner.end()) ? (*iidIt).getVal<uint64_t>() : 0;
    if (isActive() && askedIID == active.colorTempIID)
      appendReadCurrentResponse(response);
  }

  // UPDATE?
  auto itUpd = incoming.find(CTL_UPDATE);
  if (itUpd != incoming.end()) {
    TLV8 inner;
    inner.unpack(itUpd);
    handleUpdateTransition(inner, response);
  }

  // Persist so HomeSpan returns it on the write-response (PR+PW+WR).
  ctl->setTLV(response, false);
}

inline void AdaptiveLightingController::handleUpdateTransition(TLV8 &updateTlv, TLV8 &response) {

  auto cfgIt = updateTlv.find(UPD_VALUE_CFG);
  if (cfgIt == updateTlv.end()) {
    LOG0("Adaptive Lighting: malformed UPDATE (missing tag 0x01)\n");
    return;
  }
  TLV8 cfg;
  cfg.unpack(cfgIt);

  // Target characteristic IID (must be our ColorTemperature)
  auto iidIt = cfg.find(VC_CHAR_IID);
  uint64_t targetIID = (iidIt != cfg.end()) ? (*iidIt).getVal<uint64_t>() : 0;

  // ENABLE flag: absent or zero = disable, non-zero = enable
  auto enIt = cfg.find(VC_ENABLE_FLAG);
  uint8_t enable = 0;
  if (enIt != cfg.end() && (*enIt).getLen() >= 1) enable = (*enIt)[0];
  if (enable == 0) {
    LOG1("Adaptive Lighting: DISABLE received\n");
    disable();
    response.add(CTL_UPDATE);
    return;
  }

  ActiveSchedule next{};
  next.colorTempIID = (uint32_t)targetIID;

  // Transition parameters
  auto pIt = cfg.find(VC_TRANSITION_PARAMS);
  if (pIt == cfg.end()) {
    LOG0("Adaptive Lighting: malformed UPDATE (missing TRANSITION_PARAMS)\n");
    return;
  }
  TLV8 params;
  params.unpack(pIt);

  auto tidIt = params.find(P_TRANSITION_ID);
  if (tidIt != params.end() && (*tidIt).getLen() == 16)
    memcpy(next.transitionId, (*tidIt).get(), 16);

  auto stIt = params.find(P_START_TIME);
  if (stIt == params.end() || (*stIt).getLen() != 8) {
    LOG0("Adaptive Lighting: malformed UPDATE (missing START_TIME)\n");
    return;
  }
  memcpy(next.startTimeBuf, (*stIt).get(), 8);

  auto id3It = params.find(P_START_ID3);
  if (id3It != params.end() && (*id3It).getLen() <= 8) {
    memcpy(next.id3, (*id3It).get(), (*id3It).getLen());
    next.hasId3 = true;
  }

  // Intervals
  auto uiIt = cfg.find(VC_UPDATE_INTERVAL);
  if (uiIt != cfg.end()) next.updateIntervalMs = (uint16_t)(*uiIt).getVal<uint32_t>();
  auto niIt = cfg.find(VC_NOTIFY_THRESHOLD);
  if (niIt != cfg.end()) next.notifyIntervalMs = (*niIt).getVal<uint32_t>();
  if (next.updateIntervalMs  < 1000)   next.updateIntervalMs  = 60000;
  if (next.notifyIntervalMs  < 10000)  next.notifyIntervalMs  = 600000;

  // Curve
  auto cvIt = cfg.find(VC_CURVE_CFG);
  if (cvIt == cfg.end()) {
    LOG0("Adaptive Lighting: malformed UPDATE (missing CURVE_CFG)\n");
    return;
  }
  TLV8 curveCfg;
  curveCfg.unpack(cvIt);

  auto adjIt = curveCfg.find(CC_ADJUSTMENT_CHAR_IID);
  next.brightnessIID = (adjIt != curveCfg.end())
                         ? (uint32_t)(*adjIt).getVal<uint64_t>() : 0;

  auto rgIt = curveCfg.find(CC_ADJUSTMENT_RANGE);
  if (rgIt != curveCfg.end()) {
    TLV8 r;
    r.unpack(rgIt);
    auto mi = r.find(R_MIN);
    auto ma = r.find(R_MAX);
    if (mi != r.end()) next.minBrightness = (*mi).getVal<uint32_t>();
    if (ma != r.end()) next.maxBrightness = (*ma).getVal<uint32_t>();
  }

  // Walk every CC_ENTRY sub-record
  for (auto it = curveCfg.begin(); it != curveCfg.end(); ++it) {
    if ((*it).getTag() != CC_ENTRY) continue;

    TLV8 entry;
    entry.unpack(it);

    CurveEntry ce{};
    auto fIt = entry.find(E_ADJUSTMENT_FACTOR);
    auto vIt = entry.find(E_VALUE);
    auto tIt = entry.find(E_TRANSITION_OFFSET);
    auto dIt = entry.find(E_DURATION);
    if (fIt != entry.end() && (*fIt).getLen() >= 4)
      ce.brightnessFactor = readFloat32LE((*fIt).get(), (*fIt).getLen());
    if (vIt != entry.end() && (*vIt).getLen() >= 4)
      ce.temperature      = readFloat32LE((*vIt).get(), (*vIt).getLen());
    if (tIt != entry.end())
      ce.transitionTimeMs = (uint32_t)readVarUInt((*tIt).get(), (*tIt).getLen());
    if (dIt != entry.end())
      ce.durationMs       = (uint32_t)readVarUInt((*dIt).get(), (*dIt).getLen());

    next.curve.push_back(ce);
  }

  if (next.curve.empty()) {
    LOG0("Adaptive Lighting: malformed UPDATE (empty curve)\n");
    return;
  }

  next.localStartMs = millis();
  active = std::move(next);

  LOG1("Adaptive Lighting: schedule accepted -- %u entries, update=%ums\n",
       (unsigned)active.curve.size(), (unsigned)active.updateIntervalMs);

  count->setVal<uint8_t>(1, true);

  // Apply the first point immediately
  forceTick  = true;
  lastTickMs = millis() - active.updateIntervalMs;

  // Build write-response payload
  TLV8 statusInner;
  buildValueConfigurationStatus(statusInner, /*timeSinceStartMs=*/0);
  TLV8 updateResp;
  updateResp.add(R_VALUE_CFG_STATUS, statusInner);
  response.add(CTL_UPDATE, updateResp);

  if (onState) onState(true);
}

inline void AdaptiveLightingController::buildTransitionParams(TLV8 &out) const {
  out.add(P_TRANSITION_ID, 16, active.transitionId);
  out.add(P_START_TIME,     8, active.startTimeBuf);
  if (active.hasId3)
    out.add(P_START_ID3, 8, active.id3);
}

inline void AdaptiveLightingController::buildValueConfigurationStatus(TLV8 &out, int64_t timeSinceStartMs) const {
  out.add(S_CHAR_IID, (uint64_t)active.colorTempIID);
  {
    TLV8 params;
    buildTransitionParams(params);
    out.add(S_TRANSITION_PARAMS, params);
  }
  out.add(S_TIME_SINCE_START, (uint64_t)(timeSinceStartMs > 0 ? timeSinceStartMs : 0));
}

inline void AdaptiveLightingController::appendReadCurrentResponse(TLV8 &response) {

  TLV8 cfg;
  cfg.add(VC_CHAR_IID, (uint64_t)active.colorTempIID);
  {
    TLV8 params;
    buildTransitionParams(params);
    cfg.add(VC_TRANSITION_PARAMS, params);
  }
  cfg.add(VC_ENABLE_FLAG, (uint64_t)1);

  TLV8 curveCfg;
  for (size_t i = 0; i < active.curve.size(); i++) {
    const CurveEntry &e = active.curve[i];
    TLV8 entry;
    uint8_t f[4]; memcpy(f, &e.brightnessFactor, 4);
    uint8_t v[4]; memcpy(v, &e.temperature,      4);
    entry.add(E_ADJUSTMENT_FACTOR, 4, f);
    entry.add(E_VALUE,             4, v);
    entry.add(E_TRANSITION_OFFSET, (uint64_t)e.transitionTimeMs);
    entry.add(E_DURATION,          (uint64_t)e.durationMs);

    if (i > 0) curveCfg.add(0x00);   // separator between same-tag records
    curveCfg.add(CC_ENTRY, entry);
  }
  curveCfg.add(CC_ADJUSTMENT_CHAR_IID, (uint64_t)active.brightnessIID);
  {
    TLV8 r;
    uint8_t mn[4]; uint32_t x = active.minBrightness; memcpy(mn, &x, 4);
    uint8_t mx[4];          x = active.maxBrightness; memcpy(mx, &x, 4);
    r.add(R_MIN, 4, mn);
    r.add(R_MAX, 4, mx);
    curveCfg.add(CC_ADJUSTMENT_RANGE, r);
  }
  cfg.add(VC_CURVE_CFG,        curveCfg);
  cfg.add(VC_UPDATE_INTERVAL,  (uint64_t)active.updateIntervalMs);
  cfg.add(VC_NOTIFY_THRESHOLD, (uint64_t)active.notifyIntervalMs);

  TLV8 readResp;
  readResp.add(UPD_VALUE_CFG, cfg);
  response.add(CTL_READ_CURRENT, readResp);
}

// ----------------------------------------------------------------------------
// Interpolation
// ----------------------------------------------------------------------------

inline boolean AdaptiveLightingController::interpolate(int64_t timeSinceStartMs,
                                                       float  &outTemperature,
                                                       float  &outFactor) const {

  if (active.curve.empty()) return false;

  uint64_t cursor = 0;
  for (size_t i = 0; i + 1 < active.curve.size(); i++) {
    const CurveEntry &lo = active.curve[i];
    const CurveEntry &hi = active.curve[i + 1];

    uint64_t segStart = cursor + lo.transitionTimeMs;
    uint64_t holdEnd  = segStart + lo.durationMs;
    uint64_t segEnd   = holdEnd  + hi.transitionTimeMs;

    if ((uint64_t)timeSinceStartMs < segStart || (uint64_t)timeSinceStartMs <= holdEnd) {
      outTemperature = lo.temperature;
      outFactor      = lo.brightnessFactor;
      return true;
    }
    if ((uint64_t)timeSinceStartMs <= segEnd) {
      float pct = (float)((uint64_t)timeSinceStartMs - holdEnd) / (float)hi.transitionTimeMs;
      outTemperature = lo.temperature      + (hi.temperature      - lo.temperature)      * pct;
      outFactor      = lo.brightnessFactor + (hi.brightnessFactor - lo.brightnessFactor) * pct;
      return true;
    }
    cursor = segEnd;
  }
  return false;   // past the end of the schedule
}

inline void AdaptiveLightingController::computeAndApply() {

  int64_t elapsed = (int64_t)millis() - (int64_t)active.localStartMs;
  if (elapsed < 0) elapsed = 0;

  float t = 0, f = 0;
  if (!interpolate(elapsed, t, f)) {
    LOG1("Adaptive Lighting: reached end of curve -- disabling\n");
    disable();
    return;
  }

  int b = bright->getVal<int>();
  if (b < (int)active.minBrightness) b = active.minBrightness;
  if (b > (int)active.maxBrightness) b = active.maxBrightness;

  int mired = (int)lroundf(t + f * (float)b);
  if (mired < 1)   mired = 1;                 // legal HAP minimum
  if (mired > 500) mired = 500;               // legal HAP maximum

  uint32_t now = millis();
  boolean notify = (uint32_t)(now - lastNotifyMs) >= active.notifyIntervalMs;
  if (notify) lastNotifyMs = now;

  LOG2("Adaptive Lighting: tick t+%llds  bri=%d  mired=%d (notify=%d)\n",
       (long long)(elapsed / 1000), b, mired, notify ? 1 : 0);

  internalTempWrite = true;
  temp->setVal<int>(mired, notify);
  internalTempWrite = false;

  if (onColorTemp) onColorTemp((uint32_t)mired);
}
