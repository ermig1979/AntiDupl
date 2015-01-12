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

using TypeVertical = AntiDupl.NET.ResultsListView.ColumnsTypeVertical;
using TypeHorizontal = AntiDupl.NET.ResultsListView.ColumnsTypeHorizontal;

namespace AntiDupl.NET
{
    public class ClipboardContentBuilder
    {
        private StringBuilder m_builder;
        private ResultsOptions m_options;
        private bool m_insertTab;

        public ClipboardContentBuilder(ResultsOptions options)
        {
            m_options = options;
            m_builder = new StringBuilder();
        }

        public void Add(CoreResult result)
        {
            m_insertTab = false;
            switch (m_options.viewMode)
            {
                case ResultsOptions.ViewMode.VerticalPairTable:
                    AddCommon(result, m_options.columnOptionsVertical);
                    AddVertical(result, m_options.columnOptionsVertical);
                    break;
                case ResultsOptions.ViewMode.HorizontalPairTable:
                    AddCommon(result, m_options.columnOptionsHorizontal);
                    AddHorizontal(result, m_options.columnOptionsHorizontal);
                    break;
            }
            m_builder.AppendLine("");
        }

        public override string ToString()
        {
            return m_builder.ToString();
        }

        private void AddCommon(CoreResult result, ResultsOptions.ColumnOptions[] options)
        {
            if (options[(int)TypeVertical.Type].visible)
                Append(result.type);
            if (options[(int)TypeVertical.Group].visible)
                Append(result.group);
            if (options[(int)TypeVertical.Difference].visible)
                Append(result.difference.ToString("F2"));
            if (options[(int)TypeVertical.Defect].visible)
                Append(result.defect);
            if (options[(int)TypeVertical.Transform].visible)
                Append(result.transform);
            if (options[(int)TypeVertical.Hint].visible)
                Append(result.hint);
        }

        private void AddVertical(CoreResult result, ResultsOptions.ColumnOptions[] options)
        {
            if (options[(int)TypeVertical.FileName].visible ||
                options[(int)TypeVertical.FileDirectory].visible)
                Append(result.first.path);
            if (options[(int)TypeVertical.ImageSize].visible)
                Append(result.first.GetImageSizeString());
            if (options[(int)TypeVertical.ImageType].visible)
                Append(result.first.GetImageTypeString());
            if (options[(int)TypeVertical.FileSize].visible)
                Append(result.first.GetFileSizeString());
            if (options[(int)TypeVertical.FileTime].visible)
                Append(result.first.GetFileTimeString());

            if (options[(int)TypeVertical.FileName].visible ||
                options[(int)TypeVertical.FileDirectory].visible)
                Append(result.second.path);
            if (options[(int)TypeVertical.ImageSize].visible)
                Append(result.second.GetImageSizeString());
            if (options[(int)TypeVertical.ImageType].visible)
                Append(result.second.GetImageTypeString());
            if (options[(int)TypeVertical.FileSize].visible)
                Append(result.second.GetFileSizeString());
            if (options[(int)TypeVertical.FileTime].visible)
                Append(result.second.GetFileTimeString());
        }

        private void AddHorizontal(CoreResult result, ResultsOptions.ColumnOptions[] options)
        {
            if (options[(int)TypeHorizontal.FirstFileName].visible ||
                options[(int)TypeHorizontal.FirstFileDirectory].visible)
                Append(result.first.path);
            if (options[(int)TypeHorizontal.FirstImageSize].visible)
                Append(result.first.GetImageSizeString());
            if (options[(int)TypeHorizontal.FirstImageType].visible)
                Append(result.first.GetImageTypeString());
            if (options[(int)TypeHorizontal.FirstFileSize].visible)
                Append(result.first.GetFileSizeString());
            if (options[(int)TypeHorizontal.FirstFileTime].visible)
                Append(result.first.GetFileTimeString());

            if (options[(int)TypeHorizontal.SecondFileName].visible ||
                options[(int)TypeHorizontal.SecondFileDirectory].visible)
                Append(result.second.path);
            if (options[(int)TypeHorizontal.SecondImageSize].visible)
                Append(result.second.GetImageSizeString());
            if (options[(int)TypeHorizontal.SecondImageType].visible)
                Append(result.second.GetImageTypeString());
            if (options[(int)TypeHorizontal.SecondFileSize].visible)
                Append(result.second.GetFileSizeString());
            if (options[(int)TypeHorizontal.SecondFileTime].visible)
                Append(result.second.GetFileTimeString());
        }

        private void Append(object value)
        {
            if (m_insertTab)
                m_builder.Append("\t");
            m_builder.Append(value.ToString());
            m_insertTab = true;
        }
    }
}
