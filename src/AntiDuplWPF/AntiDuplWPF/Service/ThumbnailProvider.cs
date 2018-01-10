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
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;
using TinyIoC;

namespace AntiDuplWPF.Service
{
    class ThumbnailProvider : PropertyChangedBase, IDisposable
    {
        //from http://rsdn.org/article/dotnet/CSThreading1.xml
        EventWaitHandle wh = new AutoResetEvent(false);

        Thread _thumbnailLoaderThread;
        ThumbnailCache _thumbnailCache;
        IConfigurationModel _configuration;

        private Queue<ImageInfoClass> _queue = new Queue<ImageInfoClass>();
        //private Stack<ImageInfoClass> _queue = new Stack<ImageInfoClass>();

        public ThumbnailProvider()
        {
            //_semaphore = new Semaphore(0, 1);

            //StartReader();
            _thumbnailLoaderThread = new Thread(ThumbnailLoader);
            _thumbnailLoaderThread.Name = "ThumbnailLoader";
            _thumbnailLoaderThread.IsBackground = true;
            _thumbnailLoaderThread.Priority = ThreadPriority.Lowest;
            _thumbnailLoaderThread.Start();

            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            _configuration.PropertyChanged += _configuration_PropertyChanged;

            _thumbnailCache = new ThumbnailCache(_configuration);
        }

        void _configuration_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "ThumbnailWidth")
                ClearThumbnailCache();
        }

        public void Dispose()
        {
            Enqueue(null);      // Сигнал Потребителю на завершение
            _thumbnailLoaderThread.Join();          // Ожидание завершения Потребителя
            wh.Close();             // Освобождение ресурсов
        }

        #region Lazy<T> Singleton

        static readonly Lazy<ThumbnailProvider> _instance = new Lazy<ThumbnailProvider>(() => new ThumbnailProvider());

        public static ThumbnailProvider Instance
        {
            get
            {
                return _instance.Value;
            }
        }

        #endregion

        /*public void StartReader()
        {
            myThread = new Thread(Read);
            myThread.Name = "Читатель 1";
            myThread.Start();

            Thread myThread2 = new Thread(Read);
            myThread2.Name = "Читатель 2";
            myThread2.Start();

            Thread myThread3 = new Thread(Read);
            myThread3.Name = "Читатель 3";
            myThread3.Start();
        }

        public void Read()
        {
            while (count > 0)
            {
                sem.WaitOne();

                Debug.WriteLine("{0} входит в библиотеку", Thread.CurrentThread.Name);

                Debug.WriteLine("{0} читает", Thread.CurrentThread.Name);
                Thread.Sleep(1000);

                Debug.WriteLine("{0} покидает библиотеку", Thread.CurrentThread.Name);

                sem.Release();

                count--;
                Thread.Sleep(1000);
            }
        }*/

        /// <summary>
        ///  добавляем элемент
        /// </summary>
        /// <param name="path"></param>
        public void Enqueue(ImageInfoClass item)
        {
            lock (_queue)
            {
                //if (!_queue.Any(i => i.Path == item.Path))
                //{
                //Debug.WriteLine("Ставим в очередь " + item.Path);
                _queue.Enqueue(item);
                //_queue.Push(item);
                //}
                //else
                //    Debug.WriteLine("В очереди на эскиз уже есть " + item.Path);
            }

            wh.Set();
        }


        private void ThumbnailLoader()
        {
            while (true)
            {
                ImageInfoClass task = null;

                lock (_queue)
                {
                    if (_queue.Count > 0)
                    {
                        task = _queue.Dequeue();
                        //task = _queue.Pop();
                        if (task == null)
                            return;
                    }
                }

                if (task != null)
                {
                    if (!_thumbnailCache.HasThumbnail(task.Path))
                    {
                        //Debug.WriteLine("В кеше нет эскиза для " + task.Path);
                        try
                        {
                            if (File.Exists(task.Path))
                            {
                                Debug.WriteLine(String.Format("ThumbnailProvider load {0}", task.Path));
                                using (Stream stream = File.OpenRead(task.Path))
                                {
                                    var image = new BitmapImage();
                                    image.BeginInit();
                                    image.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
                                    image.CacheOption = BitmapCacheOption.OnLoad;
                                    //image.DecodePixelWidth = Const.IMAGE_WIDTH;
                                    image.DecodePixelWidth = _configuration.ThumbnailWidth;
                                    //image.DecodePixelHeight = Const.IMAGE_HEIGHT;
                                    image.StreamSource = stream;
                                    image.EndInit();
                                    image.Freeze();

                                    GC.Collect(GC.MaxGeneration);
                                    //Debug.WriteLine("Добавляем в кеше эскиз для " + task.Path);
                                    _thumbnailCache.AddThumbnail(task.Path, image);
                                    Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal,
                                   (Action)delegate
                                   {
                                       task.Image = image;
                                   });
                                }
                                Debug.WriteLine(String.Format("ThumbnailProvider loaded {0}", task.Path));
                            }
                        }
                        catch (NotSupportedException nsex)
                        {
                            Console.WriteLine(nsex.Message);
                        }
                    }
                    else
                    {
                        //Debug.WriteLine("В кеше есть эскиз для " + task.Path);

                        var image = _thumbnailCache.GetThumbnail(task.Path);
                        Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal,
                        (Action)delegate
                        {
                            task.Image = image;
                        });
                    }

                    //Application.Current.Dispatcher.Invoke(DispatcherPriority.Normal,
                    //  (Action)delegate
                    //  {

                    lock (_queue)
                    {
                        ThumbnailCacheStatus = string.Format("Thumbnail Cached {0}/{1}, queue {2}",
                             _thumbnailCache.Cached, _configuration.MaxCachedThumbnail, _queue.Count);
                        //   });
                    }
                }
                else
                    wh.WaitOne();       // Больше задач нет, ждем сигнала...
            }
        }

        public void ClearThumbnailCache()
        {
            //lock (_queue)
            //{
            _thumbnailCache.Clear();
            //    _queue.Clear;
            //}
        }

        string _thumbnailCacheStatus;
        public string ThumbnailCacheStatus
        {
            get
            {
                return _thumbnailCacheStatus;
            }
            set
            {
                _thumbnailCacheStatus = value;
                RaisePropertyChangedEvent("ThumbnailCacheStatus");
            }
        }
    }
}
