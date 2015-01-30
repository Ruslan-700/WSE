#pragma once

#include "rgl.h"

namespace wb
{
	struct mesh
	{
		rgl::string name;
#if defined WARBAND
		rgl::mesh *resource;
#endif
		unsigned int flags;
		rgl::string id;
		float translation_x;
		float translation_y;
		float translation_z;
		float rotation_x;
		float rotation_y;
		float rotation_z;
		float scale_x;
		float scale_y;
		float scale_z;
	};

	struct mesh_manager
	{
		mesh *meshes;
		int num_meshes;
	};
}
