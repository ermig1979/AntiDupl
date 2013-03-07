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
using System.Diagnostics;
using System.IO;
using Microsoft.Win32;
using System.ComponentModel;

namespace AntiDupl.NET
{
    public class ImagePreviewContextMenu : ContextMenuStrip
    {
        private CoreLib m_core;
        private Options m_options;
        private ImagePreviewPanel m_imagePreviewPanel;
        private ResultsListView m_resultsListView;

        private ToolStripMenuItem m_copyPathItem;
        private ToolStripMenuItem m_openImageItem;
        private ToolStripMenuItem m_openFolderItem;
        private ToolStripMenuItem m_renameImageItem;
        private ToolStripMenuItem m_moveImageToNeighbourItem;
        
        public ImagePreviewContextMenu(CoreLib core, Options options, ImagePreviewPanel imagePreviewPanel, ResultsListView resultsListView)
        {
            m_core = core;
            m_options = options;
            m_imagePreviewPanel = imagePreviewPanel;
            m_resultsListView = resultsListView;
            InitializeComponents();
            UpdateStrings();
            Resources.Strings.OnCurrentChange += new Resources.Strings.CurrentChangeHandler(UpdateStrings);
            Opening += new CancelEventHandler(OnOpening);
        }

        private void InitializeComponents()
        {
            RenderMode = ToolStripRenderMode.System;

            m_copyPathItem = InitFactory.MenuItem.Create(null, null, CopyPath);
            m_openImageItem = InitFactory.MenuItem.Create(null, null, OpenImage);
            m_openFolderItem = InitFactory.MenuItem.Create(null, null, OpenFolder);
            m_renameImageItem = InitFactory.MenuItem.Create(null, null, m_imagePreviewPanel.RenameImage);
            m_moveImageToNeighbourItem = InitFactory.MenuItem.Create(null, null, MoveImageToNeighbour);
            
            Items.Add(new ToolStripSeparator());
        }
        
        private void OnOpening(object sender, EventArgs e)
        {
            Items.Clear();
            
            Items.Add(m_copyPathItem);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_openImageItem);
            Items.Add(m_openFolderItem);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_renameImageItem);
            if (MoveImageToNeighbourEnable())
            {
                Items.Add(m_moveImageToNeighbourItem);
            }
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_copyPathItem.Text = s.ImagePreviewContextMenu_CopyPathItem_Text;
            m_openImageItem.Text = s.ImagePreviewContextMenu_OpenImageItem_Text;
            m_openFolderItem.Text = s.ImagePreviewContextMenu_OpenFolderItem_Text;
            m_renameImageItem.Text = s.ImagePreviewContextMenu_RenameImageItem_Text;
            m_moveImageToNeighbourItem.Text = s.ImagePreviewContextMenu_MoveImageToNeighbourItem_Text;
        }

        private void OpenImage(object sender, EventArgs e)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = m_imagePreviewPanel.CurrentImageInfo.path;
            try
            {
                Process.Start(startInfo);
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }

        private void OpenFolder(object sender, EventArgs e)
        {
            FolderOpener.OpenContainingFolder(m_imagePreviewPanel.CurrentImageInfo);
        }

        private void CopyPath(object sender, EventArgs e)
        {
            Clipboard.SetText(m_imagePreviewPanel.CurrentImageInfo.path);
        }

        private void MoveImageToNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.RenameCurrent(m_imagePreviewPanel.RenameCurrentType, GetNewImagePath());
        }
        
        private bool MoveImageToNeighbourEnable()
        {
            if (m_imagePreviewPanel.NeighbourImageInfo != null)
            {
                FileInfo fileInfo = new FileInfo(GetNewImagePath());
                return !fileInfo.Exists;
            }
            return false;
        }
        
        private string GetNewImagePath()
        {
            StringBuilder path = new StringBuilder(m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString());
            path.Append("\\");
            path.Append(Path.GetFileName(m_imagePreviewPanel.CurrentImageInfo.path));
            return path.ToString();
        }
    }
}
