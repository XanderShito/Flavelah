# Flavelah v2 - SpeedHack de vehiculos (codigo fuente)

## Implementacion

El modulo fue integrado sobre el proyecto `Flavelah_Codigo_Fuente_v2`.
Aparece en **Player > Vehicle Assistance**, en la columna derecha,
debajo de **Sync Modifications**.

- Speed Hack: casilla independiente, **apagada por defecto**.
- Speed Multiplier: deslizador de **1.0x a 3.0x**, predeterminado **1.2x**.
- Activation Keys: `ALT + W / ALT + S` o `CTRL + W / CTRL + S`.
- El coche debe estar **conducido por el personaje local**.
- Intervalo entre ajustes: **85 milisegundos**.
- Igual que el CLEO de referencia `SpeedHack(1).cs`, toma la velocidad del
  vehiculo (opcode `02E3`), escala su magnitud y añade `2.0` unidades, y
  aplica velocidad de avance o retroceso (opcode `04BA`).
- Se ha agregado un limite de seguridad de **75 unidades** a la velocidad
  objetivo para reducir el riesgo de descontrol fisico.
- No modifica la salud, el armamento, el GM, las macros existentes ni el aimbot.
- Deshabilitado durante chat/menu y cuando Air Break o Map Run estan activos.
- Las opciones del SpeedHack se guardan en los archivos de configuracion del menu.
- Sin bypass de anticheat ni manipulacion de verificaciones del servidor.

## Archivos modificados

- `Stealth Remastered/Features/Player.cpp`
- `Stealth Remastered/Features/Player.h`
- `Stealth Remastered/Features/VehicleSpeedMath.h` (nuevo)
- `Stealth Remastered/Menu/Menu.cpp`
- `Stealth Remastered/Cheat/Config/Config.h`
- `Stealth Remastered/Cheat/Config/Config.cpp`
- `Stealth Remastered/Stealth Remastered.vcxproj`
- `Stealth Remastered/Stealth Remastered.vcxproj.filters`
- `tests/vehicle_speed_test.cpp` (nuevo)

## Compilacion de ASI en Windows

1. Descomprimir ZIP.
2. Instalar Visual Studio 2022 con compilacion C++ para escritorio.
3. Instalar/configurar `plugin-sdk` y DirectX SDK como se indica en el README original.
4. Abrir `Stealth Remastered.sln` y seleccionar `Release | Win32`.
5. Compilar solucion con `Ctrl + Shift + B`.
6. Verificar el ASI resultante antes de instalarlo. **Este ZIP contiene fuente, no un ASI compilado.**

Importante: no fue posible compilar ni ejecutar el ASI entero en Windows en
este entorno, por falta de Visual Studio, DirectX y el Plugin-SDK del proyecto.
Se verifico la matematica del modulo con un test C++17 ejecutado localmente.
La modificacion de velocidad del cliente podria ser corregida o rechazada
por un servidor multijugador.

## Pruebas de calculo (Linux/macOS/Windows con g++)

```sh
g++ -std=c++17 -Wall -Wextra -Werror tests/vehicle_speed_test.cpp -o vehicle_speed_test
./vehicle_speed_test
```

Los valores por defecto reproducen la idea del CLEO enviado, con limites
de seguridad y controles nuevos.
