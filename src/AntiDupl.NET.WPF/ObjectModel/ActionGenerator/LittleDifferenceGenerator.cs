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
	class LittleDifferenceGenerator : ConfigurableActionGenerator
	{
		public LittleDifferenceGenerator(ObservableCollection<GettingNumber.IGettingNumber> algorithmsOfGettingNumberFromFileName, 
			ObservableCollection<ByPath.IBestSelectingByPath> bestSelectingByPathList, 
			ObservableCollection<DirectoryFilter.IDirectoryFilter> allDirectoryFilters,
			UndoRedo.IUndoRedoEngine undoRedoEngine)
			: base(undoRedoEngine)
		{
			Name = "Little difference";

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
				ValueDouble = 2.0
			});
			FilterConditions.Add(new FilterCondition()
			{
				Parametr = FilterConditionParametr.BlockinessDifference,
				Mode = new LessConditionMode(),
				ValueDouble = 0.004
			});
			FilterConditions.Add(new FilterCondition()
			{
				Parametr = FilterConditionParametr.BlurringDifference,
				Mode = new LessConditionMode(),
				ValueDouble = 0.001
			});
			FilterConditions.Add(new FilterCondition()
			{
				Parametr = FilterConditionParametr.DctHistogramPeaksDifferenceInPercent,
				Mode = new LessConditionMode(),
				ValueDouble = 9.0
			});

			SelectingBestImageByProperty = new SelectingBestImageByProperty();
			SelectingBestImageByProperty.ConditionsOfSelectingBestImageByProperty.Add(
				new ConditionOfSelectingBestImageByProperty() { Parametr = BestConditionParametr.BlockinessBest });

			BestByPath = bestSelectingByPathList.Single(s => s is AntiDupl.NET.WPF.ObjectModel.ByPath.BestImageShouldHaveASmallerNumber);

			DirectoryFilter = allDirectoryFilters.Single(df => df is AntiDupl.NET.WPF.ObjectModel.DirectoryFilter.SameDirectory);
		}
	}
}
