using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.Helper
{
    class PercentHelper
    {
        public static uint CalculateDifference(uint p1, uint p2)
        {
            return Math.Max(p1, p2) - Math.Min(p1, p2);
        }

        public static double CalculatePercent(uint p1, uint p2)
        {
            return 100 - Math.Min(p1, p2) * 100 / Math.Max(p1, p2);
        }

        public static decimal CalculateDifference(ulong p1, ulong p2)
        {
            return Math.Abs(Convert.ToDecimal(p1) - Convert.ToDecimal(p2));
        }

        public static double CalculatePercent(ulong p1, ulong p2)
        {
            return 100 - Math.Min(p1, p2) * 100 / Math.Max(p1, p2);
        }

        public static double CalculatePercent(double p1, double p2, double maxValue)
        {
            return Math.Abs(p1 - p2) * 100 / maxValue;
        }
    }
}
