#pragma once

#include "rgl.h"

namespace wb
{
	struct flora_kind_instance
	{
		rgl::string id;
#if defined WARBAND
		rgl::meta_mesh *meta_mesh;
#endif
		rgl::body *body_object;
		rgl::body_part *body_part;
#if defined WARBAND
		rgl::vector4 b_min;
		rgl::vector4 b_max;
#endif
	};

	struct flora_kind
	{
		rgl::string id;
		unsigned __int64 flags;
		int num_instances;
		float colony_radius;
		float colony_threshold;
		flora_kind_instance *instances;
	};
}
