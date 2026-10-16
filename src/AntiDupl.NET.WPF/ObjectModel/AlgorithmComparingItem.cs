using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.ObjectModel
{
    /// <summary>
    /// Элемент выпадающего списка режимов сравнения изображений.
    /// </summary>
    public class AlgorithmComparingItem : PropertyChangedBase
    {
        public AlgorithmComparing Value { get; set; }

        string _name;
        public string Name
        {
            get { return _name; }
            set
            {
                if (_name != value)
                {
                    _name = value;
                    RaisePropertyChangedEvent();
                }
            }
        }

        public AlgorithmComparingItem(AlgorithmComparing value, string name)
        {
            Value = value;
            _name = name;
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
