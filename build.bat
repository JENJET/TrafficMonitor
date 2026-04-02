@echo off

rem 设置命令行窗口编码为UTF-8
chcp 65001 >nul

rem 编译TrafficMonitor项目解决方案

echo Starting to build TrafficMonitor project...
echo ================================

rem 直接使用找到的msbuild路径
set "MSBUILD_PATH=C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"

rem 检查msbuild是否存在
if not exist "%MSBUILD_PATH%" (
    echo Error: Cannot find msbuild command. Please make sure Visual Studio 2026 is installed correctly.
    echo Path not found: %MSBUILD_PATH%
    pause
    exit /b 1
)

echo Found msbuild: %MSBUILD_PATH%

rem 然后编译TrafficMonitor项目的所有配置（包括lite配置）
call :build_project "TrafficMonitor.sln" "Debug (lite)" x64
call :build_project "TrafficMonitor.sln" "Debug" x64
call :build_project "TrafficMonitor.sln" "Release (lite)" x64
call :build_project "TrafficMonitor.sln" "Release" x64
call :build_project "TrafficMonitor.sln" "Debug (lite)" x86
call :build_project "TrafficMonitor.sln" "Debug" x86
call :build_project "TrafficMonitor.sln" "Release (lite)" x86
call :build_project "TrafficMonitor.sln" "Release" x86

echo ================================
echo Build successful!
echo Executable files are located in:
echo x64: D:\projects\TrafficMonitor\Bin\x64\*
echo x86: D:\projects\TrafficMonitor\Bin\Win32\*

pause
exit /b 0

:build_project
set PROJECT=%1
set CONFIG=%2
set PLATFORM=%3
echo Building %PROJECT% - %CONFIG% %PLATFORM%...
"%MSBUILD_PATH%" %PROJECT% /t:build /p:Configuration=%CONFIG% /p:Platform=%PLATFORM% /m
if errorlevel 1 (
    echo Build failed for %PROJECT% - %CONFIG% %PLATFORM%!
    pause
    exit /b 1
)
goto :eof