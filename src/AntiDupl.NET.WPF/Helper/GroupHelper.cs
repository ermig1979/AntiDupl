using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Helper
{
    class GroupHelper
    {
        public static void ConvertToGroup(IList<DuplicateGroup> groups, IEnumerable<DuplPairViewModel> resultList)
        {
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

        public static void GroupToList(IEnumerable<DuplicateGroup> groups, IList<DuplPairViewModel> results)
        {
            foreach (var group in groups)
            {
                foreach (var result in group.ResultList)
                {
                    results.Add(result);
                }
            }
        }
    }
}
