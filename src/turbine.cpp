#include "turbine.h"

static uint8_t s_current = 0;

void turbine_init() {
  ledcSetup(TURBINE_CHANNEL, TURBINE_FREQ, TURBINE_RES);
  ledcAttachPin(TURBINE_PIN, TURBINE_CHANNEL);
  ledcWrite(TURBINE_CHANNEL, 0);
  s_current = 0;
}

uint8_t turbine_get_current() { return s_current; }

void turbine_set(uint8_t duty_target, uint8_t step, uint16_t step_delay_ms) {
  uint8_t start = s_current;
  if (duty_target == start) {
    return;
  }

  if (duty_target > start) {
    for (uint16_t v = start; v <= duty_target; v = (v + step > 255 ? 255 : v + step)) {
      ledcWrite(TURBINE_CHANNEL, v);
      delay(step_delay_ms);
      if (v == 255) break;
    }
  } else {
    for (int16_t v = start; v >= duty_target; v = (v - step < 0 ? 0 : v - step)) {
      ledcWrite(TURBINE_CHANNEL, static_cast<uint8_t>(v));
      delay(step_delay_ms);
      if (v == 0) break;
    }
  }
  s_current = duty_target;
}

void turbine_on(uint8_t duty_target, uint8_t step, uint16_t step_delay_ms) {
  turbine_set(duty_target, step, step_delay_ms);
}

void turbine_off(uint8_t step, uint16_t step_delay_ms) {
  turbine_set(0, step, step_delay_ms);
}

