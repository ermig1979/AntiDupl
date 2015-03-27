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
using System.Xml;
using System.Xml.Serialization;
using System.IO;

namespace AntiDupl.NET
{
    public class MainForm : Form
    {
        public const int MIN_HEIGHT = 520;
        public const int MIN_WIDTH = 790;

        private Options m_options;
        private CoreLib m_core;
        private CoreOptions m_coreOptions;

        private MainSplitContainer m_mainSplitContainer;
        private MainMenu m_mainMenu;
        private MainToolStrip m_mainToolStrip;
        private MainStatusStrip m_mainStatusStrip;

        public MainForm()
        {
            m_core = new CoreLib();
            m_options = Options.Load();
            m_coreOptions = CoreOptions.Load(m_options.coreOptionsFileName, m_core, m_options.onePath);
            Resources.Strings.SetCurrent(m_options.Language);

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.ExecuteStart();

            InitializeComponents();
        }

        private void InitializeComponents()
        {
            m_mainSplitContainer = new MainSplitContainer(m_core, m_options, m_coreOptions, this);
            m_mainSplitContainer.Dock = DockStyle.Fill;
            m_mainSplitContainer.Location = new System.Drawing.Point(0, 0);

            m_mainMenu = new MainMenu(m_core, m_options, m_coreOptions, this, m_mainSplitContainer);

            m_mainToolStrip = new MainToolStrip(m_core, m_options, m_coreOptions, m_mainMenu, this, m_mainSplitContainer);

            m_mainStatusStrip = new MainStatusStrip(m_mainSplitContainer, m_options);

            Size = new Size(MIN_WIDTH, MIN_HEIGHT);
            MinimumSize = new Size(MIN_WIDTH, MIN_HEIGHT);
            Icon = Resources.Icons.Get(Icon.Size);

            Controls.Add(m_mainSplitContainer);
            Controls.Add(m_mainToolStrip);
            Controls.Add(m_mainStatusStrip);
            Controls.Add(m_mainMenu);

            FormClosed += new FormClosedEventHandler(OnFormClosed);
            Shown += new EventHandler(OnFormShown);

            UpdateCaption();
        }

        private void OnFormClosed(object sender, FormClosedEventArgs e)
        {
            m_coreOptions.Save(m_options.coreOptionsFileName);

            m_mainSplitContainer.ClearResults();
            GetSavedViewOptions();
            m_options.Save();

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.ExecuteFinish();
            m_core.Dispose();
        }

        private void SetLoadedViewOptions()
        {
            Point loc = m_options.mainFormOptions.location;
            Size size = m_options.mainFormOptions.size;

            Size sizeMin = new Size(MIN_WIDTH, MIN_HEIGHT);
            Size sizeMax = Screen.PrimaryScreen.WorkingArea.Size;

            Point locMin = Screen.PrimaryScreen.WorkingArea.Location;
            Point locMax = locMin + Screen.PrimaryScreen.WorkingArea.Size - sizeMin;

            loc = new Point(Restrict(loc.X, locMin.X, locMax.X), Restrict(loc.Y, locMin.Y, locMax.Y));
            size = new Size(Restrict(size.Width, sizeMin.Width, sizeMax.Width - loc.X),
                Restrict(size.Height, sizeMin.Height, sizeMax.Height - loc.Y));

            Location = loc;
            Size = size;
            WindowState = (m_options.mainFormOptions.maximized ? FormWindowState.Maximized : FormWindowState.Normal);
        }

        private int Restrict(int value, int min, int max)
        {
            return Math.Min(Math.Max(value, min), max);
        }

        private void GetSavedViewOptions()
        {
            m_options.mainFormOptions.maximized = (WindowState == FormWindowState.Maximized);
            if (WindowState == FormWindowState.Normal)
            {
                m_options.mainFormOptions.size = Size;
                m_options.mainFormOptions.location = Location;
            }
        }

        private void OnFormShown(object sender, EventArgs e)
        {
            SetLoadedViewOptions();
            m_mainSplitContainer.SetViewMode(m_options.resultsOptions.viewMode);
        }

        public void UpdateCaption()
        {
            Text = string.Format("{0} - {1}", Application.ProductName, Path.GetFileNameWithoutExtension(m_options.coreOptionsFileName));
        }
    }
}
