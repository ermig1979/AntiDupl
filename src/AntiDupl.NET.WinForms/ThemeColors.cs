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
using System.Runtime.InteropServices;
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

        /// <summary>
        /// The form's text colour, which its controls inherit unless they
        /// set their own, the style of its buttons, the theme of the
        /// drop-down lists on its tab pages, and no white flash when it
        /// opens.
        /// </summary>
        static public void Apply(Form form)
        {
            if (!Dark)
                return;
            form.ForeColor = Text;
            ApplyToControls(form);
            HideUntilPainted(form);
        }

        [DllImport("dwmapi.dll")]
        static private extern int DwmSetWindowAttribute(System.IntPtr hwnd, int attribute, ref int value, int size);

        [DllImport("user32.dll")]
        static private extern bool RedrawWindow(System.IntPtr hwnd, System.IntPtr rect, System.IntPtr region, uint flags);

        private const int DWMWA_CLOAK = 13;
        private const uint RDW_INVALIDATE = 0x0001, RDW_ALLCHILDREN = 0x0080, RDW_UPDATENOW = 0x0100;

        // A new window shows each of its controls as a white box until the
        // control has painted itself once, a white flash on a dark window.
        // The window manager keeps the window hidden (cloaked) from its
        // creation until it and all its controls have been painted. Only
        // before it is first shown: a window created again later stays
        // visible, since nothing would uncloak it.
        static private void HideUntilPainted(Form form)
        {
            bool shown = false;
            form.HandleCreated += delegate
            {
                if (!shown)
                    SetCloak(form, true);
            };
            form.Shown += delegate
            {
                shown = true;
                form.BeginInvoke(new System.Action(() =>
                {
                    if (form.IsDisposed || !form.IsHandleCreated)
                        return;
                    RedrawWindow(form.Handle, System.IntPtr.Zero, System.IntPtr.Zero, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
                    SetCloak(form, false);
                }));
            };
        }

        static private void SetCloak(Form form, bool cloak)
        {
            int value = cloak ? 1 : 0;
            DwmSetWindowAttribute(form.Handle, DWMWA_CLOAK, ref value, sizeof(int));
        }

        static private void ApplyToControls(Control parent)
        {
            foreach (Control control in parent.Controls)
            {
                // In dark mode Windows draws a standard button with a white
                // frame when it has the focus or is the default button;
                // .NET's flat style has a grey one, and marks the default
                // button with a second line.
                if (control is Button button)
                    button.FlatStyle = FlatStyle.Flat;
                if (control is TabControl tabControl)
                    tabControl.HandleCreated += new System.EventHandler(OnTabControlHandleCreated);
                ApplyToControls(control);
            }
        }

        [DllImport("uxtheme.dll", CharSet = CharSet.Unicode)]
        static private extern int SetWindowTheme(System.IntPtr hwnd, string appName, string idList);

        // Once its handle is created, .NET's TabControl gives every window on
        // its selected page Explorer's dark theme. The drop-down lists there
        // already have the dark combo-box theme, which this replaces, and
        // they draw light; it is set again once the tab control is done.
        static private void OnTabControlHandleCreated(object sender, System.EventArgs e)
        {
            TabControl tabControl = (TabControl)sender;
            tabControl.BeginInvoke(new System.Action(() => SetComboBoxTheme(tabControl)));
        }

        static private void SetComboBoxTheme(Control parent)
        {
            foreach (Control control in parent.Controls)
            {
                if (control is ComboBox && control.IsHandleCreated)
                {
                    SetWindowTheme(control.Handle, "DarkMode_CFD", null);
                    control.Invalidate();
                }
                SetComboBoxTheme(control);
            }
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

        /// <summary>The same for a list, which fills its frame.</summary>
        static public Control Framed(ListBox listBox)
        {
            if (!Dark)
                return listBox;
            listBox.BorderStyle = BorderStyle.None;
            return new FrameHost(listBox, listBox.Height);
        }

        /// <summary>
        /// A panel in the inner control's background colour with a 1 px
        /// ControlDark line, taking over the inner control's place (size,
        /// margin, dock, anchor) in the layout. A list fills it; the
        /// borderless text or number box sits in it vertically centred.
        /// </summary>
        private class FrameHost : Panel
        {
            private readonly Control m_inner;
            private readonly TextPartPlacer m_textPart;

            public FrameHost(Control inner, int height)
            {
                m_inner = inner;
                if (inner is UpDownBase)
                    foreach (Control child in inner.Controls)
                        if (child is TextBoxBase)
                            m_textPart = new TextPartPlacer(child);
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
                if (m_inner is ListBox)
                {
                    m_inner.SetBounds(1, 1, Width - 2, Height - 2);
                    return;
                }
                // Both boxes draw their text line at the top of their text
                // part. Capitals and digits sit a pixel high in the line, so
                // an odd pixel of room goes above it.
                int line = m_inner.Font.Height;
                int lineTop = (Height - line + 1) / 2;
                if (m_textPart != null)
                {
                    // The number box keeps its own height, its arrows
                    // centred; only its text part is moved.
                    m_inner.SetBounds(3, (Height - m_inner.Height) / 2, Width - 5, m_inner.Height);
                    m_textPart.Place(lineTop - m_inner.Top, line);
                }
                else
                    m_inner.SetBounds(3, lineTop, Width - 5, line);
            }

            /// <summary>
            /// Keeps a number box's text part at the given top and one text
            /// line tall. The number box lays it out at its own full height
            /// with the text at the top, on every resize, font change and
            /// layout, so the position is changed on its way to the window.
            /// </summary>
            private class TextPartPlacer : NativeWindow
            {
                private const int WM_WINDOWPOSCHANGING = 0x0046;
                private const int SWP_NOSIZE = 0x0001;
                private const int SWP_NOMOVE = 0x0002;

                [StructLayout(LayoutKind.Sequential)]
                private struct WINDOWPOS
                {
                    public System.IntPtr hwnd, hwndInsertAfter;
                    public int x, y, cx, cy, flags;
                }

                private readonly Control m_control;
                private int m_top, m_height;

                public TextPartPlacer(Control control)
                {
                    m_control = control;
                    control.HandleCreated += delegate { AssignHandle(control.Handle); };
                    control.HandleDestroyed += delegate { ReleaseHandle(); };
                    if (control.IsHandleCreated)
                        AssignHandle(control.Handle);
                }

                public void Place(int top, int height)
                {
                    m_top = top;
                    m_height = height;
                    if (m_control.Top != top || m_control.Height != height)
                        m_control.SetBounds(m_control.Left, top, m_control.Width, height);
                }

                protected override void WndProc(ref Message m)
                {
                    if (m.Msg == WM_WINDOWPOSCHANGING && m_height > 0)
                    {
                        WINDOWPOS pos = Marshal.PtrToStructure<WINDOWPOS>(m.LParam);
                        if ((pos.flags & SWP_NOMOVE) != 0)
                            pos.x = m_control.Left;
                        if ((pos.flags & SWP_NOSIZE) != 0)
                            pos.cx = m_control.Width;
                        pos.y = m_top;
                        pos.cy = m_height;
                        pos.flags &= ~(SWP_NOMOVE | SWP_NOSIZE);
                        Marshal.StructureToPtr(pos, m.LParam, false);
                    }
                    base.WndProc(ref m);
                }
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
