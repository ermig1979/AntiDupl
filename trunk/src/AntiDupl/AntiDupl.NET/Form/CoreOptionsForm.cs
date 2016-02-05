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
using System.Windows.Forms;
using System.IO;

namespace AntiDupl.NET
{
    public class CoreOptionsForm : Form
    {
        static public int THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM = 15;
        static public int THRESHOLD_DIFFERENCE_MAX_SSIM = 50;
        static public int THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM = 5;
        static public int THRESHOLD_DIFFERENCE_DEFAULT_SSIM = 20;
        static public int THRESHOLD_BLOCKINESS_MAX = 60;
        static public int THRESHOLD_BLURRING_MAX = 32;
        static public int IGNORE_FRAME_WIDTH_MAX = 12;
        static public int IGNORE_FRAME_WIDTH_STEP = 3;
        
        static public int FORM_WIDTH = 450;
        static public int FORM_HEIGHT = 400;
        static public int COMBO_BOX_WIDTH = 65;
        static public int COMBO_BOX_HEIGHT = 20;

        private CoreLib m_core;
        private Options m_options;
        private CoreOptions m_oldCoreOptions;
        private CoreOptions m_newCoreOptions;
        private CoreOptions m_defaultCoreOptions;
        private ResultsOptions m_oldResultsOptions;

        private Button m_okButton;
        private Button m_cancelButton;
        private Button m_setDefaultButton;
        private TabControl m_mainTabControl;

        private TabPage m_compareTabPage;
        private CheckBox m_checkOnEqualityCheckBox;
        private CheckBox m_transformedImageCheckBox;
        private CheckBox m_sizeControlCheckBox;
        private CheckBox m_typeControlCheckBox;
        private CheckBox m_ratioControlCheckBox;
        private LabeledComboBox m_algorithmComparingLabeledComboBox;
        private LabeledComboBox m_thresholdDifferenceLabeledComboBox;
        private LabeledIntegerEdit m_minimalImageSizeLabeledIntegerEdit;
        private LabeledIntegerEdit m_maximalImageSizeLabeledIntegerEdit;
        private CheckBox m_compareInsideOneFolderCheckBox;
        private CheckBox m_compareInsideOneSearchPathCheckBox;

        private TabPage m_defectTabPage;
        private CheckBox m_checkOnDefectCheckBox;
        private CheckBox m_checkOnBlockinessCheckBox;
        private LabeledComboBox m_blockinessThresholdLabeledComboBox;
        private CheckBox m_checkOnBlockinessOnlyNotJpegCheckBox;
        private CheckBox m_checkOnBlurringCheckBox;
        private LabeledComboBox m_blurringThresholdLabeledComboBox;

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

        private TabPage m_highlightTabPage;
        private CheckBox m_highlightDifferenceCheckBox;
        private LabeledNumericUpDown m_difrentValue;
        private CheckBox m_notHighlightIfFragmentsMoreThemCheckBox;
        private LabeledIntegerEdit m_maxFragmentsForDisableHighlightLabeledIntegerEdit;
        private CheckBox m_highlightAllDifferencesCheckBox;
        private LabeledIntegerEdit m_maxFragmentsForHighlightLabeledIntegerEdit;
        private LabeledIntegerEdit m_amountOfFragmentsOnXLabeledIntegerEdit;
        private LabeledIntegerEdit m_amountOfFragmentsOnYLabeledIntegerEdit;
        private LabeledIntegerEdit m_normalizedSizeOfImageLabeledIntegerEdit;
        private LabeledIntegerEdit m_penThicknessLabeledIntegerEdit;

        private TabPage m_hintTabPage;
        private LabeledComboBox m_hintTypeLabeledComboBox;
        private Button m_trainButton;
            
        /// <summary>
        /// Все компоненты иницализированы.
        /// </summary>
        private bool m_inited = false;

        public CoreOptionsForm(CoreLib core, Options options, CoreOptions coreOptions)
        {
            m_core = core;
            m_options = options;
            m_oldCoreOptions = coreOptions; //old options - cancel
            m_newCoreOptions = m_oldCoreOptions.Clone();  //new created options
            m_defaultCoreOptions = new CoreOptions(m_core, m_options.onePath); //default options
            m_oldResultsOptions = m_options.resultsOptions.Clone();
            InitializeComponent();
            UpdateStrings();
            GetOptions();
            UpdateItemsEnabling();
            m_inited = true;
        }

        void InitializeComponent()
        {
            ClientSize = new System.Drawing.Size(FORM_WIDTH, FORM_HEIGHT);
            FormBorderStyle = FormBorderStyle.Sizable;
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

            InitilizeCompareTabPage();

            InitilizeDefectTabPage();

            InitilizeSearchTabPage();

            InitilizeAdvancedTabPage();

            InitilizeHighlightTabPage();

            InitilizeHintTabPage();

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
            m_setDefaultButton.Width = 100;
            m_setDefaultButton.Click += new EventHandler(OnSetDefaultButtonClick);
            mainButtonsTableLayoutPanel.Controls.Add(m_setDefaultButton, 3, 0);
        }

        private void InitilizeCompareTabPage()
        {
            m_compareTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_compareTabPage);

            TableLayoutPanel checkTableLayoutPanel = InitFactory.Layout.Create(1, 10, 5); //column, row, padding
            checkTableLayoutPanel.AutoScroll = true;
            m_compareTabPage.Controls.Add(checkTableLayoutPanel);

            m_checkOnEqualityCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_checkOnEqualityCheckBox, 0, 0); //control, column, row

            m_transformedImageCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_transformedImageCheckBox, 0, 1);

            m_sizeControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_sizeControlCheckBox, 0, 2);

            m_typeControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_typeControlCheckBox, 0, 3);

            m_ratioControlCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_ratioControlCheckBox, 0, 4);

            m_algorithmComparingLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH + 115, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_algorithmComparingLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(0, Resources.Strings.Current.CoreOptionsForm_AlgorithmComparingLabeledComboBox_SquaredSum));
            m_algorithmComparingLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(1, "SSIM"));
            checkTableLayoutPanel.Controls.Add(m_algorithmComparingLabeledComboBox, 0, 5);

            m_thresholdDifferenceLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            if (m_newCoreOptions.compareOptions.algorithmComparing == CoreDll.AlgorithmComparing.SquaredSum)
                for (int i = 0; i <= THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM; i++)
                    m_thresholdDifferenceLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
            else
                for (int i = 0; i <= THRESHOLD_DIFFERENCE_MAX_SSIM; i++)
                    m_thresholdDifferenceLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
            checkTableLayoutPanel.Controls.Add(m_thresholdDifferenceLabeledComboBox, 0, 6);

            m_minimalImageSizeLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_minimalImageSizeLabeledIntegerEdit.Min = 0;
            m_minimalImageSizeLabeledIntegerEdit.Default = m_defaultCoreOptions.compareOptions.minimalImageSize;
            checkTableLayoutPanel.Controls.Add(m_minimalImageSizeLabeledIntegerEdit, 0, 7);

            m_maximalImageSizeLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_maximalImageSizeLabeledIntegerEdit.Min = 0;
            m_maximalImageSizeLabeledIntegerEdit.Default = m_defaultCoreOptions.compareOptions.maximalImageSize;
            checkTableLayoutPanel.Controls.Add(m_maximalImageSizeLabeledIntegerEdit, 0, 8);

            m_compareInsideOneFolderCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_compareInsideOneFolderCheckBox, 0, 9);
            m_compareInsideOneSearchPathCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            checkTableLayoutPanel.Controls.Add(m_compareInsideOneSearchPathCheckBox, 0, 10);
        }

        private void InitilizeDefectTabPage()
        {
            m_defectTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_defectTabPage);

            TableLayoutPanel defectTableLayoutPanel = InitFactory.Layout.Create(1, 6, 5); //column, row, padding
            defectTableLayoutPanel.AutoScroll = true;
            m_defectTabPage.Controls.Add(defectTableLayoutPanel);

            m_checkOnDefectCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            defectTableLayoutPanel.Controls.Add(m_checkOnDefectCheckBox, 0, 0);

            m_checkOnBlockinessCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            defectTableLayoutPanel.Controls.Add(m_checkOnBlockinessCheckBox, 0, 1);

            m_blockinessThresholdLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            for (int i = 0; i <= THRESHOLD_BLOCKINESS_MAX; i++)
                m_blockinessThresholdLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0}", i)));
            defectTableLayoutPanel.Controls.Add(m_blockinessThresholdLabeledComboBox, 0, 2);

            m_checkOnBlockinessOnlyNotJpegCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            defectTableLayoutPanel.Controls.Add(m_checkOnBlockinessOnlyNotJpegCheckBox, 0, 3);

            m_checkOnBlurringCheckBox = InitFactory.CheckBox.Create(OnOptionChanged);
            defectTableLayoutPanel.Controls.Add(m_checkOnBlurringCheckBox, 0, 4);

            m_blurringThresholdLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnOptionChanged);
            for (int i = 0; i <= THRESHOLD_BLURRING_MAX; i++)
                m_blurringThresholdLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0}", i)));
            defectTableLayoutPanel.Controls.Add(m_blurringThresholdLabeledComboBox, 0, 5);
        }

        private void InitilizeSearchTabPage()
        {
            m_searchTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_searchTabPage);

            TableLayoutPanel searchTableLayoutPanel = InitFactory.Layout.Create(1, 4, 5);
            searchTableLayoutPanel.AutoScroll = true;
            m_searchTabPage.Controls.Add(searchTableLayoutPanel);

            m_searchFileTypeGroupBox = new GroupBox();
            m_searchFileTypeGroupBox.Size = new System.Drawing.Size(200, 140);
            searchTableLayoutPanel.Controls.Add(m_searchFileTypeGroupBox, 0, 0);

            TableLayoutPanel searchFileTypeTableLayoutPanel = InitFactory.Layout.Create(3, 5, 5);
            searchFileTypeTableLayoutPanel.AutoScroll = true;
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
        }

        private void InitilizeAdvancedTabPage()
        {
            m_advancedTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_advancedTabPage);

            TableLayoutPanel advancedTableLayoutPanel = InitFactory.Layout.Create(1, 10, 5);
            advancedTableLayoutPanel.AutoScroll = true;
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

        private void InitilizeHighlightTabPage()
        {
            m_highlightTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_highlightTabPage);

            TableLayoutPanel highlightTableLayoutPanel = InitFactory.Layout.Create(1, 10, 5);
            highlightTableLayoutPanel.AutoScroll = true;
            m_highlightTabPage.Controls.Add(highlightTableLayoutPanel);

            m_highlightDifferenceCheckBox = InitFactory.CheckBox.Create(OnHighlightChanged);
            m_highlightDifferenceCheckBox.Checked = m_options.resultsOptions.HighlightDifference;
            highlightTableLayoutPanel.Controls.Add(m_highlightDifferenceCheckBox, 0, 1);

            m_difrentValue = new LabeledNumericUpDown(2, 
                                                new decimal(new int[] { 1, 0, 0, 65536}), 
                                                new decimal(0), 
                                                new decimal(m_options.resultsOptions.DifferenceThreshold), 
                                                OnHighlightChanged);
            /*m_difrentNumericUpDown = new System.Windows.Forms.NumericUpDown();
            m_difrentNumericUpDown.Margin = new Padding(0);
            m_difrentNumericUpDown.DecimalPlaces = 2;
            m_difrentNumericUpDown.Increment = new decimal(new int[] { 1, 0, 0, 65536});
            m_difrentNumericUpDown.Minimum = new decimal(0);
            m_difrentNumericUpDown.Value = new decimal(m_options.resultsOptions.DiffrentThreshold);
            m_difrentNumericUpDown.ValueChanged += new System.EventHandler(OnHighlightChanged);*/
            highlightTableLayoutPanel.Controls.Add(m_difrentValue, 0, 2);

            m_notHighlightIfFragmentsMoreThemCheckBox = InitFactory.CheckBox.Create(OnHighlightChanged);
            m_notHighlightIfFragmentsMoreThemCheckBox.Checked = m_options.resultsOptions.NotHighlightIfFragmentsMoreThan;
            highlightTableLayoutPanel.Controls.Add(m_notHighlightIfFragmentsMoreThemCheckBox, 0, 3);

            m_maxFragmentsForDisableHighlightLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Min = 0;
            m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Max = 4000;
            m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Value = m_options.resultsOptions. NotHighlightMaxFragments;
            highlightTableLayoutPanel.Controls.Add(m_maxFragmentsForDisableHighlightLabeledIntegerEdit, 0, 4);

            m_highlightAllDifferencesCheckBox = InitFactory.CheckBox.Create(OnHighlightChanged);
            m_highlightAllDifferencesCheckBox.Checked = m_options.resultsOptions.HighlightAllDifferences;
            highlightTableLayoutPanel.Controls.Add(m_highlightAllDifferencesCheckBox, 0, 5);

            m_maxFragmentsForHighlightLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_maxFragmentsForHighlightLabeledIntegerEdit.Min = 0;
            m_maxFragmentsForHighlightLabeledIntegerEdit.Max = 4000;
            m_maxFragmentsForHighlightLabeledIntegerEdit.Value = m_options.resultsOptions.MaxFragmentsForHighlight;
            highlightTableLayoutPanel.Controls.Add(m_maxFragmentsForHighlightLabeledIntegerEdit, 0, 6);

            m_amountOfFragmentsOnXLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_amountOfFragmentsOnXLabeledIntegerEdit.Min = 0;
            m_amountOfFragmentsOnXLabeledIntegerEdit.Max = 100;
            m_amountOfFragmentsOnXLabeledIntegerEdit.Value = m_options.resultsOptions.AmountOfFragmentsOnX;
            highlightTableLayoutPanel.Controls.Add(m_amountOfFragmentsOnXLabeledIntegerEdit, 0, 7);

            m_amountOfFragmentsOnYLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_amountOfFragmentsOnYLabeledIntegerEdit.Min = 0;
            m_amountOfFragmentsOnYLabeledIntegerEdit.Max = 100;
            m_amountOfFragmentsOnYLabeledIntegerEdit.Value = m_options.resultsOptions.AmountOfFragmentsOnY;
            highlightTableLayoutPanel.Controls.Add(m_amountOfFragmentsOnYLabeledIntegerEdit, 0, 8);

            m_normalizedSizeOfImageLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_normalizedSizeOfImageLabeledIntegerEdit.Min = 0;
            m_normalizedSizeOfImageLabeledIntegerEdit.Max = 2048;
            m_normalizedSizeOfImageLabeledIntegerEdit.Value = m_options.resultsOptions.NormalizedSizeOfImage;
            highlightTableLayoutPanel.Controls.Add(m_normalizedSizeOfImageLabeledIntegerEdit, 0, 9);

            m_penThicknessLabeledIntegerEdit = new LabeledIntegerEdit(COMBO_BOX_WIDTH, COMBO_BOX_HEIGHT, OnHighlightChanged);
            m_penThicknessLabeledIntegerEdit.Min = 0;
            m_penThicknessLabeledIntegerEdit.Max = 100;
            m_penThicknessLabeledIntegerEdit.Value = m_options.resultsOptions.PenThickness;
            highlightTableLayoutPanel.Controls.Add(m_penThicknessLabeledIntegerEdit, 0, 10);

            UpdateHighlightItemsEnabling();
        }

        private void InitilizeHintTabPage()
        {
            m_hintTabPage = new TabPage();
            m_mainTabControl.Controls.Add(m_hintTabPage);

            TableLayoutPanel tableLayoutPanel = InitFactory.Layout.Create(1, 3, 5);
            tableLayoutPanel.AutoScroll = true;
            m_hintTabPage.Controls.Add(tableLayoutPanel);

            m_hintTypeLabeledComboBox = new LabeledComboBox(COMBO_BOX_WIDTH + 115, COMBO_BOX_HEIGHT, OnOptionChanged);
            m_hintTypeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(0, "Algo"));
            m_hintTypeLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(1, "Neural Network"));
            tableLayoutPanel.Controls.Add(m_hintTypeLabeledComboBox, 0, 0);

            m_trainButton = new Button();
            m_trainButton.Click += new EventHandler(OnTrainButtonClick);
            tableLayoutPanel.Controls.Add(m_trainButton, 0, 1);
        }

        private void OnTrainButtonClick(object sender, EventArgs e)
        {
            //MessageBox.Show("Train started!");
            m_core.TrainNeuralNetwork();
            //MessageBox.Show("Train started!");
        }

        private void OnHighlightChanged(object sender, EventArgs e)
        {
            if (m_inited)
            {
                m_options.resultsOptions.HighlightDifference = m_highlightDifferenceCheckBox.Checked;
                m_options.resultsOptions.DifferenceThreshold = m_difrentValue.Value;
                m_options.resultsOptions.NotHighlightIfFragmentsMoreThan = m_notHighlightIfFragmentsMoreThemCheckBox.Checked;
                m_options.resultsOptions.NotHighlightMaxFragments = m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Value;
                m_options.resultsOptions.HighlightAllDifferences = m_highlightAllDifferencesCheckBox.Checked;
                m_options.resultsOptions.MaxFragmentsForHighlight = m_maxFragmentsForHighlightLabeledIntegerEdit.Value;
                m_options.resultsOptions.AmountOfFragmentsOnX = m_amountOfFragmentsOnXLabeledIntegerEdit.Value;
                m_options.resultsOptions.AmountOfFragmentsOnY = m_amountOfFragmentsOnYLabeledIntegerEdit.Value;
                m_options.resultsOptions.NormalizedSizeOfImage = m_normalizedSizeOfImageLabeledIntegerEdit.Value;
                m_options.resultsOptions.PenThickness = m_penThicknessLabeledIntegerEdit.Value;
                UpdateHighlightItemsEnabling();
                m_options.resultsOptions.RaiseEventOnHighlightDifferenceChange();
            }
        }

        /// <summary>
        /// Проверка возмодных состояний настроек.
        /// </summary>
        private void UpdateHighlightItemsEnabling()
        {
            if (m_highlightDifferenceCheckBox.Checked)
            {
                m_difrentValue.Enabled = true;
                m_highlightAllDifferencesCheckBox.Enabled = true;
                m_notHighlightIfFragmentsMoreThemCheckBox.Enabled = true;
                m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Enabled = m_notHighlightIfFragmentsMoreThemCheckBox.Checked;
                m_maxFragmentsForHighlightLabeledIntegerEdit.Enabled = !m_highlightAllDifferencesCheckBox.Checked;
                m_amountOfFragmentsOnXLabeledIntegerEdit.Enabled = true;
                m_amountOfFragmentsOnYLabeledIntegerEdit.Enabled = true;
                m_normalizedSizeOfImageLabeledIntegerEdit.Enabled = true;
                m_penThicknessLabeledIntegerEdit.Enabled = true;
            }
            else 
            {
                m_difrentValue.Enabled = false;
                m_highlightAllDifferencesCheckBox.Enabled = false;
                m_notHighlightIfFragmentsMoreThemCheckBox.Enabled = false;
                m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Enabled = false;
                m_maxFragmentsForHighlightLabeledIntegerEdit.Enabled = false;
                m_amountOfFragmentsOnXLabeledIntegerEdit.Enabled = false;
                m_amountOfFragmentsOnYLabeledIntegerEdit.Enabled = false;
                m_normalizedSizeOfImageLabeledIntegerEdit.Enabled = false;
                m_penThicknessLabeledIntegerEdit.Enabled = false;
            }
        }

        /// <summary>
        /// Чтение опций из CoreOptions.
        /// </summary>
        private void GetOptions()
        {
            m_checkOnEqualityCheckBox.Checked = m_newCoreOptions.compareOptions.checkOnEquality;
            m_transformedImageCheckBox.Checked = m_newCoreOptions.compareOptions.transformedImage;
            m_sizeControlCheckBox.Checked = m_newCoreOptions.compareOptions.sizeControl;
            m_typeControlCheckBox.Checked = m_newCoreOptions.compareOptions.typeControl;
            m_ratioControlCheckBox.Checked = m_newCoreOptions.compareOptions.ratioControl;
            m_algorithmComparingLabeledComboBox.SelectedValue = (int)m_newCoreOptions.compareOptions.algorithmComparing;
            m_thresholdDifferenceLabeledComboBox.SelectedValue = m_newCoreOptions.compareOptions.thresholdDifference;
            m_minimalImageSizeLabeledIntegerEdit.Value = m_newCoreOptions.compareOptions.minimalImageSize;
            m_maximalImageSizeLabeledIntegerEdit.Value = m_newCoreOptions.compareOptions.maximalImageSize;
            m_compareInsideOneFolderCheckBox.Checked = m_newCoreOptions.compareOptions.compareInsideOneFolder;
            m_compareInsideOneSearchPathCheckBox.Checked = m_newCoreOptions.compareOptions.compareInsideOneSearchPath;

            m_checkOnDefectCheckBox.Checked = m_newCoreOptions.defectOptions.checkOnDefect;
            m_checkOnBlockinessCheckBox.Checked = m_newCoreOptions.defectOptions.checkOnBlockiness;
            m_blockinessThresholdLabeledComboBox.SelectedValue = m_newCoreOptions.defectOptions.blockinessThreshold;
			m_checkOnBlockinessOnlyNotJpegCheckBox.Checked = m_newCoreOptions.defectOptions.checkOnBlockinessOnlyNotJpeg;
            m_checkOnBlurringCheckBox.Checked = m_newCoreOptions.defectOptions.checkOnBlurring;
            m_blurringThresholdLabeledComboBox.SelectedValue = m_newCoreOptions.defectOptions.blurringThreshold;

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

        /// <summary>
        /// Установка опций в CoreOptions.
        /// </summary>
        private void SetOptions()
        {
            m_newCoreOptions.compareOptions.checkOnEquality = m_checkOnEqualityCheckBox.Checked;
            m_newCoreOptions.compareOptions.transformedImage = m_transformedImageCheckBox.Checked;
            m_newCoreOptions.compareOptions.sizeControl = m_sizeControlCheckBox.Checked;
            m_newCoreOptions.compareOptions.typeControl = m_typeControlCheckBox.Checked;
            m_newCoreOptions.compareOptions.ratioControl = m_ratioControlCheckBox.Checked;
            m_newCoreOptions.compareOptions.algorithmComparing = (CoreDll.AlgorithmComparing)m_algorithmComparingLabeledComboBox.SelectedValue;
            m_newCoreOptions.compareOptions.thresholdDifference = m_thresholdDifferenceLabeledComboBox.SelectedValue;
            m_newCoreOptions.compareOptions.minimalImageSize = m_minimalImageSizeLabeledIntegerEdit.Value;
            m_newCoreOptions.compareOptions.maximalImageSize = m_maximalImageSizeLabeledIntegerEdit.Value;
            m_newCoreOptions.compareOptions.compareInsideOneFolder = m_compareInsideOneFolderCheckBox.Checked;
            m_newCoreOptions.compareOptions.compareInsideOneSearchPath = m_compareInsideOneSearchPathCheckBox.Checked;

            m_newCoreOptions.defectOptions.checkOnDefect = m_checkOnDefectCheckBox.Checked;
            m_newCoreOptions.defectOptions.checkOnBlockiness = m_checkOnBlockinessCheckBox.Checked;
            m_newCoreOptions.defectOptions.blockinessThreshold = m_blockinessThresholdLabeledComboBox.SelectedValue;
			m_newCoreOptions.defectOptions.checkOnBlockinessOnlyNotJpeg = m_checkOnBlockinessOnlyNotJpegCheckBox.Checked;
            m_newCoreOptions.defectOptions.checkOnBlurring = m_checkOnBlurringCheckBox.Checked;
            m_newCoreOptions.defectOptions.blurringThreshold = m_blurringThresholdLabeledComboBox.SelectedValue;

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

            m_compareTabPage.Text = s.CoreOptionsForm_CompareTabPage_Text;
            m_checkOnEqualityCheckBox.Text = s.CoreOptionsForm_CheckOnEqualityCheckBox_Text;
            m_transformedImageCheckBox.Text = s.CoreOptionsForm_TransformedImageCheckBox_Text;
            m_sizeControlCheckBox.Text = s.CoreOptionsForm_SizeControlCheckBox_Text;
            m_typeControlCheckBox.Text = s.CoreOptionsForm_TypeControlCheckBox_Text;
            m_ratioControlCheckBox.Text = s.CoreOptionsForm_RatioControlCheckBox_Text;
            m_algorithmComparingLabeledComboBox.Text = s.CoreOptionsForm_AlgorithmComparingLabeledComboBox_Text;
            m_thresholdDifferenceLabeledComboBox.Text = s.CoreOptionsForm_ThresholdDifferenceLabeledComboBox_Text;
            m_minimalImageSizeLabeledIntegerEdit.Text = s.CoreOptionsForm_MinimalImageSizeLabeledIntegerEdit_Text;
            m_maximalImageSizeLabeledIntegerEdit.Text = s.CoreOptionsForm_MaximalImageSizeLabeledIntegerEdit_Text;
            m_compareInsideOneFolderCheckBox.Text = s.CoreOptionsForm_CompareInsideOneFolderCheckBox_Text;
            m_compareInsideOneSearchPathCheckBox.Text = s.CoreOptionsForm_CompareInsideOneSearchPathCheckBox_Text;

            m_defectTabPage.Text = s.CoreOptionsForm_DefectTabPage_Text;
            m_checkOnDefectCheckBox.Text = s.CoreOptionsForm_CheckOnDefectCheckBox_Text;
            m_checkOnBlockinessCheckBox.Text = s.CoreOptionsForm_CheckOnBlockinessCheckBox_Text;
            m_blockinessThresholdLabeledComboBox.Text = s.CoreOptionsForm_BlockinessThresholdLabeledComboBox_Text;
            m_checkOnBlockinessOnlyNotJpegCheckBox.Text = s.CoreOptionsForm_CheckOnBlockinessOnlyNotJpegCheckBox_Text;
            m_checkOnBlurringCheckBox.Text = s.CoreOptionsForm_CheckOnBlurringCheckBox_Text;
            m_blurringThresholdLabeledComboBox.Text = s.CoreOptionsForm_BlurringThresholdLabeledComboBox_Text;

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

            m_highlightTabPage.Text = s.CoreOptionsForm_HighlightTabPage_Text;
            m_highlightDifferenceCheckBox.Text = s.CoreOptionsForm_HighlightDifferenceCheckBox_Text;
            m_difrentValue.Text = s.CoreOptionsForm_DifrentValue_Text;
            m_notHighlightIfFragmentsMoreThemCheckBox.Text = s.CoreOptionsForm_NotHighlightIfFragmentsMoreThemCheckBox_Text;
            m_maxFragmentsForDisableHighlightLabeledIntegerEdit.Text = s.CoreOptionsForm_MaxFragmentsForDisableHighlightLabeledIntegerEdit_Text;
            m_highlightAllDifferencesCheckBox.Text = s.CoreOptionsForm_HighlightAllDifferencesCheckBox_Text;
            m_maxFragmentsForHighlightLabeledIntegerEdit.Text = s.CoreOptionsForm_MaxFragmentsForHighlightLabeledIntegerEdit_Text;
            m_amountOfFragmentsOnXLabeledIntegerEdit.Text = s.CoreOptionsForm_AmountOfFragmentsOnXLabeledIntegerEdit_Text;
            m_amountOfFragmentsOnYLabeledIntegerEdit.Text = s.CoreOptionsForm_AmountOfFragmentsOnYLabeledIntegerEdit_Text;
            m_normalizedSizeOfImageLabeledIntegerEdit.Text = s.CoreOptionsForm_NormalizedSizeOfImageLabeledIntegerEdit_Text;
            m_penThicknessLabeledIntegerEdit.Text = s.CoreOptionsForm_PenThicknessLabeledIntegerEdit_Text;

            m_hintTabPage.Text = "Подсказки";
        }

        /// <summary>
        /// Проверка возмодных состояний настроек.
        /// </summary>
        private void UpdateItemsEnabling()
        {
            // Если не проставлены галочки поиска дубликатов и дефектов, то проставляем сами
            if (!(m_newCoreOptions.defectOptions.checkOnDefect || m_newCoreOptions.compareOptions.checkOnEquality ||
                m_newCoreOptions.defectOptions.checkOnBlockiness || m_newCoreOptions.defectOptions.checkOnBlurring))
            {
                m_inited = false;
                m_newCoreOptions.compareOptions.checkOnEquality = true;
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

            m_transformedImageCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_sizeControlCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_typeControlCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_ratioControlCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality &&
              !m_newCoreOptions.compareOptions.sizeControl;
            m_thresholdDifferenceLabeledComboBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_compareInsideOneFolderCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_compareInsideOneSearchPathCheckBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;

            m_ratioResolutionLabeledComboBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality &&
              !m_newCoreOptions.compareOptions.sizeControl && m_newCoreOptions.compareOptions.ratioControl;
            m_compareThreadCountLabeledComboBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_reducedImageSizeLabeledComboBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;
            m_ignoreFrameWidthLabeledComboBox.Enabled = m_newCoreOptions.compareOptions.checkOnEquality;

            m_blockinessThresholdLabeledComboBox.Enabled = m_newCoreOptions.defectOptions.checkOnBlockiness;
			m_checkOnBlockinessOnlyNotJpegCheckBox.Enabled = m_newCoreOptions.defectOptions.checkOnBlockiness;

            m_blurringThresholdLabeledComboBox.Enabled = m_newCoreOptions.defectOptions.checkOnBlurring;

            int step = Math.Max(1, 64 / m_newCoreOptions.advancedOptions.reducedImageSize) * IGNORE_FRAME_WIDTH_STEP;
            if (m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Count != IGNORE_FRAME_WIDTH_MAX/step + 1)
            {
                m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Clear();
                for (int i = 0; i <= IGNORE_FRAME_WIDTH_MAX; i += step)
                    m_ignoreFrameWidthLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
                m_ignoreFrameWidthLabeledComboBox.SelectedValue = (m_newCoreOptions.advancedOptions.ignoreFrameWidth + step - 1) / step * step;
            }

            if (m_algorithmComparingLabeledComboBox.SelectedValue == (int)CoreDll.AlgorithmComparing.SquaredSum &&
                m_thresholdDifferenceLabeledComboBox.comboBox.Items.Count > THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM + 1)
            {
                m_thresholdDifferenceLabeledComboBox.comboBox.Items.Clear();
                for (int i = 0; i <= THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM; i++)
                    m_thresholdDifferenceLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
                m_thresholdDifferenceLabeledComboBox.SelectedValue = THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM;
            }
            if (m_algorithmComparingLabeledComboBox.SelectedValue == (int)CoreDll.AlgorithmComparing.SSIM &&
                m_thresholdDifferenceLabeledComboBox.comboBox.Items.Count < THRESHOLD_DIFFERENCE_MAX_SSIM + 1)
            {
                m_thresholdDifferenceLabeledComboBox.comboBox.Items.Clear();
                for (int i = 0; i <= THRESHOLD_DIFFERENCE_MAX_SSIM; i++)
                    m_thresholdDifferenceLabeledComboBox.comboBox.Items.Add(new LabeledComboBox.Value(i, string.Format("{0} %", i)));
                m_thresholdDifferenceLabeledComboBox.SelectedValue = THRESHOLD_DIFFERENCE_DEFAULT_SSIM;
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
            m_options.resultsOptions = m_oldResultsOptions;
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
