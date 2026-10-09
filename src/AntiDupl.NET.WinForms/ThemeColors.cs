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
    }
}
