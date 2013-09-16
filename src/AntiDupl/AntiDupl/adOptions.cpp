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
#include "adIniFile.h"
#include "adFileUtils.h"
#include "adOptions.h"

namespace ad
{
    //----------------Default values:--------------------------------------------

    const TChar DEFAULT_fileName[] = TEXT("AntiDupl.ini");

    //---------------------------------------------------------------------------
    TOption::TOption(int *pValue, const TChar* section, const TChar* key, int default, int min, int max)
        :m_pValue(pValue),
        m_section(section),
        m_key(key),
        m_default(default),
        m_min(min),
        m_max(max)
    {
    }

    void TOption::SetDefault()
    {
        *m_pValue = m_default;
    }

    void TOption::Validate()
    {
        if(*m_pValue < m_min || *m_pValue > m_max)
            *m_pValue = m_default;
    }

    void TOption::Load(const TChar *fileName)
    {
        TIniFile iniFile(fileName, m_section.c_str());
        *m_pValue = iniFile.ReadInteger(m_key.c_str(), m_default);
    }

    bool TOption::Save(const TChar *fileName) const
    {
        TIniFile iniFile(fileName, m_section.c_str());
        return iniFile.WriteInteger(m_key.c_str(), *m_pValue);
    }
    //---------------------------------------------------------------------------
    TOptions::TOptions()
        :searchPaths(TEXT("SearchPaths"), true),
        ignorePaths(TEXT("IgnorePaths")),
        validPaths(TEXT("ValidPaths")),
        deletePaths(TEXT("DeletePaths"))
    {
        m_options.push_back(TOption(&search.subFolders, TEXT("SearchOptions"), TEXT("SubFolders"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.system, TEXT("SearchOptions"), TEXT("System"), FALSE, FALSE, TRUE));
        m_options.push_back(TOption(&search.hidden, TEXT("SearchOptions"), TEXT("Hidden"), FALSE, FALSE, TRUE));
        m_options.push_back(TOption(&search.JPEG, TEXT("SearchOptions"), TEXT("JPEG"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.BMP, TEXT("SearchOptions"), TEXT("BMP"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.GIF, TEXT("SearchOptions"), TEXT("GIF"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.PNG, TEXT("SearchOptions"), TEXT("PNG"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.TIFF, TEXT("SearchOptions"), TEXT("TIFF"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.EMF, TEXT("SearchOptions"), TEXT("EMF"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.WMF, TEXT("SearchOptions"), TEXT("WMF"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.EXIF, TEXT("SearchOptions"), TEXT("EXIF"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.ICON, TEXT("SearchOptions"), TEXT("ICON"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.JP2, TEXT("SearchOptions"), TEXT("JP2"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&search.PSD, TEXT("SearchOptions"), TEXT("PSD"), TRUE, FALSE, TRUE));

        m_options.push_back(TOption(&check.checkOnDefect, TEXT("CheckOptions"), TEXT("CheckOnDefect"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&check.checkOnBlockiness, TEXT("CheckOptions"), TEXT("CheckOnBlockiness"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&check.blockinessThreshold, TEXT("CheckOptions"), TEXT("BlockinessThreshold"), 10, 0, 100));
        m_options.push_back(TOption(&check.checkOnEquality, TEXT("CheckOptions"), TEXT("CheckOnEquality"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&check.transformedImage, TEXT("CheckOptions"), TEXT("TransformedImage"), FALSE, FALSE, TRUE));
        m_options.push_back(TOption(&check.sizeControl, TEXT("CheckOptions"), TEXT("SizeControl"), FALSE, FALSE, TRUE));
        m_options.push_back(TOption(&check.typeControl, TEXT("CheckOptions"), TEXT("TypeControl"), FALSE, FALSE, TRUE));
        m_options.push_back(TOption(&check.ratioControl, TEXT("CheckOptions"), TEXT("RatioControl"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&check.thresholdDifference, TEXT("CheckOptions"), TEXT("ThresholdDifference"), 5, 0, 15));
        m_options.push_back(TOption(&check.minimalImageSize, TEXT("CheckOptions"), TEXT("MinimalImageSize"), 64, 0, INT_MAX));
		m_options.push_back(TOption(&check.maximalImageSize, TEXT("CheckOptions"), TEXT("MaximalImageSize"), 8196, 0, INT_MAX));
        m_options.push_back(TOption(&check.compareInsideOneFolder, TEXT("CheckOptions"), TEXT("CompareInsideOneFolder"), TRUE, FALSE, TRUE));

        m_options.push_back(TOption(&advanced.deleteToRecycleBin, TEXT("AdvancedOptions"), TEXT("DeleteToRecycleBin"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&advanced.mistakeDataBase, TEXT("AdvancedOptions"), TEXT("MistakeDataBase"), TRUE, FALSE, TRUE));
        m_options.push_back(TOption(&advanced.ratioResolution, TEXT("AdvancedOptions"), TEXT("RatioResolution"), 32, 8, 128));
        m_options.push_back(TOption(&advanced.compareThreadCount, TEXT("AdvancedOptions"), TEXT("CompareThreadCount"), 0, 0, 256));
        m_options.push_back(TOption(&advanced.collectThreadCount, TEXT("AdvancedOptions"), TEXT("CollectThreadCount"), 0, 0, 256));
        m_options.push_back(TOption(&advanced.reducedImageSize, TEXT("AdvancedOptions"), TEXT("ReducedImageSize"), 32, REDUCED_IMAGE_SIZE_MIN, INITIAL_REDUCED_IMAGE_SIZE >> 1));
        m_options.push_back(TOption(&advanced.undoQueueSize, TEXT("AdvancedOptions"), TEXT("UndoQueueSize"), 10, 0, 16));
        m_options.push_back(TOption(&advanced.resultCountMax, TEXT("AdvancedOptions"), TEXT("ResultCountMax"), 100000, 1, INT_MAX));
        m_options.push_back(TOption(&advanced.ignoreFrameWidth, TEXT("AdvancedOptions"), TEXT("IgnoreFrameWidth"), 0, 0, 12));

        SetDefault();
    }

    void TOptions::SetDefault()
    {
        for(TOptionsList::iterator it = m_options.begin();  it != m_options.end(); ++it)
            it->SetDefault();
    }

    void TOptions::Validate()
    {
        for(TOptionsList::iterator it = m_options.begin();  it != m_options.end(); ++it)
            it->Validate();

        for(int size = REDUCED_IMAGE_SIZE_MIN; size < INITIAL_REDUCED_IMAGE_SIZE; size <<= 1)
        {
            if(size >= advanced.reducedImageSize)
            {
                advanced.reducedImageSize = size;
                advanced.ignoreFrameWidth = int(ceil(size*advanced.ignoreFrameWidth/double(DENOMINATOR*2))/size*DENOMINATOR*2);
                break;
            }
        }
    }

    adError TOptions::Import(adOptionsType optionsType, void* pOptions)
    {
        if(pOptions == NULL)
            return AD_ERROR_INVALID_POINTER;

        switch(optionsType)
        {
        case AD_OPTIONS_SEARCH:
            memcpy(&search, pOptions, sizeof(adSearchOptions));
            break;
        case AD_OPTIONS_CHECK:
            memcpy(&check, pOptions, sizeof(adCheckOptions));
            break;
        case AD_OPTIONS_ADVANCED:
            memcpy(&advanced, pOptions, sizeof(adAdvancedOptions));
            break;
        default:
            return AD_ERROR_INVALID_OPTIONS_TYPE;
        }

        Validate();

        return AD_OK;
    }

    adError TOptions::Export(adOptionsType optionsType, void* pOptions) const
    {
        if(pOptions == NULL)
            return AD_ERROR_INVALID_POINTER;

        switch(optionsType)
        {
        case AD_OPTIONS_SEARCH:
            memcpy(pOptions, &search, sizeof(adSearchOptions));
            break;
        case AD_OPTIONS_CHECK:
            memcpy(pOptions, &check, sizeof(adCheckOptions));
            break;
        case AD_OPTIONS_ADVANCED:
            memcpy(pOptions, &advanced, sizeof(adAdvancedOptions));
            break;
        default:
            return AD_ERROR_INVALID_OPTIONS_TYPE;
        }

        return AD_OK;
    }

    adError TOptions::Load(const TChar *fileName)
    {
        TString path;
        if(fileName == NULL)
            path = GetApplicationDirectory() + TEXT("\\") + DEFAULT_fileName;
        else
            path = fileName;

        if(!IsFileExists(path.c_str()))
            return AD_ERROR_FILE_IS_NOT_EXIST;

        searchPaths.Load(path.c_str());
        ignorePaths.Load(path.c_str());
        validPaths.Load(path.c_str());
        deletePaths.Load(path.c_str());

        for(TOptionsList::iterator it = m_options.begin();  it != m_options.end(); ++it)
            it->Load(path.c_str());

        Validate();

        return AD_OK;
    }

    adError TOptions::Save(const TChar *fileName) const
    {
        bool result = true;

        TString path;
        if(fileName == NULL)
            path = GetApplicationDirectory() + TEXT("\\") + DEFAULT_fileName;
        else
            path = fileName;

        result = result && searchPaths.Save(path.c_str());
        result = result && ignorePaths.Save(path.c_str());
        result = result && validPaths.Save(path.c_str());
        result = result && deletePaths.Save(path.c_str());

        for(TOptionsList::const_iterator it = m_options.begin();  it != m_options.end(); ++it)
            result = result && it->Save(path.c_str());

        return result ? AD_OK : AD_ERROR_UNKNOWN;
    }

    int TOptions::GetIgnoreWidthFrame() const
    {
        return (int)ceil(advanced.reducedImageSize*advanced.ignoreFrameWidth/double(DENOMINATOR));
    }
    //---------------------------------------------------------------------------
}