#pragma once

#include "rgl.h"

namespace wb
{
	struct bone
	{
		rgl::string name;
		float bone_mass;
		float bone_cone_min;
		float bone_cone_max;
		rgl::vector4 rot;
		rgl::vector4 socket_dir;
		rgl::vector4 plane_dir;
		float bone_plane_min;
		float bone_plane_max;
		float bone_twist_limit_min;
		float bone_twist_limit_max;
		stl::vector<rgl::skeleton_model_body> bodies;
		stl::vector<rgl::skeleton_model_body *> ragdoll_bodies;
		stl::vector<rgl::skeleton_model_body *> collision_bodies;
	};

	struct skeleton
	{
		rgl::string name;
		stl::vector<bone> bones;
	};

	struct skeleton_manager
	{
		stl::vector<skeleton> skeletons;
	};
}
