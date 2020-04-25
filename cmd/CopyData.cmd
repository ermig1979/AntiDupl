@echo off
set SRC_DIR=%1
set OUT_DIR=%2

robocopy "%SRC_DIR%\data\resources" "%OUT_DIR%\data\resources" /MIR
if %ERRORLEVEL% GEQ 8 goto FAIL
robocopy "%SRC_DIR%\docs\data\resources" "%OUT_DIR%\data\resources" /S
if %ERRORLEVEL% GEQ 8 goto FAIL

exit /b 0
:FAIL
echo error! exitcode: %ERRORLEVEL%
exit /b %ERRORLEVEL%
