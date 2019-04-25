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
using System.IO;

namespace AntiDupl.NET
{
    /// <summary>
    /// Setting for table out (ListView).
    /// Установки для таблицы вывода.
    /// </summary>
    public class ResultsOptions
    {
        private const int DEFAULT_THUMBNAIL_WIDTH_MAX = 128;
        private const int DEFAULT_THUMBNAIL_HEIGHT_MAX = 128;

        public enum ViewMode
        {
            VerticalPairTable,
            HorizontalPairTable,
            GroupedThumbnails
        }

        public delegate void ViewModeChangeHandler(ViewMode viewMode);
        public event ViewModeChangeHandler OnViewModeChange;
        private ViewMode m_viewMode = ViewMode.VerticalPairTable;
        public ViewMode viewMode
        {
            get
            {
                return m_viewMode;
            }
            set
            {
                if (m_viewMode != value)
                {
                    m_viewMode = value;
                    if (OnViewModeChange != null)
                        OnViewModeChange(m_viewMode);
                }
            }
        }

        public bool IsPairTableView()
        {
            return (m_viewMode == ViewMode.VerticalPairTable || m_viewMode == ViewMode.HorizontalPairTable);
        }

        public delegate void ImageViewChangeHandler();
        public event ImageViewChangeHandler OnImageViewChange;
        private bool m_stretchSmallImages = false;
        public bool StretchSmallImages
        {
            get
            {
                return m_stretchSmallImages;
            }
            set
            {
                if (m_stretchSmallImages != value)
                {
                    m_stretchSmallImages = value;
                    if (OnImageViewChange != null)
                        OnImageViewChange();
                }
            }
        }
        private bool m_proportionalImageSize = true;
        public bool ProportionalImageSize
        {
            get
            {
                return m_proportionalImageSize;
            }
            set
            {
                if (m_proportionalImageSize != value)
                {
                    m_proportionalImageSize = value;
                    if (OnImageViewChange != null)
                        OnImageViewChange();
                }
            }
        }

        public delegate void HighlightDifferenceChangeHandler();
        public event HighlightDifferenceChangeHandler OnHighlightDifferenceChange;
        public void RaiseEventOnHighlightDifferenceChange()
        {
            if (OnHighlightDifferenceChange != null)
                OnHighlightDifferenceChange();
        }

        private bool m_highlightDifference = false;
        public bool HighlightDifference
        {
            get
            {
                return m_highlightDifference;
            }
            set
            {
                if (m_highlightDifference != value)
                {
                    m_highlightDifference = value;
                }
            }
        }

        private float m_differenceThreshold = 99.90F;
        public float DifferenceThreshold
        {
            get
            {
                return m_differenceThreshold;
            }
            set
            {
                if (m_differenceThreshold != value)
                {
                    m_differenceThreshold = value;
                }
            }
        }


        private bool m_notHighlightIfFragmentsMoreThan = true;
        public bool NotHighlightIfFragmentsMoreThan
        {
            get
            {
                return m_notHighlightIfFragmentsMoreThan;
            }
            set
            {
                if (m_notHighlightIfFragmentsMoreThan != value)
                {
                    m_notHighlightIfFragmentsMoreThan = value;
                }
            }
        }

        private int m_notHighlightMaxFragments = 32;
        public int NotHighlightMaxFragments
        {
            get
            {
                return m_notHighlightMaxFragments;
            }
            set
            {
                if (m_notHighlightMaxFragments != value)
                {
                    m_notHighlightMaxFragments = value;
                }
            }
        }

        private bool m_highlightAllDifferences = true;
        public bool HighlightAllDifferences
        {
            get
            {
                return m_highlightAllDifferences;
            }
            set
            {
                if (m_highlightAllDifferences != value)
                {
                    m_highlightAllDifferences = value;
                }
            }
        }

        private int m_maxFragmentsForHighlight = 10;
        public int MaxFragmentsForHighlight
        {
            get
            {
                return m_maxFragmentsForHighlight;
            }
            set
            {
                if (m_maxFragmentsForHighlight != value)
                {
                    m_maxFragmentsForHighlight = value;
                }
            }
        }

        private int m_amountOfFragmentsOnX = 16;
        public int AmountOfFragmentsOnX
        {
            get
            {
                return m_amountOfFragmentsOnX;
            }
            set
            {
                if (m_amountOfFragmentsOnX != value)
                {
                    m_amountOfFragmentsOnX = value;
                }
            }
        }

        private int m_amountOfFragmentsOnY = 16;
        public int AmountOfFragmentsOnY
        {
            get
            {
                return m_amountOfFragmentsOnY;
            }
            set
            {
                if (m_amountOfFragmentsOnY != value)
                {
                    m_amountOfFragmentsOnY = value;
                }
            }
        }

        private int m_normalizedSizeOfImage = 512;
        public int NormalizedSizeOfImage
        {
            get
            {
                return m_normalizedSizeOfImage;
            }
            set
            {
                if (m_normalizedSizeOfImage != value)
                {
                    m_normalizedSizeOfImage = value;
                }
            }
        }

        private int m_penThickness = 2;
        public int PenThickness
        {
            get
            {
                return m_penThickness;
            }
            set
            {
                if (m_penThickness != value)
                {
                    m_penThickness = value;
                }
            }
        }


        private bool m_showNeighboursImages = false;
        public bool ShowNeighboursImages
        {
            get
            {
                return m_showNeighboursImages;
            }
            set
            {
                if (m_showNeighboursImages != value)
                {
                    m_showNeighboursImages = value;
                    if (OnImageViewChange != null)
                        OnImageViewChange();
                }
            }
        }

        public struct ColumnOptions
        {
            public bool visible;
            public int width;
            public int order;
        };
        public ColumnOptions[] columnOptionsVertical;
        public ColumnOptions[] columnOptionsHorizontal;

        public int sortTypeDefault;
        public bool increasingDefault;

        public int splitterDistanceVerticalMaximized;
        public int splitterDistanceVerticalNormal;
        public int splitterDistanceHorizontalMaximized;
        public int splitterDistanceHorizontalNormal;

        public System.Drawing.Size thumbnailSizeMax = new System.Drawing.Size(DEFAULT_THUMBNAIL_WIDTH_MAX, DEFAULT_THUMBNAIL_HEIGHT_MAX);

        public ResultsOptions Clone()
        {
            return new ResultsOptions(this);
        }

        public ResultsOptions(ResultsOptions options)
        {
            columnOptionsVertical = new ColumnOptions[(int)ResultsListView.ColumnsTypeVertical.Size];
            for (int i = 0; i < columnOptionsVertical.Length; i++)
                columnOptionsVertical[i] = options.columnOptionsVertical[i];
            columnOptionsHorizontal = new ColumnOptions[(int)ResultsListView.ColumnsTypeHorizontal.Size];
            for (int i = 0; i < columnOptionsHorizontal.Length; i++)
                columnOptionsHorizontal[i] = options.columnOptionsHorizontal[i];
            sortTypeDefault = options.sortTypeDefault;
            increasingDefault = options.increasingDefault;
            splitterDistanceVerticalMaximized = options.splitterDistanceVerticalMaximized;
            splitterDistanceVerticalNormal = options.splitterDistanceVerticalNormal;
            splitterDistanceHorizontalMaximized = options.splitterDistanceHorizontalMaximized;
            splitterDistanceHorizontalNormal = options.splitterDistanceHorizontalNormal;
            thumbnailSizeMax = options.thumbnailSizeMax;

            m_highlightDifference = options.HighlightDifference;
            m_differenceThreshold = options.DifferenceThreshold;
            m_notHighlightIfFragmentsMoreThan = options.NotHighlightIfFragmentsMoreThan;
            m_notHighlightMaxFragments = options.NotHighlightMaxFragments;
            m_highlightAllDifferences = options.HighlightAllDifferences;
            m_maxFragmentsForHighlight = options.MaxFragmentsForHighlight;
            m_amountOfFragmentsOnX = options.AmountOfFragmentsOnX;
            m_amountOfFragmentsOnY = options.AmountOfFragmentsOnY;
            m_normalizedSizeOfImage = options.NormalizedSizeOfImage;
            m_penThickness = options.PenThickness;
        }

        public ResultsOptions()
        {
            columnOptionsVertical = new ColumnOptions[(int)ResultsListView.ColumnsTypeVertical.Size];
            columnOptionsHorizontal = new ColumnOptions[(int)ResultsListView.ColumnsTypeHorizontal.Size];
            SetDefault();
        }

        public void CopyTo(ref ResultsOptions options)
        {
            for (int i = 0; i < columnOptionsVertical.Length; i++)
                options.columnOptionsVertical[i] = columnOptionsVertical[i];
            for (int i = 0; i < columnOptionsHorizontal.Length; i++)
                options.columnOptionsHorizontal[i] = columnOptionsHorizontal[i];
            options.sortTypeDefault = sortTypeDefault;
            options.increasingDefault = increasingDefault;
            options.splitterDistanceVerticalMaximized = splitterDistanceVerticalMaximized;
            options.splitterDistanceVerticalNormal = splitterDistanceVerticalNormal;
            options.splitterDistanceHorizontalMaximized = splitterDistanceHorizontalMaximized;
            options.splitterDistanceHorizontalNormal = splitterDistanceHorizontalNormal;
            options.thumbnailSizeMax = thumbnailSizeMax;
        }

        public bool Equals(ResultsOptions options)
        {
            for (int i = 0; i < columnOptionsVertical.Length; i++)
                if (!Equals(columnOptionsVertical[i], options.columnOptionsVertical[i]))
                    return false;
            for (int i = 0; i < columnOptionsHorizontal.Length; i++)
                if (!Equals(columnOptionsHorizontal[i], options.columnOptionsHorizontal[i]))
                    return false;
            if (sortTypeDefault != options.sortTypeDefault)
                return false;
            if (increasingDefault != options.increasingDefault)
                return false;
            if (splitterDistanceVerticalMaximized != options.splitterDistanceVerticalMaximized)
                return false;
            if (splitterDistanceVerticalNormal != options.splitterDistanceVerticalNormal)
                return false;
            if (splitterDistanceHorizontalMaximized != options.splitterDistanceHorizontalMaximized)
                return false;
            if (splitterDistanceHorizontalNormal != options.splitterDistanceHorizontalNormal)
                return false;
            if (thumbnailSizeMax != options.thumbnailSizeMax)
                return false;

            return true;
        }

        public void SetDefault()
        {
            sortTypeDefault = (int)CoreDll.SortType.ByDifference;
            increasingDefault = true;

            splitterDistanceVerticalMaximized = MainSplitContainer.VIEW_MIN_WIDTH;
            splitterDistanceVerticalNormal = MainSplitContainer.VIEW_MIN_WIDTH;
            splitterDistanceHorizontalMaximized = MainSplitContainer.VIEW_MIN_HEIGHT;
            splitterDistanceHorizontalNormal = MainSplitContainer.VIEW_MIN_HEIGHT;

            thumbnailSizeMax = new System.Drawing.Size(DEFAULT_THUMBNAIL_WIDTH_MAX, DEFAULT_THUMBNAIL_HEIGHT_MAX);

            m_viewMode = ViewMode.VerticalPairTable;

            SetDefaultVerticalColumns();
            SetDefaultHorizontalColumns();
        }

        public void Check()
        {
            if (columnOptionsVertical.Length < (int)ResultsListView.ColumnsTypeVertical.Size ||
                columnOptionsHorizontal.Length < (int)ResultsListView.ColumnsTypeHorizontal.Size)
            {
                ResultsOptions options = new ResultsOptions();
                if (columnOptionsVertical.Length < options.columnOptionsVertical.Length)
                {
                    for (int i = 0; i < columnOptionsVertical.Length; ++i)
                    {
                        options.columnOptionsVertical[i].visible = columnOptionsVertical[i].visible;
                        options.columnOptionsVertical[i].width = columnOptionsVertical[i].width;
                        options.columnOptionsVertical[i].order = columnOptionsVertical[i].order;
                    }
                    columnOptionsVertical = options.columnOptionsVertical;
                }
                if (columnOptionsHorizontal.Length < options.columnOptionsHorizontal.Length)
                {
                    for (int i = 0; i < columnOptionsHorizontal.Length; ++i)
                    {
                        options.columnOptionsHorizontal[i].visible = columnOptionsHorizontal[i].visible;
                        options.columnOptionsHorizontal[i].width = columnOptionsHorizontal[i].width;
                        options.columnOptionsHorizontal[i].order = columnOptionsHorizontal[i].order;
                    }
                    columnOptionsHorizontal = options.columnOptionsHorizontal;
                }
            }
        }

        private void SetDefaultVerticalColumns()
        {
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Type].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Type].width = 35;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Type].order = 0;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Group].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Group].width = 40;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Group].order = 1;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Difference].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Difference].width = 40;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Difference].order = 2;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Defect].visible = false;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Defect].width = 25;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Defect].order = 3;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Transform].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Transform].width = 35;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Transform].order = 4;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Hint].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Hint].width = 30;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Hint].order = 5;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileName].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileName].width = 100;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileName].order = 6;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileDirectory].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileDirectory].width = 230;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileDirectory].order = 7;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageSize].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageSize].width = 70;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageSize].order = 8;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageType].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageType].width = 40;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.ImageType].order = 9;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blockiness].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blockiness].width = 55;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blockiness].order = 10;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blurring].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blurring].width = 55;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.Blurring].order = 11;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileSize].visible = true;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileSize].width = 55;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileSize].order = 12;

            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileTime].visible = false;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileTime].width = 115;
            columnOptionsVertical[(int)ResultsListView.ColumnsTypeVertical.FileTime].order = 13;
        }

        private void SetDefaultHorizontalColumns()
        {
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Type].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Type].width = 35;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Type].order = 0;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Group].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Group].width = 40;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Group].order = 1;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Difference].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Difference].width = 40;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Difference].order = 2;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Defect].visible = false;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Defect].width = 25;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Defect].order = 3;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Transform].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Transform].width = 35;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Transform].order = 4;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Hint].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Hint].width = 30;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.Hint].order = 5;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].width = 230;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileDirectory].order = 7;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].width = 70;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageSize].order = 8;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].width = 40;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstImageType].order = 9;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlockiness].order = 10;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstBlurring].order = 11;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileSize].order = 12;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].visible = false;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].width = 115;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.FirstFileTime].order = 13;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].width = 230;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileDirectory].order = 14;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].width = 70;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageSize].order = 15;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].width = 40;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondImageType].order = 16;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlockiness].order = 17;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondBlurring].order = 18;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].visible = true;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].width = 55;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileSize].order = 19;

            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].visible = false;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].width = 115;
            columnOptionsHorizontal[(int)ResultsListView.ColumnsTypeHorizontal.SecondFileTime].order = 20;
        }

    }
}
