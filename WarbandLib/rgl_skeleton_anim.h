#pragma once

#include "rgl_ipo.h"
#include "rgl_resource.h"

namespace rgl
{
	struct skeleton_anim_bone
	{
		rotation_ipo rotation_ipo;
		position_ipo position_ipo;
		int has_position_ipo;
	};

	struct skeleton_anim : resource
	{
		stl::vector<skeleton_anim_bone> bones;
		unsigned int flags;
		position_ipo position_ipo;
	};

	struct skeleton_anim_spec
	{
		float duration;
		skeleton_anim *skeleton_anim;
		int beginning_frame;
		int ending_frame;
	};
}
