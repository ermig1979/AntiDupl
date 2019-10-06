@echo off
set WORK_DIR=..\src\3rd\Simd
set ORIGIN_SRCDIR=..\src\3rd.git\Simd\src\Simd
set ORIGINDIR_FULLPATH=%~dp0..\src\3rd.git\Simd

if not "%1"=="overwrite" (
echo Confirm overwrite "%WORK_DIR%" from "%ORIGIN_SRCDIR%"?
pause
)

if exist %ORIGIN_SRCDIR% (
  ..\src\3rd.git\Simd\prj\cmd\GetVersion.cmd %ORIGINDIR_FULLPATH%
  robocopy %ORIGIN_SRCDIR% %WORK_DIR% /MIR
  
  REM Clean up to avoid dirty submodule
  del %ORIGIN_SRCDIR%\SimdVersion.h
  del %ORIGINDIR_FULLPATH%\prj\txt\FullVersion.txt
  
  if not "%1"=="overwrite" pause
)
