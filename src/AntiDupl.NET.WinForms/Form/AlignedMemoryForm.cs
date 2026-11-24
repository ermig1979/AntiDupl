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
using System.Drawing;
using System.Windows.Forms;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// Says, as an aligned search starts, that its smallest crops need more
    /// memory than is free, or more time than its limit, and what can be
    /// done. Not modal: the search goes on while it is read, and it stays
    /// over the results until closed.
    /// </summary>
    public class AlignedMemoryForm : Form
    {
        private const int TEXT_WIDTH = 520;

        private Action m_stop;
        private Label m_label;
        private Button m_stopButton;
        private Button m_closeButton;

        /// <summary>Whether its Stop button was pressed.</summary>
        public bool Stopped { get; private set; }

        /// <summary>
        /// Whether the time limit is worth a warning: it held Auto below the
        /// highest level, or even the lowest level, or a level set in the
        /// options, takes longer than it.
        /// </summary>
        public static bool TimeNeeded(CoreDll.adAlignedSearch search, bool auto)
        {
            if (search.limitSeconds == 0)
                return false;
            if (auto)
                return search.timeLevel < CoreCompareOptions.ALIGNED_LEVEL_COUNT || search.usedSeconds > search.limitSeconds;
            return search.usedSeconds > search.limitSeconds;
        }

        public AlignedMemoryForm(CoreDll.adAlignedSearch search, bool auto, Action stop)
        {
            m_stop = stop;
            InitializeComponent();
            ThemeColors.Apply(this);
            UpdateTimeStrings(search, auto);
        }

        /// <summary>
        /// Whether the search's choice is worth a warning: memory held Auto
        /// below the level wanted (for Auto, what the time limit allows), or
        /// the level used needs more than is free.
        /// </summary>
        public static bool Needed(CoreDll.adAlignedSearch memory)
        {
            return memory.used < memory.wanted || memory.usedBytes > memory.freeBytes;
        }

        public AlignedMemoryForm(CoreDll.adAlignedSearch memory, Action stop)
        {
            m_stop = stop;
            InitializeComponent();
            ThemeColors.Apply(this);
            UpdateStrings(memory);
        }

        private void InitializeComponent()
        {
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;
            AutoSize = true;
            AutoSizeMode = AutoSizeMode.GrowAndShrink;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 10);
            mainTableLayoutPanel.AutoSize = true;
            mainTableLayoutPanel.Dock = DockStyle.None;
            Controls.Add(mainTableLayoutPanel);

            m_label = new Label();
            m_label.AutoSize = true;
            m_label.MaximumSize = new Size(TEXT_WIDTH, 0);
            m_label.Margin = new Padding(0, 0, 0, 10);
            mainTableLayoutPanel.Controls.Add(m_label, 0, 0);

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(3, 1);
            buttonsTableLayoutPanel.AutoSize = true;
            buttonsTableLayoutPanel.Dock = DockStyle.Right;
            mainTableLayoutPanel.Controls.Add(buttonsTableLayoutPanel, 0, 1);

            m_stopButton = new Button();
            m_stopButton.AutoSize = true;
            m_stopButton.Click += new EventHandler(OnStopButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_stopButton, 0, 0);

            m_closeButton = new Button();
            m_closeButton.AutoSize = true;
            m_closeButton.Click += new EventHandler(OnCloseButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_closeButton, 1, 0);
            CancelButton = m_closeButton;
        }

        private static string Level(Strings s, int level)
        {
            return AlignedText.Level(s, level);
        }

        private static string Memory(Strings s, ulong bytes)
        {
            return AlignedText.Memory(s, bytes);
        }

        private void UpdateStrings(CoreDll.adAlignedSearch memory)
        {
            Strings s = Resources.Strings.Current;
            Text = s.AlignedMemoryForm_Text;
            m_stopButton.Text = s.AlignedMemoryForm_StopButton_Text;
            m_closeButton.Text = s.AlignedMemoryForm_CloseButton_Text;

            string size = string.Format(s.AlignedMemoryForm_Size, memory.imageCount);
            string free = string.Format(s.AlignedMemoryForm_Free, Memory(s, memory.freeBytes));
            string text;
            if (memory.used < memory.wanted && memory.usedBytes <= memory.freeBytes)
            {
                // Auto took a lower level, which fits.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_Needed, Memory(s, memory.wantedBytes), Level(s, memory.wanted)) + "\n" +
                    free + "\n" +
                    string.Format(s.AlignedMemoryForm_Instead, Level(s, memory.used), Memory(s, memory.usedBytes)) +
                    "\n\n" + s.AlignedMemoryForm_Choices;
            }
            else if (memory.used == 1)
            {
                // Not even the lowest level fits.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_NeededLowest, Memory(s, memory.usedBytes), Level(s, memory.used)) + "\n" +
                    free + "\n" + s.AlignedMemoryForm_ContinuesLowest + "\n\n" + s.AlignedMemoryForm_ChoicesLowest;
            }
            else
            {
                // A level set in the options, kept though it doesn't fit.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_Needed, Memory(s, memory.usedBytes), Level(s, memory.used)) + "\n" +
                    free + "\n" + s.AlignedMemoryForm_ContinuesForced + "\n\n" + s.AlignedMemoryForm_ChoicesForced;
            }
            m_label.Text = text;
        }

        private void UpdateTimeStrings(CoreDll.adAlignedSearch search, bool auto)
        {
            Strings s = Resources.Strings.Current;
            Text = s.AlignedMemoryForm_TimeText;
            m_stopButton.Text = s.AlignedMemoryForm_StopButton_Text;
            m_closeButton.Text = s.AlignedMemoryForm_CloseButton_Text;

            string size = string.Format(s.AlignedMemoryForm_Size, search.imageCount);
            string limit = string.Format(s.AlignedMemoryForm_TimeLimit, AlignedText.Duration(s, search.limitSeconds));
            string text;
            if (auto && search.timeLevel == 1 && search.used == 1 && search.usedSeconds > search.limitSeconds)
            {
                // Not even the lowest level is within the limit.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_TimeNeededLowest, AlignedText.Duration(s, search.usedSeconds), Level(s, 1)) + "\n" +
                    limit + "\n" + s.AlignedMemoryForm_TimeContinuesLowest + "\n\n" + s.AlignedMemoryForm_TimeChoicesLowest;
            }
            else if (auto)
            {
                // Auto took the level the limit allows; "wanted" is that level.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_TimeNeeded, AlignedText.Duration(s, search.topSeconds),
                        Level(s, CoreCompareOptions.ALIGNED_LEVEL_COUNT)) + "\n" +
                    limit + "\n" +
                    string.Format(s.AlignedMemoryForm_TimeInstead, Level(s, search.wanted), AlignedText.Duration(s, search.wantedSeconds)) +
                    "\n\n" + s.AlignedMemoryForm_TimeChoices;
            }
            else
            {
                // A level set in the options, kept though it takes longer.
                text = size + "\n" +
                    string.Format(s.AlignedMemoryForm_TimeNeeded, AlignedText.Duration(s, search.usedSeconds), Level(s, search.used)) + "\n" +
                    limit + "\n" + s.AlignedMemoryForm_TimeContinuesForced + "\n\n" + s.AlignedMemoryForm_TimeChoicesForced;
            }
            m_label.Text = text;
        }

        /// <summary>Once the search has ended, there is nothing to stop.</summary>
        public void SearchEnded()
        {
            m_stopButton.Enabled = false;
        }

        private void OnStopButtonClick(object sender, EventArgs e)
        {
            Stopped = true;
            m_stop();
            m_stopButton.Enabled = false;
        }

        private void OnCloseButtonClick(object sender, EventArgs e)
        {
            Close();
        }
    }
}
