using System;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media.Animation;
using System.Windows;
using System.Windows.Media;

namespace AntiDuplWPF.Control
{
    public class PanAndZoomViewer : ContentControl
    {
        public double DefaultZoomFactor { get; set; }
        private FrameworkElement source;
        private Point ScreenStartPoint = new Point(0, 0);
        private TranslateTransform translateTransform;
        private ScaleTransform zoomTransform;
        private TransformGroup transformGroup;
        private Point startOffset;

        public PanAndZoomViewer()
        {
            this.DefaultZoomFactor = 1.4;
        }

        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();
            Setup(this);
        }

        void Setup(FrameworkElement control)
        {
            this.source = VisualTreeHelper.GetChild(this, 0) as FrameworkElement;

            this.translateTransform = new TranslateTransform();
            this.zoomTransform = new ScaleTransform();
            this.transformGroup = new TransformGroup();
            this.transformGroup.Children.Add(this.zoomTransform);
            this.transformGroup.Children.Add(this.translateTransform);
            this.source.RenderTransform = this.transformGroup;
            this.Focusable = true;
            this.KeyDown += new KeyEventHandler(source_KeyDown);
            this.MouseMove += new MouseEventHandler(control_MouseMove);
            this.MouseDown += new MouseButtonEventHandler(source_MouseDown);
            this.MouseUp += new MouseButtonEventHandler(source_MouseUp);
            this.MouseWheel += new MouseWheelEventHandler(source_MouseWheel);
        }

        void source_KeyDown(object sender, KeyEventArgs e)
        {
            // hit escape to reset everything
            if (e.Key == Key.Escape) Reset();
        }

        void source_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            // zoom into the content.  Calculate the zoom factor based on the direction of the mouse wheel.
            double zoomFactor = this.DefaultZoomFactor;
            if (e.Delta <= 0) zoomFactor = 1.0 / this.DefaultZoomFactor;
            // DoZoom requires both the logical and physical location of the mouse pointer
            var physicalPoint = e.GetPosition(this);
            DoZoom(zoomFactor, this.transformGroup.Inverse.Transform(physicalPoint), physicalPoint);
        }

        void source_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (this.IsMouseCaptured)
            {
                // we're done.  reset the cursor and release the mouse pointer
                this.Cursor = Cursors.Arrow;
                this.ReleaseMouseCapture();
            }
        }

        void source_MouseDown(object sender, MouseButtonEventArgs e)
        {
            // Save starting point, used later when determining how much to scroll.
            this.ScreenStartPoint = e.GetPosition(this);
            this.startOffset = new Point(this.translateTransform.X, this.translateTransform.Y);
            this.CaptureMouse();
            this.Cursor = Cursors.ScrollAll;
        }


        void control_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.IsMouseCaptured)
            {
                // if the mouse is captured then move the content by changing the translate transform.  
                // use the Pan Animation to animate to the new location based on the delta between the 
                // starting point of the mouse and the current point.
                var physicalPoint = e.GetPosition(this);
                this.translateTransform.BeginAnimation(TranslateTransform.XProperty, CreatePanAnimation(physicalPoint.X - this.ScreenStartPoint.X + this.startOffset.X), HandoffBehavior.Compose);
                this.translateTransform.BeginAnimation(TranslateTransform.YProperty, CreatePanAnimation(physicalPoint.Y - this.ScreenStartPoint.Y + this.startOffset.Y), HandoffBehavior.Compose);
            }
        }


        /// <summary>Helper to create the panning animation for x,y coordinates.</summary>
        /// <param name="toValue">New value of the coordinate.</param>
        /// <returns>Double animation</returns>
        private DoubleAnimation CreatePanAnimation(double toValue)
        {
            var da = new DoubleAnimation(toValue, new Duration(TimeSpan.FromMilliseconds(300)));
            da.AccelerationRatio = 0.1;
            da.DecelerationRatio = 0.9;
            da.FillBehavior = FillBehavior.HoldEnd;
            da.Freeze();
            return da;
        }


        /// <summary>Helper to create the zoom double animation for scaling.</summary>
        /// <param name="toValue">Value to animate to.</param>
        /// <returns>Double animation.</returns>
        private DoubleAnimation CreateZoomAnimation(double toValue)
        {
            var da = new DoubleAnimation(toValue, new Duration(TimeSpan.FromMilliseconds(500)));
            da.AccelerationRatio = 0.1;
            da.DecelerationRatio = 0.9;
            da.FillBehavior = FillBehavior.HoldEnd;
            da.Freeze();
            return da;
        }

        /// <summary>Zoom into or out of the content.</summary>
        /// <param name="deltaZoom">Factor to mutliply the zoom level by. </param>
        /// <param name="mousePosition">Logical mouse position relative to the original content.</param>
        /// <param name="physicalPosition">Actual mouse position on the screen (relative to the parent window)</param>
        public void DoZoom(double deltaZoom, Point mousePosition, Point physicalPosition)
        {
            double currentZoom = this.zoomTransform.ScaleX;
            currentZoom *= deltaZoom;
            this.translateTransform.BeginAnimation(TranslateTransform.XProperty, CreateZoomAnimation(-1 * (mousePosition.X * currentZoom - physicalPosition.X)));
            this.translateTransform.BeginAnimation(TranslateTransform.YProperty, CreateZoomAnimation(-1 * (mousePosition.Y * currentZoom - physicalPosition.Y)));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleXProperty, CreateZoomAnimation(currentZoom));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleYProperty, CreateZoomAnimation(currentZoom));
        }
        /// <summary>Zoom into or out of the content by percent.</summary>
        /// <param name="deltaZoom">Percent to set zoom too.</param>
        /// <param name="mousePosition">Logical mouse position relative to the original content.</param>
        /// <param name="physicalPosition">Actual mouse position on the screen (relative to the parent window)</param>
        public void DoZoomPercent(double percentZoom, Point mousePosition, Point physicalPosition)
        {
            this.translateTransform.BeginAnimation(TranslateTransform.XProperty, CreateZoomAnimation(-1 * (mousePosition.X * percentZoom - physicalPosition.X)));
            this.translateTransform.BeginAnimation(TranslateTransform.YProperty, CreateZoomAnimation(-1 * (mousePosition.Y * percentZoom - physicalPosition.Y)));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleXProperty, CreateZoomAnimation(percentZoom));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleYProperty, CreateZoomAnimation(percentZoom));
        }

        /// <summary>Reset to default zoom level and centered content.</summary>
        public void Reset()
        {
            this.translateTransform.BeginAnimation(TranslateTransform.XProperty, CreateZoomAnimation(0));
            this.translateTransform.BeginAnimation(TranslateTransform.YProperty, CreateZoomAnimation(0));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleXProperty, CreateZoomAnimation(1));
            this.zoomTransform.BeginAnimation(ScaleTransform.ScaleYProperty, CreateZoomAnimation(1));
        }
    }
}
