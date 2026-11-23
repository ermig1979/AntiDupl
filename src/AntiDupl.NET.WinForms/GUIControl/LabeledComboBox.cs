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

namespace AntiDupl.NET.WinForms
{
    public class LabeledComboBox : TableLayoutPanel
    {
        private ComboBox m_comboBox;
        private Label m_label;

        public ComboBox comboBox { get { return m_comboBox; } }
        public override String Text { get { return m_label.Text; } set { m_label.Text = value; } }

        public LabeledComboBox(int comboBoxWidth, int comboBoxHeight, EventHandler selectedIndexChanged)
        {
            Location = new System.Drawing.Point(0, 0);
            AutoSize = true;
            ColumnCount = 2;
            RowCount = 1;

            m_comboBox = new ComboBox();
            m_comboBox.Size = new System.Drawing.Size(comboBoxWidth, comboBoxHeight);
            m_comboBox.Padding = new Padding(0, 0, 0, 0);
            m_comboBox.Margin = new Padding(0, 0, 0, 0);
            m_comboBox.DropDownStyle = ComboBoxStyle.DropDownList;
            m_comboBox.SelectedIndexChanged += new EventHandler(selectedIndexChanged);
            Controls.Add(m_comboBox, 0, 0);

            m_label = InitFactory.Label.Create();
            m_label.Padding = new Padding(0, 5, 5, 5);
            m_labelMargin = m_label.Margin;
            Controls.Add(m_label, 1, 0);
        }

        private Padding m_labelMargin;

        /// <summary>
        /// The label beside the list, or on a line of its own below it,
        /// indented by indent and gap pixels under the list: for a label
        /// too long to leave the list room beside it.
        /// </summary>
        public void PlaceLabel(bool below, int indent, int gap)
        {
            SuspendLayout();
            Controls.Remove(m_label);
            if (below)
            {
                RowCount = 2;
                Controls.Add(m_label, 0, 1);
                SetColumnSpan(m_label, 2);
                m_label.Margin = new Padding(indent, 0, 0, 0);
                m_label.Padding = new Padding(0, gap, 5, 0);
            }
            else
            {
                RowCount = 1;
                Controls.Add(m_label, 1, 0);
                SetColumnSpan(m_label, 1);
                m_label.Margin = m_labelMargin;
                m_label.Padding = new Padding(0, 5, 5, 5);
            }
            ResumeLayout();
        }

        public class Value : object
        {
            public int value;
            public string description;

            public Value(int val, string desc)
            {
                value = val;
                description = desc;
            }

            public override string ToString()
            {
                return description;
            }
        }

        public int SelectedValue
        {
            get
            {
                Value val = (Value)m_comboBox.SelectedItem;
                return val.value;
            }
            set
            {
                int index = -1;
                int difference = int.MaxValue;
                for (int i = 0; i < m_comboBox.Items.Count; i++)
                {
                    Value current = (Value)m_comboBox.Items[i];
                    if (Math.Abs(current.value - value) < difference)
                    {
                        difference = Math.Abs(current.value - value);
                        index = i;
                    }
                }
                m_comboBox.SelectedIndex = index;
            }
        }

        public void SetDescription(int index, string description)
        {
            Value value = (Value)m_comboBox.Items[index];
            value.description = description;
        }
    }
}
