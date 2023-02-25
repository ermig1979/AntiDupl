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

namespace AntiDupl.NET
{
    public class DataGridViewCustomRow : DataGridViewRow
    {
        public bool current = false;
        public bool selected = false;
        public bool updated = false;

        protected override void Paint(Graphics graphics, Rectangle clipBounds, Rectangle rowBounds, int rowIndex, DataGridViewElementStates rowState, bool isFirstDisplayedRow, bool isLastVisibleRow)
        {
            DataGridViewElementStates state =
              (DataGridViewElementStates)((int)rowState & ~(int)DataGridViewElementStates.Selected) |
              (selected ? DataGridViewElementStates.Selected : 0);
            base.Paint(graphics, clipBounds, rowBounds, rowIndex, state, isFirstDisplayedRow, isLastVisibleRow);
            if (current)
            {
                int cellsWidth = 0;
                for (int i = 0; i < Cells.Count; i++)
                {
                    if (Cells[i].Visible)
                    {
                        cellsWidth += Cells[i].Size.Width;
                    }
                }
                Rectangle cellsBounds = new Rectangle(rowBounds.X, rowBounds.Y, 
                    rowBounds.X + cellsWidth - 2, rowBounds.Height - 2);
                
                Rectangle visibleClipBounds = new Rectangle(
                    (int)graphics.VisibleClipBounds.X, 
                    (int)graphics.VisibleClipBounds.Y,
                    (int)graphics.VisibleClipBounds.Width - 1, 
                    (int)graphics.VisibleClipBounds.Height - 1);
                visibleClipBounds.Intersect(cellsBounds);

                Pen pen = new Pen(Color.White);
                graphics.DrawRectangle(pen, visibleClipBounds);
                pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dot;
                pen.Color = Color.Black;
                graphics.DrawRectangle(pen, visibleClipBounds);
            }
        }
    }
}
