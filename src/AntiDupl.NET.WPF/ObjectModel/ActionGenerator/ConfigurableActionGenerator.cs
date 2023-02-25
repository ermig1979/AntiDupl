using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel.ByImageProperty;
using AntiDupl.NET.WPF.ObjectModel.ByPath;
using AntiDupl.NET.WPF.ObjectModel.ConfigurableAction;
using AntiDupl.NET.WPF.ObjectModel.DirectoryFilter;
using AntiDupl.NET.WPF.ObjectModel.GettingNumber;
using AntiDupl.NET.WPF.UndoRedo;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.ObjectModel.ActionGenerator
{
    public class ConfigurableActionGenerator : PropertyChangedBase
    {
        IUndoRedoEngine _undoRedoEngine;

        public ConfigurableActionGenerator(IUndoRedoEngine undoRedoEngine)
        {
            _undoRedoEngine = undoRedoEngine;
            FilterConditions = new ObservableCollection<FilterCondition>();
        }

        public string Name { get; set; }

        public override string ToString()
        {
            return Name;
        }

        //internal void Excute()
        //{
        //    throw new NotImplementedException();
        //}


        SelectingBestImageByProperty _selectingBestImageByProperty;
        public SelectingBestImageByProperty SelectingBestImageByProperty
        {
            get { return _selectingBestImageByProperty; }
            set
            {
                _selectingBestImageByProperty = value;
                RaisePropertyChangedEvent("SelectingBestImageByProperty");
            }
        }

        private ObservableCollection<FilterCondition> _selectConditions;
        public ObservableCollection<FilterCondition> FilterConditions
        {
            get { return _selectConditions; }
            set
            {
                _selectConditions = value;
                RaisePropertyChangedEvent("SelectConditions");
            }
        }

        IGettingNumber _algorithmOfGettingNumberFromFileName;
        public IGettingNumber AlgorithmOfGettingNumberFromFileName
        {
            get { return _algorithmOfGettingNumberFromFileName; }
            set
            {
                _algorithmOfGettingNumberFromFileName = value;
                RaisePropertyChangedEvent("AlgorithmOfGettingNumberFromFileName");
            }
        }


        IDirectoryFilter _directoryFilter;
        public IDirectoryFilter DirectoryFilter
        {
            get { return _directoryFilter; }
            set
            {
                _directoryFilter = value;
                RaisePropertyChangedEvent("DirectoryFilter");
            }
        }

        //IBestSelectingByImagePropertyes _bestSelectingByImagePropertyes;
        //public IBestSelectingByImagePropertyes BestSelectingByImagePropertyes
        //{
        //    get { return _bestSelectingByImagePropertyes; }
        //    set
        //    {
        //        _bestSelectingByImagePropertyes = value;
        //        RaisePropertyChangedEvent("BestSelectingByImagePropertyes");
        //    }
        //}

        IBestSelectingByPath _bestByPath;
        public IBestSelectingByPath BestByPath
        {
            get { return _bestByPath; }
            set
            {
                _bestByPath = value;
                RaisePropertyChangedEvent("BestByPath");
            }
        }

        //ActionOnDuplPair _actionOnDuplPair;
        //public ActionOnDuplPair ActionOnDuplPair
        //{
        //    get { return _actionOnDuplPair; }
        //    set
        //    {
        //        _actionOnDuplPair = value;
        //        RaisePropertyChangedEvent("ActionOnDuplPair");
        //    }
        //}

        //ActionOnDuplPair _elseActionOnDuplPair;
        //public ActionOnDuplPair ElseActionOnDuplPair
        //{
        //    get { return _elseActionOnDuplPair; }
        //    set
        //    {
        //        _elseActionOnDuplPair = value;
        //        RaisePropertyChangedEvent("ElseActionOnDuplPair");
        //    }
        //}


        //EqualAction _equalAction;
        //public EqualAction EqualAction
        //{
        //    get { return _equalAction; }
        //    set
        //    {
        //        _equalAction = value;
        //        RaisePropertyChangedEvent("EqualAction");
        //    }
        //}

        /*internal IConfigurableAction GetAction(DuplPairViewModel item)
        {
            ImageInfoClass best = null;
            ImageInfoClass bad = null;
            bool skipImageProperty = GetBestByImageProperty(item, BestSelectingByImagePropertyes, ref best, ref bad);
            ImageInfoClass best2 = null;
            ImageInfoClass bad2 = null;
            IConfigurableAction skipAction = GetBestByPath(item, BestByPath, ref best2, ref bad2);
            if (skipAction != null)
                return skipAction;

            if (skipImageProperty)
            {
                best = best2;
                bad = bad2;
            }


            if (best == best2)
            {
                switch (ActionOnDuplPair)
                {
                    case ActionOnDuplPair.DeleteBadImage:
                        return new DeleteBadAction(bad, item);
                        break;
                    case ActionOnDuplPair.ReplaceBadImage:
                        return new ReplaceBadImageAction(best, bad);
                        break;
                    default:
                        throw new ArgumentException("Неизвестный параметр " + ActionOnDuplPair);
                        break;
                }
            }
            else
            {
                switch (ElseActionOnDuplPair)
                {
                    case ActionOnDuplPair.DeleteBadImage:
                        return new DeleteBadAction(bad, item);
                        break;
                    case ActionOnDuplPair.ReplaceBadImage:
                        return new ReplaceBadImageAction(best, bad);
                        break;
                    default:
                        throw new ArgumentException("Неизвестный параметр " + ElseActionOnDuplPair);
                        break;
                }
            }
            //}
            return null;
        }

        private IConfigurableAction GetBestByPath(DuplPairViewModel item, 
            ObjectModel.BestByPath bestByPath, ref ImageInfoClass best, ref ImageInfoClass bad)
        {
            switch (bestByPath)
            {
                case BestByPath.BestImageShouldHaveASmallerNumber:
                    var firstNumber = AlgorithmOfGettingNumberFromFileName.GetNumber(item.FirstFile.FileName);
                    var secondNumber = AlgorithmOfGettingNumberFromFileName.GetNumber(item.SecondFile.FileName);
                    if (!firstNumber.HasValue || !secondNumber.HasValue)
                        return new SkipAction(String.Format("Не удалось получить номер из {0} или {1}",
                            item.FirstFile.FileName, item.SecondFile.FileName));
                        //throw new Exception(String.Format("Не удалось получить номер из {0} или {1}",
                        //    item.FirstFile.FileName, item.SecondFile.FileName));
                    if (firstNumber.Value < secondNumber.Value)
                    {
                        best = item.FirstFile;
                        bad = item.SecondFile;
                    }
                    else if (secondNumber.Value < firstNumber.Value)
                    {
                        best = item.SecondFile;
                        bad = item.FirstFile;
                    }
                    else
                        return new SkipAction("Номера файлов равны!");
                    //throw new Exception("Номера файлов равны!");
                    break;
                //case BestByPath.BestImageShouldBeInDirectory1:
                //    break;
                //case BestByPath.BestImageShouldBeInDirectory2:
                //    break;
                default:
                    throw new ArgumentException("Неизвестный параметр " + bestByPath);
                    break;
            }
            return null;
        }

        private bool GetBestByImageProperty(DuplPairViewModel item, BestSelectingByImagePropertyes bestSelecting,
            ref ImageInfoClass best, ref ImageInfoClass bad)
        {
            switch (bestSelecting)
            {
                case BestSelectingByImagePropertyes.BigFileSize:
                    if (item.FirstFile.FileSize == item.SecondFile.FileSize)
                    {
                        switch (EqualAction)
                        {
                            case EqualAction.SkipImagePropertyCompare:
                                return true;
                                break;
                            //case EqualAction.BestHaveOldestLastAccessTime:
                            //    break;
                            default:
                                throw new ArgumentException("Неизвестный параметр " + EqualAction);
                                break;
                        }
                    }
                    else if (item.FirstFile.FileSize < item.SecondFile.FileSize)
                    {
                        best = item.SecondFile;
                        bad = item.FirstFile;
                        return false;
                    }
                    else if (item.FirstFile.FileSize > item.SecondFile.FileSize)
                    {
                        best = item.FirstFile;
                        bad = item.SecondFile;
                        return false;
                    }
                    break;
                default:
                    throw new ArgumentException("Неизвестный параметр " + bestSelecting);
                    break;
            }
            throw new Exception("Неожиданный выход из switch");
        }*/

        internal IConfigurableAction GetAction(DuplicateGroup group)
        { 
            ImageInfoClass best = null;
            ImageInfoClass bad = null;

            IEnumerable<ImageInfoClass> bests = SelectingBestImageByProperty.GetBest(group.FileList);

            //IEnumerable<ImageInfoClass> bests = BestSelectingByImagePropertyes.GetBest(group);

            ImageInfoClass bestsByPath = BestByPath.GetBest(group, AlgorithmOfGettingNumberFromFileName);

            if (bests.Contains(bestsByPath))
            {
                return new DeleteOtherAction(bestsByPath, group, _undoRedoEngine);
            }
            else
            {
                return new ReplaceAndDeleteAction(bestsByPath, bests, group, _undoRedoEngine);
            }

            return new SkipAction("Test");
        }

    }
}
