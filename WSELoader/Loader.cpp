#include "Loader.h"

#include <intrin.h>
#include <Psapi.h>
#include <Shellapi.h>
#include <TlHelp32.h>
#include <Windows.h>
#include "CRC32.h"
#include <algorithm>
#include <fstream>
#include <iostream>

HANDLE __processHandle = INVALID_HANDLE_VALUE;

BOOL WINAPI HandlerRoutine(DWORD ctrlType)
{
	if ((ctrlType == CTRL_CLOSE_EVENT || ctrlType == CTRL_C_EVENT) && __processHandle != INVALID_HANDLE_VALUE)
	{
		TerminateProcess(__processHandle, 0);
		__processHandle = INVALID_HANDLE_VALUE;
	}

	return FALSE;
}

BOOL WINAPI EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	DWORD processId = 0;

	GetWindowThreadProcessId(hwnd, &processId);
	
	if (processId == ((DWORD *)lparam)[0])
	{

		((HWND *)lparam)[1] = hwnd;
		return FALSE;
	}

	return TRUE;
}

Loader::Loader()
{
	_consoleShown = false;
	_consoleAttached = false;
}

void Loader::Process(const CString &cmdLine)
{
	bool showHelp = false;
	CString commandLine;
	CString dllPath;
	CString warbandPath;

	DWORD result;
	
	if (result = GetModuleFileName(NULL, _loaderFile.GetBuffer(MAX_PATH), MAX_PATH))
		_loaderFile.ReleaseBuffer(result);

	LPTSTR filePart;

	if (result = GetFullPathName(_loaderFile, MAX_PATH, _wsePath.GetBuffer(MAX_PATH), &filePart))
		_wsePath.ReleaseBuffer(result);
	
	_wsePath.Truncate(_wsePath.GetLength() - CString(filePart).GetLength());
	
	int argc;
	LPTSTR *argv = CommandLineToArgvW("\"" + _loaderFile + "\" " + cmdLine, &argc);
	CString moduleName;
	CString windowName;

	for (int i = 1; i < argc; ++i)
	{
		CString arg = argv[i];

		if ((arg == "-p" || arg == "--path") && i <= argc)
		{
			warbandPath = argv[i++ + 1];
		}
		else if ((arg == "-m" || arg == "--module") && i <= argc)
		{
			moduleName = argv[i++ + 1];
			commandLine.AppendFormat(L" -m %s", moduleName.GetBuffer());
		}
		else if ((arg == "-r" || arg == "--read") && i <= argc)
		{
			commandLine.AppendFormat(L" -r %s", argv[i++ + 1]);
		}
		else if ((arg == "-w" || arg == "--windowname") && i <= argc)
		{
			windowName = argv[i++ + 1];
		}
		else if (arg == "-v" || arg == "--verbose")
		{
			ShowConsole();
		}
		else if (arg == "-h" || arg == "--help")
		{
			ShowConsole();
			showHelp = true;
		}
	}
	
	LocalFree(argv);

	if (showHelp)
	{
		Write(MF_INFO, L"usage: WSELoader.exe [arguments]");
		Write(MF_INFO, L"");
		Write(MF_INFO, L"arguments:");
		Write(MF_INFO, L"-m  --module <module_name>  set selected module");
		Write(MF_INFO, L"-r  --read <server_cfg>     use server config file");
		Write(MF_INFO, L"-p  --path <warband_path>   set Warband executable path (executable file name must be included)");
		Write(MF_INFO, L"-w  --windowname <name>     set Warband window name");
		Write(MF_INFO, L"-v  --verbose               print debug messages");
		Write(MF_INFO, L"-h  --help:                 print usage information");
		Write(MF_INFO | MF_QUIT, L"");
	}
	
	Write(MF_INFO, L"WSE Loader by cmpxchg8b (Build date: %S %S)", __DATE__, __TIME__);

	OSVERSIONINFOEX osVersionInfo = { sizeof(OSVERSIONINFOEX), 0 };

	GetVersionEx((LPOSVERSIONINFO)&osVersionInfo);
	Write(MF_INFO, L"OS version: %d.%d.%d SP %d.%d", osVersionInfo.dwMajorVersion, osVersionInfo.dwMinorVersion, osVersionInfo.dwBuildNumber, osVersionInfo.wServicePackMajor, osVersionInfo.wServicePackMinor);
	Write(MF_INFO, L"Command line:%s", commandLine);
	Write(MF_INFO, L"WSE path: %s", _wsePath);

	CString origin;
	HANDLE warbandFile;
	
	if (warbandPath.GetLength() > 0)
	{
		if (TryOpenFile(warbandFile, warbandPath))
			origin = "command line";
	}
	else if (TryOpenFile(warbandFile, warbandPath = MakePath(_wsePath, "mb_warband.exe")))
	{
		origin = "path";
	}
	else if (TryOpenFile(warbandFile, warbandPath = MakePath(_wsePath, "mb_warband_dedicated.exe")))
	{
		origin = "path (dedicated)";
	}
#if defined DEBUG
#if defined WARBAND_VANILLA
	else if (TryOpenFile(warbandFile, warbandPath = "C:\\Program Files (x86)\\Mount&Blade Warband\\mb_warband.exe"))
#elif defined WARBAND_DEDICATED
	else if (TryOpenFile(warbandFile, warbandPath = "C:\\Program Files (x86)\\Mount&Blade Warband Dedicated\\mb_warband_dedicated.exe"))
#elif defined WARBAND_STEAM
	else if (TryOpenFile(warbandFile, warbandPath = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\mountblade warband\\mb_warband.exe"))
#endif
	{
		_wsePath = "D:\\Dev\\WSE\\WSEBuild\\";
		origin = "debug";
	}
#endif
	else if (TryOpenFile(warbandFile, warbandPath = MakePath(ReadRegistryKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mount&Blade Warband", ""), "mb_warband.exe")))
	{
		origin = "registry";
	}
	else if (TryOpenFile(warbandFile, warbandPath = MakePath(ReadRegistryKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Mount&Blade Warband", "Install_Path"), "mb_warband.exe")))
	{
		origin = "registry (Steam)";
	}

	if (warbandFile == INVALID_HANDLE_VALUE)
		Write(MF_ERROR | MF_QUIT, L"Could not find mb_warband.exe or mb_warband_dedicated.exe");
	else
		CloseHandle(warbandFile);
	
	Write(MF_INFO, L"Game executable origin: %s", origin);
	Write(MF_INFO, L"Game executable path: %s", warbandPath);
	
	DWORD crc32 = CRC32(warbandPath);

	Write(MF_INFO, L"Game executable checksum: %X", crc32);
	
	int type;
	CString dllName;
	bool unsupported = false;

	switch (crc32)
	{
	case CRC32_WARBAND_1153:
		type = 0;
		dllName = L"WSELib";
		break;
	case CRC32_WARBAND_STEAM_1153:
		type = 1;
		dllName = L"WSELibSteam";
		break;
	case CRC32_WARBAND_DEDICATED_1153:
		type = 2;
		dllName = L"WSELibDedicated";
		break;
	default:
		unsupported = true;
		break;
	}

	if (unsupported)
	{
		if (GetFileAttributes(_wsePath + L"mb_warband_old.exe") != INVALID_FILE_ATTRIBUTES)
		{
			Write(MF_INFO, L"Pulling switcheroo");
			warbandPath.Replace(L"mb_warband.exe", L"mb_warband_old.exe");
			CopyFile(_wsePath + L"mb_warband_old.exe", warbandPath, false);

			CString fmodPath = warbandPath;
			
			fmodPath.Replace(L"mb_warband_old.exe", L"fm_old.dll");
			CopyFile(_wsePath + L"fm_old.dll", fmodPath, false);
			type = 0;
			dllName = L"WSELib";
			unsupported = false;
		}
	}

	if (unsupported)
		Write(MF_ERROR | MF_QUIT, L"You are running an unsupported version of Warband (WSE only supports version 1.153).\nPlease update or download the full game from http://www.taleworlds.com/download.aspx.");

	int cpuInfo[4];

	__cpuid(cpuInfo, 1);

	if (!((cpuInfo[3] >> 26) & 0x1))
	{
		if (type == 2)
			Write(MF_ERROR | MF_QUIT, L"WSE for dedicated servers cannot be run without SSE2 support.");
		else
			dllName += "NoSSE";
	}

	if (type == 0 || type == 1)
	{
		if (moduleName != "")
			WriteRegistryKey(HKEY_CURRENT_USER, "Software\\MountAndBladeWarbandKeys", "last_module_warband", moduleName);
	}
	else if (type == 2)
	{

#if defined DEBUG
		commandLine = " -r Sample_Deathmatch.txt -m WSE";
#endif
	}

#if defined DEBUG
	dllName += "Debug";
#endif
	dllName += ".dll";
	dllPath = MakePath(_wsePath, dllName);

	Write(MF_INFO, L"Library path: %s", dllPath);

	if (GetFileAttributes(dllPath) == INVALID_FILE_ATTRIBUTES)
		Write(MF_ERROR | MF_QUIT, L"Library file not found");

	PROCESS_INFORMATION processInfo = { 0 };
	STARTUPINFO startupInfo = { sizeof(STARTUPINFO), 0 };

#if !defined WSELOADER_NOUI
	if (!EnableDebugPrivilege())
		Write(MF_INFO | MF_WINAPI, L"Failed to set SeDebugPrivilege, trying anyway");
#endif

	if (!CreateProcess(warbandPath, commandLine.GetBuffer(), NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo))
		Write(MF_ERROR | MF_WINAPI | MF_QUIT, L"Failed to create Warband process");

	if (type == 1)
	{
		std::list<DWORD> blacklist;

		BlacklistPid("mb_warband.exe");

		Write(MF_INFO, L"Blacklisted pid count: %d", blacklist.size());
		Write(MF_INFO, L"Injecting into dummy.");

		try
		{
			Inject(processInfo.hProcess, dllPath);
		}
		catch (char *)
		{
			Write(MF_INFO, L"Failed to inject into dummy.");
		}

		ResumeThread(processInfo.hThread);
		
		Write(MF_INFO, L"Waiting for dummy to end.");

		if (WaitForSingleObject(processInfo.hProcess, 5000) != WAIT_TIMEOUT)
		{
			int steamPid = -1;
			int warbandPid = -1;

			Write(MF_INFO, L"Waiting for Steam to start.");

			while ((steamPid = FindPid("steam.exe")) < 0)
			{
				Sleep(100);
			}
			
			Write(MF_INFO, L"Waiting for Steam to start Warband.");

			while ((warbandPid = FindChildPid("mb_warband.exe", steamPid)) < 0)
			{
				Sleep(100);
			}

			processInfo.hProcess = OpenProcess(PROCESS_ALL_ACCESS, true, warbandPid);
		}
		else
		{
			Write(MF_INFO, L"Dummy process not terminating, injecting directly.");
		}

		if (!processInfo.hProcess)
			Write(MF_ERROR | MF_WINAPI | MF_QUIT, L"Failed to open child process");
	}

	bool waitForProcess = false;
	wchar_t appidPath[MAX_PATH];
	
	wcscpy_s(appidPath, warbandPath.GetBuffer());
	PathRemoveFileSpec(appidPath);
	wcscat_s(appidPath, L"\\steam_appid.txt");

	AddLibrarySearchPath(processInfo.hProcess, _wsePath);

	try
	{
		Inject(processInfo.hProcess, dllPath);
	}
	catch (char *)
	{
		bool success = false;
		CString error = _errorText;

		if (type == 1)
		{
			TerminateProcess(processInfo.hProcess, 0);
			ZeroMemory(&processInfo, sizeof(processInfo));

			while (FindPid("Steam.exe") == -1)
			{
#if !defined WSELOADER_NOUI
				MessageBox(NULL, L"Steam must be running in order to start Warband.\nStart Steam manually before you proceed...", L"Steam not found", MB_OK|MB_ICONINFORMATION);
#endif
			}

			std::wofstream file(appidPath);

			file << 48700;
			file.close();
			
			if (!CreateProcess(warbandPath, commandLine.GetBuffer(), NULL, NULL, false, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInfo))
				Write(MF_ERROR | MF_WINAPI | MF_QUIT, L"Failed to create Warband process");

			type = 0;

			try
			{
				Inject(processInfo.hProcess, dllPath);
				success = true;
				waitForProcess = true;
			}
			catch (char *)
			{
			}
		}

		if (!success)
		{
			error.Append(L"\nIf you are running an Antivirus or Security Suite, try temporarily disabling it.");
			TerminateProcess(processInfo.hProcess, 0);
			Write(MF_ERROR | MF_QUIT, L"%s", error.GetBuffer());
		}
	}

	if (type != 1)
		ResumeThread(processInfo.hThread);
	
	if (!windowName.IsEmpty())
	{
		Sleep(1000);

		DWORD data[2];

		data[0] = processInfo.dwProcessId;
		data[1] = NULL;

		EnumWindows(EnumWindowsProc, (LPARAM)data);

		if (data[1])
			SetWindowText((HWND)data[1], windowName);
	}

#if defined WSELOADER_NOUI
	waitForProcess = true;
#endif

	if (waitForProcess)
	{
		__processHandle = processInfo.hProcess;
		SetConsoleCtrlHandler(HandlerRoutine, TRUE);
		WaitForSingleObject(processInfo.hProcess, INFINITE);
	}

	DeleteFile(appidPath);
	
	if (_consoleShown)
	{
		if (!_consoleAttached)
			system("pause");
		
		FreeConsole();
		_consoleShown = false;
		_consoleAttached = false;
	}
}

void Loader::ShowConsole()
{
	if (!_consoleShown)
	{
		if (!AttachConsole(ATTACH_PARENT_PROCESS))
		{
			_consoleAttached = false;
			AllocConsole();
		}
		else
		{
			_consoleAttached = true;
		}

		_consoleShown = true;
	}
}

bool Loader::EnableDebugPrivilege()
{
	HANDLE tokenHandle;
	LUID luid;
	TOKEN_PRIVILEGES tokenPrivileges;
	
	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &tokenHandle))
		return false;

	if(!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		return false;

	tokenPrivileges.PrivilegeCount = 1;
	tokenPrivileges.Privileges[0].Luid = luid;
	tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	if(!AdjustTokenPrivileges(tokenHandle, false, &tokenPrivileges, sizeof(tokenPrivileges), NULL, NULL))
		return false;

	if(GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		return false;

	CloseHandle(tokenHandle);
	return true;
}


void Loader::AddLibrarySearchPath(HANDLE procHandle, CString libPath)
{
	Write(MF_INFO, L"Setting library search path");

	HINSTANCE kernelLibAddr = GetModuleHandle(L"kernel32.dll");
	
	if (!kernelLibAddr)
		return;
	
	Write(MF_INFO, L"kernel32.lib local address: %X", kernelLibAddr);

	LPVOID funcAddr = GetProcAddress(kernelLibAddr, "SetDllDirectoryW");

	if (!funcAddr)
		return;
	
	Write(MF_INFO, L"SetDllDirectoryW local address: %X", funcAddr);
	
	SIZE_T pathSize = libPath.GetLength() * sizeof(wchar_t) + 1;
	LPVOID dllPathAddr = VirtualAllocEx(procHandle, NULL, pathSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

	if (!dllPathAddr)
		return;

	Write(MF_INFO, L"Library path: %s", libPath.GetBuffer());
	Write(MF_INFO, L"Library path size in bytes: %d", pathSize);
	Write(MF_INFO, L"Library path remote address: %X", dllPathAddr);
	
	if (!WriteProcessMemory(procHandle, dllPathAddr, libPath.GetBuffer(), pathSize, NULL))
		return;
	
	HANDLE threadHandle = CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)funcAddr, dllPathAddr, CREATE_SUSPENDED, NULL);

	if (!threadHandle)
		return;
	
	Write(MF_INFO, L"Remote thread handle: %X", threadHandle);

	if (ResumeThread(threadHandle) < 0)
		return;

	if (WaitForSingleObject(threadHandle, THREAD_WAIT_TIMEOUT) != WAIT_OBJECT_0)
		return;

	DWORD exitCode = 0;
	
	if (!GetExitCodeThread(threadHandle, &exitCode))
		return;

	Write(MF_INFO, L"Exit code: %X", exitCode);

	if (!VirtualFreeEx(procHandle, dllPathAddr, 0, MEM_RELEASE))
		return;
	
	Write(MF_INFO, L"Set library search path");
}

void Loader::Inject(HANDLE procHandle, CString libPath)
{
	Write(MF_INFO, L"Injecting");

	HINSTANCE kernelLibAddr = GetModuleHandle(L"kernel32.dll");
	
	if (!kernelLibAddr)
		SetError("Failed to retrieve kernel32 instance.");
	
	Write(MF_INFO, L"kernel32.lib local address: %X", kernelLibAddr);

	LPVOID loadLibraryAddr = GetProcAddress(kernelLibAddr, "LoadLibraryW");

	if (!loadLibraryAddr)
		SetError("Failed to retrieve remote load library address.");
	
	Write(MF_INFO, L"LoadLibraryW local address: %X", loadLibraryAddr);
	
	SIZE_T pathSize = libPath.GetLength() * sizeof(wchar_t) + 1;
	LPVOID dllPathAddr = VirtualAllocEx(procHandle, NULL, pathSize, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

	if (!dllPathAddr)
		SetError("Failed to allocate remote process memory for library string.");
	
	Write(MF_INFO, L"Library path: %s", libPath.GetBuffer());
	Write(MF_INFO, L"Library path size in bytes: %d", pathSize);
	Write(MF_INFO, L"Library path remote address: %X", dllPathAddr);
	
	if (!WriteProcessMemory(procHandle, dllPathAddr, libPath.GetBuffer(), pathSize, NULL))
		SetError("Failed to write remote process memory for library string.");
	
	HANDLE threadHandle = NULL;
	OSVERSIONINFO osvi;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	bool done = false;

	if (osvi.dwMajorVersion >= 6)
	{
		LPFUN_NtCreateThreadEx funNtCreateThreadEx = (LPFUN_NtCreateThreadEx)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");

		if (funNtCreateThreadEx)
		{
			DWORD status = funNtCreateThreadEx(&threadHandle, 0x1FFFFF, NULL, procHandle, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllPathAddr, TRUE, 0, 0, 0, NULL);

			if (threadHandle != NULL)
				done = true;
		}
	}

	if (!done)
		threadHandle = CreateRemoteThread(procHandle, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, dllPathAddr, CREATE_SUSPENDED, NULL);

	if (!threadHandle)
		SetError("Failed to create remote load library thread.");
	
	Write(MF_INFO, L"Remote thread handle: %X", threadHandle);

	if (ResumeThread(threadHandle) < 0)
		SetError("Failed to resume remote load library thread.");

	if (WaitForSingleObject(threadHandle, THREAD_WAIT_TIMEOUT) != WAIT_OBJECT_0)
		SetError("Failed to wait for remote load library thread to exit.");

	DWORD exitCode = 0;
	
	if (!GetExitCodeThread(threadHandle, &exitCode))
		SetError("Failed to retrieve remote load library thread exit code.");

	if (!exitCode)
		SetError("Failed to inject library into remote process.");
	
	Write(MF_INFO, L"Injected library address: %X", exitCode);

	if (!VirtualFreeEx(procHandle, dllPathAddr, 0, MEM_RELEASE))
		SetError("Failed to free remote process memory for library string.");
	
	Write(MF_INFO, L"Injected");
}

CString Loader::ReadRegistryKey(HKEY root, const CString &path, const CString &value)
{
	HKEY key;
	CString result;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		DWORD bufLen = MAX_PATH;

		RegQueryValueEx(key, value, NULL, NULL, (LPBYTE)result.GetBuffer(MAX_PATH), &bufLen);
		RegCloseKey(key);
		result.ReleaseBuffer();
	}

	return result;
}

void Loader::WriteRegistryKey(HKEY root, const CString &path, const CString &name, const CString &value)
{
	HKEY key;
	CString result;
	
	if (RegOpenKeyEx(root, path, 0, KEY_WRITE, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, name, 0, REG_SZ, (BYTE *)(LPCTSTR)value, (value.GetLength() + 1) * sizeof(wchar_t));
		RegCloseKey(key);
	}
}

bool Loader::TryOpenFile(HANDLE &handle, const CString &path)
{
	handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	return handle != INVALID_HANDLE_VALUE;
}

CString Loader::MakePath(const CString &path, const CString &file)
{
	CString fullPath = path;

	fullPath.Trim();

	if (fullPath.Right(1) != L"\\")
		fullPath.Append(L"\\");

	fullPath.Append(file);
	return fullPath;
}

void Loader::FindPids(const CString &name, std::list<DWORD> &pids)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32), 0 };
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if(Process32First(h, &pe))
	{
		do
		{
			if (!name.CompareNoCase(pe.szExeFile))
				pids.push_back(pe.th32ProcessID);
		}
		while(Process32Next(h, &pe));
	}
}

void Loader::BlacklistPid(const CString &name)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32), 0 };
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	_pidBlacklist.clear();
	
	if(Process32First(h, &pe))
	{
		do
		{
			if (!name.CompareNoCase(pe.szExeFile))
				_pidBlacklist.push_back(pe.th32ProcessID);
		}
		while(Process32Next(h, &pe));
	}
}

DWORD Loader::FindPid(const CString &name)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32), 0 };
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if(Process32First(h, &pe))
	{
		do
		{
			if (!name.CompareNoCase(pe.szExeFile))
				return pe.th32ProcessID;
		}
		while(Process32Next(h, &pe));
	}

	return -1;
}

DWORD Loader::FindChildPid(const CString &name, DWORD parentPid)
{
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32), 0 };
	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if(Process32First(h, &pe))
	{
		do
		{
			if (!name.CompareNoCase(pe.szExeFile) && pe.th32ParentProcessID == parentPid && std::find(_pidBlacklist.begin(), _pidBlacklist.end(), pe.th32ProcessID) == _pidBlacklist.end())
				return pe.th32ProcessID;
		}
		while(Process32Next(h, &pe));
	}

	return -1;
}

void Loader::Write(unsigned int flags, TCHAR *format, ...)
{
	DWORD error = GetLastError();
	CString text;
	va_list ap;

	va_start(ap, format);
	text.FormatV(format, ap);
	va_end(ap);

	if (flags & MF_WINAPI)
	{
		if (error != 0)
		{
			LPTSTR buffer;

			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, error, 0, (LPTSTR)&buffer, 0, NULL);
			text.AppendFormat(L"\n\nWindows API error\nCode: %d\nMessage: %s", error, buffer);
			LocalFree(buffer);
		}
	}
	
	if (flags & MF_ERROR && !_consoleShown)
	{
		ShowConsole();

		for (std::list<CString>::iterator it = _errorLog.begin(); it != _errorLog.end(); ++it)
		{
			WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), *it + "\n", it->GetLength() + 1, NULL, NULL);
		}
	}

	_errorLog.push_back(text);
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text + "\n", text.GetLength() + 1, NULL, NULL);

#if !defined WSELOADER_NOUI
	if (flags & MF_ERROR)
	{
		CString allText = "";

		for (std::list<CString>::iterator it = _errorLog.begin(); it != _errorLog.end(); ++it)
		{
			allText += *it + "\n";
		}
		
		size_t size = (allText.GetLength() + 1) * sizeof(wchar_t);
		HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, size);

		if (mem)
		{
			memcpy(GlobalLock(mem), allText.GetBuffer(), size);
			GlobalUnlock(mem);

			if (OpenClipboard(NULL))
			{
				EmptyClipboard();
				
				if (!SetClipboardData(CF_UNICODETEXT, mem))
					GlobalFree(mem);

				CloseClipboard();
			}
			else
			{
				GlobalFree(mem);
			}
		}

		MessageBox(NULL, text, L"Error", MB_OK | MB_ICONERROR);
	}
#endif

	if (flags & MF_QUIT)
	{
		std::wofstream stream(_wsePath + "\\wse_loader_log.txt", std::fstream::trunc);

		for (std::list<CString>::iterator it = _errorLog.begin(); it != _errorLog.end(); ++it)
		{
			stream << (*it).GetBuffer() << std::endl;
		}

		FreeConsole();
		exit((flags & MF_ERROR) ? EXIT_FAILURE : EXIT_SUCCESS);
	}
}

void Loader::SetError(char *text)
{
	_errorCode = GetLastError();
	_errorText[MultiByteToWideChar(CP_UTF8, 0, text, strlen(text), _errorText, 2048)] = '\0';
	throw "";
}
