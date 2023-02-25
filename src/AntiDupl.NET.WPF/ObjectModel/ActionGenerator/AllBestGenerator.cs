using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel.ByImageProperty;
using AntiDupl.NET.WPF.ObjectModel.ByPath;
using AntiDupl.NET.WPF.ObjectModel.ConditionMode;
using AntiDupl.NET.WPF.ObjectModel.DirectoryFilter;
using AntiDupl.NET.WPF.ObjectModel.GettingNumber;
using AntiDupl.NET.WPF.UndoRedo;

namespace AntiDupl.NET.WPF.ObjectModel.ActionGenerator
{
    class AllBestGenerator : ConfigurableActionGenerator
    {
        public AllBestGenerator(ObservableCollection<IGettingNumber> algorithmsOfGettingNumberFromFileName, 
            ObservableCollection<ByPath.IBestSelectingByPath> bestSelectingByPathList, 
            ObservableCollection<IDirectoryFilter> allDirectoryFilters,
            IUndoRedoEngine undoRedoEngine) :  base(undoRedoEngine)
        {
            Name = "AllBest";

            AlgorithmOfGettingNumberFromFileName = algorithmsOfGettingNumberFromFileName.Single(a => a is NumberBeforeUnderlineGettingNumber);

     
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.NoTransformType,
                //Mode = new EqualConditionMode(),
                // TransformType = Core.CoreDll.TransformType.Turn_0
            });
            FilterConditions.Add(new FilterCondition()
            {
                Parametr = FilterConditionParametr.SameType,
            });

   
            SelectingBestImageByProperty = new SelectingBestImageByProperty();
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlockinessBest } );
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlurringBest } );
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.DctHistogramPeaksBest });
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.FileSizeBig });
            SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
                new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.ResolutionBig });

            //BestSelectingByImagePropertyes = bestSelectingByImagePropertyesList.Single(s => s is FileSizeResolutionBluringPeak);
            BestByPath = bestSelectingByPathList.Single(s => s is BestImageShouldHaveASmallerNumber);
            //ActionCondition = ObjectModel.ActionCondition.BestImageByPropertyEqualBestImageByPath;
            //ActionOnDuplPair = ObjectModel.ActionOnDuplPair.DeleteBadImage;
            //ElseActionOnDuplPair = ObjectModel.ActionOnDuplPair.ReplaceBadImage;

            DirectoryFilter = allDirectoryFilters.Single(df => df is SameDirectory);
        }

    }
}
