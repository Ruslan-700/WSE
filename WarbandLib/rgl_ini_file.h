#pragma once

#include "rgl_string.h"

namespace rgl
{
	struct ini_pair
	{
		string key;
		string value;
	};

	struct ini_file
	{
		ini_pair pairs[4096];
		int num_pairs;
	};
}
