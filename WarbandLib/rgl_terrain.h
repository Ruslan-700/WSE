#pragma once

#include "rgl_vector4.h"

namespace rgl
{
	struct terrain_cell
	{
		float altitude;
		unsigned int flags;
		int scene_manifold_face_no_1;
		int scene_manifold_face_no_2;
	};
	
	struct terrain
	{
		terrain_cell **cells;
		vector4 offset;
		int num_cells[2];
		float region_detail;
	};
}
