using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ObjectModel
{
    public enum FilterConditionParametr
    {
        Difference,
        BothBlockiness,
        BlockinessDifference,
        //BlockinessDifferenceInPercent,
        BothBlurring,
        BlurringDifference,
        //BlurringDifferenceInPercent,
        Resolution,
        ResolutionDifference,
        ResolutionDifferenceInPercent,
        FileSizeDifferenceInPercent,
        NoTransformType,
        //SameDirectory,
        SameType,
        DctHistogramPeaksDifference,
        DctHistogramPeaksDifferenceInPercent,
        OneBlockiness,
        OneBlurring,

    }
}
