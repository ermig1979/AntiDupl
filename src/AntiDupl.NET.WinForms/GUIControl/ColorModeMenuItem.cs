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
using System.Drawing;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// View → Theme: System, Light or Dark. The color mode can only be set
    /// before the first window is created, so a change applies at the next
    /// start.
    /// </summary>
    public class ColorModeMenuItem : ToolStripMenuItem
    {
        private Options m_options;

        private ToolStripMenuItem m_systemMenuItem;
        private ToolStripMenuItem m_lightMenuItem;
        private ToolStripMenuItem m_darkMenuItem;

        private Image m_trueIcon;
        private Image m_falseIcon;

        public ColorModeMenuItem(Options options)
        {
            m_options = options;
            InitializeComponents();
            UpdateStrings();
            UpdateIcons();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
        }

        private void InitializeComponents()
        {
            m_trueIcon = Resources.Images.GetImageWithCircle(16, 16, 3.5);
            m_falseIcon = Resources.Images.GetNullImage();

            m_systemMenuItem = InitFactory.MenuItem.Create(null, SystemColorMode.System, OnClick);
            m_lightMenuItem = InitFactory.MenuItem.Create(null, SystemColorMode.Classic, OnClick);
            m_darkMenuItem = InitFactory.MenuItem.Create(null, SystemColorMode.Dark, OnClick);

            DropDownItems.Add(m_systemMenuItem);
            DropDownItems.Add(m_lightMenuItem);
            DropDownItems.Add(m_darkMenuItem);
        }

        private void OnClick(object sender, EventArgs e)
        {
            SystemColorMode colorMode = (SystemColorMode)((ToolStripMenuItem)sender).Tag;
            if (colorMode == m_options.colorMode)
                return;
            m_options.colorMode = colorMode;
            UpdateIcons();
            MessageBox.Show(Resources.Strings.Current.ColorModeMenuItem_RestartMessage, Resources.ProductName,
                MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.ColorModeMenuItem_Text;
            m_systemMenuItem.Text = s.ColorModeMenuItem_SystemMenuItem_Text;
            m_lightMenuItem.Text = s.ColorModeMenuItem_LightMenuItem_Text;
            m_darkMenuItem.Text = s.ColorModeMenuItem_DarkMenuItem_Text;
        }

        private void UpdateIcons()
        {
            m_systemMenuItem.Image = m_options.colorMode == SystemColorMode.System ? m_trueIcon : m_falseIcon;
            m_lightMenuItem.Image = m_options.colorMode == SystemColorMode.Classic ? m_trueIcon : m_falseIcon;
            m_darkMenuItem.Image = m_options.colorMode == SystemColorMode.Dark ? m_trueIcon : m_falseIcon;
        }
    }
}
