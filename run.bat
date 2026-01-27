@echo off
title Uma Musume Race Calculator

if not exist uma_calculator.exe (
    echo Error: uma_calculator.exe not found!
    echo Please compile the program first.
    echo.
    pause
    exit /b 1
)

uma_calculator.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Program exited with error code: %ERRORLEVEL%
    pause
)
