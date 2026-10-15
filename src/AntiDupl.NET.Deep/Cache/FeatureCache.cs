using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Text;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Cache;

/// <summary>
/// Управляемый бинарный кэш (замена SQLite-файла референс-проекта):
/// эмбеддинги, вердикты верификации и пер-файловая статистика живут в памяти и
/// периодически пишутся в бинарные файлы рядом с программой (&lt;BaseDirectory&gt;\deep):
/// <c>deep_embeddings.dat</c>, <c>deep_wgc.dat</c>, <c>deep_filestats.dat</c>.
/// Временные/рабочие данные (эмбеддинги, графы, PQ-таблицы, tmp-файлы) — только в
/// оперативной памяти; %TEMP% нигде не используется.
///
/// Формат файла (little-endian):
///   magic (4 байта) | version (int32) | count (int32)
///   записи:
///     embeddings: pathLen (int32) | path (utf8) | size (int64) | mtime (float64) |
///                 compressedLen (int32) | data (zlib над 768×float32, RFC 1950 — как в референсе)
///     wgc: path1Len | path1 | mtime1 (float64) | path2Len | path2 | mtime2 (float64) |
///          minScore (float32) | result (u8) | angle (float32) | scale (float32) |
///          angleVotes (int32) | scaleVotes (int32)
///     filestats: pathLen | path | width (int32) | height (int32) |
///                blurring (float64) | blockiness (float64)
/// WGC-записи канонизованы: path1 &lt; path2 (ordinal, без учёта регистра).
/// minScore — порог «minimum average LightGlue match score» на момент вычисления: вердикт
/// зависит от него (PASS ⇔ … && avgScore ≥ minScore), поэтому порог входит в ключ записи
/// — смена порога в настройках не возвращает старые вердикты из кэша.
/// filestats — пер-файловая статистика (нативное разрешение, blurring/blockiness),
/// побочный продукт декодирования ядром; сохраняется, чтобы при повторном поиске
/// (декоды из кэша, ядро не вызывается) результаты всё равно несли эти поля.
/// </summary>
public sealed class FeatureCache : IFeatureCache
{
    // Магия: v3 — формат финальный (эмбеддинги 768×f32; WGC-ключ включает порог
    // minMatchScore; файл filestats вынесен отдельно). Релизов прежних версий не было,
    // чтение старых файлов не поддерживается: другой magic = кэш игнорируется (пересчёт).
    private const int MagicEmbeddings = 0x44504443; // "ADDP"
    private const int MagicWgc = 0x57504443;        // "ADPW"
    private const int MagicFileStats = 0x41444653;  // "ADFS"
    private const int FormatVersion = 1;
    private const double MtimeTolerance = 0.01; // секунды, как в референсе

    private sealed class EmbeddingEntry
    {
        public ulong Size;
        public double Mtime;
        public float[] Embedding = Array.Empty<float>();
    }

    private sealed class WgcEntry
    {
        public double Mtime1;
        public double Mtime2;
        public bool Result;
        public float Angle;
        public float Scale;
        public int AngleVotes;
        public int ScaleVotes;
    }

    private readonly string _directory;
    private readonly string _embeddingsPath;
    private readonly string _wgcPath;
    private readonly string _fileStatsPath;
    private readonly object _gate = new();
    private readonly Dictionary<string, EmbeddingEntry> _embeddings = new(StringComparer.OrdinalIgnoreCase);
    private readonly Dictionary<(string P1, string P2, float MinScore), WgcEntry> _wgc = new();
    private readonly Dictionary<string, DeepFileStats> _fileStats = new(StringComparer.OrdinalIgnoreCase);
    private bool _dirty;

    public FeatureCache(string directory)
    {
        _directory = directory ?? throw new ArgumentNullException(nameof(directory));
        Directory.CreateDirectory(_directory);
        _embeddingsPath = Path.Combine(_directory, "deep_embeddings.dat");
        _wgcPath = Path.Combine(_directory, "deep_wgc.dat");
        _fileStatsPath = Path.Combine(_directory, "deep_filestats.dat");

        if (File.Exists(_embeddingsPath))
            LoadEmbeddings();
        if (File.Exists(_wgcPath))
            LoadWgc();
        if (File.Exists(_fileStatsPath))
            LoadFileStats();
    }

    // ---------------- Embeddings ----------------

    public (double mtime, float[] embedding)? GetEmbedding(string path)
    {
        lock (_gate)
        {
            if (!_embeddings.TryGetValue(path, out EmbeddingEntry? entry))
                return null;

            // Невалидация: файл должен существовать, size и mtime — совпадать.
            try
            {
                var info = new FileInfo(path);
                if (!info.Exists) return null;
                if ((ulong)info.Length != entry.Size) return null;

                double currentMtime = File.GetLastWriteTimeUtc(path).Ticks / (double)TimeSpan.TicksPerSecond;
                if (Math.Abs(currentMtime - entry.Mtime) > MtimeTolerance) return null;
            }
            catch
            {
                return null;
            }

            return (entry.Mtime, entry.Embedding);
        }
    }

    public void SetEmbedding(string path, double mtime, float[] embedding)
    {
        Debug.Assert(!string.IsNullOrEmpty(path), nameof(path));
        // Нулевой/отсутствующий эмбеддинг ломает zlib-контракт файла кэша.
        Debug.Assert(embedding is not null && embedding.Length > 0, "embedding must be non-empty");
        lock (_gate)
        {
            ulong size = 0;
            try { size = (ulong)new FileInfo(path).Length; } catch { }

            var entry = new EmbeddingEntry { Size = size, Mtime = mtime, Embedding = embedding };
            _embeddings[path] = entry;
            _dirty = true;
        }
    }

    // ---------------- WGC ----------------

    public (bool result, float angle, float scale, int angleVotes, int scaleVotes)? GetWgc(
        string path1, string path2, double mtime1, double mtime2, float minMatchScore)
    {
        (string p1, string p2, double t1, double t2) = Canonicalize(path1, path2, mtime1, mtime2);
        lock (_gate)
        {
            if (!_wgc.TryGetValue((p1, p2, minMatchScore), out WgcEntry? entry))
                return null;

            if (Math.Abs(entry.Mtime1 - t1) > MtimeTolerance || Math.Abs(entry.Mtime2 - t2) > MtimeTolerance)
                return null;

            return (entry.Result, entry.Angle, entry.Scale, entry.AngleVotes, entry.ScaleVotes);
        }
    }

    public void SetWgc(string path1, string path2, double mtime1, double mtime2, float minMatchScore,
        bool result, float angle, float scale, int angleVotes, int scaleVotes)
    {
        // Пустой путь в канон-ключе засоряет кэш вердиктов.
        Debug.Assert(!string.IsNullOrEmpty(path1) && !string.IsNullOrEmpty(path2), "pair paths must be set");
        (string p1, string p2, double t1, double t2) = Canonicalize(path1, path2, mtime1, mtime2);
        lock (_gate)
        {
            _wgc[(p1, p2, minMatchScore)] = new WgcEntry
            {
                Mtime1 = t1,
                Mtime2 = t2,
                Result = result,
                Angle = angle,
                Scale = scale,
                AngleVotes = angleVotes,
                ScaleVotes = scaleVotes
            };
            _dirty = true;
        }
    }

    // ---------------- File stats ----------------

    public void SaveFileStats(IReadOnlyDictionary<string, DeepFileStats> stats)
    {
        lock (_gate)
        {
            _fileStats.Clear();
            foreach (var kv in stats)
                _fileStats[kv.Key] = kv.Value;
            WriteFileStats();
        }
    }

    public bool TryLoadFileStats(out IReadOnlyDictionary<string, DeepFileStats> stats)
    {
        lock (_gate)
        {
            if (_fileStats.Count == 0)
            {
                stats = new Dictionary<string, DeepFileStats>();
                return false;
            }
            stats = new Dictionary<string, DeepFileStats>(_fileStats, StringComparer.OrdinalIgnoreCase);
            return true;
        }
    }

    // ---------------- Service ----------------

    public long ClearAll()
    {
        long freed = 0;
        lock (_gate)
        {
            freed += File.Exists(_embeddingsPath) ? new FileInfo(_embeddingsPath).Length : 0;
            freed += File.Exists(_wgcPath) ? new FileInfo(_wgcPath).Length : 0;
            freed += File.Exists(_fileStatsPath) ? new FileInfo(_fileStatsPath).Length : 0;

            _embeddings.Clear();
            _wgc.Clear();
            _fileStats.Clear();

            WriteEmbeddings();
            WriteWgc();
            WriteFileStats();
            _dirty = false;
        }
        return freed;
    }

    public long GetEmbeddingCount()
    {
        lock (_gate) { return _embeddings.Count; }
    }

    public void Save()
    {
        lock (_gate)
        {
            if (!_dirty) return;
            WriteEmbeddings();
            WriteWgc();
            _dirty = false;
        }
    }

    public void Dispose()
    {
        Save();
    }

    private static (string P1, string P2, double T1, double T2) Canonicalize(
        string path1, string path2, double mtime1, double mtime2)
    {
        return string.CompareOrdinal(
                path1.ToLowerInvariant(), path2.ToLowerInvariant()) < 0
            ? (path1, path2, mtime1, mtime2)
            : (path2, path1, mtime2, mtime1);
    }

    // ---------------- Persistence ----------------

    private void WriteEmbeddings()
    {
        var tmp = _embeddingsPath + ".tmp";
        // Windows: File.Move нельзя выполнять, пока FileStream на tmp-файле открыт —
        // закрываем его до замены.
        {
            using var fs = new FileStream(tmp, FileMode.Create, FileAccess.Write);
            using var bw = new BinaryWriter(fs, Encoding.UTF8);
            bw.Write(MagicEmbeddings);
            bw.Write(FormatVersion);
            bw.Write(_embeddings.Count);
            foreach (var kv in _embeddings)
            {
                WritePath(bw, kv.Key);
                bw.Write((long)kv.Value.Size);
                bw.Write(kv.Value.Mtime);
                var bytes = new byte[kv.Value.Embedding.Length * 4];
                Buffer.BlockCopy(kv.Value.Embedding, 0, bytes, 0, bytes.Length);
                var compressed = ZlibCompress(bytes);
                bw.Write(compressed.Length);
                bw.Write(compressed);
            }
            fs.Flush(true);
        }
        File.Move(tmp, _embeddingsPath, overwrite: true);
    }

    private void LoadEmbeddings()
    {
        try
        {
            using var fs = new FileStream(_embeddingsPath, FileMode.Open, FileAccess.Read);
            using var br = new BinaryReader(fs, Encoding.UTF8);
            if (br.ReadInt32() != MagicEmbeddings) return;
            if (br.ReadInt32() != FormatVersion) return;
            int count = br.ReadInt32();
            for (int i = 0; i < count; i++)
            {
                string? path = ReadPath(br);
                if (path == null) return;
                ulong size = (ulong)br.ReadInt64();
                double mtime = br.ReadDouble();
                int compressedLen = br.ReadInt32();
                byte[] compressed = br.ReadBytes(compressedLen);
                byte[] bytes = ZlibDecompress(compressed);
                var embedding = new float[bytes.Length / 4];
                Buffer.BlockCopy(bytes, 0, embedding, 0, bytes.Length);

                _embeddings[path] = new EmbeddingEntry { Size = size, Mtime = mtime, Embedding = embedding };
            }
        }
        catch
        {
            // Повреждённый файл — начинаем с пустого кэша (пересчитаем эмбеддинги).
            _embeddings.Clear();
        }
    }

    private void WriteWgc()
    {
        var tmp = _wgcPath + ".tmp";
        {
            using var fs = new FileStream(tmp, FileMode.Create, FileAccess.Write);
            using var bw = new BinaryWriter(fs, Encoding.UTF8);
            bw.Write(MagicWgc);
            bw.Write(FormatVersion);
            bw.Write(_wgc.Count);
            foreach (var kv in _wgc)
            {
                WritePath(bw, kv.Key.P1);
                bw.Write(kv.Value.Mtime1);
                WritePath(bw, kv.Key.P2);
                bw.Write(kv.Value.Mtime2);
                bw.Write(kv.Key.MinScore);
                bw.Write((byte)(kv.Value.Result ? 1 : 0));
                bw.Write(kv.Value.Angle);
                bw.Write(kv.Value.Scale);
                bw.Write(kv.Value.AngleVotes);
                bw.Write(kv.Value.ScaleVotes);
            }
            fs.Flush(true);
        }
        File.Move(tmp, _wgcPath, overwrite: true);
    }

    private void LoadWgc()
    {
        try
        {
            using var fs = new FileStream(_wgcPath, FileMode.Open, FileAccess.Read);
            using var br = new BinaryReader(fs, Encoding.UTF8);
            if (br.ReadInt32() != MagicWgc) return;
            if (br.ReadInt32() != FormatVersion) return;
            int count = br.ReadInt32();
            for (int i = 0; i < count; i++)
            {
                string? p1 = ReadPath(br);
                if (p1 == null) return;
                double m1 = br.ReadDouble();
                string? p2 = ReadPath(br);
                if (p2 == null) return;
                double m2 = br.ReadDouble();
                float minScore = br.ReadSingle();
                bool result = br.ReadByte() != 0;
                float angle = br.ReadSingle();
                float scale = br.ReadSingle();
                int angleVotes = br.ReadInt32();
                int scaleVotes = br.ReadInt32();

                _wgc[(p1, p2, minScore)] = new WgcEntry
                {
                    Mtime1 = m1, Mtime2 = m2, Result = result,
                    Angle = angle, Scale = scale, AngleVotes = angleVotes, ScaleVotes = scaleVotes
                };
            }
        }
        catch
        {
            _wgc.Clear();
        }
    }

    private void WriteFileStats()
    {
        var tmp = _fileStatsPath + ".tmp";
        {
            using var fs = new FileStream(tmp, FileMode.Create, FileAccess.Write);
            using var bw = new BinaryWriter(fs, Encoding.UTF8);
            bw.Write(MagicFileStats);
            bw.Write(FormatVersion);
            bw.Write(_fileStats.Count);
            foreach (var kv in _fileStats)
            {
                WritePath(bw, kv.Key);
                bw.Write(kv.Value.Width);
                bw.Write(kv.Value.Height);
                bw.Write(kv.Value.Blurring);
                bw.Write(kv.Value.Blockiness);
            }
            fs.Flush(true);
        }
        File.Move(tmp, _fileStatsPath, overwrite: true);
    }

    private void LoadFileStats()
    {
        try
        {
            using var fs = new FileStream(_fileStatsPath, FileMode.Open, FileAccess.Read);
            using var br = new BinaryReader(fs, Encoding.UTF8);
            if (br.ReadInt32() != MagicFileStats) return;
            if (br.ReadInt32() != FormatVersion) return;
            int count = br.ReadInt32();
            for (int i = 0; i < count; i++)
            {
                string? path = ReadPath(br);
                if (path == null) return;
                int width = br.ReadInt32();
                int height = br.ReadInt32();
                double blurring = br.ReadDouble();
                double blockiness = br.ReadDouble();

                _fileStats[path] = new DeepFileStats
                {
                    Width = width,
                    Height = height,
                    Blurring = blurring,
                    Blockiness = blockiness
                };
            }
        }
        catch
        {
            // Повреждённый файл — начинаем с пустой статистики (пересоберётся при декодах).
            _fileStats.Clear();
        }
    }

    private static void WritePath(BinaryWriter bw, string path)
    {
        Debug.Assert(!string.IsNullOrEmpty(path), "cache path must be set");
        byte[] bytes = Encoding.UTF8.GetBytes(path);
        if (bytes.Length > int.MaxValue) throw new InvalidDataException("path too long");
        bw.Write(bytes.Length);
        bw.Write(bytes);
    }

    private static string? ReadPath(BinaryReader br)
    {
        int len = br.ReadInt32();
        if (len < 0 || len > 32768) return null;
        return Encoding.UTF8.GetString(br.ReadBytes(len));
    }

    private static byte[] ZlibCompress(byte[] data)
    {
        using var output = new MemoryStream();
        using (var zlib = new ZLibStream(output, CompressionLevel.Optimal))
            zlib.Write(data, 0, data.Length);
        return output.ToArray();
    }

    private static byte[] ZlibDecompress(byte[] data)
    {
        using var input = new MemoryStream(data);
        using var zlib = new ZLibStream(input, CompressionMode.Decompress);
        using var output = new MemoryStream();
        zlib.CopyTo(output);
        return output.ToArray();
    }
}