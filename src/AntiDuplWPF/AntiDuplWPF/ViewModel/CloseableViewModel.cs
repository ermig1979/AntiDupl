using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using AntiDuplWPF.Command;

namespace AntiDuplWPF.ViewModel
{
    public abstract class CloseableViewModel : PropertyChangedBase
    {
        public event EventHandler<BoolEventHandler> ClosingRequest;

        public void OnClosingRequest(bool dialogResult)
        {
            if (this.ClosingRequest != null)
            {
                this.ClosingRequest(this, new BoolEventHandler(dialogResult));
            }
        }

        ICommand _closeCommand;
        public virtual ICommand CloseCommand
        {
            get
            {
                return _closeCommand ?? (_closeCommand = new RelayCommand(arg =>
                {
                    OnClosingRequest(false);
                }, arg => true));
            }
        }

        public virtual void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
        }
    }
}
