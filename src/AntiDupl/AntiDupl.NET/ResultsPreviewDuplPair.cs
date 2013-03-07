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
    public class ResultsPreviewDuplPair : ResultsPreviewBase
    {
        private ImagePreviewPanel m_firstImagePreviewPanel;
        private ImagePreviewPanel m_secondImagePreviewPanel;
        private ToolStripButton m_deleteFirstButton;
        private ToolStripButton m_deleteSecondButton;
        private ToolStripButton m_renameFirstToSecondButton;
        private ToolStripButton m_renameSecondToFirstButton;
        private ToolStripButton m_deleteBothButton;
        private ToolStripButton m_mistakeButton;

        public ResultsPreviewDuplPair(CoreLib core, Options options, ResultsListView resultsListView)
            : base(core, options, resultsListView)
        {
            InitializeComponents();
            UpdateStrings();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            OnOptionsChanged();
            m_options.OnChange += new Options.ChangeHandler(OnOptionsChanged);
        }

        private void InitializeComponents()
        {
            m_firstImagePreviewPanel = new ImagePreviewPanel(m_core, m_options, m_resultsListView, ImagePreviewPanel.Position.Top);
            m_secondImagePreviewPanel = new ImagePreviewPanel(m_core, m_options, m_resultsListView, ImagePreviewPanel.Position.Bottom);

            m_deleteFirstButton = InitFactory.ToolButton.Create("DeleteFirstVerticalButton", CoreDll.LocalActionType.DeleteFirst, OnButtonClicked);
            m_deleteSecondButton = InitFactory.ToolButton.Create("DeleteSecondVerticalButton", CoreDll.LocalActionType.DeleteSecond, OnButtonClicked);
            m_deleteBothButton = InitFactory.ToolButton.Create("DeleteBothVerticalButton", CoreDll.LocalActionType.DeleteBoth, OnButtonClicked);
            m_renameFirstToSecondButton = InitFactory.ToolButton.Create("RenameFirstToSecondVerticalButton", CoreDll.LocalActionType.RenameFirstToSecond, OnButtonClicked);
            m_renameSecondToFirstButton = InitFactory.ToolButton.Create("RenameSecondToFirstVerticalButton", CoreDll.LocalActionType.RenameSecondToFirst, OnButtonClicked);
            m_mistakeButton = InitFactory.ToolButton.Create("MistakeButton", CoreDll.LocalActionType.Mistake, OnButtonClicked);
        }

        public void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_deleteFirstButton.ToolTipText = GetToolTip(s.ResultsPreviewDuplPair_DeleteFirstButton_ToolTip_Text, HotKeyOptions.Action.CurrentDuplPairDeleteFirst);
            m_deleteSecondButton.ToolTipText = GetToolTip(s.ResultsPreviewDuplPair_DeleteSecondButton_ToolTip_Text, HotKeyOptions.Action.CurrentDuplPairDeleteSecond);
            m_deleteBothButton.ToolTipText = GetToolTip(s.ResultsPreviewDuplPair_DeleteBothButton_ToolTip_Text, HotKeyOptions.Action.CurrentDuplPairDeleteBoth);
            m_renameFirstToSecondButton.ToolTipText = GetToolTip(s.ResultsPreviewDuplPair_RenameFirstToSecondButton_ToolTip_Text, HotKeyOptions.Action.CurrentDuplPairRenameFirstToSecond);
            m_renameSecondToFirstButton.ToolTipText = GetToolTip(s.ResultsPreviewDuplPair_RenameSecondToFirstButton_ToolTip_Text, HotKeyOptions.Action.CurrentDuplPairRenameSecondToFirst);
            m_mistakeButton.ToolTipText = GetToolTip(s.ResultsPreviewDefect_MistakeButton_ToolTip_Text, HotKeyOptions.Action.CurrentMistake);
        }

        public void SetCurrentSearchResult(CoreResult currentSearchResult)
        {
            m_currentSearchResult = currentSearchResult;
            m_firstImagePreviewPanel.SetResult(m_currentSearchResult);
            m_secondImagePreviewPanel.SetResult(m_currentSearchResult);
            SetHint(m_currentSearchResult.hint);
            UpdateNextAndPreviousButtonEnabling();
        }

        static private Color MixColors(Color firstColor, int firstWeight, Color secondColor, int secondWeight)
        {
            int A = (firstColor.A * firstWeight + secondColor.A * secondWeight) / (firstWeight + secondWeight);
            int R = (firstColor.R * firstWeight + secondColor.R * secondWeight) / (firstWeight + secondWeight);
            int G = (firstColor.G * firstWeight + secondColor.G * secondWeight) / (firstWeight + secondWeight);
            int B = (firstColor.B * firstWeight + secondColor.B * secondWeight) / (firstWeight + secondWeight);
            return Color.FromArgb(A, R, G, B);
        }

        private void SetHint(CoreDll.HintType hint)
        {
            RemoveHint();
            Color hintColor = MixColors(BackColor, 2, Color.Red, 1);
            switch (hint)
            {
                case CoreDll.HintType.DeleteFirst:
                    m_deleteFirstButton.BackColor = hintColor;
                    break;
                case CoreDll.HintType.DeleteSecond:
                    m_deleteSecondButton.BackColor = hintColor;
                    break;
                case CoreDll.HintType.RenameFirstToSecond:
                    m_renameFirstToSecondButton.BackColor = hintColor;
                    break;
                case CoreDll.HintType.RenameSecondToFirst:
                    m_renameSecondToFirstButton.BackColor = hintColor;
                    break;
            }
        }

        private void RemoveHint()
        {
            m_deleteFirstButton.BackColor = BackColor;
            m_deleteSecondButton.BackColor = BackColor;
            m_renameFirstToSecondButton.BackColor = BackColor;
            m_renameSecondToFirstButton.BackColor = BackColor;
        }

        private void OnButtonClicked(object sender, System.EventArgs e)
        {
            ToolStripButton item = (ToolStripButton)sender;
            CoreDll.LocalActionType action = (CoreDll.LocalActionType)item.Tag;
            m_resultsListView.MakeAction(action, CoreDll.TargetType.Current);
        }

        private void OnImageDoubleClicked(object sender, System.EventArgs e)
        {
            PictureBox pictureBox = (PictureBox)sender;
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = pictureBox.ImageLocation;
            Process.Start(startInfo);
        }

        private void OnOptionsChanged()
        {
            m_mistakeButton.Enabled = m_options.coreOptions.advancedOptions.mistakeDataBase;
        }

        protected override void AddItems(ResultsOptions.ViewMode viewMode)
        {
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_firstImagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Top);
                m_secondImagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Bottom);

                m_imageLayout.Controls.Add(m_firstImagePreviewPanel, 0, 0);
                m_imageLayout.Controls.Add(m_secondImagePreviewPanel, 0, 1);

                m_deleteFirstButton.Image = Resources.Images.Get("DeleteFirstVerticalButton");
                m_deleteSecondButton.Image = Resources.Images.Get("DeleteSecondVerticalButton");
                m_deleteBothButton.Image = Resources.Images.Get("DeleteBothVerticalButton");
                m_renameFirstToSecondButton.Image = Resources.Images.Get("RenameFirstToSecondVerticalButton");
                m_renameSecondToFirstButton.Image = Resources.Images.Get("RenameSecondToFirstVerticalButton");
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_firstImagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Left);
                m_secondImagePreviewPanel.SetPosition(ImagePreviewPanel.Position.Right);

                m_imageLayout.Controls.Add(m_firstImagePreviewPanel, 0, 0);
                m_imageLayout.Controls.Add(m_secondImagePreviewPanel, 1, 0);

                m_deleteFirstButton.Image = Resources.Images.Get("DeleteFirstHorizontalButton");
                m_deleteSecondButton.Image = Resources.Images.Get("DeleteSecondHorizontalButton");
                m_deleteBothButton.Image = Resources.Images.Get("DeleteBothHorizontalButton");
                m_renameFirstToSecondButton.Image = Resources.Images.Get("RenameFirstToSecondHorizontalButton");
                m_renameSecondToFirstButton.Image = Resources.Images.Get("RenameSecondToFirstHorizontalButton");
            }

            m_toolStrip.Items.Add(m_deleteBothButton);
            m_toolStrip.Items.Add(m_renameFirstToSecondButton);
            m_toolStrip.Items.Add(m_deleteFirstButton);
            m_toolStrip.Items.Add(new ToolStripSeparator());
            m_toolStrip.Items.Add(m_previousButton);
            m_toolStrip.Items.Add(m_nextButton);
            m_toolStrip.Items.Add(new ToolStripSeparator());
            m_toolStrip.Items.Add(m_deleteSecondButton);
            m_toolStrip.Items.Add(m_renameSecondToFirstButton);
            m_toolStrip.Items.Add(m_mistakeButton);
        }
    }
}
