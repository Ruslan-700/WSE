#pragma once

#include "rgl_io_binary_stream.h"

namespace rgl
{
	struct io_input_binary_file_stream : io_binary_stream
	{
		FILE *file;
		bool u10;
	};
}
