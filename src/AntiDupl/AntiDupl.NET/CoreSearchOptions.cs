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
using System.Collections;
using System.Text;

namespace AntiDupl.NET
{
    public class CoreSearchOptions
    {
        private static string[] s_jpegExtensions = {"JPEG", "JFIF", "JPG", "JPE", "JIFF", "JIF", "J", "JNG", "JFF"};
        private static string[] s_tiffExtensions = {"TIF", "TIFF"};
        private static string[] s_bmpExtensions = {"BMP", "DIB", "RLE"};
        private static string[] s_gifExtensions = {"GIF"};
        private static string[] s_pngExtensions = {"PNG"};
        private static string[] s_emfExtensions = {"EMF", "EMZ"};
        private static string[] s_wmfExtensions = {"WMF"};
        private static string[] s_exifExtensions = {"EXIF"};
        private static string[] s_iconExtensions = {"ICON", "ICO", "ICN"};
        private static string[] s_jp2Extensions = {"JP2", "J2K", "J2C", "JPC", "JPF", "JPX"};
        private static string[] s_psdExtensions = {"PSD"};
        private static string[] s_ddsExtensions = {"DDS"};

        public bool subFolders;
        public bool system;
        public bool hidden;
        public bool JPEG;
        public bool BMP;
        public bool GIF;
        public bool PNG;
        public bool TIFF;
        public bool EMF;
        public bool WMF;
        public bool EXIF;
        public bool ICON;
        public bool JP2;
        public bool PSD;
        public bool DDS;

        public CoreSearchOptions()
        {
        }

        public CoreSearchOptions(CoreSearchOptions searchOptions)
        {
            subFolders = searchOptions.subFolders;
            system = searchOptions.system;
            hidden = searchOptions.hidden;
            JPEG = searchOptions.JPEG;
            BMP = searchOptions.BMP;
            GIF = searchOptions.GIF;
            PNG = searchOptions.PNG;
            TIFF = searchOptions.TIFF;
            EMF = searchOptions.EMF;
            WMF = searchOptions.WMF;
            EXIF = searchOptions.EXIF;
            ICON = searchOptions.ICON;
            JP2 = searchOptions.JP2;
            PSD = searchOptions.PSD;
            DDS = searchOptions.DDS;
        }

        public CoreSearchOptions(CoreDll.adSearchOptions searchOptions)
        {
            subFolders = searchOptions.subFolders != CoreDll.FALSE;
            system = searchOptions.system != CoreDll.FALSE;
            hidden = searchOptions.hidden != CoreDll.FALSE;
            JPEG = searchOptions.JPEG != CoreDll.FALSE;
            BMP = searchOptions.BMP != CoreDll.FALSE;
            GIF = searchOptions.GIF != CoreDll.FALSE;
            PNG = searchOptions.PNG != CoreDll.FALSE;
            TIFF = searchOptions.TIFF != CoreDll.FALSE;
            EMF = searchOptions.EMF != CoreDll.FALSE;
            WMF = searchOptions.WMF != CoreDll.FALSE;
            EXIF = searchOptions.EXIF != CoreDll.FALSE;
            ICON = searchOptions.ICON != CoreDll.FALSE;
            JP2 = searchOptions.JP2 != CoreDll.FALSE;
            PSD = searchOptions.PSD != CoreDll.FALSE;
            DDS = searchOptions.DDS != CoreDll.FALSE;
        }

        public void ConvertTo(ref CoreDll.adSearchOptions searchOptions)
        {
            searchOptions.subFolders = subFolders ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.system = system ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.hidden = hidden ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.JPEG = JPEG ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.BMP = BMP ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.GIF = GIF ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.PNG = PNG ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.TIFF = TIFF ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.EMF = EMF ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.WMF = WMF ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.EXIF = EXIF ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.ICON = ICON ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.JP2 = JP2 ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.PSD = PSD ? CoreDll.TRUE : CoreDll.FALSE;
            searchOptions.DDS = DDS ? CoreDll.TRUE : CoreDll.FALSE;
        }

        public CoreSearchOptions Clone()
        {
            return new CoreSearchOptions(this);
        }

        public bool Equals(CoreSearchOptions searchOptions)
        {
            return
                subFolders == searchOptions.subFolders &&
                system == searchOptions.system &&
                hidden == searchOptions.hidden &&
                JPEG == searchOptions.JPEG &&
                BMP == searchOptions.BMP &&
                GIF == searchOptions.GIF &&
                PNG == searchOptions.PNG &&
                TIFF == searchOptions.TIFF &&
                EMF == searchOptions.EMF &&
                WMF == searchOptions.WMF &&
                EXIF == searchOptions.EXIF &&
                ICON == searchOptions.ICON &&
                JP2 == searchOptions.JP2 &&
                PSD == searchOptions.PSD &&
                DDS == searchOptions.DDS;
        }

        public string[] GetActualExtensions()
        {
            ArrayList extensions = new ArrayList();
            if(JPEG)
                extensions.AddRange(s_jpegExtensions);
            if(TIFF)
                extensions.AddRange(s_tiffExtensions);
            if(BMP)
                extensions.AddRange(s_bmpExtensions);
            if(GIF)
                extensions.AddRange(s_gifExtensions);
            if(PNG)
                extensions.AddRange(s_pngExtensions);
            if(EMF)
                extensions.AddRange(s_emfExtensions);
            if(WMF)
                extensions.AddRange(s_wmfExtensions);
            if(ICON)
                extensions.AddRange(s_iconExtensions);
            if(JP2)
                extensions.AddRange(s_jp2Extensions);
            if(PSD)
                extensions.AddRange(s_psdExtensions);
            if (DDS)
                extensions.AddRange(s_ddsExtensions);
            return (string[])extensions.ToArray(typeof(string));
        }
    }
}
