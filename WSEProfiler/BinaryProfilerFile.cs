using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class BinaryProfilerFile : IProfilerFile
	{
		private BitStream _stream;
		private bool _terminated;
		private uint _profilerVersion;
		private uint _wseVersionMajor;
		private uint _wseVersionMinor;
		private uint _wseVersionBuild;
		private ulong _length;
		private ulong _overhead;
		private float _frequency;
		private float _totalTime;
		private Dictionary<string, CallInfo> _infos = new Dictionary<string, CallInfo>();
		private CallDetails _details;

		public BinaryProfilerFile(string path)
		{
			_stream = new BitStream(File.Open(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
		}

		public void Parse(string blockName)
		{
			List<string> types = new List<string>();
			int recursionLevel = 0;
			Call baseCall = new Call("Engine");
			Call curCall = baseCall;

			_infos.Clear();
			_details = new CallDetails();
			_stream.Seek(-4, SeekOrigin.End);
			uint endMagic = _stream.ReadU32(32);

			if (endMagic == 0xDEADCAFE)
			{
				_stream.Seek(-12, SeekOrigin.End);
				_length = _stream.ReadU64(64);

				if (_length > _stream.Length())
				{
					_length = _stream.Length();
					_terminated = false;
				}
				else
				{
					_terminated = true;
				}
			}
			else
			{
				_length = _stream.Length();
				_terminated = false;
			}

			_stream.Seek(0, SeekOrigin.Begin);
			uint startMagic = _stream.ReadU32(32);

			if (startMagic != 0xDEADCAFE)
				throw new ErrorException("Unrecognized file format.");

			_profilerVersion = _stream.ReadU32(16);
			_wseVersionMajor = _stream.ReadU32(16);
			_wseVersionMinor = _stream.ReadU32(16);
			_wseVersionBuild = _stream.ReadU32(16);
			_frequency = (float)_stream.ReadU64(64);
			_overhead = _stream.ReadU64(64);
			_totalTime = 0;

			while (_stream.Position() < _length)
			{
				var type = _stream.ReadU32(1);

				if (type == 0)
				{
					var rec = _stream.ReadU32(1);
					var bci = _stream.ReadBCI15();

					if (rec > 0)
					{
						var call = new Call(types[(int)bci], curCall);

						curCall.Children.Add(call);
						curCall = call;
						recursionLevel++;
					}
					else
					{
						ulong time = bci - _overhead;

						if (time < 0)
							time = 0;

						curCall.Time = time * 1000000 / _frequency;

						if (curCall.Id == blockName)
							_details.AddCall(curCall);

						curCall = curCall.Parent;
						recursionLevel--;
					}

					if (curCall == baseCall)
					{
						if (blockName == "")
						{
							if (curCall.Children.Count != 1)
								throw new Exception("Base call with multiple children.");

							_totalTime += curCall.Children[0].TimeRecursive;
							ParseCall(curCall.Children[0]);
						}
						
						curCall.Children.Clear();
					}
				}
				else if (type == 1)
				{
					types.Add(_stream.ReadString());
				}
			}

			if (recursionLevel != 0)
			{
				this.ShowWarning("Final block depth non-zero. Is the profiling file damaged or incomplete?");

				var call = curCall;

				while (call != null)
				{
					ParseCall(call);
					call = call.Parent;
				}
			}
		}

		public void Close()
		{
			_infos.Clear();
			_details = null;
			GC.Collect();
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
			get
			{
				return string.Format("WSE v{0}.{1}.{2}, Profiler v{3}, QPC frequency: {4}, QPC overhead: {5}, total time: {6}, Status: {7}.",
					_wseVersionMajor, _wseVersionMinor, _wseVersionBuild, _profilerVersion, _frequency, _overhead, _totalTime.FormatTime(), _terminated ? "full" : "partial");
			}
		}

		public IEnumerable<ICallInfo> CallInfos
		{
			get { return _infos.Values; }
		}

		public ICallDetails CallDetails
		{
			get { return _details; }
		}

		public float TimeTotal
		{
			get { return _totalTime; }
		}
	}
}
