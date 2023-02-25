using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.ObjectModel.ByImageProperty
{
    public class SelectingBestImageByProperty : PropertyChangedBase
    {
        public SelectingBestImageByProperty()
        {
            ConditionsOfSelectingBestImageByProperty = new ObservableCollection<ConditionOfSelectingBestImageByProperty>();
        }

        ObservableCollection<ConditionOfSelectingBestImageByProperty> _conditionsOfSelectingBestImageByProperty;
        public ObservableCollection<ConditionOfSelectingBestImageByProperty> ConditionsOfSelectingBestImageByProperty
        {
            get { return _conditionsOfSelectingBestImageByProperty; }
            set
            {
                _conditionsOfSelectingBestImageByProperty = value;
                RaisePropertyChangedEvent("ConditionsOfSelectingBestImageByProperty");
            }
        }

        public IEnumerable<ImageInfoClass> GetBest(IEnumerable<ImageInfoClass> fileList)
        {
            IEnumerable<ImageInfoClass> bests = fileList;
            foreach (var item in ConditionsOfSelectingBestImageByProperty)
            {
                var result = bests.Intersect(item.GetBest(fileList));
                bests = bests.Intersect(result);
                if (!bests.Any())
                    break;
            }
            return bests;
        }

        internal bool ExistBest(DuplicateGroup group)
        {
            var best = GetBest(group.FileList);
            return best.Any() && best.Count() != group.FileList.Count;
        }
    }
}
