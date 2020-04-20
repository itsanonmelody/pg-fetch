@ECHO OFF

call cmake --debug-output -Wno-dev -G "Visual Studio 16 2019" -A x64 -S . -B .build