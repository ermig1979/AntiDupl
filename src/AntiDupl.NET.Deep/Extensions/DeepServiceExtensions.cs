using AntiDupl.NET.Core;
using AntiDupl.NET.Deep.Cache;
using AntiDupl.NET.Deep.Core;
using AntiDupl.NET.Deep.Cv;
using AntiDupl.NET.Deep.Image;
using AntiDupl.NET.Deep.Infra;
using AntiDupl.NET.Deep.Logging;
using AntiDupl.NET.Deep.Models;
using AntiDupl.NET.Onnx.Cv;
using AntiDupl.NET.Onnx.Ml;
using Microsoft.Extensions.DependencyInjection;

namespace AntiDupl.NET.Deep.Extensions;

/// <summary>
/// DI-регистрация deep-пайплайна (аналог ServiceCollectionExtension референс-проекта).
/// Вызывается хостом (WPF App.xaml.cs). Пути к каталогам моделей/кэша/логов передаёт хост
/// (WPF: &lt;UserPath&gt;\models и &lt;BaseDirectory&gt;\deep, общий &lt;BaseDirectory&gt;\logs).
/// ONNX-раннеры регистрируются из сборки AntiDupl.NET.Onnx (единственный носитель ORT-пакета).
/// </summary>
public static class DeepServiceExtensions
{
    /// <param name="resolvePaths">
    /// () → (modelsDirectory, cacheDirectory, logDirectory). Вызывается при создании
    /// FeatureCache и логгера.
    /// </param>
    /// <param name="onGpuFallback">
    /// Необязательный хук хоста: вызывается, когда GPU (CUDA) недоступен и поиск
    /// идёт на CPU (WPF показывает MessageBox) — для ОБЕИХ моделей (DINOv2 и LightGlue).
    /// Если null — только лог/прогресс.
    /// </param>
    public static IServiceCollection AddDeepSearch(
        this IServiceCollection services,
        Func<(string ModelsDirectory, string CacheDirectory, string LogDirectory)> resolvePaths,
        Action<string>? onGpuFallback = null)
    {
        // Файловый логгер deep-пайплайна (порт IDeepLogger из Abstractions).
        services.AddSingleton<IDeepLogger>(_ =>
        {
            var (_, _, logDirectory) = resolvePaths();
            var logger = new FileDeepLogger(logDirectory);
            logger.Clear(); // новый запуск — чистый лог
            return logger;
        });

        // Источник растров: по умолчанию — «core» (CoreBitmapImageSource): 224×224 квадрат для DINOv2
        // и латербокс-канвас 800×600 для LightGlue (adLoadBitmapLetterboxW; на старых ядрах без этого
        // экспорта — stretch) декодируются в ядре, OpenCV/WIC в конвейере нет.
        // A/B-замер (TestCropHard, 13 GT-пар, core 2.4.0 + letterbox): «core» и эталонный «ref»
        // (WIC + OpenCV-совместимый ресайз) эквивалентны — 13/13 GT-пар и 1 мягкое кросс-сценовное FP
        // (Sicario-2667419↔Sicario-2676063, sim≈0.67) на всех порогах. Letterbox (сохранение пропорций)
        // улучшил WGC-геометрию «core»: пары, пропущенные при stretch (Sicario-2658717,
        // Avengers(1)↔(2)/(3)), теперь находятся. ReferenceImagePipeline остаётся доступным как
        // контроль паритета препроцессинга.
        services.AddSingleton<IDeepImageSource>(sp =>
            new CoreBitmapImageSource(sp.GetRequiredService<CoreLib>()));

        // Источник кандидатов: адаптер на ядро (use case видит только порт IDeepCandidateSource).
        services.AddSingleton<IDeepCandidateSource>(sp =>
            new CoreLibCandidateSource(sp.GetRequiredService<CoreLib>()));

        services.AddSingleton<IFeatureCache>(sp =>
        {
            var (modelsDirectory, cacheDirectory, _) = resolvePaths();
            return new FeatureCache(cacheDirectory);
        });

        // Верификатор: фабрика с ленивой логикой создания по пути модели (синглтон без
        // параметризации: MinMatchScore — параметр запуска, сервис передаёт его в Verify).
        // Сессия LightGlue — через OnnxSessionFactory (GPU→CPU fallback + хук).
        services.AddSingleton<GeometricVerifierFactory>(sp =>
        {
            var logger = sp.GetRequiredService<IDeepLogger>();
            return new GeometricVerifierFactory(path =>
                new LightGlueGeometricVerifier(
                    new SuperPointLightGluePipeline(path, logger, onGpuFallback)));
        });

        services.AddSingleton<DeepSearchService>(sp =>
        {
            var logger = sp.GetRequiredService<IDeepLogger>();
            return new DeepSearchService(
                sp.GetRequiredService<IDeepCandidateSource>(),
                sp.GetRequiredService<IFeatureCache>(),
                modelPath => new EmbeddingExtractor(
                    modelPath,
                    sp.GetRequiredService<IFeatureCache>(),
                    sp.GetRequiredService<IDeepImageSource>(),
                    logger,
                    onGpuFallback),
                sp.GetRequiredService<GeometricVerifierFactory>(),
                sp.GetRequiredService<IDeepImageSource>(),
                resolvePaths().ModelsDirectory,
                logger);
        });

        return services;
    }
}