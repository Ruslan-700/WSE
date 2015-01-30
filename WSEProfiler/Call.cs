using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WSEProfiler
{
	public class Call
	{
		private string _id;
		private float _time;
		private float _totalTime = -1;
		private Call _parent;
		private List<Call> _children = new List<Call>();

		public Call(string id)
		{
			_id = id;
		}

		public Call(string id, Call parent)
		{
			_id = id;
			_parent = parent;
		}

		public float CalculateTime()
		{
			if (_totalTime == -1)
			{
				_totalTime = _time;

				foreach (var child in _children)
				{
					_totalTime += child.TimeRecursive;
				}
			}

			return _totalTime;
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

		public float TimeRecursive
		{
			get { return CalculateTime(); }
		}

		public Call Parent
		{
			get { return _parent; }
		}

		public List<Call> Children
		{
			get { return _children; }
		}
	}
}
