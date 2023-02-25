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
#ifndef __adImageGroup_h__
#define __adImageGroup_h__

#include "adConfig.h"

namespace ad
{
	class TStatus;

	struct TResult;
	struct TImageInfo;
	struct TOptions;

	typedef TResult * TResultPtr;
	typedef std::vector<TResultPtr> TResultPtrVector;
	typedef std::list<TResultPtr> TResultPtrList;

	typedef TImageInfo * TImageInfoPtr;
	typedef std::vector<TImageInfoPtr> TImageInfoPtrVector;
	// Контейнер указателей на информацию об изображение.
	typedef std::set<TImageInfoPtr> TImageInfoPtrSet;
	//-------------------------------------------------------------------------
	struct TImageGroup
	{
		TSize id;

		// Список изображений входящих в группу.
		TImageInfoPtrVector images;
		// Список результатов
		TResultPtrList results;

		bool invalidHint;

		TImageGroup(size_t id_ = AD_UNKNOWN_ID);
		TImageGroup(const TImageGroup & imageGroup);

		void UpdateImages();

		bool Export(adGroupPtr pGroup) const;
	};
	typedef TImageGroup* TImageGroupPtr;
	//-------------------------------------------------------------------------
	class TImageGroupStorage
	{
	public:
		TImageGroupStorage();
		~TImageGroupStorage();

		TImageGroupPtr Get(size_t id, bool create = true);

		void Assign(const TImageGroupStorage & storage);
		void Set(TResultPtrVector & results, TStatus * pStatus);
		void Update(TResultPtrVector & results);
		void UpdateHints(TOptions *pOptions, bool force, TStatus *pStatus);

		adError Export(adSizePtr pStartFrom, adGroupPtr pGroup, adSizePtr pGroupSize) const;
		adError Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrA pImageInfo, adSizePtr pImageInfoSize) const;
		adError Export(adSize groupId, adSizePtr pStartFrom, adImageInfoPtrW pImageInfo, adSizePtr pImageInfoSize) const;

		adError SetSelection(adSize groupId, adSize index, adSelectionType selectionType);
		adError GetSelection(adSize groupId, adSizePtr pStartFrom, adBoolPtr pSelection, adSizePtr pSelectionSize) const;

	private:
		void Erase(size_t id);
		void Clear();
		void UpdateVector();
		void SetGroupSize(TResultPtrVector & results);

		typedef std::map<size_t, TImageGroupPtr> TMap;
		typedef std::vector<TImageGroupPtr> TVector;
		
		// Хранилище групп в виде структур в карте. (ID, Группа)
		TMap m_map;
		// Хранилище групп в виде структур в векторе.
		TVector m_vector;
	};
}
#endif/*__adImageGroup_h__*/