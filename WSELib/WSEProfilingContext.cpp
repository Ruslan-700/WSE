#include "WSEProfilingContext.h"

#include <ctime>
#include "WSE.h"

WSEProfilingContext::WSEProfilingContext() : m_status(Status::stopped), m_flush_interval(0)
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
		if (m_status & Status::recording)
		{
			LARGE_INTEGER t;
			QueryPerformanceCounter(&t);

			m_profile_stream.WriteU32(1, 1);
			m_profile_stream.WriteU32(0, 1);
			m_profile_stream.Write_DeltaBCI15((unsigned int)t.QuadPart);
		}
		break;
	}
}

void WSEProfilingContext::SetAwaitStatus(Status s)
{
	assert(Status::awaitStart <= s && s <= Status::awaitResume);
	m_status = s;
}

WSEProfilingContext::Status WSEProfilingContext::GetStatus()
{
	return m_status;
}

void WSEProfilingContext::Start()
{
	if (m_status != Status::stopped)
		return;

	m_status = Status::recording;

	char path[MAX_PATH];
	char time_str[256];
	time_t raw_time;
	tm time_info;

	time(&raw_time);
	localtime_s(&time_info, &raw_time);
	strftime(time_str, MAX_PATH, "%H.%M.%S-%d.%m.%y", &time_info);
	sprintf_s(path, "%s%s-%s.wseprfb", WSE->GetPath().c_str(), warband->cur_module_name.c_str(), time_str);

	if (!m_profile_stream.Open(path))
	{
		WSE->Log.Error("Profiling: failed to open file %s", path);
		m_status = Status::stopped;
		return;
	}
	
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
	m_cur_profile_type = 0;
	m_profile_types.clear();
	m_profile_stream.WriteU64(m_frequency.QuadPart, 64);
	m_profile_stream.WriteU64(overhead, 64);
	QueryPerformanceCounter(&m_last_flush);
	WSE->Log.Info("Profiling: started (%s)", path);
}

void WSEProfilingContext::Stop()
{
	m_status = Status::stopped;

	if (m_profile_stream.IsOpen())
	{
		unsigned __int64 len = m_profile_stream.Length();

		m_profile_stream.Commit(true);
		m_profile_stream.WriteU64(len, 64);
		m_profile_stream.WriteU32(PROFILING_MAGIC, 32);
		m_profile_stream.Close();
		WSE->Log.Info("Profiling: stopped");
	}
}

void WSEProfilingContext::StartProfilingBlock(int depth, wb::operation_manager *manager)
{
	if (m_status == Status::awaitStart && depth == 0){
		m_status = Status::stopped;
		Start();
	}
	if (m_status == Status::awaitResume && depth == 0){
		m_status = Status::recording;
	}

	if (m_status & Status::recording)
	{
		if (m_profile_types.find(&manager->id) == m_profile_types.end())
		{
			m_profile_stream.WriteU32(1, 1);
			m_profile_stream.WriteU32(1, 1);
			m_profile_stream.WriteString(manager->id);
			m_profile_types[&manager->id] = m_cur_profile_type++;
		}

		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteBCI15(m_profile_types[&manager->id]);

		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);

		m_profile_stream.Write_DeltaBCI15((unsigned int)t.QuadPart);
	}
}

void WSEProfilingContext::StopProfilingBlock(int depth)
{
	if (m_status & Status::recording)
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);

		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.WriteU32(0, 1);
		m_profile_stream.Write_DeltaBCI15((unsigned int)end.QuadPart);

		if (depth == 0 && m_flush_interval > 0 && ((end.QuadPart - m_last_flush.QuadPart) / m_frequency.QuadPart >= m_flush_interval))
		{
			m_profile_stream.Flush();
			m_last_flush = end;
		}
	}

	if (m_status == Status::awaitStop && depth == 0){
		Stop();
	}
	if (m_status == Status::awaitPause && depth == 0){
		m_status = Status::paused;
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

/*	v2 Format

	Payload
		ID introduction, once for each new ID (id is script or trigger name)
			11	id [len (12bit), chars (len*8bit)]

		Frame marker
			10	time (delta_BCI15)

		Block
			01	id_index (BCI15), start time (delta_BCI15)
			00	end time (delta_BCI15)
			
*/