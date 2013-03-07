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

namespace AntiDupl.NET
{
    public class AboutProgramForm : Form
    {
        private AboutProgramPanel m_aboutProgramPanel;
        private Button m_okButton;

        public AboutProgramForm()
        {
            InitializeComponent();
            UpdateStrings();
        }

        private void InitializeComponent()
        {
            ClientSize = new System.Drawing.Size(310, 200);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 75F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 25F));
            Controls.Add(mainTableLayoutPanel);

            m_aboutProgramPanel = new AboutProgramPanel();
            m_aboutProgramPanel.Location = new System.Drawing.Point(0, 0);
            m_aboutProgramPanel.Dock = DockStyle.Fill;
            mainTableLayoutPanel.Controls.Add(m_aboutProgramPanel, 0, 0);

            TableLayoutPanel okButtonTableLayoutPanel = InitFactory.Layout.Create(3, 1);
            okButtonTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            okButtonTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            okButtonTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            mainTableLayoutPanel.Controls.Add(okButtonTableLayoutPanel, 0, 1);

            m_okButton = new Button();
            m_okButton.Click += new System.EventHandler(OnOkButtonClick);
            okButtonTableLayoutPanel.Controls.Add(m_okButton, 1, 0);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.AboutProgramForm_Text;
            m_okButton.Text = s.OkButton_Text;
        }

        private void OnOkButtonClick(object sender, EventArgs e)
        {
            Close();
        }
    }
}
