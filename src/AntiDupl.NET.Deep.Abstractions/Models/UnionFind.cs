namespace AntiDupl.NET.Deep.Models;

/// <summary>
/// Disjoint-set (union-find) с path halving и union by size.
/// Перенос UnionFind из DinoDuplicateSearchWPF (DinoDuplicateSearch.ML).
/// </summary>
public class UnionFind
{
    private readonly int[] _parent;
    private readonly int[] _size;

    public UnionFind(int n)
    {
        _parent = new int[n];
        _size = new int[n];
        for (int i = 0; i < n; i++)
        {
            _parent[i] = i;
            _size[i] = 1;
        }
    }

    public int Find(int x)
    {
        while (_parent[x] != x)
        {
            _parent[x] = _parent[_parent[x]];
            x = _parent[x];
        }
        return x;
    }

    public void Union(int a, int b)
    {
        a = Find(a);
        b = Find(b);
        if (a == b) return;
        if (_size[a] < _size[b])
            (a, b) = (b, a);
        _parent[b] = a;
        _size[a] += _size[b];
    }

    public int Size(int x) => _size[Find(x)];

    public int[] GetLabels()
    {
        int n = _parent.Length;
        var rootToLabel = new Dictionary<int, int>();
        int nextLabel = 0;
        var labels = new int[n];
        for (int i = 0; i < n; i++)
        {
            int root = Find(i);
            if (!rootToLabel.TryGetValue(root, out int label))
            {
                label = nextLabel++;
                rootToLabel[root] = label;
            }
            labels[i] = label;
        }
        return labels;
    }
}