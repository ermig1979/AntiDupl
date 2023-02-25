using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.ConditionMode
{
    class MoreConditionMode : IConditionMode
    {
        public string Name
        {
            get
            {
                return "More";
            }
        }

        public bool IsSelect(double sourceValue, double compareValue)
        {
            return sourceValue > compareValue;
        }

        public bool IsSelect(uint sourceValue, uint compareValue)
        {
            return sourceValue > compareValue;
        }

        public override bool Equals(Object obj)
        {
            // Check for null values and compare run-time types.
            if (obj == null || GetType() != obj.GetType())
                return false;

            //Если ссылки указывают на один и тот же адрес, то их идентичность гарантирована.
            if (object.ReferenceEquals(this, obj))
                return true;

            IConditionMode c = (IConditionMode)obj;
            return Name.Equals(c.Name);
        }
    }
}
