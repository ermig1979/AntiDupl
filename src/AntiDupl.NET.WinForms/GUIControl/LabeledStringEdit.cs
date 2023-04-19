/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar.
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
using static AntiDupl.NET.WinForms.LabeledComboBox;

namespace AntiDupl.NET.WinForms
{
    public class LabeledStringEdit : TableLayoutPanel
    {
        private TextBox m_textBox;
        private Label m_label;
        private string m_text = "";
        private string m_default = "";
        private EventHandler m_textChangedHandler;

        public override String Text { get { return m_label.Text; } set { m_label.Text = value; } }

        public string Value
        {
            get { return m_text; }
            set
            {
                m_text = value;
                m_textBox.Text = value;
            }
        }

        public string Default
        {
            get { return m_default; }
            set { m_default = value; }
        }

        public LabeledStringEdit(int textBoxWidth, int textBoxHeight, EventHandler valueChangedHandler)
        {
            m_textChangedHandler = valueChangedHandler;

            Location = new System.Drawing.Point(0, 0);
            AutoSize = true;
            ColumnCount = 2;
            RowCount = 1;

            m_label = new System.Windows.Forms.Label();
            m_label.AutoSize = true;
            m_label.Padding = new Padding(0, 5, 5, 5);
            Controls.Add(m_label, 0, 1);

            m_textBox = new TextBox();
            m_textBox.Size = new System.Drawing.Size(textBoxWidth, textBoxHeight);
            m_textBox.Padding = new Padding(0, 0, 0, 0);
            m_textBox.Margin = new Padding(0, 0, 0, 0);
            m_textBox.Multiline = false;
            m_textBox.KeyUp += new KeyEventHandler(OnTextBoxKeyUp);
            Controls.Add(m_textBox, 0, 2);
        }

        private void OnTextBoxKeyUp(object sender, KeyEventArgs e)
        {
            string old = m_text;

            try
            {
                m_text = m_textBox.Text;
            }
            catch (System.Exception)
            {
                m_text = old;
            }

            if (old != m_text && m_textChangedHandler != null)
            {
                m_textChangedHandler(sender, e);
            }

            m_textBox.Text = m_text;
        }
    }
}
