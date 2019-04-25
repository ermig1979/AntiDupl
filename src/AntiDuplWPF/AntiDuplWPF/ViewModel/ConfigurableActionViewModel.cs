using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AntiDuplWPF.Command;
using AntiDuplWPF.Helper;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ObjectModel.ActionGenerator;
using AntiDuplWPF.ObjectModel.ByImageProperty;
using AntiDuplWPF.ObjectModel.ByPath;
using AntiDuplWPF.ObjectModel.ConditionMode;
using AntiDuplWPF.ObjectModel.ConfigurableAction;
using AntiDuplWPF.ObjectModel.DirectoryFilter;
using AntiDuplWPF.ObjectModel.GettingNumber;
using AntiDuplWPF.UndoRedo;
using AntiDuplWPF.View;

namespace AntiDuplWPF.ViewModel
{
    class ConfigurableActionViewModel : CloseableViewModel
    {
        private System.Collections.ObjectModel.ObservableCollection<DuplPairViewModel> _resultList;
        IUndoRedoEngine _undoRedoEngine;
        IEditableCollectionView _resultCollectionView;
        IWindowService _windowService;

        ObservableCollection<DuplicateGroup> _groups = new ObservableCollection<DuplicateGroup>();

        public System.Collections.ObjectModel.ObservableCollection<ConfigurableActionGenerator> Actions { get; private set; }

        ConfigurableActionGenerator _configurableAction;
        public ConfigurableActionGenerator ConfigurableAction
        {
            get { return _configurableAction; }
            set
            {
                _configurableAction = value;
                RaisePropertyChangedEvent("ConfigurableAction");
            }
        }

        public ConfigurableActionViewModel(ObservableCollection<DuplPairViewModel> resultList,
             IConfigurationModel configuration, IUndoRedoEngine undoRedoEngine, 
            IEditableCollectionView resultCollectionView,
            IWindowService windowService)
        {
            this._resultList = resultList;
            Configuration = configuration;
            _undoRedoEngine = undoRedoEngine;
            _resultCollectionView = resultCollectionView;
            _windowService = windowService;

            GroupHelper.ConvertToGroup(_groups, _resultList);

            AlgorithmsOfGettingNumberFromFileName = new ObservableCollection<IGettingNumber>();
            AlgorithmsOfGettingNumberFromFileName.Add(new NumberBeforeUnderlineGettingNumber());

            //BestSelectingByImagePropertyesList = new ObservableCollection<IBestSelectingByImagePropertyes>();
            //BestSelectingByImagePropertyesList.Add(new BigFileSize());
            //BestSelectingByImagePropertyesList.Add(new FileSizeResolutionBluringPeak());
            //BestSelectingByImagePropertyesList.Add(new FileSizeAndPeak());

            BestSelectingByPathList = new ObservableCollection<IBestSelectingByPath>();
            BestSelectingByPathList.Add(new BestImageShouldHaveASmallerNumber());


            AllIConditionMode = new ObservableCollection<IConditionMode>();
            AllIConditionMode.Add(new EqualConditionMode());
            AllIConditionMode.Add(new LessConditionMode());
            AllIConditionMode.Add(new MoreConditionMode());

            AllDirectoryFilters = new ObservableCollection<IDirectoryFilter>();
            AllDirectoryFilters.Add(new AnywhereDirectoryFilter());
            AllDirectoryFilters.Add(new SameDirectory());
            AllDirectoryFilters.Add(new OneImageInDir1OtherInDir2());

            ConfigurableActionGenerator allBestGenerator = new AllBestGenerator(AlgorithmsOfGettingNumberFromFileName,
                BestSelectingByPathList, AllDirectoryFilters, _undoRedoEngine);
            FileSizePeakBluringGenerator fileSizeAndPeakGenerator = new FileSizePeakBluringGenerator(AlgorithmsOfGettingNumberFromFileName,
                BestSelectingByPathList, AllDirectoryFilters, _undoRedoEngine);
            FileSizePeakGenerator fileSizePeakGenerator = new FileSizePeakGenerator(AlgorithmsOfGettingNumberFromFileName,
                BestSelectingByPathList, AllDirectoryFilters, _undoRedoEngine);

            Actions = new ObservableCollection<ConfigurableActionGenerator>();
            Actions.Add(allBestGenerator);
            Actions.Add(fileSizeAndPeakGenerator);
            Actions.Add(fileSizePeakGenerator);
            Actions.Add(new ConfigurableActionGenerator(_undoRedoEngine) { Name = "Empty" });
            ConfigurableAction = Actions.FirstOrDefault();
        }

        public IConfigurationModel Configuration { get; private set; }

        ObservableCollection<IConditionMode> _allIConditionMode;
        public ObservableCollection<IConditionMode> AllIConditionMode
        {
            get { return _allIConditionMode; }
            set
            {
                _allIConditionMode = value;
                RaisePropertyChangedEvent("AllIConditionMode");
            }
        }

        ObservableCollection<IGettingNumber> _algorithmsOfGettingNumberFromFileName;
        public ObservableCollection<IGettingNumber> AlgorithmsOfGettingNumberFromFileName
        {
            get { return _algorithmsOfGettingNumberFromFileName; }
            set
            {
                _algorithmsOfGettingNumberFromFileName = value;
                RaisePropertyChangedEvent("AlgorithmsOfGettingNumberFromFileName");
            }
        }

        //ObservableCollection<IBestSelectingByImagePropertyes> _bestSelectingByImagePropertyesList;
        //public ObservableCollection<IBestSelectingByImagePropertyes> BestSelectingByImagePropertyesList
        //{
        //    get { return _bestSelectingByImagePropertyesList; }
        //    set
        //    {
        //        _bestSelectingByImagePropertyesList = value;
        //        RaisePropertyChangedEvent("BestSelectingByImagePropertyesList");
        //    }
        //}

        ObservableCollection<IBestSelectingByPath> _bestSelectingByPathList;
        public ObservableCollection<IBestSelectingByPath> BestSelectingByPathList
        {
            get { return _bestSelectingByPathList; }
            set
            {
                _bestSelectingByPathList = value;
                RaisePropertyChangedEvent("BestSelectingByPathList");
            }
        }  



        //ActionCondition _actionCondition;
        //public ActionCondition ActionCondition
        //{
        //    get { return _actionCondition; }
        //    set
        //    {
        //        _actionCondition = value;
        //        RaisePropertyChangedEvent("ActionCondition");
        //    }
        //}

       


        ObservableCollection<IDirectoryFilter> _allDirectoryFilters;
        public ObservableCollection<IDirectoryFilter> AllDirectoryFilters
        {
            get { return _allDirectoryFilters; }
            set
            {
                _allDirectoryFilters = value;
                RaisePropertyChangedEvent("AllDirectoryFilters");
            }
        }


        ICommand _showPreviewCommand;
        public ICommand ShowPreviewCommand
        {
            get
            {
                return _showPreviewCommand ?? (_showPreviewCommand = new RelayCommand(arg =>
                {
                    IEnumerable<DuplicateGroup> filtered = _groups;

                    try
                    {


                    Debug.WriteLine(String.Format("resultList = {0}", _resultList.Count));
                    filtered = _groups.Where(group => ConfigurableAction.AlgorithmOfGettingNumberFromFileName.ContainNumber(group));
                    //filtered = filtered.Where(result => SelectConditions.All(cond => cond.IsSelect(result)));
                    Debug.WriteLine(String.Format("After AlgorithmOfGettingNumberFromFileName.ContainNumber = {0}", filtered.Count()));

                    filtered = _groups.Where(group => ConfigurableAction.BestByPath.ExistBest(group, ConfigurableAction.AlgorithmOfGettingNumberFromFileName));
                    Debug.WriteLine(String.Format("After BestByPath.ExistBest = {0}", filtered.Count()));

                    //filtered = filtered.Where(group => ConfigurableAction.BestSelectingByImagePropertyes.ExistBest(group));
                    //Debug.WriteLine(String.Format("After BestSelectingByImagePropertyes.ExistBest = {0}", filtered.Count()));

                    filtered = filtered.Where(group => ConfigurableAction.SelectingBestImageByProperty.ExistBest(group));
                    Debug.WriteLine(String.Format("After ConditionsOfSelectingBestImageByProperty.ExistBest = {0}", filtered.Count()));

                    filtered = filtered.Where(g => ConfigurableAction.DirectoryFilter.IsSelect(g));
                    Debug.WriteLine(String.Format("After DirectoryFilter = {0}", filtered.Count()));
                    foreach (var item in ConfigurableAction.FilterConditions)
                    {
                        filtered = filtered.Where(r => item.IsSelect(r));
                        Debug.WriteLine(String.Format("After {0} = {1}", item.Parametr, filtered.Count()));
                    }
                    foreach (var item in filtered)
                    {
                        item.ConfigurableAction = ConfigurableAction.GetAction(item);
                    }
                    }
                    catch (Exception ex)
                    {

                        throw;
                    }
                    FilteredResults = new ObservableCollection<DuplicateGroup>(filtered);
                }, arg => _resultList != null
                    && ConfigurableAction != null
                    && ConfigurableAction.AlgorithmOfGettingNumberFromFileName != null
                    //&& ConfigurableAction.BestSelectingByImagePropertyes != null
                    && ConfigurableAction.DirectoryFilter != null
                    && ConfigurableAction.FilterConditions != null));
            }
        }


        private ObservableCollection<DuplicateGroup> _filteredResults;
        public ObservableCollection<DuplicateGroup> FilteredResults
        {
            get { return _filteredResults; }
            set
            {
                _filteredResults = value;
                RaisePropertyChangedEvent("FilteredResults");
            }
        }

        //private ObservableCollection<DuplPairViewModel> _filteredResults;
        //public ObservableCollection<DuplPairViewModel> FilteredResults
        //{
        //    get { return _filteredResults; }
        //    set
        //    {
        //        _filteredResults = value;
        //        RaisePropertyChangedEvent("FilteredResults");
        //    }
        //}

        public DuplPairViewModel SelectedPair { get; set; }

        //ICommand _closeWindowAndSelectPairCommand;
        //public ICommand CloseWindowAndSelectPairCommand
        //{
        //    get
        //    {
        //        return _closeWindowAndSelectPairCommand ?? (_closeWindowAndSelectPairCommand = new RelayCommand(arg =>
        //        {
        //            DuplPairViewModel duplPairViewModel = arg as DuplPairViewModel;
        //            if (duplPairViewModel != null)
        //            {
        //                SelectedPair = duplPairViewModel;
        //                base.OnClosingRequest(true);
        //            }
        //        }, arg => _resultList != null));
        //    }
        //}


        ICommand _openComparatorCommand;
        public ICommand OpenComparatorCommand
        {
            get
            {
                return _openComparatorCommand ?? (_openComparatorCommand = new RelayCommand(arg =>
                {
                    //DuplPairViewModel duplPairViewModel = arg as DuplPairViewModel;
                    //if (duplPairViewModel != null)
                    //{
                    //    ComparatorViewModel vm = new ComparatorViewModel(duplPairViewModel, _resultList, null,
                    //        _undoRedoEngine, _windowService, Configuration);
                    //    _windowService.OpenComparatorWindow(vm);
                    //}
                    DuplicateGroup group = arg as DuplicateGroup;
                    if (group != null)
                    {
                        ComparatorViewModel vm = new ComparatorViewModel(group, _groups, null,
                            _undoRedoEngine, _windowService, Configuration);
                        _windowService.OpenComparatorWindow(vm);
                    }
                }, arg => _resultList != null));
            }
        }



        ICommand _showDiffrenceInPercent;
        public ICommand ShowDiffrenceInPercent
        {
            get
            {
                return _showDiffrenceInPercent ?? (_showDiffrenceInPercent = new RelayCommand(arg =>
                {
                    DuplicateGroup group = arg as DuplicateGroup;
                    if (group != null)
                    {
                        StringBuilder sb = new StringBuilder();
                        foreach (var result in group.ResultList)
                        {
                            sb.AppendLine(String.Format("{0} and {1}", result.FirstFile.Path, result.SecondFile.Path));

                            DiffrenceHelper.PrintDiffrence(sb, result);
                        }
                        _windowService.ShowMessage(sb.ToString());
                    }
                }, arg => _resultList != null));
            }
        }

        ICommand _goCommand;
        public ICommand GoCommand
        {
            get
            {
                return _goCommand ?? (_goCommand = new RelayCommand(arg =>
                {
                    foreach (var action in FilteredResults)
                    {
                        action.ConfigurableAction.Excute(_undoRedoEngine, _resultList, _resultCollectionView);
                    }
                    FilteredResults.Clear();
                    //GroupHelper.GroupToList(_groups, _resultList);
                    //_resultList.RemoveAt(0);
                    
                }, arg => _resultList != null
                    && ConfigurableAction != null
                    && ConfigurableAction.AlgorithmOfGettingNumberFromFileName != null
                    //&& ConfigurableAction.BestSelectingByImagePropertyes != null
                    && ConfigurableAction.DirectoryFilter != null
                    && ConfigurableAction.FilterConditions != null));
            }
        }


    }
}
