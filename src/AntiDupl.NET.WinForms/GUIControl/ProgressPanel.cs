/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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

namespace AntiDupl.NET
{
    public class ProgressPanel : Panel
    {
        private Label m_totalLabel;
        private Label m_currentLabel;
        private ComplexProgressBar m_complexProgressBar;
        private Label m_pathLabel;

        public ProgressPanel()
        {
            InitializeComponent();
        }

        private void InitializeComponent()
        {
            BorderStyle = BorderStyle.Fixed3D;
            Location = new Point(0, 0);
            Dock = DockStyle.Fill;
            Margin = new Padding(1);
            Padding = new Padding(0);
            Height = 26;

            TableLayoutPanel tableLayoutPanel = InitFactory.Layout.Create(4, 1);
            tableLayoutPanel.Margin = new Padding(0);
            tableLayoutPanel.Padding = new Padding(0);
            tableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60));
            tableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 60));
            tableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 180));
            tableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 500));
            Controls.Add(tableLayoutPanel);

            m_totalLabel = new Label();
            m_totalLabel.Location = new Point(0, 0);
            m_totalLabel.Dock = DockStyle.Fill;
            m_totalLabel.Width = (int)tableLayoutPanel.ColumnStyles[0].Width;
            m_totalLabel.BorderStyle = BorderStyle.Fixed3D;
            m_totalLabel.TextAlign = ContentAlignment.MiddleCenter;
            m_totalLabel.Margin = new Padding(0, 0, 1, 0);
            tableLayoutPanel.Controls.Add(m_totalLabel, 0, 0);

            m_currentLabel = new Label();
            m_currentLabel.Location = new Point(0, 0);
            m_currentLabel.Dock = DockStyle.Fill;
            m_currentLabel.Width = (int)tableLayoutPanel.ColumnStyles[1].Width;
            m_currentLabel.BorderStyle = BorderStyle.Fixed3D;
            m_currentLabel.TextAlign = ContentAlignment.MiddleCenter;
            m_currentLabel.Margin = new Padding(0, 0, 1, 0);
            tableLayoutPanel.Controls.Add(m_currentLabel, 1, 0);

            m_complexProgressBar = new ComplexProgressBar();
            m_complexProgressBar.Location = new Point(0, 0);
            m_complexProgressBar.Dock = DockStyle.Fill;
            m_complexProgressBar.Width = (int)tableLayoutPanel.ColumnStyles[2].Width;
            m_complexProgressBar.Margin = new Padding(0);
            tableLayoutPanel.Controls.Add(m_complexProgressBar, 2, 0);

            m_pathLabel = new Label();
            m_pathLabel.Location = new Point(0, 0);
            m_pathLabel.Dock = DockStyle.Fill;
            m_pathLabel.Width = (int)tableLayoutPanel.ColumnStyles[3].Width;
            m_pathLabel.BorderStyle = BorderStyle.Fixed3D;
            m_pathLabel.TextAlign = ContentAlignment.MiddleLeft;
            m_pathLabel.Margin = new Padding(1, 0, 0, 0);
            tableLayoutPanel.Controls.Add(m_pathLabel, 3, 0);
        }

        public void UpdateStatus(int total, int currentFirst, int currentSecond, string path)
        {
            m_totalLabel.Text = total.ToString();
            m_currentLabel.Text = currentFirst.ToString();
            if (total > 0)
            {
                int range = m_complexProgressBar.Maximum - m_complexProgressBar.Minimum;
                int firstValue = currentFirst * range / total;
                int secondValue = currentSecond * range / total;
                m_complexProgressBar.FirstValue = Math.Max(m_complexProgressBar.Minimum, Math.Min(m_complexProgressBar.Maximum, firstValue));
                m_complexProgressBar.SecondValue = Math.Max(m_complexProgressBar.Minimum, Math.Min(m_complexProgressBar.Maximum, secondValue));
            }
            else
            {
                m_complexProgressBar.FirstValue = m_complexProgressBar.Minimum;
                m_complexProgressBar.SecondValue = m_complexProgressBar.Minimum;
            }

            Graphics graphics = Graphics.FromHwnd(m_pathLabel.Handle);
            SizeF size = graphics.MeasureString(path, m_pathLabel.Font);
            if (size.Width > m_pathLabel.Width)
            {
                int length = Convert.ToInt32(path.Length * m_pathLabel.Width / size.Width);
                m_pathLabel.Text = new string(path.ToCharArray(), 0, length);
            }
            else
            {
                m_pathLabel.Text = path;
            }
        }
    }
}
