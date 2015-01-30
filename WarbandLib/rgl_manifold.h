#pragma once

#include "rgl_quad.h"
#include "rgl_vector4.h"
#include "stl_vector.h"

namespace rgl
{
	struct manifold_vertex
	{
		vector4 position;
		vector4 normal;
		float sun_light_amount;
	};

	struct manifold_face
	{
		int num_vertices;
		int vertex_nos[4];
		int edge_nos[4];
		int terrain_type;
		int neighbor_face_nos[4];
		float neighbor_distances[4];
		int group_no;
		int adjoining_group_1_no;
		int adjoining_group_2_no;
		vector4 normal;
		float u1;
		unsigned int flags;
		int path_color;
		int path_next_face_no;
		vector4 center;
		quad *quad;
	};

	struct manifold_edge
	{
		int num_faces;
		int face_nos[2];
		int vertex_nos[2];
		vector4 center;
	};

	struct manifold_path_point
	{
		int face_no;
		float u1;
		float u2;
	};

	struct manifold
	{
		stl::vector<manifold_vertex> vertices;
		stl::vector<manifold_face> faces;
		stl::vector<manifold_edge> edges;
		stl::vector<manifold_path_point> path;
		int u1;
		vector4 min_position;
		vector4 max_position;
		int white_point;
	};
}
