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
    class FileSizePeakGenerator : ConfigurableActionGenerator
    {
        public FileSizePeakGenerator(ObservableCollection<IGettingNumber> algorithmsOfGettingNumberFromFileName, 
            ObservableCollection<ByPath.IBestSelectingByPath> bestSelectingByPathList,
            ObservableCollection<DirectoryFilter.IDirectoryFilter> allDirectoryFilters,
            IUndoRedoEngine undoRedoEngine) :  base(undoRedoEngine)
        {
            Name = "FileSizePeak";

            AlgorithmOfGettingNumberFromFileName = algorithmsOfGettingNumberFromFileName.Single(a => a is NumberBeforeUnderlineGettingNumber);

          
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.DctHistogramPeaksDifferenceInPercent,
                Mode = new MoreConditionMode(),
                ValueDouble = 40.0
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
                ValueDouble = 5.0
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.BlurringDifference,
                Mode = new LessConditionMode(),
                ValueDouble = 0.2
            });


            SelectingBestImageByProperty = new SelectingBestImageByProperty();
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.DctHistogramPeaksBest });
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.FileSizeBig });

            BestByPath = bestSelectingByPathList.Single(s => s is ByPath.BestImageShouldHaveASmallerNumber);

            DirectoryFilter = allDirectoryFilters.Single(df => df is DirectoryFilter.SameDirectory);
        }
    }

}
