/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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
using System.IO;
using System.Drawing;
using System.Threading;

namespace AntiDupl.NET
{
    public class StartFinishForm : Form
    {
        static private TimeSpan VIEW_START_TIME_MIN = TimeSpan.FromSeconds(3.0);

        private enum State
        {
            Start,
            LoadImages,
            LoadMistakes,
            LoadResults,
            ViewStart,
            SaveImages,
            SaveMistakes,
            SaveResults,
            ClearResults,
            ClearTemporary,
            Finish
        }
        State m_state = State.Start;

        private CoreLib m_core;
        private Options m_options;
        private System.Windows.Forms.Timer m_timer;

        private AboutProgramPanel m_aboutProgramPanel;
        private ProgressBar m_progressBar;

        public StartFinishForm(CoreLib core, Options options)
        {
            m_core = core;
            m_options = options;

            InitializeComponent();
        }

        private void InitializeComponent()
        {
            ClientSize = new System.Drawing.Size(310, 200);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            ControlBox = false;
            MaximizeBox = false;
            MinimizeBox = false;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 90F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 10F));
            Controls.Add(mainTableLayoutPanel);

            m_aboutProgramPanel = new AboutProgramPanel(m_core);
            m_aboutProgramPanel.Location = new Point(0, 0);
            m_aboutProgramPanel.Dock = DockStyle.Fill;
            mainTableLayoutPanel.Controls.Add(m_aboutProgramPanel, 0, 0);

            m_progressBar = new ProgressBar();
            m_progressBar.Location = new Point(0, 0);
            m_progressBar.Dock = DockStyle.Bottom;
            m_progressBar.Style = ProgressBarStyle.Continuous;
            m_progressBar.Margin = new Padding(0);
            mainTableLayoutPanel.Controls.Add(m_progressBar, 0, 1);

            m_timer = new System.Windows.Forms.Timer();
            m_timer.Interval = 100;
            m_timer.Tick += new EventHandler(TimerCallback);
            m_timer.Start();

            FormClosed += new FormClosedEventHandler(OnFormClosed);
        }

        private void CoreStartThreadTask()
        {
            DateTime startTime = DateTime.Now;

            m_state = State.LoadMistakes;
            m_core.Load(CoreDll.FileType.MistakeDataBase, Options.GetMistakeDataBaseFileName(), m_options.checkMistakesAtLoading);

            m_state = State.LoadResults;
            if (!File.Exists(m_options.resultsOptions.resultsFileName))
                m_options.resultsOptions.resultsFileName = ResultsOptions.GetDefaultResultsFileName();
            m_core.Load(CoreDll.FileType.Result, m_options.resultsOptions.resultsFileName, m_options.checkResultsAtLoading);

            TimeSpan viewTime = DateTime.Now - startTime;
            if (viewTime < VIEW_START_TIME_MIN)
            {
                m_state = State.ViewStart;
                Thread.Sleep(VIEW_START_TIME_MIN - viewTime);
            }

            m_state = State.Finish;
        }

        private void CoreFinishThreadTask()
        {
            m_state = State.SaveMistakes;
            m_core.Save(CoreDll.FileType.MistakeDataBase, Options.GetMistakeDataBaseFileName());

            m_state = State.SaveResults;
            m_core.Save(CoreDll.FileType.Result, m_options.resultsOptions.resultsFileName);

            m_state = State.ClearResults;
            m_core.Clear(CoreDll.FileType.Result);

            m_state = State.ClearTemporary;
            m_core.Clear(CoreDll.FileType.Temporary);

            m_state = State.Finish;
        }

        public void ExecuteStart()
        {
            ShowInTaskbar = true;
            m_state = State.Start;
            Thread searchThread = new Thread(CoreStartThreadTask);
            searchThread.Start();
            ShowDialog();
        }

        public void ExecuteFinish()
        {
            m_state = State.Start;
            Thread searchThread = new Thread(CoreFinishThreadTask);
            searchThread.Start();
            ShowDialog();
        }

        void TimerCallback(Object obj, EventArgs eventArgs)
        {
            if (m_state == State.Finish)
            {
                Close();
            }
            else if(m_state == State.ViewStart)
            {
                m_progressBar.Visible = false;
                Text = Application.ProductName;
            }
            else
            {
                StringBuilder builder = new StringBuilder();
                builder.Append(Application.ProductName);
                builder.Append(" - ");

                Strings s = Resources.Strings.Current;
                switch (m_state)
                {
                    case State.Start:
                    case State.LoadImages:
                        builder.Append(s.StartFinishForm_LoadImages_Text);
                        break;
                    case State.LoadMistakes:
                        builder.Append(s.StartFinishForm_LoadMistakes_Text);
                        break;
                    case State.LoadResults:
                        builder.Append(s.StartFinishForm_LoadResults_Text);
                        break;
                    case State.SaveImages:
                        builder.Append(s.StartFinishForm_SaveImages_Text);
                        break;
                    case State.SaveMistakes:
                        builder.Append(s.StartFinishForm_SaveMistakes_Text);
                        break;
                    case State.SaveResults:
                        builder.Append(s.StartFinishForm_SaveResults_Text);
                        break;
                    case State.ClearResults:
                    case State.ClearTemporary:
                        builder.Append(s.StartFinishForm_ClearTemporary_Text);
                        break;
                }
                Text = builder.ToString();

                CoreStatus status = m_core.StatusGet(CoreDll.ThreadType.Main, 0);
                if (status != null)
                {
                    if (status.total > 0)
                    {
                        m_progressBar.Value = status.current * m_progressBar.Maximum / status.total;
                    }
                    else
                    {
                        m_progressBar.Value = 0;
                    }
                }
            }
        }

        private void OnFormClosed(object sender, FormClosedEventArgs e)
        {
            m_timer.Stop();
        }
    }
}
