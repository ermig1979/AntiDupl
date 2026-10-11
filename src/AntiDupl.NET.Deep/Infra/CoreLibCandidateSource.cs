using AntiDupl.NET.Core;
using AntiDupl.NET.Deep.Models;

namespace AntiDupl.NET.Deep.Infra;

/// <summary>
/// Адаптер порта <see cref="IDeepCandidateSource"/> на ядро AntiDupl.dll:
/// <c>CoreLib.ScanCandidates()</c> (adScanCandidatesW) прогоняет фильтры хоста
/// (пути/подпапки, скрытые/системные, ignore/valid, форматы, размер) и возвращает
/// path/size/time. Use case видит только <see cref="DeepCandidate"/> (путь).
/// </summary>
public sealed class CoreLibCandidateSource : IDeepCandidateSource
{
    private readonly CoreLib _core;

    public CoreLibCandidateSource(CoreLib core)
    {
        _core = core ?? throw new ArgumentNullException(nameof(core));
    }

    public IReadOnlyList<DeepCandidate> Scan()
    {
        var candidates = _core.ScanCandidates();
        if (candidates == null || candidates.Length == 0)
            return Array.Empty<DeepCandidate>();

        var result = new DeepCandidate[candidates.Length];
        for (int i = 0; i < candidates.Length; i++)
            result[i] = new DeepCandidate(candidates[i].path, (long)candidates[i].size, (long)candidates[i].time);
        return result;
    }
}