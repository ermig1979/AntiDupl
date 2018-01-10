using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using AntiDuplWPF.Command;
using AntiDuplWPF.Core;
using AntiDuplWPF.Helper;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.Service;
using AntiDuplWPF.UndoRedo;
using AntiDuplWPF.View;
using TinyIoC;

namespace AntiDuplWPF.ViewModel
{
    public class MainViewModel : PropertyChangedBase
    {
        CoreLib _core;
        IWindowService _windowService;
        public LocationsModel LocationsModel { get; set; }
        public Option Options { get; set; }
        /// <summary>
        /// Список пар дубликатов полученный из dll
        /// </summary>
        ObservableCollection<DuplPairViewModel> _resultList;
        IUndoRedoEngine _undoRedoEngine;
        IConfigurationModel _configuration;
        public IConfigurationModel Configuration
        {
            get { return _configuration; }
        }
        IgnoreStorage _ignoreStorage;

        public MainViewModel()
        {
            _core = new CoreLib();
            _windowService = new WindowService();
            //LocationsModel = new LocationsModel(_core);
            LocationsModel = LocationsModel.Load(_core);
            Options = new Option(_core);
            _ignoreStorage = IgnoreStorage.Load();

            _configuration = TinyIoCContainer.Current.Resolve<IConfigurationModel>();
            _configuration.PropertyChanged += OnConfigurationPropertyChanged;

            UndoRedoEngine undoRedoEngine = new UndoRedoEngine(_configuration);
            _undoRedoEngine = undoRedoEngine;
            _undoRedoEngine.PropertyChanged += OnUndoRedoEnginePropertyChanged;
            TinyIoCContainer.Current.Register<IUndoRedoEngine, UndoRedoEngine>(undoRedoEngine);

            ThumbnailProvider.Instance.PropertyChanged += OnThumbnailProviderPropertyChanged;
        }

        public void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            LocationsModel.Save();
            _core.Dispose();
        }

        void OnUndoRedoEnginePropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            switch (e.PropertyName)
            {
                case "UndoTooltip":
                    RaisePropertyChangedEvent("UndoTooltip");
                    break;
            }
        }

        void OnConfigurationPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "ThumbnailWidth")
            {
                RaisePropertyChangedEvent("ThumbnailWidth");
                RaisePropertyChangedEvent("ThumbnailColumnWidth");
                if (_resultList != null)
                {
                    foreach (DuplPairViewModel resultItem in _resultList)
                    {
                        resultItem.UpdateImages();
                    }
                }
            }
        }

        ICommand _searchCommand;
        public ICommand SearchCommand
        {
            get
            {
                return _searchCommand ?? (_searchCommand = new SearchDllCommand(this, _core, _windowService));
            }
        }

        ICommand _openPathCommand;
        public ICommand OpenPathCommand
        {
            get
            {
                return _openPathCommand ?? (_openPathCommand = new RelayCommand(arg =>
                {
                    PathViewModel vm = new PathViewModel(LocationsModel);
                    _windowService.ShowDialogWindow<PathWindow>(vm);
                }, arg => true));
            }
        }

        /// <summary>
        /// Коллекция для вывода, с фильтрацией и т.д.
        /// </summary>
        IEditableCollectionView _result;
        public IEditableCollectionView Result
        {
            get { return _result; }
            set
            {
                _result = value;
                RaisePropertyChangedEvent("Result");
                RaisePropertyChangedEvent("TotalInfo");
            }
        }

        public string TotalInfo
        {
            get
            {
                if (_resultList != null)
                    return _resultList.Count.ToString();
                return "";
            }
        }

        void OnThumbnailProviderPropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "ThumbnailCacheStatus")
                RaisePropertyChangedEvent("CacheStatus");
        }

        public string CacheStatus
        {
            get
            {
                return ThumbnailProvider.Instance.ThumbnailCacheStatus;
            }
        }

        public int SelectedResultIndex { get; set; }

        public void SetResult(DuplPairViewModel[] result)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                _resultList = new ObservableCollection<DuplPairViewModel>(result);
                var res = CollectionViewSource.GetDefaultView(_resultList);
                Result = (IEditableCollectionView)res;
                Result.NewItemPlaceholderPosition = NewItemPlaceholderPosition.None;
            });
        }


        public int ThumbnailWidth
        {
            get { return _configuration.ThumbnailWidth; }
        }

        public int ThumbnailColumnWidth
        {
            get { return _configuration.ThumbnailWidth * 2 + 10; }
        }

        public Func<DuplPairViewModel, BindableMenuItem[]> MenuGeneratorDuplPair
        {
            get
            {
                return (result =>
                {
                    if (result != null)
                    {
                        string firstToSecond;
                        string secondToFirst;
                        IUCommand firstToSecondUCommand;
                        IUCommand secondToFirstUCommand;
                        RenameHelper.FillMenu(result, out firstToSecond, out secondToFirst, out firstToSecondUCommand,
                            out secondToFirstUCommand);
                        ICommand firstToSecondCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(firstToSecondUCommand);
                        }, arg => firstToSecondUCommand != null);
                        ICommand secondToFirstCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(secondToFirstUCommand);
                        }, arg => secondToFirstUCommand != null);

                        BindableMenuItem[] menu = new BindableMenuItem[2];
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = firstToSecond;
                        menu[0].Command = firstToSecondCommand;

                        menu[1] = new BindableMenuItem();
                        menu[1].Name = secondToFirst;
                        menu[1].Command = secondToFirstCommand;

                       /* IUCommand ignoreUCommand = new IgnorePairCommand(_resultList, Result, _ignoreStorage.IgnoreList,
                            SelectedResultIndex);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);
                        menu[2] = new BindableMenuItem();
                        menu[2].Name = Application.Current.Resources["mainwindow_contextmenu_IgnorePair"] as string;
                        menu[2].Command = ignoreCommand;

                        ICommand openComparatorCommand = new RelayCommand(arg =>
                        {
                            ComparatorViewModel vm = new ComparatorViewModel(result, _resultList, Result,
                                _undoRedoEngine, _windowService, _configuration);
                            _windowService.OpenComparatorWindow(vm);
                        }, arg => _windowService != null);
                        menu[3] = new BindableMenuItem();
                        menu[3].Name = Application.Current.Resources["mainwindow_contextmenu_OpenСomparator"] as string;
                        menu[3].Command = openComparatorCommand;*/

                        return menu;
                    }
                    return null;
                });
            }
        }

        public Func<IList, BindableMenuItem[]> MenuGeneratorMultiDuplPair
        {
            get
            {
                return (results =>
                {
                    if (results != null && results.Count > 0)
                    {
                        BindableMenuItem[] menu = new BindableMenuItem[2];

                        IUCommand ignoreUCommand = new IgnoreMultiPairCommand(results, _resultList, _ignoreStorage.IgnoreList);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = String.Format(
                            Application.Current.Resources["mainwindow_contextmenu_IgnorePairs"] as string,
                            results.Count);
                        menu[0].Command = ignoreCommand;

                        IUCommand autoDeleteUCommand = new AutoDeleteMultiPairCommand(results, _resultList);
                        ICommand autoDeleteCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(autoDeleteUCommand);
                        }, arg => autoDeleteUCommand != null);
                        menu[1] = new BindableMenuItem();
                        menu[1].Name = Application.Current.Resources["mainwindow_contextmenu_AutoDeleteBySaati"] as string;
                        menu[1].Command = autoDeleteCommand;

                        return menu;
                    }
                    return null;
                });
            }
        }

        ICommand _executeIgnorePairCommand;
        public ICommand ExecuteIgnorePairCommand
        {
            get
            {
                return _executeIgnorePairCommand ?? (_executeIgnorePairCommand = new RelayCommand(arg =>
                {
                    IUCommand ignoreUCommand = new IgnorePairCommand(_resultList, Result, _ignoreStorage.IgnoreList,
                        SelectedResultIndex);
                    _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                }, arg => arg != null));
            }
        }

        ICommand _openImageCommand;
        public ICommand OpenImageCommand
        {
            get
            {
                return _openImageCommand ?? (_openImageCommand = new RelayCommand(arg =>
                {
                    var path = (string)arg;
                    if (System.IO.File.Exists(path))
                        System.Diagnostics.Process.Start(path);
                }, arg => true));
            }
        }

        ICommand _openConfigurationCommand;
        public ICommand OpenConfigurationCommand
        {
            get
            {
                return _openConfigurationCommand ?? (_openConfigurationCommand = new RelayCommand(arg =>
                {
                    ConfigurationViewModel vm = new ConfigurationViewModel(_configuration, _windowService, _core, Options);
                    _windowService.ShowDialogWindow<ConfigurationWindow>(vm);
                }, arg => _configuration != null));
            }
        }

        ICommand _setPrioritiesCommand;
        public ICommand SetPrioritiesCommand
        {
            get
            {
                return _setPrioritiesCommand ?? (_setPrioritiesCommand = new RelayCommand(arg =>
                {
                    PrioritiesViewModel vm = new PrioritiesViewModel(_configuration);
                    _windowService.ShowDialogWindow<PrioritiesWindow>(vm);
                }, arg => true));
            }
        }

        ICommand _updateSaatiIndexCommand;
        public ICommand UpdateSaatiIndexCommand
        {
            get
            {
                return _updateSaatiIndexCommand ?? (_updateSaatiIndexCommand = new RelayCommand(arg =>
                {
                    foreach (var item in _resultList)
                    {
                        SaatiHelper.CalculateIndex(item, _configuration);
                    }
                }, arg => _resultList != null && _resultList.Any()));
            }
        }

        public string Version
        {
            get { return Assembly.GetExecutingAssembly().GetName().Version.ToString(); }
        }

        ICommand _undoCommand;
        public ICommand UndoCommand
        {
            get
            {
                return _undoCommand ?? (_undoCommand = new RelayCommand(arg =>
                {
                    _undoRedoEngine.Undo();
                }, arg => _undoRedoEngine.UndoEnable));
            }
        }

        ICommand _redoCommand;
        public ICommand RedoCommand
        {
            get
            {
                return _redoCommand ?? (_redoCommand = new RelayCommand(arg =>
                {
                    _undoRedoEngine.Redo();
                }, arg => _undoRedoEngine.RedoEnable));
            }
        }
    }
}
