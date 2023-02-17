using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.Helper;
using AntiDuplWPF.ObjectModel.ConditionMode;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.ObjectModel
{
    public class FilterCondition : PropertyChangedBase
    {
     
        private FilterConditionParametr _parametr;
        public FilterConditionParametr Parametr
        {
            get { return _parametr; }
            set
            {
                _parametr = value;
                RaisePropertyChangedEvent("Parametr");
            }
        }

        private IConditionMode _mode;
        public IConditionMode Mode
        {
            get { return _mode; }
            set
            {
                _mode = value;
                RaisePropertyChangedEvent("Mode");
            }
        }

        private double _valueDouble;
        public double ValueDouble
        {
            get { return _valueDouble; }
            set
            {
                _valueDouble = value;
                RaisePropertyChangedEvent("ValueDouble");
            }
        }

        private uint _valueUint;
        public uint ValueUint
        {
            get { return _valueUint; }
            set
            {
                _valueUint = value;
                RaisePropertyChangedEvent("ValueUint");
            }
        }

        //AntiDuplWPF.Core.CoreDll.TransformType _transformType;
        //public AntiDuplWPF.Core.CoreDll.TransformType TransformType
        //      {
        //          get { return _transformType; }
        //    set
        //    {
        //        _transformType = value;
        //        RaisePropertyChangedEvent("TransformType");
        //    }
        //}

        public bool IsSelect(DuplPairViewModel pair)
        {
            if (pair.Type != Core.CoreDll.ResultType.DuplImagePair)
                return false;

            double percent;
            double doubleDifference;
            uint difference;
            switch (Parametr)
            {
                case FilterConditionParametr.Difference:
                    if (Mode == null)
                        return false;
                    return Mode.IsSelect(pair.Difference, ValueDouble);
                    break;
                case FilterConditionParametr.OneBlockiness:
                    if (Mode == null)
                        return false;
                    return Mode.IsSelect(pair.FirstFile.Blockiness, ValueDouble)
                        || Mode.IsSelect(pair.SecondFile.Blockiness, ValueDouble);
                    break;
                case FilterConditionParametr.BothBlockiness:
                    if (Mode == null)
                        return false;
                    return Mode.IsSelect(pair.FirstFile.Blockiness, ValueDouble) 
                        && Mode.IsSelect(pair.SecondFile.Blockiness, ValueDouble);
                    break;
                case FilterConditionParametr.BlockinessDifference:
                    if (Mode == null)
                        return false;
                    doubleDifference = Math.Abs(pair.FirstFile.Blockiness - pair.SecondFile.Blockiness);
                    return Mode.IsSelect(doubleDifference, ValueDouble);
                //case FilterConditionParametr.BlockinessDifferenceInPercent:
                //    if (Mode == null)
                //        return false;
                //    doubleDifference = Math.Abs(pair.FirstFile.Blockiness - pair.SecondFile.Blockiness);
                //    if (doubleDifference == 0)
                //        return Mode.IsSelect(0.0, ValueDouble);
                //    else
                //    {
                //        percent = PercentHelper.CalculatePercent(pair.FirstFile.Blockiness, pair.SecondFile.Blockiness, 100);
                //        return Mode.IsSelect(percent, ValueDouble);
                //    }
                //    break;
                case FilterConditionParametr.OneBlurring:
                    if (Mode == null)
                        return false;
                    return Mode.IsSelect(pair.FirstFile.Bluring, ValueDouble)
                        || Mode.IsSelect(pair.SecondFile.Bluring, ValueDouble);
                    break;
                case FilterConditionParametr.BothBlurring:
                    if (Mode == null)
                        return false;
                    return Mode.IsSelect(pair.FirstFile.Bluring, ValueDouble) 
                        && Mode.IsSelect(pair.SecondFile.Bluring, ValueDouble);
                    break;
                case FilterConditionParametr.BlurringDifference:
                    if (Mode == null)
                        return false;
                    doubleDifference = Math.Abs(pair.FirstFile.Bluring - pair.SecondFile.Bluring);
                    return Mode.IsSelect(doubleDifference, ValueDouble);
                //case FilterConditionParametr.BlurringDifferenceInPercent:
                //    if (Mode == null)
                //        return false;
                //    doubleDifference = Math.Abs(pair.FirstFile.Bluring - pair.SecondFile.Bluring);
                //    if (doubleDifference == 0)
                //        return Mode.IsSelect(0.0, ValueDouble);
                //    else
                //    {
                //        percent = PercentHelper.CalculatePercent(pair.FirstFile.Bluring, pair.SecondFile.Bluring, 100);
                //        return Mode.IsSelect(percent, ValueDouble);
                //    }
                //    break;
                //case ConditionParametr.Resolution:
                //    break;
                case FilterConditionParametr.ResolutionDifference:
                    if (Mode == null)
                        return false;
                    difference = Math.Max(pair.FirstFile.Height * pair.FirstFile.Width, pair.SecondFile.Height * pair.SecondFile.Width)
                        - Math.Min(pair.FirstFile.Height * pair.FirstFile.Width, pair.SecondFile.Height * pair.SecondFile.Width);
                    return Mode.IsSelect(difference, ValueUint);
                    break;
                case FilterConditionParametr.ResolutionDifferenceInPercent:
                    if (Mode == null)
                        return false;
                   difference = PercentHelper.CalculateDifference(pair.FirstFile.Height * pair.FirstFile.Width, pair.SecondFile.Height * pair.SecondFile.Width);
                   if (difference == 0)
                        return Mode.IsSelect(0.0, ValueDouble);
                    else
                    {
                        percent = PercentHelper.CalculatePercent(pair.FirstFile.Height * pair.FirstFile.Width, pair.SecondFile.Height * pair.SecondFile.Width); 
                        return Mode.IsSelect(percent, ValueDouble);
                    }
                    break;
                //case ConditionParametr.FileSize:
                //    break;
                case FilterConditionParametr.NoTransformType:
                    return pair.Transform == Core.CoreDll.TransformType.Turn_0;
                    break;
                //case ConditionParametr.SameDirectory:
                //    return pair.FirstFile.Directory == pair.SecondFile.Directory;
                //    break;
                case FilterConditionParametr.SameType:
                    return pair.FirstFile.Type == pair.SecondFile.Type;
                case FilterConditionParametr.DctHistogramPeaksDifference:
                    if (Mode == null)
                        return false;
                    difference = Math.Max(pair.FirstFile.JpegPeaks, pair.SecondFile.JpegPeaks)
                        - Math.Min(pair.FirstFile.JpegPeaks, pair.SecondFile.JpegPeaks);
                    return Mode.IsSelect(difference, ValueUint);
                case FilterConditionParametr.DctHistogramPeaksDifferenceInPercent:
                    if (Mode == null)
                        return false;
                    difference = PercentHelper.CalculateDifference(pair.FirstFile.JpegPeaks, pair.SecondFile.JpegPeaks);
                    if (difference == 0)
                        return Mode.IsSelect(0.0, ValueDouble);
                    else
                    {
                        percent = PercentHelper.CalculatePercent(pair.FirstFile.JpegPeaks, pair.SecondFile.JpegPeaks);
                        return Mode.IsSelect(percent, ValueDouble);
                    }

                case FilterConditionParametr.FileSizeDifferenceInPercent:
                    if (Mode == null)
                        return false;
                    decimal fileSizeDifference = PercentHelper.CalculateDifference(pair.FirstFile.FileSize, pair.SecondFile.FileSize);
                    if (fileSizeDifference == 0)
                        return Mode.IsSelect(0.0, ValueDouble);
                    else
                    {
                        percent = PercentHelper.CalculatePercent(pair.FirstFile.FileSize, pair.SecondFile.FileSize);
                        return Mode.IsSelect(percent, ValueDouble);
                    }
                    break;
                default:
                    throw new ArgumentException("Неизвестный параметр" + Parametr);
            }
            return true;
        }

        internal bool IsSelect(DuplicateGroup group)
        {
            return group.ResultList.All(r => IsSelect(r));
        }
    }
}
