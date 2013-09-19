/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar, 2013 Borisov Dmitry.
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
using System.IO;

namespace AntiDupl.NET
{
    /// <summary>
    /// Class output result in ListView.
    /// Класс вывода результатов в ListView.
    /// </summary>
    public class ResultsListView : DataGridView
    {
        public enum ColumnsTypeVertical
        {
            Type,
            Group,
            Difference,
            Defect,
            Transform,
            Hint,
            FileName,
            FileDirectory,
            ImageSize,
            ImageType,
            ImageBlockiness,
            FileSize,
            FileTime,
            Size
        }

        public enum ColumnsTypeHorizontal
        {
            Type,
            Group,
            Difference,
            Defect,
            Transform,
            Hint,
            FirstFileName,
            FirstFileDirectory,
            FirstImageSize,
            FirstImageType,
            FirstImageBlockiness,
            FirstFileSize,
            FirstFileTime,
            SecondFileName,
            SecondFileDirectory,
            SecondImageSize,
            SecondImageType,
            SecondImageBlockiness,
            SecondFileSize,
            SecondFileTime,
            Size
        }

        private MainSplitContainer m_mainSplitContainer;
        private CoreLib m_core;
        private AntiDupl.NET.Options m_options;
        private CoreOptions m_coreOptions;
        private CoreResult[] m_results;
        private ResultsOptions.ViewMode m_viewMode = ResultsOptions.ViewMode.VerticalPairTable;

        private int m_firstSelectedRowIndex = -1;
        private int m_currentRowIndex = -1;
        private int m_rowCountOnPage = 1;
        private bool m_isShiftDown = false;
        private bool m_isControlDown = false;
        private bool m_updateColumnOrder = false;
        private bool m_makeAction = false;

        ContextMenuStrip m_contextMenuStrip;
        ResultRowSetter m_resultRowSetter;

        public ResultsListView(CoreLib core, AntiDupl.NET.Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_coreOptions = coreOptions;
            m_mainSplitContainer = mainSplitContainer;
            m_results = new CoreResult[0];
            m_resultRowSetter = new ResultRowSetter(m_options, this);
            InitializeComponents();
            if(m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_viewMode = ResultsOptions.ViewMode.HorizontalPairTable;
                SetViewMode(ResultsOptions.ViewMode.VerticalPairTable);
            }
            else
            {
                m_viewMode = ResultsOptions.ViewMode.VerticalPairTable;
                SetViewMode(ResultsOptions.ViewMode.HorizontalPairTable);
            }
            SetCurrentRow(0);
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
        }

        private void InitializeComponents()
        {
            AutoSizeRowsMode = DataGridViewAutoSizeRowsMode.None;
            ColumnHeadersVisible = true;
            RowHeadersVisible = false;
            CellBorderStyle = DataGridViewCellBorderStyle.Single;
            SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            AllowUserToResizeRows = false;
            AllowUserToDeleteRows = false;
            AllowUserToOrderColumns = true;
            this.BackgroundColor = Color.White;
            ReadOnly = true;
            ColumnHeadersDefaultCellStyle.WrapMode = DataGridViewTriState.False;
            DoubleBuffered = true;
            Location = new Point(0, 0);
            Dock = DockStyle.Fill;
            m_contextMenuStrip = new ResultsListViewContextMenu(m_core, m_options, m_coreOptions, m_mainSplitContainer);
            m_contextMenuStrip.KeyUp += new KeyEventHandler(OnContextMenuKeyUp);
        }

        public void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Columns[(int)ColumnsTypeVertical.Type].Name = s.ResultsListView_Type_Column_Text;
            Columns[(int)ColumnsTypeVertical.Group].Name = s.ResultsListView_Group_Column_Text;
            Columns[(int)ColumnsTypeVertical.Difference].Name = s.ResultsListView_Difference_Column_Text;
            Columns[(int)ColumnsTypeVertical.Defect].Name = s.ResultsListView_Defect_Column_Text;
            Columns[(int)ColumnsTypeVertical.Transform].Name = s.ResultsListView_Transform_Column_Text;
            Columns[(int)ColumnsTypeVertical.Hint].Name = s.ResultsListView_Hint_Column_Text;
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                Columns[(int)ColumnsTypeVertical.FileName].Name = s.ResultsListView_FileName_Column_Text;
                Columns[(int)ColumnsTypeVertical.FileDirectory].Name = s.ResultsListView_FileDirectory_Column_Text;
                Columns[(int)ColumnsTypeVertical.ImageSize].Name = s.ResultsListView_ImageSize_Column_Text;
                Columns[(int)ColumnsTypeVertical.ImageType].Name = s.ResultsListView_ImageType_Column_Text;
                Columns[(int)ColumnsTypeVertical.ImageBlockiness].Name = s.ResultsListView_ImageBlockiness_Column_Text;
                Columns[(int)ColumnsTypeVertical.FileSize].Name = s.ResultsListView_FileSize_Column_Text;
                Columns[(int)ColumnsTypeVertical.FileTime].Name = s.ResultsListView_FileTime_Column_Text;
            }
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                Columns[(int)ColumnsTypeHorizontal.FirstFileName].Name = s.ResultsListView_FirstFileName_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstFileDirectory].Name = s.ResultsListView_FirstFileDirectory_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstImageSize].Name = s.ResultsListView_FirstImageSize_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstImageType].Name = s.ResultsListView_FirstImageType_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstImageBlockiness].Name = s.ResultsListView_ImageBlockiness_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstFileSize].Name = s.ResultsListView_FirstFileSize_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.FirstFileTime].Name = s.ResultsListView_FirstFileTime_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondFileName].Name = s.ResultsListView_SecondFileName_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondFileDirectory].Name = s.ResultsListView_SecondFileDirectory_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondImageSize].Name = s.ResultsListView_SecondImageSize_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondImageType].Name = s.ResultsListView_SecondImageType_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondImageBlockiness].Name = s.ResultsListView_ImageBlockiness_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondFileSize].Name = s.ResultsListView_SecondFileSize_Column_Text;
                Columns[(int)ColumnsTypeHorizontal.SecondFileTime].Name = s.ResultsListView_SecondFileTime_Column_Text;
            }

            m_mainSplitContainer.UpdateResults();
        }

        public void UpdateColumnsVisibility()
        {
            m_updateColumnOrder = false;
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                for (int i = 0; i < (int)ColumnsTypeVertical.Size; i++)
                {
                    Columns[i].Visible = m_options.resultsOptions.columnOptionsVertical[i].visible;
                }
            }
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                for (int i = 0; i < (int)ColumnsTypeHorizontal.Size; i++)
                {
                    Columns[i].Visible = m_options.resultsOptions.columnOptionsHorizontal[i].visible;
                }
            }
            m_updateColumnOrder = true;
        }

        public int GetTotalResultCount()
        {
            return m_results.Length;
        }

        public int GetCurrentRowIndex()
        {
            return m_currentRowIndex;
        }

        public int GetSelectedResultCount()
        {
            int selectedResultsCount = 0;
            for (int i = 0; i < Rows.Count; i++)
            {
                DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[i];
                if (row.selected && i >= 0 && i < m_results.Length)
                {
                    selectedResultsCount++;
                }
            }
            return selectedResultsCount;
        }

        public void MakeAction(CoreDll.LocalActionType action, CoreDll.TargetType target)
        {
            m_makeAction = true;
            ProgressForm progressForm = new ProgressForm(action, target, m_core, m_options, m_coreOptions, m_mainSplitContainer);
            progressForm.Execute();
            m_makeAction = false;
        }

        public void RenameCurrent(CoreDll.RenameCurrentType renameCurrentType, string newFileName)
        {
            m_makeAction = true;
            ProgressForm progressForm = new ProgressForm(renameCurrentType, newFileName, m_core, m_options, m_coreOptions, m_mainSplitContainer);
            progressForm.Execute();
            m_makeAction = false;
        }

        private void MakeAction(Keys hotKey)
        {
            if (hotKey == (Keys.Z | Keys.Control) && m_core.CanApply(CoreDll.ActionEnableType.Undo))
            {
                ProgressForm progressForm = new ProgressForm(ProgressForm.Type.Undo, m_core, m_options, m_coreOptions, m_mainSplitContainer);
                progressForm.Execute();
                return;
            }

            if (hotKey == (Keys.Y | Keys.Control) && m_core.CanApply(CoreDll.ActionEnableType.Redo))
            {
                ProgressForm progressForm = new ProgressForm(ProgressForm.Type.Redo, m_core, m_options, m_coreOptions, m_mainSplitContainer);
                progressForm.Execute();
                return;
            }

            if (m_currentRowIndex >= 0 && m_currentRowIndex < m_results.Length)
            {
                if (m_results[m_currentRowIndex].type == CoreDll.ResultType.DefectImage)
                {
                    if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDefectDelete])
                        MakeAction(CoreDll.LocalActionType.DeleteDefect, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentMistake])
                        MakeAction(CoreDll.LocalActionType.Mistake, CoreDll.TargetType.Current);
                    return;
                }
                if (m_results[m_currentRowIndex].type == CoreDll.ResultType.DuplImagePair)
                {
                    if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDuplPairDeleteFirst])
                        MakeAction(CoreDll.LocalActionType.DeleteFirst, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDuplPairDeleteSecond])
                        MakeAction(CoreDll.LocalActionType.DeleteSecond, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDuplPairDeleteBoth])
                        MakeAction(CoreDll.LocalActionType.DeleteBoth, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDuplPairRenameFirstToSecond])
                        MakeAction(CoreDll.LocalActionType.RenameFirstToSecond, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentDuplPairRenameSecondToFirst])
                        MakeAction(CoreDll.LocalActionType.RenameSecondToFirst, CoreDll.TargetType.Current);
                    else if (hotKey == m_options.hotKeyOptions.keys[(int)HotKeyOptions.Action.CurrentMistake])
                        MakeAction(CoreDll.LocalActionType.Mistake, CoreDll.TargetType.Current);
                    return;
                }
            }
        }

        public void UpdateResults()
        {
            GetResults();
            UpdateRows();
            Invalidate();
        }

        public void ClearResults()
        {
            m_results = new CoreResult[0];
            Rows.Clear();
            RowCount = 1;
            m_currentRowIndex = 0;
        }

        private void GetResults()
        {
            uint resultSize = m_core.GetResultSize();
            if (resultSize == 0)
            {
                m_results = new CoreResult[0];
                return;
            }
            m_results = m_core.GetResult(0, resultSize);
        }

        private void UpdateRows()
        {
            if (m_results.Length == 0)
            {
                Rows.Clear();
                RowCount = 1;
                SetCurrentRow(0);
            }
            else
            {
                if (m_results.Length < RowCount - 1000)//rows are removed very slowly!!!
                    Rows.Clear();
                RowCount = m_results.Length;

                bool[] selection = m_core.GetSelection(0, (uint)m_results.Length);

                for (int i = 0; i < Rows.Count; i++)
                {
                    DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[i];
                    row.updated = false;
                    if (selection != null)
                        row.selected = selection[i];
                }
                int current = m_core.GetCurrent();
                if (current != -1)
                {
                    SetCurrentRow(current);
                }
                else
                {
                    SetCurrentRow(0);
                    SetRowSelection(0, 1, true);
                }
            }
        }

        protected override void OnRowPrePaint(DataGridViewRowPrePaintEventArgs e)
        {
            int index = e.RowIndex;
            DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[index];
            if (!row.updated && index >= 0 && index < m_results.Length)
            {
                m_resultRowSetter.Set(m_results[index], row);
                row.updated = true;
            }
            row.current = (index == m_currentRowIndex);
            base.OnRowPrePaint(e);
        }

        protected override void OnColumnHeaderMouseClick(DataGridViewCellMouseEventArgs e)
        {
            DataGridViewColumn dataGridViewColumn = Columns[e.ColumnIndex];

            CoreDll.SortType sortType = CoreDll.SortType.ByType;
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                switch ((ColumnsTypeVertical)e.ColumnIndex)
                {
                    case ColumnsTypeVertical.Type:
                        sortType = CoreDll.SortType.ByType;
                        break;
                    case ColumnsTypeVertical.Group:
                        sortType = CoreDll.SortType.ByGroup;
                        break;
                    case ColumnsTypeVertical.Difference:
                        sortType = CoreDll.SortType.ByDifference;
                        break;
                    case ColumnsTypeVertical.Defect:
                        sortType = CoreDll.SortType.ByDefect;
                        break;
                    case ColumnsTypeVertical.Transform:
                        sortType = CoreDll.SortType.ByTransform;
                        break;
                    case ColumnsTypeVertical.Hint:
                        sortType = CoreDll.SortType.ByHint;
                        break;
                    case ColumnsTypeVertical.FileName:
                        sortType = CoreDll.SortType.BySortedName;
                        break;
                    case ColumnsTypeVertical.FileDirectory:
                        sortType = CoreDll.SortType.BySortedDirectory;
                        break;
                    case ColumnsTypeVertical.ImageSize:
                        sortType = CoreDll.SortType.BySortedArea;
                        break;
                    case ColumnsTypeVertical.ImageType:
                        sortType = CoreDll.SortType.BySortedType;
                        break;
                    case ColumnsTypeVertical.ImageBlockiness:
                        sortType = CoreDll.SortType.BySortedBlockiness;
                        break;
                    case ColumnsTypeVertical.FileSize:
                        sortType = CoreDll.SortType.BySortedSize;
                        break;
                    case ColumnsTypeVertical.FileTime:
                        sortType = CoreDll.SortType.BySortedTime;
                        break;
                }
            }
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                switch ((ColumnsTypeHorizontal)e.ColumnIndex)
                {
                    case ColumnsTypeHorizontal.Type:
                        sortType = CoreDll.SortType.ByType;
                        break;
                    case ColumnsTypeHorizontal.Group:
                        sortType = CoreDll.SortType.ByGroup;
                        break;
                    case ColumnsTypeHorizontal.Difference:
                        sortType = CoreDll.SortType.ByDifference;
                        break;
                    case ColumnsTypeHorizontal.Defect:
                        sortType = CoreDll.SortType.ByDefect;
                        break;
                    case ColumnsTypeHorizontal.Transform:
                        sortType = CoreDll.SortType.ByTransform;
                        break;
                    case ColumnsTypeHorizontal.Hint:
                        sortType = CoreDll.SortType.ByHint;
                        break;
                    case ColumnsTypeHorizontal.FirstFileName:
                        sortType = CoreDll.SortType.ByFirstName;
                        break;
                    case ColumnsTypeHorizontal.FirstFileDirectory:
                        sortType = CoreDll.SortType.ByFirstDirectory;
                        break;
                    case ColumnsTypeHorizontal.FirstImageSize:
                        sortType = CoreDll.SortType.ByFirstArea;
                        break;
                    case ColumnsTypeHorizontal.FirstImageType:
                        sortType = CoreDll.SortType.ByFirstType;
                        break;
                    case ColumnsTypeHorizontal.FirstFileSize:
                        sortType = CoreDll.SortType.ByFirstSize;
                        break;
                    case ColumnsTypeHorizontal.FirstImageBlockiness:
                        sortType = CoreDll.SortType.ByFirstBlockiness;
                        break;
                    case ColumnsTypeHorizontal.FirstFileTime:
                        sortType = CoreDll.SortType.ByFirstTime;
                        break;
                    case ColumnsTypeHorizontal.SecondFileName:
                        sortType = CoreDll.SortType.BySecondName;
                        break;
                    case ColumnsTypeHorizontal.SecondFileDirectory:
                        sortType = CoreDll.SortType.BySecondDirectory;
                        break;
                    case ColumnsTypeHorizontal.SecondImageSize:
                        sortType = CoreDll.SortType.BySecondArea;
                        break;
                    case ColumnsTypeHorizontal.SecondImageType:
                        sortType = CoreDll.SortType.BySecondType;
                        break;
                    case ColumnsTypeHorizontal.SecondFileSize:
                        sortType = CoreDll.SortType.BySecondSize;
                        break;
                    case ColumnsTypeHorizontal.SecondImageBlockiness:
                        sortType = CoreDll.SortType.BySecondBlockiness;
                        break;
                    case ColumnsTypeHorizontal.SecondFileTime:
                        sortType = CoreDll.SortType.BySecondTime;
                        break;
                }
            }

            ListSortDirection direction = ListSortDirection.Ascending;
            switch (dataGridViewColumn.HeaderCell.SortGlyphDirection)
            {
                case SortOrder.None:
                    direction = ListSortDirection.Ascending;
                    dataGridViewColumn.HeaderCell.SortGlyphDirection = SortOrder.Ascending;
                    break;
                case SortOrder.Ascending:
                    direction = ListSortDirection.Descending;
                    dataGridViewColumn.HeaderCell.SortGlyphDirection = SortOrder.Descending;
                    break;
                case SortOrder.Descending:
                    direction = ListSortDirection.Ascending;
                    dataGridViewColumn.HeaderCell.SortGlyphDirection = SortOrder.Ascending;
                    break;
            }

            for (int col = 0; col < ColumnCount; col++)
                if (col != e.ColumnIndex)
                    Columns[col].HeaderCell.SortGlyphDirection = SortOrder.None;

            m_options.resultsOptions.sortTypeDefault = (int)sortType;
            m_options.resultsOptions.increasingDefault = direction == ListSortDirection.Ascending;
            m_core.SortResult(sortType, direction == ListSortDirection.Ascending);

            m_mainSplitContainer.UpdateResults();
        }

        protected override void OnColumnWidthChanged(DataGridViewColumnEventArgs e)
        {
            base.OnColumnWidthChanged(e);
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_options.resultsOptions.columnOptionsVertical[e.Column.Index].width = Columns[e.Column.Index].Width;
            }
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_options.resultsOptions.columnOptionsHorizontal[e.Column.Index].width = Columns[e.Column.Index].Width;
            }
        }

        protected override void OnColumnDisplayIndexChanged(DataGridViewColumnEventArgs e)
        {
            base.OnColumnDisplayIndexChanged(e);
            if (m_updateColumnOrder)
            {
                if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
                {
                    m_options.resultsOptions.columnOptionsVertical[e.Column.Index].order = Columns[e.Column.Index].DisplayIndex;
                }
                if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
                {
                    m_options.resultsOptions.columnOptionsHorizontal[e.Column.Index].order = Columns[e.Column.Index].DisplayIndex;
                }
            }
        }

        protected override void OnResize(EventArgs e)
        {
            InvalidateRow(m_currentRowIndex);
            base.OnResize(e);
            m_rowCountOnPage = Math.Max(1, ClientSize.Height / RowTemplate.Height - 1);
        }

        protected override void OnScroll(ScrollEventArgs e)
        {
            InvalidateRow(m_currentRowIndex);
            base.OnScroll(e);
        }

        private void OnContextMenuKeyUp(object sender, KeyEventArgs e)
        {
            m_isShiftDown = e.Shift;
            m_isControlDown = e.Control;
        }

        protected override void OnKeyUp(KeyEventArgs e)
        {
            m_isShiftDown = e.Shift;
            m_isControlDown = e.Control;
            base.OnKeyUp(e);
        }

        public void SetKeyDownEvent(KeyEventArgs e)
        {
            OnKeyDown(e);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            m_isShiftDown = e.Shift;
            m_isControlDown = e.Control;
            if (m_isShiftDown)
            {
                if (m_firstSelectedRowIndex == -1)
                    m_firstSelectedRowIndex = m_currentRowIndex;
            }
            base.OnKeyDown(e);
            MakeAction(e.KeyData);
        }

        protected override void OnCellMouseDown(DataGridViewCellMouseEventArgs e)
        {
            if (e.RowIndex >= 0 && e.RowIndex < Rows.Count)
            {
                if (e.Button == MouseButtons.Left)
                {
                    SetCurrentRow(e.RowIndex);
                    SetRowSelection(false);
                    Invalidate();
                }
                else if (e.Button == MouseButtons.Right)
                {
                    DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[e.RowIndex];
                    if (!m_isControlDown && !row.selected)
                    {
                        SetCurrentRow(e.RowIndex);
                        SetRowSelection(false);
                        Invalidate();
                    }
                    ContextMenuStrip = m_core.CanApply(CoreDll.ActionEnableType.Any) ? m_contextMenuStrip : null;
                }
            }
            else
            {
                ContextMenuStrip = null;
            }
            base.OnCellMouseDown(e);
        }

        protected override bool ProcessDataGridViewKey(KeyEventArgs e)
        {
            Keys keyData = e.KeyData;
            if (e.KeyCode == Keys.Up)
            {
                SetCurrentRow(Math.Max(0, m_currentRowIndex - 1));
                keyData = (e.Shift ? Keys.Shift : 0) | e.KeyCode;
            }
            else if (e.KeyCode == Keys.Down)
            {
                SetCurrentRow(Math.Min(m_results.Length - 1, m_currentRowIndex + 1));
                keyData = (e.Shift ? Keys.Shift : 0) | e.KeyCode;
            }
            else if (e.KeyCode == Keys.PageUp)
            {
                SetCurrentRow(Math.Max(0, m_currentRowIndex - m_rowCountOnPage));
            }
            else if (e.KeyCode == Keys.PageDown)
            {
                SetCurrentRow(Math.Min(m_results.Length - 1, m_currentRowIndex + m_rowCountOnPage));
            }
            else if (e.KeyCode == Keys.Home)
            {
                SetCurrentRow(0);
                keyData = Keys.Up | Keys.Control;
            }
            else if (e.KeyCode == Keys.End)
            {
                SetCurrentRow(m_results.Length - 1);
                keyData = Keys.Down | Keys.Control;
            }

            if (e.KeyCode == Keys.A && m_isControlDown)
            {
                SetRowSelection(true);
            }
            else
            {
                if (!m_isControlDown || m_isShiftDown)
                {
                    SetRowSelection(false);
                }
            }
            Invalidate();
            return base.ProcessDataGridViewKey(new KeyEventArgs(keyData));
        }

        private void SetCurrentRow(int index)
        {
            if (index >= 0 && index < Rows.Count)
            {
                if (m_currentRowIndex >= 0 && m_currentRowIndex < Rows.Count)
                    ((DataGridViewCustomRow)Rows[m_currentRowIndex]).current = false;
                if (m_results.Length > 0)
                {
                    ((DataGridViewCustomRow)Rows[index]).current = true;
                    m_core.SetCurrent(index);
                }
                else
                {
                    m_core.SetCurrent(-1);
                }
                m_currentRowIndex = index;

                if (m_firstSelectedRowIndex >= Rows.Count)
                    m_firstSelectedRowIndex = m_currentRowIndex;

                if (m_makeAction)
                {
                    for (int col = 0; col < Rows[index].Cells.Count; col++)
                    {
                        if (Rows[index].Cells[col].Visible)
                        {
                            CurrentCell = Rows[index].Cells[col];
                            break;
                        }
                    }
                }
                m_mainSplitContainer.CurrentResultChanged();
            }
        }

        public void SetRowSelection(bool isControlA)
        {
            if (isControlA)
            {
                SetRowSelection(0, Rows.Count, true);
            }
            else
            {
                if (m_currentRowIndex != -1)
                {
                    if (m_isShiftDown)
                    {
                        if (m_firstSelectedRowIndex == -1 || m_firstSelectedRowIndex >= Rows.Count)
                            m_firstSelectedRowIndex = m_currentRowIndex;

                        int selectionBegin = Math.Min(m_firstSelectedRowIndex, m_currentRowIndex);
                        int selectionEnd = Math.Max(m_firstSelectedRowIndex, m_currentRowIndex);
                        SetRowSelection(0, selectionBegin, false);
                        SetRowSelection(selectionBegin, selectionEnd + 1, true);
                        SetRowSelection(selectionEnd + 1, Rows.Count, false);
                    }
                    else
                    {
                        m_firstSelectedRowIndex = -1;
                        if (!m_isControlDown)
                        {
                            SetRowSelection(0, Rows.Count, false);
                        }
                        bool value = !((DataGridViewCustomRow)Rows[m_currentRowIndex]).selected;
                        SetRowSelection(m_currentRowIndex, m_currentRowIndex + 1, value);
                    }
                }
            }
            m_mainSplitContainer.SelectedResultsChanged();
        }

        private void SetRowSelection(int beginRowIndex, int endRowIndex, bool value)
        {
            for (int i = beginRowIndex; i < endRowIndex; i++)
            {
                DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[i];
                row.selected = value;
            }
            m_core.SetSelection((UInt32)beginRowIndex, (UInt32)(endRowIndex - beginRowIndex), value);
        }

        protected override void OnVisibleChanged(EventArgs e)
        {
            base.OnVisibleChanged(e);
            if (CanFocus)
            {
                Focus();
            }
        }

        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            if (m_viewMode != viewMode)
            {
                m_viewMode = viewMode;
                m_updateColumnOrder = false;
                SetColumns(viewMode);
                UpdateStrings();
                UpdateColumnsVisibility();
                ((ResultsListViewContextMenu)m_contextMenuStrip).SetViewMode(viewMode);
                m_updateColumnOrder = true;
            }
        }

        private void SetColumns(ResultsOptions.ViewMode viewMode)
        {
            Rows.Clear();
            RowCount = 1;
            if (viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                ColumnCount = (int)ColumnsTypeVertical.Size;
                for (int i = 0; i < (int)ColumnsTypeVertical.Size; i++)
                {
                    Columns[i].Name = ((ColumnsTypeVertical)i).ToString();
                    Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                    Columns[i].Width = m_options.resultsOptions.columnOptionsVertical[i].width;
                    Columns[i].DisplayIndex = m_options.resultsOptions.columnOptionsVertical[i].order;
                }
                Rows[0].Cells[0] = new DataGridViewDoubleTextBoxCell("0", "0");
            }
            if (viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                ColumnCount = (int)ColumnsTypeHorizontal.Size;
                for (int i = 0; i < (int)ColumnsTypeHorizontal.Size; i++)
                {
                    Columns[i].Name = ((ColumnsTypeHorizontal)i).ToString();
                    Columns[i].SortMode = DataGridViewColumnSortMode.Programmatic;
                    Columns[i].Width = m_options.resultsOptions.columnOptionsHorizontal[i].width;
                    Columns[i].DisplayIndex = m_options.resultsOptions.columnOptionsHorizontal[i].order;
                }
                Rows[0].Cells[0] = new DataGridViewTextBoxCell();
                Rows[0].Cells[0].Value = "0";
            }
            RowTemplate = new DataGridViewCustomRow();
            RowTemplate.Height = Rows[0].Cells[0].PreferredSize.Height;
            Rows.Clear();
            UpdateRows();
        }

        public override DataObject GetClipboardContent()
        {
            DataObject dataObject = new DataObject();
            if (m_results.Length > 0)
            {
                ClipboardContentBuilder builder = new ClipboardContentBuilder(m_options.resultsOptions);
                for (int i = 0; i < Rows.Count; i++)
                {
                    DataGridViewCustomRow row = (DataGridViewCustomRow)Rows[i];
                    if (row.selected)
                        builder.Add(m_results[i]);
                }
                dataObject.SetText(builder.ToString());
            }
            return dataObject;
        }

        public CoreResult GetCurrentResult()
        {
            if (m_currentRowIndex < m_results.Length && m_currentRowIndex >= 0)
                return m_results[m_currentRowIndex];
            return null;
        }
    }
}
