#include "script.h"

#include "warband.h"

using namespace wb;

void script::error(const char *message)
{
	char buf[1024];

	sprintf_s(buf, "%s; LINE NO: %d:", message, warband->cur_statement_no);
#if defined WARBAND
	warband->window_manager.display_message(buf, 0xFFFF5555, 0);
#endif
	warband->log_stream.write_c_str(buf);
	warband->log_stream.write_c_str("\n");
	warband->script_error_occurred = 1;
}

bool script::execute(const int &num_params, __int64 *params)
{
	return this->operations.execute(0x200000, 0, num_params, params);
}
