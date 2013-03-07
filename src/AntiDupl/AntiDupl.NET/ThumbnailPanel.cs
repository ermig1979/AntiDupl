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
using System.Drawing;
using System.IO;

namespace AntiDupl.NET
{
    public class ThumbnailPanel :  RaisedPanel
    {
        private const int IBW = 1;//Internal border width
        private const int EBW = 2;//External border width

        private CoreLib m_core;
        private CoreGroup m_group;
        private int m_index;
        private AntiDupl.NET.Options m_options;
        private ThumbnailGroupPanel m_thumbnailGroupPanel;

        private TableLayoutPanel m_mainLayout;
        private TableLayoutPanel m_controlLayout;
        private CheckBox m_checkBox;
        private PictureBox m_pictureBox;
        private TableLayoutPanel m_infoLayout;
        private Label m_fileSizeLabel;
        private Label m_imageSizeLabel;
        private Label m_imageTypeLabel;

        private Label m_fileNameLabel;
        
        public Bitmap Thumbnail
        {
            get
            {
                return (Bitmap)m_pictureBox.Image;
            }
            set
            {
                if (InvokeRequired)
                {
                    BeginInvoke(new MethodInvoker(() =>
                    {
                        m_pictureBox.Image = value;
                    }));
                }
                else
                {
                    m_pictureBox.Image = value;
                }
            }
        }

        public CoreImageInfo ImageInfo { get { return m_group.images[m_index]; } }

        public ThumbnailPanel(CoreLib core, AntiDupl.NET.Options options, CoreGroup group, int index, ThumbnailGroupPanel thumbnailGroupPanel)
        {
            m_core = core;
            m_options = options;
            m_group = group;
            m_index = index;
            m_thumbnailGroupPanel = thumbnailGroupPanel;
            InitializeComponents();
            SetImageInfo();
        }

        private void InitializeComponents()
        {
            DoubleBuffered = true;
            BackColor = Color.Transparent;

            m_mainLayout = InitFactory.Layout.Create(1, 4, 0, 0);
            m_mainLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            m_mainLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            m_mainLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            m_controlLayout = InitFactory.Layout.Create(1, 1, 0, 0);
            m_controlLayout.Height = 16;
            m_checkBox = new CheckBox();
            m_checkBox.Location = new Point(0, 0);
            m_checkBox.Margin = new Padding(0);
            m_checkBox.Padding = new Padding(0);
            m_checkBox.Height = 16;
            m_checkBox.Click += new EventHandler(OnCheckBoxClick);


            m_controlLayout.Controls.Add(m_checkBox, 0, 0);


            m_pictureBox = new PictureBox();
            m_pictureBox.Location = new Point(0, 0);
            m_pictureBox.ClientSize = m_options.resultsOptions.thumbnailSizeMax;
            m_pictureBox.SizeMode = PictureBoxSizeMode.Zoom; 
            m_pictureBox.BorderStyle = BorderStyle.Fixed3D;
            m_pictureBox.Image = null;
            m_pictureBox.Padding = new Padding(0);
            m_pictureBox.Margin = new Padding(0);
            m_pictureBox.Location = new Point(Padding.Left, Padding.Top);
            m_pictureBox.BackColor = Color.Transparent;
            m_pictureBox.Click += new EventHandler(OnClick);

            m_fileSizeLabel = CreateLabel();

            m_imageSizeLabel = CreateLabel();

            m_imageTypeLabel = CreateLabel();

            m_fileNameLabel = CreateLabel();

            m_infoLayout = InitFactory.Layout.Create(3, 1, 0, 0);
            m_infoLayout.Height = 16;

            m_infoLayout.Controls.Add(m_fileSizeLabel, 0, 0);
            m_infoLayout.Controls.Add(m_imageSizeLabel, 1, 0);
            m_infoLayout.Controls.Add(m_imageTypeLabel, 2, 0);

            m_mainLayout.Controls.Add(m_controlLayout, 0, 0);
            m_mainLayout.Controls.Add(m_pictureBox, 0, 1);
            m_mainLayout.Controls.Add(m_infoLayout, 0, 2);
            m_mainLayout.Controls.Add(m_fileNameLabel, 0, 3);

            Controls.Add(m_mainLayout);

            SetSize();
        }

        private Label CreateLabel()
        {
            Label label = new Label();
            label.Location = new Point(0, 0);
            label.Padding = new Padding(0, 0, 0, 0);
            label.Margin = new Padding(0, 0, 0, 0);
            label.TextAlign = ContentAlignment.TopCenter;
            label.AutoSize = false;
            label.BorderStyle = BorderStyle.Fixed3D;
            label.Height = label.Font.Height + 2;
            label.FlatStyle = FlatStyle.System;
            label.Text = "0";
            label.Click += new EventHandler(OnClick);
            return label;
        }

        private void SetSize()
        {
            m_fileSizeLabel.Width = 40;

            m_imageSizeLabel.Width = 60;

            m_imageTypeLabel.Width = 28;
            
            m_fileNameLabel.Width = 128;
            m_fileNameLabel.Location = new Point(0, 0);

            Font font = m_fileSizeLabel.Font;
            int width = m_pictureBox.Width + Padding.Horizontal;
            int height = m_pictureBox.Height + (font.Height + m_fileSizeLabel.Margin.Vertical + m_fileSizeLabel.Padding.Vertical +
                m_infoLayout.Padding.Vertical)*3 + m_mainLayout.Padding.Vertical + m_mainLayout.Margin.Vertical + Padding.Vertical + 8;
            ClientSize = new Size(width, height);
        }

        private void SetImageInfo()
        {
            CoreImageInfo info = m_group.images[m_index];

            m_fileSizeLabel.Text = info.GetFileSizeString();
            m_imageSizeLabel.Text = string.Format("{0}×{1}", info.width, info.height);
            m_imageTypeLabel.Text = (info.type == CoreDll.ImageType.None ? "   " : info.GetImageTypeString());
            m_fileNameLabel.Text = Path.GetFileNameWithoutExtension(info.path);

            bool[] selected = m_core.GetSelection(m_group.id, (uint)m_index, 1);
            m_checkBox.Checked = selected[0];
        }

        private void OnCheckBoxClick(object sender, EventArgs e)
        {
            if (m_checkBox.Checked)
            {
                m_core.SetSelection(m_group.id, m_index, CoreDll.SelectionType.SelectCurrent);
            }
            else
            {
                m_core.SetSelection(m_group.id, m_index, CoreDll.SelectionType.UnselectCurrent);
            }
            m_thumbnailGroupPanel.Table.ChangeCurrentThumbnail(m_group, m_index);
        }

        private void OnClick(object sender, EventArgs e)
        {
            m_thumbnailGroupPanel.Table.ChangeCurrentThumbnail(m_group, m_index);
        }
    }
}
