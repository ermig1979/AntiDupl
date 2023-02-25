using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
namespace AntiDuplWPF.ObjectModel.ConfigurableAction
{
    public interface IConfigurableAction
    {
        string Name { get; }

        string BestImage { get; }

        string BestByPath { get; }

        string ToString();

        bool Excute(UndoRedo.IUndoRedoEngine undoRedoEngine, ObservableCollection<ViewModel.DuplPairViewModel> resultList, 
            IEditableCollectionView resultCollectionView);
    }
}
