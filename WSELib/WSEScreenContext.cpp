#include "WSEScreenContext.h"

#include <Windows.h>
#include "WSE.h"

void WSEScreenContext::Message(unsigned int color, std::string format, ...)
{
	char buffer[1024];
	va_list ap;
	
	va_start(ap, format);

	DWORD chars_written = vsprintf_s(buffer, 1023, format.c_str(), ap);

	va_end(ap);

#if defined WARBAND
	warband->window_manager.display_message(buffer, color, false);
#elif defined WARBAND_DEDICATED
	buffer[chars_written++] = '\n';
	buffer[chars_written] = '0';

	DWORD dummy;

	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buffer, chars_written, &dummy, nullptr);
#endif
}
