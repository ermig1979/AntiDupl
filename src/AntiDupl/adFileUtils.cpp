/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include <tchar.h>
#include <windows.h>
#include <shellapi.h>

#include <io.h>

#include "adPerformance.h"
#include "adFileUtils.h"

namespace ad
{
    bool FileDelete(const TChar *fileName, bool toRecycle)
    {
        TChar buffer[MAX_PATH + 1];
        SHFILEOPSTRUCT shFileOpStruct;
        errno_t error;
        size_t length;

        length = _tcsnlen(fileName, MAX_PATH + 1);
        if (length >= MAX_PATH)
        {
#ifdef UNICODE
            return ::DeleteFile(fileName) != FALSE;
#else
            return false;
#endif
        }
        error = _tcscpy_s(buffer, MAX_PATH + 1, fileName);
        if(error != 0)
            return false;
        buffer[length + 1] = 0;

        ZeroMemory(&shFileOpStruct, sizeof(shFileOpStruct));
        shFileOpStruct.wFunc = FO_DELETE;
        shFileOpStruct.pTo = NULL;
        shFileOpStruct.pFrom = buffer;
        shFileOpStruct.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
        if(toRecycle)
            shFileOpStruct.fFlags |= FOF_ALLOWUNDO;
        else
            shFileOpStruct.fFlags &= ~FOF_ALLOWUNDO;

        return (SHFileOperation(&shFileOpStruct) == 0);
    }

    bool FileRename(const TChar *oldName, const TChar *newName)
    {
        TChar oldBuffer[MAX_PATH + 1];
        TChar newBuffer[MAX_PATH + 1];
        SHFILEOPSTRUCT shFileOpStruct;
        errno_t error;
        size_t oldLength, newLength;

        oldLength = _tcsnlen(oldName, MAX_PATH + 1);
        newLength = _tcsnlen(newName, MAX_PATH + 1);
        if (oldLength >= MAX_PATH || newLength >= MAX_PATH)
        {
#ifdef UNICODE
            return ::MoveFileEx(oldName, newName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != FALSE;
#else
            return false;
#endif
        }
        error = _tcscpy_s(oldBuffer, MAX_PATH + 1, oldName);
        if(error != 0)
            return false;
        oldBuffer[oldLength + 1] = 0;
        error = _tcscpy_s(newBuffer, MAX_PATH + 1, newName);
        if(error != 0)
            return false;
        newBuffer[newLength + 1] = 0;

        ZeroMemory(&shFileOpStruct, sizeof(shFileOpStruct));
        shFileOpStruct.wFunc = FO_MOVE;
        shFileOpStruct.pFrom = oldBuffer;
        shFileOpStruct.pTo = newBuffer;
        shFileOpStruct.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;

        return (SHFileOperation(&shFileOpStruct) == 0);
    }

    bool CreatePathTo(const TChar* path_)
    {
        TString path = CreatePath(path_);
        TChar *buffer = new TChar[path.size() + 1];
        TChar* p = (TChar*)path.c_str();
        TChar* b = buffer; 
        while(*p)
        {
            if((DELIMETER == *p) || (TEXT('/') == *p))
            {
                if(TEXT(':') != *(p-1))
                {
                    if(!IsDirectoryExists(buffer))
                    {
                        if(!CreateDirectory(buffer, NULL))
                        {
                            delete buffer;
                            return false;
                        }
                    }
                }
            }
            *b++ = *p++;
            *b = TEXT('\0');
        }
        if(!IsDirectoryExists(buffer))
        {
            if(!CreateDirectory(buffer, NULL))
            {
                delete buffer;
                return false;
            }
        }
        delete buffer;
        return true;
    }

    bool ClearDirectory(const TChar* directory)
    {
        if(!IsDirectoryExists(directory))
            return true;

        bool result = true;
        WIN32_FIND_DATA findData;

        TString searchPath = CreatePath(directory, TEXT("*"));
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
        if(hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                TString name = findData.cFileName;
                if(name != TEXT(".") && name != TEXT(".."))
                {
                    TString path = CreatePath(directory, name);
                    if(findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                    {
                        if(!ClearDirectory(path.c_str()))
                        {
                            result = false;
                            break;
                        }
                        if(!RemoveDirectory(path.c_str()))
                        {
                            result = false;
                            break;
                        }
                    }
                    else
                    {
                        if(!DeleteFile(path.c_str()))
                        {
                            result = false;
                            break;
                        }
                    }
                }
            } while(FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
        return result;
    }

    bool IsDirectoryExists(const TChar* directoryName)
    {
        DWORD fileAttribute = GetFileAttributes(directoryName);
        return ((fileAttribute != INVALID_FILE_ATTRIBUTES) &&
            (fileAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0);
    }

    bool IsFileExists(const TChar* fileName)
    {
        DWORD fileAttribute = GetFileAttributes(fileName);
        return (fileAttribute != INVALID_FILE_ATTRIBUTES);
    }

    TString GetApplicationDirectory()
    {
        return GetFileDirectory(GetApplicationPath());
    }

    TString GetApplicationName()
    {
        return GetFileName(GetApplicationPath(), false);
    }

    TString GetApplicationPath()
    {
        TChar path[MAX_PATH];
        DWORD result = GetModuleFileName(NULL, path, MAX_PATH);
        if(result == 0)
            return TString();
        if(result == MAX_PATH)
        {
#ifdef UNICODE
            TChar *buffer = new TChar[MAX_PATH_EX];
            TString str;
            result = GetModuleFileName(NULL, buffer, MAX_PATH_EX);
            if(result > 0 && result < MAX_PATH_EX)
                str = buffer;
            delete [] buffer;
            return str;
#else
            return TString();
#endif
        }
        return TString(path);  
    }

    TString GetSystemDirectory()
    {
        TChar systemDirectory[MAX_PATH];
        UINT error;

        error = GetWindowsDirectory(systemDirectory, MAX_PATH);
        if (error == 0)
            return TString();

        return TString(systemDirectory);
    }

    TString GetFileExtension(const TChar* path, size_t size)
    {
        ptrdiff_t i = size - 1;
        while(i >= 0 && path[i] != TEXT('.')) i--;
        if(i < 0)
            return TString();
        else
            return TString(path + i + 1);
    }

    TString GetFileExtension(const TChar* path)
    {
        return GetFileExtension(path, _tcslen(path));
    }

    TString GetFileName(const TChar* path, size_t size, bool withExtension)
    {
        if(withExtension)
        {
            ptrdiff_t i = size - 1;
            while(i >= 0 && path[i] != DELIMETER) i--;
            if(i < 0)
                return TString();
            else
                return TString(path + i + 1);
        }
        else
        {
            ptrdiff_t i = size - 1, j = size - 1; 
            while(i >= 0 && path[i] != DELIMETER) i--;
            while(j >= 0 && path[j] != TEXT('.')) j--;
            if(i < 0)
                return TString();
            else if(i > j)
                return TString(path + i + 1);
            else
                return TString(path + i + 1, j - i);
        }
    }

    TString GetFileName(const TChar* path, bool withExtension)
    {
        return GetFileName(path, _tcslen(path), withExtension);
    }

    TString GetFileDirectory(const TChar* path, size_t size)
    {
        ptrdiff_t i = size - 1;
        while(i >= 0 && path[i] != DELIMETER) i--;
        if(i < 0)
            return TString();
        else
            return TString(path, i);
    }

    TString GetFileDirectory(const TChar* path)
    {
        return GetFileDirectory(path, _tcslen(path));
    }

    TString CreatePath(const TString& path1, const TString& path2)
    {
        if(path1.empty())
            return TString();

        bool needDelimeter = path1[path1.size() - 1] != DELIMETER && path2.size() > 0;

        size_t size = path1.size() + (needDelimeter ? 1 : 0) + path2.size() + 1;

#ifdef UNICODE
        bool needPrefix = size > MAX_PATH && TString(path1.c_str(), EXTENDED_PATH_PREFIX_SIZE) != EXTENDED_PATH_PREFIX;
        if(size > MAX_PATH_EX - EXTENDED_PATH_PREFIX_SIZE)
            return TString();
#else
        bool needPrefix = false;
        if(size > MAX_PATH)
            return TString();
#endif
        return 
            (needPrefix ? TString(EXTENDED_PATH_PREFIX) : TString()) +
            path1 +
            (needDelimeter ? TString() + DELIMETER : TString()) +
            path2;
    }
    
    HGLOBAL LoadFileToMemory(const TChar* path)
    {
        AD_FUNCTION_PERFORMANCE_TEST
        HGLOBAL hGlobal = NULL;
        HANDLE hFile = ::CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile != INVALID_HANDLE_VALUE)
        {
            DWORD bytesRead = 0, fileSize = ::GetFileSize(hFile, NULL);
            if (fileSize != -1)
            {
                AD_PERFORMANCE_TEST_SET_SIZE(fileSize)
                hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, fileSize);
                if (hGlobal != NULL)
                {
                    void *data = ::GlobalLock(hGlobal);
                    ::ReadFile(hFile, data, fileSize, &bytesRead, NULL);
                    ::GlobalUnlock(hGlobal);
                    if (fileSize != bytesRead)
                    {
                        ::GlobalFree(hGlobal);
                        hGlobal = NULL;
                    }
                }
            }
            ::CloseHandle(hFile);
        }
        return hGlobal;
    }

	bool SearchFiles(const TString& directory, TStrings & files, bool subFolders, const TString & mask)
	{
		if(!IsDirectoryExists(directory.c_str()))
			return false;

		TString path;
		HANDLE hFind;
		WIN32_FIND_DATA findData;
		TString searchPath;

		if(subFolders)
		{
			searchPath = CreatePath(directory, TEXT("*"));
			hFind = ::FindFirstFile(searchPath.c_str(), &findData);
			if(hFind != INVALID_HANDLE_VALUE) 
			{
				do 
				{
					TString name = findData.cFileName;
					if(name == TEXT(".") || name == TEXT(".."))
						continue;
					if((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
						SearchFiles(CreatePath(directory, name), files, subFolders, mask);
				} while(::FindNextFile(hFind, &findData)); 
				::FindClose(hFind);
			}
		}

		searchPath = CreatePath(directory, mask);
		hFind = ::FindFirstFile(searchPath.c_str(), &findData);
		if(hFind != INVALID_HANDLE_VALUE) 
		{
			do 
			{
				TString name = findData.cFileName;
				if(name == TEXT(".") || name == TEXT(".."))
					continue;
				if((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)
					files.push_back(CreatePath(directory, name));
			} while(::FindNextFile(hFind, &findData)); 
			::FindClose(hFind);
		}

		return true;
	}

	bool DeleteFiles(const TString& directory, const TString& extension)
	{
		WIN32_FIND_DATA findData;
		bool result = true;
        TString searchPath = CreatePath(directory, TString(TEXT("*")) + extension);
        HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
        if(hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                TString name = findData.cFileName;
                if(name != TEXT(".") && name != TEXT(".."))
                {
                    TString pathForDel = CreatePath(directory, name);
                    
                    if(!DeleteFile(pathForDel.c_str()))
                    {
                        result = false;
                        break;
                    }
                }
            } while(FindNextFile(hFind, &findData));
            FindClose(hFind);
        }
		return result;
	}

	//--------------------------------------------------------------------------
	// Возвращает длину числа, переведенного в строку.
	size_t LengthOfLong(const __int64 digit)
	{
		char buffer[65];
		if (_i64toa_s(digit, buffer, 65, 10) == 0)
			return strnlen(buffer, _countof(buffer));
		else
			return 0;
	}

	// Ищет цифры в имени файла с конца.
	__int64 GetDigitInFileName(const TPath &path, TString & nameWithoutDigit, size_t & leadingZeros)
	{
		TString name = path.GetName(false);
		size_t length = name.length();
        //Находим первый не числовой символ с конца
        bool canRename;
        size_t digitPos = length;

		for (ptrdiff_t i = length - 1; i >= 0; digitPos = i, i-- )
        {
            if (!iswdigit(name[i])) //если не цифра выходим
                break;
        }

        if (digitPos <= length - 1) //если цифра найдена
            canRename = true;
        else
            canRename = false;

		__int64 result = -1;
        int numOfZero = 0;
        if (canRename)
        {
			TString forParsing = name.substr(digitPos, length - digitPos);
			result = _wtoi64(forParsing.c_str());
			if (result == _I64_MAX) //слишком длинный
			{
				digitPos = 0;
				result = -1;
			}
            leadingZeros = forParsing.length() - LengthOfLong(result);
        }

		if (digitPos > 0 && digitPos < length)
            nameWithoutDigit = name.substr(0, digitPos);
        else
            nameWithoutDigit = TString();
        return result;
	}

	// Перименовываем, добавляя _2 к имени файла.
	TString GetNewNameForFileAdd(const TPath &oldPath)
    {
		TString uniquePath;
		const int SIMILAR_PREFIX_SIZE = 16;
		const TChar *SIMILAR_PREFIX_FORMAT = TEXT("_%u");
		TChar buffer[SIMILAR_PREFIX_SIZE];
		unsigned long counter = 2;

		do 
		{
			_stprintf_s(buffer, SIMILAR_PREFIX_FORMAT, counter++);
			uniquePath = CreatePath(oldPath.GetDirectory(), oldPath.GetName(false) + TString(buffer) + oldPath.GetExtension());
		} 
		while (IsFileExists(uniquePath.c_str()));

        return uniquePath;
    }

	TString GetNewNameForFileAdd(const TPath &oldPath, const TPath &pathForRename)
    {
		TString uniquePath;
		const int SIMILAR_PREFIX_SIZE = 16;
		const TChar *SIMILAR_PREFIX_FORMAT = TEXT("_%u");
		TChar buffer[SIMILAR_PREFIX_SIZE];
		unsigned long counter = 2;

		do 
		{
			_stprintf_s(buffer, SIMILAR_PREFIX_FORMAT, counter++);
			uniquePath = CreatePath(oldPath.GetDirectory(), oldPath.GetName(false) + TString(buffer) + oldPath.GetExtension());
			if (TPath::EqualByNameWithExtension(uniquePath, pathForRename))
				break;
		} 
		while (IsFileExists(uniquePath.c_str()));
        return uniquePath;
    }

	// Перименовываем, увеличивая число в имени файла.
	TString GetNewNameForFileDigit(const TPath &oldPath, const TString &nameWithoutDigit, __int64 & counter, size_t & leadingZeros)
    {
		TString pathWithDigit;
		const int BUFFER_SIZE = 65;
		char buffer[BUFFER_SIZE];
		TString leadingZeroString;

		counter++;
		if (leadingZeros > 0)
		{
			if (LengthOfLong(counter) > LengthOfLong(counter - 1)) //если цифра удленилась
               leadingZeros--;
			for (size_t i = 0; i < leadingZeros; i++)
				leadingZeroString.push_back('0');
		}

		if (_i64toa_s(counter, buffer, BUFFER_SIZE, 10) == 0)
		{
			pathWithDigit = CreatePath(oldPath.GetDirectory(), nameWithoutDigit + leadingZeroString + TString(buffer) + oldPath.GetExtension());
			if (IsFileExists(pathWithDigit.c_str())) //если такой файл уже есть, то не увеличиваем номер, а добавляем _2
				return GetNewNameForFileAdd(oldPath);
		}
		else
			return GetNewNameForFileAdd(oldPath);
		return pathWithDigit;
    }

	TString GetNewNameForFileDigit(const TPath &oldPath, const TString &nameWithoutDigit, __int64 & counter, size_t & leadingZeros, const TPath &pathForRename)
    {
		TString pathWithDigit;
		const int BUFFER_SIZE = 65;
		char buffer[BUFFER_SIZE];
		TString leadingZeroString;

		counter++;
		if (leadingZeros > 0)
		{
			if (LengthOfLong(counter) > LengthOfLong(counter - 1)) //если цифра удленилась
               leadingZeros--;
			for (size_t i = 0; i < leadingZeros; i++)
				leadingZeroString.push_back('0');
		}

		if (_i64toa_s(counter, buffer, BUFFER_SIZE, 10) == 0)
		{
			pathWithDigit = CreatePath(oldPath.GetDirectory(), nameWithoutDigit + leadingZeroString + TString(buffer) + oldPath.GetExtension());
			if(TPath::EqualByNameWithExtension(pathWithDigit, pathForRename))
				return pathForRename.Original();
			if (IsFileExists(pathWithDigit.c_str())) //если такой файл уже есть, то не увеличиваем номер, а добавляем _2
				return GetNewNameForFileAdd(oldPath);
		}
		else
			return GetNewNameForFileAdd(oldPath);
		return pathWithDigit;
    }

	// Возврашает имя файла похожое на переданное.
	TString GetSimilarPath(const TPath &path)
	{
		__int64 digit;
		size_t leadingZeros = 0;
		TString nameWithoutDigit;

		digit = GetDigitInFileName(path, nameWithoutDigit, leadingZeros);
		if (digit == -1)
			return GetNewNameForFileAdd(path);
		else
			return GetNewNameForFileDigit(path,
									nameWithoutDigit,
									digit,
                                    leadingZeros);
	}

	// Возврашает имя файла похожое на переданное, не переименовывая существующий
	TString GetSimilarPath(const TPath &path, const TPath &pathForRename)
	{
		__int64 digit;
		size_t leadingZeros = 0;
		TString nameWithoutDigit;

		digit = GetDigitInFileName(path, nameWithoutDigit, leadingZeros);
		if (digit == -1)
			return GetNewNameForFileAdd(path, pathForRename);
		else
			return GetNewNameForFileDigit(path,
									nameWithoutDigit,
									digit,
                                    leadingZeros,
									pathForRename);
	}
}
