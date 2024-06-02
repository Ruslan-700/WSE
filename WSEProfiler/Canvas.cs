using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
    //this is just to abstract drawing with a whitespace padding so everything isnt right at the edge
    public partial class Canvas : PictureBox
    {
        private Padding _padding;

        public Canvas()
        {
            InitializeComponent();
        }

        [Browsable(true)]
        public Padding DrawPadding
        {
            get { return _padding; }
            set { _padding = value; }
        }

        public int DrawWidth  { get { return this.Width - _padding.Left - _padding.Right; } }
        public int DrawHeight { get { return this.Height - _padding.Top - _padding.Bottom; } }

        public Point PointToCanvas(Point p)
        {
            p = this.PointToClient(p);
            return new Point(p.X - _padding.Left, p.Y - _padding.Top);
        }



        protected override void OnPaint(PaintEventArgs pe)
        {
            pe.Graphics.FillRectangle(Brushes.White, pe.ClipRectangle);
            pe.Graphics.TranslateTransform(_padding.Left, _padding.Top);
            base.OnPaint(pe);
        }

        private MouseEventArgs mouseEvent2Canvas(MouseEventArgs e)
        {
            return new MouseEventArgs(e.Button, e.Clicks, e.Location.X - _padding.Left, e.Location.Y - _padding.Top, e.Delta);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(mouseEvent2Canvas(e));
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(mouseEvent2Canvas(e));
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            base.OnMouseWheel(mouseEvent2Canvas(e));
        }
    }
}
