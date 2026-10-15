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
    /// The system look of tool bars and menus; in dark mode the button or
    /// menu item under the mouse, or pressed, or open, is filled with
    /// ThemeColors.Selection, like the selected rows of the results table.
    /// Its drop-down menus use it too.
    /// </summary>
    public class ThemedToolStripRenderer : ToolStripSystemRenderer
    {
        /// <summary>
        /// Gives a tool bar or menu this renderer in dark mode; in light mode
        /// it keeps the system renderer it has.
        /// </summary>
        static public void Set(ToolStrip toolStrip)
        {
            if (Application.IsDarkModeEnabled)
                toolStrip.Renderer = new ThemedToolStripRenderer();
        }

        static private bool Highlighted(ToolStripItem item)
        {
            return Application.IsDarkModeEnabled && item != null && item.Enabled && (item.Selected || item.Pressed);
        }

        static private void Fill(Graphics graphics, Rectangle bounds)
        {
            using (SolidBrush brush = new SolidBrush(ThemeColors.Selection))
                graphics.FillRectangle(brush, bounds);
        }

        protected override void OnRenderButtonBackground(ToolStripItemRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                Fill(e.Graphics, new Rectangle(Point.Empty, e.Item.Size));
            else
                base.OnRenderButtonBackground(e);
        }

        protected override void OnRenderDropDownButtonBackground(ToolStripItemRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                Fill(e.Graphics, new Rectangle(Point.Empty, e.Item.Size));
            else
                base.OnRenderDropDownButtonBackground(e);
        }

        protected override void OnRenderSplitButtonBackground(ToolStripItemRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                Fill(e.Graphics, new Rectangle(Point.Empty, e.Item.Size));
            else
                base.OnRenderSplitButtonBackground(e);
        }

        protected override void OnRenderMenuItemBackground(ToolStripItemRenderEventArgs e)
        {
            if (Highlighted(e.Item))
            {
                // In a drop-down menu the highlight keeps off the menu's edges.
                Rectangle bounds = new Rectangle(Point.Empty, e.Item.Size);
                if (e.Item.IsOnDropDown)
                    bounds.Inflate(-2, 0);
                Fill(e.Graphics, bounds);
            }
            else
                base.OnRenderMenuItemBackground(e);
        }

        protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                e.TextColor = ThemeColors.SelectionText;
            base.OnRenderItemText(e);
        }

        protected override void OnRenderArrow(ToolStripArrowRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                e.ArrowColor = ThemeColors.SelectionText;
            base.OnRenderArrow(e);
        }
    }
}
