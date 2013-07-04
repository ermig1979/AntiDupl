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
using System.IO;

namespace AntiDupl.NET
{
    public class CoreOptionsForm : Form
    {
        static public int THRESHOLD_DIFFERENCE_MAX = 15;
        static public int IGNORE_FRAME_WIDTH_MAX = 12;
        static public int IGNORE_FRAME_WIDTH_STEP = 3;
        
        static public int FORM_WIDTH = 370;
        static public int FORM_HEIGHT = 350;
        static public int COMBO_BOX_WIDTH = 65;
        static public int COMBO_BOX_HEIGHT = 20;

        private CoreLib m_core;
        private Options m_options;
        private CoreOptions m_oldCoreOptions;
        private CoreOptions m_newCoreOptions;
        private CoreOptions m_defaultCoreOptions;

        private Button m_okButton;
        private Button m_cancelButton;
        private Button m_setDefaultButton;
        private TabControl m_mainTabControl;

        private TabPage m_checkTabPage;
        private CheckBox m_checkOnDefectCheckBox;
        private CheckBox m_checkOnEqualityCheckBox;
        private CheckBox m_transformedImageCheckBox;
        private CheckBox m_sizeControlCheckBox;
        private CheckBox m_typeControlCheckBox;
        private CheckBox m_ratioControlCheckBox;
        private LabeledComboBox m_thresholdDifferenceLabeledComboBox;
        private LabeledIntegerEdit m_minimalImageSizeLabeledIntegerEdit;
        private CheckBox m_compareInsideOneFolderCheckBox;

        private TabPage m_searchTabPage;
        private GroupBox m_searchFileTypeGroupBox;
        private CheckBox m_bmpCheckBox;
        private CheckBox m_gifCheckBox;
        private CheckBox m_jpegCheckBox;
        private CheckBox m_pngCheckBox;
        private CheckBox m_tiffCheckBox;
        private CheckBox m_emfCheckBox;
        private CheckBox m_wmfCheckBox;
        private CheckBox m_exifCheckBox;
        private CheckBox m_iconCheckBox;
        private CheckBox m_jp2CheckBox;
        private CheckBox m_psdCheckBox;
        private CheckBox m_ddsCheckBox;
        private CheckBox m_tgaCheckBox;
        private CheckBox m_searchSystemCheckBox;
        private CheckBox m_searchHiddenCheckBox;
        private CheckBox m_searchSubFoldersCheckBox;

        private TabPage m_advancedTabPage;
        private CheckBox m_deleteToRecycleBinCheckBox;
        private CheckBox m_mistakeDataBaseCheckBox;
        private LabeledComboBox m_ratioResolutionLabeledComboBox;
        private LabeledComboBox m_compareThreadCountLabeledComboBox;
        private LabeledComboBox m_collectThreadCountLabeledComboBox;
        private LabeledComboBox m_reducedImageSizeLabeledComboBox;
        private LabeledIntegerEdit m_undoQueueSizeLabeledIntegerEdit;
        private LabeledIntegerEdit m_resultCountMaxLabeledIntegerEdit;
        private LabeledComboBox m_ignoreFrameWidthLabeledComboBox;

        private bool m_inited = false;

        public CoreOptionsForm(CoreLib core, Options options, CoreOptions coreOptions)
        {
            m_core = core;
            m_options = options;
            m_oldCoreOptions = coreOptions;
            m_newCoreOptions = m_oldCoreOptions.Clone();
            m_defaultCoreOptions = new CoreOptions(m_core, m_options.onePath);
            InitializeComponent();
            UpdateStrings();
            GetOptions();
            UpdateItemsEnabling();
            m_inited = true;
        }

        void InitializeComponent()
        {
            ClientSize = new System.Drawing.Size(FORM_WIDTH, FORM_HEIGHT);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;

            Resources.Help.Bind(this, Resources.Help.Options);

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 87F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 13F));
            Controls.Add(mainTableLayoutPanel);

            m_mainTabControl = new TabControl();
            m_mainTabControl.Dock = DockStyle.Fill;
            m_mainTabControl.Location = new System.Drawing.Point(0, 0);
            mainTableLayoutPanel.Controls.Add(m_mainTabControl, 0, 0);

            InitilizeCheckTabPage();

            InitilizeSearchTabPage();

            InitilizeAdvancedTabPage();

            TableLayoutPanel mainButtonsTableLayoutPanel = InitFactory.Layout.Create(4, 1);
            mainButtonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            mainButtonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20F));
            mainButtonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20F));
            mainButtonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            mainTableLayoutPanel.Controls.Add(mainButtonsTableLayoutPanel, 0, 1);

            m_okButton = new Button();
            m_okButton.Click += new EventHandler(OnOkButtonClick);
            mainButtonsTableLayoutPanel.Controls.Add(m_okButton, 1, 0);

            m_cancelButton = new Button();
            m_cancelButton.Click += new EventHandler(OnCancelButtonClick);
            mainButtonsTableLayoutPanel.Controls.Add(m_cancelButton, 2, 0);

            m_setDefaultButton = new Button();
            m_setDefaultButton.AutoSize = true;
            m_setDefaultButton.Click += new EventHandler(OnSetDefaultButtonClick);
            mainButtonsTableLayoutPanel.Controls.Add(m_setDefaultButton, 3, 0);
        }

        private void InitilizeCheckTabPage()
        {
            m_checkTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_checkTabPage);

            TableLayoutPanel checkTableLayoutPanel = InitFactory.Layout.Create(1, 9, 5);
            m_checkTabPage.Controls.Add(checkTableLayoutPanel);

            m_checkOnDefectCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_checkOnDefectCheckBox, 0, 0);

            m_checkOnEqualityCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_checkOnEqualityCheckBox, 0, 1);

            m_transformedImageCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_transformedImageCheckBox, 0, 2);

            m_sizeControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_sizeControlCheckBox, 0, 3);

            m_typeControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_typeControlCheckBox, 0, 4);

            m_ratioControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_ratioControlCheckBox, 0, 5);

            m_thresholdDifferenceLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            for (int i = 0; i <= THRESHOLD_DIFFERENCE_MAX; i++)
                m_thresholdDifferenceLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
            checkTableLayoutPanel.Controls.Add(m_thresholdDifferenceLabeledComboBox, 0, 6);

            m_minimalImageSizeLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_minimalImageSizeLabeledIntegerEdit.Min = 0;
            m_minimalImageSizeLabeledIntegerEdit.Default = m_defaultCoreOptions.checkOptions.minimalImageSize;
            checkTableLayoutPanel.Controls.Add(m_minimalImageSizeLabeledIntegerEdit, 0, 7);

            m_compareInsideOneFolderCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_compareInsideOneFolderCheckBox, 0, 8);
        }

        private void InitilizeSearchTabPage()
        {
            m_searchTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_searchTabPage);

            TableLayoutPanel searchTableLayoutPanel = InitFactory.Layout.Create(1, 4, 5);
            m_searchTabPage.Controls.Add(searchTableLayoutPanel);

            m_searchFileTypeGroupBox = new GroupBox();
            m_searchFileTypeGroupBox.Size = new System.Drawing.Size(200, 140);
            searchTableLayoutPanel.Controls.Add(m_searchFileTypeGroupBox, 0, 0);

            TableLayoutPanel searchFileTypeTableLayoutPanel = InitFactory.Layout.Create(3, 5, 5);
            m_searchFileTypeGroupBox.Controls.Add(searchFileTypeTableLayoutPanel);

            m_bmpCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_bmpCheckBox, 0, 0);

            m_gifCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_gifCheckBox, 0, 1);

            m_jpegCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_jpegCheckBox, 0, 2);

            m_pngCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_pngCheckBox, 0, 3);

            m_tiffCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_tiffCheckBox, 0, 4);

            m_emfCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_emfCheckBox, 1, 0);

            m_wmfCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_wmfCheckBox, 1, 1);

            m_exifCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_exifCheckBox, 1, 2);

            m_iconCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_iconCheckBox, 1, 3);

            m_jp2CheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_jp2CheckBox, 1, 4);

            m_psdCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_psdCheckBox, 2, 0);

            m_ddsCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_ddsCheckBox, 2, 1);

            m_tgaCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchFileTypeTableLayoutPanel.Controls.Add(m_tgaCheckBox, 2, 2);

            m_searchSystemCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchTableLayoutPanel.Controls.Add(m_searchSystemCheckBox, 0, 1);

            m_searchHiddenCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchTableLayoutPanel.Controls.Add(m_searchHiddenCheckBox, 0, 2);

            m_searchSubFoldersCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            searchTableLayoutPanel.Controls.Add(m_searchSubFoldersCheckBox, 0, 3);
        }

        private void InitilizeAdvancedTabPage()
        {
            m_advancedTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_advancedTabPage);

            TableLayoutPanel advancedTableLayoutPanel = InitFactory.Layout.Create(1, 10, 5);
            m_advancedTabPage.Controls.Add(advancedTableLayoutPanel);

            m_deleteToRecycleBinCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            advancedTableLayoutPanel.Controls.Add(m_deleteToRecycleBinCheckBox, 0, 1);

            m_mistakeDataBaseCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            advancedTableLayoutPanel.Controls.Add(m_mistakeDataBaseCheckBox, 0, 2);

            m_ratioResolutionLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_ratioResolutionLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(8, "1/8"));
            m_ratioResolutionLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(16, "1/16"));
            m_ratioResolutionLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(32, "1/32"));
            m_ratioResolutionLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(64, "1/64"));
            advancedTableLayoutPanel.Controls.Add(m_ratioResolutionLabeledComboBox, 0, 3);

            m_compareThreadCountLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_compareThreadCountLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(0, "auto"));
            for (int i = 0; i < Environment.ProcessorCount; i ++)
                m_compareThreadCountLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i + 1, (i + 1).ToString()));
            advancedTableLayoutPanel.Controls.Add(m_compareThreadCountLabeledComboBox, 0, 4);

            m_collectThreadCountLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_collectThreadCountLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(0, "auto"));
            for (int i = 0; i < Environment.ProcessorCount; i++)
                m_collectThreadCountLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i + 1, (i + 1).ToString()));
            advancedTableLayoutPanel.Controls.Add(m_collectThreadCountLabeledComboBox, 0, 5);

            m_reducedImageSizeLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_reducedImageSizeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(16, "16x16"));
            m_reducedImageSizeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(32, "32x32"));
            m_reducedImageSizeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(64, "64x64"));
            m_reducedImageSizeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(128, "128x128"));
            advancedTableLayoutPanel.Controls.Add(m_reducedImageSizeLabeledComboBox, 0, 6);

            m_undoQueueSizeLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_undoQueueSizeLabeledIntegerEdit.Min = 0;
            m_undoQueueSizeLabeledIntegerEdit.Default = m_defaultCoreOptions.advancedOptions.undoQueueSize;
            m_undoQueueSizeLabeledIntegerEdit.Max = 16;
            advancedTableLayoutPanel.Controls.Add(m_undoQueueSizeLabeledIntegerEdit, 0, 7);

            m_resultCountMaxLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_resultCountMaxLabeledIntegerEdit.Min = 1;
            m_resultCountMaxLabeledIntegerEdit.Default = m_defaultCoreOptions.advancedOptions.resultCountMax;
            advancedTableLayoutPanel.Controls.Add(m_resultCountMaxLabeledIntegerEdit, 0, 8);

            m_ignoreFrameWidthLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            advancedTableLayoutPanel.Controls.Add(m_ignoreFrameWidthLabeledComboBox, 0, 9);
        }

        private void GetOptions()
        {
            m_checkOnDefectCheckBox.Checked = m_newCoreOptions.checkOptions.checkOnDefect;
            m_checkOnEqualityCheckBox.Checked = m_newCoreOptions.checkOptions.checkOnEquality;
            m_transformedImageCheckBox.Checked = m_newCoreOptions.checkOptions.transformedImage;
            m_sizeControlCheckBox.Checked = m_newCoreOptions.checkOptions.sizeControl;
            m_typeControlCheckBox.Checked = m_newCoreOptions.checkOptions.typeControl;
            m_ratioControlCheckBox.Checked = m_newCoreOptions.checkOptions.ratioControl;
            m_thresholdDifferenceLabeledComboBox.SelectedValue = m_newCoreOptions.checkOptions.thresholdDifference;
            m_minimalImageSizeLabeledIntegerEdit.Value = m_newCoreOptions.checkOptions.minimalImageSize;
            m_compareInsideOneFolderCheckBox.Checked = m_newCoreOptions.checkOptions.compareInsideOneFolder;

            m_bmpCheckBox.Checked = m_newCoreOptions.searchOptions.BMP;
            m_gifCheckBox.Checked = m_newCoreOptions.searchOptions.GIF;
            m_jpegCheckBox.Checked = m_newCoreOptions.searchOptions.JPEG;
            m_pngCheckBox.Checked = m_newCoreOptions.searchOptions.PNG;
            m_tiffCheckBox.Checked = m_newCoreOptions.searchOptions.TIFF;
            m_emfCheckBox.Checked = m_newCoreOptions.searchOptions.EMF;
            m_wmfCheckBox.Checked = m_newCoreOptions.searchOptions.WMF;
            m_exifCheckBox.Checked = m_newCoreOptions.searchOptions.EXIF;
            m_iconCheckBox.Checked = m_newCoreOptions.searchOptions.ICON;
            m_jp2CheckBox.Checked = m_newCoreOptions.searchOptions.JP2;
            m_psdCheckBox.Checked = m_newCoreOptions.searchOptions.PSD;
            m_ddsCheckBox.Checked = m_newCoreOptions.searchOptions.DDS;
            m_tgaCheckBox.Checked = m_newCoreOptions.searchOptions.TGA;
            m_searchSystemCheckBox.Checked = m_newCoreOptions.searchOptions.system;
            m_searchHiddenCheckBox.Checked = m_newCoreOptions.searchOptions.hidden;
            m_searchSubFoldersCheckBox.Checked = m_newCoreOptions.searchOptions.subFolders;

            m_deleteToRecycleBinCheckBox.Checked = m_newCoreOptions.advancedOptions.deleteToRecycleBin;
            m_mistakeDataBaseCheckBox.Checked = m_newCoreOptions.advancedOptions.mistakeDataBase;
            m_ratioResolutionLabeledComboBox.SelectedValue = m_newCoreOptions.advancedOptions.ratioResolution;
            m_compareThreadCountLabeledComboBox.SelectedValue = m_newCoreOptions.advancedOptions.compareThreadCount;
            m_collectThreadCountLabeledComboBox.SelectedValue = m_newCoreOptions.advancedOptions.collectThreadCount;
            m_reducedImageSizeLabeledComboBox.SelectedValue = m_newCoreOptions.advancedOptions.reducedImageSize;
            m_undoQueueSizeLabeledIntegerEdit.Value = m_newCoreOptions.advancedOptions.undoQueueSize;
            m_resultCountMaxLabeledIntegerEdit.Value = m_newCoreOptions.advancedOptions.resultCountMax;
            m_ignoreFrameWidthLabeledComboBox.SelectedValue = m_newCoreOptions.advancedOptions.ignoreFrameWidth;
        }

        private void SetOptions()
        {
            m_newCoreOptions.checkOptions.checkOnDefect = m_checkOnDefectCheckBox.Checked;
            m_newCoreOptions.checkOptions.checkOnEquality = m_checkOnEqualityCheckBox.Checked;
            m_newCoreOptions.checkOptions.transformedImage = m_transformedImageCheckBox.Checked;
            m_newCoreOptions.checkOptions.sizeControl = m_sizeControlCheckBox.Checked;
            m_newCoreOptions.checkOptions.typeControl = m_typeControlCheckBox.Checked;
            m_newCoreOptions.checkOptions.ratioControl = m_ratioControlCheckBox.Checked;
            m_newCoreOptions.checkOptions.thresholdDifference = m_thresholdDifferenceLabeledComboBox.SelectedValue;
            m_newCoreOptions.checkOptions.minimalImageSize = m_minimalImageSizeLabeledIntegerEdit.Value;
            m_newCoreOptions.checkOptions.compareInsideOneFolder = m_compareInsideOneFolderCheckBox.Checked;

            m_newCoreOptions.searchOptions.BMP = m_bmpCheckBox.Checked;
            m_newCoreOptions.searchOptions.GIF = m_gifCheckBox.Checked;
            m_newCoreOptions.searchOptions.JPEG = m_jpegCheckBox.Checked;
            m_newCoreOptions.searchOptions.PNG = m_pngCheckBox.Checked;
            m_newCoreOptions.searchOptions.TIFF = m_tiffCheckBox.Checked;
            m_newCoreOptions.searchOptions.EMF = m_emfCheckBox.Checked;
            m_newCoreOptions.searchOptions.WMF = m_wmfCheckBox.Checked;
            m_newCoreOptions.searchOptions.EXIF = m_exifCheckBox.Checked;
            m_newCoreOptions.searchOptions.ICON = m_iconCheckBox.Checked;
            m_newCoreOptions.searchOptions.JP2 = m_jp2CheckBox.Checked;
            m_newCoreOptions.searchOptions.PSD = m_psdCheckBox.Checked;
            m_newCoreOptions.searchOptions.DDS = m_ddsCheckBox.Checked;
            m_newCoreOptions.searchOptions.TGA = m_tgaCheckBox.Checked;
            m_newCoreOptions.searchOptions.system = m_searchSystemCheckBox.Checked;
            m_newCoreOptions.searchOptions.hidden = m_searchHiddenCheckBox.Checked;
            m_newCoreOptions.searchOptions.subFolders = m_searchSubFoldersCheckBox.Checked;

            m_newCoreOptions.advancedOptions.deleteToRecycleBin = m_deleteToRecycleBinCheckBox.Checked;
            m_newCoreOptions.advancedOptions.mistakeDataBase = m_mistakeDataBaseCheckBox.Checked;
            m_newCoreOptions.advancedOptions.ratioResolution = m_ratioResolutionLabeledComboBox.SelectedValue;
            m_newCoreOptions.advancedOptions.compareThreadCount = m_compareThreadCountLabeledComboBox.SelectedValue;
            m_newCoreOptions.advancedOptions.collectThreadCount = m_collectThreadCountLabeledComboBox.SelectedValue;
            m_newCoreOptions.advancedOptions.reducedImageSize = m_reducedImageSizeLabeledComboBox.SelectedValue;
            m_newCoreOptions.advancedOptions.undoQueueSize = m_undoQueueSizeLabeledIntegerEdit.Value;
            m_newCoreOptions.advancedOptions.resultCountMax = m_resultCountMaxLabeledIntegerEdit.Value;
            m_newCoreOptions.advancedOptions.ignoreFrameWidth = m_ignoreFrameWidthLabeledComboBox.SelectedValue;
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.CoreOptionsForm_Text;
            m_okButton.Text = s.OkButton_Text;
            m_cancelButton.Text = s.CancelButton_Text;
            m_setDefaultButton.Text = s.SetDefaultButton_Text;

            m_checkTabPage.Text = s.CoreOptionsForm_CheckTabPage_Text;
            m_checkOnDefectCheckBox.Text = s.CoreOptionsForm_CheckOnDefectCheckBox_Text;
            m_checkOnEqualityCheckBox.Text = s.CoreOptionsForm_CheckOnEqualityCheckBox_Text;
            m_transformedImageCheckBox.Text = s.CoreOptionsForm_TransformedImageCheckBox_Text;
            m_sizeControlCheckBox.Text = s.CoreOptionsForm_SizeControlCheckBox_Text;
            m_typeControlCheckBox.Text = s.CoreOptionsForm_TypeControlCheckBox_Text;
            m_ratioControlCheckBox.Text = s.CoreOptionsForm_RatioControlCheckBox_Text;
            m_thresholdDifferenceLabeledComboBox.Text = s.CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text;
            m_minimalImageSizeLabeledIntegerEdit.Text = s.CoreOptionsForm_MinimalImageSizeLabeledIntegerEdit_Text;
            m_compareInsideOneFolderCheckBox.Text = s.CoreOptionsForm_CompareInsideOneFolderCheckBox_Text;

            m_searchTabPage.Text = s.CoreOptionsForm_SearchTabPage_Text;
            m_searchFileTypeGroupBox.Text = s.CoreOptionsForm_SearchFileTypeGroupBox_Text;
            m_bmpCheckBox.Text = s.CoreOptionsForm_BmpCheckBox_Text;
            m_gifCheckBox.Text = s.CoreOptionsForm_GifCheckBox_Text;
            m_jpegCheckBox.Text = s.CoreOptionsForm_JpegCheckBox_Text;
            m_pngCheckBox.Text = s.CoreOptionsForm_PngCheckBox_Text;
            m_tiffCheckBox.Text = s.CoreOptionsForm_TiffCheckBox_Text;
            m_emfCheckBox.Text = s.CoreOptionsForm_EmfCheckBox_Text;
            m_wmfCheckBox.Text = s.CoreOptionsForm_WmfCheckBox_Text;
            m_exifCheckBox.Text = s.CoreOptionsForm_ExifCheckBox_Text;
            m_iconCheckBox.Text = s.CoreOptionsForm_IconCheckBox_Text;
            m_jp2CheckBox.Text = s.CoreOptionsForm_Jp2CheckBox_Text;
            m_psdCheckBox.Text = s.CoreOptionsForm_PsdCheckBox_Text;
            m_ddsCheckBox.Text = s.CoreOptionsForm_DdsCheckBox_Text;
            m_tgaCheckBox.Text = s.CoreOptionsForm_TgaCheckBox_Text;
            m_searchSystemCheckBox.Text = s.CoreOptionsForm_SearchSystemCheckBox_Text;
            m_searchHiddenCheckBox.Text = s.CoreOptionsForm_SearchHiddenCheckBox_Text;
            m_searchSubFoldersCheckBox.Text = s.CoreOptionsForm_SearchSubFoldersCheckBox_Text;

            m_advancedTabPage.Text = s.CoreOptionsForm_AdvancedTabPage_Text;
            m_deleteToRecycleBinCheckBox.Text = s.CoreOptionsForm_DeleteToRecycleBinCheckBox_Text;
            m_mistakeDataBaseCheckBox.Text = s.CoreOptionsForm_MistakeDataBaseCheckBox_Text;
            m_ratioResolutionLabeledComboBox.Text = s.CoreOptionsForm_RatioResolutionLabeledComboBox_Text;
            m_compareThreadCountLabeledComboBox.Text = s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Text;
            m_compareThreadCountLabeledComboBox.SetDescription(0, s.CoreOptionsForm_CompareThreadCountLabeledComboBox_Description_0);
            m_collectThreadCountLabeledComboBox.Text = s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Text;
            m_collectThreadCountLabeledComboBox.SetDescription(0, s.CoreOptionsForm_CollectThreadCountLabeledComboBox_Description_0);
            m_reducedImageSizeLabeledComboBox.Text = s.CoreOptionsForm_ReducedImageSizeLabeledComboBox_Text;
            m_undoQueueSizeLabeledIntegerEdit.Text = s.CoreOptionsForm_UndoQueueSizeLabeledIntegerEdit_Text;
            m_resultCountMaxLabeledIntegerEdit.Text = s.CoreOptionsForm_ResultCountMaxLabeledIntegerEdit_Text;
            m_ignoreFrameWidthLabeledComboBox.Text = s.CoreOptionsForm_IgnoreFrameWidthLabeledComboBox_Text;
        }

        private void UpdateItemsEnabling()
        {
            if (!(m_newCoreOptions.checkOptions.checkOnDefect ||
              m_newCoreOptions.checkOptions.checkOnEquality))
            {
                m_inited = false;
                m_newCoreOptions.checkOptions.checkOnEquality = true;
                GetOptions();
                m_inited = true;
            }

            if (!(m_newCoreOptions.searchOptions.BMP ||
              m_newCoreOptions.searchOptions.GIF ||
              m_newCoreOptions.searchOptions.JPEG ||
              m_newCoreOptions.searchOptions.PNG ||
              m_newCoreOptions.searchOptions.TIFF ||
              m_newCoreOptions.searchOptions.EMF ||
              m_newCoreOptions.searchOptions.WMF ||
              m_newCoreOptions.searchOptions.EXIF ||
              m_newCoreOptions.searchOptions.ICON ||
              m_newCoreOptions.searchOptions.JP2 ||
              m_newCoreOptions.searchOptions.PSD ||
              m_newCoreOptions.searchOptions.DDS ||
              m_newCoreOptions.searchOptions.TGA))
            {
                m_inited = false;
                m_newCoreOptions.searchOptions.JPEG = true;
                GetOptions();
                m_inited = true;
            }

            m_okButton.Enabled = !m_oldCoreOptions.Equals(m_newCoreOptions);
            m_setDefaultButton.Enabled = !m_defaultCoreOptions.Equals(m_newCoreOptions);

            m_transformedImageCheckBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_sizeControlCheckBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_typeControlCheckBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_ratioControlCheckBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality &&
              !m_newCoreOptions.checkOptions.sizeControl;
            m_thresholdDifferenceLabeledComboBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_minimalImageSizeLabeledIntegerEdit.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_compareInsideOneFolderCheckBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;

            m_ratioResolutionLabeledComboBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality &&
              !m_newCoreOptions.checkOptions.sizeControl && m_newCoreOptions.checkOptions.ratioControl;
            m_compareThreadCountLabeledComboBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_reducedImageSizeLabeledComboBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;
            m_ignoreFrameWidthLabeledComboBox.Enabled = m_newCoreOptions.checkOptions.checkOnEquality;

            int step = Math.Max(1, 64 / m_newCoreOptions.advancedOptions.reducedImageSize) * IGNORE_FRAME_WIDTH_STEP;
            if (m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Count != IGNORE_FRAME_WIDTH_MAX/step + 1)
            {
                m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Clear();
                for (int i = 0; i <= IGNORE_FRAME_WIDTH_MAX; i += step)
                    m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
                m_ignoreFrameWidthLabeledComboBox.SelectedValue = (m_newCoreOptions.advancedOptions.ignoreFrameWidth + step - 1) / step * step;
            }
        }

        private void OnOptionChanged(object sender, EventArgs e)
        {
            if (m_inited)
            {
                SetOptions();
                UpdateItemsEnabling();
            }
        }

        private void OnOkButtonClick(object sender, EventArgs e)
        {
            SetOptions();
            m_newCoreOptions.CopyTo(ref m_oldCoreOptions);
            m_oldCoreOptions.Validate(m_core, m_options.onePath);
            m_options.Change();
            Close();
        }

        private void OnCancelButtonClick(object sender, EventArgs e)
        {
            Close();
        }

        private void OnSetDefaultButtonClick(object sender, EventArgs e)
        {
            m_defaultCoreOptions.CopyTo(ref m_newCoreOptions);
            m_inited = false;
            GetOptions();
            m_inited = true;
            UpdateItemsEnabling();
        }
    }
}
