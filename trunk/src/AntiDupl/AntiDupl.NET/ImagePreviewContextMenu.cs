/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2014 Yermalayeu Ihar, 2013-2014 Borisov Dmitry.
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
            m_renameImageLikeNeighbourItem = InitFactory.MenuItem.Create(null, null, new EventHandler(this.RenameLikeNeighbour));
            m_moveImageToNeighbourItem = InitFactory.MenuItem.Create(null, null, MoveImageToNeighbour);
            m_moveImageAndRenameToNeighbourItem = InitFactory.MenuItem.Create(null, null, new EventHandler(this.MoveAndRenameToNeighbour));
            
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
            Items.Add(m_renameImageLikeNeighbourItem);
            if (MoveImageToNeighbourEnable())
            {
                Items.Add(m_moveImageToNeighbourItem);
            }
            if (m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString() != m_imagePreviewPanel.CurrentImageInfo.GetDirectoryString())
                Items.Add(m_moveImageAndRenameToNeighbourItem);
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            m_copyPathItem.Text = s.ImagePreviewContextMenu_CopyPathItem_Text;
            m_copyFileNameItem.Text = s.ImagePreviewContextMenu_CopyFileNameItem_Text;
            m_openImageItem.Text = s.ImagePreviewContextMenu_OpenImageItem_Text;
            m_openFolderItem.Text = s.ImagePreviewContextMenu_OpenFolderItem_Text;
            m_renameImageItem.Text = s.ImagePreviewContextMenu_RenameImageItem_Text;
            m_renameImageLikeNeighbourItem.Text = s.ImagePreviewContextMenu_RenameLikeNeighbour_Text;
            m_moveImageToNeighbourItem.Text = s.ImagePreviewContextMenu_MoveImageToNeighbourItem_Text;
            m_moveImageAndRenameToNeighbourItem.Text = s.ImagePreviewContextMenu_MoveAndRenameImageToNeighbourItem_Text;
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

        private void RenameLikeNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.RenameCurrent(this.m_imagePreviewPanel.RenameCurrentType, GetNewNameLikeNeighbour());
        }

        private void MoveAndRenameToNeighbour(object sender, EventArgs e)
        {
            m_resultsListView.RenameCurrent(this.m_imagePreviewPanel.RenameCurrentType, GetNewNameToMove());
        }

        #region Rename image
        private string GetNewNameToMove()
        {
            StringBuilder builder = new StringBuilder(m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString());
            builder.Append(@"\");
            builder.Append(Path.GetFileNameWithoutExtension(m_imagePreviewPanel.NeighbourImageInfo.path));
            builder.Append(Path.GetExtension(m_imagePreviewPanel.CurrentImageInfo.path));
            if (!System.IO.File.Exists(builder.ToString()))
            {
                return builder.ToString();
            }
            else
            {
                return SimilarRename(builder.ToString());
            }
        }

        private string GetNewNameLikeNeighbour()
        {
            StringBuilder builder = new StringBuilder(m_imagePreviewPanel.CurrentImageInfo.GetDirectoryString());
            builder.Append(@"\");
            builder.Append(Path.GetFileNameWithoutExtension(m_imagePreviewPanel.NeighbourImageInfo.path));
            builder.Append(Path.GetExtension(m_imagePreviewPanel.CurrentImageInfo.path));
            if (!System.IO.File.Exists(builder.ToString()))
            {
                return builder.ToString();
            }
            else
            {
                return SimilarRename(builder.ToString());
            }
        }

        private string SimilarRename(String targetPath)
        {
            ulong digit = 0;
            string digitName = String.Empty;
            int leadingZero = 0;

            digit = GetDigit(Path.GetFileNameWithoutExtension(targetPath), out digitName, out leadingZero);

            if (digit == 0)
                targetPath = GetNewNameForFileAdd(targetPath, 2);
            else
                targetPath = GetNewNameForFileDig(Path.Combine(Directory.GetParent(targetPath).ToString() + "\\", digitName),
                                                leadingZero,
                                                digit + 1,
                                                Path.GetExtension(targetPath),
                                                targetPath);
            return targetPath;
        }

        /// <summary>
        /// Check is in file name number separated by the character "_" from remaining part of file name. Returns number or 0 in case of failure.
        /// </summary>
        /// <param name="name">file name</param>
        /// <param name="digitName">Output file name without number and "_"</param>
        /// <param name="zero">Number of leading zero</param>
        /// <returns>0 or the received number</returns>
        private ulong GetDigit(string name, out string digitName, out int zero)
        {
            int length = name.Length;
            int sym = name.LastIndexOf('_');
            bool ren = true;
            for (int u = sym + 1; u < length; u++)
                if (!char.IsDigit(name[u]))
                    ren = false;

            zero = 0;
            ulong result = 0;
            if (ren)
            {
                string intPar = name.Substring(sym + 1);
                ulong.TryParse(intPar, out result);
                zero = intPar.Length - result.ToString().Length;
            }

            digitName = name.Substring(0, sym + 1);
            return result;
        }

        /// <summary>
        /// Adding to number file name in a case when in it it wasn't.
        /// </summary>
        /// <param name="oldName">Old name</param>
        /// <param name="i">Number</param>
        /// <returns>New name</returns>
        private string GetNewNameForFileAdd(string oldName, ulong i)
        {
            string newName = string.Format("{0}\\{1}_{2}{3}", Directory.GetParent(oldName).ToString(), Path.GetFileNameWithoutExtension(oldName), i, Path.GetExtension(oldName));
            if (File.Exists(newName))
            {
                i = i + 1;
                newName = GetNewNameForFileAdd(oldName, i);
            }
            return newName;
        }

        /// <summary>
        /// Adding to number file name in a case when in it was number.
        /// </summary>
        /// <param name="oldName">Old name</param>
        /// <param name="i">Number</param>
        /// <param name="extension">Filename extension</param>
        /// <returns>New name</returns>
        private string GetNewNameForFileDig(string oldName, int zero, ulong i, string extension, string sourceName)
        {
            string newName = String.Empty;
            if (i.ToString().Length > (i - 1).ToString().Length)
                zero--;
            StringBuilder builder = new StringBuilder(oldName);
            for (int j = 0; j < zero; j++)
                builder.Append("0");
            builder.Append(i);
            builder.Append(extension);
            newName = builder.ToString();

            if (File.Exists(newName))
            {
                newName = GetNewNameForFileAdd(sourceName, 2);
            }
            return newName;
        }
        #endregion
    }
}
