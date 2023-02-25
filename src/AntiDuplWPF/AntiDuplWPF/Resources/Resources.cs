/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2023 Yermalayeu Ihar.
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
using System.Collections; 
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Diagnostics;
using System.ComponentModel;

namespace AntiDuplWPF.Resources
{
    static public class Resources
    {
        static private string GetPath(string path, string name, string extension)
        {
            return string.Format("{0}\\{1}{2}", path, name, extension);
        }

        static private string CreateIfNotExists(string path)
        {
            DirectoryInfo directoryInfo = new DirectoryInfo(path);
            if (!directoryInfo.Exists)
                directoryInfo.Create();
            return path;
        }

        static public string GetDefaultUserPath()
        {
            return CreateIfNotExists(string.Format("{0}\\user", Application.StartupPath));
        }

        static private string m_userPath = null;
        static public string UserPath { get { return m_userPath; } set { m_userPath = value; } }

        static public string ProfilesPath { get { return CreateIfNotExists(string.Format("{0}\\profiles", UserPath)); } }

        static public string DataPath { get { return string.Format("{0}\\data", Application.StartupPath); } }

        static public string Path { get { return string.Format("{0}\\resources", DataPath); } }

    }
}

