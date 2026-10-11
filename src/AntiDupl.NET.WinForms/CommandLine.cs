/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// The program's command line:
    ///   -s &lt;folder&gt;                 the folder for settings and data;
    ///   -search &lt;folder&gt; [&lt;folder&gt; ...] search these folders, with their
    ///                              subfolders, instead of the profile's,
    ///                              for this run only;
    ///   -ignore &lt;folder&gt; [&lt;folder&gt; ...] also ignore these folders, with
    ///                              their subfolders, for this run only (in
    ///                              addition to the profile's ignore list);
    ///   -start                     start the search when the window opens.
    /// Other arguments are ignored.
    /// </summary>
    public class CommandLine
    {
        public string UserPath;
        /// <summary>Null when -search is absent.</summary>
        public string[] SearchPaths;
        /// <summary>Null when -ignore is absent.</summary>
        public string[] IgnorePaths;
        public bool StartSearch;

        private static readonly string[] OPTIONS = { "-s", "-search", "-ignore", "-start" };

        static public CommandLine Parse(string[] args)
        {
            CommandLine commandLine = new CommandLine();
            for (int i = 0; i < args.Length; i++)
            {
                switch (args[i])
                {
                    case "-s":
                        if (i + 1 < args.Length)
                            commandLine.UserPath = args[++i];
                        break;
                    case "-search":
                        commandLine.SearchPaths = Folders(args, ref i);
                        break;
                    case "-ignore":
                        commandLine.IgnorePaths = Folders(args, ref i);
                        break;
                    case "-start":
                        commandLine.StartSearch = true;
                        break;
                }
            }
            return commandLine;
        }

        // The arguments after args[i] up to the next option; i ends on the last one taken.
        static private string[] Folders(string[] args, ref int i)
        {
            List<string> paths = new List<string>();
            while (i + 1 < args.Length && Array.IndexOf(OPTIONS, args[i + 1]) < 0)
                paths.Add(args[++i]);
            return paths.ToArray();
        }
    }
}
