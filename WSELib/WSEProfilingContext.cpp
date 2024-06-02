#include "WSEProfilingContext.h"

#include <ctime>
#include "WSE.h"

WSEProfilingContext::WSEProfilingContext() : m_is_recording(false), m_flush_interval(0)
{
}

void WSEProfilingContext::OnLoad()
{
	m_flush_interval = WSE->SettingsIni.Int("profiling", "flush_interval", 60);
}

void WSEProfilingContext::OnUnload()
{
	Stop();
}

void WSEProfilingContext::OnEvent(WSEContext *sender, WSEEvent evt, void *data)
{
	switch (evt)
	{
	case ModuleLoad:
		if (WSE->SettingsIni.Bool("profiling", "enabled", false)) {
			Start();
		}
		break;

	case OnFrame:
		if (m_is_recording)
		{
			m_profile_stream.WriteU32(1, 1);
			m_profile_stream.WriteU32(0, 1);
			m_profile_stream.WriteU32(0, 1);
			
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);
			m_profile_stream.Write_DeltaBCI15(t.QuadPart);
#ifdef profiler_debug
			WSE->Log.Info("Frame marker, t=%lld", t.QuadPart);
#endif
		}
		break;
	}
}

bool WSEProfilingContext::IsRecording()
{
	return m_is_recording;
}

void WSEProfilingContext::Start()
{
	if (m_is_recording){ return; }

	m_is_recording = true;

	char path[MAX_PATH];
	char time_str[256];
	time_t raw_time;
	tm time_info;

	time(&raw_time);
	localtime_s(&time_info, &raw_time);
	strftime(time_str, MAX_PATH, "%d.%m.%y-%H.%M.%S", &time_info);
	sprintf_s(path, "%s%s-%s.wseprfb", WSE->GetPath().c_str(), warband->cur_module_name.c_str(), time_str);

	m_profile_stream.~WSEBitStream();
	new (&m_profile_stream) WSEBitStream();

	if (!m_profile_stream.Open(path))
	{
		WSE->Log.Error("Profiling: failed to open file %s", path);
		m_is_recording = false;
		return;
	}
	
	WSE->Log.Info("Profiling: started (%s)", path);
	
	m_profile_stream.WriteU32(PROFILING_MAGIC, 32);
	m_profile_stream.WriteU32(PROFILING_VERSION, 16);
	m_profile_stream.WriteU32(WSE_VERSION_MAJOR, 16);
	m_profile_stream.WriteU32(WSE_VERSION_MINOR, 16);
	m_profile_stream.WriteU32(WSE_VERSION_BUILD, 16);
	
	LARGE_INTEGER ovh1, ovh2;
	__int64 overhead = 0;

	QueryPerformanceFrequency(&m_frequency);

	for (int i = 0; i < 1000; ++i)
	{
		QueryPerformanceCounter(&ovh1);
		QueryPerformanceCounter(&ovh2);

		overhead += ovh2.QuadPart - ovh1.QuadPart;
	}

	overhead /= 1000;
	m_cur_str_id = 0;
	m_str_ids.clear();
	m_profile_stream.WriteU64(m_frequency.QuadPart, 64);
	m_profile_stream.WriteU64(overhead, 64);
	QueryPerformanceCounter(&m_last_flush);
	m_profile_stream.WriteU64(m_last_flush.QuadPart, 64);

#ifdef profiler_debug
	WSE->Log.Info("Header, version=%i, major=%i, minor=%i, build=%i", PROFILING_VERSION, WSE_VERSION_MAJOR, WSE_VERSION_MINOR, WSE_VERSION_BUILD);
	WSE->Log.Info("Header, frequency=%lld", m_frequency.QuadPart);
	WSE->Log.Info("Header, overhead=%lli", overhead);
	WSE->Log.Info("Header, t_start=%lld", m_last_flush.QuadPart);
#endif
}

void WSEProfilingContext::Stop()
{
	m_is_recording = false;

	if (m_profile_stream.IsOpen())
	{
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);

		unsigned __int64 len = m_profile_stream.Length();

		m_profile_stream.Commit(true);

		m_profile_stream.WriteU64(t.QuadPart, 64);

		m_profile_stream.WriteU64(len, 64);
		m_profile_stream.WriteU32(PROFILING_MAGIC, 32);
		m_profile_stream.Close();
		WSE->Log.Info("Profiling: stopped");

#ifdef profiler_debug
		WSE->Log.Info("Footer, time_stop=%lld", t.QuadPart);
		WSE->Log.Info("Footer, len=%llu", len);
#endif
	}
}

void inline WSEProfilingContext::IntroduceStr(const rgl::string& str)
{
	if (m_str_ids.find(str) == m_str_ids.end())
	{
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteString(str);
		m_str_ids[str] = m_cur_str_id++;

#ifdef profiler_debug
		//std::stringstream s;
		/*for (int i = 0; i < str.length(); i++){
			s << std::to_string((unsigned char)str.buffer[i]) << ' ';
		}*/

		WSE->Log.Info("String Intro, len=%d, str='%s', id=%i", str.length(), str.c_str(), m_str_ids[str]);
		//WSE->Log.Info("str pointer %p", (void*)str.c_str());
#endif
	}
}

void WSEProfilingContext::StartProfilingBlock(wb::operation_manager *manager)
{
	if (m_is_recording)
	{
		IntroduceStr(manager->id);

		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteBCI15(m_str_ids[manager->id]);

		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		m_profile_stream.Write_DeltaBCI15(t.QuadPart);

#ifdef profiler_debug
		WSE->Log.Info("Block Start, id=%i, t=%lld", m_str_ids[manager->id], t.QuadPart);
#endif
	}
}

void WSEProfilingContext::StopProfilingBlock(int depth)
{
	if (m_is_recording)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.Write_DeltaBCI15(end.QuadPart);

		if (depth == 0 && m_flush_interval > 0 && ((end.QuadPart - m_last_flush.QuadPart) / m_frequency.QuadPart >= m_flush_interval))
		{
			m_profile_stream.Flush();
			m_last_flush = end;
		}

#ifdef profiler_debug
		WSE->Log.Info("Block End, t=%lld", end.QuadPart);
#endif
	}
}

void WSEProfilingContext::AddMarker(const rgl::string& text)
{
	if (m_is_recording)
	{
		IntroduceStr(text);

		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteBCI15(m_str_ids[text]);

		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		m_profile_stream.Write_DeltaBCI15(t.QuadPart);

#ifdef profiler_debug
		WSE->Log.Info("Custom Marker, id=%i, t=%lld", m_str_ids[text], t.QuadPart);
#endif
	}
}

/*  v1 Format
	
	Header
		PROFILING_MAGIC, 32
		PROFILING_VERSION, 16
		WSE_VERSION_MAJOR, 16
		WSE_VERSION_MINOR, 16
		WSE_VERSION_BUILD, 16
		Frequency, 64
		Overhead, 64

	Payload
		ID introduction, once for each new ID (id is script or trigger name)
			1	id [len (12bit), chars (len*8bit)]
	  
		Block
			01	id_index (BCI15)
			00	(time delta between block start/end, minus time of child calls (outside)) (BCI15)

		or Nested Block
			01 id_index_0
				01 id_index_1
					01 id_index_2
						...
					00 time_2
				00 time_1
				01 id_index_3
					...
				00 time_3
			00 time_0

	Footer
		stream len, 64
		PROFILING_MAGIC, 32
*/

/*	v2 Format changes

	Header
		...
		record_start_time, 64

	Payload
		ID introduction, once for each new ID (id is script or trigger name)
			11	id [len (12bit), chars (len*8bit)]

		Frame Marker
			100 time (delta_BCI15)
		Custom marker
			101	id_index (BCI15), time (delta_BCI15)

		Block
			01	id_index (BCI15), start time (delta_BCI15)
			00	end time (delta_BCI15)

	Footer
		record_stop_time, 64
		...
*/