#include "WSEPerformanceContext.h"

#include <Psapi.h>
#include "WSE.h"

DWORD WINAPI MemoryOptimizerThread(LPVOID param)
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
	HANDLE process_handle = GetCurrentProcess();
	SIZE_T memory_threshold = (int)param;

	if (memory_threshold < 200)
		memory_threshold = 200;
	
	memory_threshold *= 1024 * 1024;

	while (true)
	{
		GetProcessMemoryInfo(process_handle, (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));

		if ((pmc.WorkingSetSize) > memory_threshold)
		{
			EmptyWorkingSet(process_handle);
			Sleep(15000);
		}
		else
		{
			Sleep(5000);
		}
	}

	return EXIT_SUCCESS;
}

WSEPerformanceContext::WSEPerformanceContext()
{
	m_optimizer_thread_handle = nullptr;
}

void WSEPerformanceContext::OnLoad()
{
	int threshold = 0;
	bool enabled = false;
	
	if (WSE->SettingsIni.Bool("memory_optimizer", "enabled", false))
	{
		enabled = true;
		threshold = WSE->SettingsIni.Int("memory_optimizer", "threshold", 500);
	}
	else
	{
		MEMORYSTATUSEX status;

		status.dwLength = sizeof(status);

		GlobalMemoryStatusEx(&status);

		int memory_mb = (int)(status.ullTotalPhys / 1024 / 1024);

		if (memory_mb < 1600)
		{
			enabled = true;
			threshold = 400;
		}
		else if (memory_mb < 2100)
		{
			enabled = true;
			threshold = 500;
		}
		else if(memory_mb < 2500)
		{
			enabled = true;
			threshold = 600;
		}
	}

	if (enabled)
		m_optimizer_thread_handle = CreateThread(NULL, 0, MemoryOptimizerThread, (LPVOID)threshold, 0, NULL);
}

void WSEPerformanceContext::OnUnload()
{
	if (m_optimizer_thread_handle)
		TerminateThread(m_optimizer_thread_handle, EXIT_SUCCESS);

	m_optimizer_thread_handle = nullptr;
}

void WSEPerformanceContext::OnEvent(WSEContext *sender, WSEEvent evt)
{
	switch (evt)
	{
	case ModuleLoad:
		SetCriticalSectionSpinCount(&warband->network_manager.network_critical_section, 4000);
		SetCriticalSectionSpinCount(&warband->network_manager.url_reply_critical_section, 1000);
#if defined WARBAND
		if (warband->rendering_event_1)
		{
			SetCriticalSectionSpinCount(&warband->rendering_critical_section, 4000);
			SetCriticalSectionSpinCount(&warband->multithreading_critical_section, 4000);
			SetCriticalSectionSpinCount(&warband->directx_critical_section, 4000);

			if (warband->config.use_ondemand_textures && warband->config.use_ondemand_textures_mt)
			{
				SetCriticalSectionSpinCount(&warband->multithreaded_texture_creator_critical_section, 2000);
				SetCriticalSectionSpinCount(&warband->multithreaded_texture_loader_critical_section, 2000);
			}
		}
#endif
		break;
	}
}
