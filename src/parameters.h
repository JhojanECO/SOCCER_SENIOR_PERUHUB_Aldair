#pragma once

#include <stdint.h>

namespace params {

inline constexpr const char* ps5_mac_address = "24:a6:fa:8f:7b:66";

inline constexpr float motor_cmd_max = 70.0f;
inline constexpr float steer_gain = 0.7f;// si es mayor tiende a girar mas rapido
inline constexpr int stick_deadzone = 12;
inline constexpr int trigger_deadzone = 3;
inline constexpr float slew_step = 4.0f;
inline constexpr uint32_t timeout_ms = 4500;

}  // namespace params
