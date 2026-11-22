# Compilar Flavelah en la nube (sin Visual Studio en tu equipo)

Este paquete contiene el código fuente y un flujo de **GitHub Actions**
para INTENTAR generar `Flavelah.asi` en una máquina Windows de GitHub.

> IMPORTANTE: Este flujo no se ha ejecutado en GitHub. Puede fallar por cambios
> en Plugin-SDK o incompatibilidades del proyecto original.
> No incluye un ASI ya compilado.

## Desde el navegador

1. Crea una cuenta de GitHub si no tienes una.
2. Crea un repositorio nuevo (puede ser privado).
3. Descomprime este ZIP en tu PC. No necesitas instalar software de programación.
4. En el repositorio, usa **Add file → Upload files** y sube **el contenido
   extraído de la carpeta raíz**, conservando las subcarpetas, incluida `.github`.
   No subas el ZIP sin descomprimir.
5. Comprueba que `Stealth Remastered.sln` y
   `.github/workflows/build-flavelah.yml` quedaron en la raíz del repositorio.
6. Abre **Actions → Compilar Flavelah (Win32) → Run workflow**, si no se ejecutó
   automáticamente después de subirlo.
7. Si aparece verde, descarga el artefacto **Flavelah-asi-Win32**
   desde la ejecución y extrae `Flavelah.asi`.
8. Si falla, abre la ejecución, copia el mensaje de error y compártelo
   para corregir la configuración o el código.

## Qué hace el flujo

- Usa una máquina Windows con herramientas de compilación C++.
- Descarga Plugin-SDK del repositorio de DK22Pac.
- Descarga D3DX9 de **Microsoft.DXSDK.D3DX** (NuGet, paquete de Microsoft).
- Intenta compilar `GTASA Release | Win32`.
- Publica un ASI solamente si la compilación termina sin errores.

No cambia las funciones del proyecto, no añade GodMode ni mecanismos de
evasión y no prueba el archivo dentro de GTA. Si la plataforma te solicita
activar GitHub Actions o configurar opciones de facturación, revisa sus
condiciones antes de continuar.
