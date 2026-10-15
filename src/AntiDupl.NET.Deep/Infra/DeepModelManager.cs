using System.Diagnostics;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Infra;

/// <summary>
/// Разрешение и загрузка ONNX-моделей.
///
/// dinov2-base.onnx — НЕ распространяется через релизы (лицензионные ограничения HF),
/// только путь: по умолчанию <c>&lt;modelsDir&gt;/dinov2-base.onnx</c>; если файла нет —
/// deep-поиск не запускается (сообщение в статус).
/// superpoint_lightglue_pipeline.onnx (фьюжн, ~51 МБ) — скачивается с GitHub Releases
/// (fabio-sim/LightGlue-ONNX v2.0), как в референс-проекте.
/// </summary>
public static class DeepModelManager
{
    public const string Dinov2ModelFileName = "dinov2-base.onnx";
    public const string LightGluePipelineFileName = "superpoint_lightglue_pipeline.onnx";
    public const string LightGluePipelineUrl =
        "https://github.com/fabio-sim/LightGlue-ONNX/releases/download/v2.0/superpoint_lightglue_pipeline.onnx";

    public static string GetModelsDirectory(string baseDirectory)
    {
        string dir = Path.Combine(baseDirectory, "models");
        Directory.CreateDirectory(dir);
        return dir;
    }

    public static string GetDefaultDinov2Path(string modelsDirectory)
        => Path.Combine(modelsDirectory, Dinov2ModelFileName);

    public static string GetDefaultLightGluePath(string modelsDirectory)
        => Path.Combine(modelsDirectory, LightGluePipelineFileName);

    /// <summary>
    /// Путь к модели DINOv2 (кастомный, если задан и существует; иначе дефолтный в models-каталоге).
    /// </summary>
    /// <returns>Путь, либо null, если модель не найдена.</returns>
    public static string? ResolveDinov2Path(string? customPath, string modelsDirectory)
    {
        if (!string.IsNullOrWhiteSpace(customPath) && File.Exists(customPath))
            return customPath;
        string defaultPath = GetDefaultDinov2Path(modelsDirectory);
        return File.Exists(defaultPath) ? defaultPath : null;
    }

    /// <summary>
    /// Путь к сжатой SuperPoint+LightGlue модели: кастомный (если существует) или дефолтный.
    /// </summary>
    public static string ResolveLightGluePath(string? customPath, string modelsDirectory)
    {
        if (!string.IsNullOrWhiteSpace(customPath) && File.Exists(customPath))
            return customPath;
        return GetDefaultLightGluePath(modelsDirectory);
    }

    /// <summary>
    /// Скачивает модель в <paramref name="destPath"/> (создаётся models-каталог).
    /// Прогресс — 0..100. Блок-вызов: выполнять из фонового потока.
    /// Отмена (<paramref name="ct"/>) прерывает запрос и чтение потока.
    /// </summary>
    public static void DownloadModel(string url, string destPath, IDeepLogger logger, IProgress<DeepProgress>? progress = null, CancellationToken ct = default)
    {
        Debug.Assert(!string.IsNullOrWhiteSpace(url) && !string.IsNullOrWhiteSpace(destPath),
            "url and destPath must be set");
        string directory = Path.GetDirectoryName(destPath) ?? "";
        if (directory.Length > 0)
            Directory.CreateDirectory(directory);

        progress?.Report(new DeepProgress(0, $"Downloading {Path.GetFileName(destPath)}..."));
        logger.Write($"[MODEL] Download: {url} -> {destPath}");

        using var http = new HttpClient { Timeout = TimeSpan.FromMinutes(30) };
        var request = new HttpRequestMessage(HttpMethod.Get, url);
        using HttpResponseMessage response = http.SendAsync(request, HttpCompletionOption.ResponseHeadersRead, ct).GetAwaiter().GetResult();
        response.EnsureSuccessStatusCode();

        long total = response.Content.Headers.ContentLength ?? -1;
        string tmpPath = destPath + ".download";
        using (var fileStream = new FileStream(tmpPath, FileMode.Create, FileAccess.Write))
        using (var contentStream = response.Content.ReadAsStream())
        {
            var buffer = new byte[81920];
            long read = 0;
            int readLen;
            int lastPct = -1;
            while ((readLen = contentStream.ReadAsync(buffer, 0, buffer.Length, ct).GetAwaiter().GetResult()) > 0)
            {
                fileStream.Write(buffer, 0, readLen);
                read += readLen;
                if (total > 0)
                {
                    int pct = (int)(read * 100 / total);
                    if (pct != lastPct)
                    {
                        lastPct = pct;
                        progress?.Report(new DeepProgress(pct,
                            $"Downloading {Path.GetFileName(destPath)}... {read / 1024 / 1024}/{total / 1024 / 1024} MB"));
                    }
                }
            }
        }

        if (File.Exists(destPath))
            File.Delete(destPath);
        File.Move(tmpPath, destPath);
        progress?.Report(new DeepProgress(100, $"Model ready: {destPath}"));
        logger.Write($"[MODEL] Download complete: {destPath} ({new FileInfo(destPath).Length} bytes)");
    }
}