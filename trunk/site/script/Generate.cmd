@echo off

set DATA_DIR=..\..\data
set TEMPLATE_DIR=..\template
set OUT_DIR=..\out

if not exist %OUT_DIR% mkdir %OUT_DIR%

if exist %OUT_DIR% (
echo Delete old files:
erase %OUT_DIR%\* /q /s /f
)

echo Copy new files:
mkdir %OUT_DIR%\data
mkdir %OUT_DIR%\data\resources
mkdir %OUT_DIR%\data\help

xcopy %DATA_DIR%\resources\images\* %OUT_DIR%\data\resources\images\* /y /i /s
xcopy %DATA_DIR%\resources\icons\* %OUT_DIR%\data\resources\icons\* /y /i /s
xcopy %DATA_DIR%\help\* %OUT_DIR%\data\help\* /y /i /s

xcopy %TEMPLATE_DIR%\* %OUT_DIR%\* /y /i /s