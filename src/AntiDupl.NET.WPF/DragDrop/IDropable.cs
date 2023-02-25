using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDupl.NET.WPF.DragDrop
{
    interface IDropable
    {
        /// <summary>
        /// Drop data into the collection.
        /// </summary>
        /// <param name="data">The data to be dropped</param>
        void Drop(object dropObject);
    }
}
