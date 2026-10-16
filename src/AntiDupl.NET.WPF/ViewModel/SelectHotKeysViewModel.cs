using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Windows.Input;
using AntiDupl.NET.WPF.Command;
using AntiDupl.NET.WPF.Model;

namespace AntiDupl.NET.WPF.ViewModel
{
    /// <summary>
    /// Строка таблицы «Действие / Включено / Клавиша» в окне SelectHotKeysWindow.
    /// </summary>
    class HotKeyRowViewModel : PropertyChangedBase
    {
        readonly HotKeyOptions _options;
        readonly HotKeyActionItem _item;

        public HotKeyActionItem Item
        {
            get { return _item; }
        }

        public HotKeyRowViewModel(HotKeyOptions options, HotKeyActionItem item)
        {
            _options = options;
            _item = item;
            ActionName = GetActionName(item.Action);
            UpdateState();
        }

        static string GetActionName(HotKeyAction action)
        {
            System.Windows.Application app = System.Windows.Application.Current;
            if (app != null && app.Resources != null)
            {
                string key = "hotkey_" + action;
                if (app.Resources[key] is string text)
                    return text;
            }
            return action.ToString();
        }

        public string ActionName { get; private set; }

        bool _enabled;
        /// <summary>Привязка включена (клавиша задана).</summary>
        public bool Enabled
        {
            get { return _enabled; }
            set
            {
                if (_enabled != value)
                {
                    _enabled = value;
                    RaisePropertyChangedEvent();
                    // Выключение — очищаем клавишу; включение — ставим дефолт действия.
                    if (value)
                        _item.Key = _options.GetDefaultKeyFor(_item.Action);
                    else
                        _item.Key = "";
                    UpdateState();
                    OptionsChanged?.Invoke(this, EventArgs.Empty);
                }
            }
        }

        string _keyDisplay;
        public string KeyDisplay
        {
            get { return _keyDisplay; }
            set
            {
                if (_keyDisplay != value)
                {
                    _keyDisplay = value;
                    RaisePropertyChangedEvent();
                }
            }
        }

        bool _isValid = true;
        public bool IsValid
        {
            get { return _isValid; }
            set
            {
                if (_isValid != value)
                {
                    _isValid = value;
                    RaisePropertyChangedEvent();
                }
            }
        }

        public event EventHandler OptionsChanged;

        public void UpdateState()
        {
            _enabled = !String.IsNullOrEmpty(_item.Key);
            RaisePropertyChangedEvent("Enabled");
            _keyDisplay = _options.ToDisplay(_item.Key);
            RaisePropertyChangedEvent("KeyDisplay");
            _isValid = _options.Valid(_item.Action);
            RaisePropertyChangedEvent("IsValid");
        }
    }

    /// <summary>
    /// VM окна выбора горячих клавиш (зеркало WF SelectHotKeysForm).
    /// Работает с копией HotKeyOptions; OK-результат присваивает вызывающий код.
    /// </summary>
    class SelectHotKeysViewModel : CloseableViewModel
    {
        readonly HotKeyOptions _options;
        public HotKeyOptions Options { get { return _options; } }

        ObservableCollection<HotKeyRowViewModel> _rows;
        public ObservableCollection<HotKeyRowViewModel> Rows { get { return _rows; } }

        public event EventHandler OptionsChanged;

        public SelectHotKeysViewModel(HotKeyOptions current)
        {
            _options = new HotKeyOptions(current ?? new HotKeyOptions());
            BuildRows();
            RefreshRowStates();
        }

        void BuildRows()
        {
            ObservableCollection<HotKeyRowViewModel> newRows = new ObservableCollection<HotKeyRowViewModel>();
            foreach (var item in Options.Items)
            {
                HotKeyRowViewModel row = new HotKeyRowViewModel(Options, item);
                row.OptionsChanged += OnRowOptionsChanged;
                newRows.Add(row);
            }
            _rows = newRows;
            RaisePropertyChangedEvent("Rows");
        }

        /// <summary>Сброс к значениям по умолчанию (зеркало WF Set default).</summary>
        public void SetDefault()
        {
            Options.SetDefault();
            BuildRows();
            RefreshRowStates();
            OptionsChanged?.Invoke(this, EventArgs.Empty);
        }

        void OnRowOptionsChanged(object sender, EventArgs e)
        {
            RefreshRowStates();
            OptionsChanged?.Invoke(this, EventArgs.Empty);
        }

        /// <summary>Пересчитывает валидность всех строк (коллизии зависят от соседей).</summary>
        public void RefreshRowStates()
        {
            foreach (var row in Rows)
                row.UpdateState();
            RaisePropertyChangedEvent("CanOk");
        }

        /// <summary>OK-кнопка разрешена при валидности (зеркало WF UpdateBottomsEnabling).</summary>
        public bool CanOk { get { return Options.Valid(); } }

        ICommand _okCommand;
        public ICommand OkCommand
        {
            get
            {
                return _okCommand ?? (_okCommand = new RelayCommand(arg =>
                {
                    if (Options.Valid())
                        OnClosingRequest(true);
                }, arg => Options.Valid()));
            }
        }
    }
}