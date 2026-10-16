using System;

namespace AntiDupl.NET.WPF.Model
{
    /// <summary>
    /// Видимость колонок таблицы результатов (зеркало WF ResultsOptions):
    /// значения по порядковому индексу колонок DataGrid.Columns.
    /// null / короткое массив = все колонки видимы по умолчанию.
    /// </summary>
    [Serializable]
    public class ResultsColumnsOptions
    {
        /// <summary>Видимость колонки по её порядковому индексу в DataGrid.Columns.</summary>
        public bool[] Visible { get; set; }
    }
}