using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Interactivity;
using AntiDuplWPF.DragDrop;

namespace AntiDuplWPF.Behavior
{
    public class FrameworkElementDropBehavior : Behavior<FrameworkElement>
    {
        protected override void OnAttached()
        {
            base.OnAttached();

            this.AssociatedObject.AllowDrop = true;
            //this.AssociatedObject.DragEnter += new DragEventHandler(AssociatedObject_DragEnter);
            this.AssociatedObject.DragOver += new DragEventHandler(AssociatedObject_DragOver);
            this.AssociatedObject.Drop += new DragEventHandler(AssociatedObject_Drop);
        }

        void AssociatedObject_Drop(object sender, DragEventArgs e)
        {
            //if the data type can be dropped 
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                //drop the data
                IDropable target = this.AssociatedObject.DataContext as IDropable;
                target.Drop(e.Data.GetData(DataFormats.FileDrop));

                //remove the data from the source
                //IDragable source = e.Data.GetData(DataFormats.FileDrop) as IDragable;
                //source.Remove(e.Data.GetData(DataFormats.FileDrop));
            }

            e.Handled = true;
            return;
        }

        void AssociatedObject_DragOver(object sender, DragEventArgs e)
        {
            //if item can be dropped
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                //give mouse effect
                this.SetDragDropEffects(e);
            }

            e.Handled = true;
        }

        /*void AssociatedObject_DragEnter(object sender, DragEventArgs e)
        {
            //if the DataContext implements IDropable, record the data type that can be dropped
            if (this.dataType == null)
            {
                if (this.AssociatedObject.DataContext != null)
                {
                    IDropable dropObject = this.AssociatedObject.DataContext as IDropable;
                    if (dropObject != null)
                    {
                        this.dataType = dropObject.DataType;
                    }
                }
            }
            e.Handled = true;
        }*/

        /// <summary>
        /// Provides feedback on if the data can be dropped
        /// </summary>
        /// <param name="e"></param>
        private void SetDragDropEffects(DragEventArgs e)
        {
            e.Effects = DragDropEffects.None;  //default to None

            //if the data type can be dropped 
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                e.Effects = DragDropEffects.Move;
            }
        }

    }
}
