/*
* AntiDupl.NET Program.
*
* Copyright (c) 2002-2015 Yermalayeu Ihar, 2014 Borisov Dmitry.
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
using System.IO;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace AntiDupl.NET
{
    public class CoreOptions
    {
        public CoreSearchOptions searchOptions;
        public CoreCompareOptions compareOptions;
        public CoreDefectOptions defectOptions;
        public CoreAdvancedOptions advancedOptions;

        public CorePathWithSubFolder[] searchPath;
        public CorePathWithSubFolder[] ignorePath;
        public CorePathWithSubFolder[] validPath;
        public CorePathWithSubFolder[] deletePath;

        public CoreOptions()
        {
            searchOptions = new CoreSearchOptions();
            compareOptions = new CoreCompareOptions();
            defectOptions = new CoreDefectOptions();
            advancedOptions = new CoreAdvancedOptions();
            searchPath = new CorePathWithSubFolder[1];
            ignorePath = new CorePathWithSubFolder[0];
            validPath = new CorePathWithSubFolder[0];
            deletePath = new CorePathWithSubFolder[0];
        }

        public CoreOptions(CoreLib core, bool onePath)
            :this()
        {
            SetDefault(core, onePath);
        }

        public CoreOptions(CoreLib core)
            : this(core, false)
        {
        }

        public CoreOptions(CoreOptions options)
        {
            searchOptions = options.searchOptions.Clone();
            compareOptions = options.compareOptions.Clone();
            defectOptions = options.defectOptions.Clone();
            advancedOptions = options.advancedOptions.Clone();
            searchPath = PathClone(options.searchPath);
            ignorePath = PathClone(options.ignorePath);
            validPath = PathClone(options.validPath);
            deletePath = PathClone(options.deletePath);
        }

        public void SetDefault(CoreLib core, bool onePath)
        {
            CoreOptions old = new CoreOptions();
            old.Get(core, onePath);
            core.SetDefaultOptions();
            Get(core, onePath);
            old.Set(core, onePath);

            ignorePath = new CorePathWithSubFolder[1];
            ignorePath[0] = new CorePathWithSubFolder();
            ignorePath[0].path = Resources.DataPath;
        }

        public void Get(CoreLib core, bool onePath)
        {
            searchOptions = core.searchOptions.Clone();
            compareOptions = core.compareOptions.Clone();
            defectOptions = core.defectOptions.Clone();
            advancedOptions = core.advancedOptions.Clone();
            if (onePath)
            {
                searchPath[0] = core.searchPath[0];
            }
            else
            {
                searchPath = core.searchPath;
                ignorePath = core.ignorePath;
                validPath = core.validPath;
                deletePath = core.deletePath;
            }
        }

        public void Set(CoreLib core, bool onePath)
        {
            core.searchOptions = searchOptions.Clone();
            core.compareOptions = compareOptions.Clone();
            core.defectOptions = defectOptions.Clone();
            core.advancedOptions = advancedOptions.Clone();
            if (onePath)
            {
                CorePathWithSubFolder[] tmpSearch = new CorePathWithSubFolder[1];
                CorePathWithSubFolder[] tmpOther = new CorePathWithSubFolder[0];
                if (searchPath.Length > 0 && Directory.Exists(searchPath[0].path))
                    tmpSearch[0] = searchPath[0];
                else
                    tmpSearch[0].path = Application.StartupPath;
                core.searchPath = tmpSearch;
                core.ignorePath = tmpOther;
                core.validPath = tmpOther;
                core.deletePath = tmpOther;
            }
            else
            {
                core.searchPath = searchPath;
                core.ignorePath = ignorePath;
                core.validPath = validPath;
                core.deletePath = deletePath;
            }
        }

        public void Validate(CoreLib core, bool onePath)
        {
            Set(core, onePath);
            Get(core, onePath);
        }

        public CoreOptions Clone()
        {
            return new CoreOptions(this);
        }

        public void CopyTo(ref CoreOptions options)
        {
            options.searchOptions = searchOptions.Clone();
            options.compareOptions = compareOptions.Clone();
            options.defectOptions = defectOptions.Clone();
            options.advancedOptions = advancedOptions.Clone();
            PathCopy(searchPath, ref options.searchPath);
            PathCopy(ignorePath, ref options.ignorePath);
            PathCopy(validPath, ref options.validPath);
            PathCopy(deletePath, ref options.deletePath);
        }

        public static void PathCopy(string[] source, ref string[] destination)
        {
            destination = new string[source.GetLength(0)];
            for (int i = 0; i < source.GetLength(0); ++i)
                destination[i] = (string)source[i].Clone();
        }

        public static void PathCopy(CorePathWithSubFolder[] source, ref CorePathWithSubFolder[] destination)
        {
            destination = new CorePathWithSubFolder[source.GetLength(0)];
            for (int i = 0; i < source.GetLength(0); ++i)
                destination[i] = source[i];
        }

        public static string[] PathClone(string[] path)
        {
            string[] clone = new string[0];
            PathCopy(path, ref clone);
            return clone;
        }

        public static CorePathWithSubFolder[] PathClone(CorePathWithSubFolder[] path)
        {
            CorePathWithSubFolder[] clone = new CorePathWithSubFolder[0];
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

        public bool Equals(CoreOptions options)
        {
            if (!searchOptions.Equals(options.searchOptions))
                return false;
            if (!compareOptions.Equals(options.compareOptions))
                return false;
            if (!defectOptions.Equals(options.defectOptions))
                return false;
            if (!advancedOptions.Equals(options.advancedOptions))
                return false;
            if (!Equals(searchPath, options.searchPath))
                return false;
            if (!Equals(ignorePath, options.ignorePath))
                return false;
            if (!Equals(validPath, options.validPath))
                return false;
            if (!Equals(deletePath, options.deletePath))
                return false;

            return true;
        }

        static public CoreOptions Load(string fileName, CoreLib core, bool onePath)
        {
            FileInfo fileInfo = new FileInfo(fileName);
            if (fileInfo.Exists)
            {
                FileStream fileStream = null;
                try
                {
                    XmlSerializer xmlSerializer = new XmlSerializer(typeof(CoreOptions));
                    fileStream = new FileStream(fileName, FileMode.Open);
                    CoreOptions coreOptions = (CoreOptions)xmlSerializer.Deserialize(fileStream);
                    fileStream.Close();
                    coreOptions.Validate(core, onePath);
                    return coreOptions;
                }
                catch
                {
                    if(fileStream != null)
                        fileStream.Close();
                    return new CoreOptions(core);
                }
            }
            else
                return new CoreOptions(core);
        }

        public void Save(string fileName)
        {
            TextWriter writer = null;
            try
            {
                writer = new StreamWriter(fileName);
                XmlSerializer xmlSerializer = new XmlSerializer(typeof(CoreOptions));
                xmlSerializer.Serialize(writer, this);
            }
            catch
            {
            }
            if (writer != null)
                writer.Close();
        }

        public string GetImageDataBasePath()
        {
            string directory = string.Format("{0}\\images\\{1}x{1}", Resources.UserPath, advancedOptions.reducedImageSize);
            DirectoryInfo directoryInfo = new DirectoryInfo(directory);
            if (!directoryInfo.Exists)
                directoryInfo.Create();
            return directory;
        }
    }
}
