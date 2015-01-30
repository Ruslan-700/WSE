#pragma once

#include "rgl.h"

namespace wb
{
	struct core_game
	{
		double frame_time;
		int pressed_key;
		int num_module_names;
		rgl::string module_names[32];
	};
}
