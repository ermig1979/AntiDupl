using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.Windows.Media;
using AntiDupl.NET.Core.Enums;
using AntiDupl.NET.WPF.ObjectModel;

namespace AntiDupl.NET.WPF.Model
{
    public interface IConfigurationModel : INotifyPropertyChanged
    {
        int ThumbnailWidth { get; set; }

        ObservableCollection<Criterion> Criterions { get; set; }
        SerializableCultureInfo Language { get; set; }

        // ==== Deep (DINOv2) search: configuration of the AntiDupl.NET.Deep pipeline ====

        /// <summary>Verify deep (DINOv2) pairs with the SuperPoint+LightGlue pipeline (geometric check).</summary>
        bool VerifyByLightGlue { get; set; }

        /// <summary>Path to the superpoint_lightglue_pipeline.onnx model (empty = default &lt;UserPath&gt;\models, auto-downloaded).</summary>
        string LightGlueModelPath { get; set; }

        /// <summary>Path to the dinov2-base.onnx model (empty = default &lt;UserPath&gt;\models; the file is not distributed officially, it is placed manually).</summary>
        string Dinov2ModelPath { get; set; }

        /// <summary>
        /// Optional URL to download dinov2-base.onnx from (the official file is not distributed;
        /// e.g. a link to your own server/archive). Empty = no auto-download at search start.
        /// </summary>
        string Dinov2ModelUrl { get; set; }

        /// <summary>
        /// Optional CUDA toolkit bin directory (cudart/cublas DLLs), e.g. C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.4\bin.
        /// Prepended to the process PATH at startup (as in the original app) so ONNX Runtime finds CUDA.
        /// Empty = not set. Requires app restart after change.
        /// </summary>
        string CudaPath { get; set; }

        /// <summary>
        /// Optional cuDNN bin directory (cudnn*.dll), e.g. C:\Program Files\NVIDIA\CUDNN\v9.23\bin\12.9\x64.
        /// Prepended to the process PATH at startup (as in the original app).
        /// Empty = not set. Requires app restart after change.
        /// </summary>
        string CudnnPath { get; set; }

        // ==== Deep pipeline parameters (parity with the reference SearchSettings) ====

        /// <summary>Minimum pair similarity (cosine) for a pair to join a verified group (0..1, default 0.5).</summary>
        double DeepMinSimilarityForPair { get; set; }

        /// <summary>Maximum cluster size; larger clusters are broken into singletons (default 50).</summary>
        int DeepMaxClusterSize { get; set; }

        /// <summary>Transitivity ratio (× distance threshold, 0..1, default 0.7): lower = stricter chain merging.</summary>
        double DeepTransitivityRatio { get; set; }

        /// <summary>WGC verification parallelism; 0 = auto (processor count / 2, as in the original).</summary>
        int DeepWgcParallelism { get; set; }

        /// <summary>Minimum average LightGlue match score (0..1, default 0.5).</summary>
        double DeepMinMatchScore { get; set; }

        // ==== Deep pipeline performance (parity with the reference SearchSettings) ====

        /// <summary>DINOv2 inference batch size (default 32; lower = less GPU memory).</summary>
        int DeepBatchSize { get; set; }

        /// <summary>Prefetch queue depth for decode/embedding batches (0 = off, default 2).</summary>
        int DeepPrefetchCount { get; set; }
        string LastResultFile { get; set; }

        /// <summary>Selected search mode (toolbar). null = not selected — default DINOv2 (when the core supports it).</summary>
        AlgorithmComparing? CompareAlgorithm { get; set; }
        uint UndoRedoStackSize { get; set; }
        uint MaxCachedThumbnail { get; set; }
        bool SaveResultInCurrentFile { get; set; }

        Color GoodColor { get; set; }
        Color BadColor { get; set; }
        Color AlternateColor { get; set; }
        Brush AlternateBrush { get; }

		bool LoadResultOnOpen { get; set; }
		bool SaveResultOnClose { get; set; }

        void Save();
        ObservableCollection<Criterion> GetDefaultCriterions();
    }
}
