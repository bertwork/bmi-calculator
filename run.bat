@echo off
if not exist build mkdir build
echo Compiling BMI Calculator...
g++ -std=c++17 -Iheaders src/main.cpp src/app.cpp src/file_manager.cpp src/ui.cpp src/user.cpp src/bmi_service.cpp -o build/bmi_calculator.exe
if %errorlevel% equ 0 (
    echo Compilation successful! Running program...
    .\build\bmi_calculator.exe
) else (
    echo Compilation failed. Make sure g++ is installed and in your PATH.
)
pause