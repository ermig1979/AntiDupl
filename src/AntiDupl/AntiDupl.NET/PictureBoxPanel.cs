/*
* AntiDupl.NET Program.
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
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.IO;

namespace AntiDupl.NET
{
    public class PictureBoxPanel : Panel
    {
        private const int MAX_PATH = 260;

        private CoreLib m_core;
        private Options m_options;
        private CoreImageInfo m_currentImageInfo;
        private Size m_neighbourSizeMax;

        private Rectangle m_bitmapRect;
        private MemoryStream m_memoryStream;
        private Bitmap m_bitmap;
        private bool m_animationEnable = false;
        private bool m_currentlyAnimating = false;

        public PictureBoxPanel(CoreLib core, Options options)
        {
            m_core = core;
            m_options = options;
            InitializeComponents();
            m_options.resultsOptions.OnImageViewChange += new ResultsOptions.ImageViewChangeHandler(OnImageViewChange);
        }
        
        private void InitializeComponents()
        {
            Location = new System.Drawing.Point(0, 0);
            Dock = DockStyle.Fill;
            BorderStyle = BorderStyle.Fixed3D;
            BackColor = Color.DarkGray;
            DoubleBuffered = true;

            DoubleClick += new EventHandler(OnImageDoubleClicked);
            SizeChanged += new EventHandler(OnSizeChanged);
        }

        public void UpdateImage(CoreImageInfo currentImageInfo)
        {
            m_currentImageInfo = currentImageInfo;
            if(currentImageInfo != null)
            {
                StopAnimate();
                if (m_currentImageInfo.type != CoreDll.ImageType.None)
                {
                    if (m_currentImageInfo.path.Length < MAX_PATH && m_currentImageInfo.type <= CoreDll.ImageType.Icon)
                    {
                        try
                        {
                            if (LoadFileToMemoryStream(m_currentImageInfo.path))
                            {
                                m_bitmap = new Bitmap(m_memoryStream);
                                m_animationEnable = ImageAnimator.CanAnimate(m_bitmap);
                                if (m_animationEnable)
                                    m_currentlyAnimating = false;
                            }
                            else
                            {
                                m_bitmap = null;
                            }
                        }
                        catch
                        {
                            m_bitmap = m_core.LoadBitmap(m_currentImageInfo);
                        }
                    }
                    else
                    {
                        m_bitmap = m_core.LoadBitmap(m_currentImageInfo);
                    } 
                }
                else
                {
                    m_bitmap = null;
                }
            }
        }

        private bool LoadFileToMemoryStream(string path)
        {
            if (m_memoryStream != null)
            {
                m_memoryStream.Close();
                m_memoryStream.Dispose();
                m_memoryStream = null;
            }
            FileInfo fileInfo = new FileInfo(path);
            if (fileInfo.Exists)
            {
                try
                {
                    FileStream fileStream = new FileStream(fileInfo.FullName, FileMode.Open, FileAccess.Read);
                    byte[] buffer = new byte[fileStream.Length];
                    fileStream.Read(buffer, 0, buffer.Length);
                    fileStream.Close();
                    m_memoryStream = new MemoryStream(buffer);
                    return true;
                }
                catch
                {
                    return false;
                }
            }
            return false;
        }


        private void AnimateImage()
        {
            if (!m_currentlyAnimating)
            {
                ImageAnimator.Animate(m_bitmap, new EventHandler(OnFrameChanged));
                m_currentlyAnimating = true;
            }
        }

        private void StopAnimate()
        {
            m_animationEnable = false;
            ImageAnimator.StopAnimate(m_bitmap, new EventHandler(OnFrameChanged));
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_animationEnable)
            {
                try
                {
                    AnimateImage();
                    ImageAnimator.UpdateFrames();
                }
                catch
                {
                    m_animationEnable = false;
                }
            }
            if (m_bitmap != null)
            {
                e.Graphics.DrawImage(m_bitmap, m_bitmapRect);
            }
        }

        private void OnFrameChanged(object sender, EventArgs e)
        {
            Invalidate();
        }
        
        private void OnImageDoubleClicked(object sender, System.EventArgs e)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = m_currentImageInfo.path;
            try
            {
                Process.Start(startInfo);
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }

        private void OnSizeChanged(object sender, EventArgs e)
        {
            UpdateImagePadding(m_neighbourSizeMax);
        }

        private void OnImageViewChange()
        {
            UpdateImagePadding(m_neighbourSizeMax);
        }

        public void UpdateImagePadding(Size neighbourSizeMax)
        {
            m_neighbourSizeMax = neighbourSizeMax;
            if (m_currentImageInfo != null)
            {
                int hp = 0, vp = 0;
                int w = ClientSize.Width;
                int h = ClientSize.Height;
                int cw = (int)m_currentImageInfo.width;
                int ch = (int)m_currentImageInfo.height;
                if (cw > 0 && ch > 0)
                {
                    if (m_options.resultsOptions.ProportionalImageSize)
                    {
                        int nw = (int)m_neighbourSizeMax.Width;
                        int nh = (int)m_neighbourSizeMax.Height;
                        int mw = Math.Max(cw, nw);
                        int mh = Math.Max(ch, nh);
                        if (m_options.resultsOptions.StretchSmallImages || mw >= w || mh >= h)
                        {
                            if (mw * h > mh * w)
                            {
                                vp = (h - ch * w / mw) / 2;
                                hp = (w - cw * w / mw) / 2;
                            }
                            else
                            {
                                vp = (h - ch * h / mh) / 2;
                                hp = (w - cw * h / mh) / 2;
                            }
                        }
                        else
                        {
                            vp = (h - ch) / 2;
                            hp = (w - cw) / 2;
                        }
                    }
                    else
                    {
                        if (m_options.resultsOptions.StretchSmallImages || cw >= w || ch >= h)
                        {
                            if (cw * h > ch * w)
                            {
                                vp = (h - w * ch / cw) / 2;
                            }
                            else
                            {
                                hp = (w - h * cw / ch) / 2;
                            }
                        }
                        else
                        {
                            vp = (h - ch) / 2;
                            hp = (w - cw) / 2;
                        }
                    }
                }
                m_bitmapRect = new Rectangle(hp, vp, w - 2 * hp, h - 2 * vp);
            }
            Refresh();
        }
    }
}
