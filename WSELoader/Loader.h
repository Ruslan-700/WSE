#pragma once

#include <atlstr.h>
#include <Windows.h>
#include <list>

#define MF_INFO 0x1
#define MF_ERROR 0x2
#define MF_WINAPI 0x4
#define MF_QUIT 0x8

#define CRC32_WARBAND_1153 0x7F079D05
#define CRC32_WARBAND_STEAM_1153 0x61923192
#define CRC32_WARBAND_DEDICATED_1153 0x7F7F3C4F

#define THREAD_WAIT_TIMEOUT 10000

typedef DWORD (WINAPI *LPFUN_NtCreateThreadEx)
(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN LPVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN LPTHREAD_START_ROUTINE lpStartAddress,
	IN LPVOID lpParameter,
	IN BOOL CreateSuspended,
	IN ULONG StackZeroBits,
	IN ULONG SizeOfStackCommit,
	IN ULONG SizeOfStackReserve,
	OUT LPVOID lpBytesBuffer
);

class Loader
{
public:
	Loader();
	void Process(const CString &cmdLine);

private:
	void ShowConsole();
	bool EnableDebugPrivilege();
	void AddLibrarySearchPath(HANDLE procHandle, CString libPath);
	void Inject(HANDLE procHandle, CString libPath);
	CString ReadRegistryKey(HKEY root, const CString &path, const CString &value);
	void WriteRegistryKey(HKEY root, const CString &path, const CString &name, const CString &value);
	bool TryOpenFile(HANDLE &handle, const CString &path);
	CString MakePath(const CString &path, const CString &file);
	void BlacklistPid(const CString &name);
	void FindPids(const CString &name, std::list<DWORD> &pids);
	DWORD FindPid(const CString &name);
	DWORD FindChildPid(const CString &name, DWORD parentPid);
	void Write(unsigned int flags, TCHAR *format, ...);
	void SetError(char *text);

private:
	std::list<DWORD> _pidBlacklist;
	std::list<CString> _errorLog;
	CString _loaderFile;
	CString _wsePath;
	DWORD _errorCode;
	WCHAR _errorText[2048];
	bool _consoleShown;
	bool _consoleAttached;
};
