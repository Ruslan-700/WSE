#pragma once

#include "rgl_quaternion.h"
#include "rgl_vector4.h"

namespace rgl
{
	struct position_ipo
	{
		vector4 *keys;
		int *time_points;
		int num_keys;
	};

	struct rotation_ipo
	{
		quaternion *keys;
		int *time_points;
		int num_keys;
	};
}
