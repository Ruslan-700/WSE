#include "WSEProfilingContext.h"

#include <ctime>
#include "WSE.h"

WSEProfilingContext::WSEProfilingContext() : m_enabled(false), m_flush_interval(0)
{
}

void WSEProfilingContext::OnLoad()
{
	m_enabled = WSE->SettingsIni.Bool("profiling", "enabled", false);
	m_flush_interval = WSE->SettingsIni.Int("profiling", "flush_interval", 60);
	
	if (!m_enabled)
		return;
}

void WSEProfilingContext::OnUnload()
{
	Stop();
}

void WSEProfilingContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ModuleLoad:
		Stop();
		Start();
		break;
	}
}

void WSEProfilingContext::Start()
{
	if (!m_enabled)
		return;

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
		m_enabled = false;
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
	m_cur_info = 0;
	m_profile_stream.WriteU64(m_frequency.QuadPart, 64);
	m_profile_stream.WriteU64(overhead, 64);
	QueryPerformanceCounter(&m_last_flush);
	WSE->Log.Info("Profiling: started (%s)", path);
}

void WSEProfilingContext::Stop()
{
	if (!m_enabled)
		return;

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

void WSEProfilingContext::StartProfilingBlock(wb::operation_manager *manager)
{
	if (!m_enabled)
		return;

	LARGE_INTEGER rec_start;
	
	QueryPerformanceCounter(&rec_start);
	
	if (m_infos[m_cur_info].rec_start.QuadPart > 0)
		m_infos[m_cur_info].outside += m_infos[m_cur_info].rec_end.QuadPart - m_infos[m_cur_info].rec_start.QuadPart;

	m_infos[m_cur_info].rec_start = rec_start;
	m_cur_info++;

	m_infos[m_cur_info].outside = 0;
	m_infos[m_cur_info].rec_start.QuadPart = 0;

	if (m_profile_types.find(&manager->id) == m_profile_types.end())
	{
		m_profile_stream.WriteU32(1, 1);
		m_profile_stream.WriteString(manager->id);
		m_profile_types[&manager->id] = m_cur_profile_type++;
	}

	m_profile_stream.WriteU32(0, 1);
	m_profile_stream.WriteU32(1, 1);
	m_profile_stream.WriteBCI15(m_profile_types[&manager->id]);
	QueryPerformanceCounter(&m_infos[m_cur_info].start);
}

void WSEProfilingContext::StopProfilingBlock()
{
	if (!m_enabled)
		return;

	LARGE_INTEGER end;

	QueryPerformanceCounter(&end);
	
	if (m_infos[m_cur_info].rec_start.QuadPart > 0)
		m_infos[m_cur_info].outside += m_infos[m_cur_info].rec_end.QuadPart - m_infos[m_cur_info].rec_start.QuadPart;
	
	m_profile_stream.WriteU32(0, 1);
	m_profile_stream.WriteU32(0, 1);
	m_profile_stream.WriteBCI15((unsigned int)(end.QuadPart - m_infos[m_cur_info].start.QuadPart - m_infos[m_cur_info].outside));
	m_cur_info--;

	if (m_cur_info == 0 && m_flush_interval > 0 && ((end.QuadPart - m_last_flush.QuadPart) / m_frequency.QuadPart >= m_flush_interval))
	{
		m_profile_stream.Flush();
		m_last_flush = end;
	}

	QueryPerformanceCounter(&m_infos[m_cur_info].rec_end);
}
