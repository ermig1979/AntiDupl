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
using System.Drawing;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// Colours with a meaning that the system colours don't cover, and
    /// borders, for the light and the dark color mode. The light ones are
    /// what the program always used, so the light mode looks as before.
    /// </summary>
    static public class ThemeColors
    {
        static private bool Dark { get { return Application.IsDarkModeEnabled; } }

        /// <summary>
        /// Text. In dark mode the framework mixes pure white (ControlText)
        /// and #F0F0F0 (WindowText, MenuText); all text uses #F8F8F8.
        /// </summary>
        static public Color Text { get { return Dark ? Color.FromArgb(0xF8, 0xF8, 0xF8) : SystemColors.ControlText; } }

        /// <summary>
        /// Selected rows of the results table. The dark-mode highlight is a
        /// darker blue than the system one, for the light text on it (the
        /// system pairs it with black text).
        /// </summary>
        static public Color Selection { get { return Dark ? Color.FromArgb(0x1E, 0x4C, 0x8A) : SystemColors.Highlight; } }
        static public Color SelectionText { get { return Dark ? Text : SystemColors.HighlightText; } }

        /// <summary>The dotted outline of the current row: light mode only.</summary>
        static public bool CurrentRowOutline { get { return !Dark; } }

        /// <summary>The form's text colour, which its controls inherit unless they set their own.</summary>
        static public void Apply(Form form)
        {
            if (Dark)
                form.ForeColor = Text;
        }

        /// <summary>Text of the value that is worse than the other image's.</summary>
        static public Color Worse { get { return Dark ? Color.FromArgb(0xFF, 0x70, 0x70) : Color.Red; } }

        /// <summary>Background around the previewed images.</summary>
        static public Color PreviewBackground { get { return Dark ? Color.FromArgb(0x2B, 0x2B, 0x2B) : Color.DarkGray; } }

        /// <summary>Line between the two values of a vertical-table cell.</summary>
        static public Color CellSeparator { get { return Dark ? SystemColors.ControlDark : Color.LightGray; } }

        /// <summary>Links; empty means the WinForms default (light mode).</summary>
        static public Color Link { get { return Dark ? Color.FromArgb(0x4D, 0xA3, 0xFF) : Color.Empty; } }

        // The 3D border is drawn with light edge colours in dark mode, so
        // there a plain 1 px line in ControlDark replaces it.
        static public void SetBorder(Label label)
        {
            if (Dark)
                SetLine(label);
            else
                label.BorderStyle = BorderStyle.Fixed3D;
        }

        static public void SetBorder(Panel panel)
        {
            if (Dark)
            {
                // Keeps docked children off the line.
                panel.Padding = new Padding(1);
                SetLine(panel);
            }
            else
                panel.BorderStyle = BorderStyle.Fixed3D;
        }

        static public void SetBorder(PictureBox pictureBox)
        {
            if (Dark)
                SetLine(pictureBox);
            else
                pictureBox.BorderStyle = BorderStyle.Fixed3D;
        }

        static private void SetLine(Control control)
        {
            control.Paint += new PaintEventHandler(OnPaintLine);
            control.Resize += new System.EventHandler(OnResizeLine);
        }

        static private void OnPaintLine(object sender, PaintEventArgs e)
        {
            Control control = (Control)sender;
            using (Pen pen = new Pen(SystemColors.ControlDark))
                e.Graphics.DrawRectangle(pen, 0, 0, control.Width - 1, control.Height - 1);
        }

        // Without a full repaint on resize the old line stays where the
        // edge used to be.
        static private void OnResizeLine(object sender, System.EventArgs e)
        {
            ((Control)sender).Invalidate();
        }

        /// <summary>The border of the main window's split area: none in dark mode.</summary>
        static public BorderStyle SplitContainerBorder { get { return Dark ? BorderStyle.None : BorderStyle.Fixed3D; } }

        /// <summary>The results table's frame: none in dark mode.</summary>
        static public BorderStyle ResultsTableBorder { get { return Dark ? BorderStyle.None : BorderStyle.FixedSingle; } }

        /// <summary>
        /// A sunken box in light mode; in dark mode, where that box is drawn
        /// light, a 1 px ControlDark divider on the right.
        /// </summary>
        static public void SetBorder(ToolStripStatusLabel label)
        {
            if (Dark)
            {
                label.BorderSides = ToolStripStatusLabelBorderSides.None;
                // The dark status bar draws item text in a low-contrast grey
                // whatever ForeColor says, so the label draws its own text.
                label.DisplayStyle = ToolStripItemDisplayStyle.None;
                label.Paint += new PaintEventHandler(OnPaintStatusLabel);
            }
            else
            {
                label.BorderStyle = Border3DStyle.SunkenOuter;
                label.BorderSides = ToolStripStatusLabelBorderSides.All;
            }
        }

        static private void OnPaintStatusLabel(object sender, PaintEventArgs e)
        {
            ToolStripItem item = (ToolStripItem)sender;
            Rectangle text = new Rectangle(2, 0, item.Width - 5, item.Height);
            TextRenderer.DrawText(e.Graphics, item.Text, item.Font, text, Text,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine | TextFormatFlags.EndEllipsis);
            using (Pen pen = new Pen(SystemColors.ControlDark))
                e.Graphics.DrawLine(pen, item.Width - 1, 2, item.Width - 1, item.Height - 3);
        }

        /// <summary>Drop-down lists: the standard style is drawn light in dark mode.</summary>
        static public FlatStyle ComboBoxStyle { get { return Dark ? FlatStyle.Popup : FlatStyle.Standard; } }

        /// <summary>
        /// The control to add to a layout for a text box: the box itself in
        /// light mode; in dark mode, where its own border is drawn thick and
        /// light, the box without a border inside a FrameHost.
        /// </summary>
        static public Control Framed(TextBox textBox)
        {
            if (!Dark)
                return textBox;
            // Read before the border goes: the frame keeps the bordered
            // height, so the layouts around it don't change.
            int height = textBox.PreferredHeight;
            textBox.BorderStyle = BorderStyle.None;
            return new FrameHost(textBox, height);
        }

        /// <summary>The same for a number box.</summary>
        static public Control Framed(NumericUpDown numericUpDown)
        {
            if (!Dark)
                return numericUpDown;
            int height = numericUpDown.PreferredHeight;
            numericUpDown.BorderStyle = BorderStyle.None;
            return new FrameHost(numericUpDown, height);
        }

        /// <summary>
        /// A panel in the inner control's background colour with a 1 px
        /// ControlDark line, taking over the inner control's place (size,
        /// margin, dock, anchor) in the layout; the borderless control sits
        /// in it vertically centred.
        /// </summary>
        private class FrameHost : Panel
        {
            private readonly Control m_inner;

            public FrameHost(Control inner, int height)
            {
                m_inner = inner;
                Margin = inner.Margin;
                Dock = inner.Dock;
                Anchor = inner.Anchor;
                Size = new Size(inner.Width, height);
                BackColor = inner.BackColor;
                ResizeRedraw = true;
                inner.Margin = new Padding(0);
                inner.Dock = DockStyle.None;
                inner.Anchor = AnchorStyles.Left | AnchorStyles.Top;
                // The hot-key boxes change their background while the window is open.
                inner.BackColorChanged += new System.EventHandler(OnInnerBackColorChanged);
                Controls.Add(inner);
            }

            private void OnInnerBackColorChanged(object sender, System.EventArgs e)
            {
                BackColor = m_inner.BackColor;
            }

            protected override void OnLayout(LayoutEventArgs e)
            {
                base.OnLayout(e);
                int height = m_inner is TextBoxBase textBox ? textBox.PreferredHeight :
                    m_inner is UpDownBase upDown ? upDown.PreferredHeight : m_inner.Height;
                m_inner.SetBounds(3, (Height - height) / 2, Width - 5, height);
            }

            protected override void OnPaint(PaintEventArgs e)
            {
                base.OnPaint(e);
                using (Pen pen = new Pen(SystemColors.ControlDark))
                    e.Graphics.DrawRectangle(pen, 0, 0, Width - 1, Height - 1);
            }
        }
    }
}
