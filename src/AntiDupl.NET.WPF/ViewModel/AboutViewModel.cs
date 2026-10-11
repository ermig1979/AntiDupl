using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Windows.Input;
using AntiDupl.NET.Core;
using AntiDupl.NET.Core.Original;
using AntiDupl.NET.WPF.Command;

namespace AntiDupl.NET.WPF.ViewModel
{
    /// <summary>
    /// Строка таблицы «Компонент | Версия» окна "О программе".
    /// </summary>
    public class AboutComponentRow
    {
        public string Name { get; set; }
        /// <summary>URL ссылки на проект компонента (null — без ссылки).</summary>
        public string Url { get; set; }
        public string Version { get; set; }
    }

    /// <summary>
    /// "О программе" (B1 — полный зеркал WF AboutProgramPanel):
    /// логотип + ссылка на сайт, строки copyright, таблица «Компонент | Версия»
    /// (AntiDupl/Simd/OpenJPEG/libwebp/libjpeg-turbo/libheif/libavif/libjxl,
    /// версии — CoreLib.GetVersion), ОК.
    /// </summary>
    public class AboutViewModel : CloseableViewModel
    {
        readonly CoreLib _core;

        /// <summary>URL сайта проекта (зеркало WF WebLinks.GithubComAntidupl).</summary>
        public const string SiteUrl = "http://ermig1979.github.io/AntiDupl";

        /// <summary>Название продукта (логотип-строка, зеркало WF Application.ProductName).</summary>
        public string ProductName { get { return "AntiDupl"; } }

        /// <summary>URL сайта для логотип-ссылки.</summary>
        public string Site { get { return SiteUrl; } }

        public ObservableCollection<AboutComponentRow> Rows { get; private set; }

        ICommand _openUrlCommand;
        /// <summary>Открывает URL (ссылки в таблице и логотипе). Параметр — Url строки.</summary>
        public ICommand OpenUrlCommand
        {
            get
            {
                return _openUrlCommand ?? (_openUrlCommand = new RelayCommand(
                    arg => OpenUrl(arg as string),
                    arg => !String.IsNullOrEmpty(arg as string)));
            }
        }

        public AboutViewModel(CoreLib core)
        {
            _core = core;
            Rows = BuildRows();
        }

        /// <summary>
        /// Таблица компонентов (порядок и URL — зеркало WF AboutProgramPanel.CreateInfoTable
        /// и WebLinks). Версии — из ядра (CoreLib.GetVersion).
        /// </summary>
        ObservableCollection<AboutComponentRow> BuildRows()
        {
            Rows = new ObservableCollection<AboutComponentRow>
            {
                new AboutComponentRow { Name = ProductName, Url = SiteUrl, Version = VersionOf(CoreDll.VersionType.AntiDupl) },
                new AboutComponentRow { Name = "Simd",          Url = "http://ermig1979.github.io/Simd",                    Version = VersionOf(CoreDll.VersionType.Simd) },
                new AboutComponentRow { Name = "OpenJPEG",      Url = "http://www.openjpeg.org",                             Version = VersionOf(CoreDll.VersionType.OpenJpeg) },
                new AboutComponentRow { Name = "libwebp",       Url = "https://github.com/webmproject/libwebp",              Version = VersionOf(CoreDll.VersionType.Webp) },
                new AboutComponentRow { Name = "libjpeg-turbo", Url = "http://www.libjpeg-turbo.org",                        Version = VersionOf(CoreDll.VersionType.TurboJpeg) },
                new AboutComponentRow { Name = "libheif",       Url = "http://www.libheif.org",                              Version = VersionOf(CoreDll.VersionType.Heif) },
                new AboutComponentRow { Name = "libavif",       Url = "https://github.com/AOMediaCodec/libavif",             Version = VersionOf(CoreDll.VersionType.Avif) },
                new AboutComponentRow { Name = "libjxl",        Url = "https://github.com/libjxl/libjxl",                    Version = VersionOf(CoreDll.VersionType.Jxl) },
            };
            return Rows;
        }

        string VersionOf(CoreDll.VersionType type)
        {
            try
            {
                CoreVersion version = _core != null ? _core.GetVersion(type) : null;
                return version != null ? version.ToString() : "n/a";
            }
            catch
            {
                return "n/a";
            }
        }

        static void OpenUrl(string url)
        {
            Process.Start(new ProcessStartInfo(url) { UseShellExecute = true });
        }
    }
}