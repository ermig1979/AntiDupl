using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel.ByImageProperty;
using AntiDuplWPF.ObjectModel.ConditionMode;
using AntiDuplWPF.ObjectModel.GettingNumber;
using AntiDuplWPF.UndoRedo;

namespace AntiDuplWPF.ObjectModel.ActionGenerator
{
    class FileSizePeakBluringGenerator : ConfigurableActionGenerator
    {
        public FileSizePeakBluringGenerator(ObservableCollection<IGettingNumber> algorithmsOfGettingNumberFromFileName, 
            ObservableCollection<ByPath.IBestSelectingByPath> bestSelectingByPathList,
            ObservableCollection<DirectoryFilter.IDirectoryFilter> allDirectoryFilters,
            IUndoRedoEngine undoRedoEngine) :  base(undoRedoEngine)
        {
            Name = "FileSizePeakBluring";

            AlgorithmOfGettingNumberFromFileName = algorithmsOfGettingNumberFromFileName.Single(a => a is NumberBeforeUnderlineGettingNumber);

          
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.ResolutionDifferenceInPercent,
                Mode = new LessConditionMode(),
                ValueDouble = 5.0
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.NoTransformType,
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.SameType,
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.BlockinessDifference,
                Mode = new LessConditionMode(),
                ValueDouble = 7.0
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.BlurringDifference,
                Mode = new MoreConditionMode(),
                ValueDouble = 0.2
            });


            SelectingBestImageByProperty = new SelectingBestImageByProperty();
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.DctHistogramPeaksBest });
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.FileSizeBig });
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
    new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlurringBest });

            //BestSelectingByImagePropertyes = bestSelectingByImagePropertyesList.Single(s => s is ByImageProperty.FileSizeAndPeak);
            BestByPath = bestSelectingByPathList.Single(s => s is ByPath.BestImageShouldHaveASmallerNumber);
            //ActionCondition = ObjectModel.ActionCondition.BestImageByPropertyEqualBestImageByPath;
            //ActionOnDuplPair = ObjectModel.ActionOnDuplPair.DeleteBadImage;
            //ElseActionOnDuplPair = ObjectModel.ActionOnDuplPair.ReplaceBadImage;

            DirectoryFilter = allDirectoryFilters.Single(df => df is DirectoryFilter.SameDirectory);
        }
    }
}
