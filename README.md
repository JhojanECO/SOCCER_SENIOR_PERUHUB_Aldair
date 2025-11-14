# SOCCER_SENIOR_PERUHUB_Aldair

Firmware para el robot Soccer Senior de Aldair basado en ESP32 (PlatformIO + Arduino) y controlado con un mando PS5 por Bluetooth clasico.

## Requisitos
- VS Code + extension PlatformIO (o PlatformIO CLI solamente).
- Placa ESP32 Dev Module.
- Mando PS5 emparejado con el ESP32 (Bluetooth clasico, no BLE).
- Cable USB para programar y monitorizar.

## Estructura
- `BOMBA/platformio.ini`: entornos PlatformIO, flags y dependencia `ps5-esp32`.
- `BOMBA/src/main.cpp`: logica principal (PS5, mezcla diferencial, watchdog y turbina).
- `BOMBA/src/motors.*`: control MCPWM usando pines AIN1=19, AIN2=16, BIN1=5, BIN2=18.
- `BOMBA/src/turbine.*`: rampa PWM para la turbina conectada al GPIO12.

## Preparacion del mando PS5
1. Abre `BOMBA/src/main.cpp` y cambia el valor de `PS5_MAC_ADDRESS` por la direccion Bluetooth del control.
2. Compila/sube y observa por monitor serie los mensajes "Buscando control PS5" y "PS5 listo" para confirmar la conexion.

## Compilar
```bash
pio run -d BOMBA
```

## Subir firmware
```bash
pio run -d BOMBA -t upload
```
- Velocidad configurada: 1.5 Mbps (estable en la mayoría de cables/adaptadores).
- Agrega `--upload-port COMx` si tienes varios puertos.

## Monitor serie
```bash
pio device monitor -d BOMBA -b 115200
```

## Controles
- Gatillos: R2 acelera, L2 retrocede (mezclados con la direccion para giro).
- Stick izquierdo X: giro con zona muerta y curva cubica suave.
- `Triangle`: enciende la turbina con rampa (`turbine_on`).
- `Cross`: apaga la turbina con rampa (`turbine_off`).
- `R1`: freno defensivo, detiene ambos motores.
- Watchdog: si no llegan datos del mando por 5 s, se detienen los motores y se apaga la turbina.

## Notas
- La libreria `ps5-esp32` se descarga automaticamente via `lib_deps` en `platformio.ini`.
- Si cambias pines o hardware, ajusta `motors.h` y `turbine.*` en consecuencia.
