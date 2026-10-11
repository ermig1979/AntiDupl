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
            _core.searchPath = search ?? new CorePathWithSubFolder[0];
            _core.ignorePath = ignore ?? new CorePathWithSubFolder[0];
            _core.validPath = valid ?? new CorePathWithSubFolder[0];
            _core.deletePath = delete ?? new CorePathWithSubFolder[0];
            _core.ignoreFilenameFilter = ignoreFilenameFilter ?? "";
        }
    }
}