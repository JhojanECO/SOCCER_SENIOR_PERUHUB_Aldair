#include "motors.h"
#include <Arduino.h>
#include <math.h>

namespace {
float clamp_duty(float duty) {
  if (duty > DUTY_TURN) return DUTY_TURN;
  if (duty < -DUTY_TURN) return -DUTY_TURN;
  return duty;
}

void stop_motor(mcpwm_unit_t unit, mcpwm_timer_t timer) {
  mcpwm_set_duty(unit, timer, MCPWM_OPR_A, 0.0f);
  mcpwm_set_duty(unit, timer, MCPWM_OPR_B, 0.0f);
}

void apply_motor(mcpwm_unit_t unit,
                 mcpwm_timer_t timer,
                 float command,
                 bool inverted) {
  float clamped = clamp_duty(command);
  float magnitude = fabsf(clamped);
  if (magnitude < 0.01f) {
    stop_motor(unit, timer);
    return;
  }

  bool forward = clamped >= 0.0f;
  if (inverted) {
    forward = !forward;
  }

  if (forward) {
    // Adelante usa el canal B (equivalente al antiguo mover_adelante)
    mcpwm_set_duty(unit, timer, MCPWM_OPR_A, 0.0f);
    mcpwm_set_duty(unit, timer, MCPWM_OPR_B, magnitude);
    mcpwm_set_duty_type(unit, timer, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);
  } else {
    // Atras usa el canal A (equivalente al antiguo mover_atras)
    mcpwm_set_duty(unit, timer, MCPWM_OPR_A, magnitude);
    mcpwm_set_duty(unit, timer, MCPWM_OPR_B, 0.0f);
    mcpwm_set_duty_type(unit, timer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
  }
}
}  // namespace

void motors_init() {
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, AIN1);
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, AIN2);
  mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, BIN1);
  mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, BIN2);

  mcpwm_config_t pwm_config = {
      .frequency = 20000,
      .cmpr_a = 0.0,
      .cmpr_b = 0.0,
      .duty_mode = MCPWM_DUTY_MODE_0,
      .counter_mode = MCPWM_UP_COUNTER};

  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);
}

void motores(float izquierdo, float derecho) {
  Serial.printf("Motores L: %.1f R: %.1f\n", izquierdo, derecho);
  apply_motor(MCPWM_UNIT_0, MCPWM_TIMER_0, izquierdo, LEFT_INVERTED);
  apply_motor(MCPWM_UNIT_1, MCPWM_TIMER_1, derecho, RIGHT_INVERTED);
}

void detener() {
  Serial.println("Motores detenidos");
  motores(0.0f, 0.0f);
}
