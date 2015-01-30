#pragma once

#include "rgl.h"

namespace wb
{
	struct post_effect
	{
		rgl::string id;
		unsigned int flags;
		int tonemap_operator_type;
		rgl::vector4 params_1;
		rgl::vector4 params_2;
		rgl::vector4 params_3;
	};
}
