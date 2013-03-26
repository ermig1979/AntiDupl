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
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.Windows.Forms;

namespace AntiDupl.NET
{
    public class Options
    {
        public delegate void VisibleChangeHandler(bool visible);

        public delegate void ChangeHandler();
        public event ChangeHandler OnChange;
        public void Change()
        {
            if (OnChange != null)
                OnChange();
        }

        private string m_language = StringsDefaultEnglish.Get().Name;
        public string Language
        {
            get
            {
                return m_language;
            }
            set
            {
                if (m_language != value)
                {
                    m_language = value;
                    Resources.Strings.SetCurrent(m_language);
                }
            }
        }

        public bool onePath = false;
        public bool checkingForUpdates = true;
        public bool useImageDataBase = true;
        public bool checkResultsAtLoading = true;

        public MainFormOptions mainFormOptions;
        public ResultsOptions resultsOptions;
        public HotKeyOptions hotKeyOptions;

        public CoreOptions coreOptions;

        static public Options Load(CoreLib core)
        {
            FileInfo fileInfo = new FileInfo(Options.GetOptionsFileName());
            if (fileInfo.Exists)
            {
                try
                {
                    XmlSerializer xmlSerializer = new XmlSerializer(typeof(Options));
                    FileStream fileStream = new FileStream(Options.GetOptionsFileName(), FileMode.Open);
                    Options options = (Options)xmlSerializer.Deserialize(fileStream);
                    fileStream.Close();
                    options.coreOptions.Validate(core, options.onePath);
                    return options;
                }
                catch
                {
                    return new Options(core);
                }
            }
            else
                return new Options(core);
        }

        public Options()
        {
            resultsOptions = new ResultsOptions();
            mainFormOptions = new MainFormOptions();
            hotKeyOptions = new HotKeyOptions();
            coreOptions = new CoreOptions();
        }

        public Options(CoreLib core)
        {
            resultsOptions = new ResultsOptions();
            mainFormOptions = new MainFormOptions();
            hotKeyOptions = new HotKeyOptions();
            coreOptions = new CoreOptions(core);
        }

        public Options(Options options)
        {
            resultsOptions = new ResultsOptions(options.resultsOptions);
            mainFormOptions = new MainFormOptions(options.mainFormOptions);
            hotKeyOptions = new HotKeyOptions(options.hotKeyOptions);
            coreOptions = new CoreOptions(options.coreOptions);
            
            Language = options.Language;
            onePath = options.onePath;
            checkingForUpdates = options.checkingForUpdates;
            useImageDataBase = options.useImageDataBase;
            checkResultsAtLoading = options.checkResultsAtLoading;
        }

        public void Save()
        {
            try
            {
                TextWriter writer = new StreamWriter(Options.GetOptionsFileName());
                XmlSerializer xmlSerializer = new XmlSerializer(typeof(Options));
                xmlSerializer.Serialize(writer, this);
            }
            catch
            {
            }
        }
 
        public Options Clone()
        {
            return new Options(this);
        }

        public void CopyTo(ref Options options)
        {
            resultsOptions.CopyTo(ref options.resultsOptions);
            mainFormOptions.CopyTo(ref options.mainFormOptions);
            hotKeyOptions.CopyTo(ref options.hotKeyOptions);
            coreOptions.CopyTo(ref options.coreOptions);

            options.Language = Language;
            options.onePath = onePath;
            options.checkingForUpdates = checkingForUpdates;
            options.useImageDataBase = useImageDataBase;
            options.checkResultsAtLoading = checkResultsAtLoading;
        }

        public static void PathCopy(string[] source, ref string[] destination)
        {
            destination = new string[source.GetLength(0)];
            for (int i = 0; i < source.GetLength(0); ++i)
                destination[i] = (string)source[i].Clone();
        }

        public static string[] PathClone(string[] path)
        {
            string[] clone = new string[0];
            PathCopy(path, ref clone);
            return clone;
        }

        public static bool Equals(string[] path1, string[] path2)
        {
            if (path1.Length != path2.Length)
                return false;
            for (int i = 0; i < path1.Length; ++i)
                if (path1[i].CompareTo(path2[i]) != 0)
                    return false;
            return true;
        }

        static public string GetOptionsFileName()
        {
            StringBuilder builder = new StringBuilder();
            builder.Append(Resources.UserPath);
            builder.Append("\\options.xml");
            return builder.ToString();
        }

        static public string GetMistakeDataBaseFileName()
        {
            StringBuilder builder = new StringBuilder();
            builder.Append(Resources.UserPath);
            builder.Append("\\mistakes.adm");
            return builder.ToString();
        }

        public string GetImageDataBasePath()
        {
            string directory = string.Format("{0}\\images\\{1}x{1}", Resources.UserPath, coreOptions.advancedOptions.reducedImageSize);
            DirectoryInfo directoryInfo = new DirectoryInfo(directory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();
            return directory;
        }

        public bool Equals(Options options)
        {
            if (checkingForUpdates != options.checkingForUpdates)
                return false;
            if (onePath != options.onePath)
                return false;
            if (useImageDataBase != options.useImageDataBase)
                return false;
            if (checkResultsAtLoading != options.checkResultsAtLoading)
                return false;
            if (m_language != options.m_language)
                return false;
            if (!resultsOptions.Equals(options.resultsOptions))
                return false;
            if (!mainFormOptions.Equals(options.mainFormOptions))
                return false;
            if (!hotKeyOptions.Equals(options.hotKeyOptions))
                return false;
            if (!coreOptions.Equals(options.coreOptions))
                return false;
            return true;
        }
    }
}
