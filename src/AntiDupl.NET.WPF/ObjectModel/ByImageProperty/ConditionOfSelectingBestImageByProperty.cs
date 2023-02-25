using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.ObjectModel.ConditionMode;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.ObjectModel.ByImageProperty
{
    public class ConditionOfSelectingBestImageByProperty : PropertyChangedBase
    {
        private BestConditionParametr _parametr;
        public BestConditionParametr Parametr
        {
            get { return _parametr; }
            set
            {
                _parametr = value;
                RaisePropertyChangedEvent("Parametr");
            }
        }

        //private IConditionMode _mode;
        //public IConditionMode Mode
        //{
        //    get { return _mode; }
        //    set
        //    {
        //        _mode = value;
        //        RaisePropertyChangedEvent("Mode");
        //    }
        //}

        //private double _valueDouble;
        //public double ValueDouble
        //{
        //    get { return _valueDouble; }
        //    set
        //    {
        //        _valueDouble = value;
        //        RaisePropertyChangedEvent("ValueDouble");
        //    }
        //}

        //private uint _valueUint;
        //public uint ValueUint
        //{
        //    get { return _valueUint; }
        //    set
        //    {
        //        _valueUint = value;
        //        RaisePropertyChangedEvent("ValueUint");
        //    }
        //}

        //public bool ExistBest(DuplicateGroup group)
        //{

        //}

        //public IEnumerable<ImageInfoClass> GetBest(DuplicateGroup group)
        //{

        //}


        //internal bool IsSelect(DuplicateGroup group)
        //{
        //    return group.ResultList.All(r => IsSelect(r));
        //}

        public IEnumerable<ImageInfoClass> GetBest(IEnumerable<ImageInfoClass> bests)
        {
           switch (Parametr)
            {
                case BestConditionParametr.FileSizeBig:
                    return bests.Where(e => e.FileSize == bests.Max(e2 => e2.FileSize));
                case BestConditionParametr.FileSizeSmall:
                    return bests.Where(e => e.FileSize == bests.Min(e2 => e2.FileSize));

                case BestConditionParametr.BlockinessBest:
                    return bests.Where(e => e.Blockiness == bests.Min(e2 => e2.Blockiness));
                case BestConditionParametr.BlockinessBad:
                    return bests.Where(e => e.Blockiness == bests.Max(e2 => e2.Blockiness));

                case BestConditionParametr.BlurringBest:
                    return bests.Where(e => e.Bluring == bests.Min(e2 => e2.Bluring));
                case BestConditionParametr.BlurringBad:
                    return bests.Where(e => e.Bluring == bests.Max(e2 => e2.Bluring));

                case BestConditionParametr.DctHistogramPeaksBest:
                    return bests.Where(e => e.JpegPeaks == bests.Min(e2 => e2.JpegPeaks));
                case BestConditionParametr.DctHistogramPeaksBad:
                    return bests.Where(e => e.JpegPeaks == bests.Max(e2 => e2.JpegPeaks));

                case BestConditionParametr.ResolutionBig:
                    return bests.Where(e => e.Width * e.Height == bests.Max(e2 => e2.Width * e2.Height));
                case BestConditionParametr.ResolutionSmall:
                    return bests.Where(e => e.Width * e.Height == bests.Min(e2 => e2.Width * e2.Height));

                default:
                    throw new ArgumentException("Неизвестный параметр " + Parametr);
           }
           throw new ArgumentException("swith не сработал");
        }



    }
}
