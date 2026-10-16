using System;
using System.Collections.ObjectModel;
using System.IO;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.UndoRedo
{
    /// <summary>
    /// Удаление дефектного изображения (дефекты). Локальный вариант зеркала
    /// WF ApplyToResult(LocalActionType.DeleteDefect): файл уводится с временным
    /// расширением, строка результата удаляется; UnExecute возвращает файл.
    /// (CoreLib.ApplyToResult требует «current» в ядре, которого нет для
    /// загруженных из профиля результатов.)
    /// </summary>
    class DeleteDefectCommand : IUCommand
    {
        readonly DuplPairViewModel _result;
        readonly ImageInfoClass _forDelete;
        readonly ObservableCollection<DuplPairViewModel> _resultList;

        string _deletedFileWithTempExtension;
        string _description;

        public DeleteDefectCommand(DuplPairViewModel result, ImageInfoClass forDelete,
            ObservableCollection<DuplPairViewModel> resultList)
        {
            _result = result;
            _forDelete = forDelete;
            _resultList = resultList;
        }

        public bool Execute()
        {
            _deletedFileWithTempExtension = _forDelete.Path + Const.ForDeleteEXTENSION;
            File.Move(_forDelete.Path, _deletedFileWithTempExtension);
            _description = String.Format(
                System.Windows.Application.Current.Resources["desc_deleteDefect"] as string ?? "Delete defect {0}",
                _forDelete.Path);

            _resultList.Remove(_result);
            return true;
        }

        public bool UnExecute()
        {
            File.Move(_deletedFileWithTempExtension, _forDelete.Path);
            _resultList.Add(_result);
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            WinAPIMethods.DeleteToRecycleBin(_deletedFileWithTempExtension);
        }
    }
}