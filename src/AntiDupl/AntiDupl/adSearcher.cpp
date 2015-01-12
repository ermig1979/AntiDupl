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
#include "adImage.h"
#include "adThreads.h"
#include "adFileUtils.h"
#include "adStatus.h"
#include "adOptions.h"
#include "adImageDataStorage.h"
#include "adEngine.h"
#include "adPerformance.h"
#include "adSearcher.h"

namespace ad
{
    TSearcher::TSearcher(TEngine *pEngine, TImageDataPtrs *pImageDataPtrs)
        :m_pImageDataPtrs(pImageDataPtrs),
        m_pStatus(pEngine->Status()),
        m_pOptions(pEngine->Options()),
        m_pImageDataStorage(pEngine->ImageDataStorage())
    {
    }

    void TSearcher::SearchImages()
    {
        AD_FUNCTION_PERFORMANCE_TEST
        InitExtensions();
        if(!m_extensions.empty())
        {
            for(size_t i = 0; i < m_pOptions->searchPaths.Size(); i++)
            {
                TString path = CreatePath(m_pOptions->searchPaths[i].Original());
                if(IsForbidden(path))
                    continue;
                if(IsDirectoryExists(path.c_str()))
                {
					SearchImages(path, m_pOptions->searchPaths[i].EnableSubFolder());
                }
                else if(IsWanted(path))
                {
                    m_pImageDataPtrs->push_back(m_pImageDataStorage->Get(TFileInfo(m_pOptions->searchPaths[i].Original())));
                }
            }
            m_pStatus->Search(NULL, m_pImageDataPtrs->size());
        }
    }

    void TSearcher::SearchImages(const TString& directory, bool enableSubFolder)
    {
        TString path;
        HANDLE hFind;
        WIN32_FIND_DATA findData;

        if(m_pOptions->ignorePaths.IsHasPath(directory) != AD_IS_NOT_EXIST)
            return;

        TString searchPath = CreatePath(directory, TEXT("*"));

        hFind = FindFirstFile(searchPath.c_str(), &findData);
        if(hFind != INVALID_HANDLE_VALUE) 
        {
            do 
            {
                TString name = findData.cFileName;
                if(name == TEXT(".") || name == TEXT(".."))
                    continue;
                if(m_pOptions->search.system == FALSE && 
                    (findData.dwFileAttributes&FILE_ATTRIBUTE_SYSTEM) != 0)
                    continue;
                if(m_pOptions->search.hidden == FALSE && 
                    (findData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN) != 0)
                    continue;

                TString path = CreatePath(directory, name);
                if((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && enableSubFolder)
                {
                    SearchImages(path, true);
                }
                else if(IsWanted(path.c_str()) && m_pOptions->ignorePaths.IsHasPath(path) == AD_IS_NOT_EXIST)
                {
                    m_pImageDataPtrs->push_back(m_pImageDataStorage->Get(TFileInfo(path,
                        findData.nFileSizeLow + findData.nFileSizeHigh*0x100000000, 
                        *(TUInt64*)&findData.ftLastWriteTime)));
                }
            } while(FindNextFile(hFind, &findData) != 0 && !m_pStatus->Stopped()); 
            FindClose(hFind);
        }
        m_pStatus->Search(directory.c_str(), m_pImageDataPtrs->size());
    }

    bool TSearcher::IsForbidden(const TString& path)
    {
        DWORD attributes;
        attributes = GetFileAttributes(path.c_str());
        if(attributes == INVALID_FILE_ATTRIBUTES)
            return true;
        if(m_pOptions->search.system == FALSE && 
            (attributes&FILE_ATTRIBUTE_SYSTEM) != 0)
            return true;
        if(m_pOptions->search.hidden == FALSE && 
            (attributes&FILE_ATTRIBUTE_HIDDEN) != 0)
            return true;
        if(m_pOptions->ignorePaths.IsHasSubPath(path) != AD_IS_NOT_EXIST)
            return true;
        if(m_pOptions->ignorePaths.IsHasPath(path) != AD_IS_NOT_EXIST)
            return true;
        return false;
    }


    bool TSearcher::IsWanted(const TString& path)
    {
        TString extension = GetFileExtension(path);
        extension.ToUpper();
        for(size_t i = 0; i < m_extensions.size(); ++i)
        {
            if(extension == m_extensions[i])
                return true;
        }
        return false;
    }

    void TSearcher::InitExtensions()
    {
        m_extensions.clear();
        AddExtensions(AD_IMAGE_BMP, m_pOptions->search.BMP);
        AddExtensions(AD_IMAGE_GIF, m_pOptions->search.GIF);
        AddExtensions(AD_IMAGE_JPEG, m_pOptions->search.JPEG);
        AddExtensions(AD_IMAGE_PNG, m_pOptions->search.PNG);
        AddExtensions(AD_IMAGE_TIFF, m_pOptions->search.TIFF);
        AddExtensions(AD_IMAGE_EMF, m_pOptions->search.EMF);
        AddExtensions(AD_IMAGE_WMF, m_pOptions->search.WMF);
        AddExtensions(AD_IMAGE_EXIF, m_pOptions->search.EXIF);
        AddExtensions(AD_IMAGE_ICON, m_pOptions->search.ICON);
        AddExtensions(AD_IMAGE_JP2, m_pOptions->search.JP2);
        AddExtensions(AD_IMAGE_PSD, m_pOptions->search.PSD);
		AddExtensions(AD_IMAGE_DDS, m_pOptions->search.DDS);
		AddExtensions(AD_IMAGE_TGA, m_pOptions->search.TGA);
    }

    void TSearcher::AddExtensions(TImageType imageType, adBool enable)
    {
        if(enable == TRUE)
        {
            TStrings extensions = TImage::Extensions((TImage::TFormat)imageType);
            m_extensions.insert(m_extensions.end(), extensions.begin(), extensions.end());
        }
    }
}
