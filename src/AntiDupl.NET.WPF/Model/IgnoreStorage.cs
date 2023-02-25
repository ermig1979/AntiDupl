using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDupl.NET.WPF.Helper;
using AntiDupl.NET.WPF.ViewModel;

namespace AntiDupl.NET.WPF.Model
{
    public class IgnoreStorage
    {
        [NonSerialized]
        private const string _fileName = @"ignored.xml";

        public IgnoreStorage()
        {
            IgnoreList = new List<DuplPairViewModel>();
        }

        public List<DuplPairViewModel> IgnoreList { get; set; }


        public static IgnoreStorage Load()
        {
            IgnoreStorage model;
            model = SerializeHelper<IgnoreStorage>.Load(_fileName);
            if (model == null)
                model = new IgnoreStorage();
            return model;
        }

        public void Save()
        {
            SerializeHelper<IgnoreStorage>.Save(this, _fileName);
        }
    }
}
