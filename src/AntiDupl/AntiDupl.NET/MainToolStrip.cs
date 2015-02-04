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
    /// <summary>
    /// Панель инструментов.
    /// </summary>
    public class MainToolStrip : ToolStrip
    {
        private CoreLib m_core;
        private Options m_options;
        private CoreOptions m_coreOptions;
        private MainMenu m_mainMenu;
        private MainForm m_mainForm;
        private MainSplitContainer m_mainSplitContainer;

        private ToolStripButton m_profileOpenButton;
        private ToolStripButton m_profileSaveAsButton;

        private ToolStripButton m_startButton;
        private ToolStripButton m_refreshButton;
        private ToolStripButton m_pathsButton;
        private ToolStripButton m_optionsButton;
        private ToolStripComboBox m_thresholdDifferenceComboBox;
        private ToolStripComboBox m_algorithmComparingComboBox;

        private ToolStripButton m_undoButton;
        private ToolStripButton m_redoButton;

        private ToolStripButton m_mistakeButton;
        private ToolStripButton m_performHintButton;
        private ToolStripButton m_deleteFirstButton;
        private ToolStripButton m_deleteSecondButton;
        private ToolStripButton m_deleteBothButton;
        private ToolStripButton m_deleteDefectButton;

        private ToolStripButton m_helpButton;

        public MainToolStrip(CoreLib core, Options options, CoreOptions coreOptions, MainMenu mainMenu, MainForm mainForm, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_coreOptions = coreOptions;
            m_mainMenu = mainMenu;
            m_mainForm = mainForm;
            m_mainSplitContainer = mainSplitContainer;
            InitializeComponents();
            UpdateStrings();
            UpdateResults();
            OnOptionsChanged();
            SetViewMode(m_options.resultsOptions.viewMode);
        }

        private void InitializeComponents()
        {
            Visible = m_options.mainFormOptions.toolStripView;
            RenderMode = ToolStripRenderMode.System;
            BackColor = SystemColors.Control;
            GripStyle = ToolStripGripStyle.Hidden;

            m_profileOpenButton = InitFactory.ToolButton.Create("ProfileOpenButton", null, m_mainMenu.ProfileOpenAction);
            m_profileSaveAsButton = InitFactory.ToolButton.Create("ProfileSaveAsButton", null, m_mainMenu.ProfileSaveAsAction);

            m_startButton = InitFactory.ToolButton.Create("StartButton", null, m_mainMenu.StartSearchAction);
            m_refreshButton = InitFactory.ToolButton.Create("RefreshButton", null, m_mainMenu.RefreshResultsAction);
            m_pathsButton = InitFactory.ToolButton.Create("PathsButton", null, m_mainMenu.PathsAction);
            m_optionsButton = InitFactory.ToolButton.Create("OptionsButton", null, m_mainMenu.OptionsAction);
            
            m_thresholdDifferenceComboBox = new ToolStripComboBox();
            m_thresholdDifferenceComboBox.AutoSize = false;
            m_thresholdDifferenceComboBox.Size = new System.Drawing.Size(50, 20);
            m_thresholdDifferenceComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            m_thresholdDifferenceComboBox.FlatStyle = FlatStyle.Popup;
            m_thresholdDifferenceComboBox.SelectedIndexChanged += new EventHandler(OnThresholdDifferenceChanged);
            if (m_coreOptions.compareOptions.algorithmComparing == CoreDll.AlgorithmComparing.SquaredSum)
                for (int i = 0; i <= CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM; i++)
                    m_thresholdDifferenceComboBox.Items.Add(string.Format("{0} %", i));
            else
                for (int i = 0; i <= CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SSIM; i++)
                    m_thresholdDifferenceComboBox.Items.Add(string.Format("{0} %", i));

            m_algorithmComparingComboBox = new ToolStripComboBox();
            m_algorithmComparingComboBox.AutoSize = false;
            m_algorithmComparingComboBox.Size = new System.Drawing.Size(70, 20);
            m_algorithmComparingComboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            m_algorithmComparingComboBox.FlatStyle = FlatStyle.Popup;
            m_algorithmComparingComboBox.SelectedIndexChanged += new EventHandler(OnAlgorithmComparingChanged);
            m_algorithmComparingComboBox.Items.Add(Resources.Strings.Current.CoreOptionsForm_AlgorithmComparingLabeledComboBox_SquaredSum);
            m_algorithmComparingComboBox.Items.Add("SSIM");

            m_undoButton = InitFactory.ToolButton.Create("UndoButton", null, m_mainMenu.UndoAction);
            m_redoButton = InitFactory.ToolButton.Create("RedoButton", null, m_mainMenu.RedoAction);

            m_mistakeButton = InitFactory.ToolButton.Create("MistakesButton", CoreDll.LocalActionType.Mistake, MakeAction);
            m_performHintButton = InitFactory.ToolButton.Create("PerformHintButton", CoreDll.LocalActionType.PerformHint, MakeAction);

            m_deleteFirstButton = InitFactory.ToolButton.Create("DeleteFirstsVerticalButton", CoreDll.LocalActionType.DeleteFirst, MakeAction);
            m_deleteSecondButton = InitFactory.ToolButton.Create("DeleteSecondsVerticalButton", CoreDll.LocalActionType.DeleteSecond, MakeAction);
            m_deleteBothButton = InitFactory.ToolButton.Create("DeleteBothesVerticalButton", CoreDll.LocalActionType.DeleteBoth, MakeAction);

            m_deleteDefectButton = InitFactory.ToolButton.Create("DeleteDefectsVerticalButton", CoreDll.LocalActionType.DeleteDefect, MakeAction);

            m_helpButton = InitFactory.ToolButton.Create("HelpButton", null, m_mainMenu.HelpAction);

            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            m_options.mainFormOptions.OnToolStripVisibleChange += new Options.VisibleChangeHandler(OnVisibleChanged);
            m_options.OnChange += new Options.ChangeHandler(OnOptionsChanged);
            m_mainSplitContainer.OnUpdateResults += new MainSplitContainer.UpdateResultsHandler(UpdateResults);
            m_mainSplitContainer.OnSelectedResultsChanged += new MainSplitContainer.SelectedResultsChangedHandler(OnSelectedResultsChanged);
            m_options.resultsOptions.OnViewModeChange += new ResultsOptions.ViewModeChangeHandler(SetViewMode);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_profileOpenButton.ToolTipText = s.MainMenu_File_OpenProfileMenuItem_Text;
            m_profileSaveAsButton.ToolTipText = s.MainMenu_File_SaveProfileAsMenuItem_Text;

            m_startButton.ToolTipText = s.MainMenu_Search_StartMenuItem_Text;
            m_refreshButton.ToolTipText = s.MainMenu_Search_RefreshResultsMenuItem_Text;
            m_pathsButton.ToolTipText = s.MainMenu_Search_PathsMenuItem_Text;
            m_optionsButton.ToolTipText = s.MainMenu_Search_OptionsMenuItem_Text;
            m_thresholdDifferenceComboBox.ToolTipText = s.CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text;
            m_algorithmComparingComboBox.ToolTipText = s.CoreOptionsForm_AlgorithmComparingLabeledComboBox_Text;

            m_undoButton.ToolTipText = s.MainMenu_Edit_UndoMenuItem_Text;
            m_redoButton.ToolTipText = s.MainMenu_Edit_RedoMenuItem_Text;

            m_mistakeButton.ToolTipText = s.ResultsListViewContextMenu_MistakeItem_Text;
            m_performHintButton.ToolTipText = s.ResultsListViewContextMenu_PerformHintItem_Text;
            m_deleteFirstButton.ToolTipText = s.ResultsListViewContextMenu_DeleteFirstItem_Text;
            m_deleteSecondButton.ToolTipText = s.ResultsListViewContextMenu_DeleteSecondItem_Text;
            m_deleteBothButton.ToolTipText = s.ResultsListViewContextMenu_DeleteBothItem_Text;
            m_deleteDefectButton.ToolTipText = s.ResultsListViewContextMenu_DeleteDefectItem_Text;

            m_helpButton.ToolTipText = s.MainMenu_Help_HelpMenuItem_Text;
        }

        private void OnVisibleChanged(bool visible)
        {
            Visible = visible;
            m_mainForm.Controls.SetChildIndex(this, 1);
        }

        private void UpdateResults()
        {
            OnSelectedResultsChanged();
        }

        private void MakeAction(object sender, EventArgs e)
        {
            ToolStripItem item = (ToolStripItem)sender;
            CoreDll.LocalActionType action = (CoreDll.LocalActionType)item.Tag;
            m_mainSplitContainer.resultsListView.MakeAction(action, CoreDll.TargetType.Selected);
        }
        
        private void OnSelectedResultsChanged()
        {
            bool mistake = m_core.CanApply(CoreDll.ActionEnableType.Any);
            bool performHint = m_core.CanApply(CoreDll.ActionEnableType.PerformHint);
            bool duplPair = m_core.CanApply(CoreDll.ActionEnableType.DuplPair);
            bool defect = m_core.CanApply(CoreDll.ActionEnableType.Defect);

            m_mistakeButton.Enabled = mistake;
            m_performHintButton.Enabled = performHint;
            m_deleteFirstButton.Enabled = duplPair;
            m_deleteSecondButton.Enabled = duplPair;
            m_deleteBothButton.Enabled = duplPair;
            m_deleteDefectButton.Enabled = defect;

            m_undoButton.Enabled = m_core.CanApply(CoreDll.ActionEnableType.Undo);
            m_redoButton.Enabled = m_core.CanApply(CoreDll.ActionEnableType.Redo);
        }

        /// <summary>
        /// Вызывается при изменение опций CoreOptions.
        /// </summary>
        private void OnOptionsChanged()
        {
            m_mistakeButton.Enabled = m_coreOptions.advancedOptions.mistakeDataBase &&
                 m_core.CanApply(CoreDll.ActionEnableType.Any);

            m_algorithmComparingComboBox.SelectedIndex = (int)m_coreOptions.compareOptions.algorithmComparing;
            if (m_coreOptions.compareOptions.algorithmComparing == CoreDll.AlgorithmComparing.SquaredSum &&
                m_thresholdDifferenceComboBox.Items.Count > CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM + 1)
            {
                m_thresholdDifferenceComboBox.Items.Clear();
                for (int i = 0; i <= CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM; i++)
                    m_thresholdDifferenceComboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
            }
            if (m_coreOptions.compareOptions.algorithmComparing == CoreDll.AlgorithmComparing.SSIM &&
                 m_thresholdDifferenceComboBox.Items.Count < CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SSIM + 1)
            {
                m_thresholdDifferenceComboBox.Items.Clear();
                for (int i = 0; i <= CoreOptionsForm.THRESHOLD_DIFFERENCE_MAX_SSIM; i++)
                    m_thresholdDifferenceComboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
            }
            m_thresholdDifferenceComboBox.SelectedIndex = m_coreOptions.compareOptions.thresholdDifference;
        }

        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            Items.Clear();

            Items.Add(m_profileOpenButton);
            Items.Add(m_profileSaveAsButton);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_startButton);
            Items.Add(m_refreshButton);
            Items.Add(m_pathsButton);
            Items.Add(m_optionsButton);
            Items.Add(m_thresholdDifferenceComboBox);
            Items.Add(m_algorithmComparingComboBox);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_undoButton);
            Items.Add(m_redoButton);
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable || viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                Items.Add(new ToolStripSeparator());
                Items.Add(m_mistakeButton);
                Items.Add(m_performHintButton);
                Items.Add(new ToolStripSeparator());
                Items.Add(m_deleteFirstButton);
                Items.Add(m_deleteSecondButton);
                Items.Add(m_deleteBothButton);
                Items.Add(new ToolStripSeparator());
                Items.Add(m_deleteDefectButton);
            }
            Items.Add(new ToolStripSeparator());
            Items.Add(m_helpButton);

            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_deleteDefectButton.Image = Resources.Images.Get("DeleteDefectsVerticalButton");
                m_deleteFirstButton.Image = Resources.Images.Get("DeleteFirstsVerticalButton");
                m_deleteSecondButton.Image = Resources.Images.Get("DeleteSecondsVerticalButton");
                m_deleteBothButton.Image = Resources.Images.Get("DeleteBothesVerticalButton");
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_deleteDefectButton.Image = Resources.Images.Get("DeleteDefectsHorizontalButton");
                m_deleteFirstButton.Image = Resources.Images.Get("DeleteFirstsHorizontalButton");
                m_deleteSecondButton.Image = Resources.Images.Get("DeleteSecondsHorizontalButton");
                m_deleteBothButton.Image = Resources.Images.Get("DeleteBothesHorizontalButton");
            }
        }

        private void OnThresholdDifferenceChanged(object sender, EventArgs e)
        {
            m_coreOptions.compareOptions.thresholdDifference = m_thresholdDifferenceComboBox.SelectedIndex;
        }

        private void OnAlgorithmComparingChanged(object sender, EventArgs e)
        {
            m_coreOptions.compareOptions.algorithmComparing = (CoreDll.AlgorithmComparing)m_algorithmComparingComboBox.SelectedIndex;
        }
    }
}
