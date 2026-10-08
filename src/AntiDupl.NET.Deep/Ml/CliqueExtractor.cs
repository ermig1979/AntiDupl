namespace AntiDupl.NET.Deep.Ml;

/// <summary>
/// Максимальные клики в неориентированном графе (Bron–Kerbosch с pivot).
/// Вынесен из DeepSearchService: чистая математика без зависимостей.
/// (HashSet-рекурсия вместо List-сравнений.)
/// </summary>
public static class CliqueExtractor
{
    /// <summary>Все максимальные клики графа (кластеры ≥2 вершин в результате).</summary>
    public static List<List<string>> FindCliques(Dictionary<string, HashSet<string>> graph)
    {
        var result = new List<List<string>>();
        var candidates = new HashSet<string>(graph.Keys, StringComparer.OrdinalIgnoreCase);
        var excluded = new HashSet<string>(StringComparer.OrdinalIgnoreCase);
        BronKerboschPivot(new List<string>(), candidates, excluded, graph, result);
        return result;
    }

    private static void BronKerboschPivot(
        List<string> current, HashSet<string> candidates, HashSet<string> excluded,
        Dictionary<string, HashSet<string>> graph, List<List<string>> result)
    {
        if (candidates.Count == 0 && excluded.Count == 0)
        {
            if (current.Count >= 2)
                result.Add(new List<string>(current));
            return;
        }

        // Пивот: вершина с максимальной степенью в C ∪ X (стандартный выбор,
        // сокращает ветвление до кандидатов, не смежных с пивотом).
        string? pivot = null;
        int pivotDegree = -1;
        foreach (var v in candidates)
        {
            int degree = graph.TryGetValue(v, out var nb) ? nb.Count : 0;
            if (degree > pivotDegree) { pivotDegree = degree; pivot = v; }
        }
        if (pivot == null)
        {
            foreach (var v in excluded)
            {
                int degree = graph.TryGetValue(v, out var nb) ? nb.Count : 0;
                if (degree > pivotDegree) { pivotDegree = degree; pivot = v; }
            }
        }

        var pivotNeighbours = (pivot != null && graph.TryGetValue(pivot, out var pn)) ? pn : null;

        foreach (var v in candidates.ToList())
        {
            if (pivotNeighbours != null && pivotNeighbours.Contains(v))
                continue;

            current.Add(v);
            var newCandidates = candidates.Where(x => graph.TryGetValue(v, out var nb) && nb.Contains(x)).ToHashSet(StringComparer.OrdinalIgnoreCase);
            var newExcluded = excluded.Where(x => graph.TryGetValue(v, out var nb2) && nb2.Contains(x)).ToHashSet(StringComparer.OrdinalIgnoreCase);
            BronKerboschPivot(current, newCandidates, newExcluded, graph, result);
            current.RemoveAt(current.Count - 1);
            candidates.Remove(v);
            excluded.Add(v);
        }
    }
}