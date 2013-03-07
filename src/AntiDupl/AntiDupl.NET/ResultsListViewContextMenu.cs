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
using System.ComponentModel;

namespace AntiDupl.NET
{
    public class ResultsListViewContextMenu : ContextMenuStrip
    {
        private CoreLib m_core;
        private Options m_options;
        private MainSplitContainer m_mainSplitContainer;

        private ToolStripMenuItem m_deleteDefectItem;
        private ToolStripMenuItem m_deleteFirstItem;
        private ToolStripMenuItem m_deleteSecondItem;
        private ToolStripMenuItem m_deleteBothItem;
        private ToolStripMenuItem m_performHintItem;
        private ToolStripMenuItem m_mistakeItem;

        public ResultsListViewContextMenu(CoreLib core, Options options, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_mainSplitContainer = mainSplitContainer;
            InitializeComponents();
            UpdateStrings();
            OnOptionsChanged();
        }

        private void InitializeComponents()
        {
            RenderMode = ToolStripRenderMode.System;

            m_deleteDefectItem = InitFactory.MenuItem.Create("DeleteDefectsVerticalMenu", CoreDll.LocalActionType.DeleteDefect, MakeAction);
            m_deleteFirstItem = InitFactory.MenuItem.Create("DeleteFirstsVerticalMenu", CoreDll.LocalActionType.DeleteFirst, MakeAction);
            m_deleteSecondItem = InitFactory.MenuItem.Create("DeleteSecondsVerticalMenu", CoreDll.LocalActionType.DeleteSecond, MakeAction);
            m_deleteBothItem = InitFactory.MenuItem.Create("DeleteBothesVerticalMenu", CoreDll.LocalActionType.DeleteBoth, MakeAction);
            m_performHintItem = InitFactory.MenuItem.Create("PerformHintMenu", CoreDll.LocalActionType.PerformHint, MakeAction);
            m_mistakeItem = InitFactory.MenuItem.Create("MistakesMenu", CoreDll.LocalActionType.Mistake, MakeAction);

            Opening += new CancelEventHandler(OnOpening);
            m_mainSplitContainer.OnUpdateResults += new MainSplitContainer.UpdateResultsHandler(UpdateResults);
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            m_options.OnChange += new Options.ChangeHandler(OnOptionsChanged);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_deleteDefectItem.Text = s.ResultsListViewContextMenu_DeleteDefectItem_Text;
            m_deleteFirstItem.Text = s.ResultsListViewContextMenu_DeleteFirstItem_Text;
            m_deleteSecondItem.Text = s.ResultsListViewContextMenu_DeleteSecondItem_Text;
            m_deleteBothItem.Text = s.ResultsListViewContextMenu_DeleteBothItem_Text;
            m_mistakeItem.Text = s.ResultsListViewContextMenu_MistakeItem_Text;
            m_performHintItem.Text = s.ResultsListViewContextMenu_PerformHintItem_Text;
        }

        private void OnOpening(object sender, EventArgs e)
        {
            Items.Clear();

            if (m_mainSplitContainer.resultsListView.GetTotalResultCount() > 0)
            {
                if (m_core.CanApply(CoreDll.ActionEnableType.Defect))
                {
                    Items.Add(m_deleteDefectItem);
                    Items.Add(new ToolStripSeparator());
                }
                if (m_core.CanApply(CoreDll.ActionEnableType.DuplPair))
                {
                    Items.Add(m_deleteFirstItem);
                    Items.Add(m_deleteSecondItem);
                    Items.Add(m_deleteBothItem);
                    Items.Add(new ToolStripSeparator());
                }
                if (m_core.CanApply(CoreDll.ActionEnableType.PerformHint))
                {
                    Items.Add(m_performHintItem);
                    Items.Add(new ToolStripSeparator());
                }
                if (m_core.CanApply(CoreDll.ActionEnableType.Any))
                {
                    Items.Add(m_mistakeItem);
                }
            }
        }

        private void MakeAction(object sender, EventArgs e)
        {
            ToolStripItem item = (ToolStripItem)sender;
            CoreDll.LocalActionType action = (CoreDll.LocalActionType)item.Tag;
            m_mainSplitContainer.resultsListView.MakeAction(action, CoreDll.TargetType.Selected);
        }

        private void UpdateResults()
        {
            if (m_mainSplitContainer.resultsListView != null && 
                m_mainSplitContainer.resultsListView.GetTotalResultCount() > 0)
            {
                Items.Add(new ToolStripSeparator());
            }
            else
            {
                Items.Clear();
            }
        }

        private void OnOptionsChanged()
        {
            m_mistakeItem.Enabled = m_options.coreOptions.advancedOptions.mistakeDataBase;
        }
        
        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_deleteDefectItem.Image = Resources.Images.Get("DeleteDefectsVerticalMenu");
                m_deleteFirstItem.Image = Resources.Images.Get("DeleteFirstsVerticalMenu");
                m_deleteSecondItem.Image = Resources.Images.Get("DeleteSecondsVerticalMenu");
                m_deleteBothItem.Image = Resources.Images.Get("DeleteBothesVerticalMenu");
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_deleteDefectItem.Image = Resources.Images.Get("DeleteDefectsHorizontalMenu");
                m_deleteFirstItem.Image = Resources.Images.Get("DeleteFirstsHorizontalMenu");
                m_deleteSecondItem.Image = Resources.Images.Get("DeleteSecondsHorizontalMenu");
                m_deleteBothItem.Image = Resources.Images.Get("DeleteBothesHorizontalMenu");
            }
        }
    }
}
