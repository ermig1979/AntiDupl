/*
* AntiDupl Dynamic-Link Library.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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
#include "adHintSetter.h"
#include "adOptions.h"
#include "adResult.h"

namespace ad
{
    THintSetter::THintSetter(TOptions *pOptions)
        :m_pOptions(pOptions)
    {
        m_autoDeleteThresholdDifference = std::min(double(AUTO_DELETE_DIFFERENCE_MAX),
            double(m_pOptions->check.thresholdDifference)/AUTO_DELETE_DIFFERENCE_FACTOR);
    }

    void THintSetter::Execute(TResult *pResult, bool canRename) const
    {
        if(pResult->type == AD_RESULT_NONE || pResult->transform != AD_TRANSFORM_TURN_0)
        {
            pResult->hint = AD_HINT_NONE;
            return;
        }

        if(pResult->type == AD_RESULT_DEFECT_IMAGE)
        {
            pResult->hint = AD_HINT_DELETE_FIRST;
            return;
        }

        bool isFirstInDeletePath = 
            m_pOptions->deletePaths.IsHasPath(pResult->first->path) != AD_IS_NOT_EXIST ||
            m_pOptions->deletePaths.IsHasSubPath(pResult->first->path)  != AD_IS_NOT_EXIST;
        bool isSecondInDeletePath = 
            m_pOptions->deletePaths.IsHasPath(pResult->second->path) != AD_IS_NOT_EXIST ||
            m_pOptions->deletePaths.IsHasSubPath(pResult->second->path) != AD_IS_NOT_EXIST;

        if(pResult->difference == 0)
        {
            if(pResult->first->size > pResult->second->size)
            {
                if(isSecondInDeletePath || !isFirstInDeletePath)
                    pResult->hint = AD_HINT_DELETE_SECOND;
                else
                    pResult->hint = canRename ? AD_HINT_RENAME_FIRST_TO_SECOND : AD_HINT_NONE;
            }
            else if(pResult->first->size < pResult->second->size)
            {
                if(!isSecondInDeletePath || isFirstInDeletePath)
                    pResult->hint = AD_HINT_DELETE_FIRST;
                else
                    pResult->hint = canRename ? AD_HINT_RENAME_SECOND_TO_FIRST : AD_HINT_NONE;
            }
            else
            {
                if(isSecondInDeletePath || !isFirstInDeletePath)
                    pResult->hint = AD_HINT_DELETE_SECOND;
                else
                    pResult->hint = AD_HINT_DELETE_FIRST;
            }
            return;
        }

        if(pResult->difference < m_autoDeleteThresholdDifference &&
            pResult->first->type == pResult->second->type)
        {
            if(pResult->first->size >= pResult->second->size && 
                pResult->first->width*pResult->first->height >= 
                pResult->second->width*pResult->second->height)
            {
                if(isSecondInDeletePath || !isFirstInDeletePath)
                    pResult->hint = AD_HINT_DELETE_SECOND;
                else
                    pResult->hint = canRename ? AD_HINT_RENAME_FIRST_TO_SECOND : AD_HINT_NONE;
                return;
            }

            if(pResult->first->size <= pResult->second->size && 
                pResult->first->width*pResult->first->height <= 
                pResult->second->width*pResult->second->height)
            {
                if(!isSecondInDeletePath || isFirstInDeletePath)
                    pResult->hint = AD_HINT_DELETE_FIRST;
                else
                    pResult->hint = canRename ? AD_HINT_RENAME_SECOND_TO_FIRST : AD_HINT_NONE;
                return;
            }
        }

        pResult->hint = AD_HINT_NONE;
    }
}