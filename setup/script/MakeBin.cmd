@echo off

set RAR_EXE="C:\Program Files\WinRAR\WinRar.exe"
if not exist %RAR_EXE% (
echo Execution file "%RAR_EXE%" is to exists!
exit 1
)

set TRUNK_DIR=..\..
set RELEASE_DIR=%TRUNK_DIR%\bin\Release
set VERSION_FILE=%TRUNK_DIR%\src\AntiDupl\version.txt

if not exist %RELEASE_DIR% (
echo Can't find "%RELEASE_DIR%" directory!
exit 0
)

if not exist %VERSION_FILE% (
echo Can't find "%VERSION_FILE%" file!
exit 0
)

set VERSION=
for /f "delims=" %%i in ('type %VERSION_FILE%') do set VERSION=%%i

set OUT_DIR=%TRUNK_DIR%\setup\out\bin
set TMP_DIR=%TRUNK_DIR%\setup\out\bin\AntiDupl.NET-%VERSION%

if not exist %OUT_DIR% mkdir %OUT_DIR%

if exist %TMP_DIR% (
echo Delete old files:
erase %TMP_DIR%\* /q /s /f
rmdir %TMP_DIR% /q /s
)

if not exist %TMP_DIR% mkdir %TMP_DIR%

xcopy %RELEASE_DIR%\data\* %TMP_DIR%\data\* /y /i /s
xcopy %RELEASE_DIR%\AntiDupl.NET.exe %TMP_DIR%\* /y /i /s
xcopy %RELEASE_DIR%\AntiDupl32.dll %TMP_DIR%\* /y /i /s
xcopy %RELEASE_DIR%\AntiDupl64.dll %TMP_DIR%\* /y /i /s

erase %TMP_DIR%\data\resources\strings\English.xml /q /s /f
erase %TMP_DIR%\data\resources\strings\Russian.xml /q /s /f

%RAR_EXE% a -ep1 -s -m5 -r -sfx %OUT_DIR%\AntiDupl.NET-%VERSION%.exe %TMP_DIR%
