/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
using System.IO;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// A settings file that could not be read. The defaults used instead are
    /// saved over it when the program closes, so a copy "file.bad" is kept
    /// and the user is told.
    /// </summary>
    public class UnreadableSettingsFile
    {
        public readonly string FileName;
        /// <summary>The kept copy, or null if copying failed.</summary>
        public readonly string CopyFileName;
        public readonly string Reason;

        public UnreadableSettingsFile(string fileName, Exception exception)
        {
            FileName = fileName;
            Reason = exception.InnerException != null ?
                exception.Message + " " + exception.InnerException.Message : exception.Message;
            try
            {
                File.Copy(fileName, fileName + ".bad", true);
                CopyFileName = fileName + ".bad";
            }
            catch (Exception)
            {
                CopyFileName = null;
            }
        }

        public void Show(IWin32Window owner)
        {
            Strings strings = Resources.Strings.Current;
            string text = CopyFileName != null ?
                string.Format(strings.ErrorMessage_SettingsFileUnreadable, FileName, Reason, CopyFileName) :
                string.Format(strings.ErrorMessage_SettingsFileUnreadableNotCopied, FileName, Reason);
            MessageBox.Show(owner, text, Resources.ProductName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
        }
    }
}
