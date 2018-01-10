using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.ObjectModel
{
    public class DuplicateGroup
    {
        private DuplPairViewModel result;
        public List<DuplPairViewModel> ResultList { get; set; }
        public ObservableCollection<ImageInfoClass> FileList { get; set; }

        public DuplicateGroup(DuplPairViewModel result)
        {
            ResultList = new List<DuplPairViewModel>(1);
            ResultList.Add(result);
            FileList = new ObservableCollection<ImageInfoClass>();
            FileList.Add(result.FirstFile);
            FileList.Add(result.SecondFile);
        }


        internal void AddResult(DuplPairViewModel result)
        {
            ResultList.Add(result);
            if (!FileList.Contains(result.FirstFile))
                FileList.Add(result.FirstFile);
            if (!FileList.Contains(result.SecondFile))
                FileList.Add(result.SecondFile);
        }

        internal bool ContainFile(ImageInfoClass imageInfoClass)
        {
            return FileList.Contains(imageInfoClass);
        }
    }
}
