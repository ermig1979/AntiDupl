using System.Diagnostics;
using Microsoft.ML.OnnxRuntime;
using AntiDupl.NET.Deep.Logging;

namespace AntiDupl.NET.Onnx;

/// <summary>
/// Единая точка создания ORT-сессий deep-пайплайна (DINOv2 и SuperPoint+LightGlue).
///
/// Надёжный паттерн (унифицирован):
/// в разных версиях ORT отсутствие CUDA/cuDNN бросает либо на
/// <c>AppendExecutionProvider_CUDA</c>, либо на самом <c>new InferenceSession</c>
/// (в 1.21 — второе), поэтому в try-блок включено и создание сессии.
/// Fallback на CPU + опциональное явное уведомление хоста (<c>onGpuFallback</c>).
/// </summary>
public static class OnnxSessionFactory
{
    public static OnnxSession CreateSession(
        string modelPath,
        IDeepLogger logger,
        Action<string>? onGpuFallback = null)
    {
        Debug.Assert(!string.IsNullOrWhiteSpace(modelPath), "model path must be set before session creation");
        // Режим детерминизации (тесты): DEEP_FORCE_CPU=1 → сессии сразу на CPU
        // (GPU-инференс ORT не детерминирован: CUDA ScatterND-атомарные, порядок
        // редукций — WGC-голоса «плавают», флейк-тесты).
        if (Environment.GetEnvironmentVariable("DEEP_FORCE_CPU") == "1")
        {
            var cpuOptions = new SessionOptions
            {
                GraphOptimizationLevel = GraphOptimizationLevel.ORT_ENABLE_ALL
            };
            logger.Write("[MODEL] DEEP_FORCE_CPU=1: CPU-only session");
            return new OnnxSession(new InferenceSession(modelPath, cpuOptions), isGpu: false);
        }

        try
        {
            var gpuOptions = new SessionOptions
            {
                GraphOptimizationLevel = GraphOptimizationLevel.ORT_ENABLE_ALL
            };
            gpuOptions.AppendExecutionProvider_CUDA();
            return new OnnxSession(new InferenceSession(modelPath, gpuOptions), isGpu: true);
        }
        catch (Exception ex)
        {
            logger.Write($"[MODEL] CUDA initialization failed: {ex.Message}");
            onGpuFallback?.Invoke(
                "GPU недоступен: ONNX Runtime не нашёл CUDA/cuDNN.\n"
                + "Поиск выполняется на CPU.\n\n"
                + "Для использования GPU укажите каталоги bin CUDA и cuDNN\n"
                + "в Configuration → Deep (поля CudaPath / CudnnPath)\n"
                + "и перезапустите приложение.");
            var cpuOptions = new SessionOptions
            {
                GraphOptimizationLevel = GraphOptimizationLevel.ORT_ENABLE_ALL
            };
            return new OnnxSession(new InferenceSession(modelPath, cpuOptions), isGpu: false);
        }
    }
}

/// <summary>
/// Обёртка над <see cref="InferenceSession"/>: сессия + флаг провайдера.
/// ORT-сессия (особенно с CUDA EP) не потокобезопасна — раннеры сериализуют
/// доступ своими замками (как до рефакторинга).
/// </summary>
public sealed class OnnxSession
{
    public OnnxSession(InferenceSession session, bool isGpu)
    {
        Debug.Assert(session is not null, nameof(session));
        Session = session;
        IsGpu = isGpu;
    }

    public InferenceSession Session { get; }

    /// <summary>True, если сессия создана с CUDA execution provider.</summary>
    public bool IsGpu { get; }

    /// <summary>Размер измерения выхода модели (0, если выход/измерение не найдено).</summary>
    public int GetOutputDim(string outputName, int dimIndex)
        => OnnxSession.GetOutputDim(Session, outputName, dimIndex);

    public static int GetOutputDim(InferenceSession session, string outputName, int dimIndex)
    {
        var meta = session.OutputMetadata.FirstOrDefault(kv => kv.Key == outputName);
        if (meta.Key == null || dimIndex >= meta.Value.Dimensions.Length)
            return 0;
        return meta.Value.Dimensions[dimIndex];
    }

    public void Dispose() => Session.Dispose();
}