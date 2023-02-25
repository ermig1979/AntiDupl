using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.GettingNumber
{
    class NumberBeforeUnderlineGettingNumber : IGettingNumber
    {
        public string Name
        {
            get
            {
                return "Number before _";
            }
        }

        public bool ContainNumber(ViewModel.DuplPairViewModel result)
        {
            return GetNumber(result.FirstFile.FileName).HasValue && GetNumber(result.SecondFile.FileName).HasValue;
        }

        public bool ContainNumber(DuplicateGroup group)
        {
            return group.FileList.All(r => GetNumber(r.FileName).HasValue);
        }

        public uint? GetNumber(string fileName)
        {
            int index = fileName.IndexOf("_");
            if (index <= 0)
            {
                Debug.WriteLine(String.Format("Не найдено _ в {0}", fileName));
                return null;
            }
            string substr = fileName.Substring(0, index);
            uint number;
            bool result = uint.TryParse(substr, out number);
            if (!result)
                return null;
            return number;
        }


    }
}
