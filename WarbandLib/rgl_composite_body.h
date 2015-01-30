#pragma once

#include "rgl_body_part.h"

namespace rgl
{
	struct composite_body : body_part
	{
		body_part **parts;
		int num_parts;
	};
}
