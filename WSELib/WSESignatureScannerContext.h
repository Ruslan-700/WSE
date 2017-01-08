#pragma once

#include "WSEContext.h"
#include "warband.h"

class WSESignatureScannerContext : public WSEContext
{
protected:
	virtual void OnLoad();

public:
	DWORD FindSignature(const char* sig, const char* mask);

private:
	bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask);
	char * HexDumpStr(const char *src, size_t size);
	void FindRglAddresses();
	void FindWbAddresses();

private:
	struct module
	{
		DWORD dwBase, dwSize;
	};

	HANDLE TargetProcess;
	module mod;
};
