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
    /// In both themes a menu item left for another one while its sub-menu
    /// is still open is drawn plain (Left). Its drop-down menus use it too.
    /// </summary>
    public class ThemedToolStripRenderer : ToolStripSystemRenderer
    {
        /// <summary>
        /// Gives a tool bar or menu this renderer. In light mode it draws as
        /// the system renderer does, apart from Left.
        /// </summary>
        static public void Set(ToolStrip toolStrip)
        {
            toolStrip.Renderer = new ThemedToolStripRenderer();
        }

        /// <summary>
        /// A menu item whose sub-menu is still open while the mouse is on
        /// another item of its menu. WinForms closes that sub-menu only after
        /// the hover delay, and until then the item stays pressed, which the
        /// system renderer draws lit like the item under the mouse: the
        /// highlight seemed to lag behind the mouse.
        /// </summary>
        static private bool Left(ToolStripItem item)
        {
            if (item == null || item.Selected || !item.Pressed || !item.IsOnDropDown || item.Owner == null)
                return false;
            foreach (ToolStripItem other in item.Owner.Items)
                if (other != item && other.Selected)
                    return true;
            return false;
        }

        static private bool Highlighted(ToolStripItem item)
        {
            return Application.IsDarkModeEnabled && item != null && item.Enabled && (item.Selected || item.Pressed) && !Left(item);
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
            else if (!Left(e.Item)) // the base would draw it pressed
                base.OnRenderMenuItemBackground(e);
        }

        protected override void OnRenderItemText(ToolStripItemTextRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                e.TextColor = ThemeColors.SelectionText;
            else if (Left(e.Item))
                e.TextColor = e.Item.ForeColor;
            base.OnRenderItemText(e);
        }

        protected override void OnRenderArrow(ToolStripArrowRenderEventArgs e)
        {
            if (Highlighted(e.Item))
                e.ArrowColor = ThemeColors.SelectionText;
            else if (Left(e.Item))
                e.ArrowColor = e.Item.ForeColor;
            base.OnRenderArrow(e);
        }
    }
}
