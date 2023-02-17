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

        [NonSerialized]
        [XmlIgnore]
        IImageLoader _imageLoader;

        [NonSerialized]
        [XmlIgnore]
        IThumbnailProvider _thumbnailProvider;

        public ImageInfoClass()
        {
            if (Application.Current != null
                && Application.Current.Dispatcher != null)
            Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal,
                       (Action)delegate
                       {
                           _dumbImage = new BitmapImage(new Uri("pack://application:,,,/AntiDuplWPF;component/Image/loading.jpg", UriKind.Absolute));
                       });


            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            _configuration.PropertyChanged += _configuration_PropertyChanged;

            _imageLoader = TinyIoC.TinyIoCContainer.Current.Resolve<IImageLoader>();
            _thumbnailProvider = TinyIoC.TinyIoCContainer.Current.Resolve<IThumbnailProvider>();
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
                if (!String.IsNullOrEmpty(_path))
                {
                    FileName = System.IO.Path.GetFileName(_path);
                    Directory = System.IO.Path.GetDirectoryName(_path);
                }
                RaisePropertyChangedEvent("Path");
                RaisePropertyChangedEvent("FileName");
                RaisePropertyChangedEvent("Directory");
            }
        }

        [XmlIgnore]
        public string FileName { get; private set; }

        [XmlIgnore]
        public string Directory { get; private set; }

        uint _jpegPeaks;
        public uint JpegPeaks
        {
            get { return _jpegPeaks; }
            set
            {
                _jpegPeaks = value;
                RaisePropertyChangedEvent("JpegPeaks");
            }
        }

        /// <summary>
        /// Блочность. Чем выше Blockiness, тем лучше изображение.
        /// </summary>
        public double Blockiness { get; set; }

        /// <summary>
        /// Размытость изображенитя. Чем ниже Bluring, тем лучше изображение
        /// </summary>
        public double Bluring { get; set; }

        public ulong FileSize { get; set; }

        uint _width;
        public uint Width
        {
            get { return _width; }
            set
            {
                try
                {
                    _width = value;
                }
                catch (Exception)
                {
                    
                    throw;
                }

                RaisePropertyChangedEvent("Width");
            }
        }


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

                _thumbnailProvider.Enqueue(this);

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
                return _imageLoader.LoadImage(Path, Width, Height, Type);
            }
        }

        public override bool Equals(Object obj)
        {
            // Check for null values and compare run-time types.
            if (obj == null || GetType() != obj.GetType())
                return false;

            //Если ссылки указывают на один и тот же адрес, то их идентичность гарантирована.
            if (object.ReferenceEquals(this, obj))
                return true;

            ImageInfoClass i = (ImageInfoClass)obj;
            return this.Path == i.Path;
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

        public override int GetHashCode()
        {
            return Path.GetHashCode();
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


        [XmlIgnoreAttribute]
        public decimal UtilityIndexNotNorm { get; set; }

        decimal _utilityIndex;
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

        public void RaiseUpdateUtilityIndex()
        {
            RaisePropertyChangedEvent("UtilityIndex");
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

        public Core.CoreDll.ImageType Type { get; set; }


    }
}
