using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using AntiDuplWPF.UndoRedo;
using TinyIoC;
using System.Windows.Input;
using System.Windows;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.Command;
using System.IO;

namespace AntiDuplWPF.Control
{
    public class RenameImageTextBox : RenameTextBox
    {
        IUndoRedoEngine _undoRedoEngine;

        public RenameImageTextBox()
        {
            Command = RenameCommand;
            _undoRedoEngine = TinyIoCContainer.Current.Resolve<IUndoRedoEngine>();
        }


        public static readonly DependencyProperty ImageInfoProperty =
           DependencyProperty.Register("ImageInfo", typeof(ImageInfoClass), typeof(RenameImageTextBox),
           new PropertyMetadata(OnImageInfoChanged));

        public ImageInfoClass ImageInfo
        {
            get
            {
                return (ImageInfoClass)GetValue(ImageInfoProperty);
            }
            set
            {
                SetValue(ImageInfoProperty, value);
            }
        }

        private static void OnImageInfoChanged(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            RenameImageTextBox myControl = sender as RenameImageTextBox;
            myControl.SubscriptionOnTextChanged = false;
            myControl.Text = GetText(myControl.ShowFullPath, (ImageInfoClass)e.NewValue);
            myControl.SubscriptionOnTextChanged = true;

            if (e.OldValue != null)
            {
                ((ImageInfoClass)e.OldValue).PropertyChanged -= ((RenameImageTextBox)sender).ImageInfoClass_PropertyChanged;
            }

            if (e.NewValue != null)
            {
                ((ImageInfoClass)e.NewValue).PropertyChanged += ((RenameImageTextBox)sender).ImageInfoClass_PropertyChanged;
            }

            // Gestion des synchonisations des colonnes
            //double width = (double)sender;

            //e.Property = (double)e.NewValue + 5;
            //myControl.imgCurrent.Source = ((ImageInfoClass)e.NewValue).Image;
        }

        private static string GetText(bool showFullPath, ImageInfoClass imageInfoClass)
        {
            if (imageInfoClass != null)
            {
                if (showFullPath)
                    return imageInfoClass.Path;
                else
                    return System.IO.Path.GetFileName(imageInfoClass.Path);
            }
            return "";
        }

        private void ImageInfoClass_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            //this.InvalidateProperty(MyClassProperty);
            if (e.PropertyName == "Path")
                this.Text = GetText(ShowFullPath, (ImageInfoClass)sender);
        }

        public static readonly DependencyProperty ShowFullPathProperty =
           DependencyProperty.Register("ShowFullPath", typeof(bool), typeof(RenameImageTextBox),
           new PropertyMetadata(false));

        public bool ShowFullPath
        {
            get
            {
                return (bool)GetValue(ShowFullPathProperty);
            }
            set
            {
                SetValue(ShowFullPathProperty, value);
            }
        }

        protected override void ExecuteCommand()
        {
            object[] parametrs = new object[3] { ImageInfo, Text, ShowFullPath };
            if (Command != null && Command.CanExecute(parametrs))
            {
                Command.Execute(parametrs);
                EditMode = TextBoxMode.ReadOnly;
            }
        }

        ICommand _renameCommand;
        public ICommand RenameCommand
        {
            get
            {
                return _renameCommand ?? (_renameCommand = new RelayCommand(arg =>
                {
                    if (arg != null)
                    {
                        object[] parametrs = arg as object[];
                        var iic = parametrs[0] as ImageInfoClass;
                        string text = parametrs[1] as string;
                        bool showFullPath = (bool)parametrs[2];

                        string newPath;
                        if (showFullPath)
                        {
                            newPath = text;
                        }
                        else
                        {
                            newPath = Path.Combine(Path.GetDirectoryName(iic.Path), text);
                        }

                        if (!File.Exists(newPath)
                            && Path.GetExtension(newPath) == Path.GetExtension(iic.Path))
                        {
                            IUCommand renameImageCommand = new RenameImageCommand(iic, newPath);
                            _undoRedoEngine.ExecuteCommand(renameImageCommand);
                        }

                        //_undoRedoEngine.R
                        /*string newFileName = iic.FileName;
                        if (!File.Exists(Path.Combine(Path.GetDirectoryName(iic.Path), newFileName))
                            && newFileName != Path.GetFileName(iic.Path)
                            && Path.GetExtension(newFileName) == Path.GetExtension(iic.Path))
                        {
                            IUCommand renameImageCommand = new RenameImageCommand(iic, newFileName);
                            _undoRedoEngine.ExecuteCommand(renameImageCommand);
                        }
                        else
                            iic.FileName = Path.GetFileName(iic.Path);*/
                    }

                }, arg => true));
            }
        }
    }
}
