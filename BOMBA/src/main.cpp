#include <Arduino.h>
#include <BluetoothSerial.h>
#include "driver/mcpwm.h"
#include "esp_task_wdt.h"

#include "motors.h"
#include "turbine.h"

BluetoothSerial SerialBT;

#define WDT_TIMEOUT_SEC 5

static void init_watchdog() {
#if defined(ESP_IDF_VERSION_MAJOR) && (ESP_IDF_VERSION_MAJOR >= 5)
  esp_task_wdt_config_t wdt_config = {
      .timeout_ms = WDT_TIMEOUT_SEC * 1000,
      .idle_core_mask = (1 << portNUM_PROCESSORS) - 1,
      .trigger_panic = true};
  esp_task_wdt_init(&wdt_config);
#else
  esp_task_wdt_init(WDT_TIMEOUT_SEC, true);
#endif
  esp_task_wdt_add(NULL);
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("PATRICIO"); // Nombre del dispositivo Bluetooth
  Serial.println("🔵 Bluetooth iniciado. Esperando conexión...");

  init_watchdog();

  motors_init();
  turbine_init();

  delay(3000);
  Serial.println("⏳ Esperando 3 segundos antes de permitir movimiento...");
}

void loop() {
  esp_task_wdt_reset();

  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    switch (cmd) {
      // Movimiento (mantener comportamiento actual)
      case 'F': motores(DUTY_FRONT, DUTY_FRONT); break;
      case 'B': motores(-DUTY_FRONT, -DUTY_FRONT); break;
      case 'L': motores(DUTY_TURN, -DUTY_TURN); break;
      case 'R': motores(-DUTY_TURN, DUTY_TURN); break;
      case 'S': detener(); break;

      // Turbina en GPIO12: 'W' enciende con rampa, 'w' apaga con rampa
      case 'W':
        Serial.println("💨 Turbina ON con rampa a 200/255");
        turbine_on(150, 3, 10); // objetivo 200, paso 3, 10 ms por paso
        break;
      case 'w':
        Serial.println("🛑 Turbina OFF con rampa");
        turbine_off(3, 10);
        break;

      default:
        Serial.println("⚠️ Comando desconocido");
        break;
    }
  }

  delay(20);
}
