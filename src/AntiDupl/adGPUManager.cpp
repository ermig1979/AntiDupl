/*
* AntiDuplPlus Program (http://github.com/Sucotasch/AntiDuplPlus).
*
* Copyright (c) 2023-2026.
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
#include "adGPUManager.h"
#include "adLogger.h"
#include <cstring>
#include <windows.h>

#define AD_DEBUG(msg) OutputDebugStringA(msg)

namespace ad
{
    TGpuManager::TGpuManager()
        : m_available(false), m_capacity(0), m_thumbSize(0)
    {
        AD_DEBUG("TGpuManager: Constructor starting\n");
        memset(&m_deviceInfo, 0, sizeof(m_deviceInfo));
        try
        {
            AD_DEBUG("TGpuManager: Calling GpuInit\n");
            if (GpuInit(&m_deviceInfo))
            {
                AD_DEBUG("TGpuManager: GpuInit succeeded, checking compatibility\n");
                if (m_deviceInfo.isCompatible)
                {
                    m_available = true;
                    AD_DEBUG("TGpuManager: GPU available and compatible\n");
                }
                else
                {
                    AD_DEBUG("TGpuManager: GPU not compatible\n");
                }
            }
            else
            {
                AD_DEBUG("TGpuManager: GpuInit failed\n");
            }
        }
        catch (const std::exception& e)
        {
            AD_DEBUG("TGpuManager: Exception caught\n");
            m_available = false;
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU: Exception during initialization.");
#endif
        }
        catch (...)
        {
            AD_DEBUG("TGpuManager: Unknown exception caught\n");
            m_available = false;
#ifdef AD_LOGGER_ENABLE
            AD_LOG("GPU: Unknown exception during initialization.");
#endif
        }
        AD_DEBUG("TGpuManager: Constructor finished\n");
    }

    TGpuManager::~TGpuManager()
    {
        if (m_available)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            GpuRelease();
        }
    }

    bool TGpuManager::EnsureCapacity(size_t required, size_t thumbSize)
    {
        if (!m_available) return false;
        
        std::lock_guard<std::mutex> lock(m_mutex);
        if (required <= m_capacity && thumbSize == m_thumbSize && m_capacity > 0) return true;

        // Reallocate if size changed or capacity is too small
        size_t newCapacity = (size_t)(required * 1.2);
        if (newCapacity < 1024) newCapacity = 1024; // Minimum buffer to avoid frequent reallocs

        if (GpuCreateBuffer(newCapacity, thumbSize))
        {
            m_capacity = newCapacity;
            m_thumbSize = thumbSize;
            return true;
        }
        return false;
    }
}
