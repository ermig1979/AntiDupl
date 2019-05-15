using AntiDuplWPF.Core;

namespace AntiDuplWPF.Model
{
    public class Option
    {
        private readonly ICoreLib _core;

        public SearchOption SearchOptions { get; private set; }
        public CompareOption CompareOptions { get; private set; }
        public AdvancedOption AdvancedOptions { get; private set; }
        public DefectOption DefectOptions { get; private set; }

        public Option(ICoreLib core)
        {
            _core = core;

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