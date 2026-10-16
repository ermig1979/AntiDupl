using System.Diagnostics;
using AntiDupl.NET.Core;

namespace AntiDupl.NET.WPF.Core
{
    /// <summary>
    /// Реализация ICoreLocationSync поверх CoreLib (конкретный класс, без интерфейсов,
    /// по CA-политике WPF-слоя).
    /// </summary>
    public class CoreLibLocationSync : ICoreLocationSync
    {
        private readonly CoreLib _core;

        public CoreLibLocationSync(CoreLib core)
        {
            _core = core;
        }

        public void SetLocations(CorePathWithSubFolder[] search,
            CorePathWithSubFolder[] ignore,
            CorePathWithSubFolder[] valid,
            CorePathWithSubFolder[] delete,
            string ignoreFilenameFilter)
        {
            // null-массивы маскируются ?? ниже: в отладке показываем, что это баг вызывающего.
            Debug.Assert(search is not null, nameof(search));
            Debug.Assert(ignore is not null, nameof(ignore));
            Debug.Assert(valid is not null, nameof(valid));
            Debug.Assert(delete is not null, nameof(delete));
            _core.searchPath = search ?? new CorePathWithSubFolder[0];
            _core.ignorePath = ignore ?? new CorePathWithSubFolder[0];
            _core.validPath = valid ?? new CorePathWithSubFolder[0];
            _core.deletePath = delete ?? new CorePathWithSubFolder[0];
            _core.ignoreFilenameFilter = ignoreFilenameFilter ?? "";
        }
    }
}