@echo off

call CopyData.cmd ..\data ..\bin\Release
call CopyData.cmd ..\data ..\bin\Debug

xcopy ..\data\resources\icons\Icon.ico ..\src\AntiDupl\AntiDupl.NET /y /i

pause