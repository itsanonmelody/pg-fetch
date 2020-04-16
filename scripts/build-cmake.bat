@ECHO OFF

set PLATFORM=%1
IF /I "%PLATFORM%"=="windows" (
    call cmake --debug-output -Wno-dev -G "Visual Studio 16 2019" -A "Win32" -S . -B .build
) ELSE (
    echo %PLATFORM% is not supported!
)