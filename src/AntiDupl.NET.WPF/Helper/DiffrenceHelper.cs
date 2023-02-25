using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.Helper
{
    class DiffrenceHelper
    {
        internal static void PrintDiffrence(StringBuilder sb, ViewModel.DuplPairViewModel pair)
        {
            decimal diffrenceDec = PercentHelper.CalculateDifference(pair.FirstFile.FileSize, pair.SecondFile.FileSize);
            sb.AppendLine(String.Format("FileSize diffrence {0}", diffrenceDec));
            var percent = PercentHelper.CalculatePercent(pair.FirstFile.FileSize, pair.SecondFile.FileSize);
            sb.AppendLine(String.Format("FileSize diffrence in percent {0}%", percent));

            var diffrence = Math.Abs(pair.FirstFile.Blockiness - pair.SecondFile.Blockiness);
            sb.AppendLine(String.Format("Blockiness diffrence {0}", diffrence));
            //percent = PercentHelper.CalculatePercent(result.FirstFile.Blockiness, result.SecondFile.Blockiness, 100);
            //sb.AppendLine(String.Format("Blockiness diffrence in percent {0}", percent));

            diffrence = Math.Abs(pair.FirstFile.Bluring - pair.SecondFile.Bluring);
            sb.AppendLine(String.Format("Bluring diffrence {0}", diffrence));
            //percent = PercentHelper.CalculatePercent(result.FirstFile.Bluring, result.SecondFile.Bluring, 100);
            //sb.AppendLine(String.Format("Bluring diffrence in percent {0}", percent));

            diffrence = Math.Abs(pair.FirstFile.Width * pair.FirstFile.Height
                - pair.SecondFile.Width * pair.SecondFile.Height);
            sb.AppendLine(String.Format("Resolution diffrence {0}", diffrence));
            percent = PercentHelper.CalculatePercent(pair.FirstFile.Width * pair.FirstFile.Height,
                pair.SecondFile.Width * pair.SecondFile.Height);
            sb.AppendLine(String.Format("Resolution diffrence in percent {0}%", percent));

            diffrence = Math.Abs(pair.FirstFile.JpegPeaks - pair.SecondFile.JpegPeaks);
            sb.AppendLine(String.Format("JPEG DCT Histogram peak diffrence {0}", diffrence));
            percent = PercentHelper.CalculatePercent(pair.FirstFile.JpegPeaks, pair.SecondFile.JpegPeaks);
            sb.AppendLine(String.Format("JPEG DCT Histogram peak diffrence in percent {0}%", percent));
        }
    }
}
