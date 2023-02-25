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
    public class ViewModeMenuItem : ToolStripMenuItem
    {
        private Options m_options;

        private ToolStripMenuItem m_verticalPairTableMenuItem;
        private ToolStripMenuItem m_horizontalPairTableMenuItem;
        private ToolStripMenuItem m_groupedThumbnailsMenuItem;

        private Image m_trueIcon;
        private Image m_falseIcon;

        public ViewModeMenuItem(Options options)
        {
            m_options = options;
            InitializeComponents();
            UpdateStrings();
            UpdateIcons(m_options.resultsOptions.viewMode);
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            m_options.resultsOptions.OnViewModeChange += new AntiDupl.NET.ResultsOptions.ViewModeChangeHandler(UpdateIcons);
        }

        private void InitializeComponents()
        {
            m_trueIcon = Resources.Images.GetImageWithBlackCircle(16, 16, 3.5);
            m_falseIcon = Resources.Images.GetNullImage();

            m_verticalPairTableMenuItem = InitFactory.MenuItem.Create(null, ResultsOptions.ViewMode.VerticalPairTable, OnClick);
            m_horizontalPairTableMenuItem = InitFactory.MenuItem.Create(null, ResultsOptions.ViewMode.HorizontalPairTable, OnClick);
            m_groupedThumbnailsMenuItem = InitFactory.MenuItem.Create(null, ResultsOptions.ViewMode.GroupedThumbnails, OnClick);

            //m_groupedThumbnailsMenuItem.Enabled = false;

            DropDownItems.Add(m_verticalPairTableMenuItem);
            DropDownItems.Add(m_horizontalPairTableMenuItem);
            //DropDownItems.Add(new ToolStripSeparator());
            //DropDownItems.Add(m_groupedThumbnailsMenuItem);
        }

        private void OnClick(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            m_options.resultsOptions.viewMode = (ResultsOptions.ViewMode)item.Tag;
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.ViewModeMenuItem_Text;
            m_verticalPairTableMenuItem.Text = s.ViewModeMenuItem_VerticalPairTableMenuItem_Text;
            m_horizontalPairTableMenuItem.Text = s.ViewModeMenuItem_HorizontalPairTableMenuItem_Text;
            m_groupedThumbnailsMenuItem.Text = s.ViewModeMenuItem_GroupedThumbnailsMenuItem_Text;
        }

        void UpdateIcons(ResultsOptions.ViewMode viewMode)
        {
            m_verticalPairTableMenuItem.Image = (viewMode == ResultsOptions.ViewMode.VerticalPairTable ? m_trueIcon : m_falseIcon);
            m_horizontalPairTableMenuItem.Image = (viewMode == ResultsOptions.ViewMode.HorizontalPairTable ? m_trueIcon : m_falseIcon);
            m_groupedThumbnailsMenuItem.Image = (viewMode == ResultsOptions.ViewMode.GroupedThumbnails ? m_trueIcon : m_falseIcon);
        }
    }
}
