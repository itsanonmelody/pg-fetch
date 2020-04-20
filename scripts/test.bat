@ECHO OFF

for /f "tokens=1,* delims= " %%a in ("%*") do set PARAMS=%%b

call "tests\.bin\%1" %PARAMS%