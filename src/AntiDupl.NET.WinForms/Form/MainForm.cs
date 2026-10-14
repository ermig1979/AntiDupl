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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Xml;
using System.Xml.Serialization;
using System.IO;

using AntiDupl.NET.Core;

namespace AntiDupl.NET.WinForms
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

        private CommandLine m_commandLine;

        public MainForm(Options options, CommandLine commandLine)
        {
            m_commandLine = commandLine;
            m_core = new CoreLib(Resources.UserPath);
            m_options = options;
            if (m_options.loadProfileOnLoading)
                m_coreOptions = CoreOptions.Load(m_options.coreOptionsFileName, m_core, m_options.onePath);
            else
            {
                m_options.coreOptionsFileName = Options.GetDefaultCoreOptionsFileName();
                m_coreOptions = new CoreOptions(m_core);
            }
            Resources.Strings.SetCurrent(m_options.Language);
            if (m_options.LoadProblem != null)
                m_options.LoadProblem.Show(null);
            if (m_coreOptions.LoadProblem != null)
                m_coreOptions.LoadProblem.Show(null);
            UseCommandLineFolders(commandLine);

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.UseResultsFile = !m_coreOptions.IsOneOffRun;
            startFinishForm.ExecuteStart();

            InitializeComponents();
            ThemeColors.Apply(this);
        }

        // -search replaces the profile's search folders and -ignore adds to
        // its ignore list, for this run only (CoreOptions.profileSearchPath,
        // profileIgnorePath); the profile's results file is neither read nor
        // written, so a one-off run leaves the usual one as it was.
        private void UseCommandLineFolders(CommandLine commandLine)
        {
            if (commandLine.SearchPaths != null)
            {
                CorePathWithSubFolder[] found = ExistingFolders(commandLine.SearchPaths, Resources.Strings.Current.MainForm_SearchFoldersNotFound);
                if (found.Length > 0)
                {
                    m_coreOptions.profileSearchPath = m_coreOptions.searchPath;
                    m_coreOptions.searchPath = found;
                }
            }
            if (commandLine.IgnorePaths != null)
            {
                CorePathWithSubFolder[] found = ExistingFolders(commandLine.IgnorePaths, Resources.Strings.Current.MainForm_IgnoreFoldersNotFound);
                if (found.Length > 0)
                {
                    m_coreOptions.profileIgnorePath = m_coreOptions.ignorePath;
                    List<CorePathWithSubFolder> ignore = new List<CorePathWithSubFolder>(m_coreOptions.ignorePath);
                    ignore.AddRange(found);
                    m_coreOptions.ignorePath = ignore.ToArray();
                }
            }
        }

        // The folders that exist, with their subfolders; the others are
        // reported with the given message and left out.
        private static CorePathWithSubFolder[] ExistingFolders(string[] paths, string missingMessage)
        {
            List<CorePathWithSubFolder> found = new List<CorePathWithSubFolder>();
            List<string> missing = new List<string>();
            foreach (string path in paths)
            {
                if (Directory.Exists(path))
                    found.Add(new CorePathWithSubFolder(Path.GetFullPath(path), true));
                else
                    missing.Add(path);
            }
            if (missing.Count > 0)
                MessageBox.Show(string.Format(missingMessage, string.Join("\n", missing)),
                    Resources.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            return found.ToArray();
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
            if (m_options.saveProfileOnClosing)
                m_coreOptions.Save(m_options.coreOptionsFileName);

            m_mainSplitContainer.ClearResults();
            GetSavedViewOptions();
            m_options.Save();

            StartFinishForm startFinishForm = new StartFinishForm(m_core, m_options);
            startFinishForm.UseResultsFile = !m_coreOptions.IsOneOffRun;
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
            // After the window has been drawn, so the search dialog opens over it.
            if (m_commandLine.StartSearch)
                BeginInvoke(new Action(() => m_mainMenu.StartSearchAction(this, EventArgs.Empty)));
        }

        public void UpdateCaption()
        {
            Text = string.Format("{0} - {1}", Resources.ProductName, Path.GetFileNameWithoutExtension(m_options.coreOptionsFileName));
            if (m_coreOptions.profileSearchPath != null)
            {
                // A one-off search names its folders, so it isn't taken for the profile's.
                List<string> names = new List<string>();
                foreach (CorePathWithSubFolder path in m_coreOptions.searchPath)
                    names.Add(Path.GetFileName(path.path.TrimEnd('\\')));
                Text += " - " + string.Join(", ", names);
            }
        }
    }
}
