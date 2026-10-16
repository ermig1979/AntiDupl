using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using AntiDupl.NET.WPF.Model;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.View
{
    /// <summary>
    /// Окно выбора горячих клавиш (зеркало WF SelectHotKeysForm).
    /// Захват клавиш — через PreviewKeyDown TextBox'ов (Tag = строка-VM).
    /// </summary>
    public partial class SelectHotKeysWindow
    {
        public SelectHotKeysWindow()
        {
            InitializeComponent();
        }

        void OnKeyBoxPreviewKeyDown(object sender, KeyEventArgs e)
        {
            e.Handled = true;
            var textBox = sender as TextBox;
            if (textBox == null)
                return;
            var row = textBox.Tag as HotKeyRowViewModel;
            if (row == null)
                return;

            if (e.Key == Key.System)
            {
                // Numpad-клавиши приходят как Key.System + SystemKey.
                var key = KeyInterop.KeyFromVirtualKey(KeyInterop.VirtualKeyFromKey(e.SystemKey));
                if (key != Key.None)
                    SetKey(row, key, Keyboard.Modifiers);
                return;
            }

            if (e.Key == Key.Escape)
            {
                // Сброс привязки.
                row.Item.Key = "";
                row.UpdateState();
                (DataContext as SelectHotKeysViewModel)?.RefreshRowStates();
                return;
            }

            if (e.Key == Key.None)
                return;

            SetKey(row, e.Key, Keyboard.Modifiers);
        }

        void SetKey(HotKeyRowViewModel row, Key key, ModifierKeys modifiers)
        {
            row.Item.Key = HotKeyOptions.ToDisplayString(new KeyGesture(key, modifiers));
            row.UpdateState();
            (DataContext as SelectHotKeysViewModel)?.RefreshRowStates();
        }

        void OnKeyBoxGotFocus(object sender, RoutedEventArgs e)
        {
            // Фокус на TextBox готовит его к вводу горячей клавиши.
        }

        void OnSetDefaultClick(object sender, RoutedEventArgs e)
        {
            (DataContext as SelectHotKeysViewModel)?.SetDefault();
        }
    }
}