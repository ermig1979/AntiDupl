/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2014 Yermalayeu Ihar, 2013-2014 Borisov Dmitry.
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

namespace AntiDupl.NET
{
    /// <summary>
    /// Set table of out defect and dublicate pair.
    /// ”становка таблицы вывода дефектов и дубликатов.
    /// </summary>
    public class ResultRowSetter
    {
        private AntiDupl.NET.Options m_options;
        private DataGridView m_dataGridView;

        private Image m_nullIcon;

        private Image m_defectIcon;
        
        private Image m_duplPairVerticalIcon;
        private Image m_duplPairHorizontalIcon;

        private Image m_unknownDefectIcon;
        private Image m_jpegEndMarkerIsAbsentIcon;
        private Image m_blockinessIcon;
        private Image m_blurringIcon;

        private Image m_deleteDefectIcon;
        private Image m_deleteFirstVerticalIcon;
        private Image m_deleteFirstHorizontalIcon;
        private Image m_deleteSecondVerticalIcon;
        private Image m_deleteSecondHorizontalIcon;
        private Image m_renameFirstToSecondVerticalIcon;
        private Image m_renameFirstToSecondHorizontalIcon;
        private Image m_renameSecondToFirstVerticalIcon;
        private Image m_renameSecondToFirstHorizontalIcon;

        private Image m_turn_0_Icon;
        private Image m_turn_90_Icon;
        private Image m_turn_180_Icon;
        private Image m_turn_270_Icon;
        private Image m_mirrorTurn_0_Icon;
        private Image m_mirrorTurn_90_Icon;
        private Image m_mirrorTurn_180_Icon;
        private Image m_mirrorTurn_270_Icon;

        private string m_defectIconToolTipText;
        private string m_duplPairIconToolTipText;

        private string m_unknownDefectIconToolTipText;
        private string m_jpegEndMarkerIsAbsentIconToolTipText;
        private string m_blockinessIconToolTipText;
        private string m_blurringIconToolTipText;

        private string m_deleteDefectIconToolTipText;
        private string m_deleteFirstIconToolTipText;
        private string m_deleteSecondIconToolTipText;
        private string m_renameFirstToSecondIconToolTipText;
        private string m_renameSecondToFirstIconToolTipText;

        private string m_turn_0_IconToolTipText;
        private string m_turn_90_IconToolTipText;
        private string m_turn_180_IconToolTipText;
        private string m_turn_270_IconToolTipText;
        private string m_mirrorTurn_0_IconToolTipText;
        private string m_mirrorTurn_90_IconToolTipText;
        private string m_mirrorTurn_180_IconToolTipText;
        private string m_mirrorTurn_270_IconToolTipText;

        public ResultRowSetter(AntiDupl.NET.Options options, DataGridView dataGridView)
        {
            m_options = options;
            m_dataGridView = dataGridView;
            InitializeImages();
            UpdateStrings();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
        }
        
        private void InitializeImages()
        {
            m_nullIcon = Resources.Images.GetNullImage();

            m_defectIcon = Resources.Images.Get("DefectIcon");
            m_duplPairVerticalIcon = Resources.Images.Get("DuplPairVerticalIcon");
            m_duplPairHorizontalIcon = Resources.Images.Get("DuplPairHorizontalIcon");

            m_unknownDefectIcon = Resources.Images.Get("UnknownDefectIcon");
            m_jpegEndMarkerIsAbsentIcon = Resources.Images.Get("JpegEndMarkerIsAbsentIcon");
            m_blockinessIcon = Resources.Images.Get("BlockinessIcon");
            m_blurringIcon = Resources.Images.Get("BlurringIcon");

            m_deleteDefectIcon = Resources.Images.Get("DeleteDefectIcon");
            m_deleteFirstVerticalIcon = Resources.Images.Get("DeleteFirstVerticalIcon");
            m_deleteFirstHorizontalIcon = Resources.Images.Get("DeleteFirstHorizontalIcon");
            m_deleteSecondVerticalIcon = Resources.Images.Get("DeleteSecondVerticalIcon");
            m_deleteSecondHorizontalIcon = Resources.Images.Get("DeleteSecondHorizontalIcon");
            m_renameFirstToSecondVerticalIcon = Resources.Images.Get("RenameFirstToSecondVerticalIcon");
            m_renameFirstToSecondHorizontalIcon = Resources.Images.Get("RenameFirstToSecondHorizontalIcon");
            m_renameSecondToFirstVerticalIcon = Resources.Images.Get("RenameSecondToFirstVerticalIcon");
            m_renameSecondToFirstHorizontalIcon = Resources.Images.Get("RenameSecondToFirstHorizontalIcon");

            m_turn_0_Icon = Resources.Images.Get("Turn_0_Icon");
            m_turn_90_Icon = Resources.Images.Get("Turn_90_Icon");
            m_turn_180_Icon = Resources.Images.Get("Turn_180_Icon");
            m_turn_270_Icon = Resources.Images.Get("Turn_270_Icon");
            m_mirrorTurn_0_Icon = Resources.Images.Get("MirrorTurn_0_Icon");
            m_mirrorTurn_90_Icon = Resources.Images.Get("MirrorTurn_90_Icon");
            m_mirrorTurn_180_Icon = Resources.Images.Get("MirrorTurn_180_Icon");
            m_mirrorTurn_270_Icon = Resources.Images.Get("MirrorTurn_270_Icon");
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_defectIconToolTipText = s.ResultRowSetter_DefectIcon_ToolTip_Text;
            m_duplPairIconToolTipText = s.ResultRowSetter_DuplPairIcon_ToolTip_Text;

            m_unknownDefectIconToolTipText = s.ResultRowSetter_UnknownDefectIcon_ToolTip_Text;
            m_jpegEndMarkerIsAbsentIconToolTipText = s.ResultRowSetter_JpegEndMarkerIsAbsentIcon_ToolTip_Text;
            m_blockinessIconToolTipText = s.ResultRowSetter_blockinessIcon_ToolTip_Text;
            m_blurringIconToolTipText = s.ResultRowSetter_blurringIcon_ToolTip_Text;

            m_deleteDefectIconToolTipText = s.ResultRowSetter_DeleteDefectIcon_ToolTip_Text;
            m_deleteFirstIconToolTipText = s.ResultRowSetter_DeleteFirstIcon_ToolTip_Text;
            m_deleteSecondIconToolTipText = s.ResultRowSetter_DeleteSecondIcon_ToolTip_Text;
            m_renameFirstToSecondIconToolTipText = s.ResultRowSetter_RenameFirstToSecondIcon_ToolTip_Text;
            m_renameSecondToFirstIconToolTipText = s.ResultRowSetter_RenameSecondToFirstIcon_ToolTip_Text;

            m_turn_0_IconToolTipText = s.ResultRowSetter_Turn_0_Icon_ToolTip_Text;
            m_turn_90_IconToolTipText = s.ResultRowSetter_Turn_90_Icon_ToolTip_Text;
            m_turn_180_IconToolTipText = s.ResultRowSetter_Turn_180_Icon_ToolTip_Text;
            m_turn_270_IconToolTipText = s.ResultRowSetter_Turn_270_Icon_ToolTip_Text;
            m_mirrorTurn_0_IconToolTipText = s.ResultRowSetter_MirrorTurn_0_Icon_ToolTip_Text;
            m_mirrorTurn_90_IconToolTipText = s.ResultRowSetter_MirrorTurn_90_Icon_ToolTip_Text;
            m_mirrorTurn_180_IconToolTipText = s.ResultRowSetter_MirrorTurn_180_Icon_ToolTip_Text;
            m_mirrorTurn_270_IconToolTipText = s.ResultRowSetter_MirrorTurn_270_Icon_ToolTip_Text;
        }

        public void Set(CoreResult result, DataGridViewCustomRow row) 
        {
            switch (result.type)
            {
                case CoreDll.ResultType.DefectImage:
                    SetDefectToRow(row.Cells, result);
                    break;
                case CoreDll.ResultType.DuplImagePair:
                    SetDuplPairToRow(row.Cells, result);
                    break;
            }
        }

        private void SetDefectToRow(DataGridViewCellCollection cells, CoreResult result)
        {
            if (result.type != CoreDll.ResultType.DefectImage)
                throw new Exception("Bad result type!");

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].Value = m_defectIcon;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].ToolTipText = m_defectIconToolTipText;

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group].Value = (result.group == -1 ? "" : result.group.ToString());
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Value = "";

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            switch (result.defect)
            {
                case CoreDll.DefectType.None:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = m_nullIcon;
                    break;
                case CoreDll.DefectType.Unknown:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = m_unknownDefectIcon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].ToolTipText = m_unknownDefectIconToolTipText;
                    break;
                case CoreDll.DefectType.JpegEndMarkerIsAbsent:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = m_jpegEndMarkerIsAbsentIcon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].ToolTipText = m_jpegEndMarkerIsAbsentIconToolTipText;
                    break;
                case CoreDll.DefectType.Blockiness:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = m_blockinessIcon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].ToolTipText = m_blockinessIconToolTipText;
                    break;
                case CoreDll.DefectType.Blurring:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = m_blurringIcon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].ToolTipText = m_blurringIconToolTipText;
                    break;
            }

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = "";

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            switch (result.hint)
            {
                case CoreDll.HintType.DeleteFirst:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_deleteDefectIcon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].ToolTipText = m_deleteDefectIconToolTipText;
                    break;
                default:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_nullIcon;
                    break;
            } 
            
            switch (m_options.resultsOptions.viewMode)
            {
                case ResultsOptions.ViewMode.VerticalPairTable:
                    SetDefectToRowVertical(cells, result);
                    break;
                case ResultsOptions.ViewMode.HorizontalPairTable:
                    SetDefectToRowHorizontal(cells, result);
                    break;
            }
        }

        /// <summary>
        /// Set cell defect in vertical mode.
        /// ”становка €йчейки дефектов в вертикальном режиме.
        /// </summary>
        private void SetDefectToRowVertical(DataGridViewCellCollection cells, CoreResult result)
        {
            for (int col = (int)ResultsListView.ColumnsTypeVertical.FileName; col < (int)ResultsListView.ColumnsTypeVertical.Size; col++)
                cells[col] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeVertical.FileName].Value = Path.GetFileName(result.first.path);
            cells[(int)ResultsListView.ColumnsTypeVertical.FileDirectory].Value = result.first.GetDirectoryString();
            cells[(int)ResultsListView.ColumnsTypeVertical.ImageSize].Value = result.first.GetImageSizeString();
            cells[(int)ResultsListView.ColumnsTypeVertical.ImageType].Value = result.first.GetImageTypeString();

            cells[(int)ResultsListView.ColumnsTypeVertical.Blockiness].Value = result.first.GetBlockinessString();
            cells[(int)ResultsListView.ColumnsTypeVertical.Blockiness].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            cells[(int)ResultsListView.ColumnsTypeVertical.Blurring].Value = result.first.GetBlurringString();
            cells[(int)ResultsListView.ColumnsTypeVertical.Blurring].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            cells[(int)ResultsListView.ColumnsTypeVertical.FileSize].Value = result.first.GetFileSizeString();
            cells[(int)ResultsListView.ColumnsTypeVertical.FileSize].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            cells[(int)ResultsListView.ColumnsTypeVertical.FileTime].Value = result.first.GetFileTimeString();
        }

        /// <summary>
        /// Set cell defect in horizontal mode.
        /// ”становка €йчейки дефектов в горизонтальном режиме.
        /// </summary>
        private void SetDefectToRowHorizontal(DataGridViewCellCollection cells, CoreResult result)
        {
            for (int col = (int)ResultsListView.ColumnsTypeHorizontal.FirstFileName; col < (int)ResultsListView.ColumnsTypeHorizontal.Size; col++)
                cells[col] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileName].Value = Path.GetFileName(result.first.path);
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].Value = result.first.GetDirectoryString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].Value = result.first.GetImageSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].Value = result.first.GetImageTypeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Value = result.first.GetBlockinessString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Value = result.first.GetBlurringString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Value = result.first.GetFileSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].Value = result.first.GetFileTimeString();
            
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileName].Value = "";
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].Value = "";
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].Value = "";
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].Value = "";
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Value = "";
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].Value = "";
        }

        private void SetDuplPairToRow(DataGridViewCellCollection cells, CoreResult result)
        {
            if (result.type != CoreDll.ResultType.DuplImagePair)
                throw new Exception("Bad result type!");

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].ToolTipText = m_duplPairIconToolTipText;

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group].Value = (result.group == -1 ? "" : result.group.ToString());
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Group].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Value = result.difference.ToString("F2");
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Style.Font =
                new Font(DataGridView.DefaultFont, result.difference == 0 ? FontStyle.Bold : FontStyle.Regular);
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Style.ForeColor =
                result.difference == 0 ? Color.LightGreen : DataGridView.DefaultForeColor;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Value = "";

            cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            switch (result.transform)
            {
                case CoreDll.TransformType.Turn_0:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_turn_0_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_turn_0_IconToolTipText;
                    break;
                case CoreDll.TransformType.Turn_90:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_turn_90_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_turn_90_IconToolTipText;
                    break;
                case CoreDll.TransformType.Turn_180:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_turn_180_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_turn_180_IconToolTipText;
                    break;
                case CoreDll.TransformType.Turn_270:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_turn_270_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_turn_270_IconToolTipText;
                    break;
                case CoreDll.TransformType.MirrorTurn_0:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_mirrorTurn_0_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_mirrorTurn_0_IconToolTipText;
                    break;
                case CoreDll.TransformType.MirrorTurn_90:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_mirrorTurn_90_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_mirrorTurn_90_IconToolTipText;
                    break;
                case CoreDll.TransformType.MirrorTurn_180:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_mirrorTurn_180_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_mirrorTurn_180_IconToolTipText;
                    break;
                case CoreDll.TransformType.MirrorTurn_270:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Value = m_mirrorTurn_270_Icon;
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Transform].ToolTipText = m_mirrorTurn_270_IconToolTipText;
                    break;
            }

            cells[(int)ResultsListView.ColumnsTypeVertical.Hint] = new DataGridViewImageCell();
            cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Style.Alignment = DataGridViewContentAlignment.MiddleCenter;
            switch (result.hint)
            {
                case CoreDll.HintType.DeleteFirst:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].ToolTipText = m_deleteFirstIconToolTipText;
                    break;
                case CoreDll.HintType.DeleteSecond:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].ToolTipText = m_deleteSecondIconToolTipText;
                    break;
                case CoreDll.HintType.RenameFirstToSecond:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].ToolTipText = m_renameFirstToSecondIconToolTipText;
                    break;
                case CoreDll.HintType.RenameSecondToFirst:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].ToolTipText = m_renameSecondToFirstIconToolTipText;
                    break;
                default:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Value = m_nullIcon;
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].ToolTipText = "";
                    break;
            }

            switch (m_options.resultsOptions.viewMode)
            {
                case ResultsOptions.ViewMode.VerticalPairTable:
                    SetDuplPairToRowVertical(cells, result);
                    break;
                case ResultsOptions.ViewMode.HorizontalPairTable:
                    SetDuplPairToRowHorizontal(cells, result);
                    break;
            }
        }

        /// <summary>
        /// Set cell duplicate pair in vertical mode.
        /// ”становка €йчеек пар дубликатов в вертикальном режиме.
        /// </summary>
        private void SetDuplPairToRowVertical(DataGridViewCellCollection cells, CoreResult result)
        {
            cells[(int)ResultsListView.ColumnsTypeVertical.Type].Value = m_duplPairVerticalIcon;

            switch (result.hint)
            {
                case CoreDll.HintType.DeleteFirst:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Value = m_deleteFirstVerticalIcon;
                    break;
                case CoreDll.HintType.DeleteSecond:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Value = m_deleteSecondVerticalIcon;
                    break;
                case CoreDll.HintType.RenameFirstToSecond:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Value = m_renameFirstToSecondVerticalIcon;
                    break;
                case CoreDll.HintType.RenameSecondToFirst:
                    cells[(int)ResultsListView.ColumnsTypeVertical.Hint].Value = m_renameSecondToFirstVerticalIcon;
                    break;
            }

            DataGridViewDoubleTextBoxCell doubleCell;
            cells[(int)ResultsListView.ColumnsTypeVertical.FileName] = new DataGridViewDoubleTextBoxCell(
              Path.GetFileName(result.first.path), Path.GetFileName(result.second.path));
            cells[(int)ResultsListView.ColumnsTypeVertical.FileDirectory] = new DataGridViewDoubleTextBoxCell(
              result.first.GetDirectoryString(), result.second.GetDirectoryString());

            doubleCell = new DataGridViewDoubleTextBoxCell(result.first.GetImageSizeString(), result.second.GetImageSizeString());
            if (result.first.height * result.first.width > result.second.height * result.second.width)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.Second;
            else if (result.first.height * result.first.width < result.second.height * result.second.width)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.First;
            cells[(int)ResultsListView.ColumnsTypeVertical.ImageSize] = doubleCell;

            doubleCell = new DataGridViewDoubleTextBoxCell(result.first.GetImageTypeString(), result.second.GetImageTypeString());
            if (result.first.type != result.second.type)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.Both;
            cells[(int)ResultsListView.ColumnsTypeVertical.ImageType] = doubleCell;

            doubleCell = new DataGridViewDoubleTextBoxCell(result.first.GetFileSizeString(), result.second.GetFileSizeString());
            if (result.first.size > result.second.size)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.Second;
            else if (result.first.size < result.second.size)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.First;
            cells[(int)ResultsListView.ColumnsTypeVertical.FileSize] = doubleCell;
            cells[(int)ResultsListView.ColumnsTypeVertical.FileSize].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            doubleCell = new DataGridViewDoubleTextBoxCell(result.first.GetBlockinessString(), result.second.GetBlockinessString());
            if (result.first.blockiness > result.second.blockiness) //подсветка highlight
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.First;
            else if (result.first.blockiness < result.second.blockiness)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.Second;
            cells[(int)ResultsListView.ColumnsTypeVertical.Blockiness] = doubleCell;
            cells[(int)ResultsListView.ColumnsTypeVertical.Blockiness].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            doubleCell = new DataGridViewDoubleTextBoxCell(result.first.GetBlurringString(), result.second.GetBlurringString());
            if (result.first.blurring > result.second.blurring) 
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.First;
            else if (result.first.blurring < result.second.blurring)
                doubleCell.markType = DataGridViewDoubleTextBoxCell.MarkType.Second;
            cells[(int)ResultsListView.ColumnsTypeVertical.Blurring] = doubleCell;
            cells[(int)ResultsListView.ColumnsTypeVertical.Blurring].Style.Alignment = DataGridViewContentAlignment.MiddleRight;

            cells[(int)ResultsListView.ColumnsTypeVertical.FileTime] = new DataGridViewDoubleTextBoxCell(result.first.GetFileTimeString(), result.second.GetFileTimeString());
        }

        private void SetDuplPairToRowHorizontal(DataGridViewCellCollection cells, CoreResult result)
        {
            cells[(int)ResultsListView.ColumnsTypeHorizontal.Type].Value = m_duplPairHorizontalIcon;

            switch (result.hint)
            {
                case CoreDll.HintType.DeleteFirst:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_deleteFirstHorizontalIcon;
                    break;
                case CoreDll.HintType.DeleteSecond:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_deleteSecondHorizontalIcon;
                    break;
                case CoreDll.HintType.RenameFirstToSecond:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_renameFirstToSecondHorizontalIcon;
                    break;
                case CoreDll.HintType.RenameSecondToFirst:
                    cells[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Value = m_renameSecondToFirstHorizontalIcon;
                    break;
            }

            for (int col = (int)ResultsListView.ColumnsTypeHorizontal.FirstFileName; col < (int)ResultsListView.ColumnsTypeHorizontal.Size; col++)
                cells[col] = new DataGridViewTextBoxCell();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileName].Value = Path.GetFileName(result.first.path);
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].Value = result.first.GetDirectoryString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].Value = result.first.GetImageSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].Value = result.first.GetImageTypeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Value = result.first.GetBlockinessString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Value = result.first.GetBlurringString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Value = result.first.GetFileSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].Value = result.first.GetFileTimeString();

            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileName].Value = Path.GetFileName(result.second.path);
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].Value = result.second.GetDirectoryString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].Value = result.second.GetImageSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].Value = result.second.GetImageTypeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].Value = result.second.GetBlockinessString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].Value = result.second.GetBlurringString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Value = result.second.GetFileSizeString();
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Style.Alignment = DataGridViewContentAlignment.MiddleRight;
            cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].Value = result.second.GetFileTimeString();

            if (result.first.height * result.first.width > result.second.height * result.second.width) //подсветка highlight
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].Style.ForeColor = Color.Red;
            }
            else if (result.first.height * result.first.width < result.second.height * result.second.width)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].Style.ForeColor = Color.Red;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
            }

            if (result.first.size > result.second.size)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Style.ForeColor = Color.Red;
            }
            else if (result.first.size < result.second.size)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Style.ForeColor = Color.Red;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
            }

            if (result.first.blockiness > result.second.blockiness)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Style.ForeColor = Color.Red;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
            }
            else if (result.first.blockiness < result.second.blockiness)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].Style.ForeColor = Color.Red;
            }

            if (result.first.blurring > result.second.blurring)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Style.ForeColor = Color.Red;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
            }
            else if (result.first.blurring < result.second.blurring)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Style.ForeColor = m_dataGridView.DefaultCellStyle.ForeColor;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].Style.ForeColor = Color.Red;
            }

            if (result.first.type != result.second.type)
            {
                cells[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].Style.ForeColor = Color.Red;
                cells[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].Style.ForeColor = Color.Red;
            }
        }
    }
}
