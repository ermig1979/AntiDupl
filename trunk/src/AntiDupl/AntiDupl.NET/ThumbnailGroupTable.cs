/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2014 Yermalayeu Ihar.
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
using System.Threading;

namespace AntiDupl.NET
{
    public class ThumbnailGroupTable : Panel
    {
        private CoreLib m_core;
        private AntiDupl.NET.Options m_options;
        private CoreGroup[] m_groups;
        private int m_maxGroupIndex = -1;
        private MainSplitContainer m_mainSplitContainer;

        private ThumbnailStorage m_thumbnailStorage = null;
        private volatile bool m_abortUpdateThumbnailsThread = false;
        private Thread m_updateThumbnailsThread = null;

        private ThumbnailGroupPanel[] m_thumbnailGroupPanels;
        private bool m_changeControls = true;
        private DateTime m_lastUpdate = DateTime.Now;

        public delegate void CurrentThumbnailChangedHandler(CoreGroup group, int index);
        public event CurrentThumbnailChangedHandler OnCurrentThumbnailChanged;

        public ThumbnailGroupTable(CoreLib core, AntiDupl.NET.Options options, MainSplitContainer mainSplitContainer)
        {
            m_core = core;
            m_options = options;
            m_mainSplitContainer = mainSplitContainer;
            m_thumbnailStorage = new ThumbnailStorage(m_core, m_options);
            InitializeComponents();
        }

        private void InitializeComponents()
        {
            Location = new Point(0, 0);
            Dock = DockStyle.Fill;
            AutoScroll = true;
            DoubleBuffered = true;
            BackColor = Color.Transparent;

            MouseEnter += new EventHandler(OnMouseEnter);
        }

        public void UpdateGroups()
        {
            UpdateThumbnailsStop();
            GetGroups();
            UpdateControls();
            UpdateThumbnailsStart();
            //if (OnUpdateResults != null)
            //    OnUpdateResults();
            Invalidate();
        }

        public void ClearGroups()
        {
            UpdateThumbnailsStop();
            m_thumbnailStorage.Clear();
            m_groups = new CoreGroup[0];
            m_maxGroupIndex = -1;
            Controls.Clear();
        }

        private void GetGroups()
        {
            uint groupSize = m_core.GetGroupSize();
            if (groupSize == 0)
            {
                m_groups = new CoreGroup[0];
                m_maxGroupIndex = -1;
                return;
            }
            m_groups = m_core.GetGroup(0, groupSize);
            int groupSizeMax = 0;
            for (int i = 0; i < m_groups.Length; ++i)
            {
                if (m_groups[i].images.Length > groupSizeMax)
                {
                    groupSizeMax = m_groups[i].images.Length;
                    m_maxGroupIndex = i;
                }
            }
        }

        private void UpdateControls()
        {
            SuspendDrawing(this);
            Controls.Clear();

            int width = Padding.Horizontal;
            int height = Padding.Vertical;
            
            if (m_groups.Length > 0)
            {
                m_thumbnailGroupPanels = new ThumbnailGroupPanel[m_groups.Length];

                AddGroupPanel(0);

                if (m_groups.Length > 1)
                {
                    AddGroupPanel(m_groups.Length - 1);
                }

                if (m_maxGroupIndex != 0 && m_maxGroupIndex != m_groups.Length - 1)
                {
                    AddGroupPanel(m_maxGroupIndex);
                }

                ThumbnailGroupPanel maxPanel = m_thumbnailGroupPanels[m_maxGroupIndex];
                height += (maxPanel.Height + maxPanel.Margin.Vertical) * m_groups.Length;
                width += maxPanel.Width + maxPanel.Margin.Horizontal;
            }
            AutoScrollMinSize = new Size(width, height);
            ResumeDrawing(this);
        }

        private bool IsSpecial(int index)
        {
            return (index == 0 || index == m_groups.Length - 1 || index == m_maxGroupIndex);
        }

        private int GetVisibleGroupIndexMin()
        {
            int height = m_thumbnailGroupPanels[0].Height + m_thumbnailGroupPanels[0].Margin.Vertical;
            int index = (int)Math.Floor((-AutoScrollPosition.Y - Padding.Top) / (double)height);
            return Math.Max(0, index);// - 1000);
        }

        private int GetVisibleGroupIndexMax()
        {
            int height = m_thumbnailGroupPanels[0].Height + m_thumbnailGroupPanels[0].Margin.Vertical;
            int index = (int)Math.Ceiling((ClientSize.Height - AutoScrollPosition.Y - Padding.Top) / (double)height);
            return Math.Min(m_groups.Length - 1, index);// + 1000);
        }

        private void AddGroupPanel(int index)
        {
            if (m_thumbnailGroupPanels[index] == null)
            {

                ThumbnailGroupPanel groupPanel = new ThumbnailGroupPanel(m_core, m_options, m_groups[index], this);
                groupPanel.Location = new Point(
                    Padding.Left + groupPanel.Margin.Left + AutoScrollPosition.X,
                    Padding.Top + groupPanel.Margin.Top + AutoScrollPosition.Y + (groupPanel.Height + groupPanel.Margin.Vertical) * index);

                ThumbnailPanel[] thumbnailPanels = groupPanel.ThumbnailPanels;
                for (int i = 0; i < thumbnailPanels.Length; ++i)
                {
                    if (m_thumbnailStorage.Exists(thumbnailPanels[i].ImageInfo))
                    {
                        thumbnailPanels[i].Thumbnail = m_thumbnailStorage.Get(thumbnailPanels[i].ImageInfo);
                    }
                }
                m_thumbnailGroupPanels[index] = groupPanel;
                Controls.Add(groupPanel);
                m_changeControls = true;
                Console.Write("a");
            }
        }

        private void AddGroupPanels(int indexMin, int indexMax)
        {
            List<Control> controls = new List<Control>();
            for (int i = indexMin; i < indexMax; ++i)
            {
                if (m_thumbnailGroupPanels[i] == null)
                {

                    ThumbnailGroupPanel groupPanel = new ThumbnailGroupPanel(m_core, m_options, m_groups[i], this);
                    groupPanel.Location = new Point(
                        Padding.Left + groupPanel.Margin.Left + AutoScrollPosition.X,
                        Padding.Top + groupPanel.Margin.Top + AutoScrollPosition.Y + (groupPanel.Height + groupPanel.Margin.Vertical) * i);

                    ThumbnailPanel[] thumbnailPanels = groupPanel.ThumbnailPanels;
                    for (int j = 0; j < thumbnailPanels.Length; ++j)
                    {
                        if (m_thumbnailStorage.Exists(thumbnailPanels[j].ImageInfo))
                        {
                            thumbnailPanels[j].Thumbnail = m_thumbnailStorage.Get(thumbnailPanels[j].ImageInfo);
                        }
                    }

                    //groupPanel.Visible = false;

                    controls.Add(groupPanel);

                    m_thumbnailGroupPanels[i] = groupPanel;

                }
            }

            if (controls.Count > 0)
            {
                Controls.AddRange(controls.ToArray());
            }
        }

        private void RemoveGroupPanel(int index)
        {
            if (index > 0 && m_thumbnailGroupPanels[index] != null && !IsSpecial(index))
            {
                Controls.Remove(m_thumbnailGroupPanels[index]);
                m_thumbnailGroupPanels[index] = null;
                m_changeControls = true;
            }
        }

        private void UpdateVisiblePanels()
        {
            if (m_thumbnailGroupPanels != null && m_thumbnailGroupPanels.Length > 0 && m_thumbnailGroupPanels[0] != null)
            {
                int minIndex = GetVisibleGroupIndexMin();
                int maxIndex = GetVisibleGroupIndexMax();

                SuspendLayout();
                //Visible = false;
                for (int i = 0; i < minIndex; ++i)
                {
                    RemoveGroupPanel(i);
                }
                for (int i = minIndex; i < maxIndex; ++i)
                {
                    AddGroupPanel(i);
                    //Application.DoEvents();
                }
                for (int i = maxIndex; i < m_thumbnailGroupPanels.Length; ++i)
                {
                    RemoveGroupPanel(i);
                }
                PerformLayout();
                //Visible = true;
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if ((DateTime.Now - m_lastUpdate).TotalMilliseconds < 10)
            {
                return;
            }
            m_lastUpdate = DateTime.Now;

            //for (int i = 0; i < Controls.Count; ++i)
            //    SuspendDrawing(Controls[i]);

            m_changeControls = false;
            DateTime t = DateTime.Now;
            UpdateVisiblePanels();
            TimeSpan updateTime = DateTime.Now - t;
            Console.WriteLine("ut = {0}; at = {1}.", updateTime.TotalMilliseconds.ToString(), t.Millisecond);

            //for (int i = 0; i < Controls.Count; ++i)
            //{
            //    ResumeDrawing(Controls[i]);
            //    Controls[i].Update();
            //}

            //ControlPaint.

            //base.OnPaint(e);
            //for (int i = 0; i < Controls.Count; ++i)
            //{
            //    //ResumeDrawing(Controls[i]);
            //    Rectangle rect = new Rectangle(Controls[i].Location, Controls[i].Size);// .RectangleToClient();// .ClientRectangle;
            //    rect.Offset(AutoScrollPosition.X, AutoScrollPosition.Y);
            //    ControlPaint.DrawBorder3D(e.Graphics, rect, Border3DStyle.Raised, Border3DSide.All);
            //}


            if (m_changeControls)
            {
                Invalidate();
            }
            else
                base.OnPaint(e);
        }

        protected override void OnScroll(ScrollEventArgs se)
        {
            //UpdateVisiblePanels();
            base.OnScroll(se);
        }

        protected override void OnResize(EventArgs eventargs)
        {
            //UpdateVisiblePanels();
            base.OnResize(eventargs);
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            //UpdateVisiblePanels();
            base.OnMouseWheel(e);
        }

        public void UpdateThumbnailsStart()
        {
            m_abortUpdateThumbnailsThread = false;
            m_updateThumbnailsThread = new Thread(UpdateThumbnailsThread);
            m_updateThumbnailsThread.Start();
        }

        public void UpdateThumbnailsStop()
        {
            m_abortUpdateThumbnailsThread = true;
            if (m_updateThumbnailsThread != null)
            {
                m_updateThumbnailsThread.Join();
            }
        }

        private void UpdateThumbnailsThread()
        {
            for (int i = 0; i < m_groups.Length; ++i)
            {
                CoreImageInfo[] images = m_groups[i].images;
                for (int j = 0; j < images.Length; ++j)
                {
                    m_thumbnailStorage.Get(images[j]);
                    if (m_abortUpdateThumbnailsThread)
                        return;
                }

                ThumbnailGroupPanel groupPanel = m_thumbnailGroupPanels[i];
                if(groupPanel != null)
                {
                    ThumbnailPanel[] thumbnailPanels = groupPanel.ThumbnailPanels;
                    for (int j = 0; j < thumbnailPanels.Length; ++j)
                    {
                        if (m_thumbnailStorage.Exists(thumbnailPanels[j].ImageInfo))
                        {
                            thumbnailPanels[j].Thumbnail = m_thumbnailStorage.Get(thumbnailPanels[j].ImageInfo);
                        }
                    }
                }
            }
        }

        private void OnMouseEnter(object sender, EventArgs e)
        {
            Focus();
        }

        private const int WM_SETREDRAW = 0x000B;

        public static void SuspendDrawing(Control control)
        {
            Message msgSuspendUpdate = Message.Create(control.Handle, WM_SETREDRAW, IntPtr.Zero,
                IntPtr.Zero);

            NativeWindow window = NativeWindow.FromHandle(control.Handle);
            window.DefWndProc(ref msgSuspendUpdate);
        }

        public static void ResumeDrawing(Control control)
        {
            // Create a C "true" boolean as an IntPtr
            IntPtr wparam = new IntPtr(1);
            Message msgResumeUpdate = Message.Create(control.Handle, WM_SETREDRAW, wparam,
                IntPtr.Zero);

            NativeWindow window = NativeWindow.FromHandle(control.Handle);
            window.DefWndProc(ref msgResumeUpdate);

            control.Invalidate();
        }

        public void ChangeCurrentThumbnail(CoreGroup group, int index)
        {
            if (OnCurrentThumbnailChanged != null)
                OnCurrentThumbnailChanged(group, index);
        }

        public bool Rename(CoreGroup group, int index, string newFileName)
        {
            if(m_core.Rename(group.id, index, newFileName))
            {
                UpdateGroups();
                return true;
            }
            return false;
        }
    }
}
