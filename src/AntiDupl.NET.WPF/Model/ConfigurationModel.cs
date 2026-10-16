using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.Resources;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Model
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
                RaisePropertyChangedEvent();
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
                RaisePropertyChangedEvent();
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

        // ==== Deep (DINOv2) search: configuration of the AntiDupl.NET.Deep pipeline ====

        // Дефолт — true, как в оригинале (SearchSettings.GeometricCheckEnabled = true):
        // без WGC-верификации DINOv2-кластеры возвращают кучу ложных пар.
        bool _verifyByLightGlue = true;
        public bool VerifyByLightGlue
        {
            get { return _verifyByLightGlue; }
            set { _verifyByLightGlue = value; RaisePropertyChangedEvent(); }
        }

        string _lightGlueModelPath = "";
        public string LightGlueModelPath
        {
            get { return _lightGlueModelPath; }
            set { _lightGlueModelPath = value; RaisePropertyChangedEvent(); }
        }

        string _dinov2ModelPath = "";
        public string Dinov2ModelPath
        {
            get { return _dinov2ModelPath; }
            set { _dinov2ModelPath = value; RaisePropertyChangedEvent(); }
        }

        string _dinov2ModelUrl = "";
        public string Dinov2ModelUrl
        {
            get { return _dinov2ModelUrl; }
            set { _dinov2ModelUrl = value; RaisePropertyChangedEvent(); }
        }

        // Каталоги CUDA/cuDNN (как в оригинале: добавляются в PATH при старте, чтобы
        // ONNX Runtime нашел cudart/cublas/cudnn). После изменения — перезапуск приложения.
        string _cudaPath = "";
        public string CudaPath
        {
            get { return _cudaPath; }
            set { _cudaPath = value; RaisePropertyChangedEvent(); }
        }

        string _cudnnPath = "";
        public string CudnnPath
        {
            get { return _cudnnPath; }
            set { _cudnnPath = value; RaisePropertyChangedEvent(); }
        }

        // ==== Параметры deep-пайплайна (паритет SearchSettings референс-проекта) ====
        // Дефолты = оригинал, кроме MinMatchScore: 0.6 (осознанно строже оригинальных 0.5 —
        // меньше мягких ложных пар); WgcParallelism: 0 = авто (processor count / 2).
        // Старые configuration.xml без этих элементов: XmlSerializer оставит дефолты.

        double _deepMinSimilarityForPair = 0.5;
        public double DeepMinSimilarityForPair
        {
            get { return _deepMinSimilarityForPair; }
            set { _deepMinSimilarityForPair = value; RaisePropertyChangedEvent(); }
        }

        int _deepMaxClusterSize = 50;
        public int DeepMaxClusterSize
        {
            get { return _deepMaxClusterSize; }
            set { _deepMaxClusterSize = value; RaisePropertyChangedEvent(); }
        }

        double _deepTransitivityRatio = 0.7;
        public double DeepTransitivityRatio
        {
            get { return _deepTransitivityRatio; }
            set { _deepTransitivityRatio = value; RaisePropertyChangedEvent(); }
        }

        int _deepWgcParallelism = 0;
        public int DeepWgcParallelism
        {
            get { return _deepWgcParallelism; }
            set { _deepWgcParallelism = value; RaisePropertyChangedEvent(); }
        }

        double _deepMinMatchScore = 0.6;
        public double DeepMinMatchScore
        {
            get { return _deepMinMatchScore; }
            set { _deepMinMatchScore = value; RaisePropertyChangedEvent(); }
        }

        // Производительность пайплайна (паритет SearchSettings оригинала: batchSize=32, prefetch=2).
        int _deepBatchSize = 32;
        public int DeepBatchSize
        {
            get { return _deepBatchSize; }
            set { _deepBatchSize = value; RaisePropertyChangedEvent(); }
        }

        int _deepPrefetchCount = 2;
        public int DeepPrefetchCount
        {
            get { return _deepPrefetchCount; }
            set { _deepPrefetchCount = value; RaisePropertyChangedEvent(); }
        }

        public string LastResultFile { get; set; }

        /// <summary>
        /// Регулярное выражение — фильтр имён файлов для ignore-списка путей
        /// (зеркало WF options.xml → ignoreFilenameFilter).
        /// </summary>
        public string IgnoreFilenameFilter { get; set; }

        /// <summary>
        /// Вид таблицы результатов (пары/группы) — восстанавливается при перезапуске
        /// и сохраняется в configuration.xml при смене режима.
        /// </summary>
        public ViewMode ResultsViewMode { get; set; } = ViewMode.Pair;

        /// <summary>
        /// Горячие клавиши (зеркало WF options.xml → hotKeyOptions).
        /// </summary>
        public HotKeyOptions HotKeys { get; set; }

        /// <summary>
        /// Видимость колонок таблицы результатов. null = все колонки видимы.
        /// Сохраняется при закрытии окна, применяется при загрузке.
        /// </summary>
        public ResultsColumnsOptions ResultsColumns { get; set; }

        // Выбранный режим поиска (тулбар). null = не выбирался — по умолчанию DINOv2
        // (если ядро поддерживает). Сохраняется в configuration.xml.
        public AlgorithmComparing? CompareAlgorithm { get; set; }

        public uint UndoRedoStackSize { get; set; }

        public uint MaxCachedThumbnail { get; set; }


        /// <summary>
        /// При загрузке результата проверять существование файлов (лог прогресс-окна,
        /// зеркало WF options.xml → checkResultsAtLoading).
        /// </summary>
        public bool CheckResultsAtLoading { get; set; } = true;

        /// <summary>
        /// При загрузке результата фильтровать пары из ignore-списка (лог прогресс-окна,
        /// зеркало WF options.xml → checkMistakesAtLoading).
        /// </summary>
        public bool CheckMistakesAtLoading { get; set; } = true;

        /// <summary>
        /// Путь к внешней diff-утилите (зеркало WF imageDiffExecutablePath).
        /// </summary>
        public string ImageDiffExecutablePath { get; set; }

        /// <summary>
        /// Аргументы внешней diff-утилиты, плейсхолдеры %1/%2
        /// (зеркало WF imageDiffExecutableArguments).
        /// </summary>
        public string ImageDiffExecutableArguments { get; set; }

        /// <summary>
        /// Использовать только один путь поиска (зеркало WF options.xml → onePath):
        /// в ядро передаётся только первый путь из списка search-путей.
        /// </summary>
        public bool UseSinglePath { get; set; }

        Color _goodColor;
        public Color GoodColor
        {
            get { return _goodColor; }
            set
            {
                _goodColor = value;
                RaisePropertyChangedEvent();
            }
        }
        Color _badColor;
        public Color BadColor
        {
            get { return _badColor; }
            set
            {
                _badColor = value;
                RaisePropertyChangedEvent();
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
                RaisePropertyChangedEvent();
            }
        }

		bool _loadResultOnOpen = true;
		public bool LoadResultOnOpen
		{
			get { return _loadResultOnOpen; }
			set
			{
				_loadResultOnOpen = value;
				RaisePropertyChangedEvent();
			}
		}

		bool _saveResultOnClose = true;
		public bool SaveResultOnClose
		{
			get { return _saveResultOnClose; }
			set
			{
				_saveResultOnClose = value;
				RaisePropertyChangedEvent();
			}
		}

        public Brush AlternateBrush
        {
            get { return new SolidColorBrush(_alternateColor); }
        }

        public static ConfigurationModel Load()
        {
            //return new ConfigurationModel();

            ConfigurationModel model = SerializeHelper<ConfigurationModel>.Load(GetFilePath());
            if (model == null)
            {
                model = new ConfigurationModel();
            }
            if (model.Criterions == null || model.Criterions.Count != 5)
            {
                model.Criterions = model.GetDefaultCriterions();
            }

            if (model.HotKeys == null)
                model.HotKeys = new HotKeyOptions();

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
            SerializeHelper<ConfigurationModel>.Save(this, GetFilePath());
        }

        /// <summary>Полный путь: пользовательские данные — в <c>user\</c> (зеркало WF UserPath).</summary>
        static string GetFilePath() => Path.Combine(AntiDupl.NET.WPF.Resources.Resources.UserPath, _fileName);
    }
}
