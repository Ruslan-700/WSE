#pragma once

#include "rgl_vector4.h"

namespace rgl
{
	struct bounding_box
	{
		vector4 min;
		vector4 max;
		vector4 center;
		float diameter;

		bounding_box();
	};
}
