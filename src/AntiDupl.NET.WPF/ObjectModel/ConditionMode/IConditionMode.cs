using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.ConditionMode
{
    public interface IConditionMode
    {
        string Name { get; }

        bool IsSelect(double sourceValue, double compareValue);
        bool IsSelect(uint sourceValue, uint compareValue);
    }
}
