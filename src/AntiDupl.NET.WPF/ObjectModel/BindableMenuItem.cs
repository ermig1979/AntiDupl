using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace AntiDupl.NET.WPF.ObjectModel
{
    public class BindableMenuItem
    {
        public string Name { get; set; }
        public BindableMenuItem[] Children { get; set; }
        public ICommand Command { get; set; }
    }
}
