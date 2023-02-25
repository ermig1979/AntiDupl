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
using System.Collections.ObjectModel;

namespace AntiDupl.NET
{
    public class ComplexProgressBar : ProgressBar
    {
        private int m_firstValue = 0;
        public int FirstValue
        {
            get { return m_firstValue; }
            set
            {
                if (m_firstValue != value)
                {
                    m_firstValue = value;
                    Invalidate();
                }
            }
        }

        private int m_secondValue = 0;
        public int SecondValue
        {
            get { return m_secondValue; }
            set
            {
                if (m_secondValue != value)
                {
                    m_secondValue = value;
                    Invalidate();
                }
            }
        }

        public Color FirstForeColor
        {
            get { return ForeColor; }
            set { ForeColor = value; }
        }

        private Color m_secondForeColor = new Color();
        public Color SecondForeColor
        {
            get { return m_secondForeColor; }
            set
            {
                m_secondForeColor = value;
                Invalidate();
            }
        }

        public ComplexProgressBar()
        {
            SetStyle(ControlStyles.UserPaint, true);
            m_secondForeColor = Color.FromArgb((ForeColor.R + BackColor.R) / 2, (ForeColor.G + BackColor.G) / 2, (ForeColor.B + BackColor.B) / 2);
        }

        override protected void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Brush secondBrush = new SolidBrush(SecondForeColor);
            e.Graphics.FillRectangle(secondBrush, 1, 1, (Width - 2) * m_secondValue / (Maximum - Minimum), Height - 2);
            Brush firstBrush = new SolidBrush(FirstForeColor);
            e.Graphics.FillRectangle(firstBrush, 1, 1, (Width - 2) * m_firstValue / (Maximum - Minimum), Height - 2);
        }
    }
}
