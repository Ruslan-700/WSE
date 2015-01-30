#include <Windows.h>
#include "WSE.h"
#include "warband.h"

LONG LoadUnloadExceptionHandler(LPEXCEPTION_POINTERS ptrs)
{
	DumpStackTrace(ptrs);
	return EXCEPTION_CONTINUE_SEARCH;
}

void LoadWSE(HINSTANCE handle)
{
	__try
	{
		WSE->Load(handle);
	}
	__except (LoadUnloadExceptionHandler(GetExceptionInformation()))
	{
	}
}

void UnloadWSE()
{
	__try
	{
		WSE->Unload();
	}
	__except (LoadUnloadExceptionHandler(GetExceptionInformation()))
	{
	}
}

bool WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved)
{
#if !defined WARBAND && !defined WARBAND_DEDICATED
	MessageBox(nullptr, "This library targets neither the vanilla game nor the dedicated server.", "Error", MB_ICONERROR);
	return false;
#endif

	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		if (!WSE)
		{
			DisableThreadLibraryCalls(handle);
			warband_init();
			WSE = new WSEEnvironment();
			LoadWSE(handle);
		}
		break;
	case DLL_PROCESS_DETACH:
		if (WSE)
		{
			UnloadWSE();
			delete WSE;
			WSE = nullptr;
		}
		break;
	}
	
	return true;
}
