# Soccer Senior BTS BOMBA (ESP32 Dev Module)

Proyecto para ESP32 (Arduino) con control de motores por MCPWM y turbina PWM en GPIO12, operado por Bluetooth clasico (SPP) con el nombre "GATITO".

## Requisitos
- PlatformIO (extension de VS Code o CLI)
- Placa: ESP32 Dev Module
- Cable USB para programacion

## Estructura
- `BOMBA/platformio.ini`: configuracion de PlatformIO (entorno `esp32dev`).
- `BOMBA/src/main.cpp`: entrada principal; Bluetooth, WDT, despacho de comandos.
- `BOMBA/src/motors.*`: control de motores por MCPWM (pines AIN1=19, AIN2=16, BIN1=5, BIN2=18).
- `BOMBA/src/turbine.*`: control de turbina por LEDC en GPIO12 con rampa.

## Compilar y subir (CLI)
Desde la raiz del repo:

- Compilar: `pio run -d BOMBA`
- Subir firmware: `pio run -d BOMBA -t upload`
- Monitor serie: `pio device monitor -b 115200 -d BOMBA`

Sugerencia: si hay multiples puertos, especifica el puerto con `-e esp32dev -t upload --upload-port COMx` o configuralo en `platformio.ini`.

## Compilar y subir (VS Code + PlatformIO)
1. Abre el directorio del proyecto en VS Code.
2. En la barra de PlatformIO, selecciona el entorno `esp32dev`.
3. Usa los botones: Build (martillo), Upload (flecha) y Monitor Serial.

## Uso
- Empareja por Bluetooth con el nombre `GATITO` (Bluetooth clasico, no BLE).
- Envia comandos por el puerto serie Bluetooth:
  - Movimiento:
    - `F`: adelante (50% de duty por defecto)
    - `B`: atras (50% de duty por defecto)
    - `L`: giro a la izquierda (motor izquierdo retrocede, derecho avanza)
    - `R`: giro a la derecha (motor izquierdo avanza, derecho retrocede)
    - `S`: detener
  - Turbina (GPIO12, PWM 8 bits):
    - `W`: encender con rampa suave hasta 200/255
    - `w`: apagar con rampa suave a 0

## Ajustes de la turbina
- Objetivo por defecto: 200/255 (editable en `turbine_on(200, 3, 10)`).
- Parametros de rampa:
  - `step`: incremento por paso (3 por defecto)
  - `step_delay_ms`: retardo entre pasos (10 ms por defecto)
- Frecuencia PWM: 20 kHz (LEDC) para evitar ruido audible; resolucion de 8 bits.

## Pines y notas
- Motores (MCPWM):
  - AIN1 = GPIO19, AIN2 = GPIO16
  - BIN1 = GPIO5, BIN2 = GPIO18
- Turbina: GPIO12 (LEDC canal 0)
- Advertencia GPIO12: es un pin de arranque (strapping); evita forzar niveles durante el boot.

## Cargas mas rapidas
- Edita `BOMBA/platformio.ini` (seccion `[env:esp32dev]`) y agrega:
  ```
  upload_speed = 921600     ; prueba hasta 2000000 si el cable/puerto lo permite
  monitor_speed = 921600    ; igual que upload_speed para no reconfigurar
  ```
- Usa `pio run -d BOMBA` para compilar y solo `pio run -d BOMBA -t upload` cuando vayas a flashear; la compilacion incremental deja el bin listo.
- Prefiere cables USB cortos/de buena calidad y evita hubs lentos; si fallan las cargas, baja `upload_speed` hasta encontrar un valor estable.
- Activa `board_build.partitions = huge_app.csv` solo si necesitas particiones grandes, porque aumenta el tiempo de escritura.

## Solucion de problemas
- "No se detecta puerto": revisa el driver (CP210x/CH34x) y selecciona el COM correcto.
- "Failed to connect to ESP32": mantten presionado `BOOT` al iniciar la subida o reduce/ajusta `upload_speed`.
- Compilacion lenta o binarios grandes: usa la compilacion por defecto (release) y cierra otros proyectos de PlatformIO.

## Licencia
Este proyecto no especifica licencia. Consulta con el autor antes de reutilizar.

