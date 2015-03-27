/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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
        hint(AD_HINT_NONE)
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
        hint(result.hint)
    {
    }

    bool TResult::ImageInfoLesser(TImageInfoPtr pFirst, TImageInfoPtr pSecond, TSortType sortType, bool increasing)
    {
        switch(sortType)
        {
        case AD_SORT_BY_SORTED_PATH:
            return (increasing ? 
                TPath::LesserByPath(pFirst->path, pSecond->path) : 
                TPath::BiggerByPath(pFirst->path, pSecond->path));
        case AD_SORT_BY_SORTED_NAME:
            return (increasing ?
                TPath::LesserByNameWithExtension(pFirst->path, pSecond->path) : 
                TPath::BiggerByNameWithExtension(pFirst->path, pSecond->path));
        case AD_SORT_BY_SORTED_DIRECTORY:
            return (increasing ?
                TPath::LesserByDirectory(pFirst->path, pSecond->path) : 
                TPath::BiggerByDirectory(pFirst->path, pSecond->path));
        case AD_SORT_BY_SORTED_SIZE:
            return (increasing ? 
                pFirst->size < pSecond->size : 
                pFirst->size > pSecond->size);
        case AD_SORT_BY_SORTED_TIME:
            return (increasing ? 
                pFirst->time < pSecond->time : 
                pFirst->time > pSecond->time);
        case AD_SORT_BY_SORTED_TYPE:
            return (increasing ? 
                pFirst->type < pSecond->type : 
                pFirst->type > pSecond->type);
        case AD_SORT_BY_SORTED_WIDTH:
            return (increasing ? 
                pFirst->width < pSecond->width : 
                pFirst->width > pSecond->width);
        case AD_SORT_BY_SORTED_HEIGHT:
            return (increasing ? 
                pFirst->height < pSecond->height : 
                pFirst->height > pSecond->height);
        case AD_SORT_BY_SORTED_AREA:
            return (increasing ? 
                pFirst->width*pFirst->height < pSecond->width*pSecond->height : 
                pFirst->width*pFirst->height > pSecond->width*pSecond->height);
        case AD_SORT_BY_SORTED_RATIO:
            return (increasing ? 
                pFirst->width*pSecond->height < pSecond->width*pFirst->height : 
                pFirst->width*pSecond->height > pSecond->width*pFirst->height);
		case AD_SORT_BY_SORTED_BLOCKINESS:
			return (increasing ? 
                pFirst->blockiness < pSecond->blockiness : 
                pFirst->blockiness > pSecond->blockiness);
		case AD_SORT_BY_SORTED_BLURRING:
			return (increasing ? 
                pFirst->blurring < pSecond->blurring : 
                pFirst->blurring > pSecond->blurring);
        }

        return false;
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

    bool TResultPtrLesser::operator() (TResultPtr pFirst, TResultPtr pSecond)
    {
        if(m_sortType > AD_SORT_BY_TYPE && m_sortType < AD_SORT_BY_DEFECT)
        {
            if(m_sortType < AD_SORT_BY_FIRST_PATH)
            {
                return TResult::ImageInfoLesser(pFirst->first, pSecond->first, m_sortType, m_increasing);
            }
            else if(m_sortType < AD_SORT_BY_SECOND_PATH)
            {
                return TResult::ImageInfoLesser(pFirst->first, pSecond->first, 
                    adSortType(m_sortType + AD_SORT_BY_SORTED_PATH - AD_SORT_BY_FIRST_PATH), m_increasing);
            }
            else
            {
                return TResult::ImageInfoLesser(pFirst->second, pSecond->second, 
                    adSortType(m_sortType + AD_SORT_BY_SORTED_PATH - AD_SORT_BY_SECOND_PATH), m_increasing);
            }
        }
        else
        {
            switch(m_sortType)
            {
            case AD_SORT_BY_TYPE:
                return m_increasing ? 
                    pFirst->type < pSecond->type : 
                    pFirst->type > pSecond->type;
            case AD_SORT_BY_DEFECT:
                return m_increasing ? 
                    pFirst->defect < pSecond->defect : 
                    pFirst->defect > pSecond->defect;
            case AD_SORT_BY_DIFFERENCE:
                return m_increasing ? 
                    pFirst->difference < pSecond->difference : 
                    pFirst->difference > pSecond->difference;
            case AD_SORT_BY_TRANSFORM:
                return m_increasing ? 
                    pFirst->transform < pSecond->transform  : 
                    pFirst->transform  > pSecond->transform ;
            case AD_SORT_BY_GROUP:
                return m_increasing ? 
                    pFirst->group < pSecond->group : 
                    pFirst->group > pSecond->group;
			case AD_SORT_BY_GROUP_SIZE:
                return m_increasing ? 
                    pFirst->groupSize < pSecond->groupSize : 
                    pFirst->groupSize > pSecond->groupSize;
            case AD_SORT_BY_HINT:
                return m_increasing ? 
                    pFirst->hint < pSecond->hint : 
                    pFirst->hint > pSecond->hint;
            }
        }

        return false;
    }
}
