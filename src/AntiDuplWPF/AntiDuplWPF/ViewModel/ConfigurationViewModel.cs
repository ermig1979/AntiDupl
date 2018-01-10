using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDuplWPF.Core;
using AntiDuplWPF.Model;
using AntiDuplWPF.Service;
using AntiDuplWPF.View;

namespace AntiDuplWPF.ViewModel
{
    public class ConfigurationViewModel : PropertyChangedBase
    {
        private IConfigurationModel _configuration;

        public int Delay { get; set; }
        private DispatcherTimer timer;
        IWindowService _windowService;
        CoreLib _core;
        public Option Option { get; set; }

        public ConfigurationViewModel(IConfigurationModel configuration, IWindowService windowService, CoreLib core, Option option)
        {
            this._configuration = configuration;
            _windowService = windowService;
            _core = core;
            Delay = 500;
            _thumbnailWidth = _configuration.ThumbnailWidth;
            Option = option;
        }

        public IConfigurationModel Configuration
        {
            get { return _configuration; }
        }

        int _thumbnailWidth;
        public int ThumbnailWidth
        {
            get { return _thumbnailWidth; }
            set
            {
                if (_thumbnailWidth != value)
                {
                    _thumbnailWidth = value;
                    //_configuration.ThumbnailWidth = value;
                    //ThumbnailProvider.Instance.ClearThumbnailCache();

                    if (this.Delay > 0)
                    {
                        if (timer != null)
                            timer.Stop();

                        if (timer == null)
                        {
                            timer = new DispatcherTimer();
                            timer.Interval = TimeSpan.FromMilliseconds(this.Delay);
                            timer.Tick += (object s, EventArgs ea) =>
                            {
                                DispatcherTimer t = s as DispatcherTimer;
                                t.Stop();

                                _configuration.ThumbnailWidth = _thumbnailWidth;
                                ThumbnailProvider.Instance.ClearThumbnailCache();
                            };
                        }

                        timer.Start();
                    }
                    else
                    {
                        _configuration.ThumbnailWidth = value;
                        ThumbnailProvider.Instance.ClearThumbnailCache();
                    }
                }
            }
        }

        //System.Windows.Media.Color _testColor;
        //public System.Windows.Media.Color TestColor
        //{
        //    get { return _testColor; }
        //    set
        //    {
        //        _testColor = value;
        //        RaisePropertyChangedEvent("TestColor");
        //    }
        //}


        private bool _deleteResized;
        public bool DeleteResized
        {
            get { return _deleteResized; }
            set
            {
                _deleteResized = value;
                this.RaisePropertyChangedEvent("DeleteResized");
            }
        }

    }
}
