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
        private ICoreLib _core;

        public SearchOption SearchOptions { get; set; }
        public CompareOption CompareOptions { get; set; }
        public AdvancedOption AdvancedOptions { get; set; }
        public DefectOption DefectOptions { get; set; }

        public Option(ICoreLib core)
        {
            this._core = core;

            SearchOptions = new SearchOption();
            CompareOptions = new CompareOption();
            AdvancedOptions = new AdvancedOption();
            DefectOptions = new DefectOption();
        }

        public void CopyToDll()
        {
            _core.SetSearchOptions(SearchOptions);
            _core.SetAdvancedOptions(AdvancedOptions);
            _core.SetCompareOptions(CompareOptions);
            _core.SetDefectOptions(DefectOptions);
        }

    }
}
