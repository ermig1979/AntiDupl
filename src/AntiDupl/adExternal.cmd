@echo off

echo Generate adExternal.h file:

set ROOT_DIR=..\..
set SOLUTION_DIR=%ROOT_DIR%\src\
set PROJECT_DIR=%SOLUTION_DIR%\AntiDupl
set EXTERNAL_FILE=%PROJECT_DIR%\adExternal.h
set VERSION_FILE=%SOLUTION_DIR%\version.txt

if not exist %VERSION_FILE% (
echo Can't find "%VERSION_FILE%" file!
exit 0
)

set VERSION=
for /f "delims=" %%i in ('type %VERSION_FILE%') do set VERSION=%%i

echo /*>%EXTERNAL_FILE%
echo * AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).>>%EXTERNAL_FILE%
echo *>>%EXTERNAL_FILE%
echo * Copyright (c) 2002-2018 Yermalayeu Ihar.>>%EXTERNAL_FILE%
echo *>>%EXTERNAL_FILE%
echo * Permission is hereby granted, free of charge, to any person obtaining a copy>>%EXTERNAL_FILE% 
echo * of this software and associated documentation files (the "Software"), to deal>>%EXTERNAL_FILE%
echo * in the Software without restriction, including without limitation the rights>>%EXTERNAL_FILE%
echo * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell>>%EXTERNAL_FILE% 
echo * copies of the Software, and to permit persons to whom the Software is>>%EXTERNAL_FILE% 
echo * furnished to do so, subject to the following conditions:>>%EXTERNAL_FILE%
echo *>>%EXTERNAL_FILE%
echo * The above copyright notice and this permission notice shall be included in>>%EXTERNAL_FILE% 
echo * all copies or substantial portions of the Software.>>%EXTERNAL_FILE%
echo *>>%EXTERNAL_FILE%
echo * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR>>%EXTERNAL_FILE% 
echo * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,>>%EXTERNAL_FILE% 
echo * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE>>%EXTERNAL_FILE% 
echo * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER>>%EXTERNAL_FILE% 
echo * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,>>%EXTERNAL_FILE%
echo * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE>>%EXTERNAL_FILE%
echo * SOFTWARE.>>%EXTERNAL_FILE%
echo */>>%EXTERNAL_FILE%
echo.>>%EXTERNAL_FILE%
echo /*>>%EXTERNAL_FILE%                                                                                       
echo * File name   : adExternal.h>>%EXTERNAL_FILE%                                                       
echo * Description : This file contains information about current version.>>%EXTERNAL_FILE%               
echo *>>%EXTERNAL_FILE%                                  
echo * Do not change this file because the file is auto generated by script adExternal.cmd.>>%EXTERNAL_FILE%
echo */>>%EXTERNAL_FILE%
echo.>>%EXTERNAL_FILE%
echo #ifndef __adExternal_h__>>%EXTERNAL_FILE%
echo #define __adExternal_h__>>%EXTERNAL_FILE%
echo.>>%EXTERNAL_FILE%
echo #define AD_VERSION "%VERSION%">>%EXTERNAL_FILE%
echo.>>%EXTERNAL_FILE%
echo #endif//__adExternal_h__>>%EXTERNAL_FILE%
echo.>>%EXTERNAL_FILE%


