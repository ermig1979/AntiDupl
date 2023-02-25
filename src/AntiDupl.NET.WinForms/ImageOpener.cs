using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    static public class ImageOpener
    {
        /// <summary>
        /// Opens the specified file using its default application.
        /// </summary>
        static public void OpenFile(string filePath)
        {
            ProcessStartInfo startInfo = new ProcessStartInfo();
            startInfo.UseShellExecute = true;
            startInfo.FileName = filePath;
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
