#pragma once

#include "rgl_body_part.h"

namespace rgl
{
	struct quad : body_part
	{
		vector4 vertices[4];
		vector4 base_vertices[4];
		vector4 normal;
		vector4 edge_normals[4];
		int num_edges;
		int orientation;
	};
}
