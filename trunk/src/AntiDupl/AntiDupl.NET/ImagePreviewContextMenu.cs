/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2013-2015 Borisov Dmitry.
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
        private ToolStripMenuItem m_copyFileNameItem;
        private ToolStripMenuItem m_openImageItem;
        private ToolStripMenuItem m_openFolderItem;
        private ToolStripMenuItem m_renameImageItem;
        private ToolStripMenuItem m_renameImageLikeNeighbourItem;
        private ToolStripMenuItem m_moveImageToNeighbourItem;
        private ToolStripMenuItem m_moveImageAndRenameToNeighbourItem;
        private ToolStripMenuItem m_moveGroupToNeighbourItem;
        private ToolStripMenuItem m_renameGroupAsNeighbourItem;


        
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
            m_copyFileNameItem = InitFactory.MenuItem.Create(null, null, new EventHandler(this.CopyFileName));
            m_openImageItem = InitFactory.MenuItem.Create(null, null, OpenImage);
            m_openFolderItem = InitFactory.MenuItem.Create(null, null, OpenFolder);
            m_renameImageItem = InitFactory.MenuItem.Create(null, null, m_imagePreviewPanel.RenameImage);
            m_moveImageAndRenameToNeighbourItem = InitFactory.MenuItem.Create(null, null, new EventHandler(this.MoveAndRenameToNeighbour));
            m_moveGroupToNeighbourItem = InitFactory.MenuItem.Create(null, null, MoveGroupToNeighbour);
            m_renameGroupAsNeighbourItem = InitFactory.MenuItem.Create(null, null, RenameCurrentGroupAsNeighbour);
            
            Items.Add(new ToolStripSeparator());
        }
        
        private void OnOpening(object sender, EventArgs e)
        {
            Items.Clear();
            
            Items.Add(m_copyPathItem);
            Items.Add(m_copyFileNameItem);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_openImageItem);
            Items.Add(m_openFolderItem);
            Items.Add(new ToolStripSeparator());
            Items.Add(m_renameImageItem);
            if (MoveAndRenameToNeighbourEnable())
            {
                Items.Add(m_moveImageAndRenameToNeighbourItem);
            }
            if (MoveGroupEnable())
            {
                Items.Add(new ToolStripSeparator());
                Items.Add(m_moveGroupToNeighbourItem);
                Items.Add(m_renameGroupAsNeighbourItem);
            }
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_copyPathItem.Text = s.ImagePreviewContextMenu_CopyPathItem_Text;
            m_copyFileNameItem.Text = s.ImagePreviewContextMenu_CopyFileNameItem_Text;
            m_openImageItem.Text = s.ImagePreviewContextMenu_OpenImageItem_Text;
            m_openFolderItem.Text = s.ImagePreviewContextMenu_OpenFolderItem_Text;
            m_renameImageItem.Text = s.ImagePreviewContextMenu_RenameImageItem_Text;
            m_moveImageAndRenameToNeighbourItem.Text = s.ImagePreviewContextMenu_MoveAndRenameImageToNeighbourItem_Text;
            m_moveGroupToNeighbourItem.Text = s.ImagePreviewContextMenu_MoveGroupToNeighbourItem_Text;
            m_renameGroupAsNeighbourItem.Text = s.ImagePreviewContextMenu_RenameGroupAsNeighbourItem_Text;
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

        private void CopyFileName(object sender, EventArgs e)
        {
            Clipboard.SetText(Path.GetFileNameWithoutExtension(m_imagePreviewPanel.CurrentImageInfo.path));
        }

        private void MoveAndRenameToNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.MoveAndRenameToNeighbour(m_imagePreviewPanel.RenameCurrentType);
        }

        /// <summary>
        /// Проверка на то, что директории у картинок разные.
        /// </summary>
        private bool MoveAndRenameToNeighbourEnable()
        {
            if (m_imagePreviewPanel.NeighbourImageInfo != null && m_imagePreviewPanel.CurrentImageInfo != null)
            {
                return m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString() != m_imagePreviewPanel.CurrentImageInfo.GetDirectoryString();
            }
            return false;
        }

        /// <summary>
        /// Возврашает истину, если в групе больше 2 файлов, тоесть есть смысл перемащать группами.
        /// </summary>
        /// <returns></returns>
        private bool MoveGroupEnable()
        {
            if (m_core.GetImageInfoSize(m_imagePreviewPanel.Group) > 2)
                return true;
            else
                return false;
        }

        /// <summary>
        /// Перенести группу в папку к соседней.
        /// </summary>
        private void MoveGroupToNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.MoveCurrentGroupToDirectory(m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString());
        }

        private void RenameCurrentGroupAsNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.RenameCurrentGroupAs(m_imagePreviewPanel.NeighbourImageInfo.GetFileNameWithoutExtensionString());
        }
    }
}
