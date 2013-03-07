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
using System.IO;
using System.Windows.Forms;

namespace AntiDupl.NET
{
    public class CoreOptions
    {
        public CoreSearchOptions searchOptions;
        public CoreCheckOptions checkOptions;
        public CoreAdvancedOptions advancedOptions;

        public string[] searchPath;
        public string[] ignorePath;
        public string[] validPath;
        public string[] deletePath;

        public CoreOptions()
        {
            searchOptions = new CoreSearchOptions();
            checkOptions = new CoreCheckOptions();
            advancedOptions = new CoreAdvancedOptions();
            searchPath = new string[1];
            ignorePath = new string[0];
            validPath = new string[0];
            deletePath = new string[0];
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
            checkOptions = options.checkOptions.Clone();
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

            ignorePath = new string[1];
            ignorePath[0] = Resources.DataPath;
        }

        public void Get(CoreLib core, bool onePath)
        {
            searchOptions = core.searchOptions.Clone();
            checkOptions = core.checkOptions.Clone();
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
            core.checkOptions = checkOptions.Clone();
            core.advancedOptions = advancedOptions.Clone();
            if (onePath)
            {
                string[] tmpSearch = new string[1];
                string[] tmpOther = new string[0];
                if (searchPath.Length > 0 && Directory.Exists(searchPath[0]))
                    tmpSearch[0] = searchPath[0];
                else
                    tmpSearch[0] = Application.StartupPath;
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
            options.checkOptions = checkOptions.Clone();
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

        public bool Equals(CoreOptions options)
        {
            if (!searchOptions.Equals(options.searchOptions))
                return false;
            if (!checkOptions.Equals(options.checkOptions))
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
    }
}
