using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.View;

using AntiDupl.NET.Core;
using System.Globalization;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class ConfigurationViewModel : PropertyChangedBase
    {
        private IConfigurationModel _configuration;
        public IConfigurationModel Configuration
        {
            get { return _configuration; }
        }

        public int Delay { get; set; }
        private DispatcherTimer _timer;
        IWindowService _windowService;
        CoreLib _core;
        public CoreOptions Option { get; private set; }
        IThumbnailProvider _thumbnailProvider;

        //public CompareOptionViewModel CompareOptions { get; private set; }

        public ConfigurationViewModel(IConfigurationModel configuration,
            IWindowService windowService,
            CoreLib core,
            CoreOptions option, 
            IThumbnailProvider thumbnailProvider,
            CompareOptions compareOptions)
        {
            this._configuration = configuration;
            _windowService = windowService;
            _core = core;
            Option = option;
            Delay = 500;
            _thumbnailWidth = _configuration.ThumbnailWidth;
            _thumbnailProvider = thumbnailProvider;

           // CompareOptions = new CompareOptionViewModel(option);
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
                        if (_timer != null)
                            _timer.Stop();

                        if (_timer == null)
                        {
                            _timer = new DispatcherTimer();
                            _timer.Interval = TimeSpan.FromMilliseconds(this.Delay);
                            _timer.Tick += (object s, EventArgs ea) =>
                            {
                                DispatcherTimer t = s as DispatcherTimer;
                                t.Stop();

                                _configuration.ThumbnailWidth = _thumbnailWidth;
                                _thumbnailProvider.ClearThumbnailCache();
                            };
                        }

                        _timer.Start();
                    }
                    else
                    {
                        _configuration.ThumbnailWidth = value;
                        _thumbnailProvider.ClearThumbnailCache();
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

        ICommand _changeGoodColorCommand;
        public ICommand ChangeGoodColorCommand
        {
            get
            {
                return _changeGoodColorCommand ?? (_changeGoodColorCommand = new RelayCommand(arg =>
                {
                    ColorViewModel vm = new ColorViewModel(Configuration.GoodColor);
                    _windowService.ShowDialogWindow<ColorWindow>(vm);
                    Configuration.GoodColor = vm.Color;
                }));
            }
        }

        ICommand _changeBadColorCommand;
        public ICommand ChangeBadColorCommand
        {
            get
            {
                return _changeBadColorCommand ?? (_changeBadColorCommand = new RelayCommand(arg =>
                {
                    ColorViewModel vm = new ColorViewModel(Configuration.BadColor);
                    _windowService.ShowDialogWindow<ColorWindow>(vm);
                    Configuration.BadColor = vm.Color;
                }));
            }
        }
    }
}
