using System.Windows;

namespace AntiDupl.NET.WPF.View
{
    /// <summary>
    /// Окно "О программе" (зеркало WF AboutProgramPanel).
    /// DataContext подставляется IWindowService-обёрткой (AboutViewModel).
    /// </summary>
    public partial class AboutWindow : Window
    {
        public AboutWindow()
        {
            InitializeComponent();
        }
    }
}