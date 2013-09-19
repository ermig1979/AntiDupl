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

namespace AntiDupl.NET
{
    public class Strings
    {
        public Strings()
        {
            StringsDefaultEnglish.CopyTo(this);
        }

        public string Name;
        public string OriginalLanguageName;

        public string OkButton_Text;
        public string CancelButton_Text;
        public string StopButton_Text;
        public string SetDefaultButton_Text;

        public string ErrorMessage_FileAlreadyExists;
        
        public string WarningMessage_ChangeFileExtension;

        public string AboutProgramPanel_CopyrightLabel0_Text;
        public string AboutProgramPanel_ComponentLabel_Text;
        public string AboutProgramPanel_VersionLabel_Text;

        public string AboutProgramForm_Text;

        public string StartFinishForm_LoadImages_Text;
        public string StartFinishForm_LoadMistakes_Text;
        public string StartFinishForm_LoadResults_Text;
        public string StartFinishForm_SaveImages_Text;
        public string StartFinishForm_SaveMistakes_Text;
        public string StartFinishForm_SaveResults_Text;
        public string StartFinishForm_ClearTemporary_Text;

        public string CoreOptionsForm_Text;

        public string CoreOptionsForm_SearchTabPage_Text;
        public string CoreOptionsForm_SearchFileTypeGroupBox_Text;
        public string CoreOptionsForm_BmpCheckBox_Text;
        public string CoreOptionsForm_GifCheckBox_Text;
        public string CoreOptionsForm_JpegCheckBox_Text;
        public string CoreOptionsForm_PngCheckBox_Text;
        public string CoreOptionsForm_TiffCheckBox_Text;
        public string CoreOptionsForm_EmfCheckBox_Text;
        public string CoreOptionsForm_WmfCheckBox_Text;
        public string CoreOptionsForm_ExifCheckBox_Text;
        public string CoreOptionsForm_IconCheckBox_Text;
        public string CoreOptionsForm_Jp2CheckBox_Text;
        public string CoreOptionsForm_PsdCheckBox_Text;
        public string CoreOptionsForm_DdsCheckBox_Text;
        public string CoreOptionsForm_TgaCheckBox_Text;
        public string CoreOptionsForm_SearchSystemCheckBox_Text;
        public string CoreOptionsForm_SearchHiddenCheckBox_Text;
        public string CoreOptionsForm_SearchSubFoldersCheckBox_Text;

        public string CoreOptionsForm_CheckTabPage_Text;
        //public string CoreOptionsForm_CheckOnDefectCheckBox_Text;
        public string CoreOptionsForm_CheckOnEqualityCheckBox_Text;
        public string CoreOptionsForm_TransformedImageCheckBox_Text;
        public string CoreOptionsForm_SizeControlCheckBox_Text;
        public string CoreOptionsForm_TypeControlCheckBox_Text;
        public string CoreOptionsForm_RatioControlCheckBox_Text;
        public string CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text;
        public string CoreOptionsForm_MinimalImageSizeLabeledIntegerEdit_Text;
        public string CoreOptionsForm_MaximalImageSizeLabeledIntegerEdit_Text;
        public string CoreOptionsForm_CompareInsideOneFolderCheckBox_Text;

        public string CoreOptionsForm_DefectTabPage_Text;
        public string CoreOptionsForm_CheckOnDefectCheckBox_Text;
        public string CoreOptionsForm_CheckOnBlockinessCheckBox_Text;
        public string CoreOptionsForm_BlockinessThresholdLabeledComboBox_Text;
        public string CoreOptionsForm_CheckOnBlockinessOnlyNotJpegCheckBox_Text;

        public string CoreOptionsForm_AdvancedTabPage_Text;
        public string CoreOptionsForm_DeleteToRecycleBinCheckBox_Text;
        public string CoreOptionsForm_MistakeDataBaseCheckBox_Text;
        public string CoreOptionsForm_RatioResolutionLabeledComboBox_Text;
        public string CoreOptionsForm_CompareThreadCountLabeledComboBox_Text;
        public string CoreOptionsForm_CompareThreadCountLabeledComboBox_Description_0;
        public string CoreOptionsForm_CollectThreadCountLabeledComboBox_Text;
        public string CoreOptionsForm_CollectThreadCountLabeledComboBox_Description_0;
        public string CoreOptionsForm_ReducedImageSizeLabeledComboBox_Text;
        public string CoreOptionsForm_UndoQueueSizeLabeledIntegerEdit_Text;
        public string CoreOptionsForm_ResultCountMaxLabeledIntegerEdit_Text;
        public string CoreOptionsForm_IgnoreFrameWidthLabeledComboBox_Text;

        public string CorePathsForm_Text;
        public string CorePathsForm_SearchTabPage_Text;
        public string CorePathsForm_IgnoreTabPage_Text;
        public string CorePathsForm_ValidTabPage_Text;
        public string CorePathsForm_DeleteTabPage_Text;
        public string CorePathsForm_AddFolderButton_Text;
        public string CorePathsForm_AddFilesButton_Text;
        public string CorePathsForm_ChangeButton_Text;
        public string CorePathsForm_RemoveButton_Text;

        public string ProgressUtils_Completed;
        public string ProgressUtils_5HoursRemaining;
        public string ProgressUtils_2HoursRemaining;
        public string ProgressUtils_5MinutesRemaining;
        public string ProgressUtils_2MinutesRemaining;
        public string ProgressUtils_5SecondsRemaining;

        public string ProgressForm_DeleteDefect;
        public string ProgressForm_DeleteFirst;
        public string ProgressForm_DeleteSecond;
        public string ProgressForm_DeleteBoth;
        public string ProgressForm_PerformHint;
        public string ProgressForm_Mistake;
        public string ProgressForm_RenameCurrent;
        public string ProgressForm_RefreshResults;
        public string ProgressForm_Undo;
        public string ProgressForm_Redo;

        public string SearchExecuterForm_Result;
        public string SearchExecuterForm_Search;
        public string SearchExecuterForm_Stopped;
        public string SearchExecuterForm_MinimizeToTaskbarButton_Text;
        public string SearchExecuterForm_MinimizeToSystrayButton_Text;

        public string ResultsPreviewBase_NextButton_ToolTip_Text;
        public string ResultsPreviewBase_PreviousButton_ToolTip_Text;

        public string ResultsPreviewDuplPair_DeleteFirstButton_ToolTip_Text;
        public string ResultsPreviewDuplPair_DeleteSecondButton_ToolTip_Text;
        public string ResultsPreviewDuplPair_DeleteBothButton_ToolTip_Text;
        public string ResultsPreviewDuplPair_RenameFirstToSecondButton_ToolTip_Text;
        public string ResultsPreviewDuplPair_RenameSecondToFirstButton_ToolTip_Text;
        public string ResultsPreviewDuplPair_MistakeButton_ToolTip_Text;

        public string ResultsPreviewDefect_DeleteButton_ToolTip_Text;
        public string ResultsPreviewDefect_MistakeButton_ToolTip_Text;

        public string ResultRowSetter_DefectIcon_ToolTip_Text;
        public string ResultRowSetter_DuplPairIcon_ToolTip_Text;

        public string ResultRowSetter_UnknownDefectIcon_ToolTip_Text;
        public string ResultRowSetter_JpegEndMarkerIsAbsentIcon_ToolTip_Text;
        public string ResultRowSetter_blockinessIcon_ToolTip_Text;

        public string ResultRowSetter_DeleteDefectIcon_ToolTip_Text;
        public string ResultRowSetter_DeleteFirstIcon_ToolTip_Text;
        public string ResultRowSetter_DeleteSecondIcon_ToolTip_Text;
        public string ResultRowSetter_RenameFirstToSecondIcon_ToolTip_Text;
        public string ResultRowSetter_RenameSecondToFirstIcon_ToolTip_Text;

        public string ResultRowSetter_Turn_0_Icon_ToolTip_Text;
        public string ResultRowSetter_Turn_90_Icon_ToolTip_Text;
        public string ResultRowSetter_Turn_180_Icon_ToolTip_Text;
        public string ResultRowSetter_Turn_270_Icon_ToolTip_Text;
        public string ResultRowSetter_MirrorTurn_0_Icon_ToolTip_Text;
        public string ResultRowSetter_MirrorTurn_90_Icon_ToolTip_Text;
        public string ResultRowSetter_MirrorTurn_180_Icon_ToolTip_Text;
        public string ResultRowSetter_MirrorTurn_270_Icon_ToolTip_Text;

        public string ResultsListView_Type_Column_Text;
        public string ResultsListView_Group_Column_Text;
        public string ResultsListView_Difference_Column_Text;
        public string ResultsListView_Defect_Column_Text;
        public string ResultsListView_Transform_Column_Text;
        public string ResultsListView_Hint_Column_Text;

        public string ResultsListView_FileName_Column_Text;
        public string ResultsListView_FileDirectory_Column_Text;
        public string ResultsListView_ImageSize_Column_Text;
        public string ResultsListView_ImageType_Column_Text;
        public string ResultsListView_ImageBlockiness_Column_Text;
        public string ResultsListView_FileSize_Column_Text;
        public string ResultsListView_FileTime_Column_Text;

        public string ResultsListView_FirstFileName_Column_Text;
        public string ResultsListView_FirstFileDirectory_Column_Text;
        public string ResultsListView_FirstImageSize_Column_Text;
        public string ResultsListView_FirstImageType_Column_Text;
        public string ResultsListView_FirstImageBlockiness_Column_Text;
        public string ResultsListView_FirstFileSize_Column_Text;
        public string ResultsListView_FirstFileTime_Column_Text;
        public string ResultsListView_SecondFileName_Column_Text;
        public string ResultsListView_SecondFileDirectory_Column_Text;
        public string ResultsListView_SecondImageSize_Column_Text;
        public string ResultsListView_SecondImageType_Column_Text;
        public string ResultsListView_SecondImageBlockiness_Column_Text;
        public string ResultsListView_SecondFileSize_Column_Text;
        public string ResultsListView_SecondFileTime_Column_Text;

        public string ResultsListViewContextMenu_DeleteDefectItem_Text;
        public string ResultsListViewContextMenu_DeleteFirstItem_Text;
        public string ResultsListViewContextMenu_DeleteSecondItem_Text;
        public string ResultsListViewContextMenu_DeleteBothItem_Text;
        public string ResultsListViewContextMenu_MistakeItem_Text;
        public string ResultsListViewContextMenu_PerformHintItem_Text;

        public string MainStatusStrip_TotalLabel_Text;
        public string MainStatusStrip_CurrentLabel_Text;
        public string MainStatusStrip_SelectedLabel_Text;

        public string MainMenu_FileMenuItem_Text;
        public string MainMenu_File_OpenProfileMenuItem_Text;
        public string MainMenu_File_SaveProfileAsMenuItem_Text;
        public string MainMenu_File_ExitMenuItem_Text;

        public string MainMenu_EditMenuItem_Text;
        public string MainMenu_Edit_UndoMenuItem_Text;
        public string MainMenu_Edit_RedoMenuItem_Text;
        public string MainMenu_Edit_SelectAllMenuItem_Text;

        public string MainMenu_ViewMenuItem_Text;
        public string MainMenu_View_ToolMenuItem_Text;
        public string MainMenu_View_StatusMenuItem_Text;
        public string MainMenu_View_SelectColumnsMenuItem_Text;
        public string MainMenu_View_HotKeysMenuItem_Text;
        public string MainMenu_View_StretchSmallImagesMenuItem_Text;
        public string MainMenu_View_ProportionalImageSizeMenuItem_Text;

        public string MainMenu_SearchMenuItem_Text;
        public string MainMenu_Search_StartMenuItem_Text;
        public string MainMenu_Search_RefreshResultsMenuItem_Text;
        public string MainMenu_Search_RefreshImagesMenuItem_Text;
        public string MainMenu_Search_PathsMenuItem_Text;
        public string MainMenu_Search_OptionsMenuItem_Text;
        public string MainMenu_Search_OnePathMenuItem_Text;
        public string MainMenu_Search_UseImageDataBaseMenuItem_Text;
        public string MainMenu_Search_CheckResultsAtLoadingMenuItem_Text;
        public string MainMenu_Search_CheckMistakesAtLoadingMenuItem_Text;

        public string MainMenu_HelpMenuItem_Text;
        public string MainMenu_Help_HelpMenuItem_Text;
        public string MainMenu_Help_AboutProgramMenuItem_Text;
        public string MainMenu_Help_CheckingForUpdatesMenuItem_Text;

        public string MainMenu_NewVersionMenuItem_Text;
        public string MainMenu_NewVersionMenuItem_Tooltip;

        public string SelectHotKeysForm_InvalidHotKeyToolTipText;

        public string LanguageMenuItem_Text;

        public string ViewModeMenuItem_Text;
        public string ViewModeMenuItem_VerticalPairTableMenuItem_Text;
        public string ViewModeMenuItem_HorizontalPairTableMenuItem_Text;
        public string ViewModeMenuItem_GroupedThumbnailsMenuItem_Text;

        public string ImagePreviewContextMenu_CopyPathItem_Text;
        public string ImagePreviewContextMenu_CopyFileNameItem_Text;
        public string ImagePreviewContextMenu_OpenImageItem_Text;
        public string ImagePreviewContextMenu_OpenFolderItem_Text;
        public string ImagePreviewContextMenu_RenameImageItem_Text;
        public string ImagePreviewContextMenu_RenameLikeNeighbour_Text;
        public string ImagePreviewContextMenu_MoveImageToNeighbourItem_Text;
        public string ImagePreviewContextMenu_MoveAndRenameImageToNeighbourItem_Text;
    }
}
