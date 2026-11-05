using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Diagnostics;
using System.Globalization;
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
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Logging;

namespace AntiDupl.NET.WPF.ViewModel
{
    public class MainViewModel : PropertyChangedBase
    {
        const int DUPL_IMAGE_TAB_INDEX = 0;
        const int COMPARE_TAB_INDEX = 2;
        
        CoreLib _core;
        IWindowService _windowService;
        ClipboardService _clipboardService;
        /// <summary>Запуск внешней diff-утилиты (diff-утилита; конкретный класс — политика команды).</summary>
        ExternalToolLauncher _externalToolLauncher;
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

        /// <summary>
        /// C#-пайплайн deep (DINOv2) поиска (AntiDupl.NET.Deep). Инжектится App.xaml.cs
        /// после BuildServiceProvider; null — deep-режим недоступен.
        /// </summary>
        public DeepSearchService DeepSearchService { get; private set; }

        /// <summary>Логгер deep-пайплайна (для команд/окон, работающих с моделями).</summary>
        public IDeepLogger DeepLogger { get; private set; }

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
                this.RaisePropertyChangedEvent();
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
            DeepSearchService deepSearchService,
            IDeepLogger deepLogger,
            ILoggerFactory loggerFactory,
            ICoreLocationSync locationSync)
        {
            _logger = loggerFactory.CreateLogger<MainViewModel>();
            _languageService = languageService;
            _windowService = new WindowService(_logger);
            _clipboardService = new ClipboardService();
            _externalToolLauncher = new ExternalToolLauncher();

            // Deep-сервис и логгер — конструкторной инъекцией из DI (без post-build-присваивания).
            DeepSearchService = deepSearchService;
            DeepLogger = deepLogger;

            _thumbnailProvider = thumbnailProvider;
            _core = coreLib;
            LocationsModel = LocationsModel.Load(locationSync);
            Options = new CoreOptions(_core);
            _ignoreStorage = IgnoreStorage.Load();

            Configuration = configuration;
            // Фильтр имён ignore-списка живёт в configuration.xml (зеркало WF options.xml).
            LocationsModel.IgnoreFilenameFilter = Configuration.IgnoreFilenameFilter;
            Configuration.PropertyChanged += OnConfigurationPropertyChanged;

            _languageService.LanguageChanged += OnLanguageChanged;
            InitializeCompareModeOptions();

            _logger.LogInformation("MainViewModel: инициализирован (режим={Algorithm}, порог={Threshold}%)",
                Options.compareOptions.AlgorithmComparing, Options.compareOptions.ThresholdDifference);

            _undoRedoEngine = undoRedoEngine;
            _undoRedoEngine.PropertyChanged += OnUndoRedoEnginePropertyChanged;

            _viewModeModel = viewModeModel;
            // Вид таблицы результатов (пары/группы): восстановить из настроек и
            // сохранять обратно при смене режима (configuration.xml при закрытии).
            _viewModeModel.ViewMode = Configuration.ResultsViewMode;
            _viewModeModel.PropertyChanged += OnViewModeModelPropertyChanged;

            _thumbnailProvider.PropertyChanged += OnThumbnailProviderPropertyChanged;
        }

        void OnViewModeModelPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(ViewModeModel.ViewMode))
                Configuration.ResultsViewMode = _viewModeModel.ViewMode;
        }

        public void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _logger.LogInformation("MainViewModel: закрытие окна");
            Configuration.PropertyChanged -= OnConfigurationPropertyChanged;
            _viewModeModel.PropertyChanged -= OnViewModeModelPropertyChanged;
            _undoRedoEngine.PropertyChanged -= OnUndoRedoEnginePropertyChanged;
            _languageService.LanguageChanged -= OnLanguageChanged;
            //_dllModel.SaveSettings();
            _core.Dispose();

            LocationsModel.Save();
            SaveResultsColumnsVisibility();
            Configuration.Save();
            _ignoreStorage.Save();
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

            // выбор колонок: применяем сохранённую видимость колонок таблицы результатов.
            ApplyResultsColumnsVisibility();
        }

        /// <summary>
        /// выбор колонок: применяет к таблице результатов сохранённую видимость колонок
        /// (Configuration.ResultsColumns, по порядковому индексу колонок).
        /// </summary>
        public void ApplyResultsColumnsVisibility()
        {
            bool[] visible = Configuration.ResultsColumns?.Visible;
            if (ResultGrid == null || visible == null || visible.Length == 0)
                return;

            for (int i = 0; i < ResultGrid.Columns.Count && i < visible.Length; i++)
                ResultGrid.Columns[i].Visibility = visible[i] ? Visibility.Visible : Visibility.Collapsed;
        }

        /// <summary>
        /// выбор колонок: читает текущую видимость колонок таблицы результатов
        /// в Configuration.ResultsColumns (сохраняется в configuration.xml).
        /// </summary>
        public void SaveResultsColumnsVisibility()
        {
            if (ResultGrid == null)
                return;

            bool[] visible = new bool[ResultGrid.Columns.Count];
            for (int i = 0; i < visible.Length; i++)
                visible[i] = ResultGrid.Columns[i].Visibility == Visibility.Visible;
            Configuration.ResultsColumns = new ResultsColumnsOptions { Visible = visible };
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

        ICommand _openPathCommand;
        public ICommand OpenPathCommand
        {
            get
            {
                return _openPathCommand ?? (_openPathCommand = new RelayCommand(arg =>
                {
                    // Окно путей (: вкладки Search/Ignore/Valid/Delete).
                    LocationsModel.IgnoreFilenameFilter = Configuration.IgnoreFilenameFilter;
                    PathViewModel vm = new PathViewModel(LocationsModel);
                    _windowService.ShowDialogWindow<PathWindow>(vm);
                    // Фильтр имён, изменённый в окне, сохраняем в configuration.xml и в ядро.
                    if (!String.Equals(LocationsModel.IgnoreFilenameFilter, Configuration.IgnoreFilenameFilter))
                    {
                        Configuration.IgnoreFilenameFilter = LocationsModel.IgnoreFilenameFilter;
                        Configuration.Save();
                    }
                    LocationsModel.CopyToDll();
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
        /// Ссылка на таблицу результатов (заполняется вью из MainWindow после загрузки).
        /// </summary>
        public System.Windows.Controls.DataGrid ResultGrid { get; set; }

        ICommand _selectAllCommand;
        /// <summary>
        /// Выбрать все строки таблицы результатов (текущая отфильтрованная видовка).
        /// Основа для пакетных операций из контекстного меню (ignore/delete всех видимых пар).
        /// </summary>
        public ICommand SelectAllCommand
        {
            get
            {
                return _selectAllCommand ?? (_selectAllCommand = new RelayCommand(arg =>
                {
                    ResultGrid?.SelectAll();
                }, arg => ResultGrid != null && Result != null
                    && Result is ICollectionView view && !view.IsEmpty));   // .NET 10: ICollectionView в System.ComponentModel, CollectionCount/IsSourceEmpty убраны
            }
        }

        ICommand _copyCommand;
        /// <summary>
        /// Копировать результаты в буфер обмена (Ctrl+C): выбранные строки,
        /// либо (если выделение пусто) все видимые пары; parameter = пара из
        /// контекстного меню одиночного ряда.
        /// </summary>
        public ICommand CopyCommand
        {
            get
            {
                return _copyCommand ?? (_copyCommand = new RelayCommand(arg =>
                {
                    _clipboardService.CopyText(ClipboardContentBuilder.Build(GetPairsForClipboard(arg as DuplPairViewModel)));
                }, arg => Result != null));
            }
        }

        ICommand _copyPathsCommand;
        /// <summary>
        /// Копировать пути файлов результатов в буфер обмена:
        /// выбранные строки либо все видимые пары; parameter = пара из контекстного меню.
        /// </summary>
        public ICommand CopyPathsCommand
        {
            get
            {
                return _copyPathsCommand ?? (_copyPathsCommand = new RelayCommand(arg =>
                {
                    _clipboardService.CopyText(ClipboardContentBuilder.BuildPaths(GetPairsForClipboard(arg as DuplPairViewModel)));
                }, arg => Result != null));
            }
        }

        /// <summary>
        /// Пара(ы) для копирования: если передана пара (контекстное меню одиночного ряда) —
        /// она; иначе выделенные строки таблицы (без дублей); иначе все видимые (отфильтрованные) пары.
        /// </summary>
        List<DuplPairViewModel> GetPairsForClipboard(DuplPairViewModel single)
        {
            if (single != null)
                return new List<DuplPairViewModel> { single };

            if (ResultGrid != null && ResultGrid.SelectedItems.Count > 0)
            {
                List<DuplPairViewModel> result = new List<DuplPairViewModel>();
                HashSet<DuplPairViewModel> seen = new HashSet<DuplPairViewModel>(ReferenceEqualityComparer<DuplPairViewModel>.DefaultInstance);
                foreach (var item in ResultGrid.SelectedItems)
                {
                    if (item is DuplPairViewModel pair && seen.Add(pair))
                        result.Add(pair);
                }
                return result;
            }

            List<DuplPairViewModel> all = new List<DuplPairViewModel>();
            if (Result is IEnumerable enumerable)
            {
                foreach (var item in enumerable)
                    if (item is DuplPairViewModel pair)
                        all.Add(pair);
            }
            return all;
        }

        #region Hot keys (mirror of WF SelectHotKeysForm + HotKeyOptions)

        /// <summary>Поднимаем в MainWindow для пересборки InputBindings после изменения привязок.</summary>
        public event EventHandler HotKeysChanged;

        ICommand _openHotKeysCommand;
        /// <summary>Открывает окно выбора горячих клавиш (зеркало WF View → SelectHotKeysForm).</summary>
        public ICommand OpenHotKeysCommand
        {
            get
            {
                return _openHotKeysCommand ?? (_openHotKeysCommand = new RelayCommand(arg =>
                {
                    SelectHotKeysViewModel dialogViewModel = new SelectHotKeysViewModel(Configuration.HotKeys);
                    bool? result = _windowService.ShowDialogWindow<View.SelectHotKeysWindow>(dialogViewModel);
                    if (result == true)
                    {
                        Configuration.HotKeys = dialogViewModel.Options;
                        Configuration.Save();
                        HotKeysChanged?.Invoke(this, EventArgs.Empty);
                    }
                }, arg => true));
            }
        }

        #region Help/About (зеркало WF Help-меню и AboutProgramPanel)

        /// <summary>Открыть страницу справки (URL зависит от текущего языка).</summary>
        ICommand _openHelpCommand;
        public ICommand OpenHelpCommand
        {
            get
            {
                return _openHelpCommand ?? (_openHelpCommand = new RelayCommand(arg =>
                {
                    CultureInfo culture = _languageService != null ? _languageService.Language : CultureInfo.CurrentUICulture;
                    bool russian = culture != null && culture.TwoLetterISOLanguageName == "ru";
                    OpenUrl(VersionInfoProvider.GetHelpUrl(russian));
                }, arg => true));
            }
        }

        /// <summary>Окно "О программе" (версия, сайт, справка, проверка обновлений).</summary>
        ICommand _openAboutCommand;
        public ICommand OpenAboutCommand
        {
            get
            {
                return _openAboutCommand ?? (_openAboutCommand = new RelayCommand(arg =>
                {
                    AboutViewModel dialogViewModel = new AboutViewModel(_core);
                    _windowService.ShowDialogWindow<View.AboutWindow>(dialogViewModel);
                }, arg => true));
            }
        }

        /// <summary>
        /// Проверить наличие нового релиза по version.xml на сайте (зеркало WF
        /// NewVersionMenuItem) и сообщить о результате.
        /// </summary>
        /// <summary>Защита от повторного клика, пока проверка идёт в фоне.</summary>
        bool _checkForUpdatesRunning;

        ICommand _checkForUpdatesCommand;
        /// <summary>
        /// Проверить новую версию по version.xml на сайте (фон, таймаут 15 c, зеркало WF
        /// NewVersionMenuItem). Результат — MessageBox; любой сетевой сбой — «не удалось».
        /// </summary>
        public ICommand CheckForUpdatesCommand
        {
            get
            {
                return _checkForUpdatesCommand ?? (_checkForUpdatesCommand = new RelayCommand(
                    arg => CheckForUpdatesAsync(),
                    arg => !_checkForUpdatesRunning));
            }
        }

        async void CheckForUpdatesAsync()
        {
            if (_checkForUpdatesRunning)
            {
                return;
            }
            _checkForUpdatesRunning = true;
            try
            {
                string latest = await VersionInfoProvider.GetLatestVersionFromWebAsync();
                if (latest == null)
                {
                    _windowService.ShowMessage(
                        Application.Current.Resources["aboutwindow_check_failed"] as string);
                    return;
                }
                _windowService.ShowMessage(String.Format(
                    Application.Current.Resources["aboutwindow_update_available"] as string,
                    latest,
                    VersionInfoProvider.BaseUrl));
            }
            finally
            {
                _checkForUpdatesRunning = false;
            }
        }

        static void OpenUrl(string url)
        {
            Process.Start(new ProcessStartInfo(url) { UseShellExecute = true });
        }

        #endregion

        /// <summary>
        /// Команда горячего действия, привязываемая к клавише (зеркало WF ApplyHotKeysCommand).
        /// Действия над текущей парой из Result; ShowNeighbours переключает на вкладку сравнения.
        /// </summary>
        public ICommand GetHotKeyCommand(HotKeyAction action)
        {
            switch (action)
            {
                case HotKeyAction.DeleteFirst:
                    return new RelayCommand(arg => DeleteImageCommand.Execute(GetCurrentPair()?.FirstFile), arg => CanActOnCurrentPair());
                case HotKeyAction.DeleteSecond:
                    return new RelayCommand(arg => DeleteImageCommand.Execute(GetCurrentPair()?.SecondFile), arg => CanActOnCurrentPair());
                case HotKeyAction.DeleteBoth:
                    return new RelayCommand(arg =>
                    {
                        DeleteImageCommand.Execute(GetCurrentPair()?.FirstFile);
                        DeleteImageCommand.Execute(GetCurrentPair()?.SecondFile);
                    }, arg => CanActOnCurrentPair());
                case HotKeyAction.RenameFirstToSecond:
                    return new RelayCommand(arg => RenameFirstLikeSecondCommand.Execute(GetCurrentPair()), arg => CanActOnCurrentPair());
                case HotKeyAction.RenameSecondToFirst:
                    return new RelayCommand(arg => RenameSecondLikeFirstCommand.Execute(GetCurrentPair()), arg => CanActOnCurrentPair());
                case HotKeyAction.Mistake:
                    return new RelayCommand(arg => ExecuteIgnorePairCommand.Execute(GetCurrentPair()), arg => CanActOnCurrentPair());
                case HotKeyAction.ShowNeighbours:
                    return new RelayCommand(arg => SelectedTabPageIndex = 1, arg => CanActOnCurrentPair());
                case HotKeyAction.OpenImageDiff:
                    return OpenImageDiffCommand;
                case HotKeyAction.DeleteDefect:
                    return DeleteDefectCommand;
                default:
                    return null;
            }
        }

        DuplPairViewModel GetCurrentPair()
        {
            return (Result as System.ComponentModel.ICollectionView)?.CurrentItem as DuplPairViewModel;
        }

        bool CanActOnCurrentPair()
        {
            return GetCurrentPair() != null;
        }

        /// <summary>
        /// Открыть текущую пару во внешней diff-утилите (зеркало WF
        /// ImageDiffOpener; путь/аргументы — Configuration.ImageDiffExecutablePath/Arguments).
        /// </summary>
        ICommand _openImageDiffCommand;
        public ICommand OpenImageDiffCommand
        {
            get
            {
                return _openImageDiffCommand ?? (_openImageDiffCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel pair = GetCurrentPair();
                    if (pair == null)
                        return;
                    if (String.IsNullOrEmpty(Configuration.ImageDiffExecutablePath))
                    {
                        _windowService.ShowMessage(
                            Application.Current.Resources["comparatorWindow_externalToolNotSet"] as string);
                        return;
                    }
                    string error = _externalToolLauncher.OpenFiles(pair.FirstFile.Path, pair.SecondFile.Path,
                        Configuration.ImageDiffExecutablePath, Configuration.ImageDiffExecutableArguments);
                    if (error != null)
                        _windowService.ShowMessage(error);
                }, arg => CanActOnCurrentPair()));
            }
        }

        /// <summary>
        /// Удаление дефекта из текущей строки результата (дефекты; локальный
        /// вариант зеркала WF ApplyToResult(DeleteDefect), см. UndoRedo.DeleteDefectCommand).
        /// </summary>
        ICommand _deleteDefectCommand;
        public ICommand DeleteDefectCommand
        {
            get
            {
                return _deleteDefectCommand ?? (_deleteDefectCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel pair = GetCurrentPair();
                    if (pair == null || !pair.IsDefect)
                        return;
                    ImageInfoClass file = pair.FirstFile;
                    if (file == null || String.IsNullOrEmpty(file.Path) || !File.Exists(file.Path))
                        return;
                    IUCommand uCommand = new UndoRedo.DeleteDefectCommand(pair, file, _resultList);
                    _undoRedoEngine.ExecuteCommand(uCommand);
                }, arg =>
                {
                    DuplPairViewModel pair = GetCurrentPair();
                    return pair != null && pair.IsDefect;
                }));
            }
        }

        #endregion

        #region Панель инструментов (зеркало WF MainToolStrip)

        /// <summary>
        /// Режим пар (вкладка 0). Кнопки действий панели (Mistake/PerformHint/Delete…)
        /// имеют смысл только здесь — в WF они видны только в режимах
        /// VerticalPairTable/HorizontalPairTable (MainToolStrip.SetViewMode).
        /// </summary>
        public bool IsPairMode
        {
            get { return _selectedTabPageIndex == 0; }
        }

        /// <summary>
        /// Пометить текущую пару как ошибочную (зеркало WF MistakesButton).
        /// WPF — локальный ignore-хранилище (дефекты), а не mistakeDataBase ядра.
        /// </summary>
        ICommand _toolMistakeCommand;
        public ICommand ToolMistakeCommand
        {
            get
            {
                return _toolMistakeCommand ?? (_toolMistakeCommand = new RelayCommand(arg =>
                {
                    DuplPairViewModel pair = GetCurrentPair();
                    if (pair != null)
                        ExecuteIgnorePairCommand.Execute(pair);
                }, arg => CanActOnCurrentPair()));
            }
        }

        /// <summary>
        /// Автоматически обработать текущую пару по подсказке ядра (зеркало WF
        /// PerformHintButton: ApplyToResult(PerformHint, Selected)). После успеха
        /// результаты перечитываются из ядра (аналог WF-обновления списка).
        /// </summary>
        ICommand _performHintCommand;
        public ICommand PerformHintCommand
        {
            get
            {
                return _performHintCommand ?? (_performHintCommand = new RelayCommand(arg =>
                {
                    if (!CanActOnCurrentPair() || _core == null)
                        return;
                    if (_core.ApplyToResult(CoreDll.LocalActionType.PerformHint, CoreDll.TargetType.Selected))
                    {
                        DuplPairViewModel[] result = (SearchCommand as SearchDllCommand)?.GetResults();
                        SetResult(result);
                    }
                }, arg => CanActOnCurrentPair() && _core != null &&
                         _core.CanApply(CoreDll.ActionEnableType.PerformHint)));
            }
        }

        /// <summary>Удалить первую картинку текущей пары (зеркало WF DeleteFirsts*Button).</summary>
        ICommand _toolDeleteFirstCommand;
        public ICommand ToolDeleteFirstCommand
        {
            get
            {
                return _toolDeleteFirstCommand ?? (_toolDeleteFirstCommand = new RelayCommand(arg =>
                    DeleteImageCommand.Execute(GetCurrentPair()?.FirstFile), arg => CanActOnCurrentPair()));
            }
        }

        /// <summary>Удалить вторую картинку текущей пары (зеркало WF DeleteSeconds*Button).</summary>
        ICommand _toolDeleteSecondCommand;
        public ICommand ToolDeleteSecondCommand
        {
            get
            {
                return _toolDeleteSecondCommand ?? (_toolDeleteSecondCommand = new RelayCommand(arg =>
                    DeleteImageCommand.Execute(GetCurrentPair()?.SecondFile), arg => CanActOnCurrentPair()));
            }
        }

        /// <summary>Удалить обе картинки текущей пары (зеркало WF DeleteBothes*Button).</summary>
        ICommand _toolDeleteBothCommand;
        public ICommand ToolDeleteBothCommand
        {
            get
            {
                return _toolDeleteBothCommand ?? (_toolDeleteBothCommand = new RelayCommand(arg =>
                {
                    DeleteImageCommand.Execute(GetCurrentPair()?.FirstFile);
                    DeleteImageCommand.Execute(GetCurrentPair()?.SecondFile);
                }, arg => CanActOnCurrentPair()));
            }
        }

        #endregion

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
                RaisePropertyChangedEvent();
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
                RaisePropertyChangedEvent();
            }
        }

        void SetResultCountStatus()
        {
            // Статус результата в соответствии с текущим видом таблицы.
            if (_viewModeModel.ViewMode == ViewMode.Group && _groups != null)
                SetStatus(LocalizationHelper.S("status_ready", Noun(_groups.Count, "группа", "группы", "групп", "group", "groups")));
            else if (_resultList != null && _resultList.Any())
                SetStatus(LocalizationHelper.S("status_ready", Noun(_resultList.Count, "пара", "пары", "пар", "pair", "pairs")));
            else
                SetStatus(string.Empty);
        }

        /// <summary>
        /// Склонение слова результата по языку интерфейса (RU — русские формы, иначе EN).
        /// </summary>
        string Noun(int n, string ruOne, string ruFew, string ruMany, string enOne, string enMany)
        {
            bool russian = _languageService != null &&
                           _languageService.Language != null &&
                           _languageService.Language.Name.StartsWith("ru", StringComparison.OrdinalIgnoreCase);
            if (russian)
                return Plural(n, ruOne, ruFew, ruMany);
            return $"{n} {(n == 1 ? enOne : enMany)}";
        }

        private static string Plural(int n, string one, string few, string many)
        {
            int m10 = n % 10, m100 = n % 100;
            if (m10 == 1 && m100 != 11)
                return $"{n} {one}";
            if (m10 >= 2 && m10 <= 4 && (m100 < 12 || m100 > 14))
                return $"{n} {few}";
            return $"{n} {many}";
        }

        public void SetResult(DuplPairViewModel[] result)
        {
            int count = result != null ? result.Length : 0;
            _logger.LogInformation("SetResult: получено пар: {Count}", count);
            SetStatus(LocalizationHelper.S("status_showingResults", count));
            Stopwatch stopwatch = Stopwatch.StartNew();

            Action fillOnUi = () =>
            {
                if (_groups != null)
                    _groups.Clear();
                if (_resultList != null)
                    _resultList.Clear();
                // Режим показа (пары/группы) не сбрасываем — сохраняется между поисками
                // и перезапусками (Configuration.ResultsViewMode).
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
                        SetStatus(LocalizationHelper.S("status_fillingTable", Math.Min(index, count), count));
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

            // Вид таблицы — тот, что выбран пользователем (сохраняется в Configuration.ResultsViewMode):
            // «Группами» — сразу строим группы; «Парами» — оставляем пары в таблице.
            if (count > 0 && _viewModeModel.ViewMode == ViewMode.Group)
            {
                GroupHelper.ConvertToGroup(_groups, _resultList);
                _resultList.Clear();

                if (GroupsCollection == null)
                    GroupsCollection = CollectionViewSource.GetDefaultView(_groups); // setter сам ставит вкладку 1
                else
                    SelectedTabPageIndex = 1;
            }
            else
            {
                SelectedTabPageIndex = 0;
            }

            _logger.LogInformation("SetResult: готово за {Ms} мс (пар: {Count})", stopwatch.ElapsedMilliseconds, count);
            // Финальный статус со счётчиком по текущему виду
            // Финальный статус со счётчиком по текущему виду (пары или группы).
            if (count == 0)
                SetStatus(string.Empty);
            else
                SetResultCountStatus();
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
                    RaisePropertyChangedEvent();
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

                        BindableMenuItem[] menu = new BindableMenuItem[12];
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

                        // Пункт «Ignore pair» (текущая пара) — восстановлен (аудит зеркала WF: потерянные пункты меню).
                        IUCommand ignoreUCommand = new IgnorePairCommand(_resultList, Result, _ignoreStorage.IgnoreList);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);
                        menu[6] = new BindableMenuItem();
                        menu[6].Name = Application.Current.Resources["mainwindow_contextmenu_IgnorePair"] as string;
                        menu[6].Command = ignoreCommand;

                        ICommand openComparatorCommand = new RelayCommand(arg =>
                        {
                            ComparatorViewModel vm = new ComparatorViewModel(result, _resultList, Result,
                                _undoRedoEngine, _windowService, Configuration);
                            _windowService.OpenComparatorWindow(vm);
                        }, arg => _windowService != null);
                        menu[7] = new BindableMenuItem();
                        menu[7].Name = Application.Current.Resources["mainwindow_contextmenu_OpenСomparator"] as string;
                        menu[7].Command = openComparatorCommand;


                        menu[8] = new BindableMenuItem();
                        menu[8].Name = Application.Current.Resources["mainwindow_contextmenu_ShowDiffInPercent"] as string;
                        menu[8].Command = new RelayCommand(arg =>
                            {
                                if (result != null && result is DuplPairViewModel)
                                {
                                    StringBuilder sb = new StringBuilder();
                                    sb.AppendLine(String.Format("{0} and {1}", result.FirstFile.Path, result.SecondFile.Path));

                                    DiffrenceHelper.PrintDiffrence(sb, result);

                                    _windowService.ShowMessage(sb.ToString());
                                }
                            }, arg => result != null);

                        // Копирование в буфер: текущая пара.
                        menu[9] = new BindableMenuItem();
                        menu[9].Name = Application.Current.Resources["mainwindow_menu_edit_copy"] as string;
                        menu[9].Command = new RelayCommand(arg =>
                            {
                                _clipboardService.CopyText(ClipboardContentBuilder.Build(GetPairsForClipboard(result)));
                            }, arg => result != null);

                        menu[10] = new BindableMenuItem();
                        menu[10].Name = Application.Current.Resources["mainwindow_menu_edit_copy_paths"] as string;
                        menu[10].Command = new RelayCommand(arg =>
                            {
                                _clipboardService.CopyText(ClipboardContentBuilder.BuildPaths(GetPairsForClipboard(result)));
                            }, arg => result != null);

                        // Пункт «Delete defect» (дефекты) — только для строк-дефектов.
                        if (result.IsDefect)
                        {
                            IUCommand deleteDefectUCommand = new UndoRedo.DeleteDefectCommand(result, result.FirstFile, _resultList);
                            menu[11] = new BindableMenuItem();
                            menu[11].Name = Application.Current.Resources["mainwindow_contextmenu_DeleteDefect"] as string;
                            menu[11].Command = new RelayCommand(arg =>
                                {
                                    _undoRedoEngine.ExecuteCommand(deleteDefectUCommand);
                                }, arg => deleteDefectUCommand != null);
                        }

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
                        // Пункт «Ignore group» — восстановлен (аудит зеркала WF: потерянные пункты меню).
                        IUCommand ignoreUCommand = new IgnoreGroupCommand(_groups, GroupsCollection, _resultList, _ignoreStorage.IgnoreList, group);
                        ICommand ignoreCommand = new RelayCommand(arg =>
                        {
                            _undoRedoEngine.ExecuteCommand(ignoreUCommand);
                        }, arg => ignoreUCommand != null);

                        BindableMenuItem[] menu = new BindableMenuItem[2];
                        menu[0] = new BindableMenuItem();
                        menu[0].Name = Application.Current.Resources["mainwindow_contextmenu_IgnoreGroup"] as string;
                        menu[0].Command = ignoreCommand;

                        ICommand openComparatorCommand = new RelayCommand(arg =>
                        {
                            ComparatorViewModel vm = new ComparatorViewModel(group, _groups, GroupsCollection,
                                _undoRedoEngine, _windowService, Configuration);
                            _windowService.OpenComparatorWindow(vm);
                        }, arg => _windowService != null);
                        menu[1] = new BindableMenuItem();
                        menu[1].Name = Application.Current.Resources["mainwindow_contextmenu_OpenСomparator"] as string;
                        menu[1].Command = openComparatorCommand;
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
        const int THRESHOLD_DIFFERENCE_MAX_DINOV2 = 100;
        const int THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM = 5;
        const int THRESHOLD_DIFFERENCE_DEFAULT_SSIM = 20;
        const int THRESHOLD_DIFFERENCE_DEFAULT_DINOV2 = 45;

        ObservableCollection<AlgorithmComparingItem> _algorithmItems = new ObservableCollection<AlgorithmComparingItem>();
        /// <summary>
        /// Режимы (алгоритмы) сравнения изображений
        /// для выпадающего списка на панели инструментов.
        /// </summary>
        public ObservableCollection<AlgorithmComparingItem> AlgorithmItems
        {
            get { return _algorithmItems; }
        }

        ObservableCollection<double> _thresholdItems = new ObservableCollection<double>();
        /// <summary>
        /// Допустимые значения пороговой разности для текущего режима сравнения.
        /// </summary>
        public ObservableCollection<double> ThresholdItems
        {
            get { return _thresholdItems; }
        }

        /// <summary>
        /// Текущий режим (алгоритм) сравнения изображений — источник истины UI.
        /// Для Dinov2 ядро не участвует в сравнении (в ядре такого алгоритма нет).
        /// </summary>
        private AlgorithmComparing _uiAlgorithm = AlgorithmComparing.SquaredSum;

        public AlgorithmComparing AlgorithmComparing
        {
            get { return _uiAlgorithm; }
            set
            {
                if (value == _uiAlgorithm)
                    return;

                _uiAlgorithm = value;
                RebuildThresholdItems(GetMaxThresholdDifference(value));
                ApplyCompareOptions(value, GetDefaultThresholdDifference(value));

                // Запоминаем режим между перезапусками (configuration.xml).
                if (Configuration != null)
                {
                    Configuration.CompareAlgorithm = value;
                    Configuration.Save();
                }

                RaisePropertyChangedEvent();
                RaisePropertyChangedEvent("ThresholdDifference");
            }
        }

        /// <summary>
        /// Пороговая разность (в процентах) для текущего режима сравнения.
        /// </summary>
        public double ThresholdDifference
        {
            get { return Options != null ? Options.compareOptions.ThresholdDifference : 0; }
            set
            {
                value = CoreCompareOptions.NearestThresholdStep(value, GetMaxThresholdDifference(AlgorithmComparing));
                if (value == ThresholdDifference)
                    return;

                ApplyCompareOptions(AlgorithmComparing, value);
                RaisePropertyChangedEvent();
            }
        }

        static int GetMaxThresholdDifference(AlgorithmComparing algorithm)
        {
            if (algorithm == AlgorithmComparing.SSIM)
                return THRESHOLD_DIFFERENCE_MAX_SSIM;
            if (algorithm == AlgorithmComparing.Dinov2)
                return THRESHOLD_DIFFERENCE_MAX_DINOV2;
            return THRESHOLD_DIFFERENCE_MAX_SQUARED_SUM;
        }

        static int GetDefaultThresholdDifference(AlgorithmComparing algorithm)
        {
            if (algorithm == AlgorithmComparing.SSIM)
                return THRESHOLD_DIFFERENCE_DEFAULT_SSIM;
            if (algorithm == AlgorithmComparing.Dinov2)
                return THRESHOLD_DIFFERENCE_DEFAULT_DINOV2;
            return THRESHOLD_DIFFERENCE_DEFAULT_SQUARED_SUM;
        }

        static string GetCompareAlgorithmName(AlgorithmComparing algorithm)
        {
            string key;
            if (algorithm == AlgorithmComparing.SSIM)
                key = "toolBar_compareAlgorithm_ssim";
            else if (algorithm == AlgorithmComparing.Dinov2)
                key = "toolBar_compareAlgorithm_dinov2";
            else
                key = "toolBar_compareAlgorithm_squaredSum";
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

            // Deep (DINOv2) режим показываем только если ядро поддерживает scan-only поиск
            // (экспорт adScanCandidatesW, AntiDupl.dll 2.4.0+). Старая dll → режим скрыт.
            if (_core.DeepSearchSupported)
                _algorithmItems.Add(new AlgorithmComparingItem(AlgorithmComparing.Dinov2, GetCompareAlgorithmName(AlgorithmComparing.Dinov2)));

            // Снимок Options, созданный конструктором CoreOptions(_core), несёт дефолтные
            // значения (побочный эффект SetDefaultOptions в SetDefault), поэтому
            // берём реальные текущие значения опций из ядра:
            // (ядро не знает Dinov2 — из него всегда приходит SquaredSum/SSIM)
            CoreCompareOptions coreCompare = _core.compareOptions;
            bool dinoSupported = _core.DeepSearchSupported;

            // Режим поиска: восстанавливаем выбранный пользователем (configuration.xml);
            // если не выбран — классический режим (SquaredSum) по умолчанию.
            // Dino на старом ядре (нет adScanCandidatesW) — откат на SquaredSum.
            var savedMode = Configuration.CompareAlgorithm;
            AlgorithmComparing selected;
            if (savedMode == AlgorithmComparing.Dinov2 && !dinoSupported)
                selected = AlgorithmComparing.SquaredSum;
            else if (savedMode.HasValue)
                selected = savedMode.Value;
            else
                selected = AlgorithmComparing.SquaredSum;
            _uiAlgorithm = selected;

            RebuildThresholdItems(GetMaxThresholdDifference(selected));

            double threshold = CoreCompareOptions.NearestThresholdStep(selected == AlgorithmComparing.Dinov2
                ? GetDefaultThresholdDifference(AlgorithmComparing.Dinov2)
                : coreCompare.thresholdDifference, GetMaxThresholdDifference(selected));
            if (selected != Options.compareOptions.AlgorithmComparing ||
                threshold != Options.compareOptions.ThresholdDifference)
                ApplyCompareOptions(selected, threshold);
        }

        void RebuildThresholdItems(int max)
        {
            _thresholdItems.Clear();
            foreach (double step in CoreCompareOptions.ThresholdSteps(max))
                _thresholdItems.Add(step);
        }

        /// <summary>
        /// Сохраняет режим сравнения и пороговую разность: в текущий снимок Options
        /// и в опции ядра (dll), которые используются при следующем поиске.
        /// </summary>
        void ApplyCompareOptions(AlgorithmComparing algorithm, double threshold)
        {
            if (Options != null)
            {
                Options.compareOptions.algorithmComparing = algorithm;
                Options.compareOptions.thresholdDifference = threshold;
            }
            // ВАЖНО: C++ adAlgorithmComparing не имеет Dinov2 (2 = SIZE там!).
            // Deep-режиму ядро не получает алгоритм/порог — их использует только
            // C#-пайплайн (DeepSearchService) по снимку Options.
            if (algorithm == AlgorithmComparing.Dinov2)
                return;
            if (_core != null)
            {
                CoreCompareOptions coreOptions = _core.compareOptions;
                coreOptions.algorithmComparing = algorithm;
                coreOptions.thresholdDifference = threshold;
                _core.compareOptions = coreOptions;
            }
        }

        /// <summary>
        /// Заменяет снимок опций после поиска. В deep (Dinov2) режиме снимок ядра
        /// несёт реальные значения ядра (deep-значения ядру не передавались),
        /// поэтому копируем deep-значение алгоритма/порога из старого снимка.
        /// </summary>
        public void SetOptions(CoreOptions newOptions)
        {
            if (newOptions == null)
                return;
            if (_uiAlgorithm == AlgorithmComparing.Dinov2 && Options != null)
            {
                newOptions.compareOptions.algorithmComparing = AlgorithmComparing.Dinov2;
                newOptions.compareOptions.thresholdDifference = Options.compareOptions.ThresholdDifference;
            }
            Options = newOptions;
        }

        void OnLanguageChanged(object sender, EventArgs e)
        {
            foreach (AlgorithmComparingItem item in _algorithmItems)
                item.Name = GetCompareAlgorithmName(item.Value);
        }

        /// <summary>
        /// После поиска объект Options заменяется на новый снимок опций ядра,
        /// поэтому обновляем выпадающие списки режимов работы.
        /// Алгоритм берём из _uiAlgorithm (источник истины UI), порог — из снимка.
        /// </summary>
        void RefreshCompareModeOptions()
        {
            if (Options == null)
                return;

            AlgorithmComparing algorithm = _uiAlgorithm;
            int max = GetMaxThresholdDifference(algorithm);
            if (_thresholdItems.Count != CoreCompareOptions.ThresholdSteps(max).Length)
                RebuildThresholdItems(max);

            double threshold = CoreCompareOptions.NearestThresholdStep(Options.compareOptions.ThresholdDifference, max);
            if (threshold != Options.compareOptions.ThresholdDifference)
                ApplyCompareOptions(algorithm, threshold);

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
                    ConfigurationViewModel vm = new ConfigurationViewModel(Configuration, _windowService, _core, Options, _thumbnailProvider, DeepLogger);
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
        /// <summary>
        /// Отменить одно последнее действие (пункт меню «Undo» / Ctrl+Z).
        /// </summary>
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


        ICommand _undoAllCommand;
        /// <summary>
        /// Отменить все действия до состояния сразу после получения результата (пункт «Undo all»).
        /// </summary>
        public ICommand UndoAllCommand
        {
            get
            {
                return _undoAllCommand ?? (_undoAllCommand = new RelayCommand(arg =>
                {
                    while (_undoRedoEngine.UndoEnable)
                    {
                        _undoRedoEngine.Undo();
                    }
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
                    _viewModeModel.ViewMode = ViewMode.Group;
                    _windowService.ClosePleaseWait();
                    SetResultCountStatus(); // F17
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
                    _viewModeModel.ViewMode = ViewMode.Pair;
                    _windowService.ClosePleaseWait();
                    SetResultCountStatus(); // F17
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

                   // лог прогресс-окна (зеркало WF checkResultsAtLoading/checkMistakesAtLoading):
                   // при загрузке — проверка существования файлов и фильтрация
                   // пар из ignore-списка (mistakes).
                   bool checkResults = Configuration.CheckResultsAtLoading;
                   bool checkMistakes = Configuration.CheckMistakesAtLoading;
                   HashSet<DuplPairViewModel> ignored = checkMistakes
                       && _ignoreStorage != null && _ignoreStorage.IgnoreList != null
                           ? new HashSet<DuplPairViewModel>(_ignoreStorage.IgnoreList)
                           : null;
                   List<DuplPairViewModel> kept = new List<DuplPairViewModel>(readed.Length);

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

                       bool keep = true;
                       if (checkResults && !item.CheckAllExist())
                           keep = false;
                       if (ignored != null && ignored.Contains(item))
                           keep = false;
                       if (keep)
                           kept.Add(item);
                      // if (item.CheckAllExist())
                       //    filtered.Add(item);

                       progressDialogViewModel.Progress++;
                       processed++;
                       if (processed % statusStep == 0)
                           SetStatus(LocalizationHelper.S("status_loadingResults", processed, readed.Length));
                       if (progressDialogViewModel.IsCancelled)
                           break;
                   }

                    SetStatus(LocalizationHelper.S("status_processed", kept.Count, readed.Length));
                   }
                   catch (Exception ex)
                   {
                       _logger.LogError(ex, "LoadResults: ошибка обработки файла {File}", resPath);
                       throw;
                   }
                   try
                   {
                       SetResult(kept.ToArray());
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
                    this.RaisePropertyChangedEvent();
                    this.RaisePropertyChangedEvent(nameof(IsPairMode));
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
                RaisePropertyChangedEvent();
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
                    if (Result != null)
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
