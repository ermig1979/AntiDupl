using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.ObjectModel.ByPath
{
    class BestImageShouldHaveASmallerNumber : IBestSelectingByPath
    {
        public string Name { get { return "BestImageShouldHaveASmallerNumber"; } }

        public ImageInfoClass GetBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName)
        {
            //var firstNumber = algorithmOfGettingNumberFromFileName.GetNumber(item.FirstFile.FileName);
            //var secondNumber = algorithmOfGettingNumberFromFileName.GetNumber(item.SecondFile.FileName);
            //if (!firstNumber.HasValue || !secondNumber.HasValue)
            //    return new SkipAction(String.Format("Не удалось получить номер из {0} или {1}",
            //        item.FirstFile.FileName, item.SecondFile.FileName));
            ////throw new Exception(String.Format("Не удалось получить номер из {0} или {1}",
            ////    item.FirstFile.FileName, item.SecondFile.FileName));
            //if (firstNumber.Value < secondNumber.Value)
            //{
            //    best = item.FirstFile;
            //    bad = item.SecondFile;
            //}
            //else if (secondNumber.Value < firstNumber.Value)
            //{
            //    best = item.SecondFile;
            //    bad = item.FirstFile;
            //}
            //else
            //    return new SkipAction("Номера файлов равны!");
            ////throw new Exception("Номера файлов равны!");

            var minNumber = group.FileList.Min(e => algorithmOfGettingNumberFromFileName.GetNumber(e.FileName));
            var filtered = group.FileList.Where(e => algorithmOfGettingNumberFromFileName.GetNumber(e.FileName) == minNumber);
            if (filtered.Count() > 1)
            {
                Debug.WriteLine("Одинаковые номера у следующих файлов");
                foreach (var item in filtered)
                {
                    Debug.WriteLine(item.Path);
                }
            }
            return group.FileList.Single(e => algorithmOfGettingNumberFromFileName.GetNumber(e.FileName) == minNumber);
        }


        public bool ExistBest(DuplicateGroup group, GettingNumber.IGettingNumber algorithmOfGettingNumberFromFileName)
        {
            var minNumber = group.FileList.Min(e => algorithmOfGettingNumberFromFileName.GetNumber(e.FileName));
            var filtered = group.FileList.Where(e => algorithmOfGettingNumberFromFileName.GetNumber(e.FileName) == minNumber);
            return filtered.Count() == 1;
        }
    }
}
