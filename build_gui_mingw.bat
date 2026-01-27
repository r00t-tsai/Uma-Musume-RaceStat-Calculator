@echo off
echo ===================================================
echo    Uma Musume Calculator - GUI Build (MinGW)       
echo ===================================================
echo.

REM Check for g++
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: g++ not found!
    echo Please install MinGW-w64
    pause
    exit /b 1
)

echo Cleaning previous build...
if exist obj_gui rmdir /s /q obj_gui
if exist uma_calculator_gui.exe del uma_calculator_gui.exe

echo Creating obj_gui directory...
mkdir obj_gui

echo.
echo Compiling GUI version with MinGW...
echo.

g++ -std=c++17 -O2 -Wall -Iinclude -c -o obj_gui/calculator.o src/calculator.cpp
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -O2 -Wall -Iinclude -c -o obj_gui/main_gui.o src/main_gui.cpp
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -O2 -Wall -Iinclude -c -o obj_gui/skill.o src/skill.cpp
if %ERRORLEVEL% NEQ 0 goto :error

g++ -std=c++17 -O2 -Wall -Iinclude -c -o obj_gui/stats.o src/stats.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo Linking GUI application...
g++ -std=c++17 -mwindows -o uma_calculator_gui.exe ^
    obj_gui/calculator.o ^
    obj_gui/main_gui.o ^
    obj_gui/skill.o ^
    obj_gui/stats.o ^
    -lcomctl32 -lgdi32 -luser32

if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo ===================================================
echo  GUI Build SUCCESSFUL!
echo ===================================================
echo.
echo Run: uma_calculator_gui.exe
echo.
pause
exit /b 0

:error
echo.
echo ===================================================
echo  Build FAILED! Check errors above.
echo ===================================================
echo.
pause
exit /b 1
