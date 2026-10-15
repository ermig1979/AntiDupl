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
        public const int BOX_SIZE = 13;

        public ThemedCheckBox()
        {
            if (Application.IsDarkModeEnabled)
                SetStyle(ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer, true);
        }

        /// <summary>The dark-mode box, also used by the themed list with check boxes.</summary>
        static public void DrawBox(Graphics g, int x, int centerY, bool isChecked, bool enabled)
        {
            Rectangle box = new Rectangle(x, centerY - BOX_SIZE / 2, BOX_SIZE - 1, BOX_SIZE - 1);
            Color fill = !enabled ? SystemColors.Control : isChecked ? ThemeColors.Selection : SystemColors.Window;
            Color line = enabled && isChecked ? ThemeColors.Selection : SystemColors.ControlDark;
            using (SolidBrush brush = new SolidBrush(fill))
                g.FillRectangle(brush, box);
            using (Pen pen = new Pen(line))
                g.DrawRectangle(pen, box);
            if (isChecked)
            {
                SmoothingMode smoothing = g.SmoothingMode;
                g.SmoothingMode = SmoothingMode.AntiAlias;
                using (Pen tick = new Pen(enabled ? ThemeColors.SelectionText : SystemColors.GrayText, 2))
                    g.DrawLines(tick, new[] {
                        new Point(box.X + 3, box.Y + 6), new Point(box.X + 5, box.Y + 9), new Point(box.X + 10, box.Y + 3) });
                g.SmoothingMode = smoothing;
            }
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
            DrawBox(g, 1, Height / 2, Checked, Enabled);
            Color text = Enabled ? ForeColor : SystemColors.GrayText;
            Rectangle textBounds = new Rectangle(BOX_SIZE + 4, 0, Width - BOX_SIZE - 4, Height);
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

    /// <summary>
    /// A list with check boxes that in dark mode draws its items: the same
    /// box as ThemedCheckBox, and selected items in ThemeColors.Selection
    /// with light text instead of the system's black text.
    /// </summary>
    public class ThemedCheckedListBox : CheckedListBox
    {
        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            if (!Application.IsDarkModeEnabled)
            {
                base.OnDrawItem(e);
                return;
            }
            if (e.Index < 0 || e.Index >= Items.Count)
                return;
            bool selected = (e.State & DrawItemState.Selected) != 0;
            using (SolidBrush back = new SolidBrush(selected ? ThemeColors.Selection : BackColor))
                e.Graphics.FillRectangle(back, e.Bounds);
            // The list toggles the check when the click falls on its own
            // glyph area, which starts at the item's left edge.
            ThemedCheckBox.DrawBox(e.Graphics, e.Bounds.X + 1, e.Bounds.Y + e.Bounds.Height / 2, GetItemChecked(e.Index), Enabled);
            Rectangle text = new Rectangle(e.Bounds.X + ThemedCheckBox.BOX_SIZE + 4, e.Bounds.Y,
                e.Bounds.Width - ThemedCheckBox.BOX_SIZE - 4, e.Bounds.Height);
            TextRenderer.DrawText(e.Graphics, GetItemText(Items[e.Index]), Font, text,
                selected ? ThemeColors.SelectionText : ThemeColors.Text,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine | TextFormatFlags.EndEllipsis);
        }
    }

    /// <summary>
    /// A list that in dark mode draws its items, with selected items in
    /// ThemeColors.Selection and light text instead of the system's black
    /// text. In light mode it is the standard list.
    /// </summary>
    public class ThemedListBox : ListBox
    {
        public ThemedListBox()
        {
            if (Application.IsDarkModeEnabled)
                DrawMode = DrawMode.OwnerDrawFixed;
        }

        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            if (DrawMode == DrawMode.Normal)
            {
                base.OnDrawItem(e);
                return;
            }
            if (e.Index < 0 || e.Index >= Items.Count)
                return;
            bool selected = (e.State & DrawItemState.Selected) != 0;
            using (SolidBrush back = new SolidBrush(selected ? ThemeColors.Selection : BackColor))
                e.Graphics.FillRectangle(back, e.Bounds);
            Rectangle text = new Rectangle(e.Bounds.X + 2, e.Bounds.Y, e.Bounds.Width - 2, e.Bounds.Height);
            TextRenderer.DrawText(e.Graphics, GetItemText(Items[e.Index]), Font, text,
                selected ? ThemeColors.SelectionText : ThemeColors.Text,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.SingleLine | TextFormatFlags.EndEllipsis);
        }
    }
}
