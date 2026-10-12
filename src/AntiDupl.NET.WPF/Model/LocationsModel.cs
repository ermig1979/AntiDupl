using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using System.Xml.Serialization;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.Service;
using AntiDupl.NET.WPF.ViewModel;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using PathType = AntiDupl.NET.Core.Original.CoreDll.PathType;

namespace AntiDupl.NET.WPF.Model
{
    /// <summary>
    /// Списки путей четырёх видов (зеркало WF CorePathsForm):
    /// search / ignore / valid / delete. Каждый вид хранится в своём XML-файле
    /// (locations.xml — backward-compat для search, locations_ignore.xml,
    /// locations_valid.xml, locations_delete.xml) рядом с приложением.
    /// </summary>
    [Serializable()]
    [XmlInclude(typeof(Location))]
    public class LocationsModel : PropertyChangedBase
    {
        [NonSerialized]
        private const string _searchFileName = @"locations.xml";
        [NonSerialized]
        private const string _ignoreFileName = @"locations_ignore.xml";
        [NonSerialized]
        private const string _validFileName = @"locations_valid.xml";
        [NonSerialized]
        private const string _deleteFileName = @"locations_delete.xml";

        ObservableCollection<SearchPathViewModel> _searchLocationsObservable;
        public ObservableCollection<SearchPathViewModel> SearchLocations
        {
            get { return _searchLocationsObservable; }
        }

        ObservableCollection<SearchPathViewModel> _ignoreLocations;
        public ObservableCollection<SearchPathViewModel> IgnoreLocations
        {
            get { return _ignoreLocations; }
        }

        ObservableCollection<SearchPathViewModel> _validLocations;
        public ObservableCollection<SearchPathViewModel> ValidLocations
        {
            get { return _validLocations; }
        }

        ObservableCollection<SearchPathViewModel> _deleteLocations;
        public ObservableCollection<SearchPathViewModel> DeleteLocations
        {
            get { return _deleteLocations; }
        }

        /// <summary>Текущее выделение DataGrid окна путей (общее для всех вкладок).</summary>
        public object Current { get; set; }

        [NonSerialized]
        ICoreLocationSync _sync;

        string _ignoreFilenameFilter = "";
        /// <summary>
        /// Регулярное выражение — фильтр имён файлов для ignore-списка (зеркало WF
        /// options.xml → ignoreFilenameFilter). Сохраняется в configuration.xml.
        /// </summary>
        public string IgnoreFilenameFilter
        {
            get { return _ignoreFilenameFilter; }
            set
            {
                if (_ignoreFilenameFilter != value)
                {
                    _ignoreFilenameFilter = value ?? "";
                    RaisePropertyChangedEvent();
                    // Изменение фильтра меняет поведение ядра — синхронизируем сразу.
                    CopyToDll();
                }
            }
        }

        public LocationsModel(ICoreLocationSync sync,
            List<Location> search, List<Location> ignore, List<Location> valid, List<Location> delete)
        {
            _sync = sync;

            _searchLocationsObservable = new ObservableCollection<SearchPathViewModel>();
            foreach (var path in search)
                _searchLocationsObservable.Add(new SearchPathViewModel(path));

            _ignoreLocations = new ObservableCollection<SearchPathViewModel>();
            foreach (var path in ignore)
                _ignoreLocations.Add(new SearchPathViewModel(path));

            _validLocations = new ObservableCollection<SearchPathViewModel>();
            foreach (var path in valid)
                _validLocations.Add(new SearchPathViewModel(path));

            _deleteLocations = new ObservableCollection<SearchPathViewModel>();
            foreach (var path in delete)
                _deleteLocations.Add(new SearchPathViewModel(path));
        }

        ~LocationsModel()
        {
            foreach (var vm in AllCollections())
                vm.PropertyChanged -= OnSearchPathPropertyChanged;
        }

        IEnumerable<SearchPathViewModel> AllCollections()
        {
            if (SearchLocations != null)
                foreach (var vm in SearchLocations)
                    yield return vm;
            if (IgnoreLocations != null)
                foreach (var vm in IgnoreLocations)
                    yield return vm;
            if (ValidLocations != null)
                foreach (var vm in ValidLocations)
                    yield return vm;
            if (DeleteLocations != null)
                foreach (var vm in DeleteLocations)
                    yield return vm;
        }

        public static LocationsModel Load(ICoreLocationSync sync)
        {
            // search — в legacy-файле locations.xml (backward-compat), остальные — в отдельных.
            List<Location> search = LoadList(_searchFileName);
            List<Location> ignore = LoadList(_ignoreFileName);
            List<Location> valid = LoadList(_validFileName);
            List<Location> delete = LoadList(_deleteFileName);

            return new LocationsModel(sync, search, ignore, valid, delete);
        }

        static List<Location> LoadList(string fileName)
        {
            List<Location> list = SerializeHelper<List<Location>>.Load(GetFilePath(fileName));
            return list ?? new List<Location>();
        }

        public void Save()
        {
            SaveList(SearchLocations, _searchFileName);
            SaveList(IgnoreLocations, _ignoreFileName);
            SaveList(ValidLocations, _validFileName);
            SaveList(DeleteLocations, _deleteFileName);
        }

        static void SaveList(ObservableCollection<SearchPathViewModel> collection, string fileName)
        {
            var locations = collection.Select(l => l.Model).ToList();
            SerializeHelper<List<Location>>.Save(locations, GetFilePath(fileName));
        }

        /// <summary>Полный путь: пользовательские данные — в <c>user\</c> (зеркало WF UserPath).</summary>
        static string GetFilePath(string fileName) => System.IO.Path.Combine(AntiDupl.NET.WPF.Resources.Resources.UserPath, fileName);

        /// <summary>Коллекция выбранного вида путей.</summary>
        public ObservableCollection<SearchPathViewModel> GetCollection(PathType type)
        {
            switch (type)
            {
                case PathType.Ignore:
                    return IgnoreLocations;
                case PathType.Valid:
                    return ValidLocations;
                case PathType.Delete:
                    return DeleteLocations;
                case PathType.Search:
                default:
                    return SearchLocations;
            }
        }

        /// <summary>Добавляет путь в коллекцию указанного вида (без дублей) и синхронизирует ядро.</summary>
        public void AddPath(string path, PathType type)
        {
            var collection = GetCollection(type);
            if (collection == null)
                return;
            if (!collection.Any(sl => sl.Path.Equals(path)))
            {
                SearchPathViewModel spvm = new SearchPathViewModel(new Location(path));
                spvm.PropertyChanged += OnSearchPathPropertyChanged;
                collection.Add(spvm);
                CopyToDll();
            }
        }

        void OnSearchPathPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            CopyToDll();
        }

        /// <summary>
        /// Проталкивает все четыре списка путей и фильтр имён в ядро (через ICoreLocationSync).
        /// </summary>
        public void CopyToDll()
        {
            if (_sync == null)
                return;

            // UseSinglePath (зеркало WF onePath): при включённом режиме в ядро
            // передаётся только первый путь поиска.
            var configuration = ServiceLocator.Resolve<IConfigurationModel>();
            var searchCollection = SearchLocations;
            if (configuration != null && configuration.UseSinglePath
                && searchCollection != null && searchCollection.Count > 1)
            {
                searchCollection = new ObservableCollection<SearchPathViewModel> { searchCollection[0] };
            }

            _sync.SetLocations(
                ToCoreArray(searchCollection),
                ToCoreArray(IgnoreLocations),
                ToCoreArray(ValidLocations),
                ToCoreArray(DeleteLocations),
                IgnoreFilenameFilter);
        }

        static CorePathWithSubFolder[] ToCoreArray(ObservableCollection<SearchPathViewModel> collection)
        {
            List<CorePathWithSubFolder> result = new List<CorePathWithSubFolder>();
            if (collection != null)
            {
                foreach (var location in collection.Select(l => l.Model))
                {
                    CorePathWithSubFolder sfPath = new CorePathWithSubFolder();
                    sfPath.path = location.Path;
                    sfPath.enableSubFolder = location.SearchInSubfolder;
                    result.Add(sfPath);
                }
            }
            return result.ToArray();
        }

        ICommand _deleteLocationCommand;
        public ICommand DeleteLocationCommand
        {
            get
            {
                return _deleteLocationCommand ?? (_deleteLocationCommand = new RelayCommand(arg =>
                {
                    var current = Current as SearchPathViewModel;
                    if (current == null)
                        return;
                    foreach (var collection in new[] { SearchLocations, IgnoreLocations, ValidLocations, DeleteLocations })
                    {
                        if (collection != null && collection.Remove(current))
                        {
                            current.PropertyChanged -= OnSearchPathPropertyChanged;
                            CopyToDll();
                            break;
                        }
                    }
                }, arg => Current is SearchPathViewModel));
            }
        }
    }
}