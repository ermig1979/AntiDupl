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
using System.Windows.Forms;
using System.IO;

namespace AntiDupl.NET
{
    static class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            if (IsDotNet35Installed)
            {
                string customSavePath = null;
                if (GetParameter(args, "-s", ref customSavePath))
                {
                    DirectoryInfo directoryInfo = new DirectoryInfo(customSavePath);
                    if (!directoryInfo.Exists)
                        throw new Exception(String.Format("The directory '{0}' is not exists!", customSavePath));
                    Resources.UserPath = customSavePath;
                }
                else
                {
                    Resources.UserPath = Resources.GetDefaultUserPath();
                }
                Resources.Strings.Initialize();
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainForm());
            }
            else if (MessageBox.Show("You Need Microsoft .NET Framework 3.5 in order to run this program. Want to download .Net Framework 3.5?", "Warning",
                        MessageBoxButtons.YesNo, MessageBoxIcon.Error) == DialogResult.Yes)
                System.Diagnostics.Process.Start("http://www.microsoft.com/ru-ru/download/details.aspx?id=22");
        }

        static bool GetParameter(string[] args, string name, ref string value)
        {
            for(int i = 0; i < args.Length - 1; i++)
            {
                if(String.Compare(args[i], name) == 0)
                {
                    value = args[i + 1];
                    return true;
                }
            }
            return false;
        }

        private static bool IsDotNet35Installed
        {
            get
            {
                try
                {
                    return (Convert.ToInt32(Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\NET Framework Setup\NDP\v3.5").GetValue("Install")) == 1);
                }
                catch
                {
                    return false;
                }
            }
        }

    }
}