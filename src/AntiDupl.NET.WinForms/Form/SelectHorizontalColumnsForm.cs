/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Windows.Forms;

namespace AntiDupl.NET
{
    public class SelectHorizontalColumnsForm : Form
    {
        private ResultsListView m_resultsListView;
        private Options m_options;
        private ResultsOptions m_newResultOptions;
        private bool m_inited = false;

        private CheckBox[] m_checkBoxes;
        private Button m_okButton;
        private Button m_cancelButton;

        public SelectHorizontalColumnsForm(ResultsListView resultsListView, Options options)
        {
            if (options.resultsOptions.viewMode != ResultsOptions.ViewMode.HorizontalPairTable)
                throw new Exception("Bad view mode!");

            m_resultsListView = resultsListView;
            m_options = options;
            m_newResultOptions = new ResultsOptions(m_options.resultsOptions);
            InitializeComponents();
            UpdateStrings();
            UpdateItemsEnabling();
            GetOptions();
            m_inited = true;
        }

        private void InitializeComponents()
        {
            ClientSize = new System.Drawing.Size(450, 270);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 75F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            Controls.Add(mainTableLayoutPanel);

            TableLayoutPanel columnsTableLayoutPanel = InitFactory.Layout.Create(3, 8, 5); //columns, rows, padding
            columnsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33F));
            columnsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 34F));
            columnsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 33F));
            columnsTableLayoutPanel.AutoScroll = true;
            mainTableLayoutPanel.Controls.Add(columnsTableLayoutPanel, 0, 0);

            m_checkBoxes = new CheckBox[(int)ResultsListView.ColumnsTypeHorizontal.Size];
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Type] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Type], 0, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Group] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Group], 0, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.GroupSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.GroupSize], 0, 2);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Difference] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Difference], 0, 3);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Defect] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Defect], 0, 4);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Transform] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Transform], 0, 5);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Hint] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Hint], 0, 6);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileName] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileName], 1, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory], 1, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize], 1, 2);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType], 1, 3);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness], 1, 4);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring], 1, 5);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize], 1, 6);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime], 1, 7);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileName] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileName], 2, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory], 2, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize], 2, 2);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType], 2, 3);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness], 2, 4);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring], 2, 5);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize], 2, 6);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime], 2, 7);

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(3, 1);
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 25F));
            mainTableLayoutPanel.Controls.Add(buttonsTableLayoutPanel, 0, 1);

            m_okButton = new Button();
            m_okButton.Click += new System.EventHandler(OnButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_okButton, 1, 0);

            m_cancelButton = new Button();
            m_cancelButton.Click += new System.EventHandler(OnButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_cancelButton, 2, 0);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.MainMenu_View_SelectColumnsMenuItem_Text;

            m_okButton.Text = s.OkButton_Text;
            m_cancelButton.Text = s.CancelButton_Text;

            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Type].Text = s.ResultsListView_Type_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Group].Text = s.ResultsListView_Group_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.GroupSize].Text = s.ResultsListView_GroupSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Difference].Text = s.ResultsListView_Difference_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Defect].Text = s.ResultsListView_Defect_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Transform].Text = s.ResultsListView_Transform_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.Hint].Text = s.ResultsListView_Hint_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileName].Text = s.ResultsListView_FirstFileName_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].Text = s.ResultsListView_FirstFileDirectory_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].Text = s.ResultsListView_FirstImageSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].Text = s.ResultsListView_FirstImageType_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].Text = s.ResultsListView_FirstBlockiness_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].Text = s.ResultsListView_FirstBlurring_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].Text = s.ResultsListView_FirstFileSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].Text = s.ResultsListView_FirstFileTime_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileName].Text = s.ResultsListView_SecondFileName_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].Text = s.ResultsListView_SecondFileDirectory_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].Text = s.ResultsListView_SecondImageSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].Text = s.ResultsListView_SecondImageType_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].Text = s.ResultsListView_SecondBlockiness_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].Text = s.ResultsListView_SecondBlurring_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].Text = s.ResultsListView_SecondFileSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].Text = s.ResultsListView_SecondFileTime_Column_Text;
        }

        private void OnButtonClick(object sender, EventArgs e)
        {
            Button button = (Button)sender;
            if(button == m_okButton)
            {
                m_newResultOptions.CopyTo(ref m_options.resultsOptions);
                m_resultsListView.UpdateColumnsVisibility();
            }
            Close();
        }

        private void OnOptionChanged(object sender, EventArgs e)
        {
            if (m_inited)
            {
                SetOptions();
                UpdateItemsEnabling();
            }
        }

        private void GetOptions()
        {
            for (int i = 0; i < (int)ResultsListView.ColumnsTypeHorizontal.Size; i++)
            {
                m_checkBoxes[i].Checked = m_newResultOptions.columnOptionsHorizontal[i].visible;
            }
        }

        private void SetOptions()
        {
            for (int i = 0; i < (int)ResultsListView.ColumnsTypeHorizontal.Size; i++)
            {
                m_newResultOptions.columnOptionsHorizontal[i].visible = m_checkBoxes[i].Checked;
            }
        }

        private void UpdateItemsEnabling()
        {
            m_okButton.Enabled = !m_newResultOptions.Equals(m_options.resultsOptions);
        }
    }
}
