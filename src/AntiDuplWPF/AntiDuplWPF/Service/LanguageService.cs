using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using AntiDuplWPF.Command;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Service
{
    class LanguageService : ILanguageService
    {
        //Евент для оповещения всех окон приложения
        public event EventHandler LanguageChanged;

        IConfigurationModel _config;

        public LanguageService(IConfigurationModel config)
        {
            _config = config;

            foreach (LanguageEnum value in Enum.GetValues(typeof(LanguageEnum)))
            {
                LanguageViewModel myClass = new LanguageViewModel();
                myClass.Enum = value;
                myClass.IsChecked = value == LanguageEnum.Russian ? true : false; //check value is last
                myClass.Title = Enum.GetName(typeof(LanguageEnum), value);
                _languageCollection.Add(myClass);
            }
            if (_config != null)
            {
                Language = _config.Language.ToCultureInfo();
                LanguageEnum langEnum = GetLanguageEnumFromCulture(Language);
                SetCheckedForLanguageCollection(langEnum, LanguageCollection);
            }
            else //по умолчанию
                Language = new CultureInfo("en-US");
        }


        ObservableCollection<LanguageViewModel> _languageCollection = new ObservableCollection<LanguageViewModel>();
        public ObservableCollection<LanguageViewModel> LanguageCollection
        {
            get
            {
                return _languageCollection;
            }
        }

        ICommand _languageSelectedCommand;
        public ICommand LanguageSelectedCommand
        {
            get
            {
                return _languageSelectedCommand ?? (_languageSelectedCommand = new RelayCommand(arg =>
                {
                    LanguageEnum langEnum = (LanguageEnum)arg;
                    SetCheckedForLanguageCollection(langEnum, LanguageCollection);

                    CultureInfo newLanguage = GetCultureFromLanguageEnum(langEnum);
                    if (_config != null)
                        _config.Language = SerializableCultureInfo.FromCultureInfo(newLanguage);
                    Language = newLanguage;
                }));
            }
        }

        private void SetCheckedForLanguageCollection(LanguageEnum langEnum, ObservableCollection<LanguageViewModel> collection)
        {
            LanguageViewModel theClass = collection.First<LanguageViewModel>(t => t.Enum == langEnum);

            // ok, they want to check this one, let them and uncheck all else
            foreach (var iter in collection)
            {
                iter.IsChecked = false;
            }
            theClass.IsChecked = true;
        }

        private CultureInfo GetCultureFromLanguageEnum(LanguageEnum langEnum)
        {
            switch (langEnum)
            {
                case LanguageEnum.English:
                    return new CultureInfo("en-US");
                case LanguageEnum.Russian:
                    return new CultureInfo("ru-RU");
            }
            throw new ArgumentException("Неизвестное значение LanguageEnum " + langEnum);
        }

        private LanguageEnum GetLanguageEnumFromCulture(CultureInfo culture)
        {
            switch (culture.Name)
            {
                case "en-US":
                    return LanguageEnum.English;
                case "ru-RU":
                    return LanguageEnum.Russian;
            }
            throw new ArgumentException(String.Format("{0} нет в поддерживаемых языках", culture.Name));
        }

        public CultureInfo Language
        {
            get
            {
                return System.Threading.Thread.CurrentThread.CurrentUICulture;
            }
            set
            {
                if (value == null) throw new ArgumentNullException("value");
                if (value == System.Threading.Thread.CurrentThread.CurrentUICulture) return;

                //1. Меняем язык приложения:
                System.Threading.Thread.CurrentThread.CurrentUICulture = value;

                //2. Создаём ResourceDictionary для новой культуры
                ResourceDictionary dict = new ResourceDictionary();
                switch (value.Name)
                {
                    case "ru-RU":
                        dict.Source = new Uri(String.Format("Resources/lang.{0}.xaml", value.Name), UriKind.Relative);
                        break;
                    default:
                        dict.Source = new Uri("Resources/lang.xaml", UriKind.Relative);
                        break;
                }

                //3. Находим старую ResourceDictionary и удаляем его и добавляем новую ResourceDictionary
                if (Application.Current.Resources.MergedDictionaries.Count > 0)
                {
                    ResourceDictionary oldDict = (from d in Application.Current.Resources.MergedDictionaries
                                                  where d.Source != null && d.Source.OriginalString.StartsWith("Resources/lang.")
                                                  select d).First();
                    if (oldDict != null)
                    {
                        int ind = Application.Current.Resources.MergedDictionaries.IndexOf(oldDict);
                        Application.Current.Resources.MergedDictionaries.Remove(oldDict);
                        Application.Current.Resources.MergedDictionaries.Insert(ind, dict);
                    }
                }
                else
                {
                    Application.Current.Resources.MergedDictionaries.Add(dict);
                }

                //4. Вызываем евент для оповещения всех окон.
                if (LanguageChanged != null)
                    LanguageChanged(Application.Current, new EventArgs());
            }
        }
    }
}
