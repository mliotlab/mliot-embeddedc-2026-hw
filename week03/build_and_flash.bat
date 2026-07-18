@echo off
rem ============================================================================
rem  SCRIPT TỰ ĐỘNG HÓA BIÊN DỊCH VÀ NẠP FIRMWARE CHO STM32 BLUE PILL
rem ============================================================================

echo.
echo STEP 1: CLEANING BUILD DIRECTORY
echo Deleting existing build folder...
if exist build (
    rmdir /s /q build
    echo Old build folder removed successfully.
) else (
    echo No existing build folder found. Processing...
)

echo.
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
cmake -G Ninja -B build
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b %errorlevel%
)

echo.
echo STEP 3: COMPILING FIRMWARE WITH NINJA
ninja -C build
if %errorlevel% neq 0 (
    echo [ERROR] Compilation failed!
    pause
    exit /b %errorlevel%
)

echo.
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
rem Kiểm tra sự tồn tại của file firmware trước khi nạp
if not exist build\app_firmware.bin (
    echo [ERROR] Firmware file 'build/app_firmware.bin' not found!
    pause
    exit /b 1
)

rem Gọi STM32_Programmer_CLI kết nối qua ST-Link, giao thức SWD, nạp vào địa chỉ 0x08000000 và tự động Reset
STM32_Programmer_CLI -c port=SWD -w build/app_firmware.bin 0x08000000 -rst
if %errorlevel% neq 0 (
    echo [ERROR] Flashing firmware failed!
    pause
    exit /b %errorlevel%
)

echo.
echo ============================================================================
echo [SUCCESS] ALL STEPS COMPLETED SUCCESSFULLY!
echo ============================================================================
pause