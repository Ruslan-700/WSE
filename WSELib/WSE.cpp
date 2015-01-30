#include "WSE.h"

#include <DbgHelp.h>
#include <iomanip>
#include <sstream>
#include <vector>
#include "WSEPlugin.h"

extern WSEEnvironment *WSE = nullptr;

struct ModuleTable
{
	std::vector<std::string> m_names;
	std::vector<DWORD64> m_addrs;
	std::vector<ULONG> m_sizes;
};

BOOL CALLBACK MyEnumerateModule(PCTSTR ModuleName, DWORD64 ModuleBase, ULONG ModuleSize, PVOID UserContext)
{
	ModuleTable *table = (ModuleTable *)UserContext;
	
	table->m_names.push_back(ModuleName);
	table->m_addrs.push_back(ModuleBase);
	table->m_sizes.push_back(ModuleSize);

	return TRUE;
}

const char *GetExceptionName(DWORD code)
{
	switch (code)
	{
	case EXCEPTION_ACCESS_VIOLATION:
		return "EXCEPTION_ACCESS_VIOLATION";
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
	case EXCEPTION_BREAKPOINT:
		return "EXCEPTION_BREAKPOINT";
	case EXCEPTION_DATATYPE_MISALIGNMENT:
		return "EXCEPTION_DATATYPE_MISALIGNMENT";
	case EXCEPTION_FLT_DENORMAL_OPERAND:
		return "EXCEPTION_FLT_DENORMAL_OPERAND";
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:
		return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
	case EXCEPTION_FLT_INEXACT_RESULT:
		return "EXCEPTION_FLT_INEXACT_RESULT";
	case EXCEPTION_FLT_INVALID_OPERATION:
		return "EXCEPTION_FLT_INVALID_OPERATION";
	case EXCEPTION_FLT_OVERFLOW:
		return "EXCEPTION_FLT_OVERFLOW";
	case EXCEPTION_FLT_STACK_CHECK:
		return "EXCEPTION_FLT_STACK_CHECK";
	case EXCEPTION_FLT_UNDERFLOW:
		return "EXCEPTION_FLT_UNDERFLOW";
	case EXCEPTION_GUARD_PAGE:
		return "EXCEPTION_GUARD_PAGE";
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		return "EXCEPTION_ILLEGAL_INSTRUCTION";
	case EXCEPTION_IN_PAGE_ERROR:
		return "EXCEPTION_IN_PAGE_ERROR";
	case EXCEPTION_INT_DIVIDE_BY_ZERO:
		return "EXCEPTION_INT_DIVIDE_BY_ZERO";
	case EXCEPTION_INT_OVERFLOW:
		return "EXCEPTION_INT_OVERFLOW";
	case EXCEPTION_INVALID_DISPOSITION:
		return "EXCEPTION_INVALID_DISPOSITION";
	case EXCEPTION_INVALID_HANDLE:
		return "EXCEPTION_INVALID_HANDLE";
	case EXCEPTION_NONCONTINUABLE_EXCEPTION:
		return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
	case EXCEPTION_PRIV_INSTRUCTION:
		return "EXCEPTION_PRIV_INSTRUCTION";
	case EXCEPTION_SINGLE_STEP:
		return "EXCEPTION_SINGLE_STEP";
	case EXCEPTION_STACK_OVERFLOW:
		return "EXCEPTION_STACK_OVERFLOW";
	default:
		return "EXCEPTION_UNKNOWN";
	}
}

void DumpStackTrace(LPEXCEPTION_POINTERS ptrs)
{
	HANDLE proc_handle = GetCurrentProcess();
	HANDLE thread_handle = GetCurrentThread();
	BOOL walking = TRUE;
	ModuleTable table;
	STACKFRAME64 stack;
	
	EnumerateLoadedModules64(GetCurrentProcess(), MyEnumerateModule, &table);
	ZeroMemory(&stack, sizeof(STACKFRAME64));
	stack.AddrPC.Offset = ptrs->ContextRecord->Eip;
	stack.AddrPC.Mode = AddrModeFlat;
	stack.AddrFrame.Offset = ptrs->ContextRecord->Ebp;
	stack.AddrFrame.Mode = AddrModeFlat;
	stack.AddrStack.Offset = ptrs->ContextRecord->Esp;
	stack.AddrStack.Mode = AddrModeFlat;
	
	WSELog crash_log(WSE->GetPath() + "wse_crash_log.txt", false, true);
	std::string text;
	std::stringstream ss;
	time_t cur_time_raw;
	tm cur_time;

	time(&cur_time_raw);
	localtime_s(&cur_time, &cur_time_raw);
	
	ss << std::setw(2) << std::setfill('0') << cur_time.tm_mday << "/";
	ss << std::setw(2) << std::setfill('0') << 1 + cur_time.tm_mon << "/";
	ss << std::setw(2) << std::setfill('0') << 1900 + cur_time.tm_year << " ";
	ss << std::setw(2) << std::setfill('0') << cur_time.tm_hour << ":";
	ss << std::setw(2) << std::setfill('0') << cur_time.tm_min << ":";
	ss << std::setw(2) << std::setfill('0') << cur_time.tm_sec << " ";
	crash_log.Info("> Info");
	crash_log.Info("Time: %s", ss.str().c_str());
	ss.str("");
	ss << GetExceptionName(ptrs->ExceptionRecord->ExceptionCode) << " (0x" << std::hex << std::setw(8) << std::setfill('0') << std::uppercase << ptrs->ExceptionRecord->ExceptionCode;

	for (size_t i = 0; i < ptrs->ExceptionRecord->NumberParameters; ++i)
	{
		ss << ", 0x" << std::setw(8) << std::setfill('0') << std::uppercase << ptrs->ExceptionRecord->ExceptionInformation[i];
	}

	ss << ")";
	crash_log.Info("Type: %s", ss.str().c_str());
	crash_log.Info("> Stack trace");
	SymInitialize(proc_handle, NULL, true);

	while (walking)
	{
		walking = StackWalk64(IMAGE_FILE_MACHINE_I386, proc_handle, thread_handle, &stack, ptrs->ContextRecord, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL);

		DWORD64 mod_base = 0;
		std::string mod_name = "??";
		std::string sym_name = "??";
		DWORD64 sym_offset = 0;

		for (size_t i = 0; i < table.m_addrs.size(); ++i)
		{
			if (stack.AddrPC.Offset >= table.m_addrs[i] && stack.AddrPC.Offset < table.m_addrs[i] + table.m_sizes[i])
			{
				mod_base = table.m_addrs[i];
				mod_name = table.m_names[i];

				if (mod_name.rfind("\\") != std::string::npos)
					mod_name = mod_name.substr(mod_name.rfind("\\") + 1);

				break;
			}
		}

		unsigned char buf[sizeof(IMAGEHLP_SYMBOL64) + 256];
		PIMAGEHLP_SYMBOL64 symbol = (PIMAGEHLP_SYMBOL64)&buf;
		
		ZeroMemory(symbol, sizeof(IMAGEHLP_SYMBOL64) + 256);
		symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		symbol->MaxNameLength = 256;

		if (SymGetSymFromAddr64(proc_handle, stack.AddrPC.Offset, NULL, symbol))
		{
			sym_name = std::string(symbol->Name);
			sym_offset = stack.AddrPC.Offset - symbol->Address;
		}

		crash_log.Info("0x%08I64X %s+0x%I64X (%s+0x%I64X)", stack.AddrPC.Offset, mod_name.c_str(), stack.AddrPC.Offset - mod_base, sym_name.c_str(), sym_offset);
	}

	if (WSE->Scripting.m_cur_statement_block)
	{
		crash_log.Info("> Scripting context");
		crash_log.Info("Statement block: %s", WSE->Scripting.m_cur_statement_block->id.c_str());
		crash_log.Info("Statement: %d", warband->cur_statement_no);
		crash_log.Info("Opcode: %d", warband->cur_opcode);
	}

	crash_log.Info(">");
	SymCleanup(proc_handle);
}

LONG WINAPI MyUnhandledExceptionFilter(LPEXCEPTION_POINTERS ptrs)
{
	DumpStackTrace(ptrs);

#if defined WARBAND_DEDICATED
	exit(0);
#endif

	return EXCEPTION_CONTINUE_SEARCH;
}

LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER)
{
    return NULL;
}

const std::string &WSEEnvironment::GetPath() const
{
	return m_path;
}

void WSEEnvironment::Load(HINSTANCE instance)
{
#if defined DEBUG
	HeapSetInformation(GetProcessHeap(), HeapEnableTerminationOnCorruption, NULL, 0);
#endif

	m_instance = instance;

	char path_full[MAX_PATH];
	char path[MAX_PATH];
	LPTSTR filepart;

	GetModuleFileName(instance, path_full, MAX_PATH);
	GetFullPathName(path_full, MAX_PATH, path, &filepart);
	*filepart = '\0';
	m_path = path;

	SettingsIni.Open(m_path + "wse_settings.ini");

	std::string log_path = SettingsIni.String("log", "file", m_path + "wse_log.txt");

	if (PathIsRelative(log_path.c_str()))
		log_path = m_path + log_path;

	Log.Open(log_path);
	Log.Info("WSE v%s by cmpxchg8b (Build date: %s %s)", WSE_VERSION, __DATE__, __TIME__);
	Log.Info("WSE path: %s", m_path.c_str());
	Log.Info("WSE netcode version: %d", WSE_NETCODE_VERSION);
	
#if defined WARBAND_VANILLA
	Log.Info("Warband version: vanilla");
#elif defined WARBAND_STEAM
	Log.Info("Warband version: Steam");
#elif defined WARBAND_DEDICATED
	Log.Info("Warband version: dedicated server");
#endif

#if defined DEBUG
	AddContext(&Debug, true);
#endif
	AddContext(&Performance, true);
	AddContext(&Game, true);
	AddContext(&Profiling, true);
	AddContext(&Screen, true);
	AddContext(&Network, true);
	AddContext(&Mission, true);
	AddContext(&Scripting, true);

	AddContext(&AgentOperations, false);
	AddContext(&CoreOperations, false);
#if defined DEBUG
	AddContext(&DebugOperations, false);
#endif
	AddContext(&DictionaryOperations, false);
	AddContext(&EditModeOperations, false);
	AddContext(&EngineOperations, false);
	AddContext(&FloatingPointOperations, false);
	AddContext(&InputOperations, false);
	AddContext(&ItemOperations, false);
	AddContext(&MathOperations, false);
	AddContext(&MenuOperations, false);
	AddContext(&MissionOperations, false);
	AddContext(&MultiplayerOperations, false);
	AddContext(&NetworkOperations, false);
	AddContext(&OptionsOperations, false);
	AddContext(&PartyOperations, false);
	AddContext(&PlayerOperations, false);
	AddContext(&PositionOperations, false);
	AddContext(&PresentationOperations, false);
	AddContext(&SiteOperations, false);
	AddContext(&SkinOperations, false);
	AddContext(&StringOperations, false);
	AddContext(&TroopOperations, false);
	
	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile((m_path + "\\*WSE*Plugin*.dll").c_str(), &data);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			HINSTANCE lib_handle = LoadLibrary((m_path + data.cFileName).c_str());

			if (!lib_handle)
			{
				FreeLibrary(lib_handle);
				continue;
			}

			WSEPluginInitFunc init_func = (WSEPluginInitFunc)GetProcAddress(lib_handle, "WSEPluginInit");

			if (!init_func)
			{
				FreeLibrary(lib_handle);
				continue;
			}

			WSEPluginInfo *info = new WSEPluginInfo();

			ZeroMemory(info, sizeof(WSEPluginInfo));

			if (!init_func(WSE_PLUGIN_API_VERSION, info, this))
			{
				FreeLibrary(lib_handle);
				continue;
			}
		
			info->m_name[255] = '\0';
			info->m_author[255] = '\0';
		
	#if defined DEBUG
			if (!info->m_wse_debug)
	#else
			if (info->m_wse_debug)
	#endif
			{
				FreeLibrary(lib_handle);
				continue;
			}
			else if (info->m_wse_type >= 0 && info->m_wse_type != WSE_TYPE)
			{
				std::string build_name = "";

				if (info->m_wse_type == 0)
					build_name = "vanilla";
				else if (info->m_wse_type == 1)
					build_name = "Steam";
				else if (info->m_wse_type == 2)
					build_name = "dedicated server";

				WSE->Log.Warning("%s targets a different WSE build (%s), skipped", data.cFileName, build_name.c_str());
				FreeLibrary(lib_handle);
				continue;
			}
			else if (info->m_wse_version_major != WSE_VERSION_MAJOR && info->m_wse_version_minor > WSE_VERSION_MINOR)
			{
				WSE->Log.Warning("%s targets a different WSE version (%d.%d.x), skipped", data.cFileName, info->m_wse_version_major, info->m_wse_version_minor);
				FreeLibrary(lib_handle);
				continue;
			}
		
			WSE->Log.Info("Loaded %s v%d.%d by %s", info->m_name, info->m_version_major, info->m_version_minor, info->m_author);
			AddContext(new WSEPluginWrapperContext(info), true);
		}
		while (FindNextFile(handle, &data));
	}

#if !defined DEBUG
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	WSE->Hooks.HookFunction(nullptr, (unsigned int)GetProcAddress(LoadLibrary("kernel32.dll"), "SetUnhandledExceptionFilter"), (WSEHookCallback)MySetUnhandledExceptionFilter);
#endif
}

void WSEEnvironment::Unload()
{
#if !defined DEBUG
	SetUnhandledExceptionFilter(NULL);
#endif

	std::list<WSEContext *>::const_reverse_iterator it;

	for (it = m_contexts.rbegin(); it != m_contexts.rend(); ++it)
	{
		(*it)->Unload();
	}

	WSE->Hooks.UnhookAll();
}

void WSEEnvironment::AddContext(WSEContext *context, bool load)
{
	m_contexts.push_back(context);

	if (load)
		context->Load();
}

void WSEEnvironment::SendContextEvent(WSEContext *sender, WSEEvent evt)
{
	if (!sender->IsLoaded())
		return;

	std::list<WSEContext *>::const_iterator it;

	for (it = m_contexts.begin(); it != m_contexts.end(); ++it)
	{
		(*it)->Event(sender, evt);
	}
}
