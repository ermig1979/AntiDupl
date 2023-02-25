using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.Windows.Media;
using AntiDupl.NET.WPF.ObjectModel;

namespace AntiDupl.NET.WPF.Model
{
    public interface IConfigurationModel : INotifyPropertyChanged
    {
        int ThumbnailWidth { get; set; }

        ObservableCollection<Criterion> Criterions { get; set; }
        SerializableCultureInfo Language { get; set; }
        string LastResultFile { get; set; }
        uint UndoRedoStackSize { get; set; }
        uint MaxCachedThumbnail { get; set; }
        bool SaveResultInCurrentFile { get; set; }

        Color GoodColor { get; set; }
        Color BadColor { get; set; }
        Color AlternateColor { get; set; }
        Brush AlternateBrush { get; }

		bool LoadResultOnOpen { get; set; }
		bool SaveResultOnClose { get; set; }

        void Save();
        ObservableCollection<Criterion> GetDefaultCriterions();
    }
}
