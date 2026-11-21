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
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;
using System.Diagnostics;
using System.Windows.Forms;
using System.Threading;
using System.Runtime.InteropServices;

using AntiDupl.NET.Core;

namespace AntiDupl.NET.WinForms
{
    static public class FolderOpener
    {
        static private bool m_canOpenFolderWithExplorer = CanOpenFolderWithExplorer();

        static private bool CanOpenFolderWithExplorer()
        {
            RegistryKey rkFolder = Registry.ClassesRoot.OpenSubKey("Folder");
            if (rkFolder != null)
            {
                RegistryKey rkShell = rkFolder.OpenSubKey("shell");
                if (rkShell != null)
                {
                    RegistryKey rkExplore = rkShell.OpenSubKey("explore");
                    if (rkExplore != null)
                    {
                        RegistryKey rkCommand = rkExplore.OpenSubKey("command");
                        if (rkCommand != null)
                        {
                            string defaultValue = (string)rkCommand.GetValue("");
                            if (defaultValue != null)
                            {
                                if (defaultValue.ToLowerInvariant().Contains("explorer.exe"))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                    RegistryKey rkOpen = rkShell.OpenSubKey("open");
                    if (rkOpen != null)
                    {
                        RegistryKey rkCommand = rkOpen.OpenSubKey("command");
                        if (rkCommand != null)
                        {
                            string defaultValue = (string)rkCommand.GetValue("");
                            if (defaultValue != null)
                            {
                                if (defaultValue.ToLowerInvariant().Contains("explorer.exe"))
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
            return false;
        }

        [DllImport("shell32.dll", CharSet = CharSet.Unicode)]
        private static extern int SHParseDisplayName(string name, IntPtr bindingContext, out IntPtr pidl, uint attributesIn, out uint attributesOut);

        [DllImport("shell32.dll")]
        private static extern int SHOpenFolderAndSelectItems(IntPtr folder, uint count, IntPtr[] items, uint flags);

        [DllImport("ole32.dll")]
        private static extern void CoTaskMemFree(IntPtr memory);

        // Explorer's /select switch leaves the file unselected when Explorer
        // has not listed its folder yet (a large folder not opened since
        // logon opens at its top); SHOpenFolderAndSelectItems selects it.
        static private bool SelectInExplorer(string path)
        {
            // The shell doesn't parse the \\?\ prefix of long paths.
            if (path.StartsWith(@"\\?\UNC\"))
                path = @"\\" + path.Substring(8);
            else if (path.StartsWith(@"\\?\"))
                path = path.Substring(4);
            IntPtr pidl;
            uint attributes;
            if (SHParseDisplayName(path, IntPtr.Zero, out pidl, 0, out attributes) != 0)
                return false;
            try
            {
                // No items: pidl names the file itself, which gets selected.
                return SHOpenFolderAndSelectItems(pidl, 0, null, 0) == 0;
            }
            finally
            {
                CoTaskMemFree(pidl);
            }
        }

        static public void OpenContainingFolder(CoreImageInfo imageInfo)
        {
            try
            {
                if (m_canOpenFolderWithExplorer && SelectInExplorer(imageInfo.path))
                    return;
                ProcessStartInfo startInfo = new ProcessStartInfo();
                if (m_canOpenFolderWithExplorer)
                {
                    startInfo.FileName = "explorer.exe";
                    startInfo.Arguments = string.Format("/e, /select, \"{0}\"", imageInfo.path);
                }
                else
                {
                    startInfo.FileName = imageInfo.GetDirectoryString();
                }
                var process = Process.Start(startInfo);
                Thread.Sleep(System.TimeSpan.FromMilliseconds(100));
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }
    }
}
