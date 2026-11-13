using System;
using System.Drawing;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// The status bar's counts, for the bar beside the preview's buttons: side
    /// by side, or stacked under short titles for the vertical view's narrow
    /// bar, right-aligned and cut off at its left edge.
    /// </summary>
    public class ResultCountsPanel : Control
    {
        private const int GAP = 12;
        private const int LEFT_PADDING = 1;
        private const int RIGHT_PADDING = 3;
        // Side by side, the total keeps this much room, so that the columns
        // stay put as the count grows.
        private const string TOTAL_ROOM = "000000";
        // One pixel under the bar's centre.
        private const int SIDE_TOP = 1;
        // Below the bar's row of 5 pixels, this puts the first title's letters
        // 12 pixels under the lowest button's drawing.
        private const int STACK_TOP = 5;
        private const int PAIR_TIGHTENING = 7;
        private const TextFormatFlags FLAGS = TextFormatFlags.NoPrefix | TextFormatFlags.SingleLine | TextFormatFlags.NoPadding;

        private string[] m_titles = { "", "", "" };
        private string[] m_shortTitles = { "", "", "" };
        private string[] m_values = { "0", "0", "0" };
        private bool m_stacked;

        public ResultCountsPanel()
        {
            DoubleBuffered = true;
            ForeColor = ThemeColors.Text;
            Margin = new Padding(0);
        }

        public bool Stacked
        {
            get { return m_stacked; }
            set { m_stacked = value; Fit(); }
        }

        public void SetTitles(string total, string current, string selected, string currentShort, string selectedShort)
        {
            m_titles = new string[] { total, current, selected };
            m_shortTitles = new string[] { total, currentShort, selectedShort };
            Fit();
        }

        public void SetCounts(int total, int current, int selected)
        {
            m_values = new string[] { total.ToString(), current.ToString(), selected.ToString() };
            Fit();
        }

        private int Measure(string text)
        {
            return TextRenderer.MeasureText(text, Font, Size.Empty, FLAGS).Width;
        }

        private int ColumnWidth(int i)
        {
            int width = Math.Max(Measure(m_titles[i]), Measure(m_values[i]));
            return i == 0 ? Math.Max(width, Measure(TOTAL_ROOM)) : width;
        }

        // Side by side the panel is as large as its text; stacked it takes
        // the bar's width from the layout, and adds nothing to it.
        private void Fit()
        {
            if (!m_stacked)
            {
                int width = LEFT_PADDING + 2 * GAP;
                for (int i = 0; i < 3; i++)
                    width += ColumnWidth(i);
                Size = new Size(width + 1, 2 * Font.Height);
            }
            Invalidate();
        }

        private void DrawRight(Graphics graphics, string text, int right, int top)
        {
            TextRenderer.DrawText(graphics, text, Font, new Point(right - Measure(text), top), ForeColor, FLAGS);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            int line = Font.Height;
            if (m_stacked)
            {
                int right = ClientSize.Width - RIGHT_PADDING;
                for (int i = 0; i < 3; i++)
                {
                    int top = STACK_TOP + i * (3 * line - PAIR_TIGHTENING);
                    DrawRight(e.Graphics, m_shortTitles[i], right, top);
                    DrawRight(e.Graphics, m_values[i], right, top + line);
                }
                return;
            }
            int column = LEFT_PADDING;
            for (int i = 0; i < 3; i++)
            {
                column += ColumnWidth(i);
                DrawRight(e.Graphics, m_titles[i], column, SIDE_TOP);
                DrawRight(e.Graphics, m_values[i], column, SIDE_TOP + line);
                column += GAP;
            }
        }
    }
}
