@echo off
set DATA_DIR=%1
set OUT_DIR=%2

if exist %OUT_DIR%\data (
echo Delete old data files:
erase %OUT_DIR%\data\* /q /s /f
rmdir %OUT_DIR%\data /q /s
)

echo Copy new data files:
mkdir %OUT_DIR%\data
mkdir %OUT_DIR%\data\resources
mkdir %OUT_DIR%\data\help

xcopy %DATA_DIR%\resources\* %OUT_DIR%\data\resources\* /y /i /s
xcopy %DATA_DIR%\help\* %OUT_DIR%\data\help\* /y /i /s
