using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.Windows.Media;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.Model
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

        void Save();
    }
}
