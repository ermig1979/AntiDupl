using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Threading;
using System.Xml.Serialization;
using AntiDuplWPF.Model;
using AntiDuplWPF.Service;
using AntiDuplWPF.ViewModel;
using TinyIoC;

namespace AntiDuplWPF.ObjectModel
{
    public class ImageInfoClass : PropertyChangedBase, IEquatable<ImageInfoClass>
    {
        [NonSerialized]
        [XmlIgnore]
        BitmapImage _dumbImage;

        [NonSerialized]
        [XmlIgnore]
        IConfigurationModel _configuration;

        public ImageInfoClass()
        {
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal,
                       (Action)delegate
                       {
                           _dumbImage = new BitmapImage(new Uri("pack://application:,,,/AntiDuplWPF;component/Image/loading.jpg", UriKind.Absolute));
                       });


            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            _configuration.PropertyChanged += _configuration_PropertyChanged;
        }

        ~ImageInfoClass()
        {
            _configuration.PropertyChanged -= _configuration_PropertyChanged;
        }


        void _configuration_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "ThumbnailWidth")
            {
                _image = null;
                RaisePropertyChangedEvent("Image");
            }
        }

        string _path;
        public string Path
        {
            get { return _path; }
            set
            {
                _path = value;
                RaisePropertyChangedEvent("Path");
                //RaisePropertyChangedEvent("FileName");
            }
        }

        /// <summary>
        /// Качество JPEG. Чем выше JpegQuality, тем лучше изображение.
        /// </summary>
        public double JpegQuality { get; set; }
        /// <summary>
        /// Резкость изображения. Чем выше Sharpness, тем лучше изображение.
        /// </summary>
        public double Sharpness { get; set; }
        public ulong FileSize { get; set; }
        public uint Width { get; set; }
        public uint Height { get; set; }
        public string Resolution
        {
            get { return String.Format("{0}x{1}", Width, Height); }
        }

        [NonSerialized]
        [XmlIgnoreAttribute]
        BitmapSource _image;
        [XmlIgnoreAttribute]
        public BitmapSource Image
        {
            get
            {
                if (_image != null && _image != _dumbImage)
                    return _image;

                if (String.IsNullOrEmpty(Path))
                    return null;

                ThumbnailProvider.Instance.Enqueue(this);

                /*ThreadPool.QueueUserWorkItem(delegate(object state)
                {
                    //non UI thread
                    var model = state as ImageInfoClass;
                    Debug.WriteLine(String.Format("Получаем изображение для {0} из ThumbnailCache", model.Path));

                    //var image = ThumbnailCache.Instance.GetThumbnail(Path);
                    var image = ThumbnailCache2.Instance.GetThumbnail(Path);
                    //var image = ThumbnailProvider

                    Application.Current.Dispatcher.Invoke(
                        DispatcherPriority.Normal,
                    (Action)delegate
                    {
                        Debug.WriteLine(String.Format("Изображение для {0} получено", model.Path));

                        model.Image = image;

                        RaisePropertyChangedEvent("Image"); 
                    }
                    );

                }, this);*/


                //return null;
                //return new BitmapImage(new Uri("pack://application:,,,/loading.jpg", UriKind.Absolute));
                //Debug.WriteLine("Возвращаем заглушку для " + Path);
                return _dumbImage;
                ///ImageComparerGUIWPF;component/Images/undo-3-32.png
            }
            set
            {
                if (value == _dumbImage)
                    throw new Exception("value = _dumbImage");
                //Debug.WriteLine("Для картинки " + Path + " устанавливается изображение");
                _image = value;
                RaisePropertyChangedEvent("Image");
            }
        }

        [XmlIgnoreAttribute]
        public BitmapSource ImageWithoutCache
        {
            get
            {
                if (File.Exists(Path))
                    using (var fs = new FileStream(Path, FileMode.Open, FileAccess.Read))
                    {
                        return BitmapFrame.Create(
                            fs, BitmapCreateOptions.None, BitmapCacheOption.OnLoad);
                    }
                return null;
            }
        }

        public bool Equals(ImageInfoClass other)
        {
            if (this.Path == other.Path)
            {
                return true;
            }
            else
            {
                return false;
            }
        }


        /*private MetadataExtractor _metadata;
        public MetadataExtractor EXIF
        {
            get { return _metadata; }
        }

        private string _exifTooltip;
        public string ExifTooltip
        {
            get
            {
                if (_metadata == null)
                {
                    //var source = new Uri(Path);
                    //_metadata = new ExifMetadata(source);
                    _metadata = MetadataExtractor.FromFile(Path, true);
                    _exifTooltip = _metadata.ToString();
                }
                return _exifTooltip;
            }
        }*/

        /*[XmlIgnoreAttribute]
        public decimal ResolutionNorm { get; set; }

        [XmlIgnoreAttribute]
        public decimal JpegQualityNorm { get; set; }

        [XmlIgnoreAttribute]
        public decimal SharpnessNorm { get; set; }

        [XmlIgnoreAttribute]
        public decimal FileSizeNorm { get; set; }*/

        [XmlIgnoreAttribute]
        public decimal UtilityIndexNotNorm { get; set; }

        decimal _utilityIndex;
        /// <summary>
        /// Чем выше JpegQuality, тем лучше изображение
        /// </summary>
        [XmlIgnoreAttribute]
        public decimal UtilityIndex
        {
            get { return _utilityIndex; }
            set
            {
                _utilityIndex = value;
                RaisePropertyChangedEvent("UtilityIndex");
            }
        }

        string _utilityIndexCalculateDecription;
        [XmlIgnoreAttribute]
        public string UtilityIndexCalculateDecription
        {
            get { return _utilityIndexCalculateDecription; }
            set
            {
                _utilityIndexCalculateDecription = value;
                RaisePropertyChangedEvent("UtilityIndexCalculateDecription");
            }
        }
    }
}
