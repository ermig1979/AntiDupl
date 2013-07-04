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
using System.Diagnostics;

namespace AntiDupl.NET
{
    public class ResultsPreviewDefect : ResultsPreviewBase
    {
        ImagePreviewPanel m_imagePreviewPanel;
        private ToolStripButton m_deleteButton;
        private ToolStripButton m_mistakeButton;

        public ResultsPreviewDefect(CoreLib core, Options options, CoreOptions coreOptions, ResultsListView resultsListView)
            : base(core, options, coreOptions, resultsListView)
        {
            InitializeComponents();
            UpdateStrings();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            OnOptionsChanged();
            m_options.OnChange += new Options.ChangeHandler(OnOptionsChanged);
        }

        private void InitializeComponents()
        {
            m_imagePreviewPanel = new ImagePreviewPanel(m_core, m_options, m_resultsListView, ImagePreviewPanel.Position.Top);
            m_deleteButton = InitFactory.ToolButton.Create("DeleteDefectVerticalButton", CoreDll.LocalActionType.DeleteDefect, OnButtonClicked);
            m_mistakeButton = InitFactory.ToolButton.Create("MistakeButton", CoreDll.LocalActionType.Mistake, OnButtonClicked);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_deleteButton.ToolTipText = GetToolTip(s.ResultsPreviewDefect_DeleteButton_ToolTip_Text, HotKeyOptions.Action.CurrentDefectDelete);
            m_mistakeButton.ToolTipText = GetToolTip(s.ResultsPreviewDefect_MistakeButton_ToolTip_Text, HotKeyOptions.Action.CurrentMistake);
        }

        public void SetCurrentSearchResult(CoreResult currentSearchResult)
        {
            m_currentSearchResult = currentSearchResult;
            m_imagePreviewPanel.SetResult(m_currentSearchResult);
            UpdateNextAndPreviousButtonEnabling();
        }

        private void OnImageDoubleClicked(object sender, System.EventArgs e)
        {
            PictureBox pictureBox = (PictureBox)sender;
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = pictureBox.ImageLocation;
            Process.Start(startInfo);
        }

        private void OnButtonClicked(object sender, System.EventArgs e)
        {
            ToolStripButton item = (ToolStripButton)sender;
            CoreDll.LocalActionType action = (CoreDll.LocalActionType)item.Tag;
            m_resultsListView.MakeAction(action, CoreDll.TargetType.Current);
        }

        private void OnOptionsChanged()
        {
            m_mistakeButton.Enabled = m_coreOptions.advancedOptions.mistakeDataBase;
        }

        protected override void AddItems(ResultsOptions.ViewMode viewMode)
        {
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_imagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Top);
                
                m_deleteButton.Image = Resources.Images.Get("DeleteDefectVerticalButton");
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_imagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Left);
                
                m_deleteButton.Image = Resources.Images.Get("DeleteDefectHorizontalButton");
            }
            m_imageLayout.Controls.Add(m_imagePreviewPanel, 0, 0);
            
            m_toolStrip.Items.Add(m_deleteButton);
            m_toolStrip.Items.Add(new ToolStripSeparator());
            m_toolStrip.Items.Add(m_previousButton);
            m_toolStrip.Items.Add(m_nextButton);
            m_toolStrip.Items.Add(new ToolStripSeparator());
            m_toolStrip.Items.Add(m_mistakeButton);
        }
    }
}
