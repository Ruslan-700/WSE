using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class TextProfilerFile : IProfilerFile
	{
		private StreamReader _stream;
		private uint _wseVersion;
		private ulong _overhead;
		private float _frequency;
		private float _totalTime;
		private Dictionary<string, CallInfo> _infos = new Dictionary<string, CallInfo>();

		public TextProfilerFile(string path)
		{
			_stream = new StreamReader(File.Open(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
		}

		public void Parse(string blockName) // TODO: use blockname
		{
			Dictionary<int, string> types = new Dictionary<int,string>();
			int recursionLevel = 0;
			Call baseCall = new Call("Engine");
			Call curCall = baseCall;

			_infos.Clear();
			_wseVersion = uint.Parse(_stream.ReadLine());
			_frequency = float.Parse(_stream.ReadLine());
			_overhead = ulong.Parse(_stream.ReadLine());
			_totalTime = 0;

			while (_stream.Peek() >= 0)
			{
				var line = _stream.ReadLine();
				var cells = line.Trim().Split(',');

				if (cells[0] == "#")
				{
					var id = int.Parse(cells[1]);
					var type = cells[2];

					types.Add(id, type);
					continue;
				}

				var rec = int.Parse(cells[0]);

				if (rec > recursionLevel)
				{
					var call = new Call(types[int.Parse(cells[1])], curCall);

					curCall.Children.Add(call);
					curCall = call;
					recursionLevel++;
				}
				else
				{
					ulong time = ulong.Parse(cells[1]) - _overhead;

					if (time < 0)
						time = 0;

					curCall.Time = time * 1000000 / _frequency;

					curCall = curCall.Parent;
					recursionLevel--;
				}

				if (curCall == baseCall)
				{
					if (curCall.Children.Count != 1)
						throw new Exception("Base call with multiple children.");

					_totalTime += curCall.Children[0].TimeRecursive;
					ParseCall(curCall.Children[0]);
					curCall.Children.Clear();
				}
			}

			if (recursionLevel != 0)
				this.ShowWarning("Final block depth non-zero. Is the profiling file damaged or incomplete?");
		}

		public void Close()
		{
			throw new NotImplementedException();
		}

		private void ParseCall(Call call)
		{
			if (!_infos.ContainsKey(call.Id))
				_infos.Add(call.Id, new CallInfo(call.Id));

			var info = _infos[call.Id];

			info.AddTime(call.Time, call.TimeRecursive);

			foreach (var child in call.Children)
			{
				ParseCall(child);
			}
		}

		public string InfoString
		{
			get { return string.Format("WSE v{0}, QPC frequency: {1}, QPC overhead: {2}, total time: {3}.", _wseVersion, _frequency, _overhead, _totalTime.FormatTime()); }
		}

		public IEnumerable<ICallInfo> CallInfos
		{
			get { return _infos.Values; }
		}

		public ICallDetails CallDetails
		{
			get { throw new NotImplementedException(); }
		}

		public float TimeTotal
		{
			get { return _totalTime; }
		}
	}
}
