using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.UndoRedo
{
    struct RenamedImage
    {
        public ImageInfoClass ImageInfo;
        public string PathWithTempExtension;

        public RenamedImage(ImageInfoClass item)
        {
            ImageInfo = item;
            PathWithTempExtension = item.Path + Const.ForDeleteEXTENSION;
        }
    }
}
