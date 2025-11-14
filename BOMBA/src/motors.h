#pragma once

#include "driver/mcpwm.h"
#include "parameters.h"

// Pines del motor izquierdo (canal A)
#define AIN1 16
#define AIN2 19

// Pines del motor derecho (canal B)
#define BIN1 5
#define BIN2 18

// Inversión de direcciones por cableado
// Ajusta a true si ese motor gira al revés de lo esperado
#define RIGHT_INVERTED false
#define LEFT_INVERTED  true

void motors_init();
void motores(float izquierdo, float derecho);
void detener();
