using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ViewModel
{
    public enum LanguageEnum
    {
        English = 1,
        Russian = 2
    }

    public class LanguageViewModel : PropertyChangedBase
    {
        public LanguageViewModel()
        {
            IsChecked = false;
        }

        public LanguageEnum Enum { get; set; }
        private bool _isChecked = false;

        public bool IsChecked
        {
            get { return _isChecked; }
            set
            {
                _isChecked = value;
                RaisePropertyChangedEvent("IsChecked");
            }

        }

        public string Title { get; set; }
    }
}
