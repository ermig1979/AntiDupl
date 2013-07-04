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
using System.Threading;

namespace AntiDupl.NET
{
    public class ProgressForm : Form
    {
        public enum Type
        {
            ApplyAction,
            RenameCurrent,

            RefreshResults,
            Undo,
            Redo,

            LoadResults,
            SaveResults,

            ClearResults,
            ClearTemporary,

            RefreshImages,
            LoadImages,
            SaveImages,
        }
        private Type m_type;
        
        private CoreDll.LocalActionType m_action;
        private CoreDll.TargetType m_target;

        private CoreDll.RenameCurrentType m_renameCurrentType;
        private string m_newFileName;

        private enum State
        {
            Start,
            Work,
            Stopped,
            Finish
        }
        private State m_state = State.Start;

        private CoreLib m_core;
        private Options m_options;
        private CoreOptions m_coreOptions;
        private MainSplitContainer m_mainSplitContainer;
        private System.Windows.Forms.Timer m_timer;
        private DateTime m_startDateTime;
        private bool m_updateResults = true;

        private Button m_cancelButton;
        private ProgressPanel m_progressPanel;

        public ProgressForm(Type type, CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            if (type == Type.ApplyAction)
                throw new Exception("Wrong using of ProgressForm type!");
            m_type = type;
            Initialize(core, options, coreOptions, mainSplitContainer);
        }

        public ProgressForm(CoreDll.LocalActionType action, CoreDll.TargetType target, CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            m_type = Type.ApplyAction;
            m_action = action;
            m_target = target;
            Initialize(core, options, coreOptions, mainSplitContainer);
        }

        public ProgressForm(CoreDll.RenameCurrentType renameCurrentType, string newFileName, CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            m_type = Type.RenameCurrent; 
            m_renameCurrentType = renameCurrentType;
            m_newFileName = newFileName;
            Initialize(core, options, coreOptions, mainSplitContainer);
        }

        private void Initialize(CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_coreOptions = coreOptions;
            m_mainSplitContainer = mainSplitContainer;
            InitializeComponent();
            UpdateStrings();
        }

        private void InitializeComponent()
        {
            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2);
            mainTableLayoutPanel.Padding = new Padding(1, 5, 1, 0);
            Controls.Add(mainTableLayoutPanel);

            m_progressPanel = new ProgressPanel();
            mainTableLayoutPanel.Controls.Add(m_progressPanel, 0, 0);

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(3, 1);
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 50F));
            mainTableLayoutPanel.Controls.Add(buttonsTableLayoutPanel, 0, 1);

            m_cancelButton = new Button();
            m_cancelButton.Click += new EventHandler(OnCancelButtonClick);
            m_cancelButton.AutoSize = true;
            buttonsTableLayoutPanel.Controls.Add(m_cancelButton, 1, 0);

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
                  m_cancelButton.Height + m_cancelButton.Padding.Vertical + mainTableLayoutPanel.Margin.Vertical +
                  mainTableLayoutPanel.Padding.Vertical;
                ClientSize = new System.Drawing.Size(width, height);
            }

            m_timer = new System.Windows.Forms.Timer();
            m_timer.Interval = 100;
            m_timer.Tick += new EventHandler(TimerCallback);
            m_timer.Start();

            FormClosed += new FormClosedEventHandler(OnFormClosed);
            KeyDown += new KeyEventHandler(OnKeyDown);
        }

        public void Execute()
        {
            m_state = State.Start;
            Thread coreThread = new Thread(CoreThreadTask);
            coreThread.Start();
            if (m_type >= Type.ApplyAction && m_type <= Type.RefreshResults)
            {
                m_cancelButton.Enabled = true;
            }
            else
            {
                m_cancelButton.Enabled = false;
            }

            Thread.Sleep(200);

            if (m_state != State.Finish)
            {
                ShowDialog();
            }
            else
            {
                OnFormClosed(this, null);
            }
        }

        private void CoreThreadTask()
        {
            m_startDateTime = DateTime.Now;
            m_state = State.Work;
            m_coreOptions.Set(m_core, m_options.onePath);
            switch (m_type)
            {
                case Type.ApplyAction:
                    {
                        m_updateResults = m_core.ApplyToResult(m_action, m_target);
                        m_type = Type.ClearTemporary;
                        m_core.Clear(CoreDll.FileType.Temporary);
                        break;
                    }
                case Type.RenameCurrent:
                    {
                        m_updateResults = m_core.RenameCurrent(m_renameCurrentType, m_newFileName);
                        m_type = Type.ClearTemporary;
                        m_core.Clear(CoreDll.FileType.Temporary);
                        break;
                    }
                case Type.RefreshResults:
                    {
                        m_updateResults = m_core.ApplyToResult(CoreDll.GlobalActionType.Refresh);
                        m_type = Type.ClearTemporary;
                        m_core.Clear(CoreDll.FileType.Temporary);
                        break;
                    }
               case Type.Undo:
                    {
                        m_updateResults = m_core.ApplyToResult(CoreDll.GlobalActionType.Undo);
                        break;
                    }
               case Type.Redo:
                    {
                        m_updateResults = m_core.ApplyToResult(CoreDll.GlobalActionType.Redo);
                        break;
                    }
               case Type.LoadResults:
                    {
                        m_type = Type.ClearResults;
                        m_core.Clear(CoreDll.FileType.Result);
                        m_type = Type.ClearTemporary;
                        m_core.Clear(CoreDll.FileType.Temporary);
                        m_type = Type.LoadResults;
                        m_core.Load(CoreDll.FileType.Result, m_options.GetResultsFileName(), m_options.checkResultsAtLoading);
                        m_updateResults = true;
                        break;
                    }
               case Type.SaveResults:
                    {
                        m_core.Save(CoreDll.FileType.Result, m_options.GetResultsFileName());
                        m_updateResults = false;
                        break;
                    }
               case Type.RefreshImages:
                    {
                        m_type = Type.LoadImages;
                        m_core.Load(CoreDll.FileType.ImageDataBase, m_coreOptions.GetImageDataBasePath(), true);
                        if (m_state == State.Work)
                        {
                            m_type = Type.SaveImages;
                            m_core.Save(CoreDll.FileType.ImageDataBase, m_coreOptions.GetImageDataBasePath());
                        }
                        m_updateResults = false;
                        break;
                    }
               default:
                    throw new Exception("Unknown Action!!!");
            }
            m_state = State.Finish;
        }

        private void OnFormClosed(object sender, FormClosedEventArgs e)
        {
            m_timer.Stop();
            if (m_updateResults)
            {
                m_mainSplitContainer.UpdateResults();
            }
        }

        private void OnCancelButtonClick(Object obj, EventArgs eventArgs)
        {
            if (m_state == State.Work)
            {
                m_core.Stop();
                m_state = State.Stopped;
                m_cancelButton.Enabled = false;
            }
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_cancelButton.Text = s.CancelButton_Text;
        }

        void TimerCallback(Object obj, EventArgs eventArgs)
        {
            if (m_state == State.Finish)
            {
                Close();
            }
            else
            {
                StringBuilder builder = new StringBuilder();
                Strings s = Resources.Strings.Current;
                switch (m_state)
                {
                    case State.Start:
                    case State.Work:
                        {
                            switch (m_type)
                            {
                                case Type.ApplyAction:
                                    switch (m_action)
                                    {
                                        case CoreDll.LocalActionType.DeleteDefect:
                                            builder.Append(s.ProgressForm_DeleteDefect);
                                            break;
                                        case CoreDll.LocalActionType.DeleteFirst:
                                            builder.Append(s.ProgressForm_DeleteFirst);
                                            break;
                                        case CoreDll.LocalActionType.DeleteSecond:
                                            builder.Append(s.ProgressForm_DeleteSecond);
                                            break;
                                        case CoreDll.LocalActionType.DeleteBoth:
                                            builder.Append(s.ProgressForm_DeleteBoth);
                                            break;
                                        case CoreDll.LocalActionType.PerformHint:
                                            builder.Append(s.ProgressForm_PerformHint);
                                            break;
                                        case CoreDll.LocalActionType.Mistake:
                                            builder.Append(s.ProgressForm_Mistake);
                                            break;
                                        case CoreDll.LocalActionType.RenameFirstToSecond:
                                        case CoreDll.LocalActionType.RenameSecondToFirst:
                                            builder.Append(s.ProgressForm_RenameCurrent);
                                            break;
                                    }
                                    m_cancelButton.Enabled = true;
                                    break;
                               case Type.RenameCurrent:
                                    builder.Append(s.ProgressForm_RenameCurrent);
                                    m_cancelButton.Enabled = false;
                                    break;
                               case Type.RefreshResults:
                                    builder.Append(s.ProgressForm_RefreshResults);
                                    m_cancelButton.Enabled = true;
                                    break;
                                case Type.Undo:
                                    builder.Append(s.ProgressForm_Undo);
                                    m_cancelButton.Enabled = false;
                                    break;
                                case Type.Redo:
                                    builder.Append(s.ProgressForm_Redo);
                                    m_cancelButton.Enabled = false;
                                    break;
                                case Type.LoadResults:
                                    builder.Append(s.StartFinishForm_LoadResults_Text);
                                    m_cancelButton.Enabled = true;
                                    break;
                                case Type.SaveResults:
                                    builder.Append(s.StartFinishForm_SaveResults_Text);
                                    m_cancelButton.Enabled = true;
                                    break;
                                case Type.ClearResults:
                                    builder.Append(s.StartFinishForm_ClearTemporary_Text);
                                    m_cancelButton.Enabled = false;
                                    break;
                                case Type.ClearTemporary:
                                    builder.Append(s.StartFinishForm_ClearTemporary_Text);
                                    m_cancelButton.Enabled = true;
                                    break;
                                case Type.RefreshImages:
                                    builder.Append(s.StartFinishForm_LoadImages_Text);
                                    m_cancelButton.Enabled = false;
                                    break;
                                case Type.LoadImages:
                                    builder.Append(s.StartFinishForm_LoadImages_Text);
                                    m_cancelButton.Enabled = true;
                                    break;
                                case Type.SaveImages:
                                    builder.Append(s.StartFinishForm_SaveImages_Text);
                                    m_cancelButton.Enabled = false;
                                    break;
                            }

                            CoreStatus status = m_core.StatusGet(CoreDll.ThreadType.Main, 0);
                            if (status != null)
                            {
                                double progress = status.total > 0 ? ((double)status.current) / status.total : 0;
                                builder.AppendFormat(" ({0})...", ProgressUtils.GetProgressString(progress, m_startDateTime));
                                m_progressPanel.UpdateStatus(status.total, status.current, status.current, status.path);
                            }
                            else
                            {
                                m_progressPanel.UpdateStatus(0, 0, 0, "");
                            }
                        }
                        break;
                    case State.Stopped:
                        {
                            builder.Append(s.CancelButton_Text);
                            builder.Append("...");
                        }
                        break;
                }
                Text = builder.ToString();
            }
        }

        private void OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyData == Keys.Escape)
            {
                if (m_state == State.Work)
                {
                    m_core.Stop();
                    m_state = State.Stopped;
                    m_cancelButton.Enabled = false;
                }
            }
            else
            {
                e.Handled = true;
            }
        }
    }
}
