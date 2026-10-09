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
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// A check box that in dark mode draws itself: a ticked box filled with
    /// ThemeColors.Selection instead of the system's bright accent colour.
    /// In light mode it is the standard check box.
    /// </summary>
    public class ThemedCheckBox : CheckBox
    {
        private const int BOX_SIZE = 13;

        public ThemedCheckBox()
        {
            if (Application.IsDarkModeEnabled)
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (!Application.IsDarkModeEnabled)
            {
                base.OnPaint(e);
                return;
            }

            Graphics g = e.Graphics;
            g.Clear(BackColor);
            Rectangle box = new Rectangle(1, (Height - BOX_SIZE) / 2, BOX_SIZE - 1, BOX_SIZE - 1);
            Color fill = Checked ? ThemeColors.Selection : SystemColors.Window;
            Color line = Checked ? ThemeColors.Selection : SystemColors.ControlDark;
            Color text = Enabled ? ForeColor : SystemColors.GrayText;
            if (!Enabled)
            {
                fill = SystemColors.Control;
                line = SystemColors.ControlDark;
            }
            using (SolidBrush brush = new SolidBrush(fill))
                g.FillRectangle(brush, box);
            using (Pen pen = new Pen(line))
                g.DrawRectangle(pen, box);
            if (Checked)
            {
                g.SmoothingMode = SmoothingMode.AntiAlias;
                using (Pen tick = new Pen(Enabled ? ThemeColors.SelectionText : SystemColors.GrayText, 2))
                    g.DrawLines(tick, new[] {
                        new Point(box.X + 3, box.Y + 6), new Point(box.X + 5, box.Y + 9), new Point(box.X + 10, box.Y + 3) });
                g.SmoothingMode = SmoothingMode.Default;
            }

            Rectangle textBounds = new Rectangle(box.Right + 4, 0, Width - box.Right - 4, Height);
            TextRenderer.DrawText(g, Text, Font, textBounds, text,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine);
            if (Focused && ShowFocusCues && Text.Length > 0)
            {
                Size size = TextRenderer.MeasureText(g, Text, Font);
                ControlPaint.DrawFocusRectangle(g, new Rectangle(textBounds.X - 1, (Height - size.Height) / 2,
                    size.Width + 1, size.Height), text, BackColor);
            }
        }
    }
}
