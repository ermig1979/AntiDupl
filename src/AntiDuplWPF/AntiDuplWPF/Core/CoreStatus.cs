using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.Core
{
    public class CoreStatus
    {
        public CoreDll.StateType state;
        public string path;
        public int current;
        public int total;

        public CoreStatus(ref CoreDll.adStatusW status)
        {
            state = status.state;
            path = status.path;
            current = (int)status.current.ToUInt32();
            total = (int)status.total.ToUInt32();
        }
    }
}
