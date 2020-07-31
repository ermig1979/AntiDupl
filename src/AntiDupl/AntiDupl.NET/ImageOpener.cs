using System.Diagnostics;
using System.Windows.Forms;

namespace AntiDupl.NET
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
                Process.Start(startInfo);
            }
            catch (System.Exception exeption)
            {
                MessageBox.Show(exeption.Message);
            }
        }
    }
}
