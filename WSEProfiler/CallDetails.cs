using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class CallDetails : ICallDetails
	{
		private Dictionary<string, int> _callers = new Dictionary<string, int>();
		private Dictionary<string, float> _callees = new Dictionary<string, float>();
		private float _timeSelf;
		private float _timeTotal;

		public void AddCall(Call call)
		{
			_timeSelf += call.Time;
			_timeTotal += call.TimeRecursive;

			if (call.Parent != null)
			{
				if (_callers.ContainsKey(call.Parent.Id))
					_callers[call.Parent.Id] += 1;
				else
					_callers.Add(call.Parent.Id, 1);
			}

			foreach (var child in call.Children)
			{
				if (_callees.ContainsKey(child.Id))
					_callees[child.Id] += child.TimeRecursive;
				else
					_callees.Add(child.Id, child.TimeRecursive);
			}
		}

		public IDictionary<string, int> Callers
		{
			get { return _callers; }
		}

		public IDictionary<string, float> Callees
		{
			get { return _callees; }
		}

		public float TimeSelf
		{
			get { return _timeSelf; }
		}

		public float TimeTotal
		{
			get { return _timeTotal; }
		}
	}
}
