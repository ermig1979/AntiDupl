/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar, 2013-2018 Borisov Dmitry.
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
#include "adStrings.h"
#include "adIO.h"

namespace
{
	UINT GetCurrentCodePage()
	{
		TCHAR codePage[6];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE,
			(LPTSTR)&codePage, 6);
		return _ttoi(codePage);
	}

	//-------------------------------------------------------------------------

	UINT g_currentCodePage = GetCurrentCodePage();
}

namespace ad
{
	//-------------------------------------------------------------------------
	bool AnsiToUnicode(wchar_t* unicode, const char* ansi, size_t count)
	{
		return (MultiByteToWideChar(g_currentCodePage, MB_PRECOMPOSED, 
			ansi, (int)count, unicode, (int)count) > 0);
	}

	bool UnicodeToAnsi(char* ansi, const wchar_t* unicode, size_t count)
	{
		return (WideCharToMultiByte(g_currentCodePage, WC_COMPOSITECHECK,
			unicode, (int)count, ansi, (int)count, NULL, NULL) > 0);
	}
	//-------------------------------------------------------------------------
#ifdef UNICODE
	// Создает TString из char * и размера
	TString::TString(const char *str, size_t size)
	{
		this->resize(size);
		if(!AnsiToUnicode((wchar_t*)this->c_str(), str, size + 1))
			this->clear();
	}

	TString::TString(const wchar_t *wstr, size_t size)
		:std::basic_string<TChar>(wstr, size)
	{
	}

	// Создает TString из фрагмента char * от first до last
	TString::TString(const char *first, const char *last)
	{
		size_t size = last - first;
		this->resize(size);
		if(!AnsiToUnicode((wchar_t*)this->c_str(), first, size + 1))
			this->clear();
	}

	TString::TString(const wchar_t *first, const wchar_t *last)
		:std::basic_string<TChar>(first, last)
	{
	}

	// Создает TString из char * и размера
	TString::TString(const char *str)
	{
		size_t size = strlen(str);
		this->resize(size);
		if(!AnsiToUnicode((wchar_t*)this->c_str(), str, size + 1))
			this->clear();
	}

	TString::TString(const wchar_t *wstr)
		:std::basic_string<TChar>(wstr)
	{
	}

	TString::TString(const std::string &str)
	{
		this->resize(str.size());
		if(!AnsiToUnicode((wchar_t*)this->c_str(), str.c_str(), str.size() + 1))
			this->clear();
	}

	TString::TString(const std::wstring &wstr)
		:std::basic_string<TChar>(wstr)
	{
	}

	std::string TString::ToString() const
	{
		std::string result(this->size(), 0);
		if(!UnicodeToAnsi((char*)result.c_str(), this->c_str(), this->size() + 1))
			result.clear();
		return result;
	}

	// Обрезает начальные и концевые пробелы.
	void TString::Trim()
	{
		if (this->length() > 0)
		{
			// Находим начало без пробелов.
			TString::const_iterator it = this->begin();
			while (it != this->end() && iswspace(*it))
				it++;

			it = this->erase(this->begin(), it);

			// Находим конец без пробелов.
			TString::const_reverse_iterator rit = this->rbegin();
			while (rit.base() != it && iswspace(*rit))
				rit++;

			this->erase(rit.base(), this->end());
		}
	}

	std::wstring TString::ToWString() const
	{
		return TString(*this);
	}

	bool TString::CopyTo(char *buffer, size_t size) const
	{
		if(length() >= size)
			return false;

		if(UnicodeToAnsi((char*)buffer, c_str(), length()))
		{
			buffer[length()] = 0;
			return true;
		}
		return false;
	}

	// Копировать в буфер. Передается буфер и размер буфера.
	bool TString::CopyTo(wchar_t *buffer, size_t size) const
	{
		if(length() >= size)
			return false;

		memcpy(buffer, c_str(), sizeof(wchar_t)*length());
		buffer[length()] = 0;
		return true;
	}
#else//UNICODE
	TString::TString(const char *str, size_t size)
		:std::basic_string<TChar>(str, size)
	{
	}

	TString::TString(const wchar_t *wstr, size_t size)
	{
		this->resize(size);
		if(!UnicodeToAnsi((char*)this->c_str(), wstr, size + 1))
			this->clear();
	}

	TString::TString(const char *first, const char *last)
		:std::basic_string<TChar>(first, last)
	{
	}

	TString::TString(const wchar_t *first, const wchar_t *last)
	{
		size_t size = last - first;
		this->resize(size);
		if(!UnicodeToAnsi((char*)this->c_str(), first, size + 1))
			this->clear();
	}

	TString::TString(const char *str)
		:std::basic_string<TChar>(str)
	{
	}

	TString::TString(const wchar_t *wstr)
	{
		size_t size = wcslen(wstr);
		this->resize(size);
		if(!UnicodeToAnsi((char*)this->c_str(), wstr, size + 1))
			this->clear();
	}

	TString::TString(const std::string &str)
		:std::basic_string<TChar>(str)
	{
	}


	TString::TString(const std::wstring &wstr)
	{
		this->resize(wstr.size());
		if(!UnicodeToAnsi((char*)this->c_str(), wstr.c_str(), wstr.size() + 1))
			this->clear();
	}

	std::string TString::ToString() const
	{
		return std::string(*this);
	}

	std::wstring TString::ToWString() const
	{
		std::wstring result(this->size(), 0);
		if(!AnsiToUnicode((wchar_t*)result.c_str(), this->c_str(), this->size() + 1))
			result.clear();
		return result;
	}

	bool TString::CopyTo(char *buffer, size_t size) const
	{
		if(length() >= size)
			return false;

		memcpy(buffer, c_str(), sizeof(char)*length());
		buffer[length()] = 0;
		return true;
	}

	bool TString::CopyTo(wchar_t *buffer, size_t size) const
	{
		if(length() >= size)
			return false;

		if(AnsiToUnicode(buffer, c_str(), length()))
		{
			buffer[length()] = 0;
			return true;
		}
		return false;
	}
#endif//UNICODE

	bool TString::ToUpper()
	{
		return _tcsupr_s((TChar*)this->c_str(), this->size() + 1) == 0;
	}

	TString TString::GetUpper()
	{
		TString buffer(*this);
		buffer.ToUpper();
		return buffer;
	}
	//-------------------------------------------------------------------------
}