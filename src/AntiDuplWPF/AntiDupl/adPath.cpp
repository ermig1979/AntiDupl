/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2014 Borisov Dmitry.
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
#include <algorithm>
#include <iostream>

#include "adFileUtils.h"
#include "adIniFile.h"
#include "adPath.h"

namespace ad
{
	void TPath::TParsedPath::Parse()
	{
		prefix.first = (TChar*)string.c_str();
		prefix.second = prefix.first;
		extension.second = prefix.first + string.length();
#ifdef UNICODE
		for(; prefix.second - prefix.first < EXTENDED_PATH_PREFIX_SIZE && prefix.second < extension.second; ++prefix.second)
		{
			if(*prefix.second != EXTENDED_PATH_PREFIX[prefix.second - prefix.first])
			{
				prefix.second = prefix.first;
				break;
			}
		}
#endif
		directory.first = prefix.second;
		directory.second = directory.first;

		name.second = extension.second;
		for(extension.first = extension.second; extension.first > directory.first; --extension.first)
		{
			if(*extension.first == TEXT('.'))
			{
				name.second = extension.first;
				++extension.first;
				break;
			}
		}
		for(name.first = extension.second; name.first > directory.first; --name.first)
		{
			if(*name.first == DELIMETER)
			{
				directory.second = name.first;
				++name.first;
				break;
			}
		}
		if(name.first >= name.second)
			name.second = extension.second;
		if(extension.first == directory.first || extension.first <= name.second)
			extension.first = extension.second;
	}

	void TPath::TParsedPath::UpdateFrom(const TParsedPath& path)
	{
		ptrdiff_t difference = string.c_str() - path.string.c_str();

		prefix.first = path.prefix.first + difference;
		prefix.second = path.prefix.second + difference;

		directory.first = path.directory.first + difference;
		directory.second = path.directory.second + difference;

		name.first = path.name.first + difference;
		name.second = path.name.second + difference;

		extension.first = path.extension.first + difference;
		extension.second = path.extension.second + difference;
	}

	void TPath::Update(const TPath& path, const adBool& enableSubFolder)
	{
		m_original.string = path.m_original.string;;
		m_original.UpdateFrom(path.m_original);

		m_compare.string = path.m_compare.string;
		m_compare.UpdateFrom(path.m_compare);

		m_enableSubFolder = enableSubFolder ? TRUE : FALSE;
	}

	void TPath::Update(const TString& path, const adBool& enableSubFolder)
	{
		m_original.string = path;
		m_original.Parse();

		m_compare.string = m_original.string.GetUpper();
		m_compare.UpdateFrom(m_original);

		m_enableSubFolder = enableSubFolder ? TRUE : FALSE;		
	}

	void TPath::Update(const TString& path)
	{
		m_original.string = path;
		m_original.Parse();
		m_compare.string = m_original.string.GetUpper();
		m_compare.UpdateFrom(m_original);
	}

	void TPath::Update(const TPath& path)
	{
		m_original.string = path.m_original.string;
		m_original.UpdateFrom(path.m_original);

		m_compare.string = path.m_compare.string;
		m_compare.UpdateFrom(path.m_compare);

		m_enableSubFolder = path.EnableSubFolder();
	}

	void TPath::Update(const Location& location)
	{
		m_original.string = location.Path;
		m_enableSubFolder = location.IsDirectory;
		m_original.Parse();
		m_compare.string = m_original.string.GetUpper();
		m_compare.UpdateFrom(m_original);
	}

	int TPath::Compare(const TChar* begin1, const TChar* end1, const TChar* begin2, const TChar* end2)
	{
		for(;begin1 != end1 && begin2 != end2; ++begin1, ++begin2)
		{
			if(*begin1 < *begin2)
				return -1;
			if(*begin1 > *begin2)
				return 1;
		}
		if(begin1 == end1 && begin2 != end2)
			return -1;
		if(begin2 == end2 && begin1 != end1)
			return 1;
		return 0;
	}

	TPath::TIsSubPath TPath::IsSubPath(const TPath& path1, const TPath& path2)
	{
		const TChar *p1 = path1.m_compare.directory.first;
		const TChar *p2 = path2.m_compare.directory.first;
		while(*p1 != 0 && *p2 != 0 && *p1 == *p2) //если символы равны удаляем их
		{
			p1++;
			p2++;
		}
		if(*p1 == 0 && *p2 == 0)
			return EQUAL; 
		if(*p1 == 0 && *p2 == DELIMETER && path1.m_enableSubFolder == true) //если второй начинается со слеша и включены субдиректории
			return SECOND; //второй путь входит в первый
		if(*p2 == 0 && *p1 == DELIMETER && path2.m_enableSubFolder == true)
			return FIRST;  //первая входит во вторую
		return NONE;
	}

	bool TPath::Valid(size_t size)
	{
#ifdef UNICODE
		return size < MAX_PATH_EX;
#else//UNICODE
		return size < MAX_PATH;
#endif//UNICODE
	}

    //----------------Key names:-----------------------------------------------

    const TChar KEY_pathCount[] = TEXT("PathCount");
    const TChar KEY_pathTemplate[] = TEXT("Path%d");

    //-------------------------------------------------------------------------

    TPathContainer::TPathContainer(bool defaultPathEnabled)
        : m_defaultPathEnabled(defaultPathEnabled)
    {
        if(m_defaultPathEnabled)
            m_paths.push_back(GetApplicationDirectory());
    }

    TPathContainer::~TPathContainer()
    {
    }

	adError TPathContainer::AddPath(const Location * location)
    {
		TPath path(*location);
		m_paths.push_back(path);
		return AD_OK;
    }

	adError TPathContainer::Clear()
	{
		m_paths.clear();
		return AD_OK;
	}

	//проверяет содержится ли в наших путях m_paths переданный path
    size_t TPathContainer::IsHasPath(const TPath& path) const
    {
        const_iterator i = std::lower_bound(m_paths.begin(), m_paths.end(), path, TPath::LesserByPath);
        if((i != m_paths.end()) && TPath::EqualByPath(*i, path))
        {
            return i - m_paths.begin();
        }
        return AD_IS_NOT_EXIST;
    }

    size_t TPathContainer::IsHasSubPath(const TPath& subPath) const
    {
        if(m_paths.size() == 0)
            return AD_IS_NOT_EXIST;
        const_iterator i = std::lower_bound(m_paths.begin(), m_paths.end(), subPath, TPath::LesserByPath);
        if(i != m_paths.begin())
            i--;
        if(i != m_paths.end())
        {
            if(i->IsSubPath(subPath))
            {
                return i - m_paths.begin();
            }
        }
        return AD_IS_NOT_EXIST;
    }

	//Проверка на вхождение путей друг в друга
    void TPathContainer::Set(const TPathContainer::TPathVector& tmp)
    {
        m_paths.clear();
        m_paths.reserve(tmp.size());
        for(const_iterator i_tmp = tmp.begin(); i_tmp != tmp.end(); i_tmp++)
        {
            TPath::TIsSubPath isSubPath = TPath::NONE;
            iterator i_main;
            for(i_main = m_paths.begin(); i_main != m_paths.end(); i_main++)
            {
                isSubPath = TPath::IsSubPath(*i_tmp, *i_main);
                if(isSubPath != TPath::NONE)
                    break;
            }
            if(IsFileExists(i_tmp->Original().c_str()))
            {
                switch(isSubPath)
                {
                case TPath::NONE:
                    m_paths.push_back(*i_tmp);
                    break;
                case TPath::FIRST: //уже есть в списке, не добавляем
                    break;
                case TPath::SECOND: //добавляем директорию которая включает ту что уже в списке, оставляем эту, ту стираем
                    *i_main = i_tmp->Original();
                    break;
                case TPath::EQUAL:
                    break;
                }
            }
        }
        if(m_defaultPathEnabled && m_paths.size() == 0)
            m_paths.push_back(GetApplicationDirectory());
        if(m_paths.size() > 0)
            std::sort(m_paths.begin(), m_paths .end(), TPath::LesserByPath);
    }
    //-------------------------------------------------------------------------
}
