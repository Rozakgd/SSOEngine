@echo off
setlocal enabledelayedexpansion

:: ===================================================
:: SSOEngine - Build System
:: (c) 2026 SSOGames
:: ===================================================

:: -------------------------------
:: Default Configuration
:: -------------------------------
set "PROJECT_NAME=Game"
set "BUILD_DIR=build"
set "ASSET_SRC=assets"
set "ICON_RES=resource.rc"
set "RAYLIB_LIB=..\lib\libraylib.a"
set "BUILD_MODE=Release"

echo [INFO] Detecting compiler...
for /f "tokens=*" %%i in ('where gcc 2^>nul') do set "GCC_PATH=%%i"
for /f "tokens=*" %%i in ('where g++ 2^>nul') do set "GXX_PATH=%%i"

:: Change \ to / so CMake is happy
if defined GCC_PATH set "GCC_PATH=%GCC_PATH:\=/%"
if defined GXX_PATH set "GXX_PATH=%GXX_PATH:\=/%"

if not defined GCC_PATH (
    echo [ERROR] GCC not found!
    pause & exit /b 1
)
:: -----------------------------------------------------

echo ===================================================
echo        SSOEngine - Automated Build System
echo ===================================================

:: ===================================================
:: READ EXISTING PROJECT NAME FROM RESOURCE.RC (IF AVAILABLE)
:: ===================================================
if exist "%ICON_RES%" (
    for /f "usebackq tokens=*" %%a in (`findstr "OriginalFilename" "%ICON_RES%"`) do (
        set "line=%%a"
        set "line=!line:*OriginalFilename=!"
        set "line=!line:~1!"
        set "line=!line:,=!"
        set "line=!line: =!"
        set "line=!line:;=!"
        set "PROJECT_NAME=!line:.exe=!"
    )
    
    if "!PROJECT_NAME!"=="" set "PROJECT_NAME=Game"
    echo [INFO] Loaded project: !PROJECT_NAME!
) else (
    echo [INFO] No existing resource.rc found. Using default.
)

:: ===================================================
:: PLATFORM SELECTION MENU
:: ===================================================
echo.
echo Select Build Platform:
echo [1] Windows (MinGW)
echo [2] Android (NDK)
echo [3] Clean Build
echo [4] Set Android NDK Path
echo.
set /p platform_choice="Choose [1/2/3/4]: "

if "%platform_choice%"=="3" (
    echo [INFO] Cleaning build directory...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
    echo [SUCCESS] Build directory cleaned.
    pause
    exit /b 0
)

if "%platform_choice%"=="4" (
    echo [INFO] Set Android NDK Path
    echo.
    
    if exist "config.txt" (
        set /p NDK_PATH=<config.txt
        echo Current NDK Path: !NDK_PATH!
        echo.
        echo [1] Keep current path
        echo [2] Set new path
        echo.
        set /p ndk_choice="Choose [1/2]: "
        
        if "!ndk_choice!"=="2" (
            echo.
            set /p new_ndk_path="Enter new NDK path: "
            if not "!new_ndk_path!"=="" (
                echo !new_ndk_path! > config.txt
                set "NDK_PATH=!new_ndk_path!"
                echo [SUCCESS] NDK Path updated to: !NDK_PATH!
            ) else (
                echo [ERROR] No path provided.
            )
        ) else (
            echo [INFO] NDK Path unchanged: !NDK_PATH!
        )
    ) else (
        echo No NDK path configured.
        echo.
        set /p new_ndk_path="Enter NDK path: "
        if not "!new_ndk_path!"=="" (
            echo !new_ndk_path! > config.txt
            set "NDK_PATH=!new_ndk_path!"
            echo [SUCCESS] NDK Path set to: !NDK_PATH!
        ) else (
            echo [ERROR] No path provided.
        )
    )
    echo.
    pause
    goto :start_menu
)

if "%platform_choice%"=="2" (
    goto :android_build
) else (
    goto :windows_build
)

:android_build
echo [INFO] Android Build Selected
set "NDK_PATH="
if exist config.txt (
    for /f "usebackq delims=" %%i in ("config.txt") do set "RAW_PATH=%%i"
    set "NDK_PATH=!RAW_PATH: =!"
    set "NDK_PATH=!NDK_PATH:\=/!"
    echo [INFO] Using NDK from config: !NDK_PATH!
) else (
    echo [INFO] No config.txt found, attempting auto-detection...
    for %%p in ("%PROGRAMFILES%\Android\Sdk\ndk" "%PROGRAMFILES(X86)%\Android\Sdk\ndk" "%LOCALAPPDATA%\Android\Sdk\ndk") do (
        if exist "%%p" (
            set "NDK_PATH=%%p"
            set "NDK_PATH=!NDK_PATH:\=/!"
            echo !NDK_PATH! > config.txt
            goto :ndk_found
        )
    )
    echo [ERROR] Android NDK not found in standard locations
    echo Please use option [4] to set NDK path manually
    pause
    exit /b 1
)

:ndk_found
echo [INFO] Using NDK at: !NDK_PATH!

:: Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: Configure CMake for Android
cmake -S . -B %BUILD_DIR% -G "MinGW Makefiles" ^
    -DCMAKE_TOOLCHAIN_FILE="!NDK_PATH!/build/cmake/android.toolchain.cmake" ^
    -DANDROID_ABI=arm64-v8a ^
    -DANDROID_PLATFORM=android-29 ^
    -DPLATFORM=ANDROID ^
    -DCMAKE_MAKE_PROGRAM=make

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed for Android
    pause
    exit /b %errorlevel%
)

:: Build
cmake --build %BUILD_DIR%
if %errorlevel% neq 0 (
    echo [ERROR] Android build failed
    pause
    exit /b %errorlevel%
)

echo [SUCCESS] Android build completed: libSSOEngine.so

:: ===================================================
:: COPY TO ANDROID FOLDERS
:: ===================================================
echo [INFO] Copying library to Android folders...

if exist "..\..\SSOEngine\Android\app\src\main\jniLibs" (
    copy "%BUILD_DIR%\libSSOEngine.so" "..\..\SSOEngine\Android\app\src\main\jniLibs\" >nul
    echo [SUCCESS] Copied to jniLibs folder
) else (
    echo [WARNING] jniLibs folder not found
)

if exist "..\..\SSOEngine\Android\app\libs" (
    copy "%BUILD_DIR%\libSSOEngine.so" "..\..\SSOEngine\Android\app\libs\" >nul
    echo [SUCCESS] Copied to libs folder
) else (
    echo [WARNING] libs folder not found
)

echo [INFO] Android library deployment completed

:: ===================================================
:: APK BUILD STAGE
:: ===================================================
echo.
echo [INFO] Starting APK build...

:: CRITICAL CLEANUP: Prevent folder inception
rd /s /q "lib" 2>nul
rd /s /q "temp_pkg" 2>nul
rd /s /q "arm64-v8a" 2>nul

:: Auto-detect Android SDK build-tools
set "BUILD_TOOLS_PATH="
for %%p in ("%PROGRAMFILES%\Android\Sdk\build-tools" "%PROGRAMFILES(X86)%\Android\Sdk\build-tools" "%LOCALAPPDATA%\Android\Sdk\build-tools") do (
    if exist "%%p" (
        for /f "delims=" %%v in ('dir /b /ad "%%p" 2^>nul ^| sort /r') do (
            set "BUILD_TOOLS_PATH=%%p\%%v"
            goto :build_tools_found
        )
    )
)
echo [ERROR] Android SDK build-tools not found
echo Please install Android SDK with build-tools
pause
exit /b 1

:build_tools_found
echo [INFO] Using build-tools: !BUILD_TOOLS_PATH!

:: Path Sanitization: Strip any existing quotes
set "BUILD_TOOLS_PATH=!BUILD_TOOLS_PATH:"=!"

:: Dynamic SDK Root: Derive from BUILD_TOOLS_PATH
for %%I in ("!BUILD_TOOLS_PATH!\..\..") do set "SDK_ROOT=%%~fI"

:: Verify android.jar exists, fallback to any available version
if not exist "!SDK_ROOT!\platforms\android-33\android.jar" (
    echo [WARNING] android-33 not found, searching for available version...
    for /d %%P in ("!SDK_ROOT!\platforms\android-*") do (
        if exist "%%P\android.jar" (
            set "ANDROID_JAR_PATH=%%P\android.jar"
            echo [INFO] Using fallback: !ANDROID_JAR_PATH!
            goto :jar_found
        )
    )
    echo [ERROR] No android.jar found in platforms folder
    echo.
    echo [FALLBACK] Please enter android.jar path manually:
    set /p MANUAL_JAR_PATH="Enter full path to android.jar: "
    if "!MANUAL_JAR_PATH!"=="" (
        echo [ERROR] No path provided
        pause
        exit /b 1
    )
    
    :: Validate manual path
    if not exist "!MANUAL_JAR_PATH!" (
        echo [ERROR] File not found: !MANUAL_JAR_PATH!
        pause
        exit /b 1
    )
    
    :: Check if it's actually android.jar
    echo !MANUAL_JAR_PATH! | findstr /i "android.jar" >nul
    if !errorlevel! neq 0 (
        echo [ERROR] File must be android.jar
        pause
        exit /b 1
    )
    
    set "ANDROID_JAR_PATH=!MANUAL_JAR_PATH!"
    echo [SUCCESS] Using manual android.jar: !ANDROID_JAR_PATH!
) else (
    set "ANDROID_JAR_PATH=!SDK_ROOT!\platforms\android-33\android.jar"
)

:jar_found

:: Create Android directory structure
if not exist "android\libs\arm64-v8a" mkdir "android\libs\arm64-v8a"
if not exist "android\jniLibs\arm64-v8a" mkdir "android\jniLibs\arm64-v8a"
if not exist "android\bin" mkdir "android\bin"
if not exist "android\gen" mkdir "android\gen"
if not exist "android\obj" mkdir "android\obj"

:: Copy .so file to Android lib folders
if not exist "android\libs\arm64-v8a" mkdir "android\libs\arm64-v8a"
copy /Y "%BUILD_DIR%\libSSOEngine.so" "android\libs\arm64-v8a\"
if !errorlevel! neq 0 (
    echo [ERROR] Failed to copy .so file to libs
    pause
    exit /b 1
)

copy /Y "%BUILD_DIR%\libSSOEngine.so" "android\jniLibs\arm64-v8a\libSSOEngine.so"
if !errorlevel! neq 0 (
    echo [ERROR] Failed to copy .so file to jniLibs
    pause
    exit /b 1
)

:: Create R.java file
call "!BUILD_TOOLS_PATH!\aapt.exe" package -f -m -J "android\src" -M "android\AndroidManifest.xml" -S "android\res" -I "!ANDROID_JAR_PATH!" -F "android\bin\SSOEngine.unsigned.apk"
if !errorlevel! neq 0 (
    echo [ERROR] Failed to create R.java
    pause
    exit /b 1
)

:: Compile Java files
javac -d "android\bin" -classpath "!ANDROID_JAR_PATH!" "android\src\com\ssogames\ssoengine\*.java"
if !errorlevel! neq 0 (
    echo [ERROR] Failed to compile Java files
    pause
    exit /b 1
)

:: PROPER STAGING: Create temporary staging folder for native library
echo [INFO] Creating staging folder for native library...
if exist "temp_lib_staging" rd /s /q "temp_lib_staging"
mkdir "temp_lib_staging\lib\arm64-v8a"
copy /Y "%BUILD_DIR%\libSSOEngine.so" "temp_lib_staging\lib\arm64-v8a\"

if !errorlevel! neq 0 (
    echo [ERROR] Failed to copy .so to staging folder
    pause
    exit /b 1
)

:: Create DEX file
echo [INFO] Creating DEX file...
if not exist "android\bin" mkdir "android\bin"

set "D8_EXE=!BUILD_TOOLS_PATH!\d8.bat"
call "!D8_EXE!" --output "android\bin\" "android\bin\com\ssogames\ssoengine\*.class" --lib "!ANDROID_JAR_PATH!"

if !errorlevel! neq 0 (
    echo [ERROR] Failed to create DEX file
    pause
    exit /b 1
)

:: ADD DEX TO APK (THE CORRECT WAY)
echo [INFO] Injecting DEX into APK root...

set "AAPT_EXE=!BUILD_TOOLS_PATH!\aapt.exe"

pushd "android\bin"
"!AAPT_EXE!" add "SSOEngine.unsigned.apk" "classes.dex"
popd

if !errorlevel! neq 0 (
    echo [ERROR] Failed to add DEX to APK
    pause
    exit /b 1
)

:: APK PACKAGING: Add library using pushd/popd from staging folder
echo [INFO] Adding native library to APK...
pushd "temp_lib_staging"
"!BUILD_TOOLS_PATH!\aapt.exe" add "..\android\bin\SSOEngine.unsigned.apk" "lib/arm64-v8a/libSSOEngine.so"
popd

if !errorlevel! neq 0 (
    echo [ERROR] Failed to add native library to APK
    pause
    exit /b 1
)

:: Cleanup staging folder
rd /s /q "temp_lib_staging"

:: Align APK
call "!BUILD_TOOLS_PATH!\zipalign.exe" -f 4 android\bin\SSOEngine.unsigned.apk android\bin\SSOEngine.aligned.apk
if !errorlevel! neq 0 (
    echo [ERROR] Failed to align APK
    pause
    exit /b 1
)

:: Generate ssoengine keystore if not exists
if not exist "android\ssoengine.keystore" (
    echo [INFO] Creating ssoengine keystore...
    if not exist "android" mkdir "android"
    keytool -genkey -v -keystore "android\ssoengine.keystore" -storepass rozak123 -alias ssoengine -keypass rozak123 -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=SSOEngine,O=SSOGames,C=US"
    if !errorlevel! neq 0 (
        echo [ERROR] Failed to create ssoengine keystore
        pause
        exit /b 1
    )
)

:: FIX SIGNING: Force V2 signature for Snapdragon 665/Android 11
call "!BUILD_TOOLS_PATH!\apksigner.bat" sign --ks "android\ssoengine.keystore" --ks-pass pass:rozak123 --v2-signing-enabled true --out "android\bin\SSOEngine.apk" "android\bin\SSOEngine.unsigned.apk"
if !errorlevel! neq 0 (
    echo [ERROR] Failed to sign APK
    pause
    exit /b 1
)

:: Cleanup temp directory
if exist "temp_pkg" rd /s /q "temp_pkg"

echo [SUCCESS] APK built successfully: android\bin\SSOEngine.apk

:: FINAL VERIFICATION: Check APK contents
echo [INFO] Verifying APK signature and contents...
call "!BUILD_TOOLS_PATH!\apksigner.bat" verify --verbose android\bin\SSOEngine.apk
if !errorlevel! neq 0 (
    echo [WARNING] APK verification failed - may not install on all devices
) else (
    echo [SUCCESS] APK verification passed - ready for distribution
)

echo [INFO] APK Contents Verification:
"!BUILD_TOOLS_PATH!\aapt.exe" list "android\bin\SSOEngine.apk"

pause
exit /b 0

:windows_build
:: ===================================================
:: WINDOWS BUILD CONFIGURATION
:: ===================================================
echo [INFO] Windows Build Selected

:: -------------------------------
:: BUILD MODE SELECTION
:: -------------------------------
echo.
echo Select Build Mode:
echo [1] Release (No console window)
echo [2] Debug (With console window for debugging)
echo.
set /p mode_choice="Choose [1/2]: "

if "%mode_choice%"=="2" (
    set "BUILD_MODE=Debug"
    :: DEBUG MODE: Console window appears (NO -mwindows flag)
    set "CONSOLE_FLAG="
    echo [INFO] Debug Mode: Console enabled
) else (
    set "BUILD_MODE=Release"
    :: RELEASE MODE: No console window
    set "CONSOLE_FLAG=-mwindows -Wl,--subsystem,windows"
    echo [INFO] Release Mode: Console disabled
)

:: ===================================================
:: RAYLIB CHECK
:: ===================================================
if not exist "%RAYLIB_LIB%" (

echo.
echo [SSOEngine] Raylib not found. Installing...

if not exist "..\lib" mkdir "..\lib"
if not exist "..\include" mkdir "..\include"

echo [INFO] Downloading Raylib...

powershell -Command "Invoke-WebRequest https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip -OutFile raylib.zip"

if not exist "raylib.zip" (
echo [ERROR] Failed to download Raylib.
pause
exit /b 1
)

echo [INFO] Extracting Raylib...

powershell -Command "Expand-Archive raylib.zip -DestinationPath raylib_temp"

echo [INFO] Installing Raylib...

copy "raylib_temp\raylib-5.0_win64_mingw-w64\lib\libraylib.a" "..\lib\" >nul
copy "raylib_temp\raylib-5.0_win64_mingw-w64\include\raylib.h" "..\include\" >nul
copy "raylib_temp\raylib-5.0_win64_mingw-w64\include\raymath.h" "..\include\" >nul
copy "raylib_temp\raylib-5.0_win64_mingw-w64\include\rlgl.h" "..\include\" >nul

rmdir /s /q raylib_temp
del raylib.zip

)

:: Final check
if not exist "%RAYLIB_LIB%" (
echo.
echo [ERROR] Raylib installation failed.
echo Please install manually.
pause
exit /b 1
)

echo [SUCCESS] Raylib ready.

:: ===================================================
:: ICON CHECK
:: ===================================================
if not exist "app_icon.ico" (
    echo [WARNING] app_icon.ico not found. Creating dummy icon...
    powershell -Command "Add-Type -AssemblyName System.Drawing; $bmp = New-Object System.Drawing.Bitmap 32,32; $graphics = [System.Drawing.Graphics]::FromImage($bmp); $graphics.Clear([System.Drawing.Color]::Blue); $bmp.Save('app_icon.ico', [System.Drawing.Imaging.ImageFormat]::Icon); $graphics.Dispose(); $bmp.Dispose()"
    if not exist "app_icon.ico" (
        echo [ERROR] Failed to create dummy icon. Please provide app_icon.ico
        pause
        exit /b 1
    )
    echo [SUCCESS] Dummy icon created.
)

:: ===================================================
:: METADATA WIZARD
:: ===================================================

set /p update_meta="Do you want to edit Game Details? [y/n]: "

if /i "!update_meta!"=="y" goto :edit_metadata
goto :skip_metadata

:edit_metadata
echo.

set /p g_name="Enter Game Name: "
if "!g_name!"=="" set "g_name=Game"
set "g_name=!g_name: =_!"
set "PROJECT_NAME=!g_name!"

set /p g_desc="Enter Game Description: "
if "!g_desc!"=="" set "g_desc=My Game"

set /p g_comp="Enter Company Name: "
if "!g_comp!"=="" set "g_comp=My Company"

set /p g_vers="Enter Version (e.g. 1.0.0.0): "
if "!g_vers!"=="" set "g_vers=1.0.0.0"

echo [SSOEngine] Updating resource.rc...

(
echo // AUTO GENERATED BY SSOENGINE
echo IDI_ICON1 ICON DISCARDABLE "./app_icon.ico"
echo.
echo 1 VERSIONINFO
echo FILEVERSION 1,0,0,0
echo PRODUCTVERSION 1,0,0,0
echo BEGIN
echo   BLOCK "StringFileInfo"
echo   BEGIN
echo     BLOCK "040904b0"
echo     BEGIN
echo       VALUE "CompanyName", "!g_comp!"
echo       VALUE "FileDescription", "!g_desc!"
echo       VALUE "FileVersion", "!g_vers!"
echo       VALUE "InternalName", "!g_name!"
echo       VALUE "OriginalFilename", "!g_name!.exe"
echo       VALUE "ProductName", "!g_name!"
echo       VALUE "ProductVersion", "!g_vers!"
echo     END
echo   END
echo   BLOCK "VarFileInfo"
echo   BEGIN
echo     VALUE "Translation", 0x409, 1200
echo   END
echo END
) > %ICON_RES%

echo [SUCCESS] Metadata saved.
goto :skip_metadata

:skip_metadata

:: ===================================================
:: BUILD DIRECTORY
:: ===================================================

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

set "OUTPUT_EXE=%BUILD_DIR%\%PROJECT_NAME%.exe"

:: ===================================================
:: BUILD TOOLS (SSO PACKER)
:: ===================================================

if exist "%ASSET_SRC%" (

echo.
echo [INFO] Checking Build Tools...

pushd ..\00_BuildTools

if not exist "sso_packer.exe" (

echo [INFO] Compiling SSO Asset Packer...

g++ sso_packer.cpp -o sso_packer.exe -std=c++17

if !errorlevel! neq 0 (
echo [ERROR] Failed to compile sso_packer.
popd
pause
exit /b 1
)

echo [SUCCESS] SSO Packer compiled.

)

echo.
echo ==========================================
echo    SSOEngine Asset Packer
echo ==========================================

sso_packer.exe

popd

)

:: ===================================================
:: CMAKE BUILD FOR WINDOWS
:: ===================================================

echo.
echo [INFO] Configuring CMake for Windows...

cmake -S . -B %BUILD_DIR% -G "MinGW Makefiles" ^
    -DPLATFORM=DESKTOP ^
    -DCMAKE_C_COMPILER="%GCC_PATH%" ^
    -DCMAKE_CXX_COMPILER="%GXX_PATH%"

if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed for Windows
    pause
    exit /b %errorlevel%
)

echo [INFO] Building with CMake...
cmake --build %BUILD_DIR%

if %errorlevel% neq 0 (
    echo [ERROR] Windows build failed
    pause
    exit /b %errorlevel%
)

:: ===================================================
:: RESULT
:: ===================================================

if %errorlevel% neq 0 (

echo.
echo [ERROR] Build Failed
echo.

echo Troubleshooting:
echo 1. Ensure MinGW-w64 is installed
echo 2. Ensure raylib installed correctly
echo 3. Verify code syntax

pause
exit /b %errorlevel%

)

echo.
echo ===================================================
echo BUILD SUCCESSFUL
echo ===================================================
echo Mode      : %BUILD_MODE%
echo EXE       : %OUTPUT_EXE%
echo ===================================================

if "%BUILD_MODE%"=="Debug" (
    echo.
    echo [INFO] Debug mode: Console will appear when you run the .exe
    echo [INFO] You can see printf/cout output in the console
)

pause
