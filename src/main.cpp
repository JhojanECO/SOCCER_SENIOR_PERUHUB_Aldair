#include <Arduino.h>
#include <math.h>
#include <ps5Controller.h>

#include "motors.h"
#include "parameters.h"
#include "turbine.h"

namespace {
float cmdLeft = 0.0f;
float cmdRight = 0.0f;
bool turbineActive = false;
unsigned long lastCommandMs = 0;
}  // namespace

static inline float clampf(float value, float lo, float hi) {
  return (value < lo) ? lo : (value > hi) ? hi : value;
}

static inline float applySlew(float target, float current, float step) {
  if (target > current) return (target - current > step) ? current + step : target;
  if (target < current) return (current - target > step) ? current - step : target;
  return current;
}

static inline float cubic(float x) { return x * x * x; }

static void stop_motors() {
  cmdLeft = 0.0f;
  cmdRight = 0.0f;
  detener();
}

static void handleTurbineButtons() {
  if (ps5.Triangle()) {
    if (!turbineActive) {
      turbine_on(150, 3, 10);
      turbineActive = true;
    }
  } else if (ps5.Cross()) {
    if (turbineActive) {
      turbine_off(3, 10);
      turbineActive = false;
    }
  }
}

static void notify() {
  lastCommandMs = millis();
  handleTurbineButtons();

  int r2 = ps5.R2Value();
  int l2 = ps5.L2Value();
  if (r2 < params::trigger_deadzone) r2 = 0;
  if (l2 < params::trigger_deadzone) l2 = 0;

  float throttle = static_cast<float>(r2 - l2) * (params::motor_cmd_max / 255.0f);

  int rawX = ps5.LStickX();
  if (abs(rawX) < params::stick_deadzone) rawX = 0;

  float xNorm = static_cast<float>(rawX) / 127.0f;
  float steer = cubic(xNorm) * (params::steer_gain * params::motor_cmd_max);
  if (throttle < 0.0f) {
    steer = -steer;
  }

  float targetLeft = clampf(throttle - steer, -params::motor_cmd_max, params::motor_cmd_max);
  float targetRight = clampf(throttle + steer, -params::motor_cmd_max, params::motor_cmd_max);

  cmdLeft = applySlew(targetLeft, cmdLeft, params::slew_step);
  cmdRight = applySlew(targetRight, cmdRight, params::slew_step);

  motores(cmdLeft, cmdRight);

  if (ps5.R1()) {
    stop_motors();
  }
}

static void onConnect() {
  lastCommandMs = millis();
}

static void onDisconnect() {
  stop_motors();
}

void setup() {
  motors_init();
  turbine_init();

  ps5.attach(notify);
  ps5.attachOnConnect(onConnect);
  ps5.attachOnDisconnect(onDisconnect);

  ps5.begin(params::ps5_mac_address);
  
  while (!ps5.isConnected()) {
    delay(100);
  }
  lastCommandMs = millis();
}

void loop() {
  if (!ps5.isConnected()) {
    stop_motors();
  } else if (millis() - lastCommandMs > params::timeout_ms) {
    stop_motors();
    lastCommandMs = millis();
  }
}
