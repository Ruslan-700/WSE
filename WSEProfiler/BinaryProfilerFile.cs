using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace WSEProfiler
{
	class BinaryProfilerFile
	{
		private BitStream _stream;
		private bool _terminated;
		private uint _profilerVersion;
		private uint _wseVersionMajor;
		private uint _wseVersionMinor;
		private uint _wseVersionBuild;
		private ulong _length;
		private ulong _overhead;
        private ulong _int_frequency;
		private float _frequency;
        private uint _t_record_start = 0;
        private float _totalTime;

		private Dictionary<string, CallInfo> _infos = new Dictionary<string, CallInfo>();
		private CallDetails _details;

		public BinaryProfilerFile(string path)
		{
			_stream = new BitStream(File.Open(path, FileMode.Open, FileAccess.Read, FileShare.ReadWrite));
		}

        public void Parse(string blockName, List<Call> call_list = null, List<Marker> marker_list = null)
		{
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
            _int_frequency = _stream.ReadU64(64);
            _frequency = (float)_int_frequency;
			_overhead = _stream.ReadU64(64);
			_totalTime = 0;

            if (_profilerVersion == 1)
            {
                ParsePayload_V1(blockName);
            }
            else
            {
                ParsePayload_V2(blockName, call_list, marker_list);
            }
		}

        private void ParsePayload_V1(string blockName)
        {
            List<string> types = new List<string>();
            int recursionLevel = 0;
            Call baseCall = new Call("Engine");
            Call curCall = baseCall;

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

                            _totalTime += curCall.Children[0].TimeTotal;
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

        private void ParsePayload_V2(string blockName, List<Call> call_list, List<Marker> marker_list)
        {
            _t_record_start = _stream.ReadU32(32);

            List<string> types = new List<string>();
            int recursionLevel = 0;
            Call baseCall = new Call("Engine");
            Call curCall = baseCall;

            while (_stream.Position() < _length)
            {
                uint type = (_stream.ReadU32(1) << 1) | _stream.ReadU32(1);

                if (type == 3) //0b11, type name
                {
                    types.Add(_stream.ReadString());
                }
                else if (type == 2) //0b10, frame marker
                {
                    //frame mark
                    ulong t = _stream.Read_deltaBCI15() - _t_record_start;
                    if (marker_list != null)
                    {
                        t *= 1000000;
                        t /= _int_frequency;

                        marker_list.Add(new Marker((uint)t, Marker.Marker_Type.Frame));
                    }
                }
                else if (type == 1) //0b01, block start
                {
                    var id_idx = _stream.ReadBCI15();
                    ulong t = _stream.Read_deltaBCI15() - _t_record_start;

                    var call = new Call(types[(int)id_idx], curCall);

                    t *= 1000000;
                    t /= _int_frequency;
                    call.TimeStart = (uint)t;

                    curCall.Children.Add(call);
                    curCall = call;
                    recursionLevel++;
                }
                else if (type == 0){ //0b00, block end
                    ulong t = _stream.Read_deltaBCI15() - _t_record_start;

                    t *= 1000000;
                    t /= _int_frequency;
                    curCall.TimeStop = (uint)t;

                    curCall.Time = (float)(curCall.TimeStop - curCall.TimeStart);
                    //curCall.Time *= 1000000;
                    //curCall.Time /= _frequency;
                    curCall.Time -= curCall.TimeChilds;

                    if (curCall.Id == blockName)
                        _details.AddCall(curCall);

                    if (recursionLevel == 1 && call_list != null)
                        call_list.Add(curCall);

                    curCall = curCall.Parent;
                    recursionLevel--;

                    if (curCall == baseCall)
                    {
                        if (blockName == "")
                        {
                            if (curCall.Children.Count != 1)
                                throw new Exception("Base call with multiple children.");

                            _totalTime += curCall.Children[0].TimeTotal;
                            ParseCall(curCall.Children[0]);
                        }

                        curCall.Children.Clear();
                    }
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

			info.AddTime(call.Time, call.TimeTotal);

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

        public ulong iFrequency
        {
            get { return _int_frequency; }
        }
	}
}
