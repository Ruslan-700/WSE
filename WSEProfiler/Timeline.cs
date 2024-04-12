using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WSEProfiler
{
    public partial class Timeline : UserControl
    {
        public List<Call> calls = new List<Call>();
        public List<Marker> markers = new List<Marker>();
        public List<Marker> search_markers = new List<Marker>();

        //all time in microseconds

        private long _view_time_start = 0;
        private long _view_time_end = 0;

        private Point origin_offset = new Point(7, 5);
        private Rectangle _canvas;

        bool dragging = false;
        int drag_last_x;

        bool selecting = false;
        private long _select_time_a = -1;
        private long _select_time_b = -1;

        private Action draw_hover_info; //Callback for drawing tooltip last

        //Cache some drawing stuff
        private Pen _marker_frame_pen = new Pen(Color.LightGray, 1);
        private Pen _marker_search_pen = new Pen(Color.DarkRed, 1);

        private Pen _call_border_pen = Pens.Black;
        private Font _call_label_font = DefaultFont;
        private Brush _call_brush = Brushes.Black;


        //For searchfiled placeholder
        private const int EM_SETCUEBANNER = 0x1501;
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        private static extern Int32 SendMessage(IntPtr hWnd, int msg, int wParam, [MarshalAs(UnmanagedType.LPWStr)]string lParam);


        public Timeline()
        {
            InitializeComponent();
            SendMessage(textBox1.Handle, EM_SETCUEBANNER, 0, "Search");

            float[] dash = { 10, 20 };
            _marker_frame_pen.DashPattern = dash;
        }

        public long duration
        {
            get { return Ready ? calls.Last().TimeStop : 0; }
        }

        public long view_duration
        {
            get { return _view_time_end - _view_time_start; }
        }

        public bool is_zoomed_out
        {
            get { return _view_time_start == 0 && _view_time_end == duration; }
        }

        public bool Ready
        {
            get { return calls.Count != 0; }
        }

        public void refresh()
        {
            _view_time_start = 0;
            _view_time_end = duration;
            PictureBox1.Invalidate();
        }

        public void reset()
        {
            _view_time_start = 0;
            _view_time_end = 0;
            dragging = false;
            selecting = false;
            calls.Clear();
            markers.Clear();
            PictureBox1.Invalidate();
        }

        //time -> draw coord
        private int t2x(long t)
        {
            long x = (long)_canvas.Width * (t - _view_time_start);

            x /= view_duration;

            return (int)x;
        }

        //draw coord -> time
        private long x2t(int x)
        {
            long t = view_duration;
            t *= x;
            t /= _canvas.Width;
            t += _view_time_start;

            return t;
        }

        //time -> draw coord (fixed version)
        private int t2x_f(long t)
        {
            long x = _canvas.Width * t;

            x /= duration;

            return (int)x;
        }

        private Point mouse2canvas(Point location)
        {
            return new Point(location.X - origin_offset.X, location.Y - origin_offset.Y);
        }

        private MouseEventArgs mouseEvent2Canvas(MouseEventArgs e)
        {
            return new MouseEventArgs(e.Button, e.Clicks, e.Location.X - origin_offset.X, e.Location.Y - origin_offset.Y, e.Delta);
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

            uint seconds = (uint)(duration / 1000000);
            uint step = seconds / 15;

            for (uint i = 0; i <= seconds; i += step)
            {
                x = t2x_f(i * 1000000);

                e.Graphics.DrawLine(p1, x, 15, x, 25);
                e.Graphics.DrawString(i.ToString() + "s", SystemFonts.DefaultFont, Brushes.Black, x - 7, 0);

                if (step == 1)
                {
                    for (uint j = 1; j < 10; j++)
                    {
                        x = t2x_f(i * 1000000 + j * 100000);
                        e.Graphics.DrawLine(p2, x, 15, x, 20);
                    }
                }
            }

            var c = Color.FromArgb(100, SystemColors.InactiveCaption);
            var b = new SolidBrush(c);

            x = t2x_f(_view_time_start);
            e.Graphics.FillRectangle(b, 0, 0, x, 25);
            e.Graphics.DrawLine(SystemPens.Highlight, x, 0, x, 25);

            x = t2x_f(_view_time_end);
            e.Graphics.FillRectangle(b, x, 0, _canvas.Width - x, 25);
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

            //get nearest multiple
            long i = _view_time_start / unit / step;
            i *= step;

            while (i < _view_time_end / unit)
            {
                var x = t2x(i * unit);

                e.Graphics.DrawLine(p1, x, _canvas.Height - 20, x, _canvas.Height - 30);
                e.Graphics.DrawString(i.ToString() + ustr, SystemFonts.DefaultFont, Brushes.Black, x - 7, _canvas.Height-20);

                i += step;
            }
        }

        void draw_marker(PaintEventArgs e, Marker m)
        {
            if (m.time > _view_time_end || m.time < _view_time_start)
                return;

            int x1 = t2x(m.time);

            if (m.type == Marker.Marker_Type.Frame)
            {
                e.Graphics.DrawLine(_marker_frame_pen, x1, 30, x1, _canvas.Height - 30);
            }
            else
            {
                e.Graphics.DrawLine(_marker_search_pen, x1, 20, x1, 150);
            }
        }

        void draw_call(PaintEventArgs e, Call c, int depth = 0)
        {
            if (c.TimeStart > _view_time_end || c.TimeStop < _view_time_start)
                return;

            int x1 = t2x(c.TimeStart);
            int x2 = t2x(c.TimeStop);
            int w = x2 - x1;

            int y1 = 40 + 20 * depth;

            if(w <= 1)
            {
                e.Graphics.DrawLine(c.Timeline_Pen, x1, y1, x1, y1 + 20);
                return;
            }

            Rectangle r = new Rectangle(x1, y1, w, 20);

            e.Graphics.FillRectangle(c.Timeline_Brush, r);

            if (w > 5)
            {
                e.Graphics.DrawRectangle(_call_border_pen, r);
            }
            if (w > 40)
            {
                //Font f = new Font(SystemFonts.SmallCaptionFont.Name, 8);
                //e.Graphics.DrawString(c.Id, f, Brushes.Black, r);
                e.Graphics.DrawString(c.Id, _call_label_font, _call_brush, r, new StringFormat(StringFormatFlags.NoWrap));
            }

            Point p = mouse2canvas(PictureBox1.PointToClient(System.Windows.Forms.Control.MousePosition));
            if (r.Contains(p))
            {
                draw_hover_info = delegate()
                {
                    var x = p.X + 5;
                    var y = p.Y + 5;

                    var text = string.Format("{0}\nTime total: {1}\nTime self: {2}", c.Id, c.TimeTotal.FormatTime(), c.Time.FormatTime());

                    SizeF size = e.Graphics.MeasureString(text, DefaultFont);

                    e.Graphics.FillRectangle(SystemBrushes.Control, x, y, size.Width + 10, size.Height + 10);

                    x += 5;
                    y += 5;
                    e.Graphics.DrawString(text, DefaultFont, Brushes.Black, x, y);
                };
            };

            foreach (var child in c.Children)
            {
                draw_call(e, child, depth + 1);
            }
        }

        void create_search_markers(String text)
        {
            search_markers.Clear();

            if (text != "")
            {
                text = text.ToLower();

                foreach (var c in calls)
                {
                    if (c.Id.ToLower().Contains(text))
                    {
                        search_markers.Add(new Marker(c.TimeStart, Marker.Marker_Type.Search));
                    }
                }
            }
        }

        void drag_timeline(long delta_t)
        {
            if (delta_t < 0)
            {
                if (_view_time_start + delta_t < 0)
                {
                    _view_time_end -= _view_time_start;
                    _view_time_start = 0;
                }
                else
                {
                    _view_time_start += delta_t;
                    _view_time_end += delta_t;
                }
            }
            else
            {
                if (_view_time_end + delta_t > duration)
                {
                    _view_time_start += (duration - _view_time_end);
                    _view_time_end = duration;
                }
                else
                {
                    _view_time_start += delta_t;
                    _view_time_end += delta_t;
                }
            }
        }


        //###########
        //Events
        //###########
        private void PictureBox1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.FillRectangle(Brushes.White, e.ClipRectangle);

            //We want some "padding", otherwise everything draws right at the edge
            e.Graphics.TranslateTransform(origin_offset.X, origin_offset.Y);
            _canvas = new Rectangle(0, 0, PictureBox1.Width - 14, PictureBox1.Height);

            if (!Ready)
                return;

            draw_fixed_time_axis(e);
            draw_time_axis(e);

            draw_hover_info = null;

            foreach (var marker in markers)
            {
                draw_marker(e, marker);
            }

            foreach (var call in calls)
            {
                draw_call(e, call);
            }

            foreach (var marker in search_markers)
            {
                draw_marker(e, marker);
            }

            if (selecting && _select_time_b > -1)
            {
                long start = Math.Min(_select_time_a, _select_time_b);
                long end = Math.Max(_select_time_a, _select_time_b);

                var c = Color.FromArgb(20, SystemColors.Highlight);
                var b = new SolidBrush(c);

                int x = t2x(start);
                int w = t2x(end) - x;

                Rectangle r = new Rectangle(x, 30, w, _canvas.Height - 30);

                e.Graphics.FillRectangle(b, r);

                StringFormat format = new StringFormat();
                format.LineAlignment = StringAlignment.Center;
                format.Alignment = StringAlignment.Center;

                Point p = new Point(r.X + r.Width / 2, r.Y + r.Height / 2);

                float d = (float)(end - start);
                e.Graphics.DrawString(d.FormatTime(), DefaultFont, Brushes.Black, p, format);
            }
            else
            {
                if (draw_hover_info != null)
                {
                    draw_hover_info();
                }
            }
        }

        private void PictureBox1_Wheel(object sender, MouseEventArgs e)
        {
            if (!Ready)
                return;

            e = mouseEvent2Canvas(e);

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
            if (!Ready)
                return;

            e = mouseEvent2Canvas(e);

            if (e.Button == MouseButtons.Right && selecting)
            {
                _select_time_a = -1;
                _select_time_b = -1;
                MouseEnd();
                return;
            }

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
            if (!Ready)
                return;

            e = mouseEvent2Canvas(e);

            if (dragging)
            {
                long dx = x2t(drag_last_x) - x2t(e.Location.X);
                drag_last_x = e.Location.X;

                drag_timeline(dx);

                //this.PictureBox1.Invalidate();
            }

            if (selecting)
            {
                _select_time_b = clamp(x2t(e.Location.X), 0, duration);
                //this.PictureBox1.Invalidate();
            }
            this.PictureBox1.Invalidate();
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
                }
                if (_select_time_a > _select_time_b)
                {
                    _view_time_start = _select_time_b;
                    _view_time_end = _select_time_a;
                }
                PictureBox1.Invalidate();
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

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Enter)
            {
                create_search_markers(textBox1.Text);
            }
        }

        //mousewheel "tilt" (click mousewheel left/right)
        protected override void WndProc(ref Message m)
        {
            base.WndProc(ref m);

            if (!Ready)
                return;

            if (m.HWnd != this.Handle)
            {
                return;
            }
            switch (m.Msg)
            {
                case 0x020E: //WM_MOUSEHWHEEL 
                    Int32 tilt = m.WParam.ToInt32() >> 16;

                    if (tilt < 0)
                    {
                        drag_timeline(-Math.Max(view_duration / 10, 1));
                    }
                    else
                    {
                        drag_timeline(Math.Max(view_duration / 10, 1));
                    }

                    m.Result = (IntPtr)1;
                    break;
                default:
                    break;
            }
        }
    }

    public class Marker
    {
        public enum Marker_Type
        {
            Frame,
            Search
        }

        public uint time;
        public Marker_Type type;

        public Marker(uint t, Marker_Type ty)
        {
            time = t;
            type = ty;
        }
    }
}
