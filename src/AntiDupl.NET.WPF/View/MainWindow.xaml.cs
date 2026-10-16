using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.View
{
    public partial class MainWindow
    {
        public MainWindow()
        {
            InitializeComponent();

            Loaded += (sender, e) =>
            {
                if (DataContext is MainViewModel vm)
                {
                    vm.ResultGrid = (DataGrid)((FrameworkElement)Content).FindName("ResultDataGrid");
                    ApplyHotKeyBindings(vm);
                    vm.HotKeysChanged += (s, _) => ApplyHotKeyBindings(vm);
                }
            };
        }

        readonly List<KeyBinding> _dynamicKeyBindings = new List<KeyBinding>();
        readonly List<CommandBinding> _dynamicCommandBindings = new List<CommandBinding>();

        /// <summary>
        /// Пересобирает динамические InputBindings/CommandBindings по Configuration.HotKeys
        /// (зеркало WF ApplyHotKeysCommand). Статические KeyBinding'и из XAML
        /// (Ctrl+A/C/Z/Y) не трогаем — удаляем только ранее добавленные динамические.
        /// </summary>
        void ApplyHotKeyBindings(MainViewModel vm)
        {
            foreach (KeyBinding b in _dynamicKeyBindings)
                InputBindings.Remove(b);
            foreach (CommandBinding b in _dynamicCommandBindings)
                CommandBindings.Remove(b);
            _dynamicKeyBindings.Clear();
            _dynamicCommandBindings.Clear();

            HotKeyOptions options = vm.Configuration?.HotKeys;
            if (options == null)
                return;

            foreach (var item in options.Items)
            {
                KeyGesture gesture = HotKeyOptions.Parse(item.Key);
                ICommand command = vm.GetHotKeyCommand(item.Action);
                if (gesture == null || command == null)
                    continue;

                KeyBinding keyBinding = new KeyBinding(command, gesture);
                InputBindings.Add(keyBinding);
                _dynamicKeyBindings.Add(keyBinding);

                // CommandBinding без обработчиков — для ре-запроса CanExecute
                // (KeyBinding сам по себе CanExecute не обновляет).
                CommandBinding commandBinding = new CommandBinding(command);
                CommandBindings.Add(commandBinding);
                _dynamicCommandBindings.Add(commandBinding);
            }
        }
    }
}