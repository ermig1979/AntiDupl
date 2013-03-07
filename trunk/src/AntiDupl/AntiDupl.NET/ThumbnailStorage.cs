/*
* AntiDupl.NET Program.
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
using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Threading;

namespace AntiDupl.NET
{
    public class ThumbnailStorage
    {
        private CoreLib m_core;
        private Options m_options;
        private Dictionary<ulong, Bitmap> m_storage = new Dictionary<ulong,Bitmap>();
        private Mutex m_mutex = new Mutex();
        
        public ThumbnailStorage(CoreLib core, Options options)
        {
            m_core = core;
            m_options = options;
        }
        
        public void Clear()
        {
            m_mutex.WaitOne();
            m_storage.Clear();
            m_mutex.ReleaseMutex();
        }
        
        public bool Exists(CoreImageInfo imageInfo)
        {
            bool result = false;
            m_mutex.WaitOne();
            if (m_storage.ContainsKey(imageInfo.id))
            {
                Bitmap bitmap = m_storage[imageInfo.id];
                if(bitmap != null)
                {
                    Size size = GetThumbnailSize(imageInfo);
                    result = (bitmap.Height == size.Height && bitmap.Width == size.Width);
                }
            }
            m_mutex.ReleaseMutex();
            return result;
        }

        public Bitmap Get(CoreImageInfo imageInfo)
        {
            Bitmap bitmap = null;
            Size size = GetThumbnailSize(imageInfo);
            m_mutex.WaitOne();
            m_storage.TryGetValue(imageInfo.id, out bitmap);
            if (bitmap == null || bitmap.Height != size.Height || bitmap.Width != size.Width)
            {
                m_mutex.ReleaseMutex();
                bitmap = m_core.LoadBitmap(size, imageInfo.path);
                m_mutex.WaitOne();
                m_storage[imageInfo.id] = bitmap;
            }
            m_mutex.ReleaseMutex();
            return bitmap;
        }

        private Size GetThumbnailSize(CoreImageInfo imageInfo)
        {
            Size sizeMax = m_options.resultsOptions.thumbnailSizeMax;
            if (sizeMax.Width * imageInfo.height > sizeMax.Height * imageInfo.width)
            {
                return new Size(sizeMax.Width, (int)(sizeMax.Height * imageInfo.height / imageInfo.width));
            }
            else
            {
                return new Size((int)(sizeMax.Width * imageInfo.width / imageInfo.height), sizeMax.Height);
            }
        } 
    }
}
