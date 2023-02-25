using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Helper
{
    public class SaatiHelper
    {
        public static void CalculateIndex(IEnumerable<ImageInfoClass> images, IConfigurationModel configuration)
        {
            if (configuration.Criterions.All(cr => !cr.IsUse))
            {
                foreach (var item in images)
                {
                    item.UtilityIndexCalculateDecription = "All criterions not in use";
                }
            }
            else
            {
                var resolutionCr = configuration.Criterions.First(cr => cr.Type == CriterionType.Resolution);
                var blockinessCr = configuration.Criterions.First(cr => cr.Type == CriterionType.Blockiness);
                var bluringCr = configuration.Criterions.First(cr => cr.Type == CriterionType.Bluring);
                var fileSizeSmalCr = configuration.Criterions.First(cr => cr.Type == CriterionType.SmallerFileSize);
                var fileSizeLarCr = configuration.Criterions.First(cr => cr.Type == CriterionType.LargerFileSize);
                if (fileSizeLarCr.IsUse && fileSizeSmalCr.IsUse)
                    throw new ArgumentException("Задан одновременно поиск и наибольшего и наименьшего файлов!");

                decimal sumResolution = (decimal)images.Sum(i => i.Width * i.Height);
                decimal sumBlockiness = (decimal)images.Sum(i => double.IsNaN(i.Blockiness) ? 0 : i.Blockiness);
                decimal sumBluring = (decimal)images.Sum(i => i.Bluring);
                decimal sumFileSize = images.Sum(i => (decimal)i.FileSize);

                if (sumResolution == 0 && resolutionCr.IsUse)
                    return;
                    //throw new ArgumentException("sumResolution == 0!");
                if (sumBlockiness == 0 && blockinessCr.IsUse)
                    return;
                    //throw new ArgumentException("sumJpegQuality == 0!");
                if (sumBluring == 0 && bluringCr.IsUse)
                    return;
                    //throw new ArgumentException("sumSharpness == 0!");
                if (sumFileSize == 0 && (fileSizeSmalCr.IsUse || fileSizeLarCr.IsUse))
                    return;
                    //throw new ArgumentException("sumFileSize == 0!");
                //decimal sum = (decimal)(FirstFile.Width * FirstFile.Height + SecondFile.Width * FirstFile.Height);
                foreach (var item in images)
                {
                    decimal resolutionNorm = (item.Width * item.Height) / sumResolution;
                    decimal blockinessNorm = (decimal)item.Blockiness / sumBlockiness;
                    decimal bluringNorm = (decimal)item.Bluring / sumBluring;
                    decimal fileSizeSmalNorm = 1 - (decimal)item.FileSize / sumFileSize;
                    decimal fileSizeLarNorm = (decimal)item.FileSize / sumFileSize;

                    item.UtilityIndexNotNorm = (resolutionCr.IsUse ? resolutionNorm * resolutionCr.Weight : 0)
                        + (blockinessCr.IsUse ? blockinessNorm * blockinessCr.Weight : 0)
                        + (bluringCr.IsUse ? bluringNorm * bluringCr.Weight : 0)
                        + (fileSizeSmalCr.IsUse ? fileSizeSmalNorm * fileSizeSmalCr.Weight : 0)
                        + (fileSizeLarCr.IsUse ? fileSizeLarNorm * fileSizeLarCr.Weight : 0);

                    string fileSizeDesc = "";
                    if (fileSizeSmalCr.IsUse)
                        fileSizeDesc = "fileSizeSmal " + fileSizeSmalCr.Weight;
                    if (fileSizeLarCr.IsUse)
                        fileSizeDesc = "fileSizeLarg " + fileSizeLarCr.Weight;
                    item.UtilityIndexCalculateDecription =
                        String.Format("{0}{1}{2}{3} = {4}",
                        resolutionCr.IsUse ? String.Format("resolution {0} +", resolutionNorm * resolutionCr.Weight) : "",
                        blockinessCr.IsUse ? String.Format("blockiness {0} +", blockinessNorm * blockinessCr.Weight) : "",
                        bluringCr.IsUse ? String.Format("bluring {0} +", bluringNorm * bluringCr.Weight) : "",
                        fileSizeDesc,
                        item.UtilityIndexNotNorm);
                }

                decimal sumUtilityIndex = images.Sum(i => i.UtilityIndexNotNorm);
                if (sumUtilityIndex > 0)
                {
                    foreach (var item in images)
                    {
                        item.UtilityIndex = item.UtilityIndexNotNorm / sumUtilityIndex;
                    }
                    //для того чтобы правильно подсветка сработала
                    foreach (var item in images)
                    {
                        item.RaiseUpdateUtilityIndex();
                    }
                }

                decimal totalSum = Math.Round(images.Sum(i => i.UtilityIndex), 15);
                if (totalSum > 1)
                    throw new Exception(String.Format("Sum of UtilityIndex ({0}) > 1", totalSum));
            }
        }

        public static void CalculateIndex(DuplPairViewModel viewResultOfDupl, IConfigurationModel configuration)
        {
            List<ImageInfoClass> list = new List<ImageInfoClass>(2);
            list.Add(viewResultOfDupl.FirstFile);
            list.Add(viewResultOfDupl.SecondFile);
            CalculateIndex(list, configuration);
        }

        public static void RecalculateIndex(IEnumerable<Criterion> criterions, Criterion changedCriterion)
        {
            List<Criterion> forChange = criterions.Where(cr => cr.IsUse && cr.Type != changedCriterion.Type).ToList();
            decimal sumForChange = forChange.Sum(cr => cr.Weight);
            if (sumForChange + changedCriterion.Weight > 1)
            {
                decimal delta = changedCriterion.Weight + sumForChange - 1;
                decimal forMinus = delta / forChange.Count();
                foreach (var cr in forChange)
                {
                    if (cr.Weight > forMinus)
                        cr.Weight -= forMinus;
                }
            }
            else if (sumForChange + changedCriterion.Weight < 1)
            {
                decimal delta = 1 - (changedCriterion.Weight + sumForChange);
                decimal forPlus = delta / forChange.Count();
                foreach (var cr in forChange)
                {
                    cr.Weight += forPlus;
                }
            }
        }
    }
}
