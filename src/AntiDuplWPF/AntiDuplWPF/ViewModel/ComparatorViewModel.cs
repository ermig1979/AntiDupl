using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using AntiDuplWPF.Command;
using AntiDuplWPF.Helper;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.UndoRedo;
using AntiDuplWPF.View;

namespace AntiDuplWPF.ViewModel
{
    public class ComparatorViewModel : PropertyChangedBase
    {
        private DuplicateGroup _group;
        ObservableCollection<DuplicateGroup> _groups;
        ICollectionView _groupsView;


        private DuplPairViewModel _result;
        ObservableCollection<DuplPairViewModel> _resultList;
        IEditableCollectionView _collectionView;

        WindowService _windowService;
        IUndoRedoEngine _undoRedoEngine;

        LinkedList<ImageInfoClass> _list;
        public IEnumerable<ImageInfoClass> ImageInfos
        {
            get { return _list; }
        }

        IConfigurationModel _configuration;
        public IConfigurationModel Configuration
        {
            get { return _configuration; }
        }

        public ComparatorViewModel(IUndoRedoEngine undoRedoEngine, WindowService windowService,
           IConfigurationModel configuration)
        {
            _undoRedoEngine = undoRedoEngine;
            _windowService = windowService;
            _configuration = configuration;
        }

        public ComparatorViewModel(DuplicateGroup group, ObservableCollection<DuplicateGroup> groups,
            ICollectionView groupsView, IUndoRedoEngine undoRedoEngine, WindowService windowService,
            IConfigurationModel configuration)
            : this(undoRedoEngine, windowService, configuration)
        {
            _group = group;
            _groups = groups;
            _groupsView = groupsView;

            _list = new LinkedList<ImageInfoClass>(group.FileList);
            _current = _list.First;
            //_list.First.Next

            SaatiHelper.CalculateIndex(_list, configuration);
        }

        public ComparatorViewModel(DuplPairViewModel result, ObservableCollection<DuplPairViewModel> resultList,
            IEditableCollectionView collectionView, IUndoRedoEngine undoRedoEngine, WindowService windowService,
            IConfigurationModel configuration)
            : this(undoRedoEngine, windowService, configuration)
        {
            _result = result;
            _resultList = resultList;
            _collectionView = collectionView;

            List<ImageInfoClass> list = new List<ImageInfoClass>();
            list.Add(result.FirstFile);
            list.Add(result.SecondFile);
            _list = new LinkedList<ImageInfoClass>(list);
            _current = _list.First;
            SaatiHelper.CalculateIndex(_list, configuration);
        }

        LinkedListNode<ImageInfoClass> _current;
        public LinkedListNode<ImageInfoClass> Current
        {
            get { return _current; }
            set
            {
                _current = value;
                RaisePropertyChangedEvent("Current");
                RaisePropertyChangedEvent("CurrentInfo");
                RaisePropertyChangedEvent("Image");
            }
        }

        public ImageInfoClass CurrentInfo
        {
            get
            {
                return _current.Value;
            }
        }

        public BitmapImage Image
        {
            get
            {
                return ReadImage(_current.Value.Path);
            }
        }

        private BitmapImage ReadImage(string path)
        {
            if (File.Exists(path))
            {
                using (Stream stream = File.OpenRead(path))
                {
                    var bitmap = new BitmapImage();
                    bitmap.BeginInit();
                    bitmap.CreateOptions = BitmapCreateOptions.IgnoreColorProfile;
                    bitmap.CacheOption = BitmapCacheOption.OnLoad;
                    //bitmap.DecodePixelWidth = Const.IMAGE_WIDTH;
                    //bitmap.DecodePixelHeight = Const.IMAGE_HEIGHT;
                    bitmap.StreamSource = stream;
                    bitmap.EndInit();
                    bitmap.Freeze();
                    return bitmap;
                }
            }

            return null;
        }

        ICommand _previousCommand;
        public ICommand PreviousCommand
        {
            get
            {
                return _previousCommand ?? (_previousCommand = new RelayCommand(arg =>
                {
                    Current = Current.Previous;
                }, arg => _current != null && _current.Previous != null));
            }
        }

        ICommand _nextCommand;
        public ICommand NextCommand
        {
            get
            {
                return _nextCommand ?? (_nextCommand = new RelayCommand(arg =>
                {
                    Current = Current.Next;
                }, arg => _current != null
                && _current.Next != null));
            }
        }

        ICommand _deleteOtherCommand;
        public ICommand DeleteOtherCommand
        {
            get
            {
                return _deleteOtherCommand ?? (_deleteOtherCommand = new RelayCommand(arg =>
                {
                    if (_groups != null && _groupsView != null)
                    {
                        DeleteOtherFromGroupCommand command = new DeleteOtherFromGroupCommand(CurrentInfo, _groups, _groupsView);
                        _undoRedoEngine.ExecuteCommand(command);
                        _windowService.CloseComparatorWindow();
                    }
                    else if (_result != null && _collectionView != null)
                    {
                        DeleteOtherFromPairCommand command = new DeleteOtherFromPairCommand(CurrentInfo, _result, _resultList,
                            _collectionView, ((ICollectionView)_collectionView).CurrentPosition);
                        _undoRedoEngine.ExecuteCommand(command);
                        _windowService.CloseComparatorWindow();
                    }
                }));
            }
        }
    }
}
