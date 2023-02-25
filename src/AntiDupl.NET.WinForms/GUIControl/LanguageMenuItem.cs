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
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AntiDupl.NET
{
    public class LanguageMenuItem : ToolStripMenuItem
    {
        private Options m_options;

        private ToolStripMenuItem[] m_items;

        private Image m_trueIcon;
        private Image m_falseIcon;

        public LanguageMenuItem(Options options)
        {
            m_options = options;
            InitializeComponents();
            UpdateStrings();
            UpdateIcons();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateIcons);
        }

        private void InitializeComponents()
        {
            m_trueIcon = Resources.Images.GetImageWithBlackCircle(16, 16, 3.5);
            m_falseIcon = Resources.Images.GetNullImage();

            m_items = new ToolStripMenuItem[Resources.Strings.Count];
            for (int i = 0; i < Resources.Strings.Count; i++)
            {
                m_items[i] = InitFactory.MenuItem.Create(null, i, OnClick);
                m_items[i].Text = Resources.Strings.Get(i).OriginalLanguageName;
                DropDownItems.Add(m_items[i]);
            }
        }

        private void OnClick(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            m_options.Language = Resources.Strings.Get((int)item.Tag).Name;
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.LanguageMenuItem_Text;
        }

        void UpdateIcons()
        {
            for (int i = 0; i < m_items.Length; i++)
            {
                m_items[i].Image = (i == Resources.Strings.CurrentIndex ? m_trueIcon : m_falseIcon);
            }
        }
    }
}
