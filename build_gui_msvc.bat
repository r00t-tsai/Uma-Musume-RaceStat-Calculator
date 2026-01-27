@echo off
echo ===================================================
echo    Uma Musume Calculator - GUI Build (MSVC)       
echo ===================================================
echo.

REM Check for MSVC
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: MSVC compiler not found!
    echo Please run from Developer Command Prompt for VS
    pause
    exit /b 1
)

echo Cleaning previous build...
if exist *.obj del *.obj
if exist uma_calculator_gui.exe del uma_calculator_gui.exe
if exist *.pdb del *.pdb
if exist *.ilk del *.ilk

echo.
echo Building GUI version with MSVC...
echo.

REM Compile - Note: Using main_gui.cpp instead of main.cpp
cl /nologo /EHsc /std:c++17 /O2 /W3 ^
   /Fe:uma_calculator_gui.exe ^
   /Iinclude ^
   src\main_gui.cpp ^
   src\calculator.cpp ^
   src\skill.cpp ^
   src\stats.cpp ^
   user32.lib gdi32.lib comctl32.lib ^
   /link /SUBSYSTEM:WINDOWS

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ===================================================
    echo  Build FAILED! Check errors above.
    echo ===================================================
    echo.
    pause
    exit /b 1
)

echo.
echo Cleaning up intermediate files...
if exist *.obj del *.obj
if exist *.ilk del *.ilk

echo.
echo ===================================================
echo  GUI Build SUCCESSFUL!
echo ===================================================
echo.
echo Run: uma_calculator_gui.exe
echo.
echo This will open a Windows GUI application.
echo.
pause
