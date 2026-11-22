# Flavelah — reconstrucción desde el código fuente

Base: Stealth-Remastered-origin.zip, proporcionado por el usuario.
Los créditos originales permanecen en README.md.

## Cambios
- Logotipo FLAVELAH verde esmeralda (#2ECC71) reutilizado de la copia anterior.
- Tema Default con fondo #080808 y controles verdes.
- Health y Armour como indicadores de solo lectura encima de Anti Stun.
  El proyecto original no traía los botones de recarga; no se han implementado.
- Weapon > Combat Assistance, página 3/3: cinco macros de entrenamiento,
  desactivadas al iniciar. El Aimbot y el resto de módulos originales no se han cambiado.
- No se incorpora Ghost GM, Heavy Fist ni el CLEO A-SHIELD.
- La configuración se guarda en la carpeta Flavelah de Documentos.
  Las casillas de macros son temporales: no se reactivan al cargar una configuración.
- El ejecutable de salida se genera en build, no se instala automáticamente en GTA.

## Compilación (Windows)
1. Instala Visual Studio 2022 con Desarrollo para el escritorio con C++ (v143),
   Windows SDK, DirectX SDK de junio de 2010 y Plugin-SDK para GTA SA.
2. Compila Plugin-SDK para Win32 Release (plugin.lib). Configura las variables
   PLUGIN_SDK_DIR y DIRECTX9_SDK_DIR de acuerdo con las rutas de tu instalación.
3. Abre Stealth Remastered.sln.
4. Selecciona GTASA Release | x86 (Win32 en el proyecto) y Compilar solución.
   El objetivo se llama Flavelah.asi.
   Alternativa desde Developer Command Prompt:
   msbuild "Stealth Remastered.sln" /p:Configuration="GTASA Release" /p:Platform=x86
5. Busca el archivo en build/GTASA Release/Flavelah.asi.

## Prueba de las macros
Solamente para un servidor local de entrenamiento (127.0.0.1), no servidores públicos.
No se añaden técnicas de evasión ni se modifica la protección existente.
La página 3/3 del apartado Weapon incluye el diagnóstico de estado.
Cierra el menú y el chat, y desactiva Auto C-Bug y Auto Scroll antes de entrenar.
El número "sent" registra las secuencias enviadas, no confirma que GTA las haya procesado.
El envío usa la interfaz estándar de Windows; su recepción real por GTA sigue sin verificar.

## Verificación
La máquina de estados se probó con g++ C++17 en Linux:
    g++ -std=c++17 -Wall -Wextra -Werror -I "Stealth Remastered" tests/local_macro_test.cpp -o local_macro_test
    ./local_macro_test
Resultado: PASS (cinco secuencias, estado inicial, puerta local, temporización y cancelación).
No se compiló la solución Windows ni se ejecutó GTA. El ZIP no incluye un ASI nuevo.
