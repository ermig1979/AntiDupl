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
            StringBuilder targetPath = new StringBuilder(m_imagePreviewPanel.NeighbourImageInfo.GetDirectoryString());
            targetPath.Append(@"\");
            targetPath.Append(Path.GetFileNameWithoutExtension(m_imagePreviewPanel.NeighbourImageInfo.path));
            targetPath.Append(Path.GetExtension(m_imagePreviewPanel.CurrentImageInfo.path));
            if (!System.IO.File.Exists(targetPath.ToString()))
            {
                return targetPath.ToString();
            }
            else
            {
                return SimilarRename(targetPath.ToString(), "");
            }
        }

        private string GetNewNameLikeNeighbour()
        {
            StringBuilder targetPath = new StringBuilder(m_imagePreviewPanel.CurrentImageInfo.GetDirectoryString());
            targetPath.Append(@"\");
            targetPath.Append(Path.GetFileNameWithoutExtension(m_imagePreviewPanel.NeighbourImageInfo.path));
            targetPath.Append(Path.GetExtension(m_imagePreviewPanel.CurrentImageInfo.path));
            if (!System.IO.File.Exists(targetPath.ToString()))
            {
                return targetPath.ToString();
            }
            else
            {
                return SimilarRename(targetPath.ToString(), m_imagePreviewPanel.CurrentImageInfo.path);
            }
        }

        /// <summary>
        /// Возврашает похожее имя, чтобы в списке файлов файлы лежали рядом.
        /// </summary>
        /// <param name="targetPath">Путь на который должен быть похоже результирующее имя</param>
        /// <param name="currentName">Текущее имя файла</param>
        /// <returns></returns>
        private string SimilarRename(string targetPath, string currentName)
        {
            ulong digit = 0;
            string nameWithoutNumber = String.Empty;
            int leadingZero = 0;

            digit = GetDigit(Path.GetFileNameWithoutExtension(targetPath), out nameWithoutNumber, out leadingZero);

            if (digit == 0)
                targetPath = GetNewNameForFileAdd(targetPath, 2);
            else
                targetPath = GetNewNameForFileDig(Path.Combine(Directory.GetParent(targetPath).ToString() + "\\", nameWithoutNumber),
                                                leadingZero,
                                                digit + 1,
                                                Path.GetExtension(targetPath),
                                                targetPath,
                                                currentName);
            return targetPath;
        }

        /// <summary>
        /// Check is in file name number separated by the non digit character from remaining part of file name. Returns number or 0 in case of failure.
        /// </summary>
        /// <param name="name">file name</param>
        /// <param name="pathWithoutNumber">Output file name without number and "_"</param>
        /// <param name="numOfZero">Number of leading numOfZero</param>
        /// <returns>0 or the received number</returns>
        private ulong GetDigit(string name, out string nameWithoutDigit, out int numOfZero)
        {
            int length = name.Length;
            //Находим первый не числовой символ с конца
            bool canRename;
            int digitPos = length;
            for (int u = length - 1; u >= 0; u--)
                if (!char.IsDigit(name[u]))
                {
                    digitPos = u;
                    break;
                }
            if (digitPos < length) //если цифра найдена
                canRename = true;
            else
                canRename = false;

            ulong result = 0;
            numOfZero = 0;
            if (canRename)
            {
                string forParsing = name.Substring(digitPos + 1);
                ulong.TryParse(forParsing, out result);
                numOfZero = forParsing.Length - result.ToString().Length;
            }

            if (digitPos < length)
                nameWithoutDigit = name.Substring(0, digitPos + 1);
            else
                nameWithoutDigit = string.Empty;
            return result;
        }

        /// <summary>
        /// Adding to number file name in a case when in it it wasn't.
        /// </summary>
        /// <param name="pathWithoutNumber">Old name</param>
        /// <param name="digit">Number</param>
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
        /// <param name="pathWithoutNumber">Old name</param>
        /// <param name="digit">Number</param>
        /// <param name="extension">Filename extension</param>
        /// <returns>New name</returns>
        private string GetNewNameForFileDig(string pathWithoutNumber, int zero, ulong digit, string extension, string sourceName, string currentName)
        {
            string newName = String.Empty;
            if (digit.ToString().Length > (digit - 1).ToString().Length) //не добавлять один ноль если цифра удленилась
                zero--;

            StringBuilder builder = new StringBuilder(pathWithoutNumber);
            for (int j = 0; j < zero; j++)
                builder.Append("0");
            builder.Append(digit); //incresed number
            builder.Append(extension);
            newName = builder.ToString();

            if (File.Exists(newName))
            {
                if (string.Compare(newName, currentName, true) != 0)
                    newName = GetNewNameForFileAdd(sourceName, 2);
            }
            return newName;
        }
        #endregion
    }
}
