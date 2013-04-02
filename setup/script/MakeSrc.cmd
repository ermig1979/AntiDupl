@echo off

set RAR_EXE="C:\Program Files\WinRAR\WinRar.exe"
if not exist %RAR_EXE% (
echo Execution file "%RAR_EXE%" is to exists!
exit 1
)

set TRUNK_DIR=..\..
set VERSION_FILE=%TRUNK_DIR%\src\AntiDupl\version.txt

if not exist %VERSION_FILE% (
echo Can't find "%VERSION_FILE%" file!
exit 0
)

set VERSION=
for /f "delims=" %%i in ('type %VERSION_FILE%') do set VERSION=%%i

set OUT_DIR=%TRUNK_DIR%\setup\out\src
set TMP_DIR=%TRUNK_DIR%\setup\out\src\AntiDupl.NET-%VERSION%

if not exist %OUT_DIR% mkdir %OUT_DIR%

if exist %TMP_DIR% (
echo Delete old files:
erase %TMP_DIR%\* /q /s /f
rmdir %TMP_DIR% /q /s
)

if not exist %TMP_DIR% mkdir %TMP_DIR%

xcopy %TRUNK_DIR%\src\* %TMP_DIR%\src\* /y /i /s
xcopy %TRUNK_DIR%\data\* %TMP_DIR%\data\* /y /i /s
xcopy %TRUNK_DIR%\setup\script\* %TMP_DIR%\setup\script\* /y /i /s

erase %TMP_DIR%\src\*.user /q /s /f
erase %TMP_DIR%\src\*.suo /q /s /f
erase %TMP_DIR%\src\*.ncb /q /s /f

%RAR_EXE% a -afzip -ep1 -r %OUT_DIR%\AntiDupl.NET-%VERSION%.zip %TMP_DIR%
