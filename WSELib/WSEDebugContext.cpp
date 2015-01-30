#include "WSEDebugContext.h"

#include "WSE.h"
#include <fstream>

void __stdcall DebugFunc()
{
}

void __declspec(naked) DebugHook()
{
	_asm
	{
	}
}

WSEDebugContext::WSEDebugContext()
{
}

void WSEDebugContext::OnLoad()
{
}
