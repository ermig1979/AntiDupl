using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ObjectModel;

namespace AntiDupl.NET.WPF.UndoRedo
{
    /// <summary>
    /// Автоматическое удаление по значению Саати индекса
    /// для нескольких выделенных групп дубликатов.
    /// В каждой группе сохраняется файл с лучшим индексом,
    /// остальные файлы перемещаются в резервную область.
    /// </summary>
    class AutoDeleteMultiGroupCommand : IUCommand
    {
        struct Renamed
        {
            public ImageInfoClass ImageInfo;
            public string PathWithTempExtension;
        }

        private IList _groupsForDelete;
        private List<DuplicateGroup> _groupsForDeleteCopy;
        private ObservableCollection<DuplicateGroup> _sourceGroups;
        private List<Renamed> _renamedFiles = new List<Renamed>();
        private string _description;

        public AutoDeleteMultiGroupCommand(IList groupsForDelete,
            ObservableCollection<DuplicateGroup> sourceGroups)
        {
            _groupsForDelete = groupsForDelete;
            _groupsForDeleteCopy = groupsForDelete.Cast<DuplicateGroup>().ToList();
            _sourceGroups = sourceGroups;
        }

        public bool Execute()
        {
            // После UnExecute файлы вернулись на места: перечитаем список с нуля,
            // иначе redo добавит те же записи повторно и следующий undo/dispose
            // упадёт на повторном FileMove отсутствующего временного пути.
            _renamedFiles.Clear();
            foreach (DuplicateGroup group in _groupsForDeleteCopy)
            {
                if (group == null || group.FileList == null || group.FileList.Count == 0)
                    continue;

                ImageInfoClass bestImageInfo = GetBestImage(group);
                // Группа непустая (пустые пропущены выше) — лучший файл обязан быть.
                Debug.Assert(bestImageInfo != null, "non-empty group must have a best image");
                foreach (var image in group.FileList)
                {
                    if (bestImageInfo != null && !image.Equals(bestImageInfo))
                        Rename(image);
                }

                _sourceGroups.Remove(group);
            }

            string res = Application.Current.Resources["desc_deleteFiles"] as string;
            _description = string.Format(res, _renamedFiles.Count);
            return true;
        }

        private void Rename(ImageInfoClass image)
        {
            if (File.Exists(image.Path))
            {
                Renamed renamed = new Renamed();
                renamed.ImageInfo = image;
                renamed.PathWithTempExtension = image.Path + Const.ForDeleteEXTENSION;
                _renamedFiles.Add(renamed);
                // Занятый временный путь — двойное переименование одного файла:
                // FileMove бросит только на самом Move с неясной причиной.
                Debug.Assert(!File.Exists(renamed.PathWithTempExtension),
                    $"temp path must be free: {renamed.PathWithTempExtension}");
                File.Move(image.Path, renamed.PathWithTempExtension);
            }
        }

        private static ImageInfoClass GetBestImage(DuplicateGroup group)
        {
            ImageInfoClass bestImageInfo = null;
            foreach (var image in group.FileList)
            {
                if (bestImageInfo == null)
                {
                    bestImageInfo = image;
                    continue;
                }
                if (image.UtilityIndex > bestImageInfo.UtilityIndex)
                    bestImageInfo = image;
            }
            return bestImageInfo;
        }

        public bool UnExecute()
        {
            foreach (Renamed item in _renamedFiles)
            {
                File.Move(item.PathWithTempExtension, item.ImageInfo.Path);
            }
            foreach (var group in _groupsForDeleteCopy)
            {
                if (!_sourceGroups.Contains(group))
                    _sourceGroups.Add(group);
            }
            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            //Удаляем физически. В корзину.
            foreach (Renamed item in _renamedFiles)
            {
                File.Move(item.PathWithTempExtension, item.ImageInfo.Path);
                WinAPIMethods.DeleteToRecycleBin(item.ImageInfo.Path);
            }
        }
    }
}