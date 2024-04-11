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

        bool dragging = false;
        int drag_last_x;

        bool selecting = false;
        private long _select_time_a = -1;
        private long _select_time_b = -1;

        public Timeline()
        {
            InitializeComponent();
        }

        private long duration
        {
            get { return calls.Count == 0 ? 0 : calls.Last().TimeStop; }
        }

        public long view_duration
        {
            get { return _view_time_end - _view_time_start; }
        }

        public bool is_zoomed_out
        {
            get { return _view_time_start == 0 && _view_time_end == duration; }
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
            long unit = 1000;
            string ustr = "ms";
            long steps = 6;

            Pen p1 = new Pen(Color.DarkGray, 2);
            Pen p2 = new Pen(Color.DarkGray, 1);

            long a = _view_time_start / unit;
            long b = _view_time_end / unit;
            long d = b - a;


            //find the best suited of 2, 5, or 10 (*10^n)
            long step = d / steps;

            long best_delta = Math.Abs(step - 2);
            long best_step = 2;

            long[] facs = {2,5,10};

            foreach(long fac in facs)
            {
                long fac2 = fac;
                do
                {
                    long delta = Math.Abs(step - fac2);
                    if (delta < best_delta)
                    {
                        best_delta = delta;
                        best_step = fac2;
                    }
                    fac2 *= 10;
                } while(fac2 < step);
            }
            step = best_step;

            Console.WriteLine("step "+step.ToString());

            //get nearest multiple
            long i = _view_time_start / unit / step;
            i *= step;

            while (i < _view_time_end / unit)
            {
                var x = t2x(i * unit);

                e.Graphics.DrawLine(p1, x, canvas.Height - 20, x, canvas.Height - 30);
                e.Graphics.DrawString(i.ToString() + ustr, SystemFonts.DefaultFont, Brushes.Black, x - 7, canvas.Height-20);

                i += step;
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

            e.Graphics.DrawLine(p, x1, 30, x1, canvas.Height - 30);
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

            if(w == 1)
            {
                e.Graphics.DrawLine(Pens.Black, x1, y1, x1, y1 + 20);
                return;
            }

            Rectangle r = new Rectangle(x1, y1, w, 20);

            e.Graphics.FillRectangle(c.Timeline_Brush, r);

            if (w > 40)
            {
                e.Graphics.DrawRectangle(Pens.Black, r);
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
            draw_time_axis(e);

            foreach (var marker in markers)
            {
                draw_marker(e, marker);
            }

            foreach (var call in calls)
            {
                draw_call(e, call);
            }

            if (selecting && _select_time_b > -1)
            {
                var c = Color.FromArgb(20, SystemColors.Highlight);
                var b = new SolidBrush(c);

                int x = t2x(_select_time_a);
                int w = t2x(_select_time_b) - x;

                e.Graphics.FillRectangle(b, x, 30, w, canvas.Height - 30);
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

        private void PictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (is_zoomed_out || Control.ModifierKeys == Keys.Shift)
            {
                selecting = true;
                _select_time_a = clamp(x2t(e.Location.X), 0, duration);
                _select_time_b = -1;
                PictureBox1.Cursor = Cursors.IBeam;
            }
            else
            {
                dragging = true;
                drag_last_x = e.Location.X;
                PictureBox1.Cursor = Cursors.Hand;
            }
        }

        private void PictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (dragging)
            {
                long dx = x2t(drag_last_x) - x2t(e.Location.X);
                drag_last_x = e.Location.X;

                if (dx < 0)
                {
                    if (_view_time_start + dx < 0)
                    {
                        _view_time_end -= _view_time_start;
                        _view_time_start = 0;
                    }
                    else
                    {
                        _view_time_start += dx;
                        _view_time_end += dx;
                    }
                }
                else
                {
                    if (_view_time_end + dx > duration)
                    {
                        _view_time_start += (duration - _view_time_end);
                        _view_time_end = duration;
                    }
                    else
                    {
                        _view_time_start += dx;
                        _view_time_end += dx;
                    }
                }

                this.PictureBox1.Invalidate();
            }

            if (selecting)
            {
                _select_time_b = clamp(x2t(e.Location.X), 0, duration);
                this.PictureBox1.Invalidate();
            }
        }

        private void MouseEnd()
        {
            if (dragging)
            {
                dragging = false;
                PictureBox1.Cursor = Cursors.Default;
            }
            if (selecting)
            {
                selecting = false;
                PictureBox1.Cursor = Cursors.Default;

                if (_select_time_a < _select_time_b)
                {
                    _view_time_start = _select_time_a;
                    _view_time_end = _select_time_b;
                    PictureBox1.Invalidate();
                }
                if (_select_time_a > _select_time_b)
                {
                    _view_time_start = _select_time_b;
                    _view_time_end = _select_time_a;
                    PictureBox1.Invalidate();
                }
            }
        }

        private void PictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            MouseEnd();
        }

        private void PictureBox1_MouseLeave(object sender, EventArgs e)
        {
            MouseEnd();
        }

        private void button_zoom_Click(object sender, EventArgs e)
        {
            _view_time_start = 0;
            _view_time_end = duration;
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
