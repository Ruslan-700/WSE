#pragma once

#include "rgl.h"
#include "simple_trigger.h"

namespace wb
{
	struct scene_prop
	{
		rgl::string id;
		unsigned __int64 flags;
		int hit_points;
		float u1[10];
#if defined WARBAND
		rgl::meta_mesh *meta_mesh;
#endif
		rgl::body *body_object;
		rgl::body_part *body_part;
		rgl::vector4 b_center;
		float b_diameter;
		simple_trigger_manager simple_triggers;
	};
}
