using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel
{
    public enum BestConditionParametr
    {
        FileSizeBig,
        FileSizeSmall,
        BlockinessBest,
        BlockinessBad,
        BlurringBest,
        BlurringBad,
        ResolutionBig,
        ResolutionSmall,
        DctHistogramPeaksBest,
        DctHistogramPeaksBad
    }
}
