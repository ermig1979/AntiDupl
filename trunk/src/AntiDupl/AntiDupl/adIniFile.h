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
#ifndef __adIniFile_h__
#define __adIniFile_h__

#include "adStrings.h"

namespace ad
{
    class TIniFile 
    {
    public:
        TIniFile(const TChar* fileName, const TChar* section = TEXT("default"));

        void SetSection(const TChar* section);
        bool ClearSection(const TChar* section);

        bool WriteString(const TChar* section, const TChar*  key, const TChar*  value);
        inline bool WriteString(const TChar* key, const TChar*  value) {return WriteString(m_section.c_str(), key, value);}

        bool WriteInteger(const TChar* section, const TChar* key, int value);
        inline bool WriteInteger(const TChar* key, int value) {return WriteInteger(m_section.c_str(), key, value);}

        bool WriteFloat(const TChar* section, const TChar* key, double value);
        inline bool WriteFloat(const TChar* key, double value) {return WriteFloat(m_section.c_str(), key, value);}

        bool WriteBool(const TChar* section, const TChar* key, bool value);
        inline bool WriteBool(const TChar* key, bool value) {return WriteBool(m_section.c_str(), key, value);}

        TString ReadString(const TChar* section, const TChar* key, const TChar* defaultValue);
        inline TString ReadString(const TChar* key, const TChar* defaultValue) {return ReadString(m_section.c_str(), key, defaultValue);}

        int ReadInteger(const TChar* section, const TChar* key, int defaultValue);
        inline int ReadInteger(const TChar* key, int defaultValue) {return ReadInteger(m_section.c_str(), key, defaultValue);}

        double ReadFloat(const TChar* section, const TChar* key, double defaultValue);
        inline double ReadFloat(const TChar* key, double defaultValue) {return ReadFloat(m_section.c_str(), key, defaultValue);}

        bool ReadBool(const TChar* section, const TChar* key, bool defaultValue);
        inline bool ReadBool(const TChar* key, bool defaultValue) {return ReadBool(m_section.c_str(), key, defaultValue);}

    private:
        TString m_fileName;
        TString m_section;
    };
}

#endif/*__adIniFile_h__*/