@echo off
rem

rem Path to generated files
set OUTPUT_DIR=Build

rem create if doesnt exist
if not exist %OUTPUT_DIR% (
    mkdir %OUTPUT_DIR%
)

rem Arduino CLI path
set ARDUINO_CLI_PATH=3rdParty\arduino\arduino-cli.exe

rem  HW_Task3 path
set ARDUINO_PROJECT_PATH=src\HW_Task3

rem port and board
set BOARD=arduino:avr:mega
set PORT=COM3

rem Compiled fikes will be saved in OUTPUT_DIR
rem  --build-path for saved files

%ARDUINO_CLI_PATH% compile --fqbn %BOARD% --build-path %OUTPUT_DIR% %ARDUINO_PROJECT_PATH%
if %errorlevel% neq 0 (
    echo "Arduino project compilation failed"
    exit /b %errorlevel%
)

rem  Arduino upload
%ARDUINO_CLI_PATH% upload -p %PORT% --fqbn %BOARD% --build-path %OUTPUT_DIR% %ARDUINO_PROJECT_PATH%
if %errorlevel% neq 0 (
    echo "Arduino project upload failed"
    exit /b %errorlevel%
)

echo "Arduino project uploaded successfully!"

rem SW_Task3 path
set CLIENT_APP_PATH=src\SW_Task3\SW_Task3\SW_Task3.cpp

rem create dir for clients's exe
set CLIENT_BUILD_OUTPUT=%OUTPUT_DIR%\SW_Task3_build

if not exist %CLIENT_BUILD_OUTPUT% (
    mkdir %CLIENT_BUILD_OUTPUT%
)

rem MinGW 
where g++ > nul 2>&1
if %errorlevel% neq 0 (
    echo "MinGW (g++) not found, please ensure it's installed and added to the system PATH."
    exit /b %errorlevel%
)

rem compile with MinGW (g++)
g++ -o %CLIENT_BUILD_OUTPUT%\SW_Task3.exe %CLIENT_APP_PATH% -lgdi32 -luser32

if %errorlevel% neq 0 (
    echo "Client application build failed"
    exit /b %errorlevel%
)

echo "Client application built successfully!"

rem start client
start "" %CLIENT_BUILD_OUTPUT%\SW_Task3.exe
