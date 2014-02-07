/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2014 Yermalayeu Ihar.
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

namespace AntiDupl.NET
{
    public class ThumbnailGroupPanel : RaisedPanel
    {
        private CoreLib m_core;
        private CoreGroup m_group;
        private AntiDupl.NET.Options m_options;

        private ThumbnailPanel[] m_thumbnailPanels;
        public ThumbnailPanel[] ThumbnailPanels { get { return m_thumbnailPanels; } }

        private ThumbnailGroupTable m_thumbnailGroupTable;
        public ThumbnailGroupTable Table { get { return m_thumbnailGroupTable; } }

        public ThumbnailGroupPanel(CoreLib core, AntiDupl.NET.Options options, CoreGroup group, ThumbnailGroupTable thumbnailGroupTable)
        {
            m_core = core;
            m_options = options;
            m_group = group;
            m_thumbnailGroupTable = thumbnailGroupTable;
            InitializeComponents();
        }

        private void InitializeComponents()
        {
            DoubleBuffered = true;
            BackColor = Color.Transparent;

            int width = 0;
            int height = 0;
            m_thumbnailPanels = new ThumbnailPanel[m_group.images.Length];
            for (int i = 0; i < m_group.images.Length; ++i)
            {
                m_thumbnailPanels[i] = new ThumbnailPanel(m_core, m_options, m_group, i, this);
                m_thumbnailPanels[i].Location = new Point(Padding.Left + m_thumbnailPanels[i].Margin.Left + (m_thumbnailPanels[i].Width + m_thumbnailPanels[i].Margin.Horizontal)*i,
                    Padding.Top + m_thumbnailPanels[i].Margin.Top);

                width += m_thumbnailPanels[i].Width + m_thumbnailPanels[i].Padding.Horizontal + Margin.Horizontal;
                height = Math.Max(height, m_thumbnailPanels[i].Height + m_thumbnailPanels[i].Padding.Vertical + Margin.Vertical);
            }
            ClientSize = new Size(width, height);

            Controls.AddRange(m_thumbnailPanels);
        }
    }
}
