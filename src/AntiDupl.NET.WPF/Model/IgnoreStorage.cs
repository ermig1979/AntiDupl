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
            model = SerializeHelper<IgnoreStorage>.Load(GetFilePath());
            if (model == null)
                model = new IgnoreStorage();
            return model;
        }

        public void Save()
        {
            SerializeHelper<IgnoreStorage>.Save(this, GetFilePath());
        }

        /// <summary>Полный путь: пользовательские данные — в <c>user\</c> (зеркало WF UserPath).</summary>
        static string GetFilePath() => System.IO.Path.Combine(AntiDupl.NET.WPF.Resources.Resources.UserPath, _fileName);
    }
}
