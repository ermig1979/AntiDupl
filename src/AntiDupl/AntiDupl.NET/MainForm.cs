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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

namespace AntiDupl.NET
{
    public class MainForm : Form
    {
        public const int MIN_HEIGHT = 520;
        public const int MIN_WIDTH = 660;

        private Options m_options;
        private CoreLib m_core;

        private MainSplitContainer m_mainSplitContainer;
        private MainMenu m_mainMenu;
        private MainToolStrip m_mainToolStrip;
        private MainStatusStrip m_mainStatusStrip;

        public MainForm()
        {
            m_core = new CoreLib();
            m_options = Options.Load(m_core);
            Resources.Strings.SetCurrent(m_options.Language);

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.ExecuteStart();

            InitializeComponents();
        }

        private void InitializeComponents()
        {
            m_mainSplitContainer = new MainSplitContainer(m_core, m_options, this);
            m_mainSplitContainer.Dock = DockStyle.Fill;
            m_mainSplitContainer.Location = new System.Drawing.Point(0, 0);

            m_mainMenu = new MainMenu(m_core, m_options, this, m_mainSplitContainer);

            m_mainToolStrip = new MainToolStrip(m_core, m_options, m_mainMenu, this, m_mainSplitContainer);

            m_mainStatusStrip = new MainStatusStrip(m_mainSplitContainer, m_options);

            Text = Application.ProductName;
            Size = new Size(MIN_WIDTH, MIN_HEIGHT);
            MinimumSize = new Size(MIN_WIDTH, MIN_HEIGHT);
            Icon = Resources.Icons.Get(Icon.Size);

            Controls.Add(m_mainSplitContainer);
            Controls.Add(m_mainToolStrip);
            Controls.Add(m_mainStatusStrip);
            Controls.Add(m_mainMenu);

            FormClosed += new FormClosedEventHandler(OnFormClosed);
            SizeChanged += new EventHandler(OnSizeChanged);
            Shown += new EventHandler(OnFormShown);
        }

        private void OnFormClosed(object sender, FormClosedEventArgs e)
        {
            m_mainSplitContainer.ClearResults();
            GetSavedViewOptions();
            m_options.Save();

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.ExecuteFinish();
            m_core.Dispose();
        }

        private void SetLoadedViewOptions()
        {
            Size = new Size(
              Math.Min(Math.Max(m_options.mainFormOptions.width, MIN_WIDTH), Screen.PrimaryScreen.WorkingArea.Width),
              Math.Min(Math.Max(m_options.mainFormOptions.height, MIN_HEIGHT), Screen.PrimaryScreen.WorkingArea.Height));

            WindowState = (m_options.mainFormOptions.maximized ? FormWindowState.Maximized : FormWindowState.Normal);
        }

        private void GetSavedViewOptions()
        {
            m_options.mainFormOptions.maximized = (WindowState == FormWindowState.Maximized);
        }

        private void OnSizeChanged(object sender, System.EventArgs e)
        {
            if (WindowState == FormWindowState.Normal)
            {
                m_options.mainFormOptions.width = Size.Width;
                m_options.mainFormOptions.height = Size.Height;
            }
        }

        private void OnFormShown(object sender, EventArgs e)
        {
            SetLoadedViewOptions();
            m_mainSplitContainer.SetViewMode(m_options.resultsOptions.viewMode);
        } 
    }
}
