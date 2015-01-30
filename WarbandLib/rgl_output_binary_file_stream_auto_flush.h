#pragma once

#include "rgl_output_binary_file_stream.h"

namespace rgl
{
	struct output_binary_file_stream_auto_flush : io_output_binary_file_stream
	{
		int u10;
		int u11;
		int u12;
	};
}
