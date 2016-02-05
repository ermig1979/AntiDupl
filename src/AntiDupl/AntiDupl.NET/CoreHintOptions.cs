using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AntiDupl.NET
{
    public class CoreHintOptions
    {
        public CoreDll.AlgorithmOfHintSetting algorithmOfHintSetting;

        public CoreHintOptions()
        {
        }

        public CoreHintOptions(ref CoreDll.adHintOptions hintOptions)
        {
            algorithmOfHintSetting = hintOptions.algorithmOfHintSetting;
        }

        public void ConvertTo(ref CoreDll.adHintOptions hintOptions)
        {
            hintOptions.algorithmOfHintSetting = algorithmOfHintSetting;
        }

    }
}
