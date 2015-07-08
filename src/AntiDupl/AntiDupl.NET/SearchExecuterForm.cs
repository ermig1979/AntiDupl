/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar.
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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;

namespace AntiDupl.NET
{
    public class SearchExecuterForm : Form
    {
        private enum State
        {
            Start,
            ClearResults,
            ClearTemporary,
            LoadImages,
            Search,
            SetGroup,
            SetHint,
            SaveImages,
            Stopped,
            Finish
        }
        State m_state = State.Start;

        private CoreLib m_core;
        private Options m_options;
        private CoreOptions m_coreOptions;
        private MainSplitContainer m_mainSplitContainer;
        private MainForm m_mainForm;
        private System.Windows.Forms.Timer m_timer;
        private DateTime m_startDateTime;
        private FormWindowState m_mainFormWindowState;

        private Button m_stopButton;
        private Button m_minimizeToTaskbarButton;
        private Button m_minimizeToSystrayButton;
        private ProgressPanel m_progressPanel;
        private NotifyIcon m_notifyIcon;

        public SearchExecuterForm(CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer, MainForm mainForm)
        {
            m_core = core;
            m_options = options;
            m_coreOptions = coreOptions;
            m_mainSplitContainer = mainSplitContainer;
            m_mainForm = mainForm;
            m_mainFormWindowState = m_mainForm.WindowState;
            InitializeComponent();
            UpdateStrings();
            Owner = m_mainForm;
            m_mainForm.Resize += new EventHandler(OnMainFormResize);
        }

        private void InitializeComponent()
        {
            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2);
            mainTableLayoutPanel.Padding = new Padding(1, 5, 1, 0);
            Controls.Add(mainTableLayoutPanel);

            m_progressPanel = new ProgressPanel();
            mainTableLayoutPanel.Controls.Add(m_progressPanel, 0, 0);

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(5, 1);
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            mainTableLayoutPanel.Controls.Add(buttonsTableLayoutPanel, 0, 1);

            m_stopButton = new Button();
            m_stopButton.Click += new EventHandler(OnStopButtonClick);
            m_stopButton.AutoSize = true;
            buttonsTableLayoutPanel.Controls.Add(m_stopButton, 1, 0);

            m_minimizeToTaskbarButton = new Button();
            m_minimizeToTaskbarButton.Click += new EventHandler(OnMinimizeToTaskbarButtonClick);
            m_minimizeToTaskbarButton.AutoSize = true;
            buttonsTableLayoutPanel.Controls.Add(m_minimizeToTaskbarButton, 2, 0);

            m_minimizeToSystrayButton = new Button();
            m_minimizeToSystrayButton.Click += new EventHandler(OnMinimizeToSystrayButtonClick);
            m_minimizeToSystrayButton.AutoSize = true;
            buttonsTableLayoutPanel.Controls.Add(m_minimizeToSystrayButton, 3, 0);

            m_notifyIcon = new NotifyIcon();
            m_notifyIcon.Icon = Resources.Icons.Get();
            m_notifyIcon.Text = Application.ProductName;
            m_notifyIcon.DoubleClick += new EventHandler(OnNotifyIconDoubleClick);

            FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            ControlBox = false;
            MaximizeBox = false;
            MinimizeBox = false;
            KeyPreview = true;
            {
                int width = 800;
                int height = (m_progressPanel.Height + mainTableLayoutPanel.Margin.Vertical) +
                  m_stopButton.Height + m_stopButton.Padding.Vertical + mainTableLayoutPanel.Margin.Vertical +
                  mainTableLayoutPanel.Padding.Vertical;
                ClientSize = new Size(width, height);
            }

            m_timer = new System.Windows.Forms.Timer();
            m_timer.Interval = 100;
            m_timer.Tick += new EventHandler(TimerCallback);
            m_timer.Start();

            KeyDown += new KeyEventHandler(OnKeyDown); 
        }

        private void CoreThreadTask()
        {
            m_startDateTime = DateTime.Now;
            m_coreOptions.Set(m_core, m_options.onePath);
            m_state = State.ClearResults;
            m_core.Clear(CoreDll.FileType.Result);
            m_state = State.ClearTemporary;
            m_core.Clear(CoreDll.FileType.Temporary);
            if (m_options.useImageDataBase)
            {
                m_state = State.LoadImages;
                m_core.Load(CoreDll.FileType.ImageDataBase, m_coreOptions.GetImageDataBasePath(), false);
            }
            m_state = State.Search;
            m_core.Search();
            m_state = State.SetGroup;
            m_core.ApplyToResult(CoreDll.GlobalActionType.SetGroup);
            m_state = State.SetHint;
            m_core.ApplyToResult(CoreDll.GlobalActionType.SetHint);
            if (m_options.useImageDataBase)
            {
                m_state = State.SaveImages;
                m_core.Save(CoreDll.FileType.ImageDataBase, m_coreOptions.GetImageDataBasePath());
            }
            m_core.Clear(CoreDll.FileType.ImageDataBase);
            m_core.SortResult((CoreDll.SortType)m_options.resultsOptions.sortTypeDefault, m_options.resultsOptions.increasingDefault);
            m_state = State.Finish;
        }

        public void Execute()
        {
            m_mainSplitContainer.ClearResults();
            m_state = State.Start;
            Thread searchThread = new Thread(CoreThreadTask);
            searchThread.Start();
            m_stopButton.Enabled = true;
            ShowDialog();
        }

        void TimerCallback(Object obj, EventArgs eventArgs)
        {
            if (m_state == State.Finish)
            {
                if(m_notifyIcon.Visible)
                    OnNotifyIconDoubleClick(null, null);
                m_timer.Stop();
                Close();
                m_mainForm.Resize -= new EventHandler(OnMainFormResize);
                m_mainForm.WindowState = m_mainFormWindowState;
                m_mainForm.UpdateCaption();
                m_mainForm.Activate();
                m_mainSplitContainer.UpdateResults();
            }
            else
            {
                StringBuilder builder = new StringBuilder();
                if (m_notifyIcon.Visible || WindowState == FormWindowState.Minimized)
                {
                    builder.Append(Application.ProductName);
                    builder.Append(" - ");
                }
                Strings s = Resources.Strings.Current;
                switch (m_state)
                {
                    case State.Start:
                    case State.ClearResults:
                    case State.ClearTemporary:
                        {
                            m_stopButton.Enabled = false;
                            builder.Append(s.StartFinishForm_ClearTemporary_Text);
                            builder.Append("...");
                            EstimateOtherProgress();
                        }
                        break;
                    case State.LoadImages:
                        {
                            m_stopButton.Enabled = false; 
                            builder.Append(s.StartFinishForm_LoadImages_Text);
                            builder.Append("...");
                            EstimateOtherProgress();
                        }
                        break;
                    case State.Search:
                        {
                            m_stopButton.Enabled = true;
                            builder.Append(s.SearchExecuterForm_Search);
                            double progress = EstimateSearchProgress();
                            builder.AppendFormat(" ({0})...", ProgressUtils.GetProgressString(progress, m_startDateTime));
                        }
                        break;
                    case State.SetGroup:
                    case State.SetHint:
                        {
                            m_stopButton.Enabled = false;
                            builder.Append(s.SearchExecuterForm_Result);
                            builder.Append("...");
                            EstimateOtherProgress();
                        }
                        break;
                    case State.SaveImages:
                        {
                            m_stopButton.Enabled = false;
                            builder.Append(s.StartFinishForm_SaveImages_Text);
                            builder.Append("...");
                            EstimateOtherProgress();
                        }
                        break;
                    case State.Stopped:
                        {
                            builder.Append(s.SearchExecuterForm_Stopped);
                            builder.Append("...");
                        }
                        break;
                }
                if (m_notifyIcon.Visible)
                {
                    m_notifyIcon.Text = builder.ToString();
                }
                else if(WindowState == FormWindowState.Minimized)
                {
                    m_mainForm.Text = builder.ToString();
                }
                else
                {
                    Text = builder.ToString();
                    m_mainForm.UpdateCaption();
                }
            }
        }

        private void OnStopButtonClick(Object obj, EventArgs eventArgs)
        {
            if(m_state == State.Search)
            {
                m_core.Stop();
                m_state = State.Stopped;
                m_stopButton.Enabled = false;
            }
        }

        private void OnMinimizeToTaskbarButtonClick(Object obj, EventArgs eventArgs)
        {
            m_mainFormWindowState = m_mainForm.WindowState;
            WindowState = FormWindowState.Minimized;
            m_mainForm.WindowState = FormWindowState.Minimized;
        }

        private void OnMainFormResize(Object obj, EventArgs eventArgs)
        {
            if (!Modal)
            {
                m_mainForm.Hide();
                m_mainForm.WindowState = m_mainFormWindowState;
                m_mainForm.Show();
                WindowState = FormWindowState.Normal;
                ShowDialog();
                Activate();
            }
        }

        private void OnMinimizeToSystrayButtonClick(Object obj, EventArgs eventArgs)
        {
            m_mainFormWindowState = m_mainForm.WindowState;
            m_notifyIcon.Visible = true;
            Hide();
            m_mainForm.Hide();
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.Idle;
        }

        private void OnNotifyIconDoubleClick(Object obj, EventArgs eventArgs)
        {
            Process.GetCurrentProcess().PriorityClass = ProcessPriorityClass.Normal;
            m_notifyIcon.Visible = false;
            m_mainForm.WindowState = m_mainFormWindowState;
            m_mainForm.Show();
            WindowState = FormWindowState.Normal;
            ShowDialog();
            Activate();
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_stopButton.Text = s.StopButton_Text;
            m_minimizeToTaskbarButton.Text = s.SearchExecuterForm_MinimizeToTaskbarButton_Text;
            m_minimizeToSystrayButton.Text = s.SearchExecuterForm_MinimizeToSystrayButton_Text;
        }

        private double EstimateSearchProgress()
        {
            double progress = 0;
            int total = 0, currentFirst = 0, currentSecond = 0;
            string path = "";

            CoreStatus mainThreadStatus = m_core.StatusGet(CoreDll.ThreadType.Main, 0);
            if (mainThreadStatus != null)
            {
                total = mainThreadStatus.total;
                if(mainThreadStatus.current > 0)
                {
                    if(m_coreOptions.compareOptions.checkOnEquality)
                    {
                        for(int i = 0; ; i++)
                        {
                            CoreStatus compareThreadStatus = m_core.StatusGet(CoreDll.ThreadType.Compare, i);
                            if (compareThreadStatus == null)
                                break;
                            if(i == 0)
                            {
                                path = compareThreadStatus.path;
                            }
                            currentFirst += compareThreadStatus.current;
                            currentSecond += compareThreadStatus.total;
                        }
                    }
                    else
                    {
                        currentFirst = mainThreadStatus.current;
                        for (int i = 0; ; i++)
                        {
                            CoreStatus collectThreadStatus = m_core.StatusGet(CoreDll.ThreadType.Collect, i);
                            if(collectThreadStatus == null)
                                break;
                            if (i == 0)
                            {
                                path = collectThreadStatus.path;
                            }
                            currentFirst += collectThreadStatus.current;
                            currentFirst -= collectThreadStatus.total;
                        }
                    }
                }
                else
                {
                    path = mainThreadStatus.path;
                }
            }

            m_progressPanel.UpdateStatus(total, currentFirst, currentSecond, path);

            if(total > 0)
            {
                progress = (double)currentFirst/(double)total;
            }

            return progress;
        }

        private void EstimateOtherProgress()
        {
            CoreStatus status = m_core.StatusGet(CoreDll.ThreadType.Main, 0);
            if (status != null)
                m_progressPanel.UpdateStatus(status.total, status.current, status.current, "");
            else
                m_progressPanel.UpdateStatus(0, 0, 0, "");
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Escape)
            {
                if (m_state == State.Search)
                {
                    m_core.Stop();
                    m_state = State.Stopped;
                    m_stopButton.Enabled = false;
                }
            }
            else
            {
                e.Handled = true;
            }
        }
    }
}