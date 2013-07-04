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
using System.Windows.Forms;
using System.Drawing;

namespace AntiDupl.NET
{
    public class MainSplitContainer : SplitContainer
    {
        public const int VIEW_MIN_WIDTH = 260;
        public const int LIST_MIN_WIDTH = 0;
        public const int VIEW_MIN_HEIGHT = 200;
        public const int LIST_MIN_HEIGHT = 0;

        private MainForm m_mainForm;
        private Options m_options;
        CoreLib m_core;
        private CoreOptions m_coreOptions;

        private ResultsListView m_resultsListView;
        private ResultsPreviewContainer m_resultsPreviewContainer;

        private ThumbnailGroupTable m_thumbnailGroupTable;
        private ThumbnailPreview m_thumbnailPreview;
        
        bool m_setOrientationNow = false;
        bool m_atLeastOneTimeSetOrientation = false;

        public ResultsListView resultsListView { get { return m_resultsListView; } }

        public delegate void UpdateResultsHandler();
        public event UpdateResultsHandler OnUpdateResults;

        public delegate void CurrentResultChangedHandler();
        public event CurrentResultChangedHandler OnCurrentResultChanged;

        public delegate void SelectedResultsChangedHandler();
        public event SelectedResultsChangedHandler OnSelectedResultsChanged;

        public MainSplitContainer(CoreLib core, Options options, CoreOptions coreOptions, MainForm mainForm)
        {
            m_mainForm = mainForm;
            m_options = options;
            m_core = core;
            m_coreOptions = coreOptions;

            m_resultsListView = new ResultsListView(m_core, m_options, m_coreOptions, this);
            m_resultsPreviewContainer = new ResultsPreviewContainer(m_core, m_options, m_coreOptions, this);

            //m_resultsListView.UpdateResults();

            m_thumbnailGroupTable = new ThumbnailGroupTable(m_core, m_options, this);
            m_thumbnailPreview = new ThumbnailPreview(m_core, m_options, this);
            m_thumbnailPreview.ContextMenuStrip = new ThumbnailPreviewContextMenu(m_core, m_options, m_thumbnailPreview, m_thumbnailGroupTable);
            m_thumbnailGroupTable.OnCurrentThumbnailChanged += m_thumbnailPreview.SetThumbnail;

            //m_thumbnailGroupTable.UpdateGroups();

            InitializeComponents();

            m_options.resultsOptions.OnViewModeChange += new ResultsOptions.ViewModeChangeHandler(SetViewMode);

            SplitterMoved += new SplitterEventHandler(OnSplitterPositionChanged);
            Resize += new EventHandler(OnSizeChanged);
        }

        private void InitializeComponents()
        {
            BorderStyle = BorderStyle.Fixed3D;
            Dock = DockStyle.Fill;
            Location = new Point(0, 0);
            SplitterWidth = 2;
            FixedPanel = FixedPanel.Panel1;
            Size = new Size(VIEW_MIN_WIDTH + LIST_MIN_WIDTH, VIEW_MIN_HEIGHT + LIST_MIN_HEIGHT + SplitterWidth);
            Panel1MinSize = VIEW_MIN_WIDTH;
            Panel2MinSize = LIST_MIN_WIDTH;
            SplitterDistance = VIEW_MIN_WIDTH;
            Orientation = Orientation.Vertical;
        }

        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            Panel2.Controls.Clear();
            Panel1.Controls.Clear();
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable || viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                Panel2.Controls.Add(m_resultsListView);
                Panel1.Controls.Add(m_resultsPreviewContainer);
            }
            if (viewMode == ResultsOptions.ViewMode.GroupedThumbnails)
            {
                Panel2.Controls.Add(m_thumbnailGroupTable);
                Panel1.Controls.Add(m_thumbnailPreview);
            }

            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable || viewMode == ResultsOptions.ViewMode.GroupedThumbnails)
            {
                Panel1MinSize = 0;
                Panel2MinSize = 0;
                m_setOrientationNow = true;
                Orientation = Orientation.Vertical;
                m_setOrientationNow = false;
                SetSplitterDistance();
                Panel1MinSize = VIEW_MIN_WIDTH;
                Panel2MinSize = LIST_MIN_WIDTH;
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                Panel1MinSize = 0;
                Panel2MinSize = 0;
                m_setOrientationNow = true;
                Orientation = Orientation.Horizontal;
                m_setOrientationNow = false;
                SetSplitterDistance();
                Panel1MinSize = VIEW_MIN_HEIGHT;
                Panel2MinSize = LIST_MIN_HEIGHT;
            }

            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable || viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_resultsPreviewContainer.SetViewMode(viewMode);
                m_resultsListView.SetViewMode(viewMode);
            }

            m_atLeastOneTimeSetOrientation = true;

            UpdateResults();
        }

        private void OnSplitterPositionChanged(Object sender, SplitterEventArgs e)
        {
            if (!m_setOrientationNow && m_atLeastOneTimeSetOrientation)
            {
                GetSplitterDistance();
            }
        }

        private void OnSizeChanged(object sender, System.EventArgs e)
        {
            if (m_atLeastOneTimeSetOrientation)
            {
                SetSplitterDistance();
            }
        }
        
        private void SetSplitterDistance()
        {
            ResultsOptions options = m_options.resultsOptions;
            if (options.viewMode == ResultsOptions.ViewMode.VerticalPairTable || options.viewMode == ResultsOptions.ViewMode.GroupedThumbnails)
            {
                if (m_mainForm.WindowState == FormWindowState.Maximized)
                {
                    SplitterDistance = Math.Min(Math.Max(options.splitterDistanceVerticalMaximized, VIEW_MIN_WIDTH), Width - LIST_MIN_WIDTH - SplitterWidth);
                }
                if (m_mainForm.WindowState == FormWindowState.Normal)
                {
                    SplitterDistance = Math.Min(Math.Max(options.splitterDistanceVerticalNormal, VIEW_MIN_WIDTH), Width - LIST_MIN_WIDTH - SplitterWidth);
                }
            }
            if (options.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                if (m_mainForm.WindowState == FormWindowState.Maximized)
                {
                    SplitterDistance = Math.Min(Math.Max(options.splitterDistanceHorizontalMaximized, VIEW_MIN_HEIGHT), Height - LIST_MIN_HEIGHT - SplitterWidth);
                }
                if (m_mainForm.WindowState == FormWindowState.Normal)
                {
                    SplitterDistance = Math.Min(Math.Max(options.splitterDistanceHorizontalNormal, VIEW_MIN_HEIGHT), Height - LIST_MIN_HEIGHT - SplitterWidth);
                }
            }
        }
        
        private void GetSplitterDistance()
        {
            ResultsOptions options = m_options.resultsOptions;
            if (options.viewMode == ResultsOptions.ViewMode.VerticalPairTable || options.viewMode == ResultsOptions.ViewMode.GroupedThumbnails)
            {
                if (m_mainForm.WindowState == FormWindowState.Maximized)
                {
                    options.splitterDistanceVerticalMaximized = SplitterDistance;
                }
                if (m_mainForm.WindowState == FormWindowState.Normal)
                {
                    options.splitterDistanceVerticalNormal = SplitterDistance;
                }
            }
            if (options.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                if (m_mainForm.WindowState == FormWindowState.Maximized)
                {
                    options.splitterDistanceHorizontalMaximized = SplitterDistance;
                }
                if (m_mainForm.WindowState == FormWindowState.Normal)
                {
                    options.splitterDistanceHorizontalNormal = SplitterDistance;
                }
            }
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            if (m_options.resultsOptions.IsPairTableView())
            {
                m_resultsListView.SetKeyDownEvent(e);
            }
        }

        public void UpdateResults()
        {
            if (m_options.resultsOptions.IsPairTableView())
            {
                if (m_resultsListView != null)
                    m_resultsListView.UpdateResults();
            }
            else
            {
                if (m_thumbnailGroupTable != null)
                    m_thumbnailGroupTable.UpdateGroups();
            }
            if (OnUpdateResults != null)
                OnUpdateResults();
        }

        public void ClearResults()
        {
            if (m_options.resultsOptions.IsPairTableView())
            {
                if (m_resultsListView != null)
                    m_resultsListView.ClearResults();
            }
            else
            {
                if (m_thumbnailGroupTable != null)
                    m_thumbnailGroupTable.ClearGroups();
            }
            if (OnUpdateResults != null)
                OnUpdateResults();
            CurrentResultChanged();
        }

        public void CurrentResultChanged()
        {
            if (OnCurrentResultChanged != null)
                OnCurrentResultChanged();
        }

        public void SelectedResultsChanged()
        {
            if (OnSelectedResultsChanged != null)
                OnSelectedResultsChanged();
        }
    }
}
