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

namespace AntiDupl.NET
{
    /// <summary>
    /// Панель содеражшая или превью дефекта или дубликатов.
    /// </summary>
    public class ResultsPreviewContainer : Panel
    {
        private enum State
        {
            Empty,
            Defect,
            DuplPair
        };
        private State m_state = State.Empty;

        private ResultsPreviewDefect m_resultsPreviewDefect;
        private ResultsPreviewDuplPair m_resultsPreviewDuplPair;
        private MainSplitContainer m_mainSplitContainer;

        public ResultsPreviewContainer(CoreLib core, Options options, CoreOptions coreOptions, MainSplitContainer mainSplitContainer)
        {
            Location = new Point(0, 0);
            Dock = DockStyle.Fill;

            m_mainSplitContainer = mainSplitContainer;
            m_resultsPreviewDefect = new ResultsPreviewDefect(core, options, coreOptions, m_mainSplitContainer.resultsListView);
            m_resultsPreviewDefect.Location = new System.Drawing.Point(0, 0);
            m_resultsPreviewDefect.Dock = DockStyle.Fill;

            m_resultsPreviewDuplPair = new ResultsPreviewDuplPair(core, options, coreOptions, m_mainSplitContainer.resultsListView);
            m_resultsPreviewDuplPair.Location = new System.Drawing.Point(0, 0);
            m_resultsPreviewDuplPair.Dock = DockStyle.Fill;

            m_mainSplitContainer.OnCurrentResultChanged += 
                new MainSplitContainer.CurrentResultChangedHandler(CurrentResultChanged);
        }

        private void CurrentResultChanged()
        {
            CoreResult result = m_mainSplitContainer.resultsListView.GetCurrentResult();
            State state = State.Empty;
            if (result != null)
            {
                switch (result.type)
                {
                    case CoreDll.ResultType.DefectImage:
                        state = State.Defect;
                        break;
                    case CoreDll.ResultType.DuplImagePair:
                        state = State.DuplPair;
                        break;
                }
            }

            switch (state)
            {
                case State.Empty:
                    break;
                case State.Defect:
                    m_resultsPreviewDefect.SetCurrentSearchResult(result);
                    break;
                case State.DuplPair:
                    m_resultsPreviewDuplPair.SetCurrentSearchResult(result);
                    break;
            }

            if(state != m_state)
            {
                m_state = state;

                m_mainSplitContainer.OnCurrentResultChanged -= new MainSplitContainer.CurrentResultChangedHandler(CurrentResultChanged);
                Controls.Clear(); //Очишаем панель
                m_mainSplitContainer.OnCurrentResultChanged += new MainSplitContainer.CurrentResultChangedHandler(CurrentResultChanged);

                switch (m_state)
                {
                    case State.Empty:
                        break;
                    case State.Defect:
                        Controls.Add(m_resultsPreviewDefect);
                        break;
                    case State.DuplPair:
                        Controls.Add(m_resultsPreviewDuplPair);
                        break;
                }
            }
        }

        public void SetViewMode(ResultsOptions.ViewMode viewMode)
        {
            m_resultsPreviewDefect.SetViewMode(viewMode);
            m_resultsPreviewDuplPair.SetViewMode(viewMode);
        }
    }
}
