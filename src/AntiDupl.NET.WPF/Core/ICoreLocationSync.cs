using AntiDupl.NET.Core;

namespace AntiDupl.NET.WPF.Core
{
    /// <summary>
    /// Порт синхронизации списков путей (search/ignore/valid/delete + фильтр имён)
    /// из UI-моделей в ядро. Единственное новое интерфейс-исключение в WPF-слое,
    /// явно разрешённое планом миграции (wpf-migration-analysis.md, окно путей):
    /// позволяет LocationsModel не зависеть от конкретного CoreLib.
    /// </summary>
    public interface ICoreLocationSync
    {
        /// <summary>Проталкивает списки путей и фильтр имён в ядро перед поиском.</summary>
        void SetLocations(CorePathWithSubFolder[] search,
            CorePathWithSubFolder[] ignore,
            CorePathWithSubFolder[] valid,
            CorePathWithSubFolder[] delete,
            string ignoreFilenameFilter);
    }
}