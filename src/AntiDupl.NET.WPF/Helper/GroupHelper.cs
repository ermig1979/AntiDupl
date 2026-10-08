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
            var byId = new Dictionary<int, DuplicateGroup>();
            foreach (var result in resultList)
            {
                if (result.DeepGroupId >= 0)
                {
                    // Deep-режим: номер группы присвоен поиском (группа = клик WGC-графа).
                    // Слияние по общим файлам НЕ делается: файл может входить в несколько
                    // групп (семантика референса) — пары из разных групп остаются отдельными.
                    if (!byId.TryGetValue(result.DeepGroupId, out var deepGroup))
                    {
                        deepGroup = new DuplicateGroup(result);
                        byId[result.DeepGroupId] = deepGroup;
                        groups.Add(deepGroup);
                    }
                    else
                    {
                        deepGroup.AddResult(result);
                    }
                    continue;
                }

                // Классический режим: транзитивное замыкание по общим файлам (семантика ядра).
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
