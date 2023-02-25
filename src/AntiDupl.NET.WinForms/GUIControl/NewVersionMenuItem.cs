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
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Drawing;
using System.IO;
using System.Threading;

namespace AntiDupl.NET
{
    public class NewVersionMenuItem : ToolStripMenuItem
    {
        bool m_downloadingFinished = false;

        private Version m_localVersion;
        private Version m_onlineVersion;

        private Options m_options;

        private System.Windows.Forms.Timer m_timer;

        public NewVersionMenuItem(Options options)
        {
            m_options = options;
            InitializeComponents();
            if(m_options.checkingForUpdates)
            {
                InitializeVersions();
            }
            else
            {
                m_downloadingFinished = true;
            }
        }

        private void InitializeComponents()
        {
            Visible = false;
            ForeColor = Color.Red;
            Click += new EventHandler(OnClick);
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);

            m_timer = new System.Windows.Forms.Timer();
            m_timer.Interval = 1000;
            m_timer.Tick += new EventHandler(TimerCallback);
            m_timer.Start();
        }

        private void OnClick(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start(Resources.WebLinks.GithubComAntiduplCurrent);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.MainMenu_NewVersionMenuItem_Text;
            if (m_onlineVersion != null)
            {
                ToolTipText = String.Format(s.MainMenu_NewVersionMenuItem_Tooltip, m_onlineVersion.ToString());
            }
        }

        void TimerCallback(Object obj, EventArgs eventArgs)
        {
            if (m_downloadingFinished)
            {
                if (m_onlineVersion != null)
                {
                    if (Version.Compare(m_localVersion, m_onlineVersion) < 0)
                    {
                        Visible = true;
                        UpdateStrings();
                    }
                }
                m_timer.Stop();
            }
        }

        private void InitializeVersions()
        {
            m_localVersion = new Version();
            //m_localVersion.Save("version.xml");

            Thread thread = new Thread(OnlineVersionDownloadThreadTask);
            thread.Start();
        }

        private void OnlineVersionDownloadThreadTask()
        {
            try
            {
                WebClient webClient = new WebClient();
                byte[] buffer = webClient.DownloadData(Resources.WebLinks.Version);
                m_onlineVersion = Version.LoadXml(new MemoryStream(buffer));
            }
            catch
            {
                m_onlineVersion = null;
            }
            m_downloadingFinished = true;
        }
    }
}
