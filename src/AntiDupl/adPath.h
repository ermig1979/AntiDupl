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
#ifndef __adPath_h__
#define __adPath_h__

#include "adStrings.h"

namespace ad
{
	class TPath
	{
	private:
		typedef std::pair<TChar*, TChar*> TInterval;

		struct TParsedPath
		{
			TString string;
			TInterval prefix;
			TInterval directory;
			TInterval name;
			TInterval extension;

			void Parse();
			void UpdateFrom(const TParsedPath& path);
		};

		TParsedPath m_original;
		TParsedPath m_compare;
		bool m_enableSubFolder;

		void Update(const TPath& path, const adBool& enableSubFolder);
		void Update(const TString& path, const adBool& enableSubFolder);
		void Update(const TString& path);
		void Update(const TPath& path);

		static int Compare(const TChar* begin1, const TChar* end1, const TChar* begin2, const TChar* end2);
		static inline int Compare(const TInterval& int1, const TInterval& int2) 
		{
			return Compare(int1.first, int1.second, int2.first, int2.second);
		}
	public:
		TPath(const TString& path = TString()) 	{	Update(path);	}
		TPath(const TPath& path) { Update(path);}
		TPath(const TString& path, const adBool& enableSubFolder) 	{	Update(path, enableSubFolder);	}

		TPath& operator = (const TString& path) 
		{
			Update(path);
			return *this;
		}

		TPath& operator = (const TPath& path)
		{
			Update(path);
			return *this;
		}

		const TString& Original() const {return m_original.string;}

		const bool EnableSubFolder() const 
		{
			return m_enableSubFolder;
		}

		enum TIsSubPath
		{
			NONE = 0,
			FIRST = 1,
			SECOND = 2,
			EQUAL = 3 
		};
		static TIsSubPath IsSubPath(const TPath& path1, const TPath& path2);
		bool IsSubPath(const TPath& path) const {return IsSubPath(*this, path) == SECOND;}

		// Подсчет crc для пути.
		inline unsigned __int32 GetCrc32() const 
		{
			return SimdCrc32c(m_compare.string.c_str(), m_compare.string.length()*sizeof(TChar));
		}

		inline TString GetName(bool withExtension = true) const 
		{
			return TString(m_original.name.first, (withExtension ? m_original.extension.second : m_original.name.second));
		}

		inline TString GetDirectory() const 
		{
			return TString(m_original.prefix.first, m_original.directory.second);
		}
		
		inline TString GetExtension() const 
		{
			return TString(m_original.name.second);
		}

		//-----------------------------------------------------------------------------
		// Статические функции класса.

		static inline int CompareByPath(const TPath& path1, const TPath& path2) 
		{
			return Compare(path1.m_compare.directory.first, path1.m_compare.extension.second, 
				path2.m_compare.directory.first, path2.m_compare.extension.second);
		}

		static inline int CompareByDirectory(const TPath& path1, const TPath& path2) 
		{
			return Compare(path1.m_compare.directory, path2.m_compare.directory);
		}

		static inline int CompareByName(const TPath& path1, const TPath& path2) 
		{
			return Compare(path1.m_compare.name, path2.m_compare.name);
		}

		static inline int CompareByNameWithExtension(const TPath& path1, const TPath& path2) 
		{
			return Compare(path1.m_compare.name.first, path1.m_compare.extension.second, 
				path2.m_compare.name.first, path2.m_compare.extension.second);
		}

		static inline int CompareByExtension(const TPath& path1, const TPath& path2) 
		{
			return Compare(path1.m_compare.extension, path2.m_compare.extension);
		}

		static inline bool LesserByPath(const TPath& path1, const TPath& path2) {return CompareByPath(path1, path2) < 0;}
		static inline bool LesserByDirectory(const TPath& path1, const TPath& path2) {return CompareByDirectory(path1, path2) < 0;}
		static inline bool LesserByName(const TPath& path1, const TPath& path2) {return CompareByName(path1, path2) < 0;}
		static inline bool LesserByNameWithExtension(const TPath& path1, const TPath& path2) {return CompareByNameWithExtension(path1, path2) < 0;}
		static inline bool LesserByExtension(const TPath& path1, const TPath& path2) {return CompareByPath(path1, path2) < 0;}

		static inline bool EqualByPath(const TPath& path1, const TPath& path2) {return CompareByPath(path1, path2) == 0;}
		static inline bool EqualByDirectory(const TPath& path1, const TPath& path2) {return CompareByDirectory(path1, path2) == 0;}
		static inline bool EqualByName(const TPath& path1, const TPath& path2) {return CompareByName(path1, path2) == 0;}
		static inline bool EqualByNameWithExtension(const TPath& path1, const TPath& path2) {return CompareByNameWithExtension(path1, path2) == 0;}
		static inline bool EqualByExtension(const TPath& path1, const TPath& path2) {return CompareByExtension(path1, path2) == 0;}

		static inline bool BiggerByPath(const TPath& path1, const TPath& path2) {return CompareByPath(path1, path2) > 0;}
		static inline bool BiggerByDirectory(const TPath& path1, const TPath& path2) {return CompareByDirectory(path1, path2) > 0;}
		static inline bool BiggerByName(const TPath& path1, const TPath& path2) {return CompareByName(path1, path2) > 0;}
		static inline bool BiggerByNameWithExtension(const TPath& path1, const TPath& path2) {return CompareByNameWithExtension(path1, path2) > 0;}
		static inline bool BiggerByExtension(const TPath& path1, const TPath& path2) {return CompareByPath(path1, path2) > 0;}

		static bool Valid(size_t size);
	};

    //-------------------------------------------------------------------------

    class TPathContainer
    {
        typedef std::vector<TPath> TPathVector;

    public:
        typedef TPathVector::iterator iterator; 
        typedef TPathVector::const_iterator const_iterator; 

        TPathContainer(const TChar *sectionName, bool defaultPathEnabled = false);
        ~TPathContainer();

        void Load(const TChar *fileName);
        bool Save(const TChar *fileName) const;

        adError Import(adPathPtrA pPath, adSize pathCount);
        adError Import(adPathPtrW pPath, adSize pathCount);
		adError Import(adPathWSFPtr pPath, adSize pathCount);
        adError Export(adPathPtrA pPath, adSizePtr pPathCount) const;
        adError Export(adPathPtrW pPath, adSizePtr pPathCount) const;

        size_t IsHasPath(const TPath& path) const;
        size_t IsHasSubPath(const TPath& subPath) const;

        size_t Size() const {return m_paths.size();}
        TPath& operator [] (size_t i) {return m_paths[i];} 
        const TPath& operator [] (size_t i) const {return m_paths[i];}  

    private:
        void Set(const TPathVector& tmp);

        bool m_defaultPathEnabled;
        TString m_sectionName;
        TPathVector m_paths;
    };
    //-------------------------------------------------------------------------
}

#endif/*__adPath_h__*/
