#pragma once

#include "rgl.h"

namespace wb
{
	enum timer_command
	{
		TIMER_RESET           = 0,
		TIMER_START           = 1,
		TIMER_STOP            = 2,
		TIMER_ADVANCE         = 3,
		TIMER_GETABSOLUTETIME = 4,
		TIMER_GETAPPTIME      = 5,
		TIMER_GETELAPSEDTIME  = 6,
	};
	
	typedef double (__stdcall *DXUtil_Timer_func)(timer_command command);
	
	struct functions
	{
		static DXUtil_Timer_func DXUtil_Timer;
	};
}
