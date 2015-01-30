#pragma once

#include "rgl_body_part.h"
#include "stl_vector.h"

namespace rgl
{
	struct manifold_body_face
	{
		int orientation;
		unsigned int flags;
		int num_vertices;
		int vertex_nos[4];
		int scene_manifold_face_no;
	};

	struct manifold_body : body_part
	{
		stl::vector<vector4> vertices;
		stl::vector<vector4> base_vertices;
		stl::vector<manifold_body_face> faces;
	};
}
