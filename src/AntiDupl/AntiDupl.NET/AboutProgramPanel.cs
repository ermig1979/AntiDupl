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

namespace AntiDupl.NET
{
    public class AboutProgramPanel : Panel
    {
        private PictureBox m_logotypePictureBox;
        private Label m_programNameLabel;
        private Label m_versionLabel;
        private Label m_revisionLabel;
        private Label m_timeLabel;
        private Label m_placeLabel;
        private Label m_authorLabel;
        private LinkLabel m_siteLinkLabel;

        public AboutProgramPanel()
        {
            InitializeComponent();
            UpdateStrings();
        }

        private void InitializeComponent()
        {
            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(2, 1);
            mainTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            mainTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 70F));
            Controls.Add(mainTableLayoutPanel);

            m_logotypePictureBox = new PictureBox();
            m_logotypePictureBox.Location = new System.Drawing.Point(0, 0);
            m_logotypePictureBox.Dock = DockStyle.Fill;
            m_logotypePictureBox.Image = Resources.Images.Get("AboutProgramImage");
            mainTableLayoutPanel.Controls.Add(m_logotypePictureBox, 0, 0);

            TableLayoutPanel labelsTableLayoutPanel = InitFactory.Layout.Create(1, 7);
            mainTableLayoutPanel.Controls.Add(labelsTableLayoutPanel, 1, 0);

            m_programNameLabel = InitFactory.Label.Create(15F);
            labelsTableLayoutPanel.Controls.Add(m_programNameLabel, 0, 0);

            m_versionLabel = InitFactory.Label.Create(10F);
            labelsTableLayoutPanel.Controls.Add(m_versionLabel, 0, 1);

            m_revisionLabel = InitFactory.Label.Create(10F);
            labelsTableLayoutPanel.Controls.Add(m_revisionLabel, 0, 2);

            m_timeLabel = InitFactory.Label.Create(10F);
            labelsTableLayoutPanel.Controls.Add(m_timeLabel, 0, 3);

            m_placeLabel = InitFactory.Label.Create(10F);
            labelsTableLayoutPanel.Controls.Add(m_placeLabel, 0, 4);

            m_authorLabel = InitFactory.Label.Create(10F);
            labelsTableLayoutPanel.Controls.Add(m_authorLabel, 0, 5);

            m_siteLinkLabel = new LinkLabel();
            m_siteLinkLabel.AutoSize = true;
            m_siteLinkLabel.Font = InitFactory.Font.Create(10F);
            m_siteLinkLabel.Links.Add(new LinkLabel.Link());
            m_siteLinkLabel.LinkClicked += new LinkLabelLinkClickedEventHandler(OnSiteLinkLabelLinkClicked);
            labelsTableLayoutPanel.Controls.Add(m_siteLinkLabel, 0, 6);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_programNameLabel.Text = Application.ProductName;
            {
                StringBuilder builder = new StringBuilder();
                builder.Append(s.AboutProgramPanel_VersionLabel_Text);
                builder.Append(Application.ProductVersion.ToString());
                m_versionLabel.Text = builder.ToString();
            }
            {
                StringBuilder builder = new StringBuilder();
                builder.Append(s.AboutProgramPanel_RevisionLabel_Text);
                builder.Append(Version.GetRevisionString());
                m_revisionLabel.Text = builder.ToString();
            }
            m_timeLabel.Text = s.AboutProgramPanel_TimeLabel_Text;
            m_placeLabel.Text = s.AboutProgramPanel_PlaceLabel_Text;
            m_authorLabel.Text = s.AboutProgramPanel_AuthorLabel_Text;
            m_siteLinkLabel.Text = Resources.WebLinks.SiteRoot;
        }

        private void OnSiteLinkLabelLinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            m_siteLinkLabel.Links[0].Visited = true;
            System.Diagnostics.Process.Start(Resources.WebLinks.SiteCurrent);
        }
    }
}
