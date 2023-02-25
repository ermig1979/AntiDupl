using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel
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

		//ValueUint
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
