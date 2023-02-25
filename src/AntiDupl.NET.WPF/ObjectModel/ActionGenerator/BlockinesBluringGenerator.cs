using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel.ByImageProperty;
using AntiDupl.NET.WPF.ObjectModel.ConditionMode;

namespace AntiDupl.NET.WPF.ObjectModel.ActionGenerator
{
	class BlockinesBluringGenerator : ConfigurableActionGenerator
	{

		public BlockinesBluringGenerator(ObservableCollection<GettingNumber.IGettingNumber> algorithmsOfGettingNumberFromFileName, 
			ObservableCollection<ByPath.IBestSelectingByPath> bestSelectingByPathList, 
			ObservableCollection<DirectoryFilter.IDirectoryFilter> allDirectoryFilters,
			UndoRedo.IUndoRedoEngine undoRedoEngine)
			: base(undoRedoEngine)
		{
			Name = "Blockines and Bluring";

			AlgorithmOfGettingNumberFromFileName = algorithmsOfGettingNumberFromFileName.Single(a => a is AntiDupl.NET.WPF.ObjectModel.GettingNumber.NumberBeforeUnderlineGettingNumber);


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
				Parametr = FilterConditionParametr.ResolutionDifference,
				Mode = new EqualConditionMode(),
				ValueUint = 0
			});
			FilterConditions.Add(new FilterCondition()
			{
				Parametr = FilterConditionParametr.FileSizeDifferenceInPercent,
				Mode = new LessConditionMode(),
				ValueDouble = 25.0
			});
			FilterConditions.Add(new FilterCondition()
			{
				Parametr = FilterConditionParametr.DctHistogramPeaksDifferenceInPercent,
				Mode = new LessConditionMode(),
				ValueDouble = 5.0
			});

			SelectingBestImageByProperty = new SelectingBestImageByProperty();
			SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
				new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlockinessBest });
			SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
				new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlurringBest });

			BestByPath = bestSelectingByPathList.Single(s => s is AntiDupl.NET.WPF.ObjectModel.ByPath.BestImageShouldHaveASmallerNumber);

			DirectoryFilter = allDirectoryFilters.Single(df => df is AntiDupl.NET.WPF.ObjectModel.DirectoryFilter.SameDirectory);
		}
	}
}
