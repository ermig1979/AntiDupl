using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.UndoRedo
{
    class AutoDeleteMultiPairCommand : IUCommand
    {
        private IList _resultsForDelete;
        private List<DuplPairViewModel> _resultsForIgnoreCopy;
        private ObservableCollection<DuplPairViewModel> _sourceResultList;
        List<DuplPairViewModel> _resultsForDeleteCopy;
        string _description;

        /// <summary>
        /// Группы дубликатов
        /// </summary>
        List<DuplicateGroup> _groups;

        struct Renamed
        {
            public ImageInfoClass ImageInfo;
            public string PathWithTempExtension;
            //public ViewResultOfDupl Result;
        }
        List<Renamed> _renamedFiles;

        public AutoDeleteMultiPairCommand(IList resultsForDelete,
            ObservableCollection<DuplPairViewModel> sourceResultList)
        {
            _resultsForDelete = resultsForDelete;
            _resultsForDeleteCopy = resultsForDelete.Cast<DuplPairViewModel>().ToList();
            _sourceResultList = sourceResultList;
            _renamedFiles = new List<Renamed>();
            _groups = new List<DuplicateGroup>();
        }

        public bool Execute()
        {
            GroupHelper.ConvertToGroup(_groups, _resultsForDeleteCopy);
            foreach (DuplicateGroup group in _groups)
            {
                ImageInfoClass bestImageInfo = GetBestImage(group);
                foreach (var image in group.FileList)
                {
                    if (!image.Equals(bestImageInfo))
                        Rename(image);
                }
            }
            foreach (var item in _resultsForDeleteCopy)
            {
                _sourceResultList.Remove(item);
            }
        

            string res = Application.Current.Resources["desc_deleteFiles"] as string;
            _description = String.Format(res, _renamedFiles.Count);
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
                //IsFileLocked(renamed.ImageInfo.Path);
                File.Move(image.Path, renamed.PathWithTempExtension);
            }
        }

        private ImageInfoClass GetBestImage(DuplicateGroup group)
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

        private bool IsFileLocked(string filename)
        {
            FileInfo file = new FileInfo(filename);
            FileStream stream = null;

            try
            {
                stream = file.Open(FileMode.Open, FileAccess.ReadWrite, FileShare.None);
            }
            catch (IOException)
            {
                return true;
            }
            finally
            {
                if (stream != null)
                    stream.Close();
            }

            //file is not locked
            return false;
        }

        private ImageInfoClass GetBadImage(DuplPairViewModel deleteResult)
        {
            if (deleteResult.FirstFile.UtilityIndex > deleteResult.SecondFile.UtilityIndex)
                return deleteResult.SecondFile;
            else if (deleteResult.FirstFile.UtilityIndex < deleteResult.SecondFile.UtilityIndex)
                return deleteResult.FirstFile;
            else
                throw new ArgumentException("Не знаю какой файл худщий!");
        }

        public bool UnExecute()
        {
            foreach (Renamed item in _renamedFiles)
            {
                File.Move(item.PathWithTempExtension, item.ImageInfo.Path);
                //_resultsForDelete.Add(item.Result);
            }
            foreach (var item in _resultsForDeleteCopy)
            {
                _sourceResultList.Add(item);
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
