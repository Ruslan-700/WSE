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
    public partial class Timeline : UserControl
    {
        public List<Call> calls = new List<Call>();
        public List<Marker> markers = new List<Marker>();

        //all time in microseconds

        private long _view_time_start = 0;
        private long _view_time_end = 0;

        private Rectangle canvas;

        public Timeline()
        {
            InitializeComponent();
        }

        private long duration
        {
            get { return calls.Count == 0 ? 0 : calls.Last().TimeStop; }
        }

        private long view_duration
        {
            get { return _view_time_end - _view_time_start; }
        }

        public void refresh()
        {
            _view_time_start = 0;
            _view_time_end = duration;
            this.Invalidate();
        }

        //time -> draw coord
        private int t2x(long t)
        {
            long x = (long)canvas.Width * (t - _view_time_start);

            x /= view_duration;

            return (int)x;
        }

        //draw coord -> time
        private long x2t(int x)
        {
            long t = view_duration;
            t *= x;
            t /= canvas.Width;
            t += _view_time_start;

            return t;
        }

        //time -> draw coord (fixed version)
        private int t2x_f(long t)
        {
            long x = canvas.Width * t;

            x /= duration;

            return (int)x;
        }

        private long clamp(long val, long min, long max)
        {
            val = Math.Min(val, max);
            val = Math.Max(val, min);
            return val;
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
        }

        //top axis
        void draw_fixed_time_axis(PaintEventArgs e)
        {
            Pen p1 = new Pen(Color.DarkGray, 2);
            Pen p2 = new Pen(Color.DarkGray, 1);

            int x;

            for (uint i = 0; i <= (duration / 1000000); i++)
            {
                x = t2x_f(i * 1000000);

                e.Graphics.DrawLine(p1, x, 15, x, 25);
                e.Graphics.DrawString(i.ToString() + "s", SystemFonts.DefaultFont, Brushes.Black, x - 7, 0);

                for (uint j = 1; j < 10; j++)
                {
                    x = t2x_f(i * 1000000 + j * 100000);
                    e.Graphics.DrawLine(p2, x, 15, x, 20);
                }
            }

            var c = Color.FromArgb(100, SystemColors.InactiveCaption);
            var b = new SolidBrush(c);

            x = t2x_f(_view_time_start);
            e.Graphics.FillRectangle(b, 0, 0, x, 25);
            e.Graphics.DrawLine(SystemPens.Highlight, x, 0, x, 25);

            x = t2x_f(_view_time_end);
            e.Graphics.FillRectangle(b, x, 0, canvas.Width - x, 25);
            e.Graphics.DrawLine(SystemPens.Highlight, x, 0, x, 25);
        }

        //bottom axis, dynamic
        void draw_time_axis(PaintEventArgs e)
        {
            long i = _view_time_start / 1000000;

            Pen p1 = new Pen(Color.DarkGray, 2);
            Pen p2 = new Pen(Color.DarkGray, 1);

            for (; i <= (_view_time_end / 1000000); i++)
            {
                var x = t2x(i * 1000000);

                e.Graphics.DrawLine(p1, x, 15, x, 25);
                e.Graphics.DrawString(i.ToString() + "s", SystemFonts.DefaultFont, Brushes.Black, x - 7, 0);

                for (uint j = 1; j < 10; j++)
                {
                    x = t2x(i * 1000000 + j * 100000);
                    e.Graphics.DrawLine(p2, x, 15, x, 20);
                }
            }
        }

        void draw_marker(PaintEventArgs e, Marker m)
        {
            if (m.time > _view_time_end || m.time < _view_time_start)
                return;

            int x1 = t2x(m.time);
            Pen p = new Pen(Color.LightGray, 1);
            float[] dash = { 10, 20 };
            p.DashPattern = dash;

            e.Graphics.DrawLine(p, x1, 30, x1, canvas.Height);
            return;
        }

        void draw_call(PaintEventArgs e, Call c, int depth = 0)
        {
            if (c.TimeStart > _view_time_end || c.TimeStop < _view_time_start)
                return;

            int x1 = t2x(c.TimeStart);
            int x2 = t2x(c.TimeStop);
            int w = Math.Max(x2 - x1, 1);

            int y1 = 40 + 20 * depth;

            Rectangle r = new Rectangle(x1, y1, w, 20);

            e.Graphics.DrawRectangle(Pens.Black, r);

            if (w > 40)
            {
                e.Graphics.DrawString(c.Id, DefaultFont, Brushes.Black, r);
            }

            foreach (var child in c.Children)
            {
                draw_call(e, child, depth + 1);
            }
        }

        private void PictureBox1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(Brushes.White, e.ClipRectangle);

            e.Graphics.TranslateTransform(7, 5);
            canvas = new Rectangle(0, 0, PictureBox1.Width - 14, PictureBox1.Height);

            if (calls.Count == 0)
                return;

            draw_fixed_time_axis(e);

            foreach (var marker in markers)
            {
                draw_marker(e, marker);
            }

            foreach (var call in calls)
            {
                draw_call(e, call);
            }
        }

        private void PictureBox1_Wheel(object sender, MouseEventArgs e)
        {
            if (e.Delta > 0 && view_duration < 10)
                return;

            long t = x2t(e.X);

            long max_amount = Math.Max(view_duration / 10, 1);

            long a = max_amount;
            a *= (t - _view_time_start);
            a /= view_duration;

            long b = max_amount;
            b *= (_view_time_end - t);
            b /= view_duration;

            a = Math.Max(a, 1);
            b = Math.Max(b, 1);

            if (e.Delta > 0)
            {
                _view_time_start += (uint)a;
                _view_time_end -= (uint)b;
            }
            else
            {
                _view_time_start -= (uint)a;
                _view_time_end += (uint)b;
            }

            _view_time_end = clamp(_view_time_end, 0, duration);
            _view_time_start = clamp(_view_time_start, 0, _view_time_end);
            
            PictureBox1.Invalidate();
        }

    }

    public class Marker
    {
        public uint time;

        public Marker(uint t)
        {
            time = t;
        }
    }
}
