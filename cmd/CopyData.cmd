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

xcopy %DATA_DIR%\resources\* %OUT_DIR%\data\resources\* /y /i /s /e
