using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Threading;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.UndoRedo;
using AntiDupl.NET.WPF.View;
using Microsoft.Extensions.Logging;
using Microsoft.Win32;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Enums;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class MainViewModel : PropertyChangedBase
    {
        const int DUPL_IMAGE_TAB_INDEX = 0;
        const int COMPARE_TAB_INDEX = 2;
        
        CoreLib _core;
        IWindowService _windowService;
        ILogger _logger;
        public LocationsModel LocationsModel { get; set; }
        public CoreOptions Options { get; set; }

        /// <summary>
        /// Список пар дубликатов полученный из dll
        /// </summary>
        ObservableCollection<DuplPairViewModel> _resultList = new ObservableCollection<DuplPairViewModel>();
        /// <summary>
        /// Список пар дубликатов полученный из dll
        /// </summary>
        public ObservableCollection<DuplPairViewModel> ResultList
        {
            get { return _resultList; }
        }

        IUndoRedoEngine _undoRedoEngine;
        //IConfigurationModel _configuration;
        //public IConfigurationModel Configuration
        //{
        //    get { return _configuration; }
        //}
        public IConfigurationModel Configuration { get; private set; }
        IgnoreStorage _ignoreStorage;
        ILanguageService _languageService;

        IViewModeModel _viewModeModel;
        public IViewModeModel ViewModeModel //используется для скрытия табов
        {
            get { return _viewModeModel; }
        }


        /// <summary>
        /// Группы дубликатов
        /// </summary>
        ObservableCollection<DuplicateGroup> _groups = new ObservableCollection<DuplicateGroup>();
        /// <summary>
        /// Группы дубликатов
        /// </summary>
        public ObservableCollection<DuplicateGroup> Groups
        {
            get { return _groups; }
        }

        /// <summary>
        /// Коллекция для вывода групп, с отслеживанием текущего элемента
        /// </summary>
        ICollectionView _groupsCollection;
        public ICollectionView GroupsCollection
        {
            get { return _groupsCollection; }
            set
            {
                _groupsCollection = value;
                this.RaisePropertyChangedEvent("GroupsCollection");
                SelectedTabPageIndex = 1;
            }
        }

        IThumbnailProvider _thumbnailProvider;

        public MainViewModel(
            ILanguageService languageService,
            IConfigurationModel configuration,
            CoreLib coreLib,
            IThumbnailProvider thumbnailProvider,
            IUndoRedoEngine undoRedoEngine,
            IViewModeModel viewModeModel,
            ILoggerFactory loggerFactory)
        {
            _logger = loggerFactory.CreateLogger<MainViewModel>();
            _languageService = languageService;
            _windowService = new WindowService(_logger);

            _thumbnailProvider = thumbnailProvider;
            _core = coreLib;
            //LocationsModel = new LocationsModel(_core);
            LocationsModel = LocationsModel.Load(_core);
            Options = new CoreOptions(_core);
            _ignoreStorage = IgnoreStorage.Load();

            _languageService.LanguageChanged += OnLanguageChanged;
            InitializeCompareModeOptions();

            _logger.LogInformation("MainViewModel: инициализирован (режим={Algorithm}, порог={Threshold}%)",
                Options.compareOptions.AlgorithmComparing, Options.compareOptions.ThresholdDifference);

            Configuration = configuration;
            Configuration.PropertyChanged += OnConfigurationPropertyChanged;

            _undoRedoEngine = undoRedoEngine;
            _undoRedoEngine.PropertyChanged += OnUndoRedoEnginePropertyChanged;

            _viewModeModel = viewModeModel;

            _thumbnailProvider.PropertyChanged += OnThumbnailProviderPropertyChanged;
        }

        public void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _logger.LogInformation("MainViewModel: закрытие окна");
            Configuration.PropertyChanged -= OnConfigurationPropertyChanged;
            _undoRedoEngine.PropertyChanged -= OnUndoRedoEnginePropertyChanged;
            _languageService.LanguageChanged -= OnLanguageChanged;
            //_dllModel.SaveSettings();
            _core.Dispose();

            LocationsModel.Save();
            Configuration.Save();
            _undoRedoEngine.Clear();

			if (Configuration.SaveResultOnClose && !String.IsNullOrEmpty(Configuration.LastResultFile))
				SerializeHelper<DuplPairViewModel[]>.Save(_resultList.ToArray(), Configuration.LastResultFile);
        }

        public void OnLoaded(object sender, RoutedEventArgs e)
        {
            if (Configuration.LoadResultOnOpen && !String.IsNullOrEmpty(Configuration.LastResultFile))
            {
                _logger.LogInformation("MainViewModel: загрузка результата при открытии: {File}", Configuration.LastResultFile);
                LoadResults(Configuration.LastResultFile);
            }
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
                return _searchCommand ?? (_searchCommand = new SearchDllCommand(this, _core, _windowService, _logger));
            }
        }

        ICommand _deleteImageCommand;
        /// <summary>
        /// Удаление изображения из текущей пары (parameter = ImageInfoClass файла).
        /// </summary>
        public ICommand DeleteImageCommand
        {
            get
            {
                return _deleteImageCommand ?? (_deleteImageCommand = new RelayCommand(arg =>
                {
                    ImageInfoClass file = arg as ImageInfoClass;
                    DuplPairViewModel result = (Result as ICollectionView)?.CurrentItem as DuplPairViewModel;
                    if (file == null || result == null)
                        return;
                    if (file.Equals(result.FirstFile) || file.Equals(result.SecondFile))
                    {
                        _logger.LogInformation("DeleteImage: {Path}", file.Path);
                        IUCommand uCommand = new UndoRedo.DeleteImageCommand(result, file, _resultList, Result);
                        _undoRedoEngine.ExecuteCommand(uCommand);
                    }
                }, arg =>
                {
                    ImageInfoClass file = arg as ImageInfoClass;
                    if (file == null)
                        return false;
                    DuplPairViewModel result = (Result as ICollectionView)?.CurrentItem as DuplPairViewModel;
                    return result != null &&
                        (file.Equals(result.FirstFile) || file.Equals(result.SecondFile));
                }));
            }
        }

        ICommand _openComparatorForDuplPairCommand;
        /// <summary>
        /// Открыть сравнитель для пары (parameter = DuplPairViewModel или null — текущая пара).
        /// </summary>
        public ICommand OpenComparatorForDuplPairCommand
        {
            get
            {
                return _openComparatorForDuplPairCommand ?? (_openComparatorForDuplPairCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel result = arg as DuplPairViewModel;
                    if (result == null)
                    {
                        result = (Result as ICollectionView)?.CurrentItem as DuplPairViewModel;
                    }
                    if (result == null)
                        return;
                    _logger.LogInformation("OpenComparator: {First} / {Second}",
                        result.FirstFile.Path, result.SecondFile.Path);
                    ComparatorViewModel vm = new ComparatorViewModel(result, _resultList, Result,
                        _undoRedoEngine, _windowService, Configuration);
                    _windowService.OpenComparatorWindow(vm);
                }, arg => true));
            }
        }

        ICommand _calculateHistogramPeaksCommand;
        public ICommand CalculateHistogramPeaksCommand
        {
            get
            {
                //var resultEnum = (IEnumerable<DuplPairViewModel>)_resultList;
                return _calculateHistogramPeaksCommand ?? (_calculateHistogramPeaksCommand
                    = new CalculateHistogramPeaksCommand(this, _core, _windowService, _resultList));
                //return new CalculateHistogramPeaksCommand(this, _core, _windowService, _resultList);
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

        ICommand _configurableActionCommand;
        public ICommand ConfigurableActionCommand
        {
            get
            {
                return _configurableActionCommand ?? (_configurableActionCommand = new RelayCommand(arg =>
                {
                    ConfigurableActionViewModel vm = new ConfigurableActionViewModel(_resultList, Configuration, 
                        _undoRedoEngine, Result, _windowService);
                    var result = _windowService.ShowDialogWindow<ConfigurableActionWindow>(vm);
                    if (result.HasValue && result.Value)
                    {
                        var sel = ResultList.SingleOrDefault(r => r.Equals(vm.SelectedPair));
                        ((ICollectionView)_result).MoveCurrentTo(sel);
                        //((ICollectionView)_result).MoveCurrentTo(vm.SelectedPair);
                        //SelectedResultIndex = 
                    }
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
                if (_result != null)
                {
                    ((ICollectionView)_result).CurrentChanged -= Result_CurrentChanged;
                    ((ICollectionView)_result).CollectionChanged -= Result_CollectionChanged;
                }
                _result = value;
                RaisePropertyChangedEvent("Result");
                RaisePropertyChangedEvent("TotalInfo");
                ((ICollectionView)_result).CurrentChanged += Result_CurrentChanged;
                ((ICollectionView)_result).CollectionChanged += Result_CollectionChanged;
            }
        }

        void Result_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            RaisePropertyChangedEvent("TotalInfo");
        }

        void Result_CurrentChanged(object sender, EventArgs e)
        {
            RaisePropertyChangedEvent("RenameFirstLikeSecondText");
            RaisePropertyChangedEvent("RenameSecondLikeFirstText");
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
                return _thumbnailProvider.ThumbnailCacheStatus;
            }
        }

        int _selectedResultIndex;
        public int SelectedResultIndex
        {
            get { return _selectedResultIndex; }
            set
            {
                _selectedResultIndex = value;
                RaisePropertyChangedEvent("SelectedResultIndex");
            }
        }

        public void SetResult(DuplPairViewModel[] result)
        {
            int count = result != null ? result.Length : 0;
            _logger.LogInformation("SetResult: получено пар: {Count}", count);
            SetStatus(string.Format("Показ результатов в таблице ({0} пар)…", count));
            Stopwatch stopwatch = Stopwatch.StartNew();

            Action fillOnUi = () =>
            {
                if (_groups != null)
                    _groups.Clear();
                if (_resultList != null)
                    _resultList.Clear();
                _viewModeModel.ViewMode = ViewMode.Pair;
                _thumbnailProvider.ClearThumbnailCache();

                if (count == 0)
                {
                    FinalizeResultTable(stopwatch, count);
                    return;
                }

                // Порционное добавление: UI не блокируется на весь массив
                const int batchSize = 200;
                int index = 0;
                EventHandler tickHandler = null;
                var batchTimer = new DispatcherTimer { Interval = TimeSpan.FromMilliseconds(10) };
                tickHandler = (s, e) =>
                {
                    int end = Math.Min(index + batchSize, count);
                    for (; index < end; index++)
                    {
                        _resultList.Add(result[index]);
                    }
                    if (index % (batchSize * 5) == 0 || index >= count)
                    {
                        SetStatus(string.Format("Заполнение таблицы… ({0}/{1})", Math.Min(index, count), count));
                    }

                    if (index >= count)
                    {
                        batchTimer.Stop();
                        batchTimer.Tick -= tickHandler;
                        FinalizeResultTable(stopwatch, count);
                    }
                };
                batchTimer.Tick += tickHandler;
                batchTimer.Start();
            };

            if (Application.Current.Dispatcher.CheckAccess())
                fillOnUi();
            else
                Application.Current.Dispatcher.Invoke(fillOnUi);
        }

        void FinalizeResultTable(Stopwatch stopwatch, int count)
        {
            var res = CollectionViewSource.GetDefaultView(_resultList);
            Result = (IEditableCollectionView)res;
            Result.NewItemPlaceholderPosition = NewItemPlaceholderPosition.None;

            RefreshCompareModeOptions();

            _logger.LogInformation("SetResult: готово за {Ms} мс (пар: {Count})", stopwatch.ElapsedMilliseconds, count);
            SetStatus(string.Empty);
        }


        public int ThumbnailWidth
        {
            get { return Configuration.ThumbnailWidth; }
        }

        public int ThumbnailColumnWidth
        {
            get { return Configuration.ThumbnailWidth * 2 + 10; }
        }

        string _status = string.Empty;
        /// <summary>
        /// Текущее состояние приложения (выводится в строке состояния).
        /// </summary>
        public string Status
        {
            get { return _status; }
            private set
            {
                if (_status != value)
                {
                    _status = value;
                    RaisePropertyChangedEvent("Status");
                }
            }
        }

        /// <summary>
        /// Устанавливает статус (с учётом вызова из фоновых потоков) и пишет в лог.
        /// </summary>
        public void SetStatus(string status)
        {
            _logger.LogInformation("Статус: {Status}", status);
            if (Application.Current == null)
                return;
            if (Application.Current.Dispatcher.CheckAccess())
                Status = status;
            else
                Application.Current.Dispatcher.BeginInvoke(new Action(() => Status = status));
        }

        #region Context menu

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

                        BindableMenuItem[] menu = new BindableMenuItem[8];
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = firstToSecond;
                        menu[0].Command = firstToSecondCommand;

                        menu[1] = new BindableMenuItem();
                        menu[1].Name = secondToFirst;
                        menu[1].Command = secondToFirstCommand;

                        IUCommand renameImageUndoCommand = new ReplaceCommand(result, PositionInDuplPair.First, _resultList, Result);
                        ICommand renameFirstCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(renameImageUndoCommand);
                        }, arg => renameImageUndoCommand != null);
                        menu[2] = new BindableMenuItem();
                        //TODO почему то не выводится на экран символ _ в имени файла
                        menu[2].Name = String.Format("Replace _{0} to {1}", result.FirstFile.FileName, result.SecondFile.FileName);
                        menu[2].Command = renameFirstCommand;

                        IUCommand renameSecondImageUndoCommand = new ReplaceCommand(result, PositionInDuplPair.Second, _resultList,
                                                                  Result);
                        ICommand renameSecondCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(renameSecondImageUndoCommand);
                        }, arg => renameSecondImageUndoCommand != null);
                        menu[3] = new BindableMenuItem();
                        menu[3].Name = String.Format("Replace _{0} to {1}", result.SecondFile.FileName, result.FirstFile.FileName);
                        menu[3].Command = renameSecondCommand;

                        //IUCommand deleteFirstUndoCommand = new DeleteCurrentImageCommand(result, PositionInDuplPair.First, _resultList,
                        //                                         Result);
                        IUCommand deleteFirstUndoCommand = new UndoRedo.DeleteImageCommand(result, result.FirstFile, _resultList, Result);
                        ICommand deleteFirstCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(deleteFirstUndoCommand);
                        }, arg => deleteFirstUndoCommand != null);
                        menu[4] = new BindableMenuItem();
                        menu[4].Name = String.Format("Delete _{0}", result.FirstFile.FileName);
                        menu[4].Command = deleteFirstCommand;

                        IUCommand deleteSecondUndoCommand = new DeleteCurrentImageCommand(result, PositionInDuplPair.Second, _resultList,
                                                                Result);
                        ICommand deleteSecondCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(deleteSecondUndoCommand);
                        }, arg => deleteSecondUndoCommand != null);
                        menu[5] = new BindableMenuItem();
                        menu[5].Name = String.Format("Delete _{0}", result.SecondFile.FileName);
                        menu[5].Command = deleteSecondCommand;

                       /* IUCommand ignoreUCommand = new IgnorePairCommand(_resultList, Result, _ignoreStorage.IgnoreList,
                            SelectedResultIndex);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);
                        menu[2] = new BindableMenuItem();
                        menu[2].Name = Application.Current.Resources["mainwindow_contextmenu_IgnorePair"] as string;
                        menu[2].Command = ignoreCommand;*/

                        ICommand openComparatorCommand = new RelayCommand(arg =>
                        {
                            ComparatorViewModel vm = new ComparatorViewModel(result, _resultList, Result,
                                _undoRedoEngine, _windowService, Configuration);
                            _windowService.OpenComparatorWindow(vm);
                        }, arg => _windowService != null);
                        menu[6] = new BindableMenuItem();
                        menu[6].Name = Application.Current.Resources["mainwindow_contextmenu_OpenСomparator"] as string;
                        menu[6].Command = openComparatorCommand;


                        menu[7] = new BindableMenuItem();
                        menu[7].Name = "ShowDiffrenceInPercent";
                        menu[7].Command = new RelayCommand(arg =>
                            {
                                if (result != null && result is DuplPairViewModel)
                                {
                                    StringBuilder sb = new StringBuilder();
                                    sb.AppendLine(String.Format("{0} and {1}", result.FirstFile.Path, result.SecondFile.Path));

                                    DiffrenceHelper.PrintDiffrence(sb, result);

                                    _windowService.ShowMessage(sb.ToString());
                                }
                            }, arg => result != null);


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

        public Func<DuplicateGroup, BindableMenuItem[]> MenuGeneratorDuplGroup
        {
            get
            {
                return (group =>
                {
                    if (group != null)
                    {
                        /*IUCommand ignoreUCommand = new IgnoreGroupCommand(_groups, Groups, _ignoreStorage.IgnoreList);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);*/

                        BindableMenuItem[] menu = new BindableMenuItem[1];
                        /*menu[0] = new BindableMenuItem();
                        menu[0].Name = Application.Current.Resources["mainwindow_contextmenu_IgnoreGroup"] as string;
                        menu[0].Command = ignoreCommand;*/

                        ICommand openComparatorCommand = new RelayCommand(arg =>
                        {
                            ComparatorViewModel vm = new ComparatorViewModel(group, _groups, GroupsCollection,
                                _undoRedoEngine, _windowService, Configuration);
                            _windowService.OpenComparatorWindow(vm);
                        }, arg => _windowService != null);
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = Application.Current.Resources["mainwindow_contextmenu_OpenСomparator"] as string;
                        menu[0].Command = openComparatorCommand;
                        return menu;
                    }
                    return null;
                });

            }
        }

        public Func<IList, BindableMenuItem[]> MenuGeneratorMultiDuplGroup
        {
            get
            {
                return (results =>
                {
                    if (results != null && results.Count > 0)
                    {
                        BindableMenuItem[] menu = new BindableMenuItem[1];

                        IUCommand autoDeleteUCommand = new AutoDeleteMultiGroupCommand(results, _groups);
                        ICommand autoDeleteCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(autoDeleteUCommand);
                        }, arg => autoDeleteUCommand != null);
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = Application.Current.Resources["mainwindow_contextmenu_AutoDeleteBySaati"] as string;
                        menu[0].Command = autoDeleteCommand;

                        return menu;
                    }
                    return null;
                });
            }
        }

        #endregion

        #region Режим работы (алгоритм сравнения и пороговая разность)

        const int THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM = 15;
        const int THRESHOLD_DIFFERENCE_MAX_SSIM = 50;
        const int THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM = 5;
        const int THRESHOLD_DIFFERENCE_DEFAULT_SSIM = 20;

        ObservableCollection<AlgorithmComparingItem> _algorithmItems = new ObservableCollection<AlgorithmComparingItem>();
        /// <summary>
        /// Режимы (алгоритмы) сравнения изображений
        /// для выпадающего списка на панели инструментов.
        /// </summary>
        public ObservableCollection<AlgorithmComparingItem> AlgorithmItems
        {
            get { return _algorithmItems; }
        }

        ObservableCollection<int> _thresholdItems = new ObservableCollection<int>();
        /// <summary>
        /// Допустимые значения пороговой разности для текущего режима сравнения.
        /// </summary>
        public ObservableCollection<int> ThresholdItems
        {
            get { return _thresholdItems; }
        }

        /// <summary>
        /// Текущий режим (алгоритм) сравнения изображений.
        /// </summary>
        public AlgorithmComparing AlgorithmComparing
        {
            get { return Options != null ? Options.compareOptions.AlgorithmComparing : AlgorithmComparing.SquaredSum; }
            set
            {
                if (value == AlgorithmComparing)
                    return;

                RebuildThresholdItems(GetMaxThresholdDifference(value));
                ApplyCompareOptions(value, GetDefaultThresholdDifference(value));

                RaisePropertyChangedEvent("AlgorithmComparing");
                RaisePropertyChangedEvent("ThresholdDifference");
            }
        }

        /// <summary>
        /// Пороговая разность (в процентах) для текущего режима сравнения.
        /// </summary>
        public int ThresholdDifference
        {
            get { return Options != null ? Options.compareOptions.ThresholdDifference : 0; }
            set
            {
                int max = GetMaxThresholdDifference(AlgorithmComparing);
                if (value < 0)
                    value = 0;
                if (value > max)
                    value = max;
                if (value == ThresholdDifference)
                    return;

                ApplyCompareOptions(AlgorithmComparing, value);
                RaisePropertyChangedEvent("ThresholdDifference");
            }
        }

        static int GetMaxThresholdDifference(AlgorithmComparing algorithm)
        {
            return algorithm == AlgorithmComparing.SSIM ? THRESHOLD_DIFFERENCE_MAX_SSIM : THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM;
        }

        static int GetDefaultThresholdDifference(AlgorithmComparing algorithm)
        {
            return algorithm == AlgorithmComparing.SSIM ? THRESHOLD_DIFFERENCE_DEFAULT_SSIM : THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM;
        }

        static string GetCompareAlgorithmName(AlgorithmComparing algorithm)
        {
            string key = algorithm == AlgorithmComparing.SSIM
                ? "toolBar_compareAlgorithm_ssim"
                : "toolBar_compareAlgorithm_squaredSum";
            if (Application.Current != null)
            {
                object value = Application.Current.TryFindResource(key);
                if (value is string name && !String.IsNullOrEmpty(name))
                    return name;
            }
            return algorithm.ToString();
        }

        void InitializeCompareModeOptions()
        {
            _algorithmItems.Add(new AlgorithmComparingItem(AlgorithmComparing.SquaredSum, GetCompareAlgorithmName(AlgorithmComparing.SquaredSum)));
            _algorithmItems.Add(new AlgorithmComparingItem(AlgorithmComparing.SSIM, GetCompareAlgorithmName(AlgorithmComparing.SSIM)));

            // Снимок Options, созданный конструктором CoreOptions(_core), несёт дефолтные
            // значения (побочный эффект SetDefaultOptions в SetDefault), поэтому
            // берём реальные текущие значения опций из ядра:
            CoreCompareOptions coreCompare = _core.compareOptions;
            AlgorithmComparing algorithm = coreCompare.algorithmComparing;

            RebuildThresholdItems(GetMaxThresholdDifference(algorithm));

            int threshold = coreCompare.thresholdDifference;
            int max = GetMaxThresholdDifference(algorithm);
            if (threshold < 0)
                threshold = 0;
            if (threshold > max)
                threshold = max;
            if (algorithm != Options.compareOptions.AlgorithmComparing ||
                threshold != Options.compareOptions.ThresholdDifference)
                ApplyCompareOptions(algorithm, threshold);
        }

        void RebuildThresholdItems(int max)
        {
            _thresholdItems.Clear();
            for (int i = 0; i <= max; i++)
                _thresholdItems.Add(i);
        }

        /// <summary>
        /// Сохраняет режим сравнения и пороговую разность: в текущий снимок Options
        /// и в опции ядра (dll), которые используются при следующем поиске.
        /// </summary>
        void ApplyCompareOptions(AlgorithmComparing algorithm, int threshold)
        {
            if (Options != null)
            {
                Options.compareOptions.algorithmComparing = algorithm;
                Options.compareOptions.thresholdDifference = threshold;
            }
            if (_core != null)
            {
                CoreCompareOptions coreOptions = _core.compareOptions;
                coreOptions.algorithmComparing = algorithm;
                coreOptions.thresholdDifference = threshold;
                _core.compareOptions = coreOptions;
            }
        }

        void OnLanguageChanged(object sender, EventArgs e)
        {
            foreach (AlgorithmComparingItem item in _algorithmItems)
                item.Name = GetCompareAlgorithmName(item.Value);
        }

        /// <summary>
        /// После поиска объект Options заменяется на новый снимок опций ядра,
        /// поэтому обновляем выпадающие списки режимов работы.
        /// </summary>
        void RefreshCompareModeOptions()
        {
            if (Options == null)
                return;

            AlgorithmComparing algorithm = Options.compareOptions.AlgorithmComparing;
            int max = GetMaxThresholdDifference(algorithm);
            if (_thresholdItems.Count != max + 1)
                RebuildThresholdItems(max);

            int threshold = Options.compareOptions.ThresholdDifference;
            if (threshold < 0 || threshold > max)
            {
                if (threshold < 0)
                    threshold = 0;
                if (threshold > max)
                    threshold = max;
                ApplyCompareOptions(algorithm, threshold);
            }

            RaisePropertyChangedEvent("AlgorithmComparing");
            RaisePropertyChangedEvent("ThresholdDifference");
        }

        #endregion

        ICommand _executeIgnorePairCommand;
        public ICommand ExecuteIgnorePairCommand
        {
            get
            {
                return _executeIgnorePairCommand ?? (_executeIgnorePairCommand = new RelayCommand(arg =>
                {
                    IUCommand ignoreUCommand = new IgnorePairCommand(_resultList, Result, _ignoreStorage.IgnoreList);
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
                    ConfigurationViewModel vm = new ConfigurationViewModel(Configuration, _windowService, _core, Options, _thumbnailProvider);
                    _windowService.ShowDialogWindow<ConfigurationWindow>(vm);
                }, arg => Configuration != null));
            }
        }

        ICommand _setPrioritiesCommand;
        public ICommand SetPrioritiesCommand
        {
            get
            {
                return _setPrioritiesCommand ?? (_setPrioritiesCommand = new RelayCommand(arg =>
                {
                    PrioritiesViewModel vm = new PrioritiesViewModel(Configuration, _resultList, _groups);
                    _windowService.ShowDialogWindow<PrioritiesWindow>(vm);
                }, arg => true));
            }
        }

        public string Version
        {
            get { return Assembly.GetExecutingAssembly().GetName().Version.ToString(); }
        }

        /// <summary>
        /// Подсказка кнопки Undo (описание последнего отменяемого действия).
        /// </summary>
        public string UndoTooltip
        {
            get { return _undoRedoEngine != null ? _undoRedoEngine.UndoTooltip : string.Empty; }
        }

        ICommand _undoCommand;
        public ICommand UndoCommand
        {
            get
            {
                return _undoCommand ?? (_undoCommand = new RelayCommand(arg =>
                {
                    while (_undoRedoEngine.UndoEnable)
                    {
                        _undoRedoEngine.Undo();
                    }
                }, arg => _undoRedoEngine.UndoEnable));
            }
        }


        ICommand _undoAllCommand;
        public ICommand UndoAllCommand
        {
            get
            {
                return _undoAllCommand ?? (_undoAllCommand = new RelayCommand(arg =>
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

        #region Main menu

        public ObservableCollection<LanguageViewModel> LanguageCollection
        {
            get
            {
                return _languageService.LanguageCollection;
            }
        }

        public ICommand LanguageSelectedCommand
        {
            get
            {
                return _languageService.LanguageSelectedCommand;
            }
        }

        /*ICommand _convertToGroupCommand;
        public ICommand ConvertToGroupCommand
        {
            get
            {
                return _convertToGroupCommand ?? (_convertToGroupCommand = new ConvertToGroupCommand(this, _windowService));
            }
        }

        ICommand _convertToDuplPairCommand;
        public ICommand ConvertToDuplPairCommand
        {
            get
            {
                return _convertToDuplPairCommand ?? (_convertToDuplPairCommand = new ConvertToDuplPairCommand(this, _windowService));
            }
        }*/

        ICommand _convertToGroupCommand;
        public ICommand ConvertToGroupCommand
        {
            get
            {
                return _convertToGroupCommand ?? (_convertToGroupCommand = new RelayCommand(arg =>
                {
                    _windowService.ShowPleaseWait();
                    _groups.Clear();

                    GroupHelper.ConvertToGroup(_groups, _resultList);

                    if (GroupsCollection == null)
                        GroupsCollection = CollectionViewSource.GetDefaultView(_groups);
                    _resultList.Clear();
                    SelectedTabPageIndex = 1;
                    _windowService.ClosePleaseWait();
                }, arg => _resultList != null && _resultList.Any()));
            }
        }

        ICommand _convertToDuplPairCommand;
        public ICommand ConvertToDuplPairCommand
        {
            get
            {
                return _convertToDuplPairCommand ?? (_convertToDuplPairCommand = new RelayCommand(arg =>
                {
                    _resultList.Clear();
                    _windowService.ShowPleaseWait();

                    GroupHelper.GroupToList(_groups, _resultList);

                    var res = CollectionViewSource.GetDefaultView(_resultList);
                    Result = (IEditableCollectionView)res;
                    _groups.Clear();
                    SelectedTabPageIndex = 0;
                    _windowService.ClosePleaseWait();
                }, arg => _groups != null && _groups.Any()));
            }
        }

        ICommand _loadResultsCommand;
        public ICommand LoadResultsCommand
        {
            get
            {
                return _loadResultsCommand ?? (_loadResultsCommand = new RelayCommand(arg =>
                {
                    string directory = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Result");
                    if (Directory.Exists(directory))
                    {
                        OpenFileDialog dialog = new OpenFileDialog();
                        dialog.Filter = "Result Files(*.anr)|*.anr";
                        dialog.CheckFileExists = true;
                        dialog.InitialDirectory = directory;

                        if (dialog.ShowDialog() == true)
                        {
                            LoadResults(dialog.FileName);
                            Configuration.LastResultFile = dialog.FileName;
                        }
                    }
                }, arg =>
                {
                    string directory = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, "Result");
                    if (!Directory.Exists(directory))
                        return false;
                    string[] files = System.IO.Directory.GetFiles(directory, "*.anr");
                    if (files.Any())
                        return true;
                    return false;
                }));
            }
        }

        private void LoadResults(string resPath)
        {
           //_windowService.ShowPleaseWait();
            _logger.LogInformation("LoadResults: загрузка результата из файла: {File}", resPath);
            var progressDialogViewModel = new ProgressDialogViewModel();
           Task.Factory.StartNew(() =>
           {
               var loadWatch = Stopwatch.StartNew();
               var readed = SerializeHelper<DuplPairViewModel[]>.Load(resPath);
               if (readed != null)
               {
                   //List<DuplPairViewModel> filtered = new List<DuplPairViewModel>(readed.Length);
                   _logger.LogInformation("LoadResults: файл прочитан за {Ms} мс (пар: {Count})",
                       loadWatch.ElapsedMilliseconds, readed.Length);
                   progressDialogViewModel.ProgressMax = (uint)readed.Length;
                   try
                   {
                       int statusStep = Math.Max(1, readed.Length / 20);
                       int processed = 0;

                   foreach (var item in readed)
                   {
                       progressDialogViewModel.ProgressMessage = item.FirstFile.FileName;

                       SaatiHelper.CalculateIndex(item, Configuration);
                       item.FillFolderAreDiffrent();
                       //TODO переделать чтобы запускалось по требоыванию
                       //долго выполняется
                       //item.SetNeighboursFileNames();
                      // if (item.CheckAllExist())
                       //    filtered.Add(item);

                       progressDialogViewModel.Progress++;
                       processed++;
                       if (processed % statusStep == 0)
                           SetStatus(string.Format("Загрузка результатов… ({0}/{1})", processed, readed.Length));
                       if (progressDialogViewModel.IsCancelled)
                           break;
                   }

                    SetStatus(string.Format("Обработка завершена ({0} пар), показ в таблице…", readed.Length));
                   }
                   catch (Exception ex)
                   {
                       _logger.LogError(ex, "LoadResults: ошибка обработки файла {File}", resPath);
                       throw;
                   }
                   try
                   {
                       SetResult(readed);
                   }
                   catch (Exception)
                   {
                       throw;
                   }

                   Title = String.Format("AntiDupl.NET.WPF - {0}", Path.GetFileNameWithoutExtension(Configuration.LastResultFile));
               }
           })
            .ContinueWith(t =>
            {
                _windowService.CloseProgressWindow();
            },TaskScheduler.FromCurrentSynchronizationContext());
           _windowService.OpenProgressWindow(progressDialogViewModel);
        }


        ICommand _saveResultsCommand;
        public ICommand SaveResultsCommand
        {
            get
            {
                return _saveResultsCommand ?? (_saveResultsCommand = new RelayCommand(arg =>
                {
                    string directory = DirectoryHelper.ResultDirectory;

                    SaveFileDialog dialog = new SaveFileDialog()
                    {
                        Filter = "Result Files(*.anr)|*.anr",
                        InitialDirectory = directory
                    };

                    if (dialog.ShowDialog() == true)
                    {
                        SerializeHelper<DuplPairViewModel[]>.Save(_resultList.ToArray(), dialog.FileName);
                        Configuration.LastResultFile = dialog.FileName;
                        Title = String.Format("AntiDupl.NET.WPF - {0}", Path.GetFileNameWithoutExtension(Configuration.LastResultFile));
                    }
                }, arg => _resultList != null && _resultList.Any()));
            }
        }

        #endregion

        private int _selectedTabPageIndex;
        public int SelectedTabPageIndex
        {
            get { return _selectedTabPageIndex; }
            set
            {
                if (_selectedTabPageIndex != value)
                {
                    _selectedTabPageIndex = value;
                    this.RaisePropertyChangedEvent("SelectedTabPageIndex");
                    //if (Result != null)
                    //{
                    //    if (_selectedTabPageIndex == COMPARE_TAB_INDEX)
                    //        ((ICollectionView)Result).MoveCurrentToPosition(SelectedResultIndex);
                    //    else if (_selectedTabPageIndex == DUPL_IMAGE_TAB_INDEX)
                    //        SelectedResultIndex = ((ICollectionView)Result).CurrentPosition;
                    //}
                }
            }
        }
  

        #region Compare mode buttons

        private static Predicate<object> CheckCurrentResultFromCollectionView()
        {
            return arg => arg != null && arg is ICollectionView
                && ((ICollectionView)arg).CurrentItem != null
                && ((ICollectionView)arg).CurrentItem is DuplPairViewModel;
        }


        ICommand _renameFirstLikeSecondCommand;
        public ICommand RenameFirstLikeSecondCommand
        {
            get
            {
                return _renameFirstLikeSecondCommand ?? (_renameFirstLikeSecondCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel result = ((ICollectionView)arg).CurrentItem as DuplPairViewModel;

                    string firstNewName = RenameHelper.GetSimilarName(result.FirstFile.Path, result.SecondFile.Path);
                    IUCommand command = new RenameLikeCommand(result, firstNewName, PositionInDuplPair.First);
                    _undoRedoEngine.ExecuteCommand(command);
                }, CheckCurrentResultFromCollectionView()));
            }
        }

        public string RenameFirstLikeSecondText
        {
            get
            {
                if (Result != null)
                {
                    var col = Result as ICollectionView;
                    if (col != null && col.CurrentItem != null)
                    {
                        DuplPairViewModel result = col.CurrentItem as DuplPairViewModel;
                        if (result != null)
                        {
                            var newName = RenameHelper.GetSimilarName(result.FirstFile.Path, result.SecondFile.Path);
                            string mask = Application.Current.Resources["mainwindow_tooltip_RenameTo"] as string;
                            string oldName = Path.GetFileName(result.FirstFile.Path);
                            return String.Format(mask, oldName, Path.GetFileName(newName));
                        }
                    }
                }
                return "";
            }
        }

        ICommand _renameSecondLikeFirstCommand;
        public ICommand RenameSecondLikeFirstCommand
        {
            get
            {
                return _renameSecondLikeFirstCommand ?? (_renameSecondLikeFirstCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel result = ((ICollectionView)arg).CurrentItem as DuplPairViewModel;

                    string secondNewName = RenameHelper.GetSimilarName(result.SecondFile.Path, result.FirstFile.Path);
                    IUCommand command = new RenameLikeCommand(result, secondNewName, PositionInDuplPair.Second);
                    _undoRedoEngine.ExecuteCommand(command);
                }, CheckCurrentResultFromCollectionView()));
            }
        }

        public string RenameSecondLikeFirstText
        {
            get
            {
                if (Result != null)
                {
                    var col = Result as ICollectionView;
                    if (col != null && col.CurrentItem != null)
                    {
                        DuplPairViewModel result = col.CurrentItem as DuplPairViewModel;
                        if (result != null)
                        {
                            var newName = RenameHelper.GetSimilarName(result.SecondFile.Path, result.FirstFile.Path);
                            string mask = Application.Current.Resources["mainwindow_tooltip_RenameTo"] as string;
                            string oldName = Path.GetFileName(result.SecondFile.Path);
                            return String.Format(mask, oldName, Path.GetFileName(newName));
                        }
                    }
                }
                return "";
            }
        }

        ICommand _moveFirstToNeighbourDirectoryCommand;
        public ICommand MoveFirstToNeighbourDirectoryCommand
        {
            get
            {
                return _moveFirstToNeighbourDirectoryCommand ?? (_moveFirstToNeighbourDirectoryCommand = new RelayCommand(arg =>
                {

                }, DirectoryIsDifferent()));
            }
        }
        ICommand _moveSecondToNeighbourDirectoryCommand;
        public ICommand MoveSecondToNeighbourDirectoryCommand
        {
            get
            {
                return _moveSecondToNeighbourDirectoryCommand ?? (_moveSecondToNeighbourDirectoryCommand = new RelayCommand(arg =>
                {

                }, DirectoryIsDifferent()));
            }
        }

        private static Predicate<object> DirectoryIsDifferent()
        {
            return arg =>
            {
                if (arg == null)
                    return false;
                if (!(arg is DuplPairViewModel))
                    return false;
                var res = arg as DuplPairViewModel;
                if (Path.GetDirectoryName(res.FirstFile.Path) != Path.GetDirectoryName(res.SecondFile.Path))
                    return true;
                return false;
            };
        }

        ICommand _moveAndRenameCommand;
        public ICommand MoveAndRenameCommand
        {
            get
            {
                return _moveAndRenameCommand ?? (_moveAndRenameCommand = new RelayCommand(arg =>
                {

                }, DirectoryIsDifferent()));
            }
        }

        ICommand _replaseFirstCommand;
        public ICommand ReplaseFirstCommand
        {
            get
            {
                return _replaseFirstCommand ?? (_replaseFirstCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel result = ((ICollectionView)arg).CurrentItem as DuplPairViewModel;
                    IUCommand renameImageCommand = new ReplaceCommand(result, PositionInDuplPair.First, _resultList,
                        Result);
                    _undoRedoEngine.ExecuteCommand(renameImageCommand);
                }, CheckCurrentResultFromCollectionView()));
            }
        }

        ICommand _replaseSecondCommand;
        public ICommand ReplaseSecondCommand
        {
            get
            {
                return _replaseSecondCommand ?? (_replaseSecondCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel result = ((ICollectionView)arg).CurrentItem as DuplPairViewModel;
                    IUCommand renameImageCommand = new ReplaceCommand(result, PositionInDuplPair.Second, _resultList, Result);
                    _undoRedoEngine.ExecuteCommand(renameImageCommand);
                }, CheckCurrentResultFromCollectionView()));
            }
        }

        #endregion

        string _title = "AntiDupl.NET.WPF";
        public string Title
        {
            get
            {
                return _title;
            }
            set
            {
                _title = value;
                RaisePropertyChangedEvent("Title");
            }
        }

        string _search;

        ICommand _filterResultCommand;
        public ICommand FilterResultCommand
        {
            get
            {
                return _filterResultCommand ?? (_filterResultCommand = new RelayCommand(arg =>
                {
                    _search = arg as string;
                    ((ICollectionView)Result).Filter = new Predicate<object>(Contains);
                    ((ICollectionView)Result).Refresh();

                }, arg => !String.IsNullOrEmpty((string)arg)));
            }
        }

        public bool Contains(object obj)
        {
            DuplPairViewModel duplPair = obj as DuplPairViewModel;

            if (duplPair.FirstFile.Path.Contains(_search))
                return true;
            if (duplPair.SecondFile.Path.Contains(_search))
                return true;
            return false;
        }



        ICommand _clearFilterResultCommand;
        public ICommand ClearFilterResultCommand
        {
            get
            {
                return _clearFilterResultCommand ?? (_clearFilterResultCommand = new RelayCommand(arg =>
                {
                    ((ICollectionView)Result).Filter = null;
                }));
            }
        }

        ICommand _refreshCommand;
        public ICommand RefreshCommand
        {
            get
            {
                return _refreshCommand ?? (_refreshCommand = new RelayCommand(arg =>
                {
                    var progressDialogViewModel = new ProgressDialogViewModel();
                    List<DuplPairViewModel> forDelete = new List<DuplPairViewModel>();

                    Task.Factory.StartNew(() =>
                    {
                        progressDialogViewModel.ProgressMax = (uint)_resultList.Count;
                        
                            try
                            {
								foreach (var item in _resultList)
								{
									progressDialogViewModel.ProgressMessage = item.FirstFile.FileName;
									//Debug.WriteLine("{0} width {1}, {2} width {3}",
									//item.FirstFile.FileName, item.FirstFile.Width, item.SecondFile.FileName, item.SecondFile.Width);
									//if (item.FirstFile.FileName == "15441_6PRej_D3Mp0.jpg")
									//    Debugger.Break();

									if (!item.CheckAllExist())
										 forDelete.Add(item);


									progressDialogViewModel.Progress++;
									if (progressDialogViewModel.IsCancelled)
										break;
								}
                            }
                            catch (Exception ex)
                            {
                                throw;
                            }
                    })
                    .ContinueWith(t =>
                    {
                        foreach (var item in forDelete)
                        {
                            _resultList.Remove(item);
                        }
                        ((ICollectionView)Result).Refresh();
                        _windowService.CloseProgressWindow();
                    },TaskScheduler.FromCurrentSynchronizationContext());
                   _windowService.OpenProgressWindow(progressDialogViewModel);
                }));
            }
        }

        ICommand _showDiffrenceInPercent;
        public ICommand ShowDiffrenceInPercent
        {
            get
            {
                return _showDiffrenceInPercent ?? (_showDiffrenceInPercent = new RelayCommand(arg =>
                {
                    DuplPairViewModel pair = arg as DuplPairViewModel;
                    if (pair != null)
                    {
                        StringBuilder sb = new StringBuilder();
                        sb.AppendLine(String.Format("{0} and {1}", pair.FirstFile.Path, pair.SecondFile.Path));

                        DiffrenceHelper.PrintDiffrence(sb, pair);

                        _windowService.ShowMessage(sb.ToString());
                    }
                }, arg => _resultList != null));
            }
        }

        private ICommand _closeCmd;
        public ICommand CloseCmd
        {
            get { return _closeCmd ?? (_closeCmd = new RelayCommand(arg => { Application.Current.Shutdown(); })); }
        }

    }
}
