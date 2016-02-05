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
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Diagnostics;
using System.IO;

namespace AntiDupl.NET
{
    public class PictureBoxPanel : Panel
    {
        private const int MAX_PATH = 260;

        private CoreLib m_core;
        private Options m_options;
        private CoreImageInfo m_currentImageInfo;
        private Size m_neighbourSizeMax;

        private Rectangle m_bitmapRect;
        private MemoryStream m_memoryStream;
        private Bitmap m_bitmap;
        /// <summary>
        /// Изображение загруженное из файла.
        /// </summary>
        public Bitmap Bitmap { get 
        {
            if (m_bitmap != m_originalBitmap && m_originalBitmap != null)
            {
                return m_originalBitmap;
            }
            return m_bitmap; 
        } }
        private bool m_animationEnable = false;
        private bool m_currentlyAnimating = false;

        private Bitmap m_originalBitmap;
        private Rectangle[] m_rectanglesOfDifferences;
        private Pen penForDifferences = new Pen(new SolidBrush(Color.Red), 4);

        private ImagePreviewPanel.Position m_position;
        public ImagePreviewPanel.Position Position
        {
            get { return m_position; }
            set
            {
                m_position = value;
            }
        }
        private string m_prevFile;
        private string m_nextFile;
        private Bitmap m_prevBitmap;
        private Bitmap m_nextBitmap;
        private Rectangle m_prevBitmapRect;
        private Rectangle m_nextBitmapRect;
        private enum Neighbour
        {
            Previous,
            Next,
        }


        public PictureBoxPanel(CoreLib core, Options options)
        {
            m_core = core;
            m_options = options;
            InitializeComponents();
            m_options.resultsOptions.OnImageViewChange += new ResultsOptions.ImageViewChangeHandler(OnImageViewChange);
        }

        private void InitializeComponents()
        {
            Location = new System.Drawing.Point(0, 0);
            Dock = DockStyle.Fill;
            BorderStyle = BorderStyle.Fixed3D;
            BackColor = Color.DarkGray;
            DoubleBuffered = true;

            DoubleClick += new EventHandler(OnImageDoubleClicked);
            SizeChanged += new EventHandler(OnSizeChanged);
        }

        public void UpdateImage(CoreImageInfo currentImageInfo)
        {
            if (m_originalBitmap != null)
            {
                m_originalBitmap.Dispose();
                m_originalBitmap = null;
            }
            m_currentImageInfo = currentImageInfo;
            if (currentImageInfo != null)
            {
                StopAnimate();
                if (m_currentImageInfo.type != CoreDll.ImageType.None)
                {
                    if (m_currentImageInfo.path.Length < MAX_PATH && m_currentImageInfo.type <= CoreDll.ImageType.Icon)
                    {
                        try
                        {
                            if (LoadFileToMemoryStream(ref m_memoryStream, m_currentImageInfo.path))
                            {
                                m_bitmap = new Bitmap(m_memoryStream);
                                m_animationEnable = ImageAnimator.CanAnimate(m_bitmap);
                                if (m_animationEnable)
                                    m_currentlyAnimating = false;
                            }
                            else
                            {
                                m_bitmap = null;
                            }
                        }
                        catch
                        {
                            m_bitmap = m_core.LoadBitmap(m_currentImageInfo);
                        }
                    }
                    else
                    {
                        m_bitmap = m_core.LoadBitmap(m_currentImageInfo);
                    }
                }
                else
                {
                    m_bitmap = null;
                }

                if (m_options.resultsOptions.ShowNeighboursImages)
                    ShowNeighboursImages(true);
            }
        }

        private bool LoadFileToMemoryStream(ref MemoryStream memoryStream, string path)
        {
            if (memoryStream != null)
            {
                memoryStream.Close();
                memoryStream.Dispose();
                memoryStream = null;
            }
            FileInfo fileInfo = new FileInfo(path);
            if (fileInfo.Exists)
            {
                try
                {
                    FileStream fileStream = new FileStream(fileInfo.FullName, FileMode.Open, FileAccess.Read);
                    byte[] buffer = new byte[fileStream.Length];
                    fileStream.Read(buffer, 0, buffer.Length);
                    fileStream.Close();
                    memoryStream = new MemoryStream(buffer);
                    return true;
                }
                catch
                {
                    return false;
                }
            }
            return false;
        }


        private void AnimateImage()
        {
            if (!m_currentlyAnimating)
            {
                ImageAnimator.Animate(m_bitmap, new EventHandler(OnFrameChanged));
                m_currentlyAnimating = true;
            }
        }

        private void StopAnimate()
        {
            m_animationEnable = false;
            ImageAnimator.StopAnimate(m_bitmap, new EventHandler(OnFrameChanged));
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (m_animationEnable)
            {
                try
                {
                    AnimateImage();
                    ImageAnimator.UpdateFrames();
                }
                catch
                {
                    m_animationEnable = false;
                }
            }
            if (m_bitmap != null)
            {
                e.Graphics.DrawImage(m_bitmap, m_bitmapRect);
            }
            if (m_options.resultsOptions.ShowNeighboursImages)
            {
                if (m_prevBitmap != null)
                    e.Graphics.DrawImage(m_prevBitmap, m_prevBitmapRect);
                if (m_nextBitmap != null)
                    e.Graphics.DrawImage(m_nextBitmap, m_nextBitmapRect);
            }
        }

        private void OnFrameChanged(object sender, EventArgs e)
        {
            Invalidate();
        }

        private void OnImageDoubleClicked(object sender, System.EventArgs e)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = m_currentImageInfo.path;
            try
            {
                Process.Start(startInfo);
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }

        private void OnSizeChanged(object sender, EventArgs e)
        {
            UpdateImagePadding(m_neighbourSizeMax);
            if (m_options.resultsOptions.ShowNeighboursImages)
                ShowNeighboursImages(false);
            Refresh();
        }

        private void OnImageViewChange()
        {
            UpdateImagePadding(m_neighbourSizeMax);
            if (m_options.resultsOptions.ShowNeighboursImages)
                ShowNeighboursImages(true);
            Refresh();
        }

        public void UpdateImagePadding(Size neighbourSizeMax)
        {
            m_neighbourSizeMax = neighbourSizeMax;
            if (m_currentImageInfo != null)
            {
                int horizontalPosition = 0, verticalPosition = 0;
                int clientWidth = ClientSize.Width;
                int clientHeight = ClientSize.Height;
                int currentWidth = (int)m_currentImageInfo.width;
                int currentHeight = (int)m_currentImageInfo.height;
                int targetWidth = 100;
                int targetHeight = 100;
                if (currentWidth > 0 && currentHeight > 0)
                {
                    if (m_options.resultsOptions.ProportionalImageSize)
                    {
                        int neighbourWidth = (int)m_neighbourSizeMax.Width;
                        int neighbourHeight = (int)m_neighbourSizeMax.Height;
                        int maxWidth = Math.Max(currentWidth, neighbourWidth);
                        int maxHeight = Math.Max(currentHeight, neighbourHeight);
                        if (m_options.resultsOptions.StretchSmallImages || maxWidth >= clientWidth || maxHeight >= clientHeight)
                        {
                            if (maxWidth * clientHeight > maxHeight * clientWidth)
                            {
                                targetHeight = currentHeight * clientWidth / maxWidth;
                                targetWidth = currentWidth * clientWidth / maxWidth;

                                verticalPosition = (clientHeight - targetHeight) / 2;
                                if (m_options.resultsOptions.ShowNeighboursImages)
                                {
                                    if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                    {
                                        horizontalPosition = clientWidth - targetWidth; //сдвигаем влево
                                        verticalPosition = (clientHeight - targetHeight) / 2;
                                    }
                                    else
                                    {
                                        horizontalPosition = (clientWidth - targetWidth) / 2;
                                        verticalPosition = 0;
                                    }
                                }
                                else
                                {
                                    horizontalPosition = (clientWidth - targetWidth) / 2;
                                    verticalPosition = (clientHeight - targetHeight) / 2;
                                }
                            }
                            else
                            {
                                targetHeight = currentHeight * clientHeight / maxHeight;
                                targetWidth = currentWidth * clientHeight / maxHeight;

                                if (m_options.resultsOptions.ShowNeighboursImages)
                                {
                                    if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                    {
                                        horizontalPosition = clientWidth - targetWidth; //сдвигаем влево
                                        verticalPosition = (clientHeight - targetHeight) / 2; //посеридине
                                    }
                                    else
                                    {
                                        horizontalPosition = (clientWidth - targetWidth) / 2;
                                        verticalPosition = 0;
                                    }
                                }
                                else
                                {
                                    verticalPosition = (clientHeight - targetHeight) / 2;
                                    horizontalPosition = (clientWidth - targetWidth) / 2;
                                }
                            }
                        }
                        else
                        {
                            targetHeight = currentHeight;
                            targetWidth = currentWidth;
                            if (m_options.resultsOptions.ShowNeighboursImages)
                            {
                                if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                {
                                    verticalPosition = (clientHeight - currentHeight) / 2;
                                    horizontalPosition = clientWidth - currentWidth;
                                }
                                else
                                {
                                    verticalPosition = 0;
                                    horizontalPosition = (clientWidth - currentWidth) / 2;
                                }
                            }
                            else
                            {
                                verticalPosition = (clientHeight - currentHeight) / 2;
                                horizontalPosition = (clientWidth - currentWidth) / 2;
                            }
                        }
                    }
                    else // если не надо пропорциональными делать
                    {
                        if (m_options.resultsOptions.StretchSmallImages || currentWidth >= clientWidth || currentHeight >= clientHeight)
                        {
                            if (currentWidth * clientHeight > currentHeight * clientWidth) //если ширина больше
                            {
                                targetWidth = clientWidth;
                                targetHeight = clientWidth * currentHeight / currentWidth;
                                if (m_options.resultsOptions.ShowNeighboursImages)
                                {
                                    if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                        verticalPosition = (clientHeight - targetHeight) / 2;
                                    else
                                        verticalPosition = 0;
                                }
                                else
                                    verticalPosition = (clientHeight - targetHeight) / 2;

                            }
                            else
                            {
                                targetWidth = clientHeight * currentWidth / currentHeight;
                                targetHeight = clientHeight;
                                if (m_options.resultsOptions.ShowNeighboursImages)
                                {
                                    if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                        horizontalPosition = clientWidth - targetWidth;
                                }
                                else
                                    horizontalPosition = (clientWidth - targetWidth) / 2;
                            }
                        }
                        else
                        {
                            if (m_options.resultsOptions.ShowNeighboursImages)
                            {
                                if (m_position == ImagePreviewPanel.Position.Top || m_position == ImagePreviewPanel.Position.Bottom)
                                    horizontalPosition = clientWidth - currentWidth;
                                else
                                    verticalPosition = 0;
                            }
                            else
                            {
                                verticalPosition = (clientHeight - currentHeight) / 2;
                                horizontalPosition = (clientWidth - currentWidth) / 2;
                            }
                            targetWidth = currentWidth;
                            targetHeight = currentHeight;
                        }
                    }
                }
                m_bitmapRect = new Rectangle(horizontalPosition, verticalPosition, targetWidth, targetHeight);
            }
            //Refresh();
        }

        /// <summary>
        /// Рисует в храящемся изображение m_bitmap бордюры отличий.
        /// </summary>
        public bool SetDifference(List<Rectangle> rectanglesOfDifferenceIn)
        {
            if (!m_animationEnable)
            {
                if (m_originalBitmap == null)
                    m_originalBitmap = m_bitmap.Clone() as Bitmap;
                else if (m_bitmap != m_originalBitmap)
                    m_bitmap = m_originalBitmap.Clone() as Bitmap;

                m_rectanglesOfDifferences = new Rectangle[rectanglesOfDifferenceIn.Count];
                rectanglesOfDifferenceIn.CopyTo(m_rectanglesOfDifferences);

                //преобразуем в соответсвии с размером полного изображения
                double multiplierX = m_bitmap.Width / (double)m_options.resultsOptions.NormalizedSizeOfImage;
                double multiplierY = m_bitmap.Height / (double)m_options.resultsOptions.NormalizedSizeOfImage;

                for (int i = 0; i < m_rectanglesOfDifferences.Length; i++)
                {
                    m_rectanglesOfDifferences[i] = new Rectangle((int)(m_rectanglesOfDifferences[i].X * multiplierX),
                        (int)(m_rectanglesOfDifferences[i].Y * multiplierY),
                        (int)(m_rectanglesOfDifferences[i].Width * multiplierX),
                        (int)(m_rectanglesOfDifferences[i].Height * multiplierY));
                }

                int penThickness = Math.Min(m_bitmap.Width, m_bitmap.Height) * m_options.resultsOptions.PenThickness / m_options.resultsOptions.NormalizedSizeOfImage;
                penForDifferences = new Pen(new SolidBrush(Color.Red), penThickness);
                try
                {
                    using (Graphics gr = Graphics.FromImage(m_bitmap))
                    {
                        for (int i = 0; i < m_rectanglesOfDifferences.Length; i++)
                            gr.DrawRectangle(penForDifferences, m_rectanglesOfDifferences[i]);
                    }
                    this.Invalidate();
                    return true;
                }
                catch (Exception)
                {
                    //на этом изображение не получается рисовать
                    return false;
                }
            }
            return false;
        }

        public void ClearDifference()
        {
            if (m_originalBitmap != null)
            {
                m_bitmap = m_originalBitmap.Clone() as Bitmap;
                m_originalBitmap.Dispose();
                m_originalBitmap = null;
            }
            m_rectanglesOfDifferences = null;
            this.Invalidate();
        }

        private void ShowNeighboursImages(bool forceLoad)
        {
            if (m_options.resultsOptions.ShowNeighboursImages && m_currentImageInfo != null)
            {
                if (forceLoad)
                {
                    CleanNeighbours();
                    GetNeighboursFileNames(m_currentImageInfo.path, ref m_prevFile, ref m_nextFile);
                }
                if (m_prevFile != null)
                    m_prevBitmap = GetBitmap(m_prevFile);
                if (m_prevBitmap != null)
                    m_prevBitmapRect = GetRectangle(m_position, m_prevBitmap, Neighbour.Previous);
                if (m_nextFile != null)
                    m_nextBitmap = GetBitmap(m_nextFile);
                if (m_nextBitmap != null)
                    m_nextBitmapRect = GetRectangle(m_position, m_nextBitmap, Neighbour.Next);
            }
            else
            {
                CleanNeighbours();
            }
        }

        private void CleanNeighbours()
        {
            if (m_prevFile != null)
                m_prevFile = null;
            if (m_nextFile != null)
                m_nextFile = null;
            if (m_prevBitmap != null)
                m_prevBitmap = null;
            if (m_nextBitmap != null)
                m_nextBitmap = null;
        }

        private Rectangle GetRectangle(ImagePreviewPanel.Position position, Bitmap bitmap, Neighbour neighbour)
        {
            const float PERCENT_OF_NEIGHBOUR = 0.30F;
            int clientWidth = ClientSize.Width;
            int clientHeight = ClientSize.Height;
            int neighbourMaxWidth = (int)((float)clientWidth * PERCENT_OF_NEIGHBOUR);
            int neighbourMaxHeight = (int)((float)clientHeight * PERCENT_OF_NEIGHBOUR);
            int currentWidth = bitmap.Width;
            int currentHeight = bitmap.Height;
            int horizontalPosition = 0, verticalPosition = 0;
            int neighbourWidth, neighbourHeight;

            if (currentWidth > currentHeight)
            {
                neighbourWidth = neighbourMaxWidth;
                neighbourHeight = neighbourMaxWidth * currentHeight / currentWidth;
            }
            else if (currentWidth < currentHeight)
            {
                neighbourHeight = neighbourMaxHeight;
                neighbourWidth = neighbourMaxHeight * currentWidth / currentHeight;
            }
            else
            {
                neighbourWidth = neighbourMaxWidth;
                neighbourHeight = neighbourMaxHeight;
            }

            if (position == ImagePreviewPanel.Position.Top || position == ImagePreviewPanel.Position.Bottom)
            {
                if (neighbour == Neighbour.Next)
                {
                    verticalPosition = clientHeight - neighbourHeight;
                }
            }
            else
            {
                verticalPosition = clientHeight - neighbourHeight;
                if (neighbour == Neighbour.Next)
                {
                    horizontalPosition = clientWidth - neighbourWidth;
                }
            }

            return new Rectangle(horizontalPosition, verticalPosition, neighbourWidth, neighbourHeight);
        }

        private Bitmap GetBitmap(string m_fileName)
        {
            try
            {
                if (m_fileName != null)
                {
                    MemoryStream memoryStream = null;
                    if (LoadFileToMemoryStream(ref memoryStream, m_fileName))
                    {
                        return new Bitmap(memoryStream);
                    }
                }
            }
            catch
            {
                return m_core.LoadBitmap(m_currentImageInfo);
            }

            return null;
        }

        private void GetNeighboursFileNames(string filePreview, ref string m_prevFile, ref string m_nextFile)
        {
            if (m_prevFile == null && m_nextFile == null)
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(Path.GetDirectoryName(filePreview));
                if (directoryInfo.Exists)
                {
                    FileInfo[] filesInfos = directoryInfo.GetFiles();
                    for (int i = 0; i < filesInfos.Length; i++)
                    {
                        if (filesInfos[i].FullName == filePreview)
                        {
                            if (i > 0)  //previos
                                if (File.Exists(filesInfos[i - 1].FullName))
                                    m_prevFile = filesInfos[i - 1].FullName;
                            if (i < filesInfos.Length - 1) //next
                                if (File.Exists(filesInfos[i + 1].FullName))
                                    m_nextFile = filesInfos[i + 1].FullName;
                            break;
                        }
                    }
                }
            }
        }
    }
}
