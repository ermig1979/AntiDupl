using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.ViewModel
{
    [Serializable]
    public class DuplPairViewModel
    {
        public ImageInfoClass FirstFile { get; set; }
        public ImageInfoClass SecondFile { get; set; }

        public DuplPairViewModel(Core.CoreDll.adResultW a)
        {
            FirstFile = new ImageInfoClass();
            SecondFile = new ImageInfoClass();
            FolderAreDiffrent = false;

            FirstFile.Path = a.first.path;
            FirstFile.JpegQuality = a.first.blockiness;
            FirstFile.Sharpness = a.first.blurring;
            FirstFile.FileSize = a.first.size;
            FirstFile.Width = a.first.width;
            FirstFile.Height = a.first.height;

            SecondFile.Path = a.second.path;
            SecondFile.JpegQuality = a.second.blockiness;
            SecondFile.Sharpness = a.second.blurring;
            SecondFile.FileSize = a.second.size;
            SecondFile.Width = a.second.width;
            SecondFile.Height = a.second.height;

            //SaatiHelper.CalculateIndex(this, _configuration);
            //FillFolderAreDiffrent();
            //SetNeighboursFileNames();
        }

        public void UpdateImages()
        {
            FirstFile.Image = null;
            SecondFile.Image = null;
        }

        [XmlIgnoreAttribute]
        /// <summary>
        /// Каталоги файлов отличаются
        /// </summary>
        public bool FolderAreDiffrent { get; set; }
    }
}
