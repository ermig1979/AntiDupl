using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace AntiDupl.NET
{
    /// <summary>
    /// NumericUpDown с подписью.
    /// </summary>
    class LabeledNumericUpDown : TableLayoutPanel
    {
        private NumericUpDown m_numericUpDown;
        private Label m_label;
        private float m_value = 0;
        private float m_min = int.MinValue;
        private float m_max = int.MaxValue;
        private float m_default = 0;
        private EventHandler m_valueChangedHandler;

        public override String Text { get { return m_label.Text; } set { m_label.Text = value; } }

        public float Value
        {
            get { return Decimal.ToSingle(m_numericUpDown.Value); }
            set
            {
                if (value > m_max || value < m_min)
                {
                    m_value = m_default;
                }
                else
                {
                    m_value = value;
                }
                m_numericUpDown.Value = new decimal(m_value);
            }
        }

        public float Min
        {
            get { return m_min; }
            set
            {
                m_min = value;
                m_max = Math.Max(m_max, m_min);
                m_default = Math.Max(m_default, m_min);
                m_value = Math.Max(m_value, m_min);
                m_numericUpDown.Value = new decimal(m_value);
            }
        }

        public float Max
        {
            get { return m_max; }
            set
            {
                m_max = value;
                m_min = Math.Min(m_min, m_max);
                m_default = Math.Min(m_default, m_max);
                m_value = Math.Min(m_value, m_max);
                m_numericUpDown.Value = new decimal(m_value);
            }
        }

        public float Default
        {
            get { return m_default; }
            set
            {
                m_default = value;
                m_min = Math.Min(m_min, m_default);
                m_max = Math.Max(m_default, m_max);
            }
        }

        public LabeledNumericUpDown(int decimalPlaces,
                                    decimal increment, 
                                    decimal minimum,
                                    decimal value,
                                    EventHandler valueChangedHandler)
        {
            m_valueChangedHandler = valueChangedHandler;

            Location = new System.Drawing.Point(0, 0);
            AutoSize = true;
            ColumnCount = 2;
            RowCount = 1;

            m_numericUpDown = new NumericUpDown();
            m_numericUpDown.Margin = new Padding(0);
            m_numericUpDown.DecimalPlaces = decimalPlaces;
            m_numericUpDown.Increment = increment;
            m_numericUpDown.Minimum = minimum;
            m_numericUpDown.Value = value;
            m_numericUpDown.ValueChanged += new System.EventHandler(OnValueChanged); //вызов сначала внутренней функции
            Controls.Add(m_numericUpDown, 0, 0);

            m_label = new System.Windows.Forms.Label();
            m_label.AutoSize = true;
            m_label.Padding = new Padding(0, 5, 5, 5);
            Controls.Add(m_label, 1, 0);
        }

        private void OnValueChanged(object sender, EventArgs e)
        {
           float old = m_value;

            try
            {
                m_value = Decimal.ToSingle(m_numericUpDown.Value);
                if (m_value > m_max || m_value < m_min)
                {
                    m_value = old;
                }
            }
            catch (System.Exception)
            {
                m_value = old;
            }

            if (old != m_value && m_valueChangedHandler != null)
            {
                m_valueChangedHandler(sender, e);
            }

            m_numericUpDown.Value = new decimal(m_value);
        }
    }
}
