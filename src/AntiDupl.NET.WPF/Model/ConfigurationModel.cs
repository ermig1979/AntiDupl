using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Model
{
    public class ConfigurationModel : PropertyChangedBase, IConfigurationModel
    {
        [NonSerialized]
        private const string _fileName = @"configuration.xml";

        public ConfigurationModel()
        {
            ThumbnailWidth = 150;
            UndoRedoStackSize = 10;
            MaxCachedThumbnail = 400;
            GoodColor = Colors.LightGreen;
            BadColor = Colors.LightCoral;
            AlternateColor = Colors.WhiteSmoke;
            FolderAreDiffrentColor = Colors.AliceBlue;

            Language = new SerializableCultureInfo("en-US");
        }

        //public event EventHandler ConfigurationChanged;

        int _thumbnailWidth;
        public int ThumbnailWidth
        {
            get { return _thumbnailWidth; }
            set
            {
                _thumbnailWidth = value;
                //if (ConfigurationChanged != null)
                //    ConfigurationChanged(this, EventArgs.Empty);.
                RaisePropertyChangedEvent("ThumbnailWidth");
            }
        }

        ObservableCollection<Criterion> _criterions;
        public ObservableCollection<Criterion> Criterions
        {
            get { return _criterions; }
            set
            {
                _criterions = value;
                _criterions.CollectionChanged += criterions_CollectionChanged;
                RaisePropertyChangedEvent("Criterions");
            }
        }

        void criterions_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            if (e.OldItems != null)
            {
                foreach (INotifyPropertyChanged item in e.OldItems)
                    item.PropertyChanged -= criterion_PropertyChanged;
            }
            if (e.NewItems != null)
            {
                foreach (INotifyPropertyChanged item in e.NewItems)
                    item.PropertyChanged += criterion_PropertyChanged;
            }
        }

        bool calculateInProgress = false;
        void criterion_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "IsUse")
            {
                Criterion cr = sender as Criterion;
                if (cr.Type == CriterionType.LargerFileSize)
                {
                    var founded = _criterions.First(c => c.Type == CriterionType.SmallerFileSize);
                    founded.PropertyChanged -= criterion_PropertyChanged;
                    founded.IsUse = false;
                    founded.PropertyChanged += criterion_PropertyChanged;
                }
                else if (cr.Type == CriterionType.SmallerFileSize)
                {
                    var founded = _criterions.First(c => c.Type == CriterionType.LargerFileSize);
                    founded.PropertyChanged -= criterion_PropertyChanged;
                    founded.IsUse = false;
                    founded.PropertyChanged += criterion_PropertyChanged;
                }
            }
            else if (e.PropertyName == "Weight" && !calculateInProgress)
            {
                Criterion cr = sender as Criterion;
                if (cr.Weight > 1)
                    cr.Weight = 0.99M;

                calculateInProgress = true;
                SaatiHelper.RecalculateIndex(_criterions, cr);
                calculateInProgress = false;
            }
        }

        public SerializableCultureInfo Language { get; set; }

        public string LastResultFile { get; set; }

        public uint UndoRedoStackSize { get; set; }

        public uint MaxCachedThumbnail { get; set; }

        public bool SaveResultInCurrentFile { get; set; }

        Color _goodColor;
        public Color GoodColor
        {
            get { return _goodColor; }
            set
            {
                _goodColor = value;
                RaisePropertyChangedEvent("GoodColor");
            }
        }
        Color _badColor;
        public Color BadColor
        {
            get { return _badColor; }
            set
            {
                _badColor = value;
                RaisePropertyChangedEvent("BadColor");
            }
        }

        Color _alternateColor;
        public Color AlternateColor
        {
            get { return _alternateColor; }
            set
            {
                _alternateColor = value;
                RaisePropertyChangedEvent("BadColor");
            }
        }

        Color _folderAreDiffrentColor;
        public Color FolderAreDiffrentColor
        {
            get { return _folderAreDiffrentColor; }
            set
            {
                _folderAreDiffrentColor = value;
                RaisePropertyChangedEvent("FolderAreDiffrentColor");
            }
        }

		bool _loadResultOnOpen;
		public bool LoadResultOnOpen
		{
			get { return _loadResultOnOpen; }
			set
			{
				_loadResultOnOpen = value;
				RaisePropertyChangedEvent("LoadResultOnOpen");
			}
		}

		bool _saveResultOnClose;
		public bool SaveResultOnClose
		{
			get { return _saveResultOnClose; }
			set
			{
				_saveResultOnClose = value;
				RaisePropertyChangedEvent("SaveResultOnClose");
			}
		}

        public Brush AlternateBrush
        {
            get { return new SolidColorBrush(_alternateColor); }
        }

        public static ConfigurationModel Load()
        {
            //return new ConfigurationModel();

            ConfigurationModel model = SerializeHelper<ConfigurationModel>.Load(_fileName);
            if (model == null)
            {
                model = new ConfigurationModel();
            }
            if (model.Criterions == null || model.Criterions.Count != 5)
            {
                model.Criterions = model.GetDefaultCriterions();
            }

            return model;
        }

        public ObservableCollection<Criterion> GetDefaultCriterions()
        {
            var criterions = new ObservableCollection<Criterion>();
            criterions.Add(new Criterion() { Type = CriterionType.Resolution, Weight = 0.4M, IsUse = true });
            criterions.Add(new Criterion() { Type = CriterionType.SmallerFileSize, Weight = 0.00001M, IsUse = true });
            criterions.Add(new Criterion() { Type = CriterionType.LargerFileSize, IsUse = false });
            criterions.Add(new Criterion() { Type = CriterionType.Blockiness, Weight = 0.3001M, IsUse = true });
            criterions.Add(new Criterion() { Type = CriterionType.Bluring, Weight = 0.29989M, IsUse = true });
            return criterions;
        }

        public void Save()
        {
            SerializeHelper<ConfigurationModel>.Save(this, _fileName);
        }
    }
}
