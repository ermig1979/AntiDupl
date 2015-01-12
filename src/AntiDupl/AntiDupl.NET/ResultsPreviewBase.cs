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

namespace AntiDupl.NET
{
    public abstract class ResultsPreviewBase : Panel
    {
        protected ResultsListView m_resultsListView;
        protected Options m_options;
        protected CoreLib m_core;
        protected CoreOptions m_coreOptions;
        protected CoreResult m_currentSearchResult;

        protected TableLayoutPanel m_mainLayout;
        protected TableLayoutPanel m_imageLayout;
        protected TableLayoutPanel m_buttonLayout;

        protected ToolStripPanel m_toolStripPanel;
        protected ToolStrip m_toolStrip;

        protected ToolStripButton m_nextButton;
        protected ToolStripButton m_previousButton;

        public ResultsPreviewBase(CoreLib core, Options options, CoreOptions coreOptions, ResultsListView resultsListView)
        {
            m_core = core;
            m_options = options;
            m_coreOptions = coreOptions;
            m_resultsListView = resultsListView;

            InitializeComponents();
            UpdateStrings();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings); 
        }

        private void InitializeComponents()
        {
            m_toolStrip = new ToolStrip();
            m_toolStrip.GripStyle = ToolStripGripStyle.Hidden;
            m_toolStrip.RenderMode = ToolStripRenderMode.System;
            m_toolStrip.Renderer = new CustomToolStripSystemRenderer();
            m_toolStrip.AutoSize = true;

            m_toolStripPanel = new ToolStripPanel();
            m_toolStripPanel.BackColor = SystemColors.Control;

            m_nextButton = InitFactory.ToolButton.Create("NextButton", null, OnNextButtonClicked);
            m_previousButton = InitFactory.ToolButton.Create("PreviousButton", null, OnPreviousButtonClicked);
        }

        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            Controls.Clear();
            m_toolStripPanel.Controls.Clear();
            m_toolStrip.Items.Clear();
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_toolStripPanel.Orientation = Orientation.Vertical;

                m_buttonLayout = InitFactory.Layout.CreateVerticalCompensatedCenterAlign(30, 0);
                m_buttonLayout.Controls.Add(m_toolStripPanel, 0, 2);

                m_imageLayout = InitFactory.Layout.Create(1, 2, 0, 0);
                m_imageLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
                m_imageLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50F));
                
                m_mainLayout = InitFactory.Layout.Create(2, 1, 0, 0);
                m_mainLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100F));
                m_mainLayout.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
                m_mainLayout.Controls.Add(m_imageLayout, 0, 0);
                m_mainLayout.Controls.Add(m_buttonLayout, 1, 0);
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_toolStripPanel.Orientation = Orientation.Horizontal;

                m_buttonLayout = InitFactory.Layout.CreateHorizontalCompensatedCenterAlign(0, 1);
                m_buttonLayout.Controls.Add(m_toolStripPanel, 2, 0);

                m_imageLayout = InitFactory.Layout.Create(2, 1, 0, 0);
                m_imageLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
                m_imageLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));

                m_mainLayout = InitFactory.Layout.Create(1, 2, 0, 0);
                m_mainLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));
                m_mainLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
                m_mainLayout.Controls.Add(m_imageLayout, 0, 0);
                m_mainLayout.Controls.Add(m_buttonLayout, 0, 1);
            }
            m_imageLayout.CellBorderStyle = TableLayoutPanelCellBorderStyle.Inset;
            m_buttonLayout.AutoSize = true;
            AddItems(viewMode);
            m_toolStripPanel.Controls.Add(m_toolStrip);
            m_toolStripPanel.Size = new Size(m_toolStrip.PreferredSize.Width + 1, m_toolStrip.PreferredSize.Height + 1);
            Controls.Add(m_mainLayout);
        }

        protected abstract void AddItems(ResultsOptions.ViewMode viewMode);

        private void OnNextButtonClicked(object sender, System.EventArgs e)
        {
            m_resultsListView.SetKeyDownEvent(new KeyEventArgs(Keys.Down));
        }

        private void OnPreviousButtonClicked(object sender, System.EventArgs e)
        {
            m_resultsListView.SetKeyDownEvent(new KeyEventArgs(Keys.Up));
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_nextButton.ToolTipText = GetToolTip(s.ResultsPreviewBase_NextButton_ToolTip_Text, Keys.Down);
            m_previousButton.ToolTipText = GetToolTip(s.ResultsPreviewBase_PreviousButton_ToolTip_Text, Keys.Up);
        }
        
        protected string GetToolTip(string toolTip, Keys hotKey)
        {
            return string.Concat(toolTip, string.Format(" ({0})", hotKey));
        }

        protected string GetToolTip(string toolTip, HotKeyOptions.Action action)
        {
            return GetToolTip(toolTip, m_options.hotKeyOptions.keys[(int)action]);
        }

        
        protected void UpdateNextAndPreviousButtonEnabling()
        {
            if(m_resultsListView.GetTotalResultCount() != 0)
            {
                m_previousButton.Enabled = m_resultsListView.GetCurrentRowIndex() != 0;
                m_nextButton.Enabled = m_resultsListView.GetCurrentRowIndex() != m_resultsListView.GetTotalResultCount() - 1;
            }
            else
            {
                m_nextButton.Enabled = false;
                m_previousButton.Enabled = false;
            }
        }
    }

    internal class CustomToolStripSystemRenderer : ToolStripSystemRenderer
    {
        protected override void OnRenderToolStripBorder(ToolStripRenderEventArgs e)
        {
        }
    };
}
