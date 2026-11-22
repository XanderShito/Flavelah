@echo off
setlocal EnableExtensions
cd /d "%~dp0"

echo ============================================
echo   Flavelah - Compilacion Windows 32 bits
echo ============================================
echo.

if not exist "Stealth Remastered.sln" (
  echo ERROR: No se encuentra Stealth Remastered.sln
  exit /b 2
)

if not defined PLUGIN_SDK_DIR (
  echo ERROR: define PLUGIN_SDK_DIR con la carpeta plugin-sdk.
  echo Ejemplo: set "PLUGIN_SDK_DIR=C:\Dev\plugin-sdk"
  exit /b 3
)
if not exist "%PLUGIN_SDK_DIR%\plugin_sa" (
  echo ERROR: PLUGIN_SDK_DIR no parece apuntar a plugin-sdk.
  exit /b 3
)
if not defined DIRECTX9_SDK_DIR (
  echo ERROR: define DIRECTX9_SDK_DIR con la carpeta DirectX SDK.
  echo Ejemplo: set "DIRECTX9_SDK_DIR=C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)"
  exit /b 4
)
if not exist "%DIRECTX9_SDK_DIR%\Include\d3d9.h" (
  echo ERROR: no encontre Include\d3d9.h en DIRECTX9_SDK_DIR.
  exit /b 4
)
if not exist "%DIRECTX9_SDK_DIR%\Lib\x86\d3d9.lib" (
  echo ERROR: no encontre Lib\x86\d3d9.lib en DIRECTX9_SDK_DIR.
  exit /b 4
)

set "MSBUILD_PATH="
for /f "delims=" %%I in ('where MSBuild.exe 2^>nul') do if not defined MSBUILD_PATH set "MSBUILD_PATH=%%I"
if not defined MSBUILD_PATH (
  if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" (
    for /f "usebackq delims=" %%I in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe"`) do if not defined MSBUILD_PATH set "MSBUILD_PATH=%%I"
  )
)
if not defined MSBUILD_PATH (
  echo ERROR: no encontre MSBuild.exe. Instala Visual Studio 2022 con Desktop development with C++.
  exit /b 5
)

echo Compilando con: %MSBUILD_PATH%
"%MSBUILD_PATH%" "Stealth Remastered.sln" /m /t:Build /p:Configuration="GTASA Release" /p:Platform=Win32 /v:minimal /fl /flp:logfile=flavelah_build.log;verbosity=normal
if errorlevel 1 (
  echo.
  echo ERROR: Fallo la compilacion. Revisa flavelah_build.log.
  exit /b 6
)

set "RESULT=build\GTASA Release\Flavelah.asi"
if not exist "%RESULT%" (
  echo La compilacion termino, pero no se encontro: %RESULT%
  echo Revisa flavelah_build.log y la carpeta build.
  exit /b 7
)

echo.
echo ASI generado: %CD%\%RESULT%
echo Para usarlo, asegurate de probarlo en un entorno autorizado.
exit /b 0
