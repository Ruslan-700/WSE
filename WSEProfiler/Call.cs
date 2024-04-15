using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace WSEProfiler
{
    public class Call
    {
        private string _id;
        private float _time;
        private float _time_childs = -1;
        private uint _time_start;
        private uint _time_stop;
        private Call _parent;
        private List<Call> _children = new List<Call>();

        private Brush _timeline_brush;
        private Pen _timeline_pen;
        public bool timeline_hidden = false; //filtering

        public Call(string id)
        {
            _id = id;
            figure_out_color();
        }

        public Call(string id, Call parent)
        {
            _id = id;
            _parent = parent;
            figure_out_color();
        }

        private float CalculateChildTime()
        {
            if (_time_childs == -1)
            {
                _time_childs = 0;

                foreach (var child in _children)
                {
                    _time_childs += child.TimeTotal;
                }
            }

            return _time_childs;
        }

        public override string ToString()
        {
            return _id;
        }

        public string Id
        {
            get { return _id; }
        }

        public float Time
        {
            get { return _time; }
            set { _time = value; }
        }

        public uint TimeStart
        {
            get { return _time_start; }
            set { _time_start = value; }
        }

        public uint TimeStop
        {
            get { return _time_stop; }
            set { _time_stop = value; }
        }

        public float TimeTotal
        {
            get { return _time + CalculateChildTime(); }
        }

        public float TimeChilds
        {
            get { return CalculateChildTime(); }
        }

        public Call Parent
        {
            get { return _parent; }
        }

        public List<Call> Children
        {
            get { return _children; }
        }

        public enum Kind
        {
            mst_cond,
            mst_cons,
            script,
            prop,
            item,
            engine,
            other
        }

        public Kind kind
        {
            get
            {
                if (_id.StartsWith("Mission Template ["))
                {
                    if (_id.EndsWith("Conditions"))
                    {
                        return Kind.mst_cond;
                    }
                    else
                    {
                        return Kind.mst_cons;
                    }
                }
                else if (_id.StartsWith("Script ["))
                {
                    return Kind.script;
                }
                else if (_id.StartsWith("Scene Prop"))
                {
                    return Kind.prop;
                }
                else if (_id.StartsWith("Item"))
                {
                    return Kind.item;
                }
                else if (_id == ("Engine"))
                {
                    return Kind.engine;
                }
                else
                {
                    return Kind.other;
                }
            }
        }

        private void figure_out_color()
        {
            if (kind == Kind.mst_cond)
            {
                _timeline_brush = Brushes.SkyBlue;
                _timeline_pen = Pens.SkyBlue;
            }
            else if (kind == Kind.mst_cons)
            {
                _timeline_brush = Brushes.SteelBlue;
                _timeline_pen = Pens.SteelBlue;
            }
            else if (kind == Kind.script)
            {
                _timeline_brush = Brushes.BurlyWood;
                _timeline_pen = Pens.BurlyWood;
            }
            else if (kind == Kind.prop)
            {
                _timeline_brush = Brushes.DarkKhaki;
                _timeline_pen = Pens.DarkKhaki;
            }
            else if (kind == Kind.item)
            {
                _timeline_brush = Brushes.LightSeaGreen;
                _timeline_pen = Pens.LightSeaGreen;
            }
            else if (kind == Kind.engine)
            {
                _timeline_brush = Brushes.Azure;
                _timeline_pen = Pens.Azure;
            }
            else
            {
                _timeline_brush = Brushes.DarkRed;
                _timeline_pen = Pens.DarkRed;
            }
        }

        public Brush Timeline_Brush
        {
            get { return _timeline_brush; }
            set { _timeline_brush = value; }
        }
        public Pen Timeline_Pen
        {
            get { return _timeline_pen; }
            set { _timeline_pen = value; }
        }
    }
}