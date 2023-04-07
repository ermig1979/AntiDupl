using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    static public class ImageDiffOpener
    {
        /// <summary>
        /// Opens the specified file using its default application.
        /// </summary>
        static public void OpenFiles(string filePathFirstImage, string filePathSecondImage)
        {
            string ExecutablePath = "C:\\Program Files\\WinMerge\\WinMergeU.exe";
            string ExecutableArguments = "%1 %2";

            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.FileName = ExecutablePath;
            startInfo.Arguments = ExecutableArguments.Replace("%1",filePathFirstImage).Replace("%2",filePathSecondImage);
            startInfo.WindowStyle = ProcessWindowStyle.Maximized;
            try
            {
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
