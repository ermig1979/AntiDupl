/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar.
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
#include "adIniFile.h"

namespace ad
{
    TIniFile::TIniFile(const TChar* fileName, const TChar* section)
        :m_fileName(fileName),
        m_section(section)
    {
    }

    void TIniFile::SetSection(const TChar* section)
    {
        m_section = section;
    }

    bool TIniFile::ClearSection(const TChar* section)
    {
        m_section = section;
        return (TRUE == WritePrivateProfileString(m_section.c_str(), NULL, 
            NULL, m_fileName.c_str()));
    }

    bool TIniFile::WriteString(const TChar* section, const TChar*  key, const TChar*  value)
    {
        m_section = section;
        return (TRUE == WritePrivateProfileString(m_section.c_str(), key, 
            value, m_fileName.c_str()));
    }

    bool TIniFile::WriteInteger(const TChar* section, const TChar* key, int value)
    {
        TChar buffer[MAX_PATH];
        m_section = section;
        _stprintf_s(buffer, MAX_PATH, TEXT("%d"), value);
        return (TRUE == WritePrivateProfileString(m_section.c_str(), key, 
            buffer, m_fileName.c_str()));
    }

    bool TIniFile::WriteFloat(const TChar* section, const TChar* key, double value)
    {
        TChar buffer[MAX_PATH];
        m_section = section;
        _stprintf_s(buffer, MAX_PATH, TEXT("%g"), value);
        return (TRUE == WritePrivateProfileString(m_section.c_str(), key, 
            buffer, m_fileName.c_str()));
    }

    bool TIniFile::WriteBool(const TChar* section, const TChar* key, bool value)
    {
        TChar buffer[MAX_PATH];
        m_section = section;
        _stprintf_s(buffer, MAX_PATH, TEXT("%d"), (int)value);
        return (TRUE == WritePrivateProfileString(m_section.c_str(), key, 
            buffer, m_fileName.c_str()));
    }

    TString TIniFile::ReadString(const TChar* section, const TChar* key, const TChar* defaultValue)
    {
        m_section = section;
#ifdef UNICODE
        TChar buffer[MAX_PATH_EX];
        GetPrivateProfileString(m_section.c_str(), key, defaultValue, 
            buffer, MAX_PATH_EX, m_fileName.c_str());
#else//UNICODE
        TChar buffer[MAX_PATH];
        GetPrivateProfileString(m_section.c_str(), key, defaultValue, 
            buffer, MAX_PATH, m_fileName.c_str());
#endif//UNICODE
        return buffer;
    }

    int TIniFile::ReadInteger(const TChar* section, const TChar* key, int defaultValue)
    {
        m_section = section;
        return GetPrivateProfileInt(m_section.c_str(), key, 
            defaultValue, m_fileName.c_str());
    }

    double TIniFile::ReadFloat(const TChar* section, const TChar* key, double defaultValue)
    {
        m_section = section;
        TChar buffer[MAX_PATH], defaultBuffer[MAX_PATH];
        double result;
        _stprintf_s(defaultBuffer, MAX_PATH, TEXT("%g"), defaultValue);
        GetPrivateProfileString(m_section.c_str(), key, defaultBuffer, 
            buffer, MAX_PATH, m_fileName.c_str());
        if(_stscanf_s(buffer, TEXT("%g"), &result) != 1)
            result = defaultValue;
        return result;
    }

    bool TIniFile::ReadBool(const TChar* section, const TChar* key, bool defaultValue)
    {
        m_section = section;
        return (GetPrivateProfileInt(m_section.c_str(), key, 
            (int)defaultValue, m_fileName.c_str()) == TRUE);
    }
}