#pragma once

#include "rgl_body_part.h"

namespace rgl
{
	struct sphere : body_part
	{
		vector4 base_center;
		vector4 center;
		float radius;
	};
}
