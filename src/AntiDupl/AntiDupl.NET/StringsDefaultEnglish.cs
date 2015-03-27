/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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
    static public class StringsDefaultEnglish
    {
        static public void CopyTo(Strings s)
        {
            s.Name = "English";
            s.OriginalLanguageName = "English";

            s.OkButton_Text = "OK";
            s.CancelButton_Text = "Cancel";
            s.StopButton_Text = "Stop";
            s.SetDefaultButton_Text = "Set default";

            s.ErrorMessage_FileAlreadyExists = "Can't rename file because file with this name already is exists!";

            s.WarningMessage_ChangeFileExtension = "Do you really want to change file extension?";

            s.AboutProgramPanel_CopyrightLabel0_Text = "Copyright (c) 2002-2015 Yermalayeu Ihar, ";
            s.AboutProgramPanel_CopyrightLabel1_Text = "2013-2015 Borisov Dmitry.";
            s.AboutProgramPanel_ComponentLabel_Text = "Component";
            s.AboutProgramPanel_VersionLabel_Text = "Version";

            s.AboutProgramForm_Text = "About program...";

            s.StartFinishForm_LoadImages_Text = "Load file of images";
            s.StartFinishForm_LoadMistakes_Text = "Load file of mistakes";
            s.StartFinishForm_LoadResults_Text = "Load file of results";
            s.StartFinishForm_SaveImages_Text = "Save file of images";
            s.StartFinishForm_SaveMistakes_Text = "Save file of mistakes";
            s.StartFinishForm_SaveResults_Text = "Save file of results";
            s.StartFinishForm_ClearTemporary_Text = "Delete temporary files";

            s.CoreOptionsForm_Text = "Options";

            s.CoreOptionsForm_SearchTabPage_Text = "Search";
            s.CoreOptionsForm_SearchFileTypeGroupBox_Text = "Search types:";
            s.CoreOptionsForm_BmpCheckBox_Text = "BMP";
            s.CoreOptionsForm_GifCheckBox_Text = "GIF";
            s.CoreOptionsForm_JpegCheckBox_Text = "JPEG";
            s.CoreOptionsForm_PngCheckBox_Text = "PNG";
            s.CoreOptionsForm_TiffCheckBox_Text = "TIFF";
            s.CoreOptionsForm_EmfCheckBox_Text = "EMF";
            s.CoreOptionsForm_WmfCheckBox_Text = "WMF";
            s.CoreOptionsForm_ExifCheckBox_Text = "EXIF";
            s.CoreOptionsForm_IconCheckBox_Text = "ICON";
            s.CoreOptionsForm_Jp2CheckBox_Text = "JP2";
            s.CoreOptionsForm_PsdCheckBox_Text = "PSD";
            s.CoreOptionsForm_DdsCheckBox_Text = "DDS";
            s.CoreOptionsForm_TgaCheckBox_Text = "TGA";
            s.CoreOptionsForm_SearchSystemCheckBox_Text = "Search system folders/files";
            s.CoreOptionsForm_SearchHiddenCheckBox_Text = "Search hidden folders/files";

            s.CoreOptionsForm_CompareTabPage_Text = "Compare";
            s.CoreOptionsForm_CheckOnEqualityCheckBox_Text = "Search image dupls";
            s.CoreOptionsForm_TransformedImageCheckBox_Text = "Search rotated and mirror image dupls";
            s.CoreOptionsForm_SizeControlCheckBox_Text = "Control image size";
            s.CoreOptionsForm_TypeControlCheckBox_Text = "Control image type";
            s.CoreOptionsForm_RatioControlCheckBox_Text = "Control image width/height ratio";
            s.CoreOptionsForm_AlgorithmComparingLabeledComboBox_Text = "Algorithm of comparing of images";
            s.CoreOptionsForm_AlgorithmComparingLabeledComboBox_SquaredSum = "Mean square difference";
            s.CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text = "Threshold difference";
            s.CoreOptionsForm_MinimalImageSizeLabeledIntegerEdit_Text = "Minimal image width/height";
            s.CoreOptionsForm_MaximalImageSizeLabeledIntegerEdit_Text = "Maximal image width/height";
            s.CoreOptionsForm_CompareInsideOneSearchPathCheckBox_Text = "Compare images from one path of search with one another";
            s.CoreOptionsForm_CompareInsideOneFolderCheckBox_Text = "Compare images inside one directory";

            s.CoreOptionsForm_DefectTabPage_Text = "Defects";
            s.CoreOptionsForm_CheckOnDefectCheckBox_Text = "Check on defect";
            s.CoreOptionsForm_CheckOnBlockinessCheckBox_Text = "Check on blockiness";
            s.CoreOptionsForm_BlockinessThresholdLabeledComboBox_Text = "Blockiness threshold";
            s.CoreOptionsForm_CheckOnBlockinessOnlyNotJpegCheckBox_Text = "Check on blockiness only on not Jpeg";
            s.CoreOptionsForm_CheckOnBlurringCheckBox_Text = "Check on blurring";
            s.CoreOptionsForm_BlurringThresholdLabeledComboBox_Text = "Blurring threshold";

            s.CoreOptionsForm_AdvancedTabPage_Text = "Advanced";
            s.CoreOptionsForm_DeleteToRecycleBinCheckBox_Text = "Delete to recycle bin";
            s.CoreOptionsForm_MistakeDataBaseCheckBox_Text = "Remember mistakes";
            s.CoreOptionsForm_RatioResolutionLabeledComboBox_Text = "Image width/height ratio precision";
            s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Text = "Compare thread count";
            s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Description_0 = "Auto";
            s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Text = "Load thread count";
            s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Description_0 = "Auto";
            s.CoreOptionsForm_ReducedImageSizeLabeledComboBox_Text = "Normalized image size";
            s.CoreOptionsForm_UndoQueueSizeLabeledIntegerEdit_Text = "Queue size of undo";
            s.CoreOptionsForm_ResultCountMaxLabeledIntegerEdit_Text = "Maximal count of results";
            s.CoreOptionsForm_IgnoreFrameWidthLabeledComboBox_Text = "Width of ignored frame of image";

            s.CorePathsForm_Text = "Paths";
            s.CorePathsForm_SearchTabPage_Text = "Search";
            s.CorePathsForm_IgnoreTabPage_Text = "Ignore";
            s.CorePathsForm_ValidTabPage_Text = "Valid";
            s.CorePathsForm_DeleteTabPage_Text = "Delete";
            s.CorePathsForm_AddFolderButton_Text = "Add Folder";
            s.CorePathsForm_AddFilesButton_Text = "Add Files";
            s.CorePathsForm_ChangeButton_Text = "Change";
            s.CorePathsForm_RemoveButton_Text = "Remove";
            s.CorePathsForm_SearchCheckedListBox_ToolTip_Text = "Checked box mean search inside subdirectories this directories.";

            s.ProgressUtils_Completed = "{0}% completed";
            s.ProgressUtils_5HoursRemaining = "{0} hours remaining";
            s.ProgressUtils_2HoursRemaining = "{0} hours remaining";
            s.ProgressUtils_5MinutesRemaining = "{0} minutes remaining";
            s.ProgressUtils_2MinutesRemaining = "{0} minutes remaining";
            s.ProgressUtils_5SecondsRemaining = "{0} seconds remaining";

            s.ProgressForm_DeleteDefect = "Deletion of defect pictures in selected results";
            s.ProgressForm_DeleteFirst = "Deletion of first pictures in selected results";
            s.ProgressForm_DeleteSecond = "Deletion of second pictures in selected results";
            s.ProgressForm_DeleteBoth = "Deletion of all duplicated pictures in selected results";
            s.ProgressForm_PerformHint = "Automatic processing of selected results";
            s.ProgressForm_Mistake = "Marking of selected results as a mistakes";
            s.ProgressForm_RenameCurrent = "Renaming/replacing of images";
            s.ProgressForm_RefreshResults = "Refresh of results";
            s.ProgressForm_Undo = "Action undoing";
            s.ProgressForm_Redo = "Action redoing";

            s.SearchExecuterForm_Result = "Result processing";
            s.SearchExecuterForm_Search = "Search";
            s.SearchExecuterForm_Stopped = "Stopping of search";
            s.SearchExecuterForm_MinimizeToTaskbarButton_Text = "Minimize";
            s.SearchExecuterForm_MinimizeToSystrayButton_Text = "Background Mode";

            s.ResultsPreviewBase_NextButton_ToolTip_Text = "Go to the next result";
            s.ResultsPreviewBase_PreviousButton_ToolTip_Text = "Go to the previous result";

            s.ResultsPreviewDuplPair_DeleteFirstButton_ToolTip_Text = "Delete first picture";
            s.ResultsPreviewDuplPair_DeleteSecondButton_ToolTip_Text = "Delete second picture";
            s.ResultsPreviewDuplPair_DeleteBothButton_ToolTip_Text = "Delete both pictures";
            s.ResultsPreviewDuplPair_RenameFirstToSecondButton_ToolTip_Text = "Replace second picture by the first picture";
            s.ResultsPreviewDuplPair_RenameSecondToFirstButton_ToolTip_Text = "Replace first picture by the second picture";
            s.ResultsPreviewDuplPair_MistakeButton_ToolTip_Text = "Mark current result as a mistake";

            s.ResultsPreviewDefect_DeleteButton_ToolTip_Text = "Delete picture";
            s.ResultsPreviewDefect_MistakeButton_ToolTip_Text = "Mark current result as a mistake";

            s.ResultRowSetter_DefectIcon_ToolTip_Text = "Defect picture";
            s.ResultRowSetter_DuplPairIcon_ToolTip_Text = "Dubbed picture pair";

            s.ResultRowSetter_UnknownDefectIcon_ToolTip_Text = "Unknown defect";
            s.ResultRowSetter_JpegEndMarkerIsAbsentIcon_ToolTip_Text = "JPEG file end marker is absent";
            s.ResultRowSetter_blockinessIcon_ToolTip_Text = "Image with blockiness";
            s.ResultRowSetter_blurringIcon_ToolTip_Text = "Blurred image";

            s.ResultRowSetter_DeleteDefectIcon_ToolTip_Text = "Delete picture";
            s.ResultRowSetter_DeleteFirstIcon_ToolTip_Text = "Delete first picture";
            s.ResultRowSetter_DeleteSecondIcon_ToolTip_Text = "Delete second picture";
            s.ResultRowSetter_RenameFirstToSecondIcon_ToolTip_Text = "Replace second picture by the first picture";
            s.ResultRowSetter_RenameSecondToFirstIcon_ToolTip_Text = "Replace first picture by the second picture";

            s.ResultRowSetter_Turn_0_Icon_ToolTip_Text = "Original";
            s.ResultRowSetter_Turn_90_Icon_ToolTip_Text = "Rotated on 90°";
            s.ResultRowSetter_Turn_180_Icon_ToolTip_Text = "Rotated on 180°";
            s.ResultRowSetter_Turn_270_Icon_ToolTip_Text = "Rotated on 270°";
            s.ResultRowSetter_MirrorTurn_0_Icon_ToolTip_Text = "Horizontally flipped";
            s.ResultRowSetter_MirrorTurn_90_Icon_ToolTip_Text = "Horizontally flipped and rotated on 90°";
            s.ResultRowSetter_MirrorTurn_180_Icon_ToolTip_Text = "Horizontally flipped and rotated on 180°";
            s.ResultRowSetter_MirrorTurn_270_Icon_ToolTip_Text = "Horizontally flipped and rotated on 270°";

            s.ResultsListView_Type_Column_Text = "Type";
            s.ResultsListView_Group_Column_Text = "Group";
            s.ResultsListView_GroupSize_Column_Text = "Group size";
            s.ResultsListView_Difference_Column_Text = "Difference";
            s.ResultsListView_Defect_Column_Text = "Defect type";
            s.ResultsListView_Transform_Column_Text = "Transformation";
            s.ResultsListView_Hint_Column_Text = "Hint";

            s.ResultsListView_FileName_Column_Text = "Name";
            s.ResultsListView_FileDirectory_Column_Text = "In folder";
            s.ResultsListView_ImageSize_Column_Text = "Dimensions";
            s.ResultsListView_ImageType_Column_Text = "Image type";
            s.ResultsListView_Blockiness_Column_Text = "Blockiness";
            s.ResultsListView_Blurring_Column_Text = "Blurring";
            s.ResultsListView_FileSize_Column_Text = "Size";
            s.ResultsListView_FileTime_Column_Text = "Date modified";

            s.ResultsListView_FirstFileName_Column_Text = "1: Name";
            s.ResultsListView_FirstFileDirectory_Column_Text = "1: In folder";
            s.ResultsListView_FirstImageSize_Column_Text = "1: Dimensions";
            s.ResultsListView_FirstImageType_Column_Text = "1: Image type";
            s.ResultsListView_FirstBlockiness_Column_Text = "1: Blockiness";
            s.ResultsListView_FirstBlurring_Column_Text = "1: Blurring";
            s.ResultsListView_FirstFileSize_Column_Text = "1: Size";
            s.ResultsListView_FirstFileTime_Column_Text = "1: First Date modified";
            s.ResultsListView_SecondFileName_Column_Text = "2: Name";
            s.ResultsListView_SecondFileDirectory_Column_Text = "2: In folder";
            s.ResultsListView_SecondImageSize_Column_Text = "2: Dimensions";
            s.ResultsListView_SecondImageType_Column_Text = "2: Image type";
            s.ResultsListView_SecondBlockiness_Column_Text = "2: Blockiness";
            s.ResultsListView_SecondBlurring_Column_Text = "2: Blurring";
            s.ResultsListView_SecondFileSize_Column_Text = "2: Size";
            s.ResultsListView_SecondFileTime_Column_Text = "2: Date modified";

            s.ResultsListViewContextMenu_DeleteDefectItem_Text = "Delete defect pictures in selected results";
            s.ResultsListViewContextMenu_DeleteFirstItem_Text = "Delete first pictures in selected results";
            s.ResultsListViewContextMenu_DeleteSecondItem_Text = "Delete second pictures in selected results";
            s.ResultsListViewContextMenu_DeleteBothItem_Text = "Delete all duplicated pictures in selected results";
            s.ResultsListViewContextMenu_RenameFirstToSecondIcon_ToolTip_Text = "Replace second pictures by the first pictures in selected results";
            s.ResultsListViewContextMenu_RenameSecondToFirstIcon_ToolTip_Text = "Replace first pictures by the second pictures in selected results";
            s.ResultsListViewContextMenu_RenameFirstLikeSecondButton_ToolTip_Text = "Rename first pictures as the second pictures in selected results";
            s.ResultsListViewContextMenu_RenameSecondLikeFirstButton_ToolTipText = "Rename second pictures as the first pictures in selected results";
            s.ResultsListViewContextMenu_MoveFirstToSecondButton_ToolTipText = "Move first pictures to the second pictures in selected results";
            s.ResultsListViewContextMenu_MoveSecondToFirstButton_ToolTipText = "Move second pictures to the first pictures in selected results";
            s.ResultsListViewContextMenu_MistakeItem_Text = "Mark selected results as a mistakes";
            s.ResultsListViewContextMenu_PerformHintItem_Text = "Process selected results automatically";

            s.MainStatusStrip_TotalLabel_Text = "Total: ";
            s.MainStatusStrip_CurrentLabel_Text = "Current: ";
            s.MainStatusStrip_SelectedLabel_Text = "Selected: ";

            s.MainMenu_FileMenuItem_Text = "File";
            s.MainMenu_File_OpenProfileMenuItem_Text = "Open search profile";
            s.MainMenu_File_SaveProfileAsMenuItem_Text = "Save search profile as";
            s.MainMenu_File_ExitMenuItem_Text = "Exit";

            s.MainMenu_EditMenuItem_Text = "Edit";
            s.MainMenu_Edit_UndoMenuItem_Text = "Undo (Ctrl-Z)";
            s.MainMenu_Edit_RedoMenuItem_Text = "Redo (Ctrl-Y)";
            s.MainMenu_Edit_SelectAllMenuItem_Text = "Select all (Ctrl-A)";

            s.MainMenu_ViewMenuItem_Text = "View";
            s.MainMenu_View_ToolMenuItem_Text = "Tool bar";
            s.MainMenu_View_StatusMenuItem_Text = "Status bar";
            s.MainMenu_View_SelectColumnsMenuItem_Text = "Select columns";
            s.MainMenu_View_HotKeysMenuItem_Text = "Hot keys";
            s.MainMenu_View_StretchSmallImagesMenuItem_Text = "Stretch small images";
            s.MainMenu_View_ProportionalImageSizeMenuItem_Text = "Proportional size of images";

            s.MainMenu_SearchMenuItem_Text = "Search";
            s.MainMenu_Search_StartMenuItem_Text = "Start search";
            s.MainMenu_Search_RefreshResultsMenuItem_Text = "Refresh results";
            s.MainMenu_Search_RefreshImagesMenuItem_Text = "Delete irrelevant records from a database of image";
            s.MainMenu_Search_PathsMenuItem_Text = "Paths";
            s.MainMenu_Search_OptionsMenuItem_Text = "Options";
            s.MainMenu_Search_OnePathMenuItem_Text = "One search path";
            s.MainMenu_Search_UseImageDataBaseMenuItem_Text = "Use database of image";
            s.MainMenu_Search_CheckResultsAtLoadingMenuItem_Text = "Check the results at loading";
            s.MainMenu_Search_CheckMistakesAtLoadingMenuItem_Text = "Check the database of mistakes at loading";

            s.MainMenu_HelpMenuItem_Text = "Help";
            s.MainMenu_Help_HelpMenuItem_Text = "Help";
            s.MainMenu_Help_AboutProgramMenuItem_Text = "About program...";
            s.MainMenu_Help_CheckingForUpdatesMenuItem_Text = "Checking for updates";

            s.MainMenu_NewVersionMenuItem_Text = "New Version";
            s.MainMenu_NewVersionMenuItem_Tooltip = "AntiDupl.NET-{0} is available on the website!";

            s.SelectHotKeysForm_InvalidHotKeyToolTipText = "This hot key combination is already used for another purpose.";

            s.LanguageMenuItem_Text = "Language";

            s.ViewModeMenuItem_Text = "View mode of results";
            s.ViewModeMenuItem_VerticalPairTableMenuItem_Text = "Table of vertical pairs";
            s.ViewModeMenuItem_HorizontalPairTableMenuItem_Text = "Table of horizontal pairs";
            s.ViewModeMenuItem_GroupedThumbnailsMenuItem_Text = "Grouped thumbnails";

            s.ImagePreviewContextMenu_CopyPathItem_Text = "Copy path";
            s.ImagePreviewContextMenu_CopyFileNameItem_Text = "Copy image name";
            s.ImagePreviewContextMenu_OpenImageItem_Text = "Open image";
            s.ImagePreviewContextMenu_OpenFolderItem_Text = "Open containing folder";
            s.ImagePreviewContextMenu_AddToIgnore_Text = "Add to the ignored";
            s.ImagePreviewContextMenu_AddToIgnoreDirectory_Text = "Add directory to the ignored";
            s.ImagePreviewContextMenu_RenameImageItem_Text = "Rename image";
            s.ImagePreviewContextMenu_RenameImageLikeNeighbour_Text = "Rename like neighbour";
            s.ImagePreviewContextMenu_MoveImageToNeighbourItem_Text = "Move image to neighbour";
            s.ImagePreviewContextMenu_MoveAndRenameImageToNeighbourItem_Text = "Move and rename image to neighbour";
            s.ImagePreviewContextMenu_MoveGroupToNeighbourItem_Text = "Move group of image in neighbour directory";
            s.ImagePreviewContextMenu_RenameGroupAsNeighbourItem_Text = "Rename group of image as neighbour image";

            s.ImagePreviewPanel_EXIF_Text = "EXIF";
            s.ImagePreviewPanel_EXIF_Tooltip_ImageDescription = "Description: ";
            s.ImagePreviewPanel_EXIF_Tooltip_EquipMake = "Equipment Make: ";
            s.ImagePreviewPanel_EXIF_Tooltip_EquipModel = "Equipment Model: ";
            s.ImagePreviewPanel_EXIF_Tooltip_SoftwareUsed = "Software: ";
            s.ImagePreviewPanel_EXIF_Tooltip_DateTime = "Date/Time: ";
            s.ImagePreviewPanel_EXIF_Tooltip_Artist = "Artist: ";
            s.ImagePreviewPanel_EXIF_Tooltip_UserComment = "Comment: ";
        }

        static private Strings Create()
        {
            Strings strings = new Strings();
            CopyTo(strings);
            return strings;
        }

        static public Strings Get()
        {
            return m_strings;
        }

        static private Strings m_strings = Create();
    }
}
