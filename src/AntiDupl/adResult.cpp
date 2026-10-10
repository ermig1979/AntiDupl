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

#include "adResult.h"

namespace ad
{
    TResult::TResult()
        :id(-1),
        selected(false),
        current(false),
        type(AD_RESULT_NONE),
        first(NULL),
        second(NULL),
        defect(AD_DEFECT_NONE),
        difference(0),
        transform(AD_TRANSFORM_TURN_0),
        group(AD_UNDEFINED),
		groupSize(AD_UNDEFINED),
        hint(AD_HINT_NONE),
		deleteByHint(false)
    {
    }

    TResult::TResult(const TResult& result)
        :id(result.id),
        selected(result.selected),
        current(result.current),
        type(result.type),
        first(result.first),
        second(result.second),
        defect(result.defect),
        difference(result.difference),
        transform(result.transform),
        group(result.group),
		groupSize(result.groupSize),
        hint(result.hint),
		deleteByHint(result.deleteByHint)
    {
    }

    int TResult::ImageInfoCompare(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType)
    {
        switch(sortType)
        {
        case AD_SORT_BY_SORTED_PATH:
            return TPath::NaturalCompareByPath(pFirst->path, pSecond->path);
        case AD_SORT_BY_SORTED_NAME:
            return TPath::NaturalCompareByNameWithExtension(pFirst->path, pSecond->path);
        case AD_SORT_BY_SORTED_DIRECTORY:
            return TPath::NaturalCompareByDirectory(pFirst->path, pSecond->path);
        case AD_SORT_BY_SORTED_SIZE:
            return CompareValues(pFirst->size, pSecond->size);
        case AD_SORT_BY_SORTED_TIME:
            return CompareValues(pFirst->time, pSecond->time);
        case AD_SORT_BY_SORTED_TYPE:
            return CompareValues(pFirst->type, pSecond->type);
        case AD_SORT_BY_SORTED_WIDTH:
            return CompareValues(pFirst->width, pSecond->width);
        case AD_SORT_BY_SORTED_HEIGHT:
            return CompareValues(pFirst->height, pSecond->height);
        case AD_SORT_BY_SORTED_AREA:
            return CompareValues((TUInt64)pFirst->width*pFirst->height, (TUInt64)pSecond->width*pSecond->height);
        case AD_SORT_BY_SORTED_RATIO:
            return CompareValues((TUInt64)pFirst->width*pSecond->height, (TUInt64)pSecond->width*pFirst->height);
        case AD_SORT_BY_SORTED_BLOCKINESS:
            return CompareValues(pFirst->blockiness, pSecond->blockiness);
        case AD_SORT_BY_SORTED_BLURRING:
            return CompareValues(pFirst->blurring, pSecond->blurring);
        }
        return 0;
    }

    bool TResult::ImageInfoLesser(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType, bool increasing)
    {
        int result = ImageInfoCompare(pFirst, pSecond, sortType);
        return increasing ? result < 0 : result > 0;
    }

    void TResult::OrientByPath()
    {
        if(type == AD_RESULT_DUPL_IMAGE_PAIR && TPath::NaturalCompareByPath(second->path, first->path) < 0)
            Swap();
    }

    int TResult::SortedImage(TSortType sortType)
    {
        if(sortType >= AD_SORT_BY_SORTED_PATH && sortType < AD_SORT_BY_SECOND_PATH)
            return 1;
        if(sortType >= AD_SORT_BY_SECOND_PATH && sortType < AD_SORT_BY_DEFECT)
            return 2;
        return 0;
    }

    TSortType TResult::ImageSortType(TSortType sortType)
    {
        if(sortType >= AD_SORT_BY_SECOND_PATH)
            return adSortType(sortType + AD_SORT_BY_SORTED_PATH - AD_SORT_BY_SECOND_PATH);
        if(sortType >= AD_SORT_BY_FIRST_PATH)
            return adSortType(sortType + AD_SORT_BY_SORTED_PATH - AD_SORT_BY_FIRST_PATH);
        return sortType;
    }

    void TResult::OrientFor(TSortType sortType, bool increasing)
    {
        int image = SortedImage(sortType);
        if(type != AD_RESULT_DUPL_IMAGE_PAIR || image == 0)
            return;
        // Negative: the second image comes first in the sort's order.
        int order = ImageInfoCompare(second, first, ImageSortType(sortType));
        if(order == 0)
            order = TPath::NaturalCompareByPath(second->path, first->path);
        if(!increasing)
            order = -order;
        if((image == 1 && order < 0) || (image == 2 && order > 0))
            Swap();
    }

    void TResult::Swap()
    {
        if(type != AD_RESULT_DUPL_IMAGE_PAIR)
            return;

        std::swap(first, second);

        switch(transform)
        {
        case AD_TRANSFORM_TURN_90:
            transform = AD_TRANSFORM_TURN_270;
            break;
        case AD_TRANSFORM_TURN_270:
            transform = AD_TRANSFORM_TURN_90;
            break;
        case AD_TRANSFORM_MIRROR_TURN_90:
            transform = AD_TRANSFORM_MIRROR_TURN_270;
            break;
        case AD_TRANSFORM_MIRROR_TURN_270:
            transform = AD_TRANSFORM_MIRROR_TURN_90;
            break;
        }
        switch(hint)
        {
        case AD_HINT_DELETE_FIRST:
            hint = AD_HINT_DELETE_SECOND;
            break;
        case AD_HINT_DELETE_SECOND:
            hint = AD_HINT_DELETE_FIRST;
            break;
        case AD_HINT_RENAME_FIRST_TO_SECOND:
            hint = AD_HINT_RENAME_SECOND_TO_FIRST;
            break;
        case AD_HINT_RENAME_SECOND_TO_FIRST:
            hint = AD_HINT_RENAME_FIRST_TO_SECOND;
            break;
        }
    }

    bool TResult::Export(adResultPtrA pResult) const
    {
        if(pResult == NULL)
            return false;

        bool result = true;

        pResult->type = type;
        result = result && first->Export(&(pResult->first));
        result = result && second->Export(&(pResult->second));
        pResult->defect = defect;
        pResult->difference = difference;
        pResult->transform = transform;
		pResult->group = group;
		pResult->groupSize = groupSize;
        pResult->hint = hint;

        return result;
    }

    bool TResult::Export(adResultPtrW pResult) const
    {
        if(pResult == NULL)
            return false;

        bool result = true;

        pResult->type = type;
        result = result && first->Export(&(pResult->first));
        result = result && second->Export(&(pResult->second));
        pResult->defect = defect;
        pResult->difference = difference;
        pResult->transform = transform;
        pResult->group = group;
		pResult->groupSize = groupSize;
        pResult->hint = hint;

        return result;
    }

	//-------------------------------------------------------------------------

    TResultPtrLesser::TResultPtrLesser(TSortType sortType, bool increasing) 
        :m_sortType(sortType),
        m_increasing(increasing)
    {
    }

    int TResultPtrLesser::Compare(TResultPtr pFirst, TResultPtr pSecond) const
    {
        switch(TResult::SortedImage(m_sortType))
        {
        case 1:
            return TResult::ImageInfoCompare(pFirst->first, pSecond->first, TResult::ImageSortType(m_sortType));
        case 2:
            return TResult::ImageInfoCompare(pFirst->second, pSecond->second, TResult::ImageSortType(m_sortType));
        }
        switch(m_sortType)
        {
        case AD_SORT_BY_TYPE:
            return CompareValues(pFirst->type, pSecond->type);
        case AD_SORT_BY_DEFECT:
            return CompareValues(pFirst->defect, pSecond->defect);
        case AD_SORT_BY_DIFFERENCE:
            return CompareValues(pFirst->difference, pSecond->difference);
        case AD_SORT_BY_TRANSFORM:
            return CompareValues(pFirst->transform, pSecond->transform);
        case AD_SORT_BY_GROUP:
            return CompareValues(pFirst->group, pSecond->group);
        case AD_SORT_BY_GROUP_SIZE:
            return CompareValues(pFirst->groupSize, pSecond->groupSize);
        case AD_SORT_BY_HINT:
            return CompareValues(pFirst->hint, pSecond->hint);
        }
        return 0;
    }

    // The same property of the pair's other image, for sorts by one image's
    // property: sorting by the first image's directory then orders the rows
    // of each directory by the second image's directory.
    int TResultPtrLesser::CompareOtherImage(TResultPtr pFirst, TResultPtr pSecond) const
    {
        switch(TResult::SortedImage(m_sortType))
        {
        case 1:
            return TResult::ImageInfoCompare(pFirst->second, pSecond->second, TResult::ImageSortType(m_sortType));
        case 2:
            return TResult::ImageInfoCompare(pFirst->first, pSecond->first, TResult::ImageSortType(m_sortType));
        }
        return 0;
    }

    // The paths of the sorted image, then of the other image.
    int TResultPtrLesser::CompareImagePaths(TResultPtr pFirst, TResultPtr pSecond) const
    {
        int result = 0;
        switch(TResult::SortedImage(m_sortType))
        {
        case 1:
            result = TPath::NaturalCompareByPath(pFirst->first->path, pSecond->first->path);
            if(result == 0)
                result = TPath::NaturalCompareByPath(pFirst->second->path, pSecond->second->path);
            break;
        case 2:
            result = TPath::NaturalCompareByPath(pFirst->second->path, pSecond->second->path);
            if(result == 0)
                result = TPath::NaturalCompareByPath(pFirst->first->path, pSecond->first->path);
            break;
        }
        return result;
    }

    // A sort by one image's property orders by that property, the other
    // image's same property, then the sorted image's path and the other
    // image's path, all in the sort's direction - like a sort of single
    // images by that property, then by path. Whatever is still equal goes in
    // a fixed ascending order: type, first path, second path, difference,
    // then id, which is unique and makes the order total, so the same sort
    // always gives the same rows whatever the previous order was.
    bool TResultPtrLesser::operator() (TResultPtr pFirst, TResultPtr pSecond)
    {
        int result = Compare(pFirst, pSecond);
        if(result == 0)
            result = CompareOtherImage(pFirst, pSecond);
        if(result == 0)
            result = CompareImagePaths(pFirst, pSecond);
        if(result != 0)
            return m_increasing ? result < 0 : result > 0;

        result = CompareValues(pFirst->type, pSecond->type);
        if(result == 0)
            result = TPath::NaturalCompareByPath(pFirst->first->path, pSecond->first->path);
        if(result == 0)
            result = TPath::NaturalCompareByPath(pFirst->second->path, pSecond->second->path);
        if(result == 0)
            result = CompareValues(pFirst->difference, pSecond->difference);
        if(result == 0)
            result = CompareValues(pFirst->id, pSecond->id);
        return result < 0;
    }
}
