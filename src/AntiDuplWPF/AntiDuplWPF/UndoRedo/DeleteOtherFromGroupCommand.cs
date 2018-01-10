using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.UndoRedo
{
    struct RenamedImage
    {
        public ImageInfoClass ImageInfo;
        public string PathWithTempExtension;

        public RenamedImage(ImageInfoClass item)
        {
            ImageInfo = item;
            PathWithTempExtension = item.Path + Const.ForDeleteEXTENSION;
        }
    }

    class DeleteOtherFromGroupCommand : IUCommand
    {
        private ImageInfoClass _forSaveInfo;
        private ObservableCollection<DuplicateGroup> _groups;
        private ICollectionView _groupsView;

        List<RenamedImage> _forDeleteList;
        int _index;
        DuplicateGroup _savedGroup;
        string _description;

        public DeleteOtherFromGroupCommand(ImageInfoClass currentInfo,
            ObservableCollection<DuplicateGroup> groups, ICollectionView groupsView)
        {
            _forSaveInfo = currentInfo;
            _groups = groups;
            _groupsView = groupsView;

            _forDeleteList = new List<RenamedImage>();
        }

        public bool Execute()
        {
            _index = _groupsView.CurrentPosition;
            _savedGroup = _groups[_index];

            foreach (var item in _savedGroup.FileList)
            {
                if (!item.Equals(_forSaveInfo))
                    _forDeleteList.Add(new RenamedImage(item));
            }

            foreach (var image in _forDeleteList)
            {
                File.Move(image.ImageInfo.Path, image.PathWithTempExtension);
            }

            _groups.RemoveAt(_index);
            //_groupsView.Refresh();

            string res = Application.Current.Resources["desc_deleteOtherFromGroup"] as string;
            _description = String.Format(res, _forSaveInfo.Path);

            return true;
        }

        public bool UnExecute()
        {
            foreach (var image in _forDeleteList)
            {
                File.Move(image.PathWithTempExtension, image.ImageInfo.Path);
            }

            _groups.Insert(_index, _savedGroup);

            return true;
        }

        public string Description
        {
            get { return _description; }
        }

        public void Dispose()
        {
            foreach (var image in _forDeleteList)
            {
                File.Move(image.PathWithTempExtension, image.ImageInfo.Path);
                WinAPIMethods.DeleteToRecycleBin(image.ImageInfo.Path);
            }
        }
    }
}
