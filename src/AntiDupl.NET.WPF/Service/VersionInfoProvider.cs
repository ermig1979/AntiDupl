using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using System.Reflection;
using System.Text.RegularExpressions;

namespace AntiDupl.NET.WPF.Service
{
    /// <summary>
    /// Версионная информация и ссылки (зеркало WF VersionInfo/AboutProgramPanel):
    /// сайт/справка — http://ermig1979.github.io/AntiDupl, версия ядра — CoreLib,
    /// проверка обновлений — version.xml на сайте.
    /// Конкретный класс без интерфейса — политика команды.
    /// </summary>
    public class VersionInfoProvider
    {
        /// <summary>База сайта (зеркало WF Resources.BaseURL).</summary>
        public const string BaseUrl = "http://ermig1979.github.io/AntiDupl";

        /// <summary>Текущая версия программы (версия сборки).</summary>
        public static string GetCurrentProgramVersion()
        {
            Version version = Assembly.GetExecutingAssembly().GetName().Version;
            return version != null ? version.ToString(3) : "0.0.0";
        }

        /// <summary>URL страницы справки на языке ru/en (зеркало WF Help-ссылок).</summary>
        public static string GetHelpUrl(bool russian)
        {
            return String.Format("{0}/data/help/{1}/index.html", BaseUrl, russian ? "russian" : "english");
        }

        /// <summary>
        /// Последняя версия с сайта (version.xml). null — оффлайн/ошибка/файл отсутствует.
        /// Фон + таймаут 15 c (HttpClient вместо WebClient): UI-поток не блокируется,
        /// все сетевые исключения (DNS/TLS/404/таймаут) дают null.
        /// </summary>
        public static async Task<string> GetLatestVersionFromWebAsync()
        {
            try
            {
                using (HttpClient client = new HttpClient { Timeout = TimeSpan.FromSeconds(15) })
                {
                    string content = await client.GetStringAsync(BaseUrl + "/version.xml").ConfigureAwait(false);
                    Match match = Regex.Match(content, @"<version>\s*([\d.]+)\s*</version>");
                    if (match.Success)
                        return match.Groups[1].Value;
                }
            }
            catch
            {
                // Офлайн/сетевая ошибка — трактуется как "версию получить не удалось".
            }
            return null;
        }
    }
}