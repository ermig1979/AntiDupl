﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml.Serialization;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Core;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ViewModel;

using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using static System.Formats.Asn1.AsnWriter;

namespace AntiDupl.NET.WPF.Model
{
    [Serializable()]
    [XmlInclude(typeof(Location))]
    public class LocationsModel
    {
        [NonSerialized]
        private const string _fileName = @"locations.xml";

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

        public object Current { get; set; }

        CoreLib _core;

        /*public LocationsModel()
        {
            _searchPathList = new List<SearchPath>();
            _locationsObservable = new ObservableCollection<SearchPathViewModel>();
        }*/

        public LocationsModel(CoreLib core, List<Location> list)
        {
            _core = core;

            _searchLocationsObservable = new ObservableCollection<SearchPathViewModel>();
            foreach (var path in list)
            {
                SearchPathViewModel spvm = new SearchPathViewModel(path);
                _searchLocationsObservable.Add(spvm);
            }
        }

        ~LocationsModel()
        {
            //foreach (var location in Locations.LocationList)
            //{
            //   location.PropertyChanged -= OnSearchPathPropertyChanged;
            // }
            foreach (var vm in SearchLocations)
            {
                vm.PropertyChanged -= OnSearchPathPropertyChanged;
            }
        }

        public static LocationsModel Load(CoreLib core)
        {
            List<Location> list = SerializeHelper<List<Location>>.Load(_fileName);
            if (list == null)
                list = new List<Location>();

            LocationsModel model = new LocationsModel(core, list);

            return model;
        }

        public void Save()
        {
            var locations = SearchLocations.Select(l => l.Model).ToList();
            SerializeHelper<List<Location>>.Save(locations, _fileName);
        }

        public void AddSearchPath(string path)
        {
            if (!SearchLocations.Any(sl => sl.Path.Equals(path)))
            {
                Location sp = new Location(path);
                //_searchPathList.Add(sp);
                SearchPathViewModel spvm = new SearchPathViewModel(sp);
                spvm.PropertyChanged += OnSearchPathPropertyChanged;
                SearchLocations.Add(spvm);
            }
        }

        void OnSearchPathPropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            CopyToDll();
        }

        public void CopyToDll()
        {
            var locations = SearchLocations.Select(l => l.Model);
            List<CorePathWithSubFolder> searchPathes = new List<CorePathWithSubFolder>();
            foreach (Location location in locations)
            {
                CorePathWithSubFolder sfPath = new CorePathWithSubFolder();
                sfPath.path = location.Path;
                sfPath.enableSubFolder = location.SearchInSubfolder;
                searchPathes.Add(sfPath);
            }
            _core.searchPath = (CorePathWithSubFolder[])searchPathes.ToArray();
            /*locations = IgnoreLocations.Select(l => l.Model);
            _core.SetLocation(locations, AntiDupl.NET.WPF.Core.CoreDll.PathType.Ignore);
            locations = ValidLocations.Select(l => l.Model);
            _core.SetLocation(locations, AntiDupl.NET.WPF.Core.CoreDll.PathType.Valid);*/
        }

        ICommand _deleteLocationCommand;
        public ICommand DeleteLocationCommand
        {
            get
            {
                return _deleteLocationCommand ?? (_deleteLocationCommand = new RelayCommand(arg =>
                {
                    var current = Current as SearchPathViewModel;
                    if (current != null)
                        SearchLocations.Remove(current);
                }, arg => SearchLocations != null && SearchLocations.Count > 0 && Current != null && Current is SearchPathViewModel));
            }
        }
    }
}
