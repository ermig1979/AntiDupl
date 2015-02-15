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
#ifndef __adStrings_h__
#define __adStrings_h__

#include "adConfig.h"

namespace ad
{
	//-------------------------------------------------------------------------
	bool AnsiToUnicode(wchar_t* unicode, const char* ansi, size_t count);
	bool UnicodeToAnsi(char* ansi, const wchar_t* unicode, size_t count);
	//-------------------------------------------------------------------------
	class TString : public std::basic_string<TChar> 
	{
	public:
		TString() :std::basic_string<TChar>() {};
		TString(const TString &str) :std::basic_string<TChar>(str.c_str(), str.size()) {};

		TString(const char *str, size_t size);
		TString(const wchar_t *wstr, size_t size); 
		TString(const char *first, const char *last);
		TString(const wchar_t *first, const wchar_t *last); 
		TString(const char *str);
		TString(const wchar_t *wstr); 
		TString(const std::string &str);
		TString(const std::wstring &wstr);
		
		void TString::Trim();

		std::string ToString() const; // Convert TString to ANSI string 
		std::wstring ToWString() const; // Convert TString to UNICODE string

		bool CopyTo(char *buffer, size_t size) const;
		bool CopyTo(wchar_t *buffer, size_t size) const;

		bool ToUpper(); // Convert string characters to upper case;
		TString GetUpper(); // Get string with characters converted to upper case;
	};

	typedef std::vector<TString> TStrings;
	//-------------------------------------------------------------------------
}

#endif//__adStrings_h__
