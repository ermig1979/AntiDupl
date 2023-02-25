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
    public class SelectVerticalColumnsForm : Form
    {
        private ResultsListView m_resultsListView;
        private Options m_options;
        private ResultsOptions m_newResultOptions;
        private bool m_inited = false;

        private CheckBox[] m_checkBoxes;
        private Button m_okButton;
        private Button m_cancelButton;

        public SelectVerticalColumnsForm(ResultsListView resultsListView, Options options)
        {
            if (options.resultsOptions.viewMode != ResultsOptions.ViewMode.VerticalPairTable)
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
            ClientSize = new System.Drawing.Size(400, 210);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 75F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            Controls.Add(mainTableLayoutPanel);

            TableLayoutPanel columnsTableLayoutPanel = InitFactory.Layout.Create(3, 6, 5); //columns, rows, padding
            columnsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            columnsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            columnsTableLayoutPanel.AutoScroll = true;
            mainTableLayoutPanel.Controls.Add(columnsTableLayoutPanel, 0, 0);

            m_checkBoxes = new CheckBox[(int)ResultsListView.ColumnsTypeVertical.Size];
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Type] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Type], 0, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Group] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Group], 0, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.GroupSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.GroupSize], 0, 2);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Difference] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Difference], 0, 3);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Defect] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Defect], 0, 4);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Transform] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Transform], 0, 5);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Hint] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Hint], 1, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileName] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileName], 1, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileDirectory] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileDirectory], 1, 2);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageSize], 1, 3);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageType] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageType], 1, 4);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blockiness] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blockiness], 1, 5);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blurring] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blurring], 2, 0);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileSize] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileSize], 2, 1);
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileTime] = InitFactory.CheckBox.Create(OnOptionChanged);
            columnsTableLayoutPanel.Controls.Add(m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileTime], 2, 2);

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(3, 1);
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 40F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
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

            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Type].Text = s.ResultsListView_Type_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Group].Text = s.ResultsListView_Group_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.GroupSize].Text = s.ResultsListView_GroupSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Difference].Text = s.ResultsListView_Difference_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Defect].Text = s.ResultsListView_Defect_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Transform].Text = s.ResultsListView_Transform_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Hint].Text = s.ResultsListView_Hint_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileName].Text = s.ResultsListView_FileName_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileDirectory].Text = s.ResultsListView_FileDirectory_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageSize].Text = s.ResultsListView_ImageSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.ImageType].Text = s.ResultsListView_ImageType_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blockiness].Text = s.ResultsListView_Blockiness_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.Blurring].Text = s.ResultsListView_Blurring_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileSize].Text = s.ResultsListView_FileSize_Column_Text;
            m_checkBoxes[(int)ResultsListView.ColumnsTypeVertical.FileTime].Text = s.ResultsListView_FileTime_Column_Text;
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
            for (int i = 0; i < (int)ResultsListView.ColumnsTypeVertical.Size; i++)
            {
                m_checkBoxes[i].Checked = m_newResultOptions.columnOptionsVertical[i].visible;
            }
        }

        private void SetOptions()
        {
            for (int i = 0; i < (int)ResultsListView.ColumnsTypeVertical.Size; i++)
            {
                m_newResultOptions.columnOptionsVertical[i].visible = m_checkBoxes[i].Checked;
            }
        }

        private void UpdateItemsEnabling()
        {
            m_okButton.Enabled = !m_newResultOptions.Equals(m_options.resultsOptions);
        }
    }
}
