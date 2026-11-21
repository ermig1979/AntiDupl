using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// A label whose text can be selected with the mouse and copied, with
    /// parts in colours of their own. It keeps the focus only while something
    /// is selected: any other key goes to Release, for the results list's hot
    /// keys to keep working.
    /// </summary>
    public class SelectableLabel : Label
    {
        private const TextFormatFlags FLAGS = TextFormatFlags.NoPrefix | TextFormatFlags.SingleLine |
            TextFormatFlags.NoPadding | TextFormatFlags.PreserveGraphicsClipping;
        private const string ELLIPSIS = "...";

        private struct Mark
        {
            public int start;
            public int length;
            public Color color;
        }
        private readonly List<Mark> m_marks = new List<Mark>();

        // What is drawn: the text, or with MiddleEllipsis its start and end
        // around an ellipsis, m_head characters from the start, the end
        // from m_tail on.
        private string m_shown;
        private int m_head = -1;
        private int m_tail;
        private int[] m_x;
        private int m_left;

        private int m_anchor;
        private int m_caret;
        private bool m_selecting;

        /// <summary>Cuts the middle of a text that doesn't fit, and keeps the file name.</summary>
        public bool MiddleEllipsis { get; set; }

        /// <summary>Called with the key, or null after a click, when the label is done with the focus.</summary>
        public Action<KeyEventArgs> Release { get; set; }

        public SelectableLabel()
        {
            DoubleBuffered = true;
            UseMnemonic = false;
            TabStop = false;
            Cursor = Cursors.IBeam;
            SetStyle(ControlStyles.Selectable, true);

            ContextMenuStrip menu = new ContextMenuStrip();
            ThemedToolStripRenderer.Set(menu);
            ToolStripMenuItem copy = new ToolStripMenuItem();
            copy.Click += delegate
            {
                Copy();
                Done(null);
            };
            menu.Items.Add(copy);
            menu.Opening += delegate { copy.Text = Resources.Strings.Current.SelectableLabel_Copy_Text; };
            ContextMenuStrip = menu;
        }

        public void ClearMarks()
        {
            if (m_marks.Count > 0)
            {
                m_marks.Clear();
                Invalidate();
            }
        }

        public void AddMark(int start, int length, Color color)
        {
            m_marks.Add(new Mark { start = start, length = length, color = color });
            Invalidate();
        }

        private Rectangle TextArea()
        {
            return new Rectangle(Padding.Left, Padding.Top, ClientSize.Width - Padding.Horizontal, ClientSize.Height - Padding.Vertical);
        }

        private int Measure(string text)
        {
            return text.Length == 0 ? 0 : TextRenderer.MeasureText(text, Font, Size.Empty, FLAGS).Width;
        }

        private void Forget()
        {
            m_shown = null;
            m_anchor = m_caret = 0;
            Invalidate();
        }

        private void Prepare()
        {
            if (m_shown != null)
                return;
            Rectangle area = TextArea();
            string text = Text;
            m_shown = text;
            m_head = -1;
            if (MiddleEllipsis && Measure(text) > area.Width)
                CutMiddle(text, area.Width);

            m_x = new int[m_shown.Length + 1];
            for (int i = 1; i <= m_shown.Length; i++)
                m_x[i] = Measure(m_shown.Substring(0, i));

            int width = m_x[m_shown.Length];
            switch (TextAlign)
            {
                case ContentAlignment.TopCenter:
                case ContentAlignment.MiddleCenter:
                case ContentAlignment.BottomCenter:
                    m_left = area.Left + (area.Width - width) / 2;
                    break;
                case ContentAlignment.TopRight:
                case ContentAlignment.MiddleRight:
                case ContentAlignment.BottomRight:
                    m_left = area.Right - width;
                    break;
                default:
                    m_left = area.Left;
                    break;
            }
        }

        private void CutMiddle(string text, int width)
        {
            int name = text.LastIndexOf('\\') + 1;
            m_tail = name;
            if (Measure(ELLIPSIS + text.Substring(name)) > width)
            {
                // Not even the file name fits: as much of its end as does.
                int low = name, high = text.Length;
                while (low < high)
                {
                    int middle = (low + high) / 2;
                    if (Measure(ELLIPSIS + text.Substring(middle)) > width)
                        low = middle + 1;
                    else
                        high = middle;
                }
                m_tail = low;
                m_head = 0;
            }
            else
            {
                int low = 0, high = name;
                while (low < high)
                {
                    int middle = (low + high + 1) / 2;
                    if (Measure(text.Substring(0, middle) + ELLIPSIS + text.Substring(name)) > width)
                        high = middle - 1;
                    else
                        low = middle;
                }
                m_head = low;
            }
            m_shown = text.Substring(0, m_head) + ELLIPSIS + text.Substring(m_tail);
        }

        // Where a place in what is drawn is in the text; inside the ellipsis
        // that is the end of the part before it, or with after the start of
        // the part after it.
        private int TextIndex(int shown, bool after)
        {
            if (m_head < 0 || shown <= m_head)
                return shown;
            if (shown >= m_head + ELLIPSIS.Length)
                return m_tail + shown - m_head - ELLIPSIS.Length;
            return after ? m_tail : m_head;
        }

        private int ShownIndex(int x)
        {
            Prepare();
            int place = x - m_left;
            for (int i = 0; i < m_shown.Length; i++)
                if (place < (m_x[i] + m_x[i + 1]) / 2)
                    return i;
            return m_shown.Length;
        }

        private Color ColorAt(int shown, Color text)
        {
            Color color = text;
            if (m_head >= 0 && shown >= m_head && shown < m_head + ELLIPSIS.Length)
                return color;
            int index = TextIndex(shown, false);
            foreach (Mark mark in m_marks)
                if (index >= mark.start && index < mark.start + mark.length)
                    color = mark.color;
            return color;
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            // NOT to be left out for the text being drawn again below: it
            // raises Paint, which draws the dark theme's border.
            base.OnPaint(e);

            Prepare();
            Rectangle inner = ClientRectangle;
            inner.Inflate(-1, -1);
            using (SolidBrush back = new SolidBrush(BackColor))
                e.Graphics.FillRectangle(back, inner);

            int start = Math.Min(m_anchor, m_caret), end = Math.Max(m_anchor, m_caret);
            int top = TextArea().Top;
            if (start != end)
                using (SolidBrush selection = new SolidBrush(ThemeColors.Selection))
                    e.Graphics.FillRectangle(selection, m_left + m_x[start], top, m_x[end] - m_x[start], Font.Height);

            int run = 0;
            while (run < m_shown.Length)
            {
                bool selected = run >= start && run < end;
                Color color = selected ? ThemeColors.SelectionText : ColorAt(run, ForeColor);
                int next = run + 1;
                while (next < m_shown.Length && (next >= start && next < end) == selected &&
                    (selected || ColorAt(next, ForeColor) == color))
                    next++;
                TextRenderer.DrawText(e.Graphics, m_shown.Substring(run, next - run), Font,
                    new Point(m_left + m_x[run], top), color, FLAGS);
                run = next;
            }
        }

        protected override void OnTextChanged(EventArgs e)
        {
            base.OnTextChanged(e);
            Forget();
        }

        protected override void OnFontChanged(EventArgs e)
        {
            base.OnFontChanged(e);
            Forget();
        }

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            Forget();
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (e.Button != MouseButtons.Left)
                return;
            Focus();
            m_anchor = m_caret = ShownIndex(e.X);
            m_selecting = true;
            Invalidate();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            if (m_selecting)
            {
                m_caret = ShownIndex(e.X);
                Invalidate();
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            if (e.Button != MouseButtons.Left)
                return;
            m_selecting = false;
            if (m_anchor == m_caret)
                Done(null);
        }

        protected override void OnDoubleClick(EventArgs e)
        {
            base.OnDoubleClick(e);
            SelectAll();
        }

        protected override void OnLostFocus(EventArgs e)
        {
            base.OnLostFocus(e);
            m_anchor = m_caret = 0;
            Invalidate();
        }

        protected override bool IsInputKey(Keys keyData)
        {
            return true;
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            e.Handled = true;
            if (e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.ControlKey || e.KeyCode == Keys.Menu)
                return;
            if (e.Control && e.KeyCode == Keys.C)
                Copy();
            else if (e.Control && e.KeyCode == Keys.A)
                SelectAll();
            else
                Done(new KeyEventArgs(e.KeyData)); // not this one: the list ignores a handled key
        }

        private void SelectAll()
        {
            Prepare();
            m_anchor = 0;
            m_caret = m_shown.Length;
            Invalidate();
        }

        private void Done(KeyEventArgs key)
        {
            m_anchor = m_caret = 0;
            Invalidate();
            if (Release != null)
                Release(key);
        }

        /// <summary>Copies what is selected, or all of the text with nothing selected.</summary>
        private void Copy()
        {
            Prepare();
            string text = Text;
            int start = Math.Min(m_anchor, m_caret), end = Math.Max(m_anchor, m_caret);
            if (start != end)
            {
                int from = TextIndex(start, false), to = TextIndex(end, true);
                text = text.Substring(from, to - from);
            }
            if (text.Length > 0)
                Clipboard.SetText(text);
        }
    }
}
