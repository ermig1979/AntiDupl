using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.ViewModel
{
    public class PropertyChangedBase : INotifyPropertyChanged, INotifyPropertyChanging
    {

        #region INotifyPropertyChanged Members

        //реализация INotifyPropertyChanged
        public event PropertyChangedEventHandler PropertyChanged;
        protected virtual void RaisePropertyChangedEvent(string propertyName)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }

        #endregion

        #region INotifyPropertyChanging Members

        public event PropertyChangingEventHandler PropertyChanging;

        /// <summary>
        /// Raises the PropertyChanging event.
        /// </summary>
        /// <param name="propertyName">The name of the changing property.</param>
        public virtual void RaisePropertyChangingEvent(string propertyName)
        {
            // Exit if changes ignored
            //if (IgnorePropertyChangeEvents) return;

            // Exit if no subscribers
            if (PropertyChanging == null) return;

            // Raise event
            var e = new PropertyChangingEventArgs(propertyName);
            PropertyChanging(this, e);
        }


        #endregion
    }
}
