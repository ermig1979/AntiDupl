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
using System.IO;
using System.Diagnostics;

namespace AntiDupl.NET
{
    public class MainMenu : MenuStrip
    {
        CoreLib m_core;
        private Options m_options;

        private MainForm m_mainForm;
        private MainSplitContainer m_mainSplitContainer;

        private ToolStripMenuItem m_fileMenuItem;
        private ToolStripMenuItem m_file_saveMenuItem;
        private ToolStripMenuItem m_file_loadMenuItem;
        private ToolStripMenuItem m_file_exitMenuItem;

        private ToolStripMenuItem m_editMenuItem;
        private ToolStripMenuItem m_edit_undoMenuItem;
        private ToolStripMenuItem m_edit_redoMenuItem;
        private ToolStripMenuItem m_edit_selectAllMenuItem;

        private ToolStripMenuItem m_viewMenuItem;
        private ToolStripMenuItem m_view_toolMenuItem;
        private ToolStripMenuItem m_view_statusMenuItem;
        private ToolStripMenuItem m_view_languageMenuItem;
        private ToolStripMenuItem m_view_modeMenuItem;
        private ToolStripMenuItem m_view_selectColumnsMenuItem;
        private ToolStripMenuItem m_view_hotKeysMenuItem;
        private ToolStripMenuItem m_view_stretchSmallImageMenuItem;
        private ToolStripMenuItem m_view_proportionalImageViewMenuItem;

        private ToolStripMenuItem m_searchMenuItem;
        private ToolStripMenuItem m_search_startMenuItem;
        private ToolStripMenuItem m_search_refreshResultsMenuItem;
        private ToolStripMenuItem m_search_refreshImagesMenuItem;
        private ToolStripMenuItem m_search_pathsMenuItem;
        private ToolStripMenuItem m_search_optionsMenuItem;
        private ToolStripMenuItem m_search_onePathMenuItem;
        private ToolStripMenuItem m_search_useImageDataBaseMenuItem;
        private ToolStripMenuItem m_search_checkResultsAtLoadingMenuItem;

        private ToolStripMenuItem m_helpMenuItem;
        private ToolStripMenuItem m_help_helpMenuItem;
        private ToolStripMenuItem m_help_aboutProgramMenuItem;
        private ToolStripMenuItem m_help_checkingForUpdatesMenuItem;

        private NewVersionMenuItem m_newVersionMenuItem;

        public MainMenu(CoreLib core, Options options, MainForm mainForm, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_mainForm = mainForm;
            m_mainSplitContainer = mainSplitContainer;
            InitializeComponents();
            UpdateStrings();
            UpdateResults();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            m_mainSplitContainer.OnUpdateResults += new MainSplitContainer.UpdateResultsHandler(UpdateResults);
        }

        private void InitializeComponents()
        {
            RenderMode = ToolStripRenderMode.System;
            BackColor = SystemColors.Control;
            ShowItemToolTips = true;

            m_file_exitMenuItem = InitFactory.MenuItem.Create(null, null, ExitAction);
            m_file_saveMenuItem = InitFactory.MenuItem.Create(null, null, SaveResultsAction);
            m_file_loadMenuItem = InitFactory.MenuItem.Create(null, null, LoadResultsAction);

            m_fileMenuItem = new ToolStripMenuItem();
            m_fileMenuItem.DropDownItems.Add(m_file_saveMenuItem);
            m_fileMenuItem.DropDownItems.Add(m_file_loadMenuItem);
            m_fileMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_fileMenuItem.DropDownItems.Add(m_file_exitMenuItem);

            m_edit_undoMenuItem = InitFactory.MenuItem.Create("UndoMenu", null, UndoAction);
            m_edit_redoMenuItem = InitFactory.MenuItem.Create("RedoMenu", null, RedoAction);
            m_edit_selectAllMenuItem = InitFactory.MenuItem.Create(null, null, SelectAllAction);

            m_editMenuItem = new ToolStripMenuItem();
            m_editMenuItem.DropDownItems.Add(m_edit_undoMenuItem);
            m_editMenuItem.DropDownItems.Add(m_edit_redoMenuItem);
            m_editMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_editMenuItem.DropDownItems.Add(m_edit_selectAllMenuItem);

            m_view_toolMenuItem = InitFactory.MenuItem.Create(null, null, ViewItemCheckChangeAction, m_options.mainFormOptions.toolStripView);
            m_view_statusMenuItem = InitFactory.MenuItem.Create(null, null, ViewItemCheckChangeAction, m_options.mainFormOptions.statusStripView);
            m_view_languageMenuItem = new LanguageMenuItem(m_options);
            m_view_modeMenuItem = new ViewModeMenuItem(m_options);
            m_view_selectColumnsMenuItem = InitFactory.MenuItem.Create(null, null, OnSelectColumnsClick);
            m_view_hotKeysMenuItem = InitFactory.MenuItem.Create(null, null, OnHotKeysClick);
            m_view_stretchSmallImageMenuItem = InitFactory.MenuItem.Create(null, null, ViewItemCheckChangeAction, m_options.resultsOptions.StretchSmallImages);
            m_view_proportionalImageViewMenuItem = InitFactory.MenuItem.Create(null, null, ViewItemCheckChangeAction, m_options.resultsOptions.ProportionalImageSize);
 
            m_viewMenuItem = new ToolStripMenuItem();
            m_viewMenuItem.DropDownItems.Add(m_view_toolMenuItem);
            m_viewMenuItem.DropDownItems.Add(m_view_statusMenuItem);
            m_viewMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_viewMenuItem.DropDownItems.Add(m_view_languageMenuItem);
            m_viewMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_viewMenuItem.DropDownItems.Add(m_view_modeMenuItem);
            m_viewMenuItem.DropDownItems.Add(m_view_selectColumnsMenuItem);
            m_viewMenuItem.DropDownItems.Add(m_view_hotKeysMenuItem);
            m_viewMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_viewMenuItem.DropDownItems.Add(m_view_stretchSmallImageMenuItem);
            m_viewMenuItem.DropDownItems.Add(m_view_proportionalImageViewMenuItem);

            m_search_startMenuItem = InitFactory.MenuItem.Create("StartMenu", null, StartSearchAction);
            m_search_refreshResultsMenuItem = InitFactory.MenuItem.Create("RefreshMenu", null, RefreshResultsAction);
            m_search_refreshImagesMenuItem = InitFactory.MenuItem.Create(null, null, RefreshImagesAction);
            m_search_pathsMenuItem = InitFactory.MenuItem.Create("PathsMenu", null, PathsAction);
            m_search_optionsMenuItem = InitFactory.MenuItem.Create("OptionsMenu", null, OptionsAction);
            m_search_onePathMenuItem = InitFactory.MenuItem.Create(null, null, UseOnePathAction, m_options.onePath);
            m_search_useImageDataBaseMenuItem = InitFactory.MenuItem.Create(null, null, UseImageDataBaseAction, m_options.useImageDataBase);
            m_search_checkResultsAtLoadingMenuItem = InitFactory.MenuItem.Create(null, null, CheckResultsAtLoadingAction, m_options.checkResultsAtLoading);

            m_searchMenuItem = new ToolStripMenuItem();
            m_searchMenuItem.DropDownItems.Add(m_search_startMenuItem);
            m_searchMenuItem.DropDownItems.Add(m_search_refreshResultsMenuItem);
            m_searchMenuItem.DropDownItems.Add(m_search_refreshImagesMenuItem);
            m_searchMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_searchMenuItem.DropDownItems.Add(m_search_pathsMenuItem);
            m_searchMenuItem.DropDownItems.Add(m_search_optionsMenuItem);
            m_searchMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_searchMenuItem.DropDownItems.Add(m_search_onePathMenuItem);
            m_searchMenuItem.DropDownItems.Add(m_search_useImageDataBaseMenuItem);
            m_searchMenuItem.DropDownItems.Add(m_search_checkResultsAtLoadingMenuItem);

            m_help_helpMenuItem = InitFactory.MenuItem.Create("HelpMenu", null, HelpAction);
            m_help_aboutProgramMenuItem = InitFactory.MenuItem.Create(null, null, AboutProgramAction);
            m_help_checkingForUpdatesMenuItem = InitFactory.MenuItem.Create(null, null, OnCheckingForUpdatesClick, m_options.checkingForUpdates);

            m_helpMenuItem = new ToolStripMenuItem();
            m_helpMenuItem.DropDownItems.Add(m_help_helpMenuItem);
            m_helpMenuItem.DropDownItems.Add(m_help_aboutProgramMenuItem);
            m_helpMenuItem.DropDownItems.Add(new ToolStripSeparator());
            m_helpMenuItem.DropDownItems.Add(m_help_checkingForUpdatesMenuItem);

            m_newVersionMenuItem = new NewVersionMenuItem(m_options);

            Items.Add(m_fileMenuItem);
            Items.Add(m_editMenuItem);
            Items.Add(m_viewMenuItem);
            Items.Add(m_searchMenuItem);
            Items.Add(m_helpMenuItem);
            Items.Add(m_newVersionMenuItem);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_fileMenuItem.Text = s.MainMenu_FileMenuItem_Text;
            m_file_saveMenuItem.Text = s.MainMenu_File_SaveMenuItem_Text;
            m_file_loadMenuItem.Text = s.MainMenu_File_LoadMenuItem_Text;
            m_file_exitMenuItem.Text = s.MainMenu_File_ExitMenuItem_Text;

            m_editMenuItem.Text = s.MainMenu_EditMenuItem_Text;
            m_edit_undoMenuItem.Text = s.MainMenu_Edit_UndoMenuItem_Text;
            m_edit_redoMenuItem.Text = s.MainMenu_Edit_RedoMenuItem_Text;
            m_edit_selectAllMenuItem.Text = s.MainMenu_Edit_SelectAllMenuItem_Text;

            m_viewMenuItem.Text = s.MainMenu_ViewMenuItem_Text;
            m_view_toolMenuItem.Text = s.MainMenu_View_ToolMenuItem_Text;
            m_view_statusMenuItem.Text = s.MainMenu_View_StatusMenuItem_Text;
            m_view_selectColumnsMenuItem.Text = s.MainMenu_View_SelectColumnsMenuItem_Text;
            m_view_hotKeysMenuItem.Text = s.MainMenu_View_HotKeysMenuItem_Text;
            m_view_stretchSmallImageMenuItem.Text = s.MainMenu_View_StretchSmallImagesMenuItem_Text;
            m_view_proportionalImageViewMenuItem.Text = s.MainMenu_View_ProportionalImageSizeMenuItem_Text;

            m_searchMenuItem.Text = s.MainMenu_SearchMenuItem_Text;
            m_search_startMenuItem.Text = s.MainMenu_Search_StartMenuItem_Text;
            m_search_refreshResultsMenuItem.Text = s.MainMenu_Search_RefreshResultsMenuItem_Text;
            m_search_refreshImagesMenuItem.Text = s.MainMenu_Search_RefreshImagesMenuItem_Text;
            m_search_pathsMenuItem.Text = s.MainMenu_Search_PathsMenuItem_Text;
            m_search_optionsMenuItem.Text = s.MainMenu_Search_OptionsMenuItem_Text;
            m_search_onePathMenuItem.Text = s.MainMenu_Search_OnePathMenuItem_Text;
            m_search_useImageDataBaseMenuItem.Text = s.MainMenu_Search_UseImageDataBaseMenuItem_Text;
            m_search_checkResultsAtLoadingMenuItem.Text = s.MainMenu_Search_CheckResultsAtLoadingMenuItem_Text;
            
            m_helpMenuItem.Text = s.MainMenu_HelpMenuItem_Text;
            m_help_helpMenuItem.Text = s.MainMenu_Help_HelpMenuItem_Text;
            m_help_aboutProgramMenuItem.Text = s.MainMenu_Help_AboutProgramMenuItem_Text;
            m_help_checkingForUpdatesMenuItem.Text = s.MainMenu_Help_CheckingForUpdatesMenuItem_Text;
        }

        private void ViewItemCheckChangeAction(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;
            if (item == m_view_toolMenuItem)
            {
                m_options.mainFormOptions.toolStripView = item.Checked;
            }
            else if (item == m_view_statusMenuItem)
            {
                m_options.mainFormOptions.statusStripView = item.Checked;
            }
            else if (item == m_view_stretchSmallImageMenuItem)
            {
                m_options.resultsOptions.StretchSmallImages = item.Checked;
            }
            else if (item == m_view_proportionalImageViewMenuItem)
            {
                m_options.resultsOptions.ProportionalImageSize = item.Checked;
            }
            m_mainForm.Refresh();
        }

        private void OnSelectColumnsClick(object sender, EventArgs e)
        {
            if(m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                SelectVerticalColumnsForm form =
                    new SelectVerticalColumnsForm(m_mainSplitContainer.resultsListView, m_options);
                form.ShowDialog();
            }
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                SelectHorizontalColumnsForm form =
                    new SelectHorizontalColumnsForm(m_mainSplitContainer.resultsListView, m_options);
                form.ShowDialog();
            }
        }

        private void OnHotKeysClick(object sender, EventArgs e)
        {
            SelectHotKeysForm form = new SelectHotKeysForm(m_options);
            form.ShowDialog();
        }

        public void StartSearchAction(object sender, EventArgs e)
        {
            SearchExecuterForm searchExecuterForm = new SearchExecuterForm(m_core, m_options, m_mainSplitContainer, m_mainForm);
            searchExecuterForm.Execute();
        }

        public void RefreshResultsAction(object sender, EventArgs e)
        {
            ProgressForm progressForm = new ProgressForm(ProgressForm.Type.RefreshResults, m_core, m_options, m_mainSplitContainer);
            progressForm.Execute();
        }

        private void RefreshImagesAction(object sender, EventArgs e)
        {
            ProgressForm progressForm = new ProgressForm(ProgressForm.Type.RefreshImages, m_core, m_options, m_mainSplitContainer);
            progressForm.Execute();
        }

        public void UndoAction(object sender, EventArgs e)
        {
            ProgressForm progressForm = new ProgressForm(ProgressForm.Type.Undo, m_core, m_options, m_mainSplitContainer);
            progressForm.Execute();
        }

        public void RedoAction(object sender, EventArgs e)
        {
            ProgressForm progressForm = new ProgressForm(ProgressForm.Type.Redo, m_core, m_options, m_mainSplitContainer);
            progressForm.Execute();
        }

        public void SelectAllAction(object sender, EventArgs e)
        {
            m_mainSplitContainer.resultsListView.SetRowSelection(true);
            m_mainSplitContainer.resultsListView.Invalidate();
        }
        
        private void SaveResultsAction(object sender, EventArgs e)
        {
            SaveFileDialog dialog = new SaveFileDialog();
            if (File.Exists(m_options.resultsOptions.resultsFileName))
                dialog.FileName = m_options.resultsOptions.resultsFileName;
            else
                dialog.InitialDirectory = Application.StartupPath;
            dialog.OverwritePrompt = false;
            dialog.DefaultExt = "adr";
            dialog.Filter = "Antidupl results files (*.adr)|*.adr";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_options.resultsOptions.resultsFileName = dialog.FileName;
                ProgressForm progressForm = new ProgressForm(ProgressForm.Type.SaveResults, m_core, m_options, m_mainSplitContainer);
                progressForm.Execute();
            }
        }

        private void LoadResultsAction(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            if (File.Exists(m_options.resultsOptions.resultsFileName))
                dialog.FileName = m_options.resultsOptions.resultsFileName;
            else
                dialog.InitialDirectory = Application.StartupPath;
            dialog.DefaultExt = "adr";
            dialog.Filter = "Antidupl results files (*.adr)|*.adr";
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                m_options.resultsOptions.resultsFileName = dialog.FileName;
                ProgressForm progressForm = new ProgressForm(ProgressForm.Type.LoadResults, m_core, m_options, m_mainSplitContainer);
                progressForm.Execute();
            }
        }

        private void ExitAction(object sender, EventArgs e)
        {
            m_mainForm.Close();
        }

        public void PathsAction(object sender, EventArgs e)
        {
            if (m_options.onePath)
            {
                FolderBrowserDialog dialog = new FolderBrowserDialog();
                dialog.ShowNewFolderButton = false;
                if (Directory.Exists(m_options.coreOptions.searchPath[0]))
                    dialog.SelectedPath = m_options.coreOptions.searchPath[0];
                else
                    dialog.SelectedPath = Application.StartupPath;
                dialog.ShowDialog();
                m_options.coreOptions.searchPath[0] = dialog.SelectedPath;
            }
            else
            {
                CorePathsForm form = new CorePathsForm(m_core, m_options);
                form.ShowDialog();
            }
        }

        public void OptionsAction(object sender, EventArgs e)
        {
            CoreOptionsForm form = new CoreOptionsForm(m_core, m_options);
            form.ShowDialog();
        }

        private void UseOnePathAction(object sender, EventArgs e)
        {
            m_options.onePath = m_search_onePathMenuItem.Checked;
        }

        private void UseImageDataBaseAction(object sender, EventArgs e)
        {
            m_options.useImageDataBase = m_search_useImageDataBaseMenuItem.Checked;
        }

        private void CheckResultsAtLoadingAction(object sender, EventArgs e)
        {
            m_options.checkResultsAtLoading = m_search_checkResultsAtLoadingMenuItem.Checked;
        }

        public void HelpAction(object sender, EventArgs e)
        {
            Resources.Help.Show(Resources.Help.Index);
        }

        private void AboutProgramAction(object sender, EventArgs e)
        {
            AboutProgramForm aboutProgramForm = new AboutProgramForm(m_core);
            aboutProgramForm.ShowDialog();
        }

        private void OnCheckingForUpdatesClick(object sender, EventArgs e)
        {
            m_options.checkingForUpdates = m_help_checkingForUpdatesMenuItem.Checked;
            if(m_options.checkingForUpdates)
            {
                m_newVersionMenuItem = new NewVersionMenuItem(m_options);
            }
        }

        private void UpdateResults()
        {
            bool hasResults = m_core.GetResultSize() > 0;

            m_file_saveMenuItem.Enabled = hasResults;

            m_edit_undoMenuItem.Enabled = m_core.CanApply(CoreDll.ActionEnableType.Undo);
            m_edit_redoMenuItem.Enabled = m_core.CanApply(CoreDll.ActionEnableType.Redo);
            m_edit_selectAllMenuItem.Enabled = hasResults;
        }
    }
}
