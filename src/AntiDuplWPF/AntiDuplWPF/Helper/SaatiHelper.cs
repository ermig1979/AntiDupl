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
            var resolutionCr = configuration.Criterions.First(cr => cr.Type == CriterionType.Resolution);
            var jpegQualityCr = configuration.Criterions.First(cr => cr.Type == CriterionType.JpegQuality);
            var sharpCr = configuration.Criterions.First(cr => cr.Type == CriterionType.Sharpness);
            var fileSizeSmalCr = configuration.Criterions.First(cr => cr.Type == CriterionType.SmallerFileSize);
            var fileSizeLarCr = configuration.Criterions.First(cr => cr.Type == CriterionType.LargerFileSize);
            if (fileSizeLarCr.IsUse && fileSizeSmalCr.IsUse)
                throw new ArgumentException("Задан одновременно поиск и наибольшего и наименьшего файлов!");

            decimal sumResolution = (decimal)images.Sum(i => i.Width * i.Height);
            decimal sumJpegQuality = (decimal)images.Sum(i => i.JpegQuality);
            decimal sumSharpness = (decimal)images.Sum(i => i.Sharpness);
            decimal sumFileSize = images.Sum(i => (decimal)i.FileSize);

            if (sumResolution == 0)
                throw new ArgumentException("sumResolution == 0!");
            if (sumJpegQuality == 0)
                throw new ArgumentException("sumJpegQuality == 0!");
            if (sumSharpness == 0)
                throw new ArgumentException("sumSharpness == 0!");
            if (sumFileSize == 0)
                throw new ArgumentException("sumFileSize == 0!");
            //decimal sum = (decimal)(FirstFile.Width * FirstFile.Height + SecondFile.Width * FirstFile.Height);
            foreach (var item in images)
            {
                decimal resolutionNorm = (item.Width * item.Height) / sumResolution;
                decimal jpegQualityNorm = (decimal)item.JpegQuality / sumJpegQuality;
                decimal sharpnessNorm = (decimal)item.Sharpness / sumSharpness;
                decimal fileSizeSmalNorm = 1 - (decimal)item.FileSize / sumFileSize;
                decimal fileSizeLarNorm = (decimal)item.FileSize / sumFileSize;

                item.UtilityIndexNotNorm = (resolutionCr.IsUse ? resolutionNorm * resolutionCr.Weight : 0)
                    + (jpegQualityCr.IsUse ? jpegQualityNorm * jpegQualityCr.Weight : 0)
                    + (sharpCr.IsUse ? sharpnessNorm * sharpCr.Weight : 0)
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
                    jpegQualityCr.IsUse ? String.Format("jpegQuality {0} +", jpegQualityNorm * jpegQualityCr.Weight) : "",
                    sharpCr.IsUse ? String.Format("sharpness {0} +", sharpnessNorm * sharpCr.Weight) : "",
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
            }

            decimal totalSum = Math.Round(images.Sum(i => i.UtilityIndex), 15);
            if (totalSum > 1)
                throw new Exception(String.Format("Sum of UtilityIndex ({0}) > 1", totalSum));
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
