#pragma once

#include "rgl.h"
#include "simple_trigger.h"

namespace wb
{
	struct map_icon
	{
		rgl::string id;
		unsigned int flags;
		rgl::string mesh_name;
		float scale;
		int sound_no;
		rgl::vector4 offset;
		simple_trigger_manager simple_triggers;
	};
}
