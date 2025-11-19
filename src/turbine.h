#pragma once
#include <Arduino.h>

// Configuración de la turbina (GPIO12)
static const uint8_t TURBINE_PIN = 12;     // GPIO12
static const uint8_t TURBINE_CHANNEL = 0;  // Canal LEDC
static const uint32_t TURBINE_FREQ = 20000; // 20 kHz para evitar ruido audible
static const uint8_t TURBINE_RES = 8;      // 8 bits (0..255)

void turbine_init();
void turbine_set(uint8_t duty_target, uint8_t step = 3, uint16_t step_delay_ms = 10);
void turbine_on(uint8_t duty_target = 200, uint8_t step = 3, uint16_t step_delay_ms = 10);
void turbine_off(uint8_t step = 3, uint16_t step_delay_ms = 10);
uint8_t turbine_get_current();

