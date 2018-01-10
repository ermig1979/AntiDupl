using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Threading;

namespace AntiDuplWPF.Control
{
    /// <summary>
    /// Represents a control that can be used for text search input.
    /// </summary>
    [TemplatePart(Name = "PART_ContentHost", Type = typeof(FrameworkElement))]
    [TemplatePart(Name = "PART_PromptHost", Type = typeof(FrameworkElement))]
    [TemplatePart(Name = "PART_ApplyButtonHost", Type = typeof(ButtonBase))]
    [TemplatePart(Name = "PART_ClearButtonHost", Type = typeof(ButtonBase))]
    public class RenameTextBox : TextBox, ICommandSource
    {
        /// <summary>
        /// Initializes the <see cref="RenameTextBox"/> class.
        /// </summary>
        static RenameTextBox()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(RenameTextBox),
                new FrameworkPropertyMetadata(typeof(RenameTextBox)));
            //TextProperty.OverrideMetadata(typeof(RenameTextBox), 
            //    new FrameworkPropertyMetadata(String.Empty, new PropertyChangedCallback(OnTextPropertyChanged)));
        }

        //private static readonly DependencyPropertyKey HasTextPropertyKey =
        //    DependencyProperty.RegisterReadOnly(
        //        "HasText",
        //        typeof(Boolean),
        //        typeof(RenameTextBox),
        //        new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.AffectsRender));

        /// <summary>
        /// Identifies the <see cref="HasText" /> dependency property.
        /// </summary>
        //public static readonly DependencyProperty HasTextProperty = HasTextPropertyKey.DependencyProperty;

        private static readonly DependencyPropertyKey IsTextChangedPropertyKey =
            DependencyProperty.RegisterReadOnly(
                "IsTextChanged",
                typeof(bool),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(false, FrameworkPropertyMetadataOptions.AffectsRender));

        public static readonly DependencyProperty IsTextChangedProperty = IsTextChangedPropertyKey.DependencyProperty;

        /// <summary>
        /// Identifies the <see cref="Command" /> dependency property.
        /// </summary>
        public static readonly DependencyProperty CommandProperty =
            DependencyProperty.Register(
                "Command",
                typeof(ICommand),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(
                    null,
                    new PropertyChangedCallback(OnCommandPropsChanged)));

        /// <summary>
        /// Identifies the <see cref="CommandParameter" /> dependency property.
        /// </summary>
        public static readonly DependencyProperty CommandParameterProperty =
            DependencyProperty.Register(
                "CommandParameter",
                typeof(Object),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(
                    null,
                    new PropertyChangedCallback(OnCommandPropsChanged)));

        /// <summary>
        /// Identifies the <see cref="CommandTarget" /> dependency property.
        /// </summary>
        public static readonly DependencyProperty CommandTargetProperty =
            DependencyProperty.Register(
                "CommandTarget",
                typeof(IInputElement),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(
                    null,
                    new PropertyChangedCallback(OnCommandPropsChanged)));



        /// <summary>
        /// Identifies the <see cref="SearchMode" /> dependency property.
        /// </summary>
        public static readonly DependencyProperty TextBoxModeProperty =
            DependencyProperty.Register(
                "EditMode",
                typeof(TextBoxMode),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(
                    TextBoxMode.ReadOnly,
                    FrameworkPropertyMetadataOptions.AffectsArrange | FrameworkPropertyMetadataOptions.AffectsMeasure));

        /// <summary>
        /// Identifies the <see cref="SearchMode" /> dependency property.
        /// </summary>
        public static readonly DependencyProperty EnableEditProperty =
            DependencyProperty.Register(
                "EnableEdit",
                typeof(bool),
                typeof(RenameTextBox),
                new FrameworkPropertyMetadata(
                    false,
                    FrameworkPropertyMetadataOptions.AffectsArrange | FrameworkPropertyMetadataOptions.AffectsMeasure));

        /// <summary>
        /// Identifies the <see cref="Search"/> routed event.
        /// </summary>
        [Category("Behavior")]
        public static readonly RoutedEvent ApplyEvent
            = EventManager.RegisterRoutedEvent(
                "Apply",
                RoutingStrategy.Bubble,
                typeof(RoutedEventHandler),
                typeof(RenameTextBox));


        private static void OnCommandPropsChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        {
            RenameTextBox stb = o as RenameTextBox;
            if (stb != null)
            {
                stb.UpdateSearchButtonCommand();
                stb.UpdateApplyButtonVisible();
            }
        }

        //public string OldText;

        //private static void OnTextPropertyChanged(DependencyObject o, DependencyPropertyChangedEventArgs e)
        //{
        //    RenameTextBox stb = o as RenameTextBox;
        //    if (stb != null)
        //    {
        //        if (String.IsNullOrEmpty(stb.OldText))
        //            stb.OldText = (string)e.NewValue;
        //        stb.UpdateRenameButtonIsEnabled();
        //    }
        //}

        //private FrameworkElement _contentHost;
        //private FrameworkElement _promptHost;
        private ButtonBase _applyButtonHost;
        private ButtonBase _clearButtonHost;

        /// <summary>
        /// Initializes a new instance of the <see cref="RenameTextBox"/> class.
        /// </summary>
        public RenameTextBox()
        {
            SubscriptionOnTextChanged = true;
        }


        /// <summary>
        /// Gets or sets the command to invoke when the search button is pressed or during instant search.
        /// </summary>
        [Bindable(true),
        Localizability(LocalizationCategory.NeverLocalize)]
        public ICommand Command
        {
            get { return (ICommand)this.GetValue(CommandProperty); }
            set { this.SetValue(CommandProperty, value); }
        }
        /// <summary>
        /// Gets or sets the parameter to pass to the <see cref="P:Command"/> property.
        /// </summary>
        [Bindable(true),
        Localizability(LocalizationCategory.NeverLocalize)]
        public object CommandParameter
        {
            get { return this.GetValue(CommandParameterProperty); }
            set { this.SetValue(CommandParameterProperty, value); }
        }

        /// <summary>
        /// Gets or sets the element on which to raise the specified command.
        /// </summary>
        [Bindable(true)]
        public IInputElement CommandTarget
        {
            get { return (IInputElement)this.GetValue(CommandTargetProperty); }
            set { this.SetValue(CommandTargetProperty, value); }
        }

        /// <summary>
        /// Gets a value indicating whether this control has text entered or not.
        /// </summary>
        //[Browsable(false)]
        //public bool HasText
        //{
        //    get { return (bool)GetValue(HasTextProperty); }
        //    private set { SetValue(HasTextPropertyKey, value); }
        //}

        public bool SubscriptionOnTextChanged { get; set; }

        [Browsable(false)]
        public bool IsTextChanged
        {
            get { return (bool)GetValue(IsTextChangedProperty); }
            private set { SetValue(IsTextChangedPropertyKey, value); }
        }

        /// <summary>
        /// Occurs when the search button is pressed or during instant search.
        /// </summary>
        public event RoutedEventHandler Search
        {
            add { AddHandler(ApplyEvent, value); }
            remove { RemoveHandler(ApplyEvent, value); }
        }

        [Category("Common Properties")]
        public TextBoxMode EditMode
        {
            get { return (TextBoxMode)GetValue(TextBoxModeProperty); }
            set { SetValue(TextBoxModeProperty, value); }
        }

        public bool EnableEdit
        {
            get { return (bool)GetValue(EnableEditProperty); }
            set { SetValue(EnableEditProperty, value); }
        }

        /// <summary>
        /// Is called when a control template is applied.
        /// </summary>
        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (_applyButtonHost != null)
            {
                _applyButtonHost.Click -= HandleApplyButtonClick;
            }
            if (_clearButtonHost != null)
            {
                _clearButtonHost.Click -= HandleClearButtonClick;
            }

            _applyButtonHost = GetTemplateChild("PART_ApplyButtonHost") as ButtonBase;
            _clearButtonHost = GetTemplateChild("PART_ClearButtonHost") as ButtonBase;

            if (_applyButtonHost != null)
            {
                _applyButtonHost.Click += HandleApplyButtonClick;
                UpdateSearchButtonCommand();
                UpdateApplyButtonVisible();
            }
            if (_clearButtonHost != null)
            {
                _clearButtonHost.Click += HandleClearButtonClick;
            }
        }

        /// <summary>
        /// Invoked whenever an unhandled <see cref="E:System.Windows.Input.Keyboard.KeyDown"/> attached routed event reaches an element derived from this class in its route. Implement this method to add class handling for this event.
        /// </summary>
        /// <param name="e">Provides data about the event.</param>
        protected override void OnKeyDown(KeyEventArgs e)
        {
            /*if (SearchMode == SearchTextBoxMode.Instant && e.Key == Key.Escape)
            {
                Text = String.Empty;

                e.Handled = true;
            }
            else */
            if (e.Key == Key.Return || e.Key == Key.Enter)
            {
                ChangeTextProperty();
                RaiseApplyEvent();
                ExecuteCommand();

                e.Handled = true;
            }
            else
            {
                base.OnKeyDown(e);
            }
        }

        private void ChangeTextProperty()
        {
            var textBox = this as TextBox;
            if (textBox != null)
            {
                var bindingExpression = textBox.GetBindingExpression(TextBox.TextProperty);
                if (bindingExpression != null)
                    bindingExpression.UpdateSource();
            }
        }

        /// <summary>
        /// Is called when content in this editing control changes.
        /// </summary>
        /// <param name="e">The arguments that are associated with the <see cref="E:System.Windows.Controls.Primitives.TextBoxBase.TextChanged"/> event.</param>
        protected override void OnTextChanged(TextChangedEventArgs e)
        {
            base.OnTextChanged(e);

            //HasText = !String.IsNullOrEmpty(Text);
            if (SubscriptionOnTextChanged)
            {
                IsTextChanged = !String.IsNullOrEmpty(Text) && !String.IsNullOrEmpty(ProperValue)
                    && ProperValue != Text;

                UpdateApplyButtonVisible();
            }
        }

        private string ProperValue = String.Empty;

        protected override void OnGotFocus(RoutedEventArgs e)
        {
            base.OnGotFocus(e);

            ProperValue = Text;
        }


        protected override void OnLostFocus(RoutedEventArgs e)
        {
            base.OnLostFocus(e);

            EditMode = TextBoxMode.ReadOnly;

            // Text 
            //var textBox = this as TextBox;
            //if (textBox == null) 
            //   return;

            Text = ProperValue;

            /*var bindingExpression = textBox.GetBindingExpression(TextBox.TextProperty);
            if (bindingExpression != null)
            {
                //    bindingExpression.UpdateSource();
                //var sou = (string)bindingExpression.ResolvedSource;
                var da1 = (string)bindingExpression.DataItem;
            }*/
        }

        protected override void OnMouseDoubleClick(MouseButtonEventArgs e)
        {
            base.OnMouseDoubleClick(e);

            if (EnableEdit)
                EditMode = TextBoxMode.Edit;
        }

        /// <summary>
        /// Raises the <see cref="E:Search"/> event.
        /// </summary>
        /// <param name="e">The <see cref="System.Windows.RoutedEventArgs"/> instance containing the event data.</param>
        /// <exception cref="ArgumentException">
        /// Thrown when <paramref name="e"/> relates to an event other than <see cref="ApplyEvent"/>.
        /// </exception>
        protected void OnApply(RoutedEventArgs e)
        {
            if (e.RoutedEvent != ApplyEvent)
                throw new ArgumentException("Only RoutedEventArgs for the SearchEvent may be passed in.", "e");
            RaiseEvent(e);
        }

        private void HandleCommandCanExecuteChanged(object sender, EventArgs e)
        {
            UpdateApplyButtonVisible();
        }

        private void HandleApplyButtonClick(object sender, RoutedEventArgs e)
        {
            ChangeTextProperty();
            RaiseApplyEvent();
            ExecuteCommand();
        }

        private void HandleClearButtonClick(object sender, RoutedEventArgs e)
        {
            Text = ProperValue;
            EditMode = TextBoxMode.ReadOnly;
        }

        private void RaiseApplyEvent()
        {
            //if (AllowEmptySearches || HasText)
            OnApply(new RoutedEventArgs(ApplyEvent));
        }

        protected virtual void ExecuteCommand()
        {
            //if ((AllowEmptySearches || HasText) && Command != null && Command.CanExecute(CommandParameter))
            if (Command != null && Command.CanExecute(CommandParameter))
            {
                Command.Execute(CommandParameter);
                EditMode = TextBoxMode.ReadOnly;
            }
        }

        private void UpdateSearchButtonCommand()
        {
            if (_applyButtonHost != null)
            {
                if (_applyButtonHost.Command != null)
                {
                    _applyButtonHost.Command.CanExecuteChanged -= HandleCommandCanExecuteChanged;
                }

                _applyButtonHost.Command = Command;
                _applyButtonHost.CommandParameter = CommandParameter;
                _applyButtonHost.CommandTarget = CommandTarget;

                if (_applyButtonHost.Command != null)
                {
                    _applyButtonHost.Command.CanExecuteChanged += HandleCommandCanExecuteChanged;
                }
            }
        }

        private void UpdateApplyButtonVisible()
        {
            if (_applyButtonHost != null)
            {
                /*if (Command == null)
                {
                    _applyButtonHost.IsEnabled = IsTextChanged;
                }
                else
                {
                    _applyButtonHost.IsEnabled = IsTextChanged ? Command.CanExecute(CommandParameter) : false;
                }*/

                if (Command == null)
                {
                    _applyButtonHost.Visibility = IsTextChanged ? System.Windows.Visibility.Visible : System.Windows.Visibility.Collapsed;
                }
                else
                {
                    // _applyButtonHost.Visibility = IsTextChanged ? Command.CanExecute(CommandParameter) : false;
                    if (IsTextChanged && Command.CanExecute(CommandParameter))
                        _applyButtonHost.Visibility = System.Windows.Visibility.Visible;
                    else
                        _applyButtonHost.Visibility = System.Windows.Visibility.Collapsed;
                }
            }
        }
    }

    public enum TextBoxMode
    {
        ReadOnly,
        Edit
    }
}
