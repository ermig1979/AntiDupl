using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.Helper
{
    public class DirectoryHelper
    {
        public static string ResultDirectory
        {
            get
            {
                string directory = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Result");
                if (!Directory.Exists(directory))
                    Directory.CreateDirectory(directory);
                return directory;
            }
        }
    }
}
