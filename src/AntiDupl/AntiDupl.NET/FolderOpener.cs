/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2013 Yermalayeu Ihar.
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

namespace AntiDupl.NET
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

        static public void OpenContainingFolder(CoreImageInfo imageInfo)
        {
            try
            {
                if(m_canOpenFolderWithExplorer)
                {
                    Process.Start("explorer.exe", string.Format("/e, /select, \"{0}\"", imageInfo.path));
                }
                else
                {
                    ProcessStartInfo startInfo = new ProcessStartInfo();
                    startInfo.FileName = imageInfo.GetDirectoryString();
                    Process.Start(startInfo);
                }
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }
    }
}
