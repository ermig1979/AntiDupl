﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ObjectModel.ConfigurableAction;
using TinyIoC;

using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Core;

namespace AntiDupl.NET.WPF.ViewModel
{
    [Serializable]
    public class DuplPairViewModel
    {
        public ImageInfoClass FirstFile { get; set; }
        public ImageInfoClass SecondFile { get; set; }

        public double Difference { get; set; }
        public CoreDll.TransformType Transform { get; set; }
        public CoreDll.ResultType Type { get; set; }

        [XmlIgnoreAttribute]
        /// <summary>
        /// Каталоги файлов отличаются
        /// </summary>
        public bool FolderAreDiffrent { get; set; }

        [NonSerialized]
        IConfigurationModel _configuration;

        public DuplPairViewModel()
        {
            FirstFile = new ImageInfoClass();
            SecondFile = new ImageInfoClass();
            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
        }
        public DuplPairViewModel(CoreResult core_results)
        {
            FirstFile = new ImageInfoClass();
            SecondFile = new ImageInfoClass();

            Difference = core_results.difference;
            Transform = core_results.transform;
            Type = core_results.type;

            FirstFile.Path = core_results.first.path;
            FirstFile.JpegPeaks = core_results.first.jpegPeaks;
            FirstFile.Blockiness = core_results.first.blockiness;
            FirstFile.Bluring = core_results.first.blurring;
            FirstFile.FileSize = core_results.first.size;
            FirstFile.Width = core_results.first.width;
            FirstFile.Height = core_results.first.height;
            FirstFile.Type = core_results.first.type;

            SecondFile.Path = core_results.second.path;
            SecondFile.JpegPeaks = core_results.second.jpegPeaks;
            SecondFile.Blockiness = core_results.second.blockiness;
            SecondFile.Bluring = core_results.second.blurring;
            SecondFile.FileSize = core_results.second.size;
            SecondFile.Width = core_results.second.width;
            SecondFile.Height = core_results.second.height;
            SecondFile.Type = core_results.second.type;

            //SaatiHelper.CalculateIndex(this, _configuration);
            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            FillFolderAreDiffrent();
        }

        public DuplPairViewModel(CoreDll.adResultW a) : this()
        {
            FirstFile = new ImageInfoClass();
            SecondFile = new ImageInfoClass();

            Difference = a.difference;
            Transform = a.transform;
            Type = a.type;

            FirstFile.Path = a.first.path;
            FirstFile.JpegPeaks = a.first.jpegPeaks;
            FirstFile.Blockiness = a.first.blockiness;
            FirstFile.Bluring = a.first.blurring;
            FirstFile.FileSize = a.first.size;
            FirstFile.Width = a.first.width;
            FirstFile.Height = a.first.height;
            FirstFile.Type = a.first.type;

            SecondFile.Path = a.second.path;
            SecondFile.JpegPeaks = a.second.jpegPeaks;
            SecondFile.Blockiness = a.second.blockiness;
            SecondFile.Bluring = a.second.blurring;
            SecondFile.FileSize = a.second.size;
            SecondFile.Width = a.second.width;
            SecondFile.Height = a.second.height;
            SecondFile.Type = a.second.type;

            SaatiHelper.CalculateIndex(this, _configuration);
            FillFolderAreDiffrent();
            //TODO слишком долго
            //SetNeighboursFileNames();
        }

        public void UpdateImages()
        {
            FirstFile.Image = null;
            SecondFile.Image = null;
        }

        public override bool Equals(Object obj)
        {
            // Check for null values and compare run-time types.
            if (obj == null || GetType() != obj.GetType())
                return false;

            //Если ссылки указывают на один и тот же адрес, то их идентичность гарантирована.
            if (object.ReferenceEquals(this, obj))
                return true;

            DuplPairViewModel r = (DuplPairViewModel)obj;
            return (this.FirstFile.Path == r.FirstFile.Path) && (this.SecondFile.Path == r.SecondFile.Path);
        }

        // If Equals() returns true for a pair of objects  
        // then GetHashCode() must return the same value for these objects. 
        public override int GetHashCode()
        {
            int hashFirstFile = FirstFile == null ? 0 : FirstFile.Path.GetHashCode();

            int hashSecondFile = SecondFile == null ? 0 : SecondFile.Path.GetHashCode();

            //Calculate the hash code.
            return hashFirstFile ^ hashSecondFile;
        }

        public void FillFolderAreDiffrent()
        {
            if (!String.IsNullOrEmpty(FirstFile.Path) && !String.IsNullOrEmpty(SecondFile.Path))
            {
                var dir1 = Path.GetDirectoryName(FirstFile.Path);
                var dir2 = Path.GetDirectoryName(SecondFile.Path);
                if (dir1 != dir2)
                    FolderAreDiffrent = true;
                else
                    FolderAreDiffrent = false;
            }
        }



        public void SetNeighboursFileNames()
        {
            FillNeighboursImageInfo(FirstFile.Path, s => PreviosFirstFile = s, s => AfterFirstFile = s);
            FillNeighboursImageInfo(SecondFile.Path, s => PreviosSecondFile = s, s => AfterSecondFile = s);
        }

        private void FillNeighboursImageInfo(string path, Action<ImageInfoClass> previosSetter, Action<ImageInfoClass> afterSetter)
        {
            if (!String.IsNullOrEmpty(path))
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(Path.GetDirectoryName(path));
                if (directoryInfo.Exists)
                {
                    FileInfo[] filesInfos = directoryInfo.GetFiles();

                    Array.Sort(filesInfos, new Comparison<FileInfo>((f, f2) => f.FullName.CompareTo(f2.FullName)));

                    for (int i = 0; i < filesInfos.Length; i++)
                    {
                        if (filesInfos[i].FullName == path)
                        {
                            if (i > 0)  //previos
                                if (File.Exists(filesInfos[i - 1].FullName))
                                    previosSetter(new ImageInfoClass() { Path = filesInfos[i - 1].FullName });
                            if (i < filesInfos.Length - 1) //next
                                if (File.Exists(filesInfos[i + 1].FullName))
                                    afterSetter(new ImageInfoClass() { Path = filesInfos[i + 1].FullName });
                            break;
                        }
                    }
                }
            }
        }

        [XmlIgnoreAttribute]
        public ImageInfoClass PreviosFirstFile { get; set; }
        [XmlIgnoreAttribute]
        public ImageInfoClass AfterFirstFile { get; set; }
        [XmlIgnoreAttribute]
        public ImageInfoClass PreviosSecondFile { get; set; }
        [XmlIgnoreAttribute]
        public ImageInfoClass AfterSecondFile { get; set; }

        [XmlIgnoreAttribute]
        public IConfigurableAction ConfigurableAction { get; set; }


        public bool CheckAllExist()
        {
            return File.Exists(FirstFile.Path) && File.Exists(SecondFile.Path);
        }
    }
}
