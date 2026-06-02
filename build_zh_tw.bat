@echo off
setlocal EnableExtensions EnableDelayedExpansion

chcp 65001 >nul

set "ROOT=%~dp0"
pushd "%ROOT%" >nul

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=Release"

set "PLATFORM=%~2"
if "%PLATFORM%"=="" set "PLATFORM=x64"

set "TOOLSET=%~3"
if "%TOOLSET%"=="" set "TOOLSET=v143"

if /I not "%CONFIG%"=="Release" if /I not "%CONFIG%"=="Debug" (
    echo [ERROR] CONFIG must be Release or Debug.
    exit /b 1
)

if /I not "%PLATFORM%"=="x64" if /I not "%PLATFORM%"=="Win32" (
    echo [ERROR] PLATFORM must be x64 or Win32.
    exit /b 1
)

call :find_vs
if errorlevel 1 exit /b 1

if /I "%PLATFORM%"=="x64" (
    set "VS_ARCH=x64"
    set "LJ_DIR=x64-%CONFIG%"
) else (
    set "VS_ARCH=x86"
    set "LJ_DIR=x86-%CONFIG%"
)

echo [INFO] Using Visual Studio environment:
echo        %VSDEVCMD%
call "%VSDEVCMD%" -no_logo -arch=%VS_ARCH% -host_arch=x64
if errorlevel 1 (
    echo [ERROR] Failed to initialize the Visual Studio build environment.
    exit /b 1
)

call :extract_dependencies
if errorlevel 1 exit /b 1

call :build_solution "nana\build\vc2022\nana.sln"
if errorlevel 1 exit /b 1

call :build_solution "bit7z\bit7z.sln"
if errorlevel 1 exit /b 1

call :build_solution "libpng\libpng.sln"
if errorlevel 1 exit /b 1

if not exist "%ROOT%libjpeg-turbo-3.1.2\CMakeLists.txt" (
    echo [ERROR] Missing libjpeg-turbo source folder.
    exit /b 1
)

echo [INFO] Configuring libjpeg-turbo ^(%LJ_DIR%^)
cmake -S "%ROOT%libjpeg-turbo-3.1.2" -B "%ROOT%libjpeg-turbo-3.1.2\out\build\%LJ_DIR%" -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=%CONFIG%
if errorlevel 1 (
    echo [ERROR] CMake configure failed for libjpeg-turbo.
    exit /b 1
)

echo [INFO] Building libjpeg-turbo ^(%LJ_DIR%^)
cmake --build "%ROOT%libjpeg-turbo-3.1.2\out\build\%LJ_DIR%"
if errorlevel 1 (
    echo [ERROR] libjpeg-turbo build failed.
    exit /b 1
)

call :build_solution "ytdlp-interface\ytdlp-interface.sln"
if errorlevel 1 exit /b 1

echo [OK] Build completed.
echo [OK] Expected output:
echo      %ROOT%ytdlp-interface\%PLATFORM%\%CONFIG%\ytdlp-interface.exe
popd >nul
exit /b 0

:find_vs
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%VSWHERE%" (
    for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find Common7\Tools\VsDevCmd.bat`) do (
        set "VSDEVCMD=%%I"
    )
)

if not defined VSDEVCMD if exist "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
if not defined VSDEVCMD if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
if not defined VSDEVCMD if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
if not defined VSDEVCMD if exist "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"

if not defined VSDEVCMD (
    echo [ERROR] Could not find VsDevCmd.bat. Install Visual Studio 2022 or newer with C++ build tools.
    exit /b 1
)
exit /b 0

:extract_dependencies
if exist "%ROOT%nana\build\vc2022\nana.sln" if exist "%ROOT%bit7z\bit7z.sln" if exist "%ROOT%libpng\libpng.sln" if exist "%ROOT%libjpeg-turbo-3.1.2\CMakeLists.txt" (
    echo [INFO] Dependencies already extracted.
    exit /b 0
)

if not exist "%ROOT%ytdlp-interface dependencies.7z" (
    echo [ERROR] Missing archive: "%ROOT%ytdlp-interface dependencies.7z"
    exit /b 1
)

echo [INFO] Extracting dependency archive...
tar -xf "%ROOT%ytdlp-interface dependencies.7z"
if errorlevel 1 (
    echo [ERROR] Failed to extract the dependency archive.
    exit /b 1
)
exit /b 0

:build_solution
set "SLN=%~1"
if not exist "%ROOT%%SLN%" (
    echo [ERROR] Missing solution: %ROOT%%SLN%
    exit /b 1
)

echo [INFO] Building %SLN% ^(%CONFIG% ^| %PLATFORM% ^| %TOOLSET%^)
msbuild "%ROOT%%SLN%" /m /nologo /t:Build /p:Configuration=%CONFIG%;Platform=%PLATFORM%;PlatformToolset=%TOOLSET%
if errorlevel 1 (
    echo [ERROR] Build failed: %SLN%
    exit /b 1
)
exit /b 0
