using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using Microsoft.Xaml.Behaviors;

namespace AntiDuplWPF.Behavior
{
    class WindowCloseOnEscBehavior : Behavior<Window>
    {
        protected override void OnAttached()
        {
            base.OnAttached();

            AssociatedObject.PreviewKeyDown += AssociatedObject_PreviewKeyDown;

            //_undoRedoEngine = TinyIoCContainer.Current.Resolve<IUndoRedoEngine>();
        }

        void AssociatedObject_PreviewKeyDown(object sender, System.Windows.Input.KeyEventArgs e)
        {
            Window target = sender as Window;

            if (target != null)
            {
                if (e.Key == Key.Escape)
                {
                    target.Close();
                }
            }
        }


        protected override void OnDetaching()
        {
            base.OnDetaching();

            AssociatedObject.PreviewKeyDown -= AssociatedObject_PreviewKeyDown;
        }
    }
}
