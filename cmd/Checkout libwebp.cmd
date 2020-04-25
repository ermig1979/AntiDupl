@echo off
set WORK_DIR=..\src\3rd\libwebp
set ORIGIN_DIR=..\src\3rd.git\libwebp

if not "%1"=="overwrite" (
echo Confirm overwrite "%WORK_DIR%"' files from "%ORIGIN_DIR%"?
pause
)

if exist %ORIGIN_DIR% (
  robocopy %ORIGIN_DIR%\imageio %WORK_DIR%\imageio /MIR
  robocopy %ORIGIN_DIR%\src %WORK_DIR%\src /MIR
  if not "%1"=="overwrite" pause
)
