#pragma once

#include "rgl_resource.h"
#include "rgl_body_part.h"

namespace rgl
{
	struct body : resource
	{
		body_part *body_part;
	};
}
