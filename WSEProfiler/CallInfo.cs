using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class CallInfo : ICallInfo
	{
		private string _id;
		private List<float> _timesSelf = new List<float>();
		private List<float> _timesTotal = new List<float>();

		public CallInfo(string id)
		{
			_id = id;
		}

		public void AddTime(float self, float total)
		{
			_timesSelf.Add(self);
			_timesTotal.Add(total);
		}

		public override string ToString()
		{
			return _id;
		}

		public int Count
		{
			get { return _timesSelf.Count; }
		}

		public float SumSelf
		{
			get { return _timesSelf.Sum(); }
		}

		public float SumTotal
		{
			get { return _timesTotal.Sum(); }
		}

		public float AvgSelf
		{
			get { return _timesSelf.Average(); }
		}

		public float AvgTotal
		{
			get { return _timesTotal.Average(); }
		}

		public float MinSelf
		{
			get { return _timesSelf.Min(); }
		}

		public float MinTotal
		{
			get { return _timesTotal.Min(); }
		}

		public float MaxSelf
		{
			get { return _timesSelf.Max(); }
		}

		public float MaxTotal
		{
			get { return _timesTotal.Max(); }
		}
	}
}
