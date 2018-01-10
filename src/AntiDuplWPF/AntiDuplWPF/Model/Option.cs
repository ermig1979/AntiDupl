using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Core;

namespace AntiDuplWPF.Model
{
    public class Option
    {
        private CoreLib _core;

        public SearchOption SearchOptions { get; set; }
        public CompareOption CompareOptions { get; set; }

        public Option(CoreLib core)
        {
            this._core = core;

            SearchOptions = new SearchOption();
            CompareOptions = new CompareOption();
        }

        public void CopyToDll()
        {
            _core.SetSearchOptions(SearchOptions);
        }
    }
}
