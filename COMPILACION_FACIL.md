# Compilación de Flavelah v2 con SpeedHack

**Este paquete NO incluye un ASI compilado.**

## Requisitos

- Windows con Visual Studio 2022 y el componente «Desarrollo para escritorio con C++» (MSBuild + toolset v143, x86).
- Plugin-SDK para GTA San Andreas, con las bibliotecas `plugin.lib` compiladas.
- DirectX SDK (June 2010), especialmente `d3d9.lib`, `d3dx9.lib` y cabeceras.
- El proyecto `Stealth Remastered.sln` incluido en esta carpeta.

## Uso en CMD

Abre CMD y configura las rutas, adaptándolas a las carpetas de tu PC:

```bat
set "PLUGIN_SDK_DIR=C:\Dev\plugin-sdk"
set "DIRECTX9_SDK_DIR=C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
COMPILAR_WINDOWS.cmd
```

Si aparece un error, revisa `flavelah_build.log`. La configuración correcta es `GTASA Release|Win32`, **no** `Release|x64`.

Salida prevista: `build\GTASA Release\Flavelah.asi`.

## Comprobaciones realizadas aquí

- `tests/vehicle_speed_test.cpp`: compilación Linux C++17 y pruebas superadas.
- `tests/local_macro_test.cpp`: compilación Linux C++17 y pruebas superadas.
- Compilación completa de Windows del ASI: **no realizada**, faltan MSBuild, plugin-sdk y DirectX SDK.

No está probado en GTA SA ni en SA-MP.
