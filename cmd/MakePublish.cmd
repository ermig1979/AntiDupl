@echo off

set RAR_EXE="C:\Program Files\WinRAR\WinRar.exe"

set ROOT_DIR=..
set VERSION_FILE=%ROOT_DIR%\src\version.txt

if not exist %VERSION_FILE% (
echo Can't find "%VERSION_FILE%" file!
exit 0
)

set VERSION=
for /f "delims=" %%i in ('type %VERSION_FILE%') do set VERSION=%%i

set OUT_DIR=%ROOT_DIR%\out\publish
set TMP_DIR=%ROOT_DIR%\out\publish\AntiDupl.NET-%VERSION%

if not exist %OUT_DIR% mkdir %OUT_DIR%

if exist %TMP_DIR% (
echo Delete old files:
erase %TMP_DIR%\* /q /s /f
rmdir %TMP_DIR% /q /s
)

if not exist %TMP_DIR% mkdir %TMP_DIR%

set PUBLISH_DIR=%ROOT_DIR%\out\publish\AntiDupl.NET

robocopy "%ROOT_DIR%\data\resources" "%PUBLISH_DIR%\data\resources" /MIR
if %ERRORLEVEL% GEQ 8 exit 0
robocopy "%ROOT_DIR%\docs\data\resources" "%PUBLISH_DIR%\data\resources" /S
if %ERRORLEVEL% GEQ 8 exit 0

xcopy %ROOT_DIR%\bin\Publish\AntiDupl.dll %PUBLISH_DIR% /y
xcopy %ROOT_DIR%\bin\Publish\AntiDupl.pdb %PUBLISH_DIR% /y
xcopy %ROOT_DIR%\bin\Publish\AntiDupl.NET.WinForms.runtimeconfig.json %PUBLISH_DIR% /y
xcopy %ROOT_DIR%\bin\Publish\AntiDupl.NET.WPF.runtimeconfig.json %PUBLISH_DIR% /y

xcopy %PUBLISH_DIR% %TMP_DIR% /y /i /s

if exist %RAR_EXE% (
%RAR_EXE% a -ep1 -s -m5 -r -sfx %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.exe %TMP_DIR%
certutil -hashfile %OUT_DIR%\AntiDupl.NET-%VERSION%.exe SHA256 > %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.exe.hash.txt
%RAR_EXE% a -afzip -ep1 -r %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.zip %TMP_DIR%
certutil -hashfile %OUT_DIR%\AntiDupl.NET-%VERSION%.zip SHA256 > %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.zip.hash.txt
) else (
.\7-zip\7za_2201.exe a -sfx7z.sfx %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.exe %TMP_DIR%
certutil -hashfile %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.exe SHA256 > %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.exe.hash.txt
.\7-zip\7za_2201.exe a -tzip %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.zip .\%TMP_DIR%\*
certutil -hashfile %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.zip SHA256 > %OUT_DIR%\AntiDupl.NET-%VERSION%_SingleFilePortable.zip.hash.txt
)
