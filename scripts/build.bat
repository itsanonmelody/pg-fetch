@ECHO OFF

set CONFIG=%1
IF "%CONFIG%"=="" set CONFIG=Debug

call cmake --build .build --config "%CONFIG%"