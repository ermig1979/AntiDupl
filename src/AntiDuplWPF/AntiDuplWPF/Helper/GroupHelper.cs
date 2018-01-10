using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Helper
{
    class GroupHelper
    {
        public static void ConvertToGroup(IList<DuplicateGroup> groups, IEnumerable<DuplPairViewModel> resultList)
        {
            groups.Clear();
            foreach (var result in resultList)
            {
                DuplicateGroup finded = null;
                foreach (var group in groups)
                {
                    if (group.ContainFile(result.FirstFile) || group.ContainFile(result.SecondFile))
                    {
                        finded = group;
                        break;
                    }
                }
                if (finded != null)
                {
                    finded.AddResult(result);
                }
                else
                    groups.Add(new DuplicateGroup(result));
            }
        }
    }
}
