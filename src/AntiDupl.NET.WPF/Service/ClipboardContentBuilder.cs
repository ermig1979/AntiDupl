using System.Collections.Generic;
using System.Linq;
using System.Text;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Service
{
    /// <summary>
    /// Формирование текста для буфера обмена. Чистая статическая логика:
    /// не читает DataGrid, работает только с POCO-парами.
    /// Формат — TSV (зеркало WF ClipboardContentBuilder): по две строки на пару
    /// (первый файл / второй файл) + пустая строка между парами.
    /// </summary>
    public static class ClipboardContentBuilder
    {
        /// <summary>
        /// Полный TSV-контент: пути, размер, разрешение, blockiness, bluring,
        /// jpeg-peaks, utility index, difference, transform.
        /// </summary>
        public static string Build(IEnumerable<DuplPairViewModel> pairs)
        {
            StringBuilder sb = new StringBuilder();
            if (pairs == null)
                return sb.ToString();

            foreach (var pair in pairs)
            {
                if (pair == null)
                    continue;
                AppendFileLine(sb, pair.FirstFile);
                sb.Append('\t').Append(pair.Difference.ToString("F3"));
                sb.Append('\t').Append(pair.Transform);
                sb.AppendLine();
                AppendFileLine(sb, pair.SecondFile);
                sb.AppendLine();
                sb.AppendLine();
            }
            return sb.ToString();
        }

        /// <summary>Только пути: по две строки на пару (первый/второй файл) + пустая строка.</summary>
        public static string BuildPaths(IEnumerable<DuplPairViewModel> pairs)
        {
            StringBuilder sb = new StringBuilder();
            if (pairs == null)
                return sb.ToString();

            foreach (var pair in pairs)
            {
                if (pair == null)
                    continue;
                sb.AppendLine(pair.FirstFile?.Path);
                sb.AppendLine(pair.SecondFile?.Path);
                sb.AppendLine();
            }
            return sb.ToString();
        }

        static void AppendFileLine(StringBuilder sb, ObjectModel.ImageInfoClass file)
        {
            if (file == null)
            {
                sb.Append('\t');
                return;
            }
            sb.Append(file.Path);
            sb.Append('\t').Append(file.FileSize);
            sb.Append('\t').Append(file.Width).Append('x').Append(file.Height);
            sb.Append('\t').Append(file.Blockiness);
            sb.Append('\t').Append(file.Bluring);
            sb.Append('\t').Append(file.JpegPeaks);
            sb.Append('\t').Append(file.UtilityIndex);
        }
    }
}