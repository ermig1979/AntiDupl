using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Interactivity;

namespace AntiDuplWPF.Behavior
{
	public class MouseEnterBehaviour : Behavior<FrameworkElement>
	{
		public static readonly DependencyProperty MouseEnterCommandProperty =
			DependencyProperty.Register("MouseEnterCommand",
			typeof(ICommand),
			typeof(MouseEnterBehaviour),
			new UIPropertyMetadata(null));

		public ICommand MouseEnterCommand
		{
			get { return (ICommand)GetValue(MouseEnterCommandProperty); }
			set { SetValue(MouseEnterCommandProperty, value); }
		}

		public static readonly DependencyProperty MouseLeaveCommandProperty =
			DependencyProperty.Register("MouseLeaveCommand",
			typeof(ICommand),
			typeof(MouseEnterBehaviour),
			new UIPropertyMetadata(null));

		public ICommand MouseLeaveCommand
		{
			get { return (ICommand)GetValue(MouseLeaveCommandProperty); }
			set { SetValue(MouseLeaveCommandProperty, value); }
		}

		protected override void OnAttached()
		{
			base.OnAttached();

			AssociatedObject.MouseEnter += AssociatedObject_MouseEnter;
			AssociatedObject.MouseLeave += AssociatedObject_MouseLeave;
		}

		protected override void OnDetaching()
		{
			base.OnDetaching();

			AssociatedObject.MouseEnter -= AssociatedObject_MouseEnter;
			AssociatedObject.MouseLeave -= AssociatedObject_MouseLeave;
		}

		void AssociatedObject_MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
		{
			object[] parametrs = new object[0];
			if (MouseEnterCommand != null && MouseEnterCommand.CanExecute(parametrs))
				MouseEnterCommand.Execute(parametrs);
		}


		void AssociatedObject_MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
		{
			object[] parametrs = new object[0];
			if (MouseLeaveCommand != null && MouseLeaveCommand.CanExecute(parametrs))
				MouseLeaveCommand.Execute(parametrs);
		}

		
	}
}
