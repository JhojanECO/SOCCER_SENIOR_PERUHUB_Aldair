#pragma once

#include "driver/mcpwm.h"

// Pines del motor izquierdo (canal A)
#define AIN1 19
#define AIN2 16

// Pines del motor derecho (canal B)
#define BIN1 5
#define BIN2 18

// Inversión de direcciones por cableado
// Ajusta a true si ese motor gira al revés de lo esperado
#define RIGHT_INVERTED false
#define LEFT_INVERTED  true

#define DUTY_TURN 40.0f
#define DUTY_FRONT 50.0f

void motors_init();
void motores(float izquierdo, float derecho);
void detener();
