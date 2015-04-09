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

namespace AntiDupl.NET
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

        static public class Images
        {
            static public Image GetNullImage()
            {
                Bitmap bitmap = new Bitmap(1, 1);
                bitmap.SetPixel(0, 0, Color.FromArgb(0, 0, 0, 0));
                return bitmap;
            }

            static public Image GetImageWithBlackCircle(int width, int height, double radius)
            {
                Bitmap bitmap = new Bitmap(width, height);
                for (int x = 0; x < width; x++)
                {
                    int xx = x - width / 2;
                    for (int y = 0; y < height; y++)
                    {
                        int yy = y - height / 2;
                        if (xx * xx + yy * yy < radius * radius)
                            bitmap.SetPixel(x, y, Color.FromArgb(0xFF, 0, 0, 0));
                        else
                            bitmap.SetPixel(x, y, Color.FromArgb(0, 0, 0, 0));
                    }
                }
                return bitmap;
            }

            static public Image Get(string name)
            {
                Image image;
                try
                {
                    image = Image.FromFile(GetPath(Path, name, Extension));
                }
                catch
                {
                    image = GetNullImage();
                }
                return image;
            }

            static private string Path { get {return string.Format("{0}\\images", Resources.Path);}}

            static private string Extension { get { return ".img"; } }
        }

        static public class Icons
        {
            static public Icon Get(Size size)
            {
                Icon icon = Get();
                return new Icon(icon, size);
            }

            static public Icon Get()
            {
                Icon icon = null;
                try
                {
                    icon = new Icon(GetPath(Path, "Icon", Extension));
                }
                catch
                {
                    try
                    {
                        icon = Icon.ExtractAssociatedIcon(Application.ExecutablePath);
                    }
                    catch
                    {
                    } 
                }
                return icon;
            }

            static private string Path { get { return string.Format("{0}\\icons", Resources.Path); } }

            static private string Extension { get { return ".ico"; } }
        }
        
        static public class Strings
        {
            public delegate void CurrentChangeHandler();
            /// <summary>
            /// Событие вызываемое при смене языка
            /// </summary>
            public static event CurrentChangeHandler OnCurrentChange;

            static public void Initialize()
            {
                m_strings.Clear();

                m_strings.Add(StringsDefaultEnglish.Get());
                m_strings.Add(StringsDefaultRussian.Get());

                DirectoryInfo directoryInfo = new DirectoryInfo(Path);
                if (directoryInfo.Exists)
                {
                    FileInfo[] fileInfos = directoryInfo.GetFiles(Filter, SearchOption.TopDirectoryOnly);
                    for (int i = 0; i < fileInfos.Length; i++)
                    {
                        AntiDupl.NET.Strings strings = Load(fileInfos[i].FullName);
                        if(strings != null)
                        {
                            string name = System.IO.Path.GetFileNameWithoutExtension(fileInfos[i].FullName);
                            if (name.CompareTo(StringsDefaultRussian.Get().Name) != 0 &&
                                name.CompareTo(StringsDefaultEnglish.Get().Name) != 0)
                            {
                                strings.Name = name;
                                m_strings.Add(strings);
                            }
                        }
                    }
                }

                try
                {
                    Save(StringsDefaultEnglish.Get());
                    Save(StringsDefaultRussian.Get());
                }
                catch(Exception)
                {
                }
            }
            
            static private AntiDupl.NET.Strings Load(string path)
            {
                FileInfo fileInfo = new FileInfo(path);
                if (fileInfo.Exists)
                {
                    try
                    {
                        XmlSerializer xmlSerializer = new XmlSerializer(typeof(AntiDupl.NET.Strings));
                        FileStream fileStream = new FileStream(path, FileMode.Open);
                        AntiDupl.NET.Strings strings = (AntiDupl.NET.Strings)xmlSerializer.Deserialize(fileStream);
                        fileStream.Close();
                        return strings;
                    }
                    catch
                    {
                        return null;
                    }
                }
                else
                    return null;
            }

            static private void Save(AntiDupl.NET.Strings strings)
            {
                try
                {
                    TextWriter writer = new StreamWriter(GetPath(Path, strings.Name, Extension));
                    XmlSerializer xmlSerializer = new XmlSerializer(typeof(AntiDupl.NET.Strings));
                    xmlSerializer.Serialize(writer, strings);
                    writer.Close();
                }
                catch
                {
                }
            }

            static private string Path { get { return string.Format("{0}\\strings", Resources.Path); } }

            static private string Extension { get { return ".xml"; } }

            static private string Filter { get { return "*.xml"; } }

            public static int Count 
            { 
                get 
                {
                    return m_strings.Count;
                }
            }
            
            public static int CurrentIndex
            {
                get
                {
                    return m_currentIndex;
                }
            }
            
            public static AntiDupl.NET.Strings Current
            {
                get 
                {
                    if (m_currentIndex < Count && m_currentIndex >= 0)
                        return (AntiDupl.NET.Strings)m_strings[m_currentIndex];
                    else
                        return null; 
                }
            }
            
            public static AntiDupl.NET.Strings Get(int index)
            {
                if (index < Count && index >= 0)
                    return (AntiDupl.NET.Strings)m_strings[index];
                else
                    return null;
            }

            public static bool SetCurrent(int index)
            {
                if (index != m_currentIndex && index < Count && index >= 0)
                {
                    m_currentIndex = index;
                    if (OnCurrentChange != null)
                        OnCurrentChange();
                    return true;
                }
                else
                    return false;
            }
            
            public static bool SetCurrent(string name)
            {
                for(int i = 0; i < Count; i++)
                {
                    if(Get(i).Name.CompareTo(name) == 0)
                    {
                        return SetCurrent(i);
                    }
                }
                for(int i = 0; i < Count; i++)
                {
                    if(Get(i).Name.CompareTo(StringsDefaultEnglish.Get().Name) == 0)
                    {
                        return SetCurrent(i);
                    }
                }
                return false;
            }

            public static bool IsCurrentRussian()
            {
                return Current.Name.CompareTo(StringsDefaultRussian.Get().Name) == 0;
            }

            public static bool IsCurrentEnglish()
            {
                return Current.Name.CompareTo(StringsDefaultEnglish.Get().Name) == 0;
            }

            public static bool IsCurrentRussianFamily()
            {
                return IsCurrentRussian() ||
                    Current.Name.CompareTo("Belarusian") == 0 ||
                    Current.Name.CompareTo("Ukrainian") == 0;
            }
            
            private static ArrayList m_strings = new ArrayList();
            private static int m_currentIndex = 0;
        }

        static public class WebLinks
        {
            public const string AntiduplSourceforgeNet = "http://antidupl.sourceforge.net/";
            public const string AntiduplSourceforgeNetEnglish = "http://antidupl.sourceforge.net/english/index.html";
            public const string AntiduplSourceforgeNetRussian = "http://antidupl.sourceforge.net/russian/index.html";
            public const string Version = "http://antidupl.sourceforge.net/version.xml";

            public const string SourceforgeNetSimd = "http://sourceforge.net/projects/simd/?source=directory";
            public const string SourceforgeNetAntidupl = "http://sourceforge.net/projects/antidupl/?source=directory";

            public const string OpenjpegOrg = "http://www.openjpeg.org";

            public static string AntiduplNarodRuCurrent
            {
                get
                {
                    if (Strings.IsCurrentRussianFamily())
                        return AntiduplSourceforgeNetRussian;
                    else
                        return AntiduplSourceforgeNetEnglish;
                }
            }
        }

        static public class Help
        {        
            static private string GetUrl(string page)
            {
                Uri uri = new Uri(DataPath);
                StringBuilder builder = new StringBuilder(uri.AbsoluteUri);
                builder.Append("/help");
                if (Strings.IsCurrentRussianFamily())
                    builder.Append("/russian");
                else
                    builder.Append("/english");
                builder.Append("/index.html");
                if(page != null)
                {
                    builder.Append("?page=");
                    builder.Append(page);
                }
                return builder.ToString();
            }

            private class Starter
            {
                private string m_url;

                public Starter(Form form, string url)
                {
                    m_url = url;
                    form.HelpButton = true;
                    form.HelpButtonClicked += new CancelEventHandler(OnHelpButtonClicked);
                }

                private void OnHelpButtonClicked(Object sender, CancelEventArgs e)
                {
                    Help.Show(m_url);
                }
            }

            static public void Show(string url)
            {
                try
                {
                    if (url.Substring(0, 4).ToUpper() != "HTTP")
                    {
                        string keyName = @"HTTP\shell\open\command";
                        Microsoft.Win32.RegistryKey registryKey = Microsoft.Win32.Registry.ClassesRoot.OpenSubKey(keyName, false);
                        if (registryKey != null)
                        {
                            string defaultBrouserPath = ((string)registryKey.GetValue(null, null)).Split('"')[1];
                            Process.Start(defaultBrouserPath, url);
                        }
                    }
                    else
                    {
                        Process.Start(url);
                    }
                }
                catch
                {
                }
            }

            static public void Bind(Form form, string path)
            {
                form.Tag = new Starter(form, path);
            }

            static public string Index { get { return GetUrl(null); } }
            static public string Options { get { return GetUrl("options.html"); } }
            static public string Paths { get { return GetUrl("paths.html"); } }
            static public string HotKeys { get { return GetUrl("hotkeys.html"); } }
        }
    }
}
