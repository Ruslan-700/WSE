using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class ErrorException : Exception
	{
		public ErrorException(string text)
			: base(text)
		{
		}
	}

	public interface ICallDetails
	{
		IDictionary<string, int> Callers { get; }
		IDictionary<string, float> Callees { get; }
		float TimeSelf { get; }
		float TimeTotal { get; }
	}

	public interface ICallInfo
	{
		int Count { get; }
		float SumSelf { get; }
		float SumTotal { get; }
		float AvgSelf { get; }
		float AvgTotal { get; }
		float MinSelf { get; }
		float MinTotal { get; }
		float MaxSelf { get; }
		float MaxTotal { get; }
	}

	public interface IProfilerFile
	{
		void Parse(string blockName);
		void Close();

		IEnumerable<ICallInfo> CallInfos { get; }
		ICallDetails CallDetails { get; }
		string InfoString { get; }
		float TimeTotal { get; }
	}
}
